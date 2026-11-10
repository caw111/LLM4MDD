from fastapi import FastAPI, HTTPException
from fastapi.responses import Response
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel, Field
from dotenv import load_dotenv
from openai import OpenAI
import os
import io
import json
import re
import copy
import subprocess
import shutil
import tempfile
import hashlib
from xml.etree import ElementTree as ET
from pathlib import Path
from app.services.arduino_adapter import generate_adapter, adapter_issues
from app.services.arduino_quality import validate_arduino_static_quality, validate_param_literal_value
from app.services.sysml import sysml_bundle_prompt, generate_validated_bundle, validate_sysml, SysMLToolError, SysMLValidationError
from app.services.ir_contract import validate_ir
from app.services.ir_prompt import IR_V2_PROMPT
from app.services.interface_planner import plan_interface_contract, contract_digest, ContractPlanningError, check_explicit_requirements, add_public_behavior_checks
from app.services.hardware_adapter import validate_hardware_contract
from app.services.behavior_contract import validate_sysml_contract
from app.services.sysml_compiler import compile_sysml, ir_to_il, SysMLCompileError, validate_executable_sysml
from app.services.artifact_provenance import AUTHORITY, ProvenanceError, issue_model, text_digest
from app.services.llm_json import json_prompt, request_options, completion_text, check_completion

load_dotenv(override=True)

app = FastAPI(title="LLM4MDD API")

_cors = os.getenv("CORS_ALLOW_ORIGINS", "*")
_allow_all = _cors.strip() == "*" or _cors.strip().lower() == "all"
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"] if _allow_all else [o.strip() for o in _cors.split(",") if o.strip()],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# 配置OpenAI客户端，设置更长的超时时间（默认60秒太短）
_llm_timeout = int(os.getenv("LLM_API_TIMEOUT", "300"))  # 默认5分钟超时
_llm_base_url = os.getenv("OPENAI_BASE_URL", "https://openrouter.ai/api/v1")
_llm_api_key = os.getenv("OPENROUTER_API_KEY") or os.getenv("OPENAI_API_KEY")
_default_model = "openai/gpt-5.4-nano"
# 关闭继承代理，避免中间层剥离 Authorization 头；如需代理请显式开启 LLM_USE_PROXY=true
if os.getenv("LLM_USE_PROXY", "false").strip().lower() in {"1", "true", "yes", "on"}:
    _http_proxy = os.getenv("LLM_HTTP_PROXY") or os.getenv("HTTP_PROXY") or os.getenv("http_proxy")
    _https_proxy = os.getenv("LLM_HTTPS_PROXY") or os.getenv("HTTPS_PROXY") or os.getenv("https_proxy")
    if _http_proxy:
        os.environ["HTTP_PROXY"] = _http_proxy
    if _https_proxy:
        os.environ["HTTPS_PROXY"] = _https_proxy
else:
    os.environ.pop("HTTP_PROXY", None)
    os.environ.pop("HTTPS_PROXY", None)
    os.environ.pop("http_proxy", None)
    os.environ.pop("https_proxy", None)
client = OpenAI(
    base_url=_llm_base_url,
    api_key=_llm_api_key or 'not-configured',  # Offline compiler routes do not require an API key.
    timeout=_llm_timeout,
)

OUTPUT_DIR = os.getenv("OUTPUT_DIR", "outputs")
OUTPUT_FILENAME = os.getenv("OUTPUT_FILENAME", "last_response.txt")
OUTPUT_PATH = os.path.join(OUTPUT_DIR, OUTPUT_FILENAME)

_frontend_dist = Path(os.getenv("FRONTEND_DIST", "frontend/dist"))
if _frontend_dist.exists() and _frontend_dist.is_dir():
    app.mount("/ui", StaticFiles(directory=str(_frontend_dist), html=True), name="ui")


class ChatRequest(BaseModel):
    prompt: str = Field(..., min_length=1)
    model: str | None = None
    temperature: float | None = None
    max_tokens: int | None = None


class ChatResponse(BaseModel):
    text: str
    model: str


class ConversationMessage(BaseModel):
    role: str
    content: str = Field(..., min_length=1)


class DecideModeRequest(BaseModel):
    instruction: str = Field(..., min_length=1)
    has_complete_system: bool = False
    current_system: dict | None = None
    history: list[ConversationMessage] | None = None
    model: str | None = None
    temperature: float | None = None
    max_tokens: int | None = None


class DecideModeResponse(BaseModel):
    mode: str
    reason: str


class ConvertIrToTbLinkRequest(BaseModel):
    ir: dict


class GenerateFromPromptRequest(BaseModel):
    prompt: str | None = None
    prompt_file: str | None = None
    history: list[ConversationMessage] | None = None
    model: str | None = None
    temperature: float | None = None
    max_tokens: int | None = None


class GenerateSysml2BundleRequest(GenerateFromPromptRequest):
    interface_contract: dict | None = None
    max_repair_attempts: int = Field(default=2, ge=0, le=5)
    semantic_review_enabled: bool = False
    semantic_review_model: str | None = None
    max_semantic_review_calls: int = Field(default=3, ge=1, le=6)
    out_sysml2_path: str = Field(default="outputs/system.sysml2")
    out_req_test_map_path: str = Field(default="outputs/req_test_map.json")


class PlanInterfaceContractRequest(GenerateFromPromptRequest):
    requirement: str = Field(..., min_length=1)
    board: str | None = None
    pin_definitions: dict[str, str] = Field(default_factory=dict)
    previous_contract: dict | None = None
    interface_contract: dict | None = None
    max_repair_attempts: int = Field(default=2, ge=0, le=5)
    out_path: str = "outputs/interface_contract.json"


class CompileSysmlRequest(BaseModel):
    interface_contract: dict | None = None
    system_sysml2: str = Field(..., min_length=1)
    root: str | None = None
    out_dir: str = Field(default="outputs/sysml_compiled")


class GenerateSubsystemIrRequest(GenerateFromPromptRequest):
    interface_contract: dict | None = None
    step1_sysml2: str | None = None
    out_ir_path: str = Field(default="outputs/subsystem_ir.json")


class GenerateCFromStep1Request(GenerateFromPromptRequest):
    interface_contract: dict | None = None
    step1_sysml2: str | None = None
    step1_sysml2_path: str = Field(default="outputs/system.sysml2")
    step1_req_test_map: dict | None = None
    step1_req_test_map_path: str = Field(default="outputs/req_test_map.json")
    out_dir: str = Field(default="outputs/c_output")
    out_header_path: str | None = None
    out_source_path: str | None = None


class GenerateResponse(BaseModel):
    model: str
    files: dict
    contents: dict | None = None


class ConvertCToIlRequest(BaseModel):
    c_code: str | None = None
    c_file_path: str = Field(default="outputs/c_output/generated_model.c")
    out_il_path: str = Field(default="outputs/c_output/generated_model.il.xml")
    code2script_exe: str | None = None
    code2script_root: str | None = None
    model_path: str | None = None
    out_script_path: str | None = None


class ConvertCToIlResponse(BaseModel):
    out_il_path: str
    il_xml: str
    command: str
    returncode: int
    stdout: str
    stderr: str


class ConvertIlToCRequest(BaseModel):
    provenance: dict | None = None
    interface_contract: dict | None = None
    il_file_path: str = Field(default="outputs/c_output/generated_model.il.xml")
    out_c_dir: str = Field(default="outputs/c_output_codegen")
    codegen_root: str | None = None
    target: str = Field(default="Intel")
    optimize: bool = False
    out_il_optimized_path: str | None = None


class CodegenStepResult(BaseModel):
    name: str
    command: str
    returncode: int
    stdout: str
    stderr: str


class RunCodegenPipelineRequest(BaseModel):
    tblink_xml: str | None = None
    out_tblink_path: str = Field(default="outputs/model.tblink.xml")
    out_mir_path: str | None = None
    out_il_path: str | None = None
    out_il_optimized_path: str | None = None
    out_c_dir: str = Field(default="outputs/c_output")
    codegen_root: str | None = None
    target: str = Field(default="Intel")
    optimize: bool = False


class RunCodegenPipelineResponse(BaseModel):
    files: dict
    steps: list[CodegenStepResult]


class ConvertIlToCResponse(BaseModel):
    provenance: dict | None = None
    provenance_status: str = 'unverified_legacy'
    contract_sha256: str | None = None
    files: dict
    steps: list[CodegenStepResult]
    contents: dict | None = None


class ValidationIssue(BaseModel):
    code: str
    message: str
    path: str | None = None
    severity: str = "error"


class ValidateSubsystemIrRequest(BaseModel):
    ir: dict
    interface_contract: dict | None = None
    step1_sysml2: str | None = None
    step1_req_test_map: dict | None = None


class ValidateSubsystemIrResponse(BaseModel):
    ok: bool
    feedback_target: str
    issues: list[ValidationIssue]
    feedback: list[str]


class GenerateSubsystemIrIterativeRequest(GenerateSubsystemIrRequest):
    step1_req_test_map: dict | None = None
    max_iters: int = Field(default=3, ge=1, le=6)


class GenerateSubsystemIrIterativeResponse(BaseModel):
    ok: bool
    feedback_target: str
    model: str
    iteration: int
    files: dict
    contents: dict | None = None
    issues: list[ValidationIssue]
    feedback: list[str]


class CompiledKernel(BaseModel):
    header: str = Field(..., min_length=1)
    source: str = Field(..., min_length=1)


class ConvertCToArduinoRequest(BaseModel):
    provenance: dict | None = None
    allow_legacy_adapter: bool = False
    interface_contract: dict | None = None
    compiled_kernel: CompiledKernel | None = None
    c_code: str = Field(..., min_length=1)
    user_requirements: str | None = None
    pin_definitions: dict[str, str] | None = None
    module_specs: list[dict] | None = None
    history: list[ConversationMessage] | None = None
    model: str | None = None
    temperature: float | None = None
    out_path: str | None = None  # 可选的输出文件路径


class ConvertCToArduinoResponse(BaseModel):
    provenance: dict | None = None
    provenance_status: str = 'unverified_legacy'
    contract_sha256: str | None = None
    arduino_code: str
    model: str
    module_code_map: dict[str, str] | None = None
    params: list[dict] | None = None
    saved_path: str | None = None  # 保存的文件路径


class ArduinoCompileRequest(BaseModel):
    provenance: dict | None = None
    sketch_path: str = Field(default="outputs/arduino_output.ino", min_length=1)
    fqbn: str | None = Field(default=None, min_length=1)


class ArduinoCompileResponse(BaseModel):
    provenance_status: str = 'unverified_legacy'
    arduino_sha256: str | None = None
    success: bool
    fqbn: str
    sketch_path: str
    compile_stdout: str = ""
    compile_stderr: str = ""


class ArduinoUpdateModuleCodeRequest(BaseModel):
    module_id: str = Field(..., min_length=1)
    code: str = Field(..., min_length=1)
    sketch_path: str = Field(default="outputs/arduino_output.ino", min_length=1)


class ArduinoUpdateModuleCodeResponse(BaseModel):
    success: bool
    module_id: str
    saved_path: str
    arduino_code: str
    module_code_map: dict[str, str]


class ArduinoListParamsRequest(BaseModel):
    sketch_path: str = Field(default="outputs/arduino_output.ino", min_length=1)


class ArduinoListParamsResponse(BaseModel):
    sketch_path: str
    params: list[dict]


class ArduinoParamUpdateItem(BaseModel):
    param_id: str = Field(..., min_length=1)
    value: str = Field(..., min_length=1)


class ArduinoUpdateParamsRequest(BaseModel):
    sketch_path: str = Field(default="outputs/arduino_output.ino", min_length=1)
    updates: list[ArduinoParamUpdateItem] = Field(default_factory=list)


class ArduinoUpdateParamsResponse(BaseModel):
    success: bool
    saved_path: str
    arduino_code: str
    params: list[dict]


class ArduinoUploadRequest(BaseModel):
    sketch_path: str = Field(default="outputs/arduino_output.ino", min_length=1)
    fqbn: str = Field(default="arduino:avr:uno", min_length=1)
    port: str = Field(..., min_length=1)


class ArduinoUploadResponse(BaseModel):
    success: bool
    message: str
    fqbn: str
    port: str
    sketch_path: str
    compile_success: bool = False
    upload_success: bool = False
    compile_command: str = ""
    upload_command: str = ""
    compile_stdout: str = ""
    compile_stderr: str = ""
    upload_stdout: str = ""
    upload_stderr: str = ""


class ArduinoDevicePort(BaseModel):
    address: str
    label: str
    protocol: str | None = None
    board_name: str | None = None
    fqbn: str | None = None


class ArduinoDeviceBoard(BaseModel):
    label: str
    fqbn: str


class ArduinoDevicesResponse(BaseModel):
    ports: list[ArduinoDevicePort]
    boards: list[ArduinoDeviceBoard]


class ArduinoCodeResponse(BaseModel):
    sketch_path: str
    code: str


class ArduinoSerialReadRequest(BaseModel):
    port: str = Field(..., min_length=1)
    baud_rate: int = Field(default=115200, ge=300, le=2000000)
    read_timeout_ms: int = Field(default=800, ge=50, le=10000)
    max_chars: int = Field(default=6000, ge=1, le=50000)


class ArduinoSerialReadResponse(BaseModel):
    port: str
    baud_rate: int
    read_timeout_ms: int
    text: str = ""
    chars_read: int = 0


class DiagnosticEvent(BaseModel):
    line_no: int
    raw: str
    timestamp: str | None = None
    level: str = "INFO"
    code: str = "UNSPECIFIED"
    module: str = "general"
    message: str = ""
    context: dict[str, str] = Field(default_factory=dict)


class DiagnosticSummaryItem(BaseModel):
    code: str
    level: str
    module: str
    count: int
    latest_line_no: int


class DiagnosticParseRequest(BaseModel):
    raw_log: str = Field(..., min_length=1)
    source: str | None = None
    max_events: int = Field(default=200, ge=1, le=2000)


class DiagnosticParseResponse(BaseModel):
    source: str | None = None
    total_lines: int
    parsed_events: int
    dropped_lines: int
    events: list[DiagnosticEvent]
    summary: list[DiagnosticSummaryItem]


class DiagnosticSummarizeRequest(BaseModel):
    events: list[DiagnosticEvent] = Field(default_factory=list)


class DiagnosticSummarizeResponse(BaseModel):
    summary: list[DiagnosticSummaryItem]
    top_level: str


class DiagnosticSuggestion(BaseModel):
    title: str
    rationale: str
    patch_hint: str
    risk: str = "low"


class DiagnosticSuggestFixRequest(BaseModel):
    events: list[DiagnosticEvent] = Field(default_factory=list)
    requirement: str | None = None
    current_arduino_code: str | None = None
    model: str | None = None


class DiagnosticSuggestFixResponse(BaseModel):
    headline: str
    probable_root_causes: list[str]
    suggestions: list[DiagnosticSuggestion]
    recommended_prompt: str
    llm_used: bool = False


class GenerateModuleBlueprintRequest(GenerateFromPromptRequest):
    requirement: str = Field(..., min_length=1)
    existing_blueprint: dict | None = None
    max_modules: int = Field(default=12, ge=1, le=30)


class GenerateModuleBlueprintResponse(BaseModel):
    model: str
    blueprint: dict


class SystemStateRequest(BaseModel):
    session_id: str = Field(default="default", min_length=1)


class SystemPatchRequest(BaseModel):
    session_id: str = Field(default="default", min_length=1)
    instruction: str = Field(..., min_length=1)
    current_system: dict | None = None
    history: list[ConversationMessage] | None = None
    model: str | None = None
    temperature: float | None = None
    max_tokens: int | None = None


class SystemStateSaveRequest(BaseModel):
    session_id: str = Field(default="default", min_length=1)
    system: dict = Field(default_factory=dict)


class SystemPatchResponse(BaseModel):
    session_id: str
    version: int
    system: dict
    ops: list[dict]
    assistant: str


SYSTEM_STATE_STORE_PATH = os.getenv("SYSTEM_STATE_STORE_PATH", "outputs/system_state_store.json")
_system_state_store: dict[str, dict] = {}


def _empty_system() -> dict:
    return {"modules": [], "connections": []}


def _sanitize_system(system: dict | None) -> dict:
    if not isinstance(system, dict):
        return _empty_system()

    modules_raw = system.get("modules") if isinstance(system.get("modules"), list) else []
    connections_raw = system.get("connections") if isinstance(system.get("connections"), list) else []

    modules: list[dict] = []
    seen_ids: set[str] = set()
    for idx, item in enumerate(modules_raw):
        if not isinstance(item, dict):
            continue
        raw_id = str(item.get("id") or "").strip()
        module_id = raw_id or f"module_{idx + 1}"
        if module_id in seen_ids:
            continue
        seen_ids.add(module_id)
        modules.append(
            {
                "id": module_id,
                "title": str(item.get("title") or module_id).strip() or module_id,
                "subtitle": str(item.get("subtitle") or "由需求自动生成的模块").strip() or "由需求自动生成的模块",
                "moduleType": str(item.get("moduleType") or "custom").strip() or "custom",
                "executor": str(item.get("executor") or "generic").strip() or "generic",
                "codeSeed": str(item.get("codeSeed") or f"# {module_id}\n// TODO: implement").strip(),
                "params": item.get("params") if isinstance(item.get("params"), dict) else {},
            }
        )

    connections: list[dict] = []
    for item in connections_raw:
        if not isinstance(item, dict):
            continue
        from_id = str(item.get("from") or "").strip()
        to_id = str(item.get("to") or "").strip()
        if not from_id or not to_id or from_id == to_id:
            continue
        if from_id not in seen_ids or to_id not in seen_ids:
            continue
        connections.append(
            {
                "from": from_id,
                "to": to_id,
                "relation": str(item.get("relation") or "data_flow").strip() or "data_flow",
                "logic": str(item.get("logic") or "").strip(),
            }
        )


    if not connections and len(modules) > 1:
        for i in range(len(modules) - 1):
            connections.append(
                {
                    "from": modules[i]["id"],
                    "to": modules[i + 1]["id"],
                    "relation": "data_flow",
                    "logic": "上一模块输出作为下一模块输入",
                }
            )

    cleaned = {"modules": modules, "connections": connections}
    if isinstance(system.get("system_sysml2"), str):
        cleaned["system_sysml2"] = system["system_sysml2"]
    if isinstance(system.get("req_test_map"), dict):
        cleaned["req_test_map"] = copy.deepcopy(system["req_test_map"])
    return cleaned


def _load_system_state_store() -> None:
    global _system_state_store
    path = _resolve_path(SYSTEM_STATE_STORE_PATH)
    if not path.exists() or not path.is_file():
        _system_state_store = {}
        return

    try:
        data = json.loads(path.read_text(encoding="utf-8"))
        if isinstance(data, dict):
            cleaned: dict[str, dict] = {}
            for key, value in data.items():
                if not isinstance(value, dict):
                    continue
                version = int(value.get("version") or 0)
                system = _sanitize_system(value.get("system") if isinstance(value.get("system"), dict) else {})
                cleaned[str(key)] = {"version": max(0, version), "system": system}
            _system_state_store = cleaned
        else:
            _system_state_store = {}
    except Exception:
        _system_state_store = {}


def _save_system_state_store() -> None:
    path = _resolve_path(SYSTEM_STATE_STORE_PATH)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(_system_state_store, ensure_ascii=False, indent=2), encoding="utf-8")


def _get_or_init_state(session_id: str, current_system: dict | None = None) -> dict:
    normalized_session = (session_id or "default").strip() or "default"
    existing = _system_state_store.get(normalized_session)
    if existing:
        return existing

    seed_system = _sanitize_system(current_system if isinstance(current_system, dict) else {})
    state = {"version": 1, "system": seed_system}
    _system_state_store[normalized_session] = state
    _save_system_state_store()
    return state


def _find_module(system: dict, module_id: str) -> dict | None:
    for item in system.get("modules", []):
        if isinstance(item, dict) and str(item.get("id") or "") == module_id:
            return item
    return None


