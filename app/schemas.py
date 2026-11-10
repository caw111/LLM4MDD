from pydantic import BaseModel, Field


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
    out_sysml2_path: str = Field(default="outputs/system.sysml2")
    out_req_test_map_path: str = Field(default="outputs/req_test_map.json")


class GenerateSubsystemIrRequest(GenerateFromPromptRequest):
    out_ir_path: str = Field(default="outputs/subsystem_ir.json")


class GenerateCFromStep1Request(GenerateFromPromptRequest):
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
    step1_sysml2: str | None = None
    step1_req_test_map: dict | None = None


class ValidateSubsystemIrResponse(BaseModel):
    ok: bool
    feedback_target: str
    issues: list[ValidationIssue]
    feedback: list[str]


class GenerateSubsystemIrIterativeRequest(GenerateSubsystemIrRequest):
    step1_sysml2: str | None = None
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


class ConvertCToArduinoRequest(BaseModel):
    c_code: str = Field(..., min_length=1)
    user_requirements: str | None = None
    pin_definitions: dict[str, str] | None = None
    module_specs: list[dict] | None = None
    history: list[ConversationMessage] | None = None
    model: str | None = None
    temperature: float | None = None
    out_path: str | None = None  # 可选的输出文件路径


class ConvertCToArduinoResponse(BaseModel):
    arduino_code: str
    model: str
    module_code_map: dict[str, str] | None = None
    params: list[dict] | None = None
    quality_warnings: list[str] | None = None
    validation_issues: list[str] | None = None
    compile_log: str | None = None
    repair_attempts: int = 0
    saved_path: str | None = None  # 保存的文件路径


class ArduinoCompileRequest(BaseModel):
    sketch_path: str = Field(default="outputs/arduino_output.ino", min_length=1)
    fqbn: str = Field(default="arduino:avr:uno", min_length=1)


class ArduinoCompileResponse(BaseModel):
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

__all__ = [
    "ChatRequest",
    "ChatResponse",
    "ConversationMessage",
    "DecideModeRequest",
    "DecideModeResponse",
    "ConvertIrToTbLinkRequest",
    "GenerateFromPromptRequest",
    "GenerateSysml2BundleRequest",
    "GenerateSubsystemIrRequest",
    "GenerateCFromStep1Request",
    "GenerateResponse",
    "ConvertCToIlRequest",
    "ConvertCToIlResponse",
    "ConvertIlToCRequest",
    "CodegenStepResult",
    "RunCodegenPipelineRequest",
    "RunCodegenPipelineResponse",
    "ConvertIlToCResponse",
    "ValidationIssue",
    "ValidateSubsystemIrRequest",
    "ValidateSubsystemIrResponse",
    "GenerateSubsystemIrIterativeRequest",
    "GenerateSubsystemIrIterativeResponse",
    "ConvertCToArduinoRequest",
    "ConvertCToArduinoResponse",
    "ArduinoCompileRequest",
    "ArduinoCompileResponse",
    "ArduinoUpdateModuleCodeRequest",
    "ArduinoUpdateModuleCodeResponse",
    "ArduinoListParamsRequest",
    "ArduinoListParamsResponse",
    "ArduinoParamUpdateItem",
    "ArduinoUpdateParamsRequest",
    "ArduinoUpdateParamsResponse",
    "ArduinoUploadRequest",
    "ArduinoUploadResponse",
    "ArduinoDevicePort",
    "ArduinoDeviceBoard",
    "ArduinoDevicesResponse",
    "ArduinoCodeResponse",
    "DiagnosticEvent",
    "DiagnosticSummaryItem",
    "DiagnosticParseRequest",
    "DiagnosticParseResponse",
    "DiagnosticSummarizeRequest",
    "DiagnosticSummarizeResponse",
    "DiagnosticSuggestion",
    "DiagnosticSuggestFixRequest",
    "DiagnosticSuggestFixResponse",
    "GenerateModuleBlueprintRequest",
    "GenerateModuleBlueprintResponse",
    "SystemStateRequest",
    "SystemPatchRequest",
    "SystemStateSaveRequest",
    "SystemPatchResponse",
]