def _apply_ops_to_system(system: dict, ops: list[dict]) -> tuple[dict, list[str]]:
    next_system = _sanitize_system(copy.deepcopy(system))
    notes: list[str] = []

    for raw in ops:
        if not isinstance(raw, dict):
            continue
        op_name = str(raw.get("op") or "").strip().lower()
        if not op_name:
            continue

        if op_name == "add_module":
            module = raw.get("module") if isinstance(raw.get("module"), dict) else {}
            module_id = str(module.get("id") or "").strip() or str(raw.get("target") or "").strip()
            if not module_id:
                notes.append("add_module skipped: missing module id")
                continue
            if _find_module(next_system, module_id):
                notes.append(f"add_module skipped: {module_id} already exists")
                continue
            default_module = {
                "id": module_id,
                "title": str(module.get("title") or module_id).strip() or module_id,
                "subtitle": str(module.get("subtitle") or "由AI增量补丁新增").strip() or "由AI增量补丁新增",
                "moduleType": str(module.get("moduleType") or "custom").strip() or "custom",
                "executor": str(module.get("executor") or "generic").strip() or "generic",
                "codeSeed": str(module.get("codeSeed") or f"# {module_id}\n// TODO: implement").strip(),
                "params": module.get("params") if isinstance(module.get("params"), dict) else {},
            }
            next_system["modules"].append(default_module)
            notes.append(f"added module {module_id}")
            continue

        if op_name == "remove_module":
            module_id = str(raw.get("target") or raw.get("id") or "").strip()
            if not module_id:
                notes.append("remove_module skipped: missing target")
                continue
            before = len(next_system["modules"])
            next_system["modules"] = [m for m in next_system["modules"] if str(m.get("id") or "") != module_id]
            next_system["connections"] = [
                c
                for c in next_system["connections"]
                if str(c.get("from") or "") != module_id and str(c.get("to") or "") != module_id
            ]
            if len(next_system["modules"]) < before:
                notes.append(f"removed module {module_id}")
            else:
                notes.append(f"remove_module skipped: {module_id} not found")
            continue

        if op_name in {"update_module", "update_param"}:
            module_id = str(raw.get("target") or raw.get("id") or "").strip()
            module = _find_module(next_system, module_id)
            if not module:
                notes.append(f"{op_name} skipped: {module_id} not found")
                continue

            if op_name == "update_param":
                key = str(raw.get("key") or "").strip()
                if not key:
                    notes.append("update_param skipped: missing key")
                    continue
                params = module.get("params") if isinstance(module.get("params"), dict) else {}
                params[key] = raw.get("value")
                module["params"] = params
                notes.append(f"updated param {module_id}.{key}")
                continue

            fields = raw.get("fields") if isinstance(raw.get("fields"), dict) else {}
            if not fields:
                key = str(raw.get("key") or "").strip()
                if key:
                    fields = {key: raw.get("value")}
            for field_name in ["title", "subtitle", "moduleType", "executor", "codeSeed"]:
                if field_name in fields and fields[field_name] is not None:
                    module[field_name] = str(fields[field_name])
            notes.append(f"updated module {module_id}")
            continue

        if op_name == "connect":
            from_id = str(raw.get("from") or "").strip()
            to_id = str(raw.get("to") or "").strip()
            if not from_id or not to_id:
                notes.append("connect skipped: missing from/to")
                continue
            if not _find_module(next_system, from_id) or not _find_module(next_system, to_id):
                notes.append(f"connect skipped: module missing ({from_id}->{to_id})")
                continue
            exists = any(
                str(c.get("from") or "") == from_id and str(c.get("to") or "") == to_id
                for c in next_system.get("connections", [])
            )
            if exists:
                notes.append(f"connect skipped: {from_id}->{to_id} already exists")
                continue
            next_system["connections"].append(
                {
                    "from": from_id,
                    "to": to_id,
                    "relation": str(raw.get("relation") or "data_flow").strip() or "data_flow",
                    "logic": str(raw.get("logic") or "").strip(),
                }
            )
            notes.append(f"connected {from_id}->{to_id}")
            continue

        if op_name == "disconnect":
            from_id = str(raw.get("from") or "").strip()
            to_id = str(raw.get("to") or "").strip()
            before = len(next_system.get("connections", []))
            next_system["connections"] = [
                c
                for c in next_system.get("connections", [])
                if not (str(c.get("from") or "") == from_id and str(c.get("to") or "") == to_id)
            ]
            if len(next_system["connections"]) < before:
                notes.append(f"disconnected {from_id}->{to_id}")
            else:
                notes.append(f"disconnect skipped: {from_id}->{to_id} not found")
            continue

        notes.append(f"unsupported op: {op_name}")

    return _sanitize_system(next_system), notes


def _extract_json(text: str) -> dict:
    fenced = re.search(r"```(?:json)?\s*([\s\S]*?)```", text, flags=re.IGNORECASE)
    payload = (fenced.group(1) if fenced else text).strip()
    return json.loads(payload)


def _load_prompt_template(rel_path: str, fallback: str) -> str:
    prompt_path = Path("tools/prompts") / rel_path
    if prompt_path.exists() and prompt_path.is_file():
        try:
            return prompt_path.read_text(encoding="utf-8")
        except Exception:
            return fallback
    return fallback


def _render_prompt(template: str, variables: dict[str, str]) -> str:
    out = template
    for key, value in variables.items():
        out = out.replace(key, value)
    return out


def _normalize_history_messages(history: list[ConversationMessage] | None) -> list[dict[str, str]]:
    if not history:
        return []
    out: list[dict[str, str]] = []
    for item in history:
        role = str(item.role or "").strip().lower()
        if role not in {"system", "user", "assistant"}:
            role = "user"
        content = str(item.content or "").strip()
        if not content:
            continue
        out.append({"role": role, "content": content})
    return out[-12:]


def _sanitize_assistant_text(text: str) -> str:
    normalized = (text or "").strip()
    blocked_phrases = [
        "用户指令不明确，无法确定具体的修改操作",
        "无法确定具体的修改操作",
        "用户指令不明确",
        "指令不明确",
    ]
    for phrase in blocked_phrases:
        normalized = normalized.replace(phrase, "已根据当前系统进行方向判断并执行增量修改")
    return normalized or "已根据当前系统进行方向判断并执行增量修改。"


def _build_fallback_ops(system: dict, instruction: str) -> list[dict]:
    modules = system.get("modules") if isinstance(system.get("modules"), list) else []
    brief = (instruction or "").strip().replace("\n", " ")[:48]
    if modules and isinstance(modules[0], dict):
        module_id = str(modules[0].get("id") or "").strip()
        if module_id:
            return [
                {
                    "op": "update_module",
                    "target": module_id,
                    "fields": {
                        "subtitle": f"按最新需求调整方向: {brief}" if brief else "按最新需求进行增量调整",
                    },
                }
            ]

    return [
        {
            "op": "add_module",
            "module": {
                "id": "module_1",
                "title": "核心控制模块",
                "subtitle": f"由需求派生: {brief}" if brief else "由需求派生",
                "moduleType": "custom",
                "executor": "generic",
                "codeSeed": "# module_1\n// TODO: implement",
            },
        }
    ]


def _decide_interaction_mode(req: DecideModeRequest) -> tuple[str, str]:
    model = req.model or _default_model
    temperature = req.temperature if req.temperature is not None else 0.1
    max_tokens = req.max_tokens if req.max_tokens is not None else 300

    current_system = _sanitize_system(req.current_system if isinstance(req.current_system, dict) else {})
    system_text = json.dumps(current_system, ensure_ascii=False, indent=2)
    fallback = (
        "你是交互模式决策器。请在 regenerate 与 incremental 中二选一，并输出严格 JSON。\n"
        "输出格式: {\"mode\":\"regenerate|incremental\",\"reason\":\"...\"}\n"
        "规则: has_complete_system=false 必须 regenerate；用户明确从头重做时 regenerate；其余 incremental。\n\n"
        "has_complete_system=<<HAS_COMPLETE_SYSTEM>>\n"
        "当前系统:\n<<CURRENT_SYSTEM_JSON>>\n\n"
        "用户输入:\n<<USER_INSTRUCTION>>\n"
    )
    template = _load_prompt_template("mode_decision.prompt.md", fallback)
    user_prompt = _render_prompt(
        template,
        {
            "<<HAS_COMPLETE_SYSTEM>>": str(req.has_complete_system).lower(),
            "<<CURRENT_SYSTEM_JSON>>": system_text,
            "<<USER_INSTRUCTION>>": req.instruction,
        },
    )

    params = {
        "model": model,
        "messages": [
            {"role": "system", "content": "You output strict JSON only."},
            *_normalize_history_messages(req.history),
            {"role": "user", "content": user_prompt},
        ],
        "temperature": temperature,
    }
    if max_tokens is not None:
        params["max_tokens"] = max_tokens

    response = client.chat.completions.create(**params)
    text = response.choices[0].message.content or ""
    parsed = _extract_json(text)
    mode = str(parsed.get("mode") or "").strip().lower()
    reason = str(parsed.get("reason") or "").strip()

    if mode not in {"regenerate", "incremental"}:
        mode = "regenerate" if not req.has_complete_system else "incremental"
    if not req.has_complete_system:
        mode = "regenerate"

    if not reason:
        reason = "首次需求执行完整生成。" if mode == "regenerate" else "基于现有系统执行增量修改。"
    return mode, reason


def _generate_patch_ops(
    system: dict,
    instruction: str,
    model: str,
    temperature: float,
    max_tokens: int | None,
    history: list[ConversationMessage] | None = None,
) -> tuple[str, list[dict]]:
    system_text = json.dumps(system, ensure_ascii=False, indent=2)
    fallback = (
        "你是系统模型增量修改助手。请根据用户指令对当前系统做最小改动，返回 JSON。\n"
        "允许操作: add_module, remove_module, update_module, update_param, connect, disconnect。\n"
        "输出格式: {\"assistant\":\"...\",\"ops\":[...]}\n"
        "要求: 模糊指令也必须推断方向并输出至少1条可执行操作，禁止空 ops。\n\n"
        "当前系统:\n<<CURRENT_SYSTEM_JSON>>\n\n"
        "用户指令:\n<<USER_INSTRUCTION>>\n"
    )
    template = _load_prompt_template("incremental_patch.prompt.md", fallback)
    user_prompt = _render_prompt(
        template,
        {
            "<<CURRENT_SYSTEM_JSON>>": system_text,
            "<<USER_INSTRUCTION>>": instruction,
        },
    )

    params = {
        "model": model,
        "messages": [
            {"role": "system", "content": "You output strict JSON only."},
            *_normalize_history_messages(history),
            {"role": "user", "content": user_prompt},
        ],
        "temperature": temperature,
    }
    if max_tokens is not None:
        params["max_tokens"] = max_tokens

    response = client.chat.completions.create(**params)
    text = response.choices[0].message.content or ""
    parsed = _extract_json(text)
    assistant = _sanitize_assistant_text(str(parsed.get("assistant") or "已完成增量修改。"))
    ops = parsed.get("ops") if isinstance(parsed.get("ops"), list) else []
    normalized_ops = [item for item in ops if isinstance(item, dict)]
    if not normalized_ops:
        normalized_ops = _build_fallback_ops(system, instruction)
        assistant = "已根据当前系统自动判断修改方向并执行增量调整。"
    return assistant, normalized_ops


def _safe_list_files(root: str) -> list[str]:
    base = Path(root).resolve()
    if not base.exists():
        return []
    out: list[str] = []
    for p in base.rglob("*"):
        if p.is_file():
            out.append(str(p.relative_to(base)).replace("\\", "/"))
    return sorted(out)


def _safe_read_under(root: str, rel: str) -> str:
    base = Path(root).resolve()
    target = (base / rel).resolve()
    if base not in target.parents and target != base:
        raise HTTPException(status_code=400, detail="Invalid path")
    if not target.exists() or not target.is_file():
        raise HTTPException(status_code=404, detail="File not found")
    return target.read_text(encoding="utf-8")


def _normalize_diag_level(value: str | None) -> str:
    raw = str(value or "").strip().upper()
    if raw in {"FATAL", "PANIC", "CRITICAL"}:
        return "FATAL"
    if raw in {"ERROR", "ERR"}:
        return "ERROR"
    if raw in {"WARN", "WARNING"}:
        return "WARN"
    return "INFO"


def _parse_diag_kv_tokens(tokens: list[str]) -> dict[str, str]:
    out: dict[str, str] = {}
    for token in tokens:
        item = token.strip()
        if not item or "=" not in item:
            continue
        key, value = item.split("=", 1)
        key = key.strip().lower()
        value = value.strip()
        if key:
            out[key] = value
    return out


def _parse_diagnostic_line(line: str, line_no: int) -> DiagnosticEvent | None:
    raw = (line or "").strip()
    if not raw:
        return None

    # Structured format recommendation:
    # [DIAG]|ts=...|lvl=ERROR|code=...|module=...|msg=...|k=v
    if "[DIAG]" in raw:
        normalized = raw.replace("[DIAG]", "").strip()
        normalized = normalized[1:] if normalized.startswith("|") else normalized
        parts = [p.strip() for p in normalized.split("|") if p.strip()]
        meta = _parse_diag_kv_tokens(parts)

        message = meta.get("msg") or meta.get("message") or ""
        event = DiagnosticEvent(
            line_no=line_no,
            raw=raw,
            timestamp=meta.get("ts") or meta.get("timestamp"),
            level=_normalize_diag_level(meta.get("lvl") or meta.get("level")),
            code=(meta.get("code") or "UNSPECIFIED").strip() or "UNSPECIFIED",
            module=(meta.get("module") or "general").strip() or "general",
            message=message.strip(),
            context={k: v for k, v in meta.items() if k not in {"ts", "timestamp", "lvl", "level", "code", "module", "msg", "message"}},
        )
        return event

    lower = raw.lower()
    if "error:" in lower or " failed" in lower:
        return DiagnosticEvent(
            line_no=line_no,
            raw=raw,
            level="ERROR",
            code="BUILD_ERROR",
            module="build",
            message=raw,
            context={},
        )

    if "warning:" in lower:
        return DiagnosticEvent(
            line_no=line_no,
            raw=raw,
            level="WARN",
            code="BUILD_WARNING",
            module="build",
            message=raw,
            context={},
        )

    if "警报" in raw or "alarm" in lower:
        return DiagnosticEvent(
            line_no=line_no,
            raw=raw,
            level="WARN",
            code="ALARM_TRIGGERED",
            module="alert_system",
            message=raw,
            context={},
        )

    if "映射后的噪声级别" in raw or "mapped" in lower:
        level_val = re.findall(r"(-?\d+)", raw)
        ctx = {"mapped": level_val[-1]} if level_val else {}
        return DiagnosticEvent(
            line_no=line_no,
            raw=raw,
            level="INFO",
            code="NOISE_LEVEL_SAMPLE",
            module="noise_mapping",
            message=raw,
            context=ctx,
        )

    return None


def _summarize_diagnostic_events(events: list[DiagnosticEvent]) -> list[DiagnosticSummaryItem]:
    buckets: dict[tuple[str, str, str], DiagnosticSummaryItem] = {}
    for event in events:
        key = (event.code, event.level, event.module)
        current = buckets.get(key)
        if current is None:
            buckets[key] = DiagnosticSummaryItem(
                code=event.code,
                level=event.level,
                module=event.module,
                count=1,
                latest_line_no=event.line_no,
            )
        else:
            current.count += 1
            current.latest_line_no = max(current.latest_line_no, event.line_no)

    ordered = sorted(
        buckets.values(),
        key=lambda item: (-item.count, item.level != "ERROR", item.code, item.module),
    )
    return ordered


def _build_diag_recommended_prompt(
    req: DiagnosticSuggestFixRequest,
    summary: list[DiagnosticSummaryItem],
) -> str:
    requirement = (req.requirement or "").strip()
    req_line = requirement if requirement else "(未提供需求原文)"
    summary_lines = [f"- {item.code} | {item.level} | {item.module} | count={item.count}" for item in summary]
    code_excerpt = (req.current_arduino_code or "").strip()
    if len(code_excerpt) > 2500:
        code_excerpt = code_excerpt[:2500] + "\n... (truncated)"

    return (
        "请基于以下信息给出最小修复补丁，且保持模块边界不变:\n"
        f"需求: {req_line}\n"
        "诊断摘要:\n"
        + ("\n".join(summary_lines) if summary_lines else "- (无可用诊断摘要)")
        + "\n\n"
        "请输出: 1) 根因假设 2) 最小改动 3) 回归验证点。\n"
        "当前Arduino代码片段:\n"
        + (code_excerpt or "(未提供)")
    )


def _build_diag_suggestions(summary: list[DiagnosticSummaryItem]) -> tuple[list[str], list[DiagnosticSuggestion]]:
    causes: list[str] = []
    suggestions: list[DiagnosticSuggestion] = []

    has_build_error = any(item.code == "BUILD_ERROR" for item in summary)
    has_alarm = any(item.code == "ALARM_TRIGGERED" for item in summary)
    has_noise_samples = any(item.code == "NOISE_LEVEL_SAMPLE" for item in summary)

    if has_build_error:
        causes.append("构建阶段存在语法或依赖错误，运行期问题分析优先级应后置。")
        suggestions.append(
            DiagnosticSuggestion(
                title="先清理编译错误",
                rationale="编译失败时运行日志不具备可信度，先确保可编译可运行。",
                patch_hint="优先修复编译日志首个 error，并对同类声明/类型错误做批量检查。",
                risk="low",
            )
        )

    if has_alarm:
        causes.append("阈值策略可能过于敏感，存在误报或抖动触发风险。")
        suggestions.append(
            DiagnosticSuggestion(
                title="增加告警去抖和最小间隔",
                rationale="阈值附近噪声波动会导致频繁触发，影响可用性。",
                patch_hint="在 decision 模块引入连续命中计数与触发冷却时间，再决定是否告警。",
                risk="medium",
            )
        )

    if has_noise_samples:
        causes.append("采样值虽有输出，但缺少异常范围检查与传感器失联判定。")
        suggestions.append(
            DiagnosticSuggestion(
                title="补充传感器异常检测",
                rationale="长期同值或突变值应被标记，以便区分硬件问题和算法问题。",
                patch_hint="增加连续同值计数、异常跳变检测，并输出结构化错误码。",
                risk="low",
            )
        )

    if not causes:
        causes.append("日志样本不足或缺少结构化字段，根因判定置信度有限。")
        suggestions.append(
            DiagnosticSuggestion(
                title="升级日志格式",
                rationale="没有事件码和上下文时，难以稳定迭代。",
                patch_hint="按 [DIAG]|ts=|lvl=|code=|module=|msg=|k=v 格式输出关键事件。",
                risk="low",
            )
        )

    return causes, suggestions


@app.get("/health")
def health():
    return {"status": "ok"}


@app.post("/diagnostics/parse", response_model=DiagnosticParseResponse)
def parse_diagnostics(req: DiagnosticParseRequest):
    lines = (req.raw_log or "").splitlines()
    events: list[DiagnosticEvent] = []
    for idx, line in enumerate(lines, start=1):
        item = _parse_diagnostic_line(line, idx)
        if item is None:
            continue
        events.append(item)
        if len(events) >= req.max_events:
            break

    summary = _summarize_diagnostic_events(events)
    return DiagnosticParseResponse(
        source=req.source,
        total_lines=len(lines),
        parsed_events=len(events),
        dropped_lines=max(0, len(lines) - len(events)),
        events=events,
        summary=summary,
    )


@app.post("/diagnostics/summarize", response_model=DiagnosticSummarizeResponse)
def summarize_diagnostics(req: DiagnosticSummarizeRequest):
    summary = _summarize_diagnostic_events(req.events)
    top_level = summary[0].level if summary else "INFO"
    return DiagnosticSummarizeResponse(summary=summary, top_level=top_level)


@app.post("/diagnostics/suggest-fix", response_model=DiagnosticSuggestFixResponse)
def suggest_diagnostic_fix(req: DiagnosticSuggestFixRequest):
    summary = _summarize_diagnostic_events(req.events)
    causes, suggestions = _build_diag_suggestions(summary)
    recommended_prompt = _build_diag_recommended_prompt(req, summary)
    headline = "已根据日志生成最小修复建议" if summary else "日志信息不足，建议先补充结构化诊断"
    return DiagnosticSuggestFixResponse(
        headline=headline,
        probable_root_causes=causes,
        suggestions=suggestions,
        recommended_prompt=recommended_prompt,
        llm_used=False,
    )


@app.post("/interaction/decide-mode", response_model=DecideModeResponse)
def decide_mode(req: DecideModeRequest):
    if not _llm_api_key:
        fallback = "regenerate" if not req.has_complete_system else "incremental"
        reason = "未配置 OPENROUTER_API_KEY/OPENAI_API_KEY，使用默认模式判定。"
        return DecideModeResponse(mode=fallback, reason=reason)

    try:
        mode, reason = _decide_interaction_mode(req)
    except Exception as exc:
        mode = "regenerate" if not req.has_complete_system else "incremental"
        reason = f"模式判定降级: {type(exc).__name__}"

    return DecideModeResponse(mode=mode, reason=reason)


@app.get("/system-state")
def get_system_state(session_id: str = "default"):
    session = (session_id or "default").strip() or "default"
    existing = _system_state_store.get(session)
    if not existing:
        return {"session_id": session, "version": 0, "system": _empty_system()}
    return {"session_id": session, "version": int(existing.get("version") or 0), "system": existing.get("system")}


@app.post("/system-state/save")
def save_system_state(req: SystemStateSaveRequest):
    session = (req.session_id or "default").strip() or "default"
    system = _sanitize_system(req.system)
    existing = _system_state_store.get(session)
    next_version = int(existing.get("version") or 0) + 1 if isinstance(existing, dict) else 1
    _system_state_store[session] = {"version": next_version, "system": system}
    _save_system_state_store()
    return {"session_id": session, "version": next_version, "system": system}


@app.post("/system-state/patch", response_model=SystemPatchResponse)
def patch_system_state(req: SystemPatchRequest):
    if not _llm_api_key:
        raise HTTPException(status_code=500, detail="OPENROUTER_API_KEY/OPENAI_API_KEY is not set")

    session = (req.session_id or "default").strip() or "default"
    state = _get_or_init_state(session, req.current_system)
    base_system = state.get("system") if isinstance(state.get("system"), dict) else _empty_system()

    model = req.model or _default_model
    temperature = req.temperature if req.temperature is not None else 0.2
    max_tokens = req.max_tokens if req.max_tokens is not None else 1400

    try:
        assistant_text, ops = _generate_patch_ops(base_system, req.instruction, model, temperature, max_tokens, req.history)
    except Exception as exc:
        raise HTTPException(status_code=500, detail=f"Failed to generate patch ops: {type(exc).__name__}: {exc}")

    next_system, notes = _apply_ops_to_system(base_system, ops)
    next_version = int(state.get("version") or 0) + 1
    _system_state_store[session] = {"version": next_version, "system": next_system}
    _save_system_state_store()

    assistant_merged = assistant_text
    if notes:
        assistant_merged = f"{assistant_text}\n\n应用结果:\n- " + "\n- ".join(notes)

    return SystemPatchResponse(
        session_id=session,
        version=next_version,
        system=next_system,
        ops=ops,
        assistant=assistant_merged,
    )


def _load_prompt(req: GenerateFromPromptRequest) -> str:
    if req.prompt and req.prompt.strip():
        return req.prompt
    if req.prompt_file and req.prompt_file.strip():
        try:
            with open(req.prompt_file, "r", encoding="utf-8") as handle:
                return handle.read()
        except Exception as exc:
            raise HTTPException(status_code=400, detail=f"Failed to read prompt_file: {exc}")
    raise HTTPException(status_code=400, detail="Either 'prompt' or 'prompt_file' must be provided")


def _write_text(path: str, content: str) -> None:
    os.makedirs(os.path.dirname(path) or ".", exist_ok=True)
    with open(path, "w", encoding="utf-8") as handle:
        handle.write(content)


def _resolve_path(path_like: str) -> Path:
    raw = (path_like or "").strip().strip('"').strip("'")

    if os.name == "nt":
        # Support WSL-style absolute paths when backend runs on Windows.
        m_wsl = re.match(r"^/mnt/([A-Za-z])/(.*)$", raw)
        if m_wsl:
            drive = m_wsl.group(1).upper()
            rest = m_wsl.group(2).replace("/", "\\")
            return Path(f"{drive}:\\{rest}").resolve()

        # Recover from malformed path like C:\mnt\c\Users\...
        m_bad = re.match(r"^[A-Za-z]:[\\/]mnt[\\/]([A-Za-z])[\\/](.*)$", raw)
        if m_bad:
            drive = m_bad.group(1).upper()
            rest = m_bad.group(2).replace("/", "\\")
            return Path(f"{drive}:\\{rest}").resolve()

    path = Path(raw)
    if path.is_absolute():
        return path.resolve()
    return (Path.cwd() / path).resolve()


def _sketch_board(source: str, requested: str | None) -> str:
    boards = set(re.findall(r'^// LLM4MDD target-board: (\S+)\s*$', source, re.M))
    if len(boards) > 1:
        raise HTTPException(status_code=422, detail="Sketch has conflicting interface contract board markers")
    contract_board = next(iter(boards), None)
    if contract_board and requested and contract_board != requested:
        raise HTTPException(status_code=422, detail=f"Requested board {requested} differs from sketch contract board {contract_board}")
    return requested or contract_board or "arduino:avr:uno"


def _compile_arduino_sketch(sketch_path_like: str, fqbn: str | None, *, provenance=None) -> ArduinoCompileResponse:
    resolved_source = _resolve_path(sketch_path_like)
    if not resolved_source.exists() or not resolved_source.is_file():
        raise HTTPException(status_code=404, detail=f"Sketch file not found: {resolved_source}")

    source_text=resolved_source.read_text(encoding='utf-8')
    digest=text_digest(source_text)
    if provenance is not None:
        try:AUTHORITY.verify(provenance,'arduino',dict(arduino_sha256=digest))
        except ValueError as exc:raise HTTPException(status_code=422,detail=str(exc)) from exc
    fqbn = _sketch_board(source_text, fqbn)

    # Keep compile and upload on the same Windows Arduino CLI toolchain
    # to avoid library mismatch between WSL/Linux and Windows environments.
    cli = _resolve_windows_arduino_cli()

    stem = resolved_source.stem.strip() or "arduino_output"
    try:
        with tempfile.TemporaryDirectory(prefix='arduino-compile-') as directory:
            compile_root=Path(directory)/stem
            compile_root.mkdir()
            sketch_path = compile_root / f"{stem}.ino"
            _write_text(str(sketch_path), source_text)
            sketch_arg = _to_windows_arg_path(compile_root)
            proc = subprocess.run(
                [cli, "compile", "--fqbn", fqbn, sketch_arg],
                capture_output=True,
                text=True,
                timeout=180,
            )
    except HTTPException:
        raise
    except Exception as exc:
        raise HTTPException(status_code=500, detail=f"Arduino compile failed: {type(exc).__name__}: {exc}")

    return ArduinoCompileResponse(
        provenance_status='verified_model_chain' if provenance is not None else 'unverified_legacy',
        arduino_sha256=digest,
        success=proc.returncode == 0,
        fqbn=fqbn,
        sketch_path=str(resolved_source),
        compile_stdout=(proc.stdout or "").strip(),
        compile_stderr=(proc.stderr or "").strip(),
    )


def _resolve_windows_arduino_cli() -> str:
    env_path = (os.getenv("WINDOWS_ARDUINO_CLI_PATH") or "").strip()
    candidates: list[str] = []
    if env_path:
        candidates.append(env_path)

    if os.name == "nt":
        which_cli = shutil.which("arduino-cli")
        if which_cli:
            candidates.append(which_cli)
    else:
        cwd_str = str(Path.cwd().resolve())
        matched = re.match(r"^/mnt/c/Users/([^/]+)/", cwd_str)
        if matched:
            win_user = matched.group(1)
            candidates.append(f"/mnt/c/Users/{win_user}/AppData/Local/Microsoft/WinGet/Links/arduino-cli.exe")
        candidates.extend(
            [
                "/mnt/c/Users/34267/AppData/Local/Microsoft/WinGet/Links/arduino-cli.exe",
                "/mnt/c/Program Files/Arduino CLI/arduino-cli.exe",
            ]
        )

    for candidate in candidates:
        if not candidate:
            continue
        resolved = _resolve_path(candidate)
        if resolved.exists() and resolved.is_file():
            return str(resolved)

    raise HTTPException(
        status_code=500,
        detail="Windows arduino-cli.exe not found. Set WINDOWS_ARDUINO_CLI_PATH or install Arduino CLI on Windows.",
    )


def _resolve_powershell_command() -> str:
    candidates = [
        shutil.which("powershell.exe"),
        shutil.which("powershell"),
        shutil.which("pwsh"),
    ]
    for candidate in candidates:
        if candidate:
            return candidate
    raise HTTPException(status_code=500, detail="PowerShell not found; cannot read Arduino serial output.")


def _read_arduino_serial_via_powershell(port: str, baud_rate: int, read_timeout_ms: int, max_chars: int) -> ArduinoSerialReadResponse:
    port_name = (port or "").strip()
    if not port_name:
        raise HTTPException(status_code=400, detail="port is required")

    ps = _resolve_powershell_command()
    script = f"""
$ErrorActionPreference = 'Stop'
[Console]::OutputEncoding = [System.Text.UTF8Encoding]::new()
$portName = {json.dumps(port_name)}
$baudRate = {int(baud_rate)}
$readTimeoutMs = {int(read_timeout_ms)}
$maxChars = {int(max_chars)}
$serial = New-Object System.IO.Ports.SerialPort $portName, $baudRate, 'None', 8, 'one'
$serial.ReadTimeout = [Math]::Max(50, $readTimeoutMs)
$serial.WriteTimeout = [Math]::Max(50, $readTimeoutMs)
$serial.DtrEnable = $false
$serial.RtsEnable = $false
$serial.NewLine = "`n"
$builder = New-Object System.Text.StringBuilder
try {{
    $serial.Open()
    Start-Sleep -Milliseconds 120
    $deadline = [DateTime]::UtcNow.AddMilliseconds($readTimeoutMs)
    while(([DateTime]::UtcNow -lt $deadline) -and ($builder.Length -lt $maxChars)) {{
        if($serial.BytesToRead -gt 0) {{
            $chunk = $serial.ReadExisting()
            if($chunk) {{
                [void]$builder.Append($chunk)
            }}
        }} else {{
            Start-Sleep -Milliseconds 60
        }}
    }}
}} finally {{
    if($serial -and $serial.IsOpen) {{
        $serial.Close()
    }}
    $serial.Dispose()
}}
$text = $builder.ToString()
if($text.Length -gt $maxChars) {{
    $text = $text.Substring(0, $maxChars)
}}
Write-Output $text
"""

    try:
        proc = subprocess.run(
            [ps, "-NoProfile", "-ExecutionPolicy", "Bypass", "-Command", script],
            capture_output=True,
            text=True,
            timeout=max(5, int(read_timeout_ms / 1000) + 5),
        )
    except subprocess.TimeoutExpired:
        raise HTTPException(status_code=504, detail=f"Timed out while reading serial port {port_name}")
    except Exception as exc:
        raise HTTPException(status_code=500, detail=f"Failed to read serial port {port_name}: {type(exc).__name__}: {exc}")

    if proc.returncode != 0:
        detail = ((proc.stderr or "") + "\n" + (proc.stdout or "")).strip() or f"Unknown serial read error on {port_name}"
        raise HTTPException(status_code=500, detail=f"Serial read failed on {port_name}: {detail[-2000:]}")

    text = (proc.stdout or "").replace("\r\n", "\n")
    return ArduinoSerialReadResponse(
        port=port_name,
        baud_rate=baud_rate,
        read_timeout_ms=read_timeout_ms,
        text=text,
        chars_read=len(text),
    )


def _default_arduino_boards() -> list[ArduinoDeviceBoard]:
    return [
        ArduinoDeviceBoard(label="Arduino Uno", fqbn="arduino:avr:uno"),
        ArduinoDeviceBoard(label="Arduino Nano", fqbn="arduino:avr:nano"),
        ArduinoDeviceBoard(label="Arduino Mega 2560", fqbn="arduino:avr:mega"),
    ]


def _detect_windows_serial_ports() -> list[str]:
    candidates = [
        Path("/mnt/c/Windows/System32/mode.com"),
        Path("/mnt/c/Windows/System32/cmd.exe"),
    ]
    try:
        if candidates[0].exists():
            proc = subprocess.run([str(candidates[0]), "COM1"], capture_output=True, text=True, timeout=10)
            _ = proc
    except Exception:
        pass

    try:
        cmd_exe = candidates[1]
        if not cmd_exe.exists():
            return []
        proc = subprocess.run([str(cmd_exe), "/c", "mode"], capture_output=True, text=True, timeout=10)
        text = f"{proc.stdout}\n{proc.stderr}"
        ports = sorted(set(re.findall(r"\bCOM\d+\b", text, re.IGNORECASE)))
        return ports
    except Exception:
        return []


def _list_arduino_devices_via_windows_cli() -> ArduinoDevicesResponse:
    cli = _resolve_windows_arduino_cli()
    boards = _default_arduino_boards()
    ports: list[ArduinoDevicePort] = []

    try:
        proc = subprocess.run(
            [cli, "board", "list", "--format", "json"],
            capture_output=True,
            text=True,
            timeout=20,
        )
        raw = (proc.stdout or "").strip()
        parsed = json.loads(raw) if raw else {}
        detected = parsed.get("detected_ports") if isinstance(parsed, dict) else []
        if isinstance(detected, list):
            for item in detected:
                if not isinstance(item, dict):
                    continue
                port_info = item.get("port") if isinstance(item.get("port"), dict) else {}
                matching = item.get("matching_boards") if isinstance(item.get("matching_boards"), list) else []
                first_match = matching[0] if matching and isinstance(matching[0], dict) else {}
                address = str(port_info.get("address") or "").strip()
                if not address:
                    continue
                ports.append(
                    ArduinoDevicePort(
                        address=address,
                        label=str(port_info.get("label") or address),
                        protocol=str(port_info.get("protocol") or "").strip() or None,
                        board_name=str(first_match.get("name") or "").strip() or None,
                        fqbn=str(first_match.get("fqbn") or "").strip() or None,
                    )
                )
    except Exception:
        ports = []

    if not ports:
        fallback_ports = _detect_windows_serial_ports()
        ports = [ArduinoDevicePort(address=port, label=port) for port in fallback_ports]

    return ArduinoDevicesResponse(ports=ports, boards=boards)


def _upload_arduino_sketch_via_windows_cli(sketch_path_like: str, fqbn: str, port: str) -> ArduinoUploadResponse:
    resolved_source = _resolve_path(sketch_path_like)
    if not resolved_source.exists() or not resolved_source.is_file():
        raise HTTPException(status_code=404, detail=f"Sketch file not found: {resolved_source}")

    fqbn = _sketch_board(resolved_source.read_text(encoding="utf-8"), fqbn)

    cli = _resolve_windows_arduino_cli()
    stem = resolved_source.stem.strip() or "arduino_output"
    upload_root = Path(OUTPUT_DIR) / ".arduino_manual_upload" / stem

    try:
        if upload_root.exists():
            shutil.rmtree(upload_root)
        upload_root.mkdir(parents=True, exist_ok=True)

        sketch_path = upload_root / f"{stem}.ino"
        _write_text(str(sketch_path), resolved_source.read_text(encoding="utf-8"))

        sketch_arg = _to_windows_arg_path(upload_root)
        compile_cmd = [cli, "compile", "--fqbn", fqbn, sketch_arg]
        compile_proc = subprocess.run(
            compile_cmd,
            capture_output=True,
            text=True,
            timeout=180,
        )
        if compile_proc.returncode != 0:
            return ArduinoUploadResponse(
                success=False,
                message="编译失败，未开始上传。",
                fqbn=fqbn,
                port=port,
                sketch_path=str(resolved_source),
                compile_success=False,
                upload_success=False,
                compile_command=" ".join(compile_cmd),
                compile_stdout=(compile_proc.stdout or "").strip(),
                compile_stderr=(compile_proc.stderr or "").strip(),
            )

        upload_cmd = [cli, "upload", "-p", port, "--fqbn", fqbn, sketch_arg]
        upload_proc = subprocess.run(
            upload_cmd,
            capture_output=True,
            text=True,
            timeout=180,
        )
    except HTTPException:
        raise
    except Exception as exc:
        raise HTTPException(status_code=500, detail=f"Arduino upload failed: {type(exc).__name__}: {exc}")

    return ArduinoUploadResponse(
        success=upload_proc.returncode == 0,
        message="编译成功，上传成功。" if upload_proc.returncode == 0 else "编译成功，但上传失败。",
        fqbn=fqbn,
        port=port,
        sketch_path=str(resolved_source),
        compile_success=True,
        upload_success=upload_proc.returncode == 0,
        compile_command=" ".join(compile_cmd),
        upload_command=" ".join(upload_cmd),
        compile_stdout=(compile_proc.stdout or "").strip(),
        compile_stderr=(compile_proc.stderr or "").strip(),
        upload_stdout=(upload_proc.stdout or "").strip(),
        upload_stderr=(upload_proc.stderr or "").strip(),
    )


def _build_marked_module_code_map(arduino_code: str, module_specs: list[dict] | None) -> tuple[str, dict[str, str]]:
    specs = [item for item in (module_specs or []) if isinstance(item, dict) and str(item.get("id") or "").strip()]
    if not specs:
        return arduino_code, {}

    existing_map = _extract_marked_module_code_map(arduino_code)
    if not existing_map:
        return arduino_code, {}

    spec_ids = {str(item.get("id") or "").strip() for item in specs}
    filtered_existing = {module_id: code for module_id, code in existing_map.items() if module_id in spec_ids}
    return arduino_code, filtered_existing


def _extract_marked_module_code_map(arduino_code: str) -> dict[str, str]:
    src = arduino_code or ""
    pattern = re.compile(
        r"//\s*\[MODULE:(?P<module_id>[^:\]]+):start\]\s*\n(?P<code>.*?)\n//\s*\[MODULE:(?P=module_id):end\]",
        re.S,
    )
    out: dict[str, str] = {}
    for match in pattern.finditer(src):
        module_id = str(match.group("module_id") or "").strip()
        code = str(match.group("code") or "").strip()
        if module_id:
            out[module_id] = code
    return out


def _replace_marked_module_code(arduino_code: str, module_id: str, new_code: str) -> str:
    src = arduino_code or ""
    normalized_module_id = str(module_id or "").strip()
    normalized_code = (new_code or "").strip()
    if not normalized_module_id:
        raise HTTPException(status_code=400, detail="module_id is required")
    if not normalized_code:
        raise HTTPException(status_code=400, detail="code is required")

    pattern = re.compile(
        rf"(//\s*\[MODULE:{re.escape(normalized_module_id)}:start\]\s*\n)(.*?)(\n//\s*\[MODULE:{re.escape(normalized_module_id)}:end\])",
        re.S,
    )
    if not pattern.search(src):
        raise HTTPException(status_code=404, detail=f"Module marker not found for: {normalized_module_id}")

    updated = pattern.sub(lambda match: match.group(1) + normalized_code + match.group(3), src, count=1)
    return updated


def _parse_param_marker(marker_text: str) -> dict[str, str]:
    info: dict[str, str] = {}
    text = (marker_text or "").strip()
    if not text:
        return info
    for token in text.split("|"):
        token = token.strip()
        if "=" not in token:
            continue
        key, value = token.split("=", 1)
        key = key.strip().lower()
        value = value.strip()
        if key:
            info[key] = value
    return info


def _infer_param_zh_name(name: str) -> str:
    key = str(name or "").strip()
    if not key:
        return "参数"
    exact_map: dict[str, str] = {
        "noiseThreshold": "噪声阈值",
        "threshold": "阈值",
        "sampleIntervalMs": "采样间隔(毫秒)",
        "sampleInterval": "采样间隔",
        "interval": "间隔",
        "noiseLevel": "噪声等级",
        "level": "等级",
        "alarmTriggered": "是否触发告警",
        "alarm": "告警标志",
        "gain": "增益",
        "factor": "系数",
        "ratio": "比例",
        "freq": "频率",
        "hz": "频率(Hz)",
        "timeout": "超时时间",
        "period": "周期",
        "window": "窗口大小",
        "speed": "速度",
        "duty": "占空比",
        "limit": "限制值",
    }
    if key in exact_map:
        return exact_map[key]
    lower = key.lower()
    if "threshold" in lower:
        return "阈值"
    if "interval" in lower:
        return "间隔"
    if "sample" in lower:
        return "采样参数"
    if "level" in lower:
        return "等级参数"
    if "alarm" in lower:
        return "告警参数"
    if "gain" in lower:
        return "增益"
    if "ratio" in lower:
        return "比例"
    if "freq" in lower or "hz" in lower:
        return "频率"
    if "timeout" in lower:
        return "超时时间"
    if "period" in lower:
        return "周期"
    if "window" in lower:
        return "窗口大小"
    if "speed" in lower:
        return "速度"
    if "duty" in lower:
        return "占空比"
    if "limit" in lower:
        return "限制值"
    if (
        "pin" in lower
        or lower in {"clk", "dio", "scl", "sda", "miso", "mosi", "cs", "ss", "rx", "tx"}
        or lower.endswith("_pin")
    ):
        return "引脚"
    return key


def _extract_marked_param_list(arduino_code: str) -> list[dict]:
    params: list[dict] = []
    marker_re = re.compile(r"\[PARAM:(?P<meta>[^\]]+)\]")
    decl_re = re.compile(
        r"^\s*(?:const\s+)?(?P<type>int|long|short|float|double|bool|byte|uint8_t|uint16_t|uint32_t|unsigned\s+long|unsigned\s+int)\s+"
        r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*(?P<value>[^;]+);"
    )
    seen: set[str] = set()
    for idx, line in enumerate((arduino_code or "").splitlines(), start=1):
        marker = marker_re.search(line)
        if not marker:
            continue
        meta = _parse_param_marker(marker.group("meta"))
        param_id = str(meta.get("id") or "").strip()
        if not param_id or param_id in seen:
            continue
        decl_match = decl_re.search(line)
        if not decl_match:
            continue
        seen.add(param_id)
        params.append(
            {
                "param_id": param_id,
                "module_id": str(meta.get("module") or "").strip(),
                "name": str(meta.get("name") or decl_match.group("name") or "").strip(),
                "zh_name": str(meta.get("zh_name") or meta.get("zh") or meta.get("label_cn") or "").strip(),
                "type": str(meta.get("type") or decl_match.group("type") or "").strip(),
                "value": str(decl_match.group("value") or "").strip(),
                "min": str(meta.get("min") or "").strip(),
                "max": str(meta.get("max") or "").strip(),
                "line": idx,
            }
        )
        if not params[-1]["zh_name"]:
            params[-1]["zh_name"] = _infer_param_zh_name(params[-1]["name"])
    return params


def _mark_module_params_in_arduino_code(arduino_code: str) -> tuple[str, list[dict]]:
    src = arduino_code or ""
    lines = src.splitlines()
    if not lines:
        return src, []

    module_start_re = re.compile(r"^\s*//\s*\[MODULE:(?P<module>[^:\]]+):start\]\s*$")
    module_end_re = re.compile(r"^\s*//\s*\[MODULE:(?P<module>[^:\]]+):end\]\s*$")
    decl_re = re.compile(
        r"^(?P<indent>\s*)(?P<prefix>(?:const\s+)?(?P<type>int|long|short|float|double|bool|byte|uint8_t|uint16_t|uint32_t|unsigned\s+long|unsigned\s+int)\s+"
        r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*)(?P<value>[^;]+)(?P<tail>;\s*)(?P<comment>//.*)?$"
    )
    marker_re = re.compile(r"\[PARAM:(?P<meta>[^\]]+)\]")
    param_like_name_re = re.compile(
        r"(threshold|limit|gain|factor|interval|period|timeout|hz|freq|sample|window|ratio|level|duty|speed|pin|clk|dio|scl|sda|miso|mosi|cs|ss|rx|tx)",
        re.I,
    )

    active_module = ""
    out_lines: list[str] = []
    created_ids: set[str] = set()
    for line in lines:
        start_match = module_start_re.match(line)
        if start_match:
            active_module = str(start_match.group("module") or "").strip()
            out_lines.append(line)
            continue

        end_match = module_end_re.match(line)
        if end_match:
            out_lines.append(line)
            active_module = ""
            continue

        if not active_module:
            out_lines.append(line)
            continue

        decl_match = decl_re.match(line)
        if not decl_match:
            out_lines.append(line)
            continue

        var_name = str(decl_match.group("name") or "").strip()
        if not param_like_name_re.search(var_name):
            out_lines.append(line)
            continue

        if marker_re.search(line):
            out_lines.append(line)
            continue

        var_type = str(decl_match.group("type") or "").strip()
        param_id = f"{active_module}.{var_name}"
        zh_name = _infer_param_zh_name(var_name)
        if param_id in created_ids:
            out_lines.append(line)
            continue
        created_ids.add(param_id)

        existing_comment = (decl_match.group("comment") or "").strip()
        marker_text = (
            f"[PARAM:id={param_id}|module={active_module}|name={var_name}|zh_name={zh_name}|type={var_type}]"
        )
        marker_comment = f"// {marker_text}"
        if existing_comment:
            new_line = f"{decl_match.group('indent')}{decl_match.group('prefix')}{decl_match.group('value')}{decl_match.group('tail')}{existing_comment} {marker_comment}"
        else:
            new_line = f"{decl_match.group('indent')}{decl_match.group('prefix')}{decl_match.group('value')}{decl_match.group('tail')}{marker_comment}"
        out_lines.append(new_line.rstrip())

    updated = "\n".join(out_lines)
    return updated, _extract_marked_param_list(updated)


def _apply_param_updates_to_arduino_code(arduino_code: str, updates: list[ArduinoParamUpdateItem]) -> tuple[str, list[dict]]:
    src = arduino_code or ""
    if not updates:
        return src, _extract_marked_param_list(src)

    marker_re = re.compile(r"\[PARAM:(?P<meta>[^\]]+)\]")
    decl_re = re.compile(
        r"^(?P<indent>\s*)(?P<prefix>(?:const\s+)?(?:int|long|short|float|double|bool|byte|uint8_t|uint16_t|uint32_t|unsigned\s+long|unsigned\s+int)\s+"
        r"[A-Za-z_][A-Za-z0-9_]*\s*=\s*)(?P<value>[^;]+)(?P<tail>;\s*)(?P<comment>//.*)?$"
    )
    update_map = {str(item.param_id).strip(): str(item.value).strip() for item in updates if str(item.param_id).strip()}
    updated_found: set[str] = set()
    out_lines: list[str] = []

    for line in src.splitlines():
        marker_match = marker_re.search(line)
        if not marker_match:
            out_lines.append(line)
            continue

        meta = _parse_param_marker(marker_match.group("meta"))
        param_id = str(meta.get("id") or "").strip()
        new_value = update_map.get(param_id)
        if not new_value:
            out_lines.append(line)
            continue

        decl_match = decl_re.match(line)
        if not decl_match:
            out_lines.append(line)
            continue

        issue = validate_param_literal_value(new_value, meta.get("type", "int"), meta.get("name", ""))
        if issue:
            raise HTTPException(status_code=400, detail={"issue": issue})
        updated_found.add(param_id)
        comment = (decl_match.group("comment") or "").strip()
        new_line = f"{decl_match.group('indent')}{decl_match.group('prefix')}{new_value}{decl_match.group('tail')}{comment}"
        out_lines.append(new_line.rstrip())

    missing = sorted(set(update_map.keys()) - updated_found)
    if missing:
        raise HTTPException(status_code=404, detail=f"Parameter marker not found for: {', '.join(missing)}")

    updated = "\n".join(out_lines)
    return updated, _extract_marked_param_list(updated)


_load_system_state_store()


def _coerce_codegen_root(path_like: str) -> Path:
    if os.name != "nt":
        matched = re.match(r"^([A-Za-z]):[\\/](.*)$", path_like)
        if matched:
            drive = matched.group(1).lower()
            rest = matched.group(2).replace("\\", "/")
            return Path(f"/mnt/{drive}/{rest}").resolve()
    return _resolve_path(path_like)


def _resolve_codegen_root_with_fallback(req_root: str | None) -> Path:
    candidates_raw: list[str] = []
    if req_root and req_root.strip():
        candidates_raw.append(req_root.strip())

    env_root = os.getenv("CODEGENERATOR2_ROOT", "").strip()
    if env_root:
        candidates_raw.append(env_root)

    # Common repo-local layouts in this project.
    candidates_raw.extend(["CodeGenerator2/CodeGenerator2", "CodeGenerator2"])

    tried: list[Path] = []
    for raw in candidates_raw:
        candidate = _coerce_codegen_root(raw)
        if candidate in tried:
            continue
        tried.append(candidate)
        if candidate.exists():
            return candidate

    # Preserve original error style while exposing attempted fallbacks.
    primary = _coerce_codegen_root(candidates_raw[0] if candidates_raw else "CodeGenerator2")
    tried_msg = ", ".join(str(p) for p in tried)
    raise HTTPException(status_code=400, detail=f"CodeGenerator2 root not found: {primary}. Tried: {tried_msg}")


def _to_windows_arg_path(path: Path) -> str:
    resolved = path.resolve()
    path_str = str(resolved)
    if os.name == "nt":
        return path_str
    if path_str.startswith("/mnt/") and len(path_str) >= 7 and path_str[6] == "/":
        drive = path_str[5].upper()
        rest = path_str[7:].replace("/", "\\")
        return f"{drive}:\\{rest}"
    distro = os.getenv("WSL_DISTRO_NAME", "Ubuntu")
    return f"\\\\wsl.localhost\\{distro}{path_str.replace('/', '\\')}"


def _has_artifact(path: Path) -> bool:
    if not path.exists():
        return False
    if path.is_file():
        return path.stat().st_size > 0
    if path.is_dir():
        return any(path.iterdir())
    return False


def _code2script_candidates_from_root(root: Path) -> list[Path]:
    candidates = [
        root / "build" / "Code2Script",
        root / "build" / "Code2Script.exe",
        root / "build" / "Release" / "Code2Script",
        root / "build" / "Release" / "Code2Script.exe",
        root / "build" / "Debug" / "Code2Script",
        root / "build" / "Debug" / "Code2Script.exe",
        root / "build-prebuilt" / "Release" / "Code2Script",
        root / "build-prebuilt" / "Release" / "Code2Script.exe",
        root / "build-lite" / "Release" / "Code2Script",
        root / "build-lite" / "Release" / "Code2Script.exe",
        root / "build-clean" / "Release" / "Code2Script",
        root / "build-clean" / "Release" / "Code2Script.exe",
        root / "Code2Script",
        root / "Code2Script.exe",
    ]
    try:
        extra = sorted(root.glob("build/**/Code2Script*"))
        for candidate in extra:
            if candidate.is_file() and candidate.name in {"Code2Script", "Code2Script.exe"}:
                candidates.append(candidate)
    except Exception:
        pass
    unique: list[Path] = []
    seen: set[str] = set()
    for candidate in candidates:
        key = str(candidate)
        if key in seen:
            continue
        seen.add(key)
        unique.append(candidate)
    return unique


def _pick_code2script_exe(req: ConvertCToIlRequest) -> tuple[Path | None, list[Path]]:
    if req.code2script_exe and req.code2script_exe.strip():
        explicit = _coerce_codegen_root(req.code2script_exe.strip()).resolve()
        valid_explicit = explicit.exists() and explicit.is_file() and explicit.name in {"Code2Script", "Code2Script.exe"}
        return (explicit if valid_explicit else None, [explicit])

    roots_raw: list[str] = []
    if req.code2script_root and req.code2script_root.strip():
        roots_raw.append(req.code2script_root.strip())
    env_root = os.getenv("CODE2SCRIPT_ROOT")
    if env_root and env_root.strip():
        roots_raw.append(env_root.strip())
    roots_raw.extend(
        [
            "Code2Script",
            "./Code2Script",
        ]
    )

    roots: list[Path] = []
    seen_roots: set[str] = set()
    for root_raw in roots_raw:
        root = _coerce_codegen_root(root_raw).resolve()
        key = str(root)
        if key in seen_roots:
            continue
        seen_roots.add(key)
        roots.append(root)

    tried: list[Path] = []
    for root in roots:
        for candidate in _code2script_candidates_from_root(root):
            tried.append(candidate.resolve())
            if candidate.exists() and candidate.is_file() and candidate.name in {"Code2Script", "Code2Script.exe"}:
                return candidate.resolve(), tried

    return None, tried


def _arg_path_for_exe(path: Path, exe_path: Path) -> str:
    if os.name != "nt" and exe_path.suffix.lower() == ".exe":
        return _to_windows_arg_path(path)
    return str(path.resolve())


def _c_type_to_il_type(c_type: str) -> str:
    normalized = re.sub(r"\s+", " ", c_type.strip())
    mapping = {
        "float": "float",
        "double": "double",
        "bool": "bool",
        "int": "int",
        "unsigned int": "unsigned int",
        "short": "short",
        "long": "long",
        "char": "char",
    }
    return mapping.get(normalized, normalized)


def _default_value_for_c_type(c_type: str) -> str:
    normalized = re.sub(r"\s+", " ", c_type.strip())
    if normalized == "bool":
        return "false"
    if normalized == "double":
        return "0.0"
    if normalized == "float":
        return "0.0f"
    return "0"


def _parse_c_annotations(source: str) -> list[dict]:
    annotations: list[dict] = []
    for raw_line in source.splitlines():
        match = re.match(r"^\s*//\s*@(?P<kind>input|output)\s*<?(?P<name>[A-Za-z_]\w*)>?\s*$", raw_line)
        if match:
            annotations.append({"kind": match.group("kind"), "name": match.group("name")})
    return annotations


def _extract_c_function(source: str) -> tuple[str, str, str]:
    match = re.search(r"\bvoid\s+([A-Za-z_]\w*)\s*\((.*?)\)\s*\{", source, re.S)
    if not match:
        raise ValueError("No supported C function found")
    func_name = match.group(1)
    params_str = match.group(2)
    body_start = match.end() - 1
    depth = 0
    body_end = -1
    for index in range(body_start, len(source)):
        char = source[index]
        if char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0:
                body_end = index
                break
    if body_end < 0:
        raise ValueError("Function body is not balanced")
    return func_name, params_str, source[body_start + 1:body_end]


def _parse_c_params(params_str: str, annotations: list[dict]) -> tuple[list[dict], list[dict]]:
    parts = [part.strip() for part in params_str.split(",") if part.strip()]
    params: list[dict] = []
    for part in parts:
        normalized = part.replace("*", " * ")
        tokens = [token for token in normalized.split() if token]
        if len(tokens) < 2:
            raise ValueError(f"Unsupported parameter declaration: {part}")
        name = tokens[-1]
        pointer = "*" in tokens[:-1]
        type_tokens = [token for token in tokens[:-1] if token != "*"]
        c_type = re.sub(r"^\s*const\s+", "", " ".join(type_tokens)).strip()
        if not re.match(r"^[A-Za-z_]\w*$", name) or not c_type:
            raise ValueError(f"Unsupported parameter declaration: {part}")
        params.append(
            {
                "name": name,
                "c_type": c_type,
                "il_type": _c_type_to_il_type(c_type),
                "pointer": pointer,
            }
        )

    order_map = {(item["kind"], item["name"]): index for index, item in enumerate(annotations)}
    inputs = [param for param in params if ("input", param["name"]) in order_map or not param["pointer"]]
    outputs = [param for param in params if ("output", param["name"]) in order_map or param["pointer"]]
    inputs.sort(key=lambda item: order_map.get(("input", item["name"]), 10_000))
    outputs.sort(key=lambda item: order_map.get(("output", item["name"]), 10_000))
    return inputs, outputs


def _prepare_c_body_lines(body: str) -> list[str]:
    normalized = body.replace("} else {", "}\nelse {")
    normalized = normalized.replace("}\r\nelse {", "}\nelse {")
    normalized = re.sub(r"}\s*else\s+if\s*\(", "}\nelse if(", normalized)
    lines: list[str] = []
    for raw_line in normalized.splitlines():
        line = re.sub(r"//.*$", "", raw_line).strip()
        if not line:
            continue
        # Defensive split for compact style: `} else if (...) {` / `} else {`
        if line.startswith("} else if"):
            lines.append("}")
            lines.append(line[2:].strip())
            continue
        if line.startswith("} else {"):
            lines.append("}")
            lines.append("else {")
            continue
        lines.append(line)
    return lines


def _parse_else_chain(lines: list[str], start_index: int) -> tuple[list[dict], int]:
    if start_index >= len(lines):
        return [], start_index

    line = lines[start_index]
    if line == "else {":
        else_block, next_index = _parse_c_block(lines, start_index + 1)
        return else_block, next_index

    match = re.match(r"^else\s+if\s*\((?P<cond>.+)\)\s*\{$", line)
    if match:
        then_block, next_index = _parse_c_block(lines, start_index + 1)
        nested_else, next_index = _parse_else_chain(lines, next_index)
        return (
            [
                {
                    "kind": "if",
                    "condition": match.group("cond").strip(),
                    "then": then_block,
                    "else": nested_else,
                }
            ],
            next_index,
        )

    return [], start_index


def _parse_c_block(lines: list[str], start_index: int = 0) -> tuple[list[dict], int]:
    statements: list[dict] = []
    index = start_index
    decl_pattern = re.compile(
        r"^(?P<type>(?:const\s+)?(?:unsigned\s+int|float|double|bool|int|short|long|char))\s+"
        r"(?P<name>[A-Za-z_]\w*)(?:\s*=\s*(?P<init>.+))?;$"
    )
    if_pattern = re.compile(r"^if\s*\((?P<cond>.+)\)\s*\{$")

    while index < len(lines):
        line = lines[index]
        if line == "}":
            return statements, index + 1
        if line == "{":
            index += 1
            continue

        if_match = if_pattern.match(line)
        if if_match:
            then_block, next_index = _parse_c_block(lines, index + 1)
            else_block, index = _parse_else_chain(lines, next_index)
            statements.append(
                {
                    "kind": "if",
                    "condition": if_match.group("cond").strip(),
                    "then": then_block,
                    "else": else_block,
                }
            )
            continue

        decl_match = decl_pattern.match(line)
        if decl_match:
            statements.append(
                {
                    "kind": "decl",
                    "type": re.sub(r"^\s*const\s+", "", decl_match.group("type")).strip(),
                    "name": decl_match.group("name"),
                    "init": decl_match.group("init").strip() if decl_match.group("init") else None,
                }
            )
            index += 1
            continue

        if line.endswith(";"):
            statements.append({"kind": "expr", "expr": line[:-1].strip()})
            index += 1
            continue

        raise ValueError(f"Unsupported C statement: {line}")

    return statements, index


def _replace_output_refs(text: str, temp_names: dict[str, str]) -> str:
    updated = text
    for output_name, temp_name in temp_names.items():
        updated = re.sub(rf"\*{re.escape(output_name)}\b", temp_name, updated)
    return updated


def _normalize_c_statements(statements: list[dict], outputs: list[dict]) -> list[dict]:
    existing_names: set[str] = set()

    def collect_names(block: list[dict]) -> None:
        for stmt in block:
            if stmt["kind"] == "decl":
                existing_names.add(stmt["name"])
            elif stmt["kind"] == "if":
                collect_names(stmt["then"])
                collect_names(stmt["else"])

    collect_names(statements)
    temp_names: dict[str, str] = {}
    for output in outputs:
        base_name = f"{output['name']}_tmp"
        temp_name = base_name
        suffix = 1
        while temp_name in existing_names:
            suffix += 1
            temp_name = f"{base_name}_{suffix}"
        existing_names.add(temp_name)
        temp_names[output["name"]] = temp_name

    def transform_block(block: list[dict]) -> list[dict]:
        transformed: list[dict] = []
        for stmt in block:
            if stmt["kind"] == "decl":
                transformed.append(stmt.copy())
                if stmt["init"] is not None and any(f"*{name}" in stmt["init"] for name in temp_names):
                    transformed[-1]["init"] = _replace_output_refs(stmt["init"], temp_names)
            elif stmt["kind"] == "expr":
                expr = _replace_output_refs(stmt["expr"], temp_names)
                assign_match = re.match(r"^\*(?P<name>[A-Za-z_]\w*)\s*=\s*(?P<rhs>.+)$", stmt["expr"])
                if assign_match and assign_match.group("name") in temp_names:
                    expr = f"{temp_names[assign_match.group('name')]} = {_replace_output_refs(assign_match.group('rhs'), temp_names)}"
                transformed.append({"kind": "expr", "expr": expr})
            elif stmt["kind"] == "if":
                transformed.append(
                    {
                        "kind": "if",
                        "condition": _replace_output_refs(stmt["condition"], temp_names),
                        "then": transform_block(stmt["then"]),
                        "else": transform_block(stmt["else"]),
                    }
                )
        return transformed

    normalized: list[dict] = [
        {
            "kind": "decl",
            "type": output["c_type"],
            "name": temp_names[output["name"]],
            "init": _default_value_for_c_type(output["c_type"]),
        }
        for output in outputs
    ]
    normalized.extend(transform_block(statements))
    normalized.extend(
        {"kind": "expr", "expr": f"*{output['name']} = {temp_names[output['name']]}"}
        for output in outputs
    )
    return normalized


def _statement_to_xml(parent: ET.Element, stmt: dict) -> None:
    if stmt["kind"] == "decl":
        element = ET.SubElement(parent, "LocalVariable")
        element.set("Name", stmt["name"])
        element.set("Type", _c_type_to_il_type(stmt["type"]))
        if stmt["init"] is not None:
            element.set("DefaultValue", stmt["init"])
        return
    if stmt["kind"] == "expr":
        element = ET.SubElement(parent, "Expression")
        element.set("Expression", stmt["expr"])
        return
    if stmt["kind"] == "if":
        element = ET.SubElement(parent, "If")
        element.set("Condition", stmt["condition"])
        for child in stmt["then"]:
            _statement_to_xml(element, child)
        if stmt["else"]:
            else_element = ET.SubElement(parent, "Else")
            for child in stmt["else"]:
                _statement_to_xml(else_element, child)
        return
    raise ValueError(f"Unsupported normalized statement kind: {stmt['kind']}")


def _fallback_c_to_il_xml(source: str) -> str:
    annotations = _parse_c_annotations(source)
    func_name, params_str, body = _extract_c_function(source)
    inputs, outputs = _parse_c_params(params_str, annotations)
    if not outputs:
        raise ValueError("Fallback C->IL requires at least one output parameter")

    lines = _prepare_c_body_lines(body)
    statements, next_index = _parse_c_block(lines, 0)
    if next_index != len(lines):
        raise ValueError("Fallback C->IL parser did not consume the full function body")
    normalized_statements = _normalize_c_statements(statements, outputs)

    state_name = f"{func_name}_state"
    root = ET.Element("Root", {"xmlns:xsi": "http://www.w3.org/2001/XMLSchema-instance"})
    root.set("xsi:noNamespaceSchemaLocation", "http://www.example.org/IRMetadata")

    config = ET.SubElement(root, "Config")
    ET.SubElement(config, "IterationCount", {"Value": "1"})
    ET.SubElement(config, "MainInitFunction", {"Name": f"{func_name}_Init"})
    ET.SubElement(config, "MainExecFunction", {"Name": func_name})
    ET.SubElement(config, "MainCompositeState", {"Name": state_name})

    file_element = ET.SubElement(root, "File", {"Name": func_name})
    ET.SubElement(file_element, "Struct", {"Name": state_name})
    ET.SubElement(file_element, "GlobalVariable", {"Name": f"{func_name}_instance", "Type": state_name})

    init_function = ET.SubElement(file_element, "Function", {"Name": f"{func_name}_Init", "Type": "FunctionInit", "ReturnType": "void"})
    ET.SubElement(init_function, "Ref", {"Actor": "CompositeActor", "Path": func_name})
    ET.SubElement(init_function, "Input", {"Name": "self", "Type": state_name, "IsAddress": "1"})
    ET.SubElement(init_function, "Schedule")

    exec_function = ET.SubElement(file_element, "Function", {"Name": func_name, "Type": "FunctionExec", "ReturnType": "void"})
    ET.SubElement(exec_function, "Ref", {"Actor": "CompositeActor", "Path": func_name})
    for item in inputs:
        ET.SubElement(exec_function, "Input", {"Name": item["name"], "Type": item["il_type"]})
    for item in outputs:
        ET.SubElement(exec_function, "Output", {"Name": item["name"], "Type": item["il_type"]})

    schedule = ET.SubElement(exec_function, "Schedule")
    calculate = ET.SubElement(schedule, "Calculate", {"Name": f"{func_name}_body"})
    ET.SubElement(calculate, "Ref", {"Actor": "CompositeActor", "Path": f"{func_name}.body"})
    for item in inputs:
        ET.SubElement(calculate, "Input", {"Name": item["name"], "Type": item["il_type"], "Source": item["name"]})
    for item in outputs:
        ET.SubElement(calculate, "Output", {"Name": item["name"], "Type": item["il_type"]})
    for stmt in normalized_statements:
        _statement_to_xml(calculate, stmt)

    tree = ET.ElementTree(root)
    try:
        ET.indent(tree, space="    ")
    except AttributeError:
        pass
    return ET.tostring(root, encoding="unicode")


def _run_codegen_step(
    name: str,
    cmd: list[str],
    success_artifact: Path | None = None,
    success_markers: list[str] | None = None,
    cwd: Path | None = None,
) -> CodegenStepResult:
    proc = subprocess.run(cmd, capture_output=True, text=True, cwd=str(cwd) if cwd else None)
    result = CodegenStepResult(
        name=name,
        command=" ".join(cmd),
        returncode=proc.returncode,
        stdout=proc.stdout or "",
        stderr=proc.stderr or "",
    )
    if proc.returncode != 0:
        marker_ok = True
        if success_markers:
            marker_ok = any(marker in result.stdout for marker in success_markers)
        artifact_ok = True
        if success_artifact is not None:
            artifact_ok = _has_artifact(success_artifact)
        if marker_ok and artifact_ok:
            if result.stderr:
                result.stderr += "\n"
            result.stderr += (
                f"[WARN] Non-zero return code ({proc.returncode}) ignored for {name} "
                "because success markers and artifacts are present."
            )
            return result
        raise HTTPException(
            status_code=500,
            detail={
                "failed_step": name,
                "command": result.command,
                "returncode": proc.returncode,
                "stdout": result.stdout[-6000:],
                "stderr": result.stderr[-6000:],
            },
        )
    return result


def _find_missing_actor_templates(mir_path: Path, codegen_root: Path) -> list[dict]:
    template_root = codegen_root / "Exe" / "ILGenerator" / "ILTemplateDataflow"
    if not mir_path.exists() or not template_root.exists():
        return []
    try:
        tree = ET.parse(mir_path)
        root = tree.getroot()
    except Exception:
        return []

    actor_types: set[str] = set()
    for actor in root.findall(".//Actor"):
        actor_type = (actor.get("Type") or "").strip()
        if actor_type:
            actor_types.add(actor_type)

    missing: list[dict] = []
    for actor_type in sorted(actor_types):
        actor_dir = template_root / actor_type
        if not actor_dir.exists() or not actor_dir.is_dir():
            continue
        actor_dll = actor_dir / f"{actor_type}.dll"
        if not actor_dll.exists():
            missing.append(
                {
                    "actor_type": actor_type,
                    "expected_dll": str(actor_dll),
                }
            )
    return missing


def _extract_interface_names_from_step1(step1_sysml2: str) -> dict[str, set[str]]:
    text = step1_sysml2 or ""
    inputs: set[str] = set()
    outputs: set[str] = set()

    patterns_in = [
        re.compile(r"\bin\s+([A-Za-z_][A-Za-z0-9_]*)\s*:", re.IGNORECASE),
        re.compile(r"\bInput\s*:\s*([A-Za-z_][A-Za-z0-9_]*)", re.IGNORECASE),
    ]
    patterns_out = [
        re.compile(r"\bout\s+([A-Za-z_][A-Za-z0-9_]*)\s*:", re.IGNORECASE),
        re.compile(r"\bOutput\s*:\s*([A-Za-z_][A-Za-z0-9_]*)", re.IGNORECASE),
    ]

    for p in patterns_in:
        inputs.update(m.group(1) for m in p.finditer(text))
    for p in patterns_out:
        outputs.update(m.group(1) for m in p.finditer(text))
    return {"inputs": inputs, "outputs": outputs}


def _validate_step1_bundle(step1_sysml2: str | None, step1_req_test_map: dict | None, *, compiled: dict | None = None) -> list[ValidationIssue]:
    issues: list[ValidationIssue] = []
    if step1_sysml2 is None:
        return issues
    if not isinstance(step1_sysml2, str) or not step1_sysml2.strip():
        issues.append(ValidationIssue(code="STEP1_EMPTY_SYSML2", message="Step1 system_sysml2 is empty", path="system_sysml2"))
    else:
        try:
            checked = validate_executable_sysml(step1_sysml2)
        except SysMLToolError as exc:
            raise HTTPException(status_code=503, detail=str(exc)) from exc
        if not checked['ok']:
            issues.extend(ValidationIssue(code=i.get('code') or 'SYSML_INVALID', message=i['message'],
                                          severity=i.get('severity', 'error')) for i in checked['issues'])
        elif compiled is not None:
            compiled['ir'] = checked['ir']
    if step1_req_test_map is not None:
        if not isinstance(step1_req_test_map, dict):
            issues.append(ValidationIssue(code="STEP1_REQMAP_TYPE", message="Step1 req_test_map must be a JSON object", path="req_test_map"))
        else:
            rows = step1_req_test_map.get("requirements_to_testpoints", step1_req_test_map.get("requirements"))
            if not isinstance(rows, list) or len(rows) == 0:
                issues.append(
                    ValidationIssue(
                        code="STEP1_REQMAP_EMPTY",
                        message="Step1 req_test_map must contain non-empty requirements_to_testpoints",
                        path="req_test_map.requirements_to_testpoints",
                    )
                )
    return issues


def _validate_subsystem_ir(ir: dict, step1_sysml2: str | None = None, contract: dict | None = None, *, source_ir: dict | None = None) -> list[ValidationIssue]:
    # Check the canonical compiler artifact when a source is supplied; do not
    # infer correspondence from matching port names or source keywords.
    try:
        normalized = validate_ir(ir)
        if step1_sysml2 is not None:
            expected = validate_ir(source_ir if source_ir is not None else compile_sysml(step1_sysml2))
            if normalized != expected:
                return [ValidationIssue(code="IR_SOURCE_MISMATCH", path="$",
                    message="IR is structurally valid but differs from the canonical IR compiled from the supplied SysML. Rebuild with /compile/sysml2; this checks compiler-artifact correspondence, not equivalence of arbitrary rewritten graphs.")]
        if contract is not None:
            from app.services.behavior_contract import check_contract
            coverage = check_contract(normalized, contract)
            if coverage['issues']:
                return [ValidationIssue(code=i['code'], message=i['message']) for i in coverage['issues']]
    except ValueError as exc:
        return [ValidationIssue(code="IR_CONTRACT", message=str(exc), path="$")]
    return []


def _issues_to_feedback_lines(issues: list[ValidationIssue], limit: int = 20) -> list[str]:
    lines: list[str] = []
    for issue in issues[:limit]:
        where = f" ({issue.path})" if issue.path else ""
        lines.append(f"[{issue.code}]{where}: {issue.message}")
    if len(issues) > limit:
        lines.append(f"... and {len(issues) - limit} more issues")
    return lines


def _try_parse_json_object(text: str) -> dict:
    candidates: list[str] = [text.strip()]
    fenced = re.findall(r"```(?:json)?\s*([\s\S]*?)\s*```", text, re.IGNORECASE)
    candidates.extend(chunk.strip() for chunk in fenced if chunk.strip())
    start = text.find("{")
    end = text.rfind("}")
    if start >= 0 and end > start:
        candidates.append(text[start : end + 1].strip())

    for candidate in candidates:
        try:
            obj = json.loads(candidate)
            if isinstance(obj, dict):
                return obj
        except Exception:
            continue
    raise ValueError("top-level JSON object not found")


def _slug_module_id(text: str, index: int) -> str:
    lowered = re.sub(r"[^a-zA-Z0-9\u4e00-\u9fff_-]+", "_", text.strip().lower())
    lowered = re.sub(r"_+", "_", lowered).strip("_")
    if not lowered:
        return f"module_{index}"
    if lowered[0].isdigit():
        lowered = f"module_{lowered}"
    return lowered


def _infer_module_type(text: str) -> str:
    s = text.lower()
    if re.search(r"source|input|采集|输入|传感|mic|sensor", s):
        return "source"
    if re.search(r"analysis|analy|filter|预处理|分析|特征", s):
        return "analysis"
    if re.search(r"decision|rule|threshold|判定|阈值|策略", s):
        return "decision"
    if re.search(r"display|ui|render|show|显示|可视", s):
        return "display"
    if re.search(r"alert|alarm|buzzer|notify|告警|蜂鸣|通知", s):
        return "alert"
    return "custom"


def _infer_executor(module_type: str) -> str:
    if module_type == "source":
        return "data-source"
    if module_type == "analysis":
        return "analysis"
    if module_type == "decision":
        return "decision"
    if module_type == "display":
        return "display"
    if module_type == "alert":
        return "alert"
    return "generic"


def _fallback_module_blueprint(requirement: str, max_modules: int) -> dict:
    parts = [
        p.strip()
        for p in re.split(r"[\n,，。；;！？!?.]+", requirement)
        if isinstance(p, str) and p.strip()
    ]
    picked = parts[:max_modules]
    if not picked:
        picked = ["输入采集", "特征分析", "阈值判定", "显示输出", "告警输出"]

    modules = []
    for idx, title in enumerate(picked, start=1):
        module_type = _infer_module_type(title)
        modules.append(
            {
                "id": _slug_module_id(title, idx),
                "title": title,
                "subtitle": f"来自需求分解: {title}",
                "moduleType": module_type,
                "executor": _infer_executor(module_type),
                "codeSeed": f"# {title}\n// TODO: implement",
            }
        )

    connections = []
    for idx in range(len(modules) - 1):
        connections.append(
            {
                "from": modules[idx]["id"],
                "to": modules[idx + 1]["id"],
                "relation": "data_flow",
                "logic": "上一模块输出作为下一模块输入",
            }
        )

    return {"modules": modules, "connections": connections}


def _normalize_module_blueprint_obj(obj: dict, max_modules: int) -> dict:
    raw_modules = obj.get("modules") if isinstance(obj, dict) else None
    raw_connections = obj.get("connections") if isinstance(obj, dict) else None

    if not isinstance(raw_modules, list) or not raw_modules:
        raise ValueError("modules must be a non-empty array")

    modules = []
    seen: set[str] = set()
    for idx, item in enumerate(raw_modules[:max_modules], start=1):
        if not isinstance(item, dict):
            continue
        raw_title = str(item.get("title") or item.get("name") or f"module_{idx}").strip()
        raw_id = str(item.get("id") or _slug_module_id(raw_title, idx)).strip()
        module_id = _slug_module_id(raw_id, idx)
        if module_id in seen:
            continue
        seen.add(module_id)

        module_type = str(item.get("moduleType") or _infer_module_type(raw_title)).strip().lower()
        executor = str(item.get("executor") or _infer_executor(module_type)).strip().lower()
        code_seed = str(item.get("codeSeed") or f"# {raw_title}\n// TODO: implement").strip()

        modules.append(
            {
                "id": module_id,
                "title": raw_title,
                "subtitle": str(item.get("subtitle") or "由大模型规划生成").strip(),
                "moduleType": module_type,
                "executor": executor,
                "codeSeed": code_seed,
            }
        )

    if not modules:
        raise ValueError("no valid modules after normalization")

    module_ids = {m["id"] for m in modules}
    connections = []
    if isinstance(raw_connections, list):
        for edge in raw_connections:
            if not isinstance(edge, dict):
                continue
            frm = str(edge.get("from") or "").strip()
            to = str(edge.get("to") or "").strip()
            if frm in module_ids and to in module_ids and frm != to:
                relation = str(edge.get("relation") or edge.get("type") or "data_flow").strip() or "data_flow"
                logic = str(edge.get("logic") or edge.get("description") or "").strip()
                item = {"from": frm, "to": to, "relation": relation}
                if logic:
                    item["logic"] = logic
                connections.append(item)

    if not connections:
        for idx in range(len(modules) - 1):
            connections.append(
                {
                    "from": modules[idx]["id"],
                    "to": modules[idx + 1]["id"],
                    "relation": "data_flow",
                    "logic": "上一模块输出作为下一模块输入",
                }
            )

    return {"modules": modules, "connections": connections}


def _default_module_blueprint_prompt() -> str:
    prompt_path = Path("tools/prompts/module_blueprint.prompt.md")
    if prompt_path.exists() and prompt_path.is_file():
        try:
            return prompt_path.read_text(encoding="utf-8")
        except Exception:
            pass
    return (
        "你是嵌入式系统架构师。请根据用户需求规划前端模块蓝图。\n"
        "输出必须是JSON对象，包含modules和connections。\n"
        "connections中的每条边都要包含from,to,relation,logic。\n"
        "relation可选值: data_flow, control_flow, trigger, feedback, dependency。\n"
    )


def _call_llm(prompt: str, req: GenerateFromPromptRequest) -> tuple[str, str]:
    if not _llm_api_key:
        raise HTTPException(status_code=500, detail="OPENROUTER_API_KEY/OPENAI_API_KEY is not set")

    model = req.model or _default_model
    history_messages = _normalize_history_messages(req.history)
    params = {
        "model": model,
        "messages": [
            {"role": "system", "content": "You are an AI assistant specialized in embedded systems and code generation."},
            *history_messages,
            {"role": "user", "content": prompt}
        ],
        "temperature": req.temperature if req.temperature is not None else 0.2,
    }
    if req.max_tokens is not None:
        params["max_tokens"] = req.max_tokens

    params.update(request_options(prompt))

    try:
        response = client.chat.completions.create(**params)
    except Exception as exc:
        import traceback
        error_trace = traceback.format_exc()
        # 添加更详细的代理信息
        proxy_info = f"HTTP_PROXY: {os.getenv('HTTP_PROXY')}, HTTPS_PROXY: {os.getenv('HTTPS_PROXY')}"
        raise HTTPException(status_code=500, detail=f"LLM call failed: {type(exc).__name__}: {exc}\n{error_trace}\nProxy info: {proxy_info}")

    text = completion_text(response, prompt)
    return getattr(response, 'model', None) or model, text



@app.post("/chat", response_model=ChatResponse)
def chat(req: ChatRequest):
    if not _llm_api_key:
        raise HTTPException(status_code=500, detail="OPENROUTER_API_KEY/OPENAI_API_KEY is not set")

    model = req.model or _default_model

    params = {
        "model": model,
        "messages": [
            {"role": "system", "content": "You are an AI assistant specialized in embedded systems and code generation."},
            {"role": "user", "content": req.prompt}
        ],
        "temperature": req.temperature if req.temperature is not None else 0.2,
    }
    if req.max_tokens is not None:
        params["max_tokens"] = req.max_tokens

    try:
        response = client.chat.completions.create(**params)
    except Exception as exc:
        import traceback
        error_trace = traceback.format_exc()
        # 添加更详细的代理信息
        proxy_info = f"HTTP_PROXY: {os.getenv('HTTP_PROXY')}, HTTPS_PROXY: {os.getenv('HTTPS_PROXY')}"
        raise HTTPException(status_code=500, detail=f"LLM call failed: {type(exc).__name__}: {exc}\n{error_trace}\nProxy info: {proxy_info}")

    text = response.choices[0].message.content or ""

    try:
        os.makedirs(OUTPUT_DIR, exist_ok=True)
        with open(OUTPUT_PATH, "w", encoding="utf-8") as handle:
            handle.write(text)
    except Exception as exc:
        raise HTTPException(status_code=500, detail=f"Failed to write output file: {exc}")

    return ChatResponse(text=text, model=model)


@app.post("/convert/ir-to-tblink")
def convert_ir_to_tblink(req: ConvertIrToTbLinkRequest):
    try:
        from tools.ir_to_tblink import ir_to_tblink

        tree = ir_to_tblink(validate_ir(req.ir))
        buf = io.BytesIO()
        tree.write(buf, encoding="utf-8", xml_declaration=True)
        return Response(content=buf.getvalue(), media_type="application/xml")
    except Exception as exc:
        raise HTTPException(status_code=400, detail=str(exc))


@app.post("/convert/c-to-il", response_model=ConvertCToIlResponse)
def convert_c_to_il(req: ConvertCToIlRequest):
    c_file_path = _resolve_path(req.c_file_path)
    if req.c_code is not None:
        _write_text(str(c_file_path), req.c_code)
    if not c_file_path.exists() or not c_file_path.is_file():
        raise HTTPException(status_code=400, detail=f"C source file not found: {c_file_path}")
    c_source = c_file_path.read_text(encoding="utf-8", errors="ignore")

    out_il_path = _resolve_path(req.out_il_path)
    out_il_path.parent.mkdir(parents=True, exist_ok=True)
    if out_il_path.exists():
        out_il_path.unlink()

    exe_path, tried_candidates = _pick_code2script_exe(req)
    args: list[str] = []
    stdout = ""
    stderr = ""
    returncode = 0
    used_fallback = False

    if exe_path is not None:
        args = [
            str(exe_path),
            "-i",
            _arg_path_for_exe(c_file_path, exe_path),
            "-il",
            _arg_path_for_exe(out_il_path, exe_path),
        ]

        if req.out_script_path:
            out_script_path = _resolve_path(req.out_script_path)
            out_script_path.parent.mkdir(parents=True, exist_ok=True)
            model_path = (req.model_path or "").strip()
            if not model_path or model_path == "$$$CURRENT_MODEL_PATH$$$":
                model_path = "$$$CURRENT_MODEL_PATH$$$"
            model_path_arg = model_path
            if os.name != "nt" and exe_path.suffix.lower() == ".exe":
                model_path_path = Path(model_path)
                if model_path_path.is_absolute():
                    model_path_arg = _to_windows_arg_path(model_path_path)
            args.extend(["-o", _arg_path_for_exe(out_script_path, exe_path)])
            args.extend(["-p", model_path_arg])

        try:
            proc = subprocess.run(args, capture_output=True, text=True, cwd=str(exe_path.parent))
            stdout = proc.stdout or ""
            stderr = proc.stderr or ""
            returncode = proc.returncode
        except OSError as exc:
            stderr = (
                f"Failed to execute Code2Script: {type(exc).__name__}: {exc}. "
                f"Resolved executable: {exe_path}"
            )
            returncode = 1
    else:
        tried_display = ", ".join(str(p) for p in tried_candidates[:12]) if tried_candidates else "<none>"
        stderr = (
            "Code2Script executable not found. "
            f"Tried: {tried_display}. "
            "Using backend fallback converter."
        )
        args = ["[fallback] restricted-c-to-il"]

    il_exists = out_il_path.exists() and out_il_path.is_file() and out_il_path.stat().st_size > 0
    if not il_exists:
        try:
            il_xml = _fallback_c_to_il_xml(c_source)
            _write_text(str(out_il_path), il_xml)
            used_fallback = True
            fallback_note = "[INFO] Backend fallback converter generated IL XML."
            stderr = f"{stderr}\n{fallback_note}".strip()
            return ConvertCToIlResponse(
                out_il_path=str(out_il_path),
                il_xml=il_xml,
                command=" ".join(args),
                returncode=returncode,
                stdout=stdout,
                stderr=stderr,
            )
        except Exception as exc:
            detail = {
                "failed_step": "Code2Script",
                "message": f"IL XML not generated: {out_il_path}",
                "command": " ".join(args),
                "returncode": returncode,
                "stdout": stdout[-6000:],
                "stderr": f"{stderr}\nFallback converter failed: {type(exc).__name__}: {exc}".strip()[-6000:],
            }
            raise HTTPException(status_code=500, detail=detail)

    il_xml = out_il_path.read_text(encoding="utf-8", errors="ignore")
    if returncode != 0 and not used_fallback:
        warn = (
            f"[WARN] Non-zero return code ({returncode}) ignored for Code2Script "
            "because IL artifact was generated."
        )
        stderr = f"{stderr}\n{warn}".strip()
    return ConvertCToIlResponse(
        out_il_path=str(out_il_path),
        il_xml=il_xml,
        command=" ".join(args),
        returncode=returncode,
        stdout=stdout,
        stderr=stderr,
    )


@app.post("/convert/il-to-c", response_model=ConvertIlToCResponse)
def convert_il_to_c(req: ConvertIlToCRequest):
    il_path=_resolve_path(req.il_file_path)
    if not il_path.is_file():
        raise HTTPException(status_code=400,detail=f'IL XML not found: {il_path}')
    il_xml=il_path.read_text(encoding='utf-8')
    verified=req.provenance is not None or req.interface_contract is not None
    root=_resolve_codegen_root_with_fallback(req.codegen_root)
    try:
        if verified:
            if req.interface_contract is None:
                raise ProvenanceError('Pass the frozen interface_contract with model provenance.')
            validate_sysml_contract(req.interface_contract)
            AUTHORITY.verify(req.provenance,'sysml_ir',dict(
                contract_sha256=contract_digest(req.interface_contract),il_sha256=text_digest(il_xml)))
            if root.resolve()!=_resolve_codegen_root_with_fallback(None).resolve():
                raise ProvenanceError('Verified compilation requires the server-configured code generator.')
            if req.optimize or req.target!='Intel':
                raise ProvenanceError('Verified model compilation currently requires target=Intel and optimize=false.')
    except ValueError as exc:
        raise HTTPException(status_code=422,detail=str(exc)) from exc
    backend=root/'Exe/ILTranslator/ILTranslator.exe'
    backend_digest=hashlib.sha256(backend.read_bytes()).hexdigest() if backend.is_file() else None
    # 校验过的同一份文本写入私有目录，后端不再读取可被其他请求覆盖的原路径。
    with tempfile.TemporaryDirectory(prefix='model-codegen-') as directory:
        private=Path(directory)
        (private/'model.il.xml').write_text(il_xml,encoding='utf-8')
        isolated=req.model_copy(update={'il_file_path':str(private/'model.il.xml'),
            'out_c_dir':str(private/'c'),'out_il_optimized_path':str(private/'optimized.xml'),
            'codegen_root':str(root)})
        generated=_convert_il_to_c_unverified(isolated)
        contents=generated.contents or {}
        if verified:
            if not contents.get('define.h') or not contents.get('ProgramPage1.c'):
                raise HTTPException(status_code=422,detail='Backend did not produce both model header and source.')
            if hashlib.sha256(backend.read_bytes()).hexdigest()!=backend_digest:
                raise HTTPException(status_code=422,detail='Code generator changed during compilation.')
            generated.provenance=AUTHORITY.extend(req.provenance,'sysml_ir','kernel',
                header_sha256=text_digest(contents['define.h']),kernel_source_sha256=text_digest(contents['ProgramPage1.c']),
                backend_sha256=backend_digest,backend_target=req.target)
            generated.provenance_status='verified_model_chain'
            generated.contract_sha256=contract_digest(req.interface_contract)
        output=_resolve_path(req.out_c_dir)
        output.mkdir(parents=True,exist_ok=True)
        for name in _safe_list_files(str(private/'c')):
            # 保留后端附属头文件；不递归删除调用方目录或无关文件。
            source=private/'c'/name
            target=output/name;target.parent.mkdir(parents=True,exist_ok=True)
            shutil.copyfile(source,target)
        optimized=''
        if req.optimize:
            opt=_resolve_path(req.out_il_optimized_path or str(il_path.parent/f'{il_path.stem}_Optimized.xml'))
            opt.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(private/'optimized.xml',opt)
            optimized=str(opt)
        generated.files={'il':str(il_path),'il_optimized':optimized,'c_output_dir':str(output)}
        return generated


def _convert_il_to_c_unverified(req: ConvertIlToCRequest):
    codegen_root = _resolve_codegen_root_with_fallback(req.codegen_root)

    ilopt_exe = codegen_root / "Exe" / "ILOptimizer" / "ILOptimizer.exe"
    iltranslator_exe = codegen_root / "Exe" / "ILTranslator" / "ILTranslator.exe"

    required = [iltranslator_exe]
    if req.optimize:
        required.append(ilopt_exe)
    missing = [str(p) for p in required if not p.exists()]
    if missing:
        raise HTTPException(status_code=400, detail={"message": "Missing executable(s)", "paths": missing})

    il_path = _resolve_path(req.il_file_path)
    if not il_path.exists() or not il_path.is_file():
        raise HTTPException(status_code=400, detail=f"IL XML not found: {il_path}")

    stem = il_path.stem
    il_opt_path = _resolve_path(req.out_il_optimized_path or str(il_path.parent / f"{stem}_Optimized.xml"))
    c_out_dir = _resolve_path(req.out_c_dir)
    if il_opt_path.exists():
        il_opt_path.unlink()
    if c_out_dir.exists():
        shutil.rmtree(c_out_dir)
    c_out_dir.mkdir(parents=True, exist_ok=True)

    il_arg = _to_windows_arg_path(il_path)
    il_opt_arg = _to_windows_arg_path(il_opt_path)
    c_out_arg = _to_windows_arg_path(c_out_dir)

    steps: list[CodegenStepResult] = []
    translator_input = il_arg
    if req.optimize:
        steps.append(
            _run_codegen_step(
                "ILOptimizer",
                [
                    str(ilopt_exe),
                    "-i",
                    il_arg,
                    "-o",
                    il_opt_arg,
                    "-t",
                    req.target,
                    "-v",
                    "true",
                    "-e",
                    "true",
                    "-c",
                    "true",
                ],
                success_artifact=il_opt_path,
                cwd=ilopt_exe.parent,
            )
        )
        translator_input = il_opt_arg

    steps.append(
        _run_codegen_step(
            "ILTranslator",
            [
                str(iltranslator_exe),
                "-l",
                "c",
                "-i",
                translator_input,
                "-o",
                c_out_arg,
                "-d",
                req.target,
                "-c",
                "true",
            ],
            success_artifact=c_out_dir,
            cwd=iltranslator_exe.parent,
        )
    )

    contents: dict[str, str] = {}
    for rel in _safe_list_files(str(c_out_dir)):
        if rel not in {"define.h", "main.c", "ProgramPage1.c"}:
            continue
        try:
            contents[rel] = _safe_read_under(str(c_out_dir), rel)
        except HTTPException:
            continue

    return ConvertIlToCResponse(
        files={
            "il": str(il_path),
            "il_optimized": str(il_opt_path) if req.optimize else "",
            "c_output_dir": str(c_out_dir),
        },
        steps=steps,
        contents=contents or None,
    )


@app.post("/pipeline/codegen", response_model=RunCodegenPipelineResponse)
def run_codegen_pipeline(req: RunCodegenPipelineRequest):
    codegen_root = _resolve_codegen_root_with_fallback(req.codegen_root)

    parser_exe = codegen_root / "Exe" / "ParserForTBLink" / "ParserForTBLink.exe"
    ilgen_exe = codegen_root / "Exe" / "ILGenerator" / "ILGenerator.exe"
    ilopt_exe = codegen_root / "Exe" / "ILOptimizer" / "ILOptimizer.exe"
    iltranslator_exe = codegen_root / "Exe" / "ILTranslator" / "ILTranslator.exe"

    required = [parser_exe, ilgen_exe, iltranslator_exe]
    if req.optimize:
        required.append(ilopt_exe)
    missing = [str(p) for p in required if not p.exists()]
    if missing:
        raise HTTPException(status_code=400, detail={"message": "Missing executable(s)", "paths": missing})

    tblink_path = _resolve_path(req.out_tblink_path)
    if req.tblink_xml and req.tblink_xml.strip():
        _write_text(str(tblink_path), req.tblink_xml)
    elif not tblink_path.exists():
        raise HTTPException(status_code=400, detail=f"TBLink XML not found and no tblink_xml provided: {tblink_path}")

    out_dir = tblink_path.parent
    stem = tblink_path.stem
    mir_path = _resolve_path(req.out_mir_path or str(out_dir / f"{stem}_MIR.xml"))
    il_path = _resolve_path(req.out_il_path or str(out_dir / f"{stem}_IL.xml"))
    il_opt_path = _resolve_path(req.out_il_optimized_path or str(out_dir / f"{stem}_IL_Optimized.xml"))
    c_out_dir = _resolve_path(req.out_c_dir)
    c_out_dir.mkdir(parents=True, exist_ok=True)

    tblink_arg = _to_windows_arg_path(tblink_path)
    mir_arg = _to_windows_arg_path(mir_path)
    il_arg = _to_windows_arg_path(il_path)
    il_opt_arg = _to_windows_arg_path(il_opt_path)
    c_out_arg = _to_windows_arg_path(c_out_dir)

    steps: list[CodegenStepResult] = []
    steps.append(
        _run_codegen_step(
            "ParserForTBLink",
            [str(parser_exe), "-i", tblink_arg, "-o", mir_arg],
            success_artifact=mir_path,
            cwd=parser_exe.parent,
        )
    )

    missing_templates = _find_missing_actor_templates(mir_path, codegen_root)
    if missing_templates:
        raise HTTPException(
            status_code=500,
            detail={
                "failed_step": "ILGenerator",
                "message": "Missing actor template DLL(s) required by MIR",
                "missing_templates": missing_templates,
                "hint": "Build missing template DLLs under Exe/ILGenerator/ILTemplateDataflow/<ActorType>/<ActorType>.dll",
            },
        )

    steps.append(
        _run_codegen_step(
            "ILGenerator",
            [str(ilgen_exe), "-i", mir_arg, "-o", il_arg, "-t", "true"],
            success_artifact=il_path,
            success_markers=["Done!"],
            cwd=ilgen_exe.parent,
        )
    )

    translator_input = il_arg
    if req.optimize:
        steps.append(
            _run_codegen_step(
                "ILOptimizer",
                [
                    str(ilopt_exe),
                    "-i",
                    il_arg,
                    "-o",
                    il_opt_arg,
                    "-t",
                    req.target,
                    "-v",
                    "true",
                    "-e",
                    "true",
                    "-c",
                    "true",
                ],
                success_artifact=il_opt_path,
                cwd=ilopt_exe.parent,
            )
        )
        translator_input = il_opt_arg

    steps.append(
        _run_codegen_step(
            "ILTranslator",
            [
                str(iltranslator_exe),
                "-l",
                "c",
                "-i",
                translator_input,
                "-o",
                c_out_arg,
                "-d",
                req.target,
                "-c",
                "true",
            ],
            success_artifact=c_out_dir,
            cwd=iltranslator_exe.parent,
        )
    )

    return RunCodegenPipelineResponse(
        files={
            "tblink": str(tblink_path),
            "mir": str(mir_path),
            "il": str(il_path),
            "il_optimized": str(il_opt_path) if req.optimize else "",
            "c_output_dir": str(c_out_dir),
        },
        steps=steps,
    )


@app.post("/validate/sysml2")
def validate_sysml2_model(req: CompileSysmlRequest):
    try:
        result = validate_sysml(req.system_sysml2)
    except SysMLToolError as exc:
        raise HTTPException(status_code=503, detail=str(exc)) from exc
    return {key: result[key] for key in ("ok", "issues", "validator_version")}


@app.post("/compile/sysml2", response_model=GenerateResponse)
def compile_sysml2_model(req: CompileSysmlRequest):
    try:
        if req.interface_contract is not None:
            validate_sysml_contract(req.interface_contract)
        result = validate_sysml(req.system_sysml2)
        if not result["ok"]:
            raise HTTPException(status_code=422, detail={"issues": result["issues"]})
        ir = compile_sysml(req.system_sysml2, root=req.root, validated=result)
        from app.services.behavior_contract import check_contract
        coverage = check_contract(ir, req.interface_contract)
        if coverage['status'] == 'failed':
            raise HTTPException(status_code=422, detail=coverage)
        il_xml = ir_to_il(ir)
    except SysMLToolError as exc:
        raise HTTPException(status_code=503, detail=str(exc)) from exc
    except SysMLCompileError as exc:
        raise HTTPException(status_code=422, detail={"issues": [exc.issue]}) from exc
    except ValueError as exc:
        raise HTTPException(status_code=422, detail=str(exc)) from exc
    from tools.ir_to_tblink import ir_to_tblink
    out = _resolve_path(req.out_dir)
    out.mkdir(parents=True, exist_ok=True)
    paths = {"subsystem_ir": str(out / "subsystem_ir.json"), "il": str(out / "model.il.xml"),
             "tblink": str(out / "model.tblink.xml")}
    _write_text(paths["subsystem_ir"], json.dumps(ir, ensure_ascii=False, indent=2))
    _write_text(paths["il"], il_xml)
    if not ir.get("states") and not ir.get("updates"):
        ir_to_tblink(ir).write(paths["tblink"], encoding="utf-8", xml_declaration=True)
    else:
        Path(paths.pop("tblink")).unlink(missing_ok=True)  # Do not leave a stale stateless TBLink model.
    return GenerateResponse(model="deterministic-sysml-compiler", files=paths,
                            contents={"subsystem_ir": ir, "il_xml": il_xml,
                                      "provenance": issue_model(req.system_sysml2,ir,req.interface_contract,il_xml,result['validator_version']) if req.interface_contract is not None else None,
                                      "coverage": coverage, "interface_contract": req.interface_contract,
                                      "contract_sha256": contract_digest(req.interface_contract) if req.interface_contract else None,
                                      "execution": ir.get("execution", "combinational")})


@app.post("/generate/interface-contract")
def prepare_interface_contract(req: PlanInterfaceContractRequest):
    from uuid import uuid4
    from app.services.interface_registry import register_contract, InterfaceCapabilityError
    from app.services.interface_planner import prepare_interface_contract as prepare_proposal
    report_path = str(_resolve_path(req.out_path).with_suffix('.planning.json'))
    try:
        if req.interface_contract is not None:
            from app.services.configuration_policy import resolve_configuration
            resolution = resolve_configuration(req.requirement, req.pin_definitions, board=req.board)
            effective_requirement = resolution['effective_requirement']
            contract, construction = prepare_proposal(req.interface_contract, effective_requirement,
                resolution['pins'], target_board=req.board, resolution=resolution)
            if 'requirements' not in contract:
                contract['requirements'] = [dict(id='R1', text=req.requirement, assertion_ids=['source_requirement_R1'])]
            contract = validate_hardware_contract(contract)
            validate_sysml_contract(contract)
            check_explicit_requirements(contract, effective_requirement, resolution['pins'], target_board=req.board)
            if not any(p['direction']=='output' for p in contract['interfaces']):
                raise InterfaceCapabilityError('No executable model output registered; supply the required output or report the hardware capability gap')
            contract = add_public_behavior_checks(contract, effective_requirement)
            from app.services.interface_semantics import freeze_model_semantics
            contract = freeze_model_semantics(contract)
            contract = register_contract(contract)
            validate_sysml_contract(contract)
            report = {'contract_sha256': contract_digest(contract), 'provenance': 'supplied',
                      'functional_status': 'unverified', 'attempts': [], 'construction': construction,
                      'configuration_resolution': resolution,
                      'interface_registry': contract['interface_registry'], 'registered_relationships': 'verified'}
        else:
            contract, report = plan_interface_contract(lambda prompt: _call_llm(prompt, req),
                req.requirement, req.pin_definitions, previous=req.previous_contract,
                max_repairs=req.max_repair_attempts, target_board=req.board)
    except ContractPlanningError as exc:
        _write_text(report_path, json.dumps({'ok': False, 'blocking_category': exc.blocking_category,
                                           'attempts': exc.attempts}, ensure_ascii=False, indent=2))
        raise HTTPException(status_code=422, detail={'message': str(exc), 'blocking_category': exc.blocking_category,
                                                   'attempts': exc.attempts}) from exc
    except InterfaceCapabilityError as exc:
        detail = {'message': str(exc), 'blocking_category': 'hardware_capability', 'attempts': []}
        _write_text(report_path, json.dumps({'ok': False, **detail}, ensure_ascii=False, indent=2))
        raise HTTPException(status_code=422, detail=detail) from exc
    except ValueError as exc:
        raise HTTPException(status_code=422, detail=str(exc)) from exc
    registry_dir = _resolve_path(req.out_path).parent / 'interface_registries' / uuid4().hex
    registry_path = registry_dir / 'interface_registry.json'
    serial_path = registry_dir / 'serial_registry.json'
    _write_text(str(registry_path), json.dumps(contract['interface_registry'], ensure_ascii=False, indent=2))
    _write_text(str(serial_path), json.dumps({'version': 1, 'serial_channels': contract['interface_registry']['serial_channels'],
        'serial_inputs': contract['interface_registry'].get('serial_inputs', []),
        'serial_text_outputs': contract['interface_registry'].get('serial_text_outputs', [])}, ensure_ascii=False, indent=2))
    report['registry_paths'] = {'interface_registry': str(registry_path), 'serial_registry': str(serial_path)}
    _write_text(req.out_path, json.dumps(contract, ensure_ascii=False, indent=2))
    _write_text(report_path, json.dumps({'ok': True, 'original_requirement': req.requirement, **report}, ensure_ascii=False, indent=2))
    return {'interface_contract': contract, **report}


@app.post("/generate/sysml2-bundle", response_model=GenerateResponse)
def generate_sysml2_bundle(req: GenerateSysml2BundleRequest):
    base_prompt = _load_prompt(req)
    report_path = str(_resolve_path(req.out_sysml2_path).with_suffix(".validation.json"))
    review_req = req.model_copy(update={"model": req.semantic_review_model or req.model or _default_model,
                                        "history": None})
    review_call = (lambda prompt: _call_llm(prompt, review_req)) if req.semantic_review_enabled else None
    try:
        model, obj, report = generate_validated_bundle(
            lambda prompt: _call_llm(prompt, req),
            sysml_bundle_prompt(base_prompt, contract=req.interface_contract),
            max_repairs=req.max_repair_attempts,
            validator=validate_executable_sysml,
            contract=req.interface_contract,
            semantic_review=review_call,
            original_requirement=base_prompt if req.semantic_review_enabled else None,
            max_semantic_review_calls=req.max_semantic_review_calls,
        )
    except SysMLToolError as exc:
        raise HTTPException(status_code=503, detail=str(exc)) from exc
    except SysMLValidationError as exc:
        failure_report = {"ok": False, "attempts": exc.attempts,
                          "semantic_review": getattr(exc, 'semantic_review', {
                              "enabled": req.semantic_review_enabled, "final_status": "not_enabled"}),
                          "final_status": getattr(exc, 'final_status', 'validation_failed'),
                          "call_counts": getattr(exc, 'call_counts', None),
                          "usage": getattr(exc, 'usage', None)}
        _write_text(report_path, json.dumps(failure_report, ensure_ascii=False, indent=2))
        raise HTTPException(status_code=422, detail={"message": str(exc), "attempts": exc.attempts,
                                                   "semantic_review": failure_report["semantic_review"],
                                                   "final_status": failure_report["final_status"],
                                                   "validation_report": report_path}) from exc
    except ValueError as exc:
        raise HTTPException(status_code=422, detail=str(exc)) from exc
    system_sysml2 = obj["system_sysml2"]
    req_test_map = obj["req_test_map"]
    if req.interface_contract is not None:
        req_test_map['interface_contract'] = req.interface_contract
    _write_text(report_path, json.dumps({"ok": True, **report}, ensure_ascii=False, indent=2))

    _write_text(req.out_sysml2_path, system_sysml2)
    os.makedirs(os.path.dirname(req.out_req_test_map_path) or ".", exist_ok=True)
    with open(req.out_req_test_map_path, "w", encoding="utf-8") as handle:
        json.dump(req_test_map, handle, ensure_ascii=False, indent=2)

    return GenerateResponse(
        model=model,
        files={"system_sysml2": req.out_sysml2_path, "req_test_map": req.out_req_test_map_path,
               "validation_report": report_path},
        contents={"system_sysml2": system_sysml2, "req_test_map": req_test_map,
                  "interface_contract": req.interface_contract, "coverage": report['coverage'],
                  "semantic_review": report['semantic_review'], "final_status": report['final_status'],
                  "call_counts": report['call_counts'], "usage": report['usage'],
                  "contract_sha256": contract_digest(req.interface_contract) if req.interface_contract else None,
                  "validation": {"ok": True, "attempts": len(report["attempts"]), "validator_version": report["validator_version"],
                                 "repairs": len(report["attempts"]) - 1,
                                 "diagnostics": [{"attempt": a["attempt"], "ok": a["ok"], "issues": a["issues"]} for a in report["attempts"]]}},
    )


@app.post("/generate/subsystem-ir", response_model=GenerateResponse)
def generate_subsystem_ir(req: GenerateSubsystemIrRequest):
    if req.step1_sysml2 is not None:
        result = generate_subsystem_ir_iterative(GenerateSubsystemIrIterativeRequest(**req.model_dump()))
        if not result.ok:
            raise HTTPException(status_code=422, detail=[issue.model_dump() for issue in result.issues])
        return GenerateResponse(model=result.model, files=result.files, contents=result.contents)
    base_prompt = _load_prompt(req)
    if req.interface_contract is not None:
        base_prompt += "\nFrozen interface contract:\n" + json.dumps(req.interface_contract, ensure_ascii=False)
        from app.services.interface_semantics import model_boundary_prompt
        base_prompt += model_boundary_prompt(req.interface_contract)
    wrapper = (
        "You MUST output ONLY valid JSON (no Markdown, no code fences, no extra text).\n"
        "Return an executable scalar IR v2 JSON object.\n\n"
    )
    model, text = _call_llm(json_prompt(IR_V2_PROMPT + wrapper + base_prompt), req)

    try:
        check_completion(text)
        obj = json.loads(text)
    except Exception as exc:
        os.makedirs(OUTPUT_DIR, exist_ok=True)
        debug_path = os.path.join(OUTPUT_DIR, "last_subsystem_ir_raw.txt")
        _write_text(debug_path, text)
        raise HTTPException(
            status_code=422,
            detail=f"Model output is not valid JSON subsystem IR: {exc}. Raw output saved to {debug_path}",
        )

    if not isinstance(obj, dict):
        raise HTTPException(status_code=422, detail="subsystem_ir must be a JSON object at top-level")

    try:
        issues = _validate_subsystem_ir(obj, contract=req.interface_contract)
        if issues:
            raise ValueError('; '.join(i.message for i in issues))
        obj = validate_ir(obj)
        ir_to_il(obj)
    except ValueError as exc:
        raise HTTPException(status_code=422, detail=str(exc)) from exc

    try:
        os.makedirs(os.path.dirname(req.out_ir_path) or ".", exist_ok=True)
        with open(req.out_ir_path, "w", encoding="utf-8") as handle:
            json.dump(obj, handle, ensure_ascii=False, indent=2)
    except Exception as exc:
        raise HTTPException(status_code=500, detail=f"Failed to write subsystem_ir file: {exc}")

    return GenerateResponse(
        model=model,
        files={"subsystem_ir": req.out_ir_path},
        contents={"subsystem_ir": obj, "ir_origin": "llm_ir",
                  "interface_contract": req.interface_contract,
                  "contract_sha256": contract_digest(req.interface_contract) if req.interface_contract else None},
    )


@app.post("/generate/module-blueprint", response_model=GenerateModuleBlueprintResponse)
def generate_module_blueprint(req: GenerateModuleBlueprintRequest):
    if req.prompt and req.prompt.strip():
        user_prompt = req.prompt.strip()
    else:
        user_prompt = _default_module_blueprint_prompt().strip()

    existing = req.existing_blueprint if isinstance(req.existing_blueprint, dict) else {}
    existing_text = json.dumps(existing, ensure_ascii=False, indent=2)

    wrapper = (
        "You MUST output ONLY valid JSON (no markdown, no code fences, no extra text).\n"
        "Return a JSON object with exactly two keys: modules and connections.\n"
        "modules: array of objects, each object must include id,title,subtitle,moduleType,executor,codeSeed.\n"
        "connections: array of objects, each object must include from,to,relation and may include logic.\n"
        "Use moduleType from: source, analysis, decision, display, alert, custom.\n"
        "Use executor from: data-source, analysis, decision, display, alert, generic.\n"
        "Use relation from: data_flow, control_flow, trigger, feedback, dependency.\n"
        f"Limit module count to <= {req.max_modules}.\n"
    )

    full_prompt = (
        f"{wrapper}\n"
        f"[Planning instruction]\n{user_prompt}\n\n"
        f"[Requirement]\n{req.requirement}\n\n"
        f"[Existing blueprint]\n{existing_text}\n"
    )

    model, text = _call_llm(full_prompt, req)

    try:
        obj = _try_parse_json_object(text)
        if isinstance(obj.get("blueprint"), dict):
            obj = obj["blueprint"]
        blueprint = _normalize_module_blueprint_obj(obj, req.max_modules)
    except Exception:
        blueprint = _fallback_module_blueprint(req.requirement, req.max_modules)

    return GenerateModuleBlueprintResponse(model=model, blueprint=blueprint)


@app.post("/generate/c-from-step1", response_model=GenerateResponse)
def generate_c_from_step1(req: GenerateCFromStep1Request):
    """Compatibility endpoint: compile the model, never ask an LLM to reinterpret it."""
    model_text = req.step1_sysml2
    if not model_text:
        path = _resolve_path(req.step1_sysml2_path)
        if not path.is_file():
            raise HTTPException(status_code=400, detail=f"SysML model not found: {path}")
        model_text = path.read_text(encoding="utf-8-sig")
    out = _resolve_path(req.out_dir)
    compiled = compile_sysml2_model(CompileSysmlRequest(system_sysml2=model_text, interface_contract=req.interface_contract,
                                   out_dir=str(out.parent / (out.name + "_ir"))))
    generated = convert_il_to_c(ConvertIlToCRequest(il_file_path=compiled.files["il"], out_c_dir=str(out),
        interface_contract=req.interface_contract,provenance=compiled.contents.get('provenance')))
    header = generated.contents.get("define.h", "")
    source = generated.contents.get("ProgramPage1.c", "")
    if not header or not source:
        raise HTTPException(status_code=500, detail="ILTranslator did not produce the model header and source.")
    header_path = _resolve_path(req.out_header_path) if req.out_header_path else out / "define.h"
    source_path = _resolve_path(req.out_source_path) if req.out_source_path else out / "ProgramPage1.c"
    source = source.replace('#include "define.h"', '#include "' + header_path.name + '"')
    _write_text(str(header_path), header)
    _write_text(str(source_path), source)
    return GenerateResponse(model="deterministic-sysml-compiler",
        files={"generated_model_h": str(header_path), "generated_model_c": str(source_path),
               "noise_monitor_h": str(header_path), "noise_monitor_c": str(source_path), "c_output_dir": str(out),
               "il": compiled.files["il"]},
        contents={"generated_model_h": header, "generated_model_c": source,
                  "compiled_kernel": {"header":header,"source":generated.contents['ProgramPage1.c']},
                  "provenance": generated.provenance, "provenance_status": generated.provenance_status,
                  "contract_sha256": generated.contract_sha256,
                  "interface_contract": req.interface_contract, "coverage": compiled.contents['coverage']})


@app.post("/validate/subsystem-ir", response_model=ValidateSubsystemIrResponse)
def validate_subsystem_ir(req: ValidateSubsystemIrRequest):
    compiled = {}
    step1_issues = _validate_step1_bundle(req.step1_sysml2, req.step1_req_test_map, compiled=compiled)
    if step1_issues:
        return ValidateSubsystemIrResponse(
            ok=False,
            feedback_target="step1",
            issues=step1_issues,
            feedback=_issues_to_feedback_lines(step1_issues),
        )

    ir_issues = _validate_subsystem_ir(req.ir, req.step1_sysml2, req.interface_contract, source_ir=compiled.get('ir'))
    return ValidateSubsystemIrResponse(
        ok=(len(ir_issues) == 0),
        feedback_target="none" if len(ir_issues) == 0 else "step2",
        issues=ir_issues,
        feedback=_issues_to_feedback_lines(ir_issues),
    )


@app.post("/generate/subsystem-ir-iterative", response_model=GenerateSubsystemIrIterativeResponse)
def generate_subsystem_ir_iterative(req: GenerateSubsystemIrIterativeRequest):
    compiled = {}
    step1_issues = _validate_step1_bundle(req.step1_sysml2, req.step1_req_test_map, compiled=compiled)
    if step1_issues:
        return GenerateSubsystemIrIterativeResponse(
            ok=False,
            feedback_target="step1",
            model=req.model or _default_model,
            iteration=0,
            files={},
            contents=None,
            issues=step1_issues,
            feedback=_issues_to_feedback_lines(step1_issues),
        )

    if req.step1_sysml2 is not None:
        obj = validate_ir(compiled['ir'])
        issues = _validate_subsystem_ir(obj, contract=req.interface_contract)
        if issues:
            return GenerateSubsystemIrIterativeResponse(ok=False, feedback_target="step1",
                model="deterministic-sysml-compiler", iteration=0, files={}, contents=None,
                issues=issues, feedback=_issues_to_feedback_lines(issues))
        _write_text(req.out_ir_path, json.dumps(obj, ensure_ascii=False, indent=2))
        return GenerateSubsystemIrIterativeResponse(ok=True, feedback_target="none",
            model="deterministic-sysml-compiler", iteration=0, files={"subsystem_ir":req.out_ir_path},
            contents={"subsystem_ir":obj, "ir_origin":"compiled_sysml",
                      "interface_contract":req.interface_contract,
                      "contract_sha256":contract_digest(req.interface_contract) if req.interface_contract else None},
            issues=[], feedback=[])

    base_prompt = _load_prompt(req)
    if req.interface_contract is not None:
        base_prompt += "\nFrozen interface contract:\n" + json.dumps(req.interface_contract, ensure_ascii=False)
        from app.services.interface_semantics import model_boundary_prompt
        base_prompt += model_boundary_prompt(req.interface_contract)
    wrapper = (
        "You MUST output ONLY valid JSON (no Markdown, no code fences, no extra text).\n"
        "Return an executable scalar IR v2 JSON object.\n"
        "The result MUST satisfy structural correctness and full edge connectivity.\n\n"
    )

    current_prompt = base_prompt
    last_obj: dict | None = None
    last_issues: list[ValidationIssue] = []
    model_used = req.model or _default_model

    for iteration in range(1, req.max_iters + 1):
        model_used, text = _call_llm(json_prompt(IR_V2_PROMPT + wrapper + current_prompt), req)
        try:
            check_completion(text)
            obj = json.loads(text)
            if not isinstance(obj, dict):
                raise ValueError("top-level JSON must be object")
        except Exception as exc:
            issue = ValidationIssue(code=getattr(exc, 'code', 'IR_JSON_PARSE'), message=f"Model output is not valid JSON object: {exc}")
            last_issues = [issue]
            if issue.code == 'LLM_REFUSAL':
                break
            current_prompt = (
                base_prompt
                + "\n\n[Validation feedback from previous attempt]\n"
                + "\n".join(_issues_to_feedback_lines(last_issues))
                + "\n\nRegenerate the FULL subsystem_ir.json as valid JSON only."
            )
            continue

        issues = _validate_subsystem_ir(obj, req.step1_sysml2, req.interface_contract)
        last_obj = obj
        last_issues = issues
        if not issues:
            obj = validate_ir(obj)
            try:
                os.makedirs(os.path.dirname(req.out_ir_path) or ".", exist_ok=True)
                with open(req.out_ir_path, "w", encoding="utf-8") as handle:
                    json.dump(obj, handle, ensure_ascii=False, indent=2)
            except Exception as exc:
                raise HTTPException(status_code=500, detail=f"Failed to write subsystem_ir file: {exc}")

            return GenerateSubsystemIrIterativeResponse(
                ok=True,
                feedback_target="none",
                model=model_used,
                iteration=iteration,
                files={"subsystem_ir": req.out_ir_path},
                contents={"subsystem_ir": obj, "ir_origin": "llm_ir",
                          "interface_contract": req.interface_contract,
                          "contract_sha256": contract_digest(req.interface_contract) if req.interface_contract else None},
                issues=[],
                feedback=[],
            )

        current_prompt = (
            base_prompt
            + "\n\n[Previous complete IR candidate]\n" + json.dumps(obj, ensure_ascii=False)
            + "\n\n[Validation feedback from previous attempt]\n"
            + "\n".join(_issues_to_feedback_lines(issues))
            + "\n\nRegenerate the FULL subsystem_ir.json as valid JSON only, and fix all issues."
        )

    return GenerateSubsystemIrIterativeResponse(
        ok=False,
        feedback_target="step2",
        model=model_used,
        iteration=iteration,
        files={"subsystem_ir": req.out_ir_path} if last_obj else {},
        contents={"subsystem_ir": last_obj} if last_obj else None,
        issues=last_issues,
        feedback=_issues_to_feedback_lines(last_issues),
    )


@app.get("/prompts/list")
def list_prompts():
    return {"files": _safe_list_files("tools/prompts")}


@app.get("/prompts/get")
def get_prompt(path: str):
    if ".." in path or path.startswith(("/", "\\")):
        raise HTTPException(status_code=400, detail="Invalid path")
    return {"path": path, "content": _safe_read_under("tools/prompts", path)}


@app.get("/examples/list")
def list_examples():
    return {"files": _safe_list_files("tools/examples")}


@app.get("/examples/get")
def get_example(path: str):
    if ".." in path or path.startswith(("/", "\\")):
        raise HTTPException(status_code=400, detail="Invalid path")
    return {"path": path, "content": _safe_read_under("tools/examples", path)}


@app.post("/arduino/compile", response_model=ArduinoCompileResponse)
def compile_arduino(req: ArduinoCompileRequest):
    return _compile_arduino_sketch(req.sketch_path, req.fqbn, provenance=req.provenance)


@app.get("/arduino/devices", response_model=ArduinoDevicesResponse)
def list_arduino_devices():
    return _list_arduino_devices_via_windows_cli()


@app.get("/arduino/code", response_model=ArduinoCodeResponse)
def get_arduino_code(sketch_path: str = "outputs/arduino_output.ino"):
    path = _resolve_path(sketch_path)
    if not path.exists() or not path.is_file():
        raise HTTPException(status_code=404, detail=f"Sketch file not found: {path}")
    return ArduinoCodeResponse(sketch_path=str(path), code=path.read_text(encoding="utf-8"))


@app.post("/arduino/serial/read", response_model=ArduinoSerialReadResponse)
def read_arduino_serial(req: ArduinoSerialReadRequest):
    return _read_arduino_serial_via_powershell(
        port=req.port,
        baud_rate=req.baud_rate,
        read_timeout_ms=req.read_timeout_ms,
        max_chars=req.max_chars,
    )


def _validate_arduino_edit(original: str, updated: str) -> None:
    begin, end = "// BEGIN COMPILED SYSML MODEL", "// END COMPILED SYSML MODEL"
    if begin in original:
        def split_kernel(text):
            if text.count(begin) != 1 or text.count(end) != 1 or text.index(end) < text.index(begin):
                raise HTTPException(status_code=422, detail="Compiled SysML model markers must remain intact")
            prefix, rest = text.split(begin, 1)
            kernel, suffix = rest.split(end, 1)
            return kernel, prefix + suffix
        kernel, _ = split_kernel(original)
        updated_kernel, adapter = split_kernel(updated)
        if kernel != updated_kernel:
            raise HTTPException(status_code=422, detail="Update SysML and recompile to change model behavior")
        issues = adapter_issues(adapter)
    elif os.getenv("ARDUINO_STATIC_CHECK_STRICT", "true").strip().lower() in {"1", "true", "yes", "on"}:
        issues = validate_arduino_static_quality(updated)
    else:
        issues = []
    if issues:
        raise HTTPException(status_code=422, detail={"issues": issues})


@app.post("/arduino/update-module-code", response_model=ArduinoUpdateModuleCodeResponse)
def update_arduino_module_code(req: ArduinoUpdateModuleCodeRequest):
    sketch_path = _resolve_path(req.sketch_path)
    if not sketch_path.exists() or not sketch_path.is_file():
        raise HTTPException(status_code=404, detail=f"Sketch file not found: {sketch_path}")

    current_code = sketch_path.read_text(encoding="utf-8")
    updated_code = _replace_marked_module_code(current_code, req.module_id, req.code)
    module_code_map = _extract_marked_module_code_map(updated_code)
    _validate_arduino_edit(current_code, updated_code)
    _write_text(str(sketch_path), updated_code)

    return ArduinoUpdateModuleCodeResponse(
        success=True,
        module_id=req.module_id,
        saved_path=str(sketch_path),
        arduino_code=updated_code,
        module_code_map=module_code_map,
    )


@app.post("/arduino/params/list", response_model=ArduinoListParamsResponse)
def list_arduino_params(req: ArduinoListParamsRequest):
    sketch_path = _resolve_path(req.sketch_path)
    if not sketch_path.exists() or not sketch_path.is_file():
        raise HTTPException(status_code=404, detail=f"Sketch file not found: {sketch_path}")

    current_code = sketch_path.read_text(encoding="utf-8")
    params = _extract_marked_param_list(current_code)
    return ArduinoListParamsResponse(sketch_path=str(sketch_path), params=params)


@app.post("/arduino/params/update", response_model=ArduinoUpdateParamsResponse)
def update_arduino_params(req: ArduinoUpdateParamsRequest):
    sketch_path = _resolve_path(req.sketch_path)
    if not sketch_path.exists() or not sketch_path.is_file():
        raise HTTPException(status_code=404, detail=f"Sketch file not found: {sketch_path}")
    if not req.updates:
        raise HTTPException(status_code=400, detail="updates is required")

    current_code = sketch_path.read_text(encoding="utf-8")
    updated_code, params = _apply_param_updates_to_arduino_code(current_code, req.updates)
    _validate_arduino_edit(current_code, updated_code)
    _write_text(str(sketch_path), updated_code)

    return ArduinoUpdateParamsResponse(
        success=True,
        saved_path=str(sketch_path),
        arduino_code=updated_code,
        params=params,
    )


@app.post("/arduino/upload", response_model=ArduinoUploadResponse)
def upload_arduino(req: ArduinoUploadRequest):
    return _upload_arduino_sketch_via_windows_cli(req.sketch_path, req.fqbn, req.port)


@app.post("/convert/c-to-arduino", response_model=ConvertCToArduinoResponse)
def convert_c_to_arduino(req: ConvertCToArduinoRequest):
    if not req.allow_legacy_adapter:
        if req.compiled_kernel is None or req.interface_contract is None:
            raise HTTPException(status_code=422, detail="The production adapter requires compiled_kernel and a structured interface_contract. Legacy free-form translation is isolated behind allow_legacy_adapter=true.")
        try:
            validate_hardware_contract(req.interface_contract)
            AUTHORITY.verify(req.provenance,'kernel',dict(contract_sha256=contract_digest(req.interface_contract),
                header_sha256=text_digest(req.compiled_kernel.header),
                kernel_source_sha256=text_digest(req.compiled_kernel.source)))
        except ValueError as exc:
            raise HTTPException(status_code=422, detail=str(exc)) from exc

    if req.compiled_kernel is not None:
        llm_req = GenerateFromPromptRequest(model=req.model, temperature=req.temperature, history=req.history)
        try:
            model, sketch = generate_adapter(lambda prompt: _call_llm(prompt, llm_req),
                req.compiled_kernel.header, req.compiled_kernel.source,
                req.user_requirements or "", req.pin_definitions or {}, contract=req.interface_contract)
        except ValueError as exc:
            raise HTTPException(status_code=422, detail=str(exc)) from exc
        out = _resolve_path(req.out_path or str(Path(OUTPUT_DIR) / "arduino_output.ino"))
        receipt=AUTHORITY.extend(req.provenance,'kernel','arduino',arduino_sha256=text_digest(sketch)) if not req.allow_legacy_adapter else None
        _write_text(str(out), sketch)
        return ConvertCToArduinoResponse(arduino_code=sketch, model=model, saved_path=str(out),
            provenance=receipt,provenance_status='verified_model_chain' if receipt else 'unverified_legacy',
            contract_sha256=contract_digest(req.interface_contract) if req.interface_contract else None)

    if not _llm_api_key:
        raise HTTPException(status_code=500, detail="OPENROUTER_API_KEY/OPENAI_API_KEY is not set")

    model = req.model or _default_model
    strict_compile_check = os.getenv("ARDUINO_COMPILE_STRICT", "false").strip().lower() in {"1", "true", "yes", "on"}
    enable_compile_check = os.getenv("ARDUINO_COMPILE_CHECK", "false").strip().lower() in {"1", "true", "yes", "on"}

    # 从文件加载prompt模板
    prompt_template_path = Path("tools/prompts/c_to_arduino.prompt.md")
    if not prompt_template_path.exists():
        raise HTTPException(status_code=500, detail=f"Prompt template not found: {prompt_template_path}")
    
    try:
        with open(prompt_template_path, "r", encoding="utf-8") as f:
            prompt_template = f.read()
    except Exception as exc:
        raise HTTPException(status_code=500, detail=f"Failed to read prompt template: {exc}")

    def _repair_swallowed_comment_code(text: str) -> str:
        out = text or ""
        code_start = (
            r"(?:const|void|int|bool|float|double|long|short|byte|char|unsigned|signed)\b"
            r"|(?:if|for|while|switch|else|return)\b"
            r"|(?:pinMode|digitalWrite|digitalRead|analogRead|analogWrite|delay|tone|noTone|map)\s*\("
            r"|(?:Serial\.(?:begin|print|println))\s*\("
            r"|[A-Za-z_][A-Za-z0-9_]*\s*\("
            r"|}"
        )

        # If model accidentally appends real code after a // comment, force a line break.
        out = re.sub(rf"(//[^\n]*?)(?=\s*{code_start})", r"\1\n", out)

        # Normalize cases where a function closing brace was swallowed into the comment.
        out = re.sub(r"(//[^\n]*?)\s*}", r"\1\n}", out)

        # If setup()/loop() bodies are left unbalanced by malformed comments, close them.
        def _ensure_closed_function(src: str, func_name: str) -> str:
            match = re.search(rf"\bvoid\s+{func_name}\s*\(\s*\)\s*\{{", src)
            if not match:
                return src
            start = match.end() - 1
            depth = 0
            for idx in range(start, len(src)):
                ch = src[idx]
                if ch == "{":
                    depth += 1
                elif ch == "}":
                    depth -= 1
                    if depth == 0:
                        return src
            return src.rstrip() + "\n}\n"

        out = _ensure_closed_function(out, "setup")
        out = _ensure_closed_function(out, "loop")
        return out

    def _sanitize_arduino_code(text: str) -> str:
        def _normalize_param_marker_layout(src: str) -> str:
            # 1) Join broken multi-line PARAM comments into single-line comments.
            raw_lines = (src or "").splitlines()
            joined_lines: list[str] = []
            i = 0
            while i < len(raw_lines):
                line = raw_lines[i]
                if "[PARAM:" in line and "]" not in line:
                    buffer = line.rstrip()
                    j = i + 1
                    while j < len(raw_lines):
                        part = raw_lines[j].strip()
                        if part:
                            buffer = f"{buffer} {part}"
                        if "]" in raw_lines[j]:
                            break
                        j += 1
                    line = buffer
                    i = j
                joined_lines.append(line)
                i += 1

            # 2) Ensure standalone PARAM marker lines are attached to previous declaration line.
            marker_line_re = re.compile(r"^\s*//\s*\[PARAM:[^\]]+\]\s*$")
            decl_end_re = re.compile(r";\s*(?://.*)?$")
            marker_blob_re = re.compile(r"\[PARAM:(?P<meta>[^\]]+)\]")

            def _parse_blob(meta: str) -> dict[str, str]:
                out: dict[str, str] = {}
                for token in (meta or "").split("|"):
                    token = token.strip()
                    if "=" not in token:
                        continue
                    k, v = token.split("=", 1)
                    k = k.strip().lower()
                    v = v.strip()
                    if k:
                        out[k] = v
                return out

            def _merge_blob(a: dict[str, str], b: dict[str, str]) -> dict[str, str]:
                merged = dict(a)
                for key, value in b.items():
                    if not value:
                        continue
                    if key not in merged or not merged[key] or len(value) > len(merged[key]):
                        merged[key] = value
                return merged

            def _build_blob(meta: dict[str, str]) -> str:
                ordered_keys = ["id", "module", "name", "zh_name", "type", "min", "max"]
                parts: list[str] = []
                for key in ordered_keys:
                    val = str(meta.get(key) or "").strip()
                    if val:
                        parts.append(f"{key}={val}")
                for key in sorted(meta.keys()):
                    if key in ordered_keys:
                        continue
                    val = str(meta.get(key) or "").strip()
                    if val:
                        parts.append(f"{key}={val}")
                return "|".join(parts)

            out_lines: list[str] = []
            for line in joined_lines:
                stripped = line.strip()
                if marker_line_re.match(stripped):
                    if out_lines and decl_end_re.search(out_lines[-1]):
                        out_lines[-1] = f"{out_lines[-1].rstrip()} {stripped}"
                    else:
                        out_lines.append(line)
                    continue
                out_lines.append(line)

            # 3) Canonicalize marker comments: one marker per declaration, merging duplicates.
            canonical_lines: list[str] = []
            for line in out_lines:
                matches = list(marker_blob_re.finditer(line))
                if not matches:
                    canonical_lines.append(line)
                    continue

                merged_meta: dict[str, str] = {}
                for m in matches:
                    merged_meta = _merge_blob(merged_meta, _parse_blob(m.group("meta")))

                # Remove all PARAM markers from the line and append one canonical marker.
                line_wo_markers = marker_blob_re.sub("", line)
                line_wo_markers = re.sub(r"\s+$", "", line_wo_markers)
                line_wo_markers = re.sub(r"//\s*$", "", line_wo_markers)
                marker_text = f"[PARAM:{_build_blob(merged_meta)}]"
                if "//" in line_wo_markers:
                    canonical_lines.append(f"{line_wo_markers.rstrip()} // {marker_text}")
                else:
                    canonical_lines.append(f"{line_wo_markers.rstrip()} // {marker_text}")

            # 4) Normalize accidental whitespace in marker key-values like "|type= int".
            normalized = "\n".join(canonical_lines)
            normalized = re.sub(r"\|\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s+", r"|\1=", normalized)
            return normalized

        out = (text or "").strip()
        if out.startswith("```cpp"):
            out = out[6:]
        elif out.startswith("```"):
            out = out[3:]
        if out.endswith("```"):
            out = out[:-3]
        out = _repair_swallowed_comment_code(out)
        out = _normalize_param_marker_layout(out)
        return out.strip()

    def _validate_arduino_code(text: str) -> list[str]:
        issues: list[str] = []
        src = text or ""
        if not src.strip():
            issues.append("输出为空")
            return issues
        if "```" in src:
            issues.append("输出包含 Markdown 代码块")
        if not re.search(r"\bvoid\s+setup\s*\(\s*\)", src):
            issues.append("缺少 setup()")
        if not re.search(r"\bvoid\s+loop\s*\(\s*\)", src):
            issues.append("缺少 loop()")
        if re.search(r"#include\s+[\"<]define\.h[\">]", src):
            issues.append("包含了禁止的私有头文件 define.h")

        serial_begin_matches = list(re.finditer(r"\bSerial\.begin\s*\(", src))
        setup_match = re.search(r"\bvoid\s+setup\s*\(\s*\)\s*\{", src)
        setup_span: tuple[int, int] | None = None
        if setup_match:
            start = setup_match.start()
            depth = 0
            end = -1
            for idx in range(setup_match.end() - 1, len(src)):
                ch = src[idx]
                if ch == "{":
                    depth += 1
                elif ch == "}":
                    depth -= 1
                    if depth == 0:
                        end = idx + 1
                        break
            if end > start:
                setup_span = (start, end)

        if re.search(r"\bSerial\.(print|println)\s*\(", src):
            if not serial_begin_matches:
                issues.append("使用了 Serial.print/println，但未调用 Serial.begin")

        if serial_begin_matches:
            if len(serial_begin_matches) > 1:
                issues.append("Serial.begin 调用了多次，应只在 setup() 中初始化一次")
            if setup_span is None:
                issues.append("检测到 Serial.begin，但未找到完整 setup() 代码块")
            else:
                sb = serial_begin_matches[0].start()
                if not (setup_span[0] <= sb < setup_span[1]):
                    issues.append("Serial.begin 必须放在 setup() 中，不能在其它函数或 loop() 中调用")

        pin_defs = re.findall(
            r"\bconst\s+(?:int|byte|short|long|uint8_t|uint16_t|uint32_t|unsigned\s+int|unsigned\s+long)\s+([A-Za-z_][A-Za-z0-9_]*_PIN)\b",
            src,
        )
        for pin_name in sorted(set(pin_defs)):
            uses = len(re.findall(rf"\b{re.escape(pin_name)}\b", src))
            if uses <= 1:
                issues.append(f"引脚常量 {pin_name} 已定义但未被使用")

        return issues

    def _check_requirement_coverage(req_text: str, code_text: str) -> list[str]:
        issues: list[str] = []
        source = (req_text or "")
        m = re.search(r"用户需求\s*[:：]\s*(.+)$", source, re.S)
        if m:
            source = m.group(1).strip()
        rtxt = source.lower()
        ctxt = (code_text or "").lower()
        expectations: list[tuple[list[str], list[str], str]] = [
            (["电机", "motor"], ["motor", "analogwrite", "pwm", "servo", "digitalwrite"], "需求提到电机，但代码缺少电机控制迹象"),
            (["蜂鸣", "buzzer", "beep", "报警", "告警", "alarm"], ["buzzer", "tone(", "notone(", "digitalwrite", "alarm"], "需求提到告警/蜂鸣，但代码缺少对应控制逻辑"),
            (["按键", "button", "key"], ["digitalread", "button", "key", "input_pullup"], "需求提到按键输入，但代码缺少输入读取逻辑"),
            (["串口", "serial", "日志", "print"], ["serial.begin", "serial.print", "serial.println"], "需求提到串口/日志，但代码缺少串口输出"),
            (["显示", "display", "lcd", "oled", "数码"], ["display", "lcd", "oled", "tm1637", "segment"], "需求提到显示，但代码缺少显示相关逻辑"),
        ]
        for req_keys, code_keys, message in expectations:
            if any(key in rtxt for key in req_keys) and not any(key in ctxt for key in code_keys):
                issues.append(message)
        return issues

    def _compile_arduino_uno(code_text: str) -> tuple[bool, str]:
        cli = shutil.which("arduino-cli")
        if not cli:
            return False, "arduino-cli 不可用，无法进行可部署性编译校验"

        check_dir = Path(OUTPUT_DIR) / ".arduino_compile_check"
        try:
            if check_dir.exists():
                shutil.rmtree(check_dir)
            check_dir.mkdir(parents=True, exist_ok=True)
            sketch_path = check_dir / f"{check_dir.name}.ino"
            _write_text(str(sketch_path), code_text)
            proc = subprocess.run(
                [cli, "compile", "--fqbn", "arduino:avr:uno", str(check_dir)],
                capture_output=True,
                text=True,
                timeout=180,
            )
            if proc.returncode == 0:
                return True, (proc.stdout or "").strip()
            msg = ((proc.stdout or "") + "\n" + (proc.stderr or "")).strip()
            return False, msg[-6000:]
        except Exception as exc:
            return False, f"arduino-cli 编译校验异常: {type(exc).__name__}: {exc}"

    user_requirements_text = (req.user_requirements or "").strip()
    if not user_requirements_text:
        user_requirements_text = "请根据输入C代码生成可运行Arduino程序。"

    pin_defs: dict[str, str] = {}
    if req.pin_definitions:
        for key, value in req.pin_definitions.items():
            k = str(key or "").strip()
            v = str(value or "").strip()
            if k and v:
                pin_defs[k] = v

    if pin_defs:
        pin_lines = [f"{idx + 1}) {k}: {v}" for idx, (k, v) in enumerate(pin_defs.items())]
        pin_text = "\n".join(pin_lines)
        pin_json = json.dumps(pin_defs, ensure_ascii=False, indent=2)
    else:
        pin_text = "未提供模块引脚定义。"
        pin_json = "{}"

    pin_block = (
        f"{pin_text}\n\n"
        f"JSON:\n{pin_json}\n\n"
        "规则: 用户显式提供的引脚优先；未提供字段可按需求补全。"
    )

    pin_assignments_block = (
        "最终引脚常量参考（仅供实现时复用，不是强制固定硬件模板）:\n"
        f"JSON:\n{pin_json}"
    )

    prompt = prompt_template
    prompt = prompt.replace("{{C_CODE}}", req.c_code)
    prompt = prompt.replace("<<C_CODE>>", req.c_code)
    prompt = prompt.replace("<<USER_REQUIREMENTS>>", user_requirements_text)
    prompt = prompt.replace("<<PIN_DEFINITIONS>>", pin_block)
    if "<<PIN_ASSIGNMENTS>>" in prompt:
        prompt = prompt.replace("<<PIN_ASSIGNMENTS>>", pin_assignments_block)
    else:
        prompt = f"{prompt}\n\n## 最终引脚常量定义（参考）\n{pin_assignments_block}\n"

    # 便于核查调用链，记录本次请求和渲染后prompt
    try:
        debug_req_path = Path(OUTPUT_DIR) / "last_c_to_arduino_request.json"
        debug_prompt_path = Path(OUTPUT_DIR) / "last_c_to_arduino_prompt.txt"
        debug_req_path.parent.mkdir(parents=True, exist_ok=True)
        _write_text(str(debug_req_path), json.dumps(req.model_dump(), ensure_ascii=False, indent=2))
        _write_text(str(debug_prompt_path), prompt)
    except Exception:
        pass

    history_messages = _normalize_history_messages(req.history)
    params = {
        "model": model,
        "messages": [
            {"role": "system", "content": "You are an embedded systems engineer specializing in Arduino programming."},
            *history_messages,
            {"role": "user", "content": prompt}
        ],
        "temperature": req.temperature if req.temperature is not None else 0.2,
    }

    try:
        response = client.chat.completions.create(**params)
    except Exception as exc:
        raise HTTPException(status_code=500, detail=f"LLM call failed: {type(exc).__name__}: {exc}")

    arduino_code = _sanitize_arduino_code(response.choices[0].message.content or "")
    arduino_code, module_code_map = _build_marked_module_code_map(arduino_code, req.module_specs)
    arduino_code, param_list = _mark_module_params_in_arduino_code(arduino_code)

    # Temporary mode: Step5 only generates Arduino code and skips deployability checks.
    saved_path = None
    if req.out_path:
        out_path = _resolve_path(req.out_path)
        out_path.parent.mkdir(parents=True, exist_ok=True)
        _write_text(str(out_path), arduino_code)
        saved_path = str(out_path)
    else:
        default_path = Path(OUTPUT_DIR) / "arduino_output.ino"
        default_path.parent.mkdir(parents=True, exist_ok=True)
        _write_text(str(default_path), arduino_code)
        saved_path = str(default_path)

    return ConvertCToArduinoResponse(
        arduino_code=arduino_code,
        model=model,
        saved_path=saved_path,
        module_code_map=module_code_map or None,
        params=param_list or None,
    )

    """
    compile_log = ""
    final_issues: list[str] = []
    for attempt in range(0, 2):
        issues = _validate_arduino_code(arduino_code)
        issues.extend(_check_requirement_coverage(user_requirements_text, arduino_code))
        if enable_compile_check:
            compile_ok, compile_msg = _compile_arduino_uno(arduino_code)
            compile_log = compile_msg
            if not compile_ok:
                pass
            cli_missing = "arduino-cli 不可用" in compile_msg
            if strict_compile_check or not cli_missing:
                issues.append(f"无法通过 Arduino UNO 编译校验: {compile_msg[:500]}")

        if not issues:
            final_issues = []
            break

        final_issues = issues
        if attempt == 1:
            break

        repair_prompt = (
            "请修复下面这份Arduino代码，使其满足全部约束并可通过 Arduino UNO 编译。\n"
            "必须仅输出修复后的Arduino代码，不要解释。\n"
            f"发现的问题:\n- {chr(10).join(issues)}\n\n"
            "原始代码:\n"
            f"{arduino_code}\n"
        )
        try:
            repair_resp = client.chat.completions.create(
                model=model,
                messages=[
                    {"role": "system", "content": "You are an embedded systems engineer specializing in Arduino programming."},
                    {"role": "user", "content": repair_prompt},
                ],
                temperature=req.temperature or 0.2,
            )
            arduino_code = _sanitize_arduino_code(repair_resp.choices[0].message.content or "")
        except Exception:
            break

    """
    if final_issues:
        raise HTTPException(
            status_code=422,
            detail={
                "message": "Arduino 代码未通过可部署性校验",
                "issues": final_issues,
                "compile_log": compile_log[-4000:],
            },
        )

    # 保存到文件（如果提供了输出路径）
    saved_path = None
    if req.out_path:
        out_path = _resolve_path(req.out_path)
        out_path.parent.mkdir(parents=True, exist_ok=True)
        _write_text(str(out_path), arduino_code)
        saved_path = str(out_path)
    else:
        # 默认保存到outputs/arduino_output.ino
        default_path = Path(OUTPUT_DIR) / "arduino_output.ino"
        default_path.parent.mkdir(parents=True, exist_ok=True)
        _write_text(str(default_path), arduino_code)
        saved_path = str(default_path)

    return ConvertCToArduinoResponse(arduino_code=arduino_code, model=model, saved_path=saved_path)
