export type InterfaceRegistry = {
  version: 1;
  ports: Record<string, { direction: "input" | "output"; type: "bool" | "i32" | "f32" | "f64" }>;
  serial_channels: { id: string; trigger_port: string; value_ports: string[] }[];
  serial_inputs?: { id: string; value_port: string; valid_port: string; kind: "serial_command_input" | "serial_integer_input" | "serial_string_input" | "serial_datetime_input" | "serial_packet_input";
    baud: number; framing: "line" | "line_or_idle"; idle_timeout_ms: number; commands?: Record<string, number>; min?: number; max?: number }[];
  serial_text_outputs?: { id: string; trigger_port: string; kind: "serial_text_output"; baud: number; text: string }[];
};
export type ContractJson = string | number | boolean | null | ContractJson[] | { [key: string]: ContractJson };
export type NumericCheck =
  | { kind: "counter_edges"; output: string; increment: string; decrement: string; reset: string; delta: string; minimum: number; maximum: number; initial: number; min_repeat_ms: number; response_ms: number; evidence: string }
  | { kind: "mapped_parity"; output: string; input: string; even_input: string; odd_input: string; even_output: string; odd_output: string; adc_max: number; display_max: number; pwm_max: number; pwm_tolerance: number; response_ms: number; evidence: string };

export type InterfaceContract = {
  version: 1;
  board?: string;
  control_period_ms: number;
  interfaces: {
    name: string;
    direction: "input" | "output";
    type: "bool" | "i32" | "f32" | "f64";
    unit: string;
    meaning: string;
    sample_period_ms: number;
    initial: number | boolean;
    device: string;
    hardware: string;
    binding: Record<string, ContractJson>;
    model_semantics?: { kind: "digital_boolean"; true_level: "HIGH" | "LOW"; false_level: "HIGH" | "LOW" };
  }[];
  requirements: { id: string; text: string; assertion_ids: string[] }[];
  required_dependencies?: Record<string, string[]>;
  required_states?: { name: string; type: string; initial: string }[];
  startup_checks?: { output: string; expected: number | boolean; evidence: string }[];
  pause_checks?: { input: string; delta: string; output: string; initial_running: boolean; expected: number; evidence: string; response_tolerance_ms?: number }[];
  periodic_checks?: { delta: string; output: string; period_ms: number; pulse_ms: number; first_pulse: "immediate" | "after_period" | "after_silence"; active_value: number | boolean; inactive_value: number | boolean; tolerance_ms: number; evidence: string; pause_input?: string; initial_running?: boolean; pause_policy?: "preserve" | "reset" }[];
  planning_notes?: string[];
  interface_registry?: InterfaceRegistry;
  wiring_snapshot?: Record<string, Record<string, string | number>>;
  numeric_checks?: NumericCheck[];
};

export type GenerateSysml2BundleRequest = {
  interface_contract?: InterfaceContract;
  max_repair_attempts?: number;
  prompt?: string;
  prompt_file?: string;
  history?: { role: "user" | "assistant" | "system"; content: string }[];
  model?: string | null;
  temperature?: number | null;
  max_tokens?: number | null;
  out_sysml2_path?: string;
  out_req_test_map_path?: string;
};

export type GenerateSubsystemIrRequest = {
  step1_sysml2?: string | null;
  interface_contract?: InterfaceContract;
  prompt?: string;
  prompt_file?: string;
  history?: { role: "user" | "assistant" | "system"; content: string }[];
  model?: string | null;
  temperature?: number | null;
  max_tokens?: number | null;
  out_ir_path?: string;
};

export type GenerateCFromStep1Request = {
  interface_contract?: InterfaceContract;
  prompt?: string;
  prompt_file?: string;
  history?: { role: "user" | "assistant" | "system"; content: string }[];
  model?: string | null;
  temperature?: number | null;
  max_tokens?: number | null;
  step1_sysml2?: string | null;
  step1_sysml2_path?: string;
  step1_req_test_map?: Record<string, unknown> | null;
  step1_req_test_map_path?: string;
  out_dir?: string;
  out_define_h_path?: string | null;
  out_program_c_path?: string | null;
  out_main_c_path?: string | null;
};

export type GenerateResponse = {
  model: string;
  files: Record<string, string>;
  contents?: Record<string, unknown> | null;
};

export type ConvertCToIlRequest = {
  c_code?: string | null;
  c_file_path?: string;
  out_il_path?: string;
  code2script_exe?: string | null;
  code2script_root?: string | null;
  model_path?: string;
  out_script_path?: string | null;
};

export type ConvertCToIlResponse = {
  out_il_path: string;
  il_xml: string;
  command: string;
  returncode: number;
  stdout: string;
  stderr: string;
};

export type ConvertIlToCRequest = {
  provenance?: ProvenanceReceipt | null;
  interface_contract?: InterfaceContract;
  il_file_path?: string;
  out_c_dir?: string;
  codegen_root?: string | null;
  target?: string;
  optimize?: boolean;
  out_il_optimized_path?: string | null;
};

export type CodegenStepResult = {
  name: string;
  command: string;
  returncode: number;
  stdout: string;
  stderr: string;
};

export type ConvertIlToCResponse = {
  provenance?: ProvenanceReceipt | null;
  provenance_status: "verified_model_chain" | "unverified_legacy";
  contract_sha256?: string | null;
  files: Record<string, string>;
  steps: CodegenStepResult[];
  contents?: Record<string, string> | null;
};

export type RunCodegenPipelineRequest = {
  tblink_xml?: string | null;
  out_tblink_path?: string;
  out_mir_path?: string | null;
  out_il_path?: string | null;
  out_il_optimized_path?: string | null;
  out_c_dir?: string;
  codegen_root?: string | null;
  target?: string;
  optimize?: boolean;
};

export type RunCodegenPipelineResponse = {
  files: Record<string, string>;
  steps: CodegenStepResult[];
};

export type ValidationIssue = {
  code: string;
  message: string;
  path?: string | null;
  severity?: string;
};

export type ValidateSubsystemIrRequest = {
  interface_contract?: InterfaceContract;
  ir: Record<string, unknown>;
  step1_sysml2?: string | null;
  step1_req_test_map?: Record<string, unknown> | null;
};

export type ValidateSubsystemIrResponse = {
  ok: boolean;
  feedback_target: "none" | "step1" | "step2";
  issues: ValidationIssue[];
  feedback: string[];
};

export type GenerateSubsystemIrIterativeRequest = {
  interface_contract?: InterfaceContract;
  prompt?: string;
  prompt_file?: string;
  model?: string | null;
  temperature?: number | null;
  max_tokens?: number | null;
  out_ir_path?: string;
  step1_sysml2?: string | null;
  step1_req_test_map?: Record<string, unknown> | null;
  max_iters?: number;
};

export type GenerateSubsystemIrIterativeResponse = {
  ok: boolean;
  feedback_target: "none" | "step1" | "step2";
  model: string;
  iteration: number;
  files: Record<string, string>;
  contents?: Record<string, unknown> | null;
  issues: ValidationIssue[];
  feedback: string[];
};

export type ProvenanceReceipt = {
  claims: Record<string, unknown>;
  signature: string;
};

export type ConvertCToArduinoRequest = {
  provenance?: ProvenanceReceipt | null;
  interface_contract?: InterfaceContract;
  allow_legacy_adapter?: boolean;
  compiled_kernel?: { header: string; source: string };
  c_code: string;
  user_requirements?: string | null;
  pin_definitions?: Record<string, string> | null;
  module_specs?: {
    id: string;
    title?: string;
    subtitle?: string;
    moduleType?: string;
  }[] | null;
  history?: { role: "user" | "assistant" | "system"; content: string }[];
  model?: string | null;
  temperature?: number | null;
  out_path?: string | null;
};

export type ConvertCToArduinoResponse = {
  provenance?: ProvenanceReceipt | null;
  provenance_status: "verified_model_chain" | "unverified_legacy";
  contract_sha256?: string | null;
  arduino_code: string;
  model: string;
  saved_path?: string | null;
  module_code_map?: Record<string, string> | null;
  params?: ArduinoParamItem[] | null;
};

export type ArduinoParamItem = {
  param_id: string;
  module_id: string;
  name: string;
  zh_name?: string;
  type: string;
  value: string;
  min?: string;
  max?: string;
  line?: number;
};

export type ArduinoCompileRequest = {
  provenance?: ProvenanceReceipt | null;
  sketch_path?: string;
  fqbn?: string;
};

export type ArduinoCompileResponse = {
  provenance_status: "verified_model_chain" | "unverified_legacy";
  arduino_sha256?: string | null;
  success: boolean;
  fqbn: string;
  sketch_path: string;
  compile_stdout: string;
  compile_stderr: string;
};

export type ArduinoUpdateModuleCodeRequest = {
  module_id: string;
  code: string;
  sketch_path?: string;
};

export type ArduinoUpdateModuleCodeResponse = {
  success: boolean;
  module_id: string;
  saved_path: string;
  arduino_code: string;
  module_code_map: Record<string, string>;
};

export type ArduinoListParamsRequest = {
  sketch_path?: string;
};

export type ArduinoListParamsResponse = {
  sketch_path: string;
  params: ArduinoParamItem[];
};

export type ArduinoUpdateParamsItem = {
  param_id: string;
  value: string;
};

export type ArduinoUpdateParamsRequest = {
  sketch_path?: string;
  updates: ArduinoUpdateParamsItem[];
};

export type ArduinoUpdateParamsResponse = {
  success: boolean;
  saved_path: string;
  arduino_code: string;
  params: ArduinoParamItem[];
};

export type ArduinoUploadRequest = {
  sketch_path?: string;
  fqbn?: string;
  port: string;
};

export type ArduinoUploadResponse = {
  success: boolean;
  message: string;
  fqbn: string;
  port: string;
  sketch_path: string;
  compile_success: boolean;
  upload_success: boolean;
  compile_command: string;
  upload_command: string;
  compile_stdout: string;
  compile_stderr: string;
  upload_stdout: string;
  upload_stderr: string;
};

export type ArduinoDevicePort = {
  address: string;
  label: string;
  protocol?: string | null;
  board_name?: string | null;
  fqbn?: string | null;
};

export type ArduinoDeviceBoard = {
  label: string;
  fqbn: string;
};

export type ArduinoDevicesResponse = {
  ports: ArduinoDevicePort[];
  boards: ArduinoDeviceBoard[];
};

export type ArduinoCodeResponse = {
  sketch_path: string;
  code: string;
};

export type ArduinoSerialReadRequest = {
  port: string;
  baud_rate?: number;
  read_timeout_ms?: number;
  max_chars?: number;
};

export type ArduinoSerialReadResponse = {
  port: string;
  baud_rate: number;
  read_timeout_ms: number;
  text: string;
  chars_read: number;
};

export type DiagnosticEvent = {
  line_no: number;
  raw: string;
  timestamp?: string | null;
  level: string;
  code: string;
  module: string;
  message: string;
  context: Record<string, string>;
};

export type DiagnosticSummaryItem = {
  code: string;
  level: string;
  module: string;
  count: number;
  latest_line_no: number;
};

export type DiagnosticParseRequest = {
  raw_log: string;
  source?: string | null;
  max_events?: number;
};

export type DiagnosticParseResponse = {
  source?: string | null;
  total_lines: number;
  parsed_events: number;
  dropped_lines: number;
  events: DiagnosticEvent[];
  summary: DiagnosticSummaryItem[];
};

export type DiagnosticSuggestion = {
  title: string;
  rationale: string;
  patch_hint: string;
  risk: string;
};

export type DiagnosticSuggestFixRequest = {
  events: DiagnosticEvent[];
  requirement?: string | null;
  current_arduino_code?: string | null;
  model?: string | null;
};

export type DiagnosticSuggestFixResponse = {
  headline: string;
  probable_root_causes: string[];
  suggestions: DiagnosticSuggestion[];
  recommended_prompt: string;
  llm_used: boolean;
};

export type ModuleBlueprintSpec = {
  id: string;
  title: string;
  subtitle?: string;
  moduleType?: string;
  executor?: string;
  codeSeed?: string;
};

export type ModuleBlueprintConnection = {
  from: string;
  to: string;
  relation?: string;
  logic?: string;
};

export type GenerateModuleBlueprintRequest = {
  requirement: string;
  existing_blueprint?: {
    modules: ModuleBlueprintSpec[];
    connections: ModuleBlueprintConnection[];
  } | null;
  prompt?: string | null;
  history?: { role: "user" | "assistant" | "system"; content: string }[];
  model?: string | null;
  temperature?: number | null;
  max_tokens?: number | null;
  max_modules?: number;
};

export type GenerateModuleBlueprintResponse = {
  model: string;
  blueprint: {
    modules: ModuleBlueprintSpec[];
    connections: ModuleBlueprintConnection[];
  };
};

export type SystemStateModule = {
  id: string;
  title: string;
  subtitle: string;
  moduleType?: string;
  executor?: string;
  codeSeed?: string;
  params?: Record<string, unknown>;
};

export type SystemStateConnection = {
  from: string;
  to: string;
  relation?: string;
  logic?: string;
};

export type SystemState = {
  system_sysml2?: string;
  req_test_map?: Record<string, unknown>;
  modules: SystemStateModule[];
  connections: SystemStateConnection[];
};

export type GetSystemStateResponse = {
  session_id: string;
  version: number;
  system: SystemState;
};

export type SaveSystemStateRequest = {
  session_id: string;
  system: SystemState;
};

export type PatchSystemStateRequest = {
  session_id: string;
  instruction: string;
  current_system?: SystemState | null;
  model?: string | null;
  temperature?: number | null;
  max_tokens?: number | null;
};

export type PatchSystemStateResponse = {
  session_id: string;
  version: number;
  system: SystemState;
  ops: Record<string, unknown>[];
  assistant: string;
};

export type DecideModeRequest = {
  instruction: string;
  has_complete_system: boolean;
  current_system?: SystemState | null;
  history?: { role: "user" | "assistant" | "system"; content: string }[];
  model?: string | null;
  temperature?: number | null;
  max_tokens?: number | null;
};

export type DecideModeResponse = {
  mode: "regenerate" | "incremental";
  reason: string;
};

const API_BASE =
  (import.meta.env.VITE_API_BASE as string | undefined) ??
  (import.meta.env.DEV ? "/api" : "");

async function request<T>(path: string, init?: RequestInit): Promise<T> {
  const timeoutMs = Number((import.meta.env.VITE_API_TIMEOUT_MS as string | undefined) ?? (path === "/generate/sysml2-bundle" ? "1200000" : "180000"));
  const useTimeout = !init?.signal && Number.isFinite(timeoutMs) && timeoutMs > 0;
  const controller = useTimeout ? new AbortController() : null;
  const timer = controller ? setTimeout(() => controller.abort(), timeoutMs) : null;

  let res: Response;
  try {
    res = await fetch(`${API_BASE}${path}`, {
      ...init,
      signal: init?.signal ?? controller?.signal,
      headers: {
        "Content-Type": "application/json",
        ...(init?.headers ?? {}),
      },
    });
  } catch (err) {
    if (err instanceof DOMException && err.name === "AbortError" && useTimeout) {
      throw new Error(`请求超时(${Math.round(timeoutMs / 1000)}s): ${path}`);
    }
    throw err;
  } finally {
    if (timer) clearTimeout(timer);
  }

  if (!res.ok) {
    let detail = `${res.status} ${res.statusText}`;
    try {
      const body = (await res.json()) as { detail?: unknown };
      if (body?.detail) {
        if (typeof body.detail === "string") detail = body.detail;
        else {
          const failure = body.detail as { message?: string; attempts?: { issues?: { message?: string }[] }[]; validation_report?: string };
          detail = failure.attempts ? `${failure.message ?? "模型校验失败"} 已尝试 ${failure.attempts.length} 次。${failure.attempts.at(-1)?.issues?.map(issue => issue.message).join("；") ?? ""} 诊断记录：${failure.validation_report ?? ""}` : JSON.stringify(body.detail);
        }
      }
    } catch {
      // ignore
    }
    throw new Error(detail);
  }
  return (await res.json()) as T;
}

export async function health(): Promise<{ status: string }> {
  const res = await fetch(`${API_BASE}/health`);
  if (!res.ok) throw new Error("backend not reachable");
  return (await res.json()) as { status: string };
}

export async function listPrompts(): Promise<string[]> {
  const res = await request<{ files: string[] }>("/prompts/list");
  return res.files;
}

export async function getPrompt(path: string): Promise<string> {
  const res = await request<{ path: string; content: string }>(`/prompts/get?path=${encodeURIComponent(path)}`, {
    method: "GET",
  });
  return res.content;
}

export async function generateSysml2Bundle(payload: GenerateSysml2BundleRequest): Promise<GenerateResponse> {
  return request<GenerateResponse>("/generate/sysml2-bundle", { method: "POST", body: JSON.stringify(payload) });
}

export async function generateSubsystemIr(payload: GenerateSubsystemIrRequest): Promise<GenerateResponse> {
  return request<GenerateResponse>("/generate/subsystem-ir", { method: "POST", body: JSON.stringify(payload) });
}

export async function generateCFromStep1(payload: GenerateCFromStep1Request): Promise<GenerateResponse> {
  return request<GenerateResponse>("/generate/c-from-step1", { method: "POST", body: JSON.stringify(payload) });
}

export async function convertCToIl(payload: ConvertCToIlRequest): Promise<ConvertCToIlResponse> {
  return request<ConvertCToIlResponse>("/convert/c-to-il", { method: "POST", body: JSON.stringify(payload) });
}

export async function convertIlToC(payload: ConvertIlToCRequest): Promise<ConvertIlToCResponse> {
  return request<ConvertIlToCResponse>("/convert/il-to-c", { method: "POST", body: JSON.stringify(payload) });
}

export async function convertIrToTbLink(ir: unknown): Promise<string> {
  const res = await fetch(`${API_BASE}/convert/ir-to-tblink`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ ir }),
  });
  if (!res.ok) {
    let detail = `${res.status} ${res.statusText}`;
    try {
      const body = (await res.json()) as { detail?: unknown };
      if (body?.detail) {
        if (typeof body.detail === "string") detail = body.detail;
        else {
          const failure = body.detail as { message?: string; attempts?: { issues?: { message?: string }[] }[]; validation_report?: string };
          detail = failure.attempts ? `${failure.message ?? "模型校验失败"} 已尝试 ${failure.attempts.length} 次。${failure.attempts.at(-1)?.issues?.map(issue => issue.message).join("；") ?? ""} 诊断记录：${failure.validation_report ?? ""}` : JSON.stringify(body.detail);
        }
      }
    } catch {
      // ignore
    }
    throw new Error(detail);
  }
  return await res.text();
}

export async function convertIrToTbLinkFile(ir: unknown, outPath: string): Promise<{ out_path: string; xml: string }> {
  return request<{ out_path: string; xml: string }>("/convert/ir-to-tblink-file", {
    method: "POST",
    body: JSON.stringify({ ir, out_path: outPath }),
  });
}

export async function nextOutputDir(base = "outputs"): Promise<string> {
  const res = await request<{ dir: string }>(`/runs/next-output-dir?base=${encodeURIComponent(base)}`, { method: "GET" });
  return res.dir;
}

export async function runCodegenPipeline(payload: RunCodegenPipelineRequest): Promise<RunCodegenPipelineResponse> {
  return request<RunCodegenPipelineResponse>("/pipeline/codegen", { method: "POST", body: JSON.stringify(payload) });
}

export async function validateSubsystemIr(payload: ValidateSubsystemIrRequest): Promise<ValidateSubsystemIrResponse> {
  return request<ValidateSubsystemIrResponse>("/validate/subsystem-ir", { method: "POST", body: JSON.stringify(payload) });
}

export async function generateSubsystemIrIterative(
  payload: GenerateSubsystemIrIterativeRequest,
): Promise<GenerateSubsystemIrIterativeResponse> {
  return request<GenerateSubsystemIrIterativeResponse>("/generate/subsystem-ir-iterative", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function convertCToArduino(payload: ConvertCToArduinoRequest): Promise<ConvertCToArduinoResponse> {
  return request<ConvertCToArduinoResponse>("/convert/c-to-arduino", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function compileArduinoCode(payload: ArduinoCompileRequest): Promise<ArduinoCompileResponse> {
  return request<ArduinoCompileResponse>("/arduino/compile", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function updateArduinoModuleCode(
  payload: ArduinoUpdateModuleCodeRequest,
): Promise<ArduinoUpdateModuleCodeResponse> {
  return request<ArduinoUpdateModuleCodeResponse>("/arduino/update-module-code", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function listArduinoParams(payload: ArduinoListParamsRequest): Promise<ArduinoListParamsResponse> {
  return request<ArduinoListParamsResponse>("/arduino/params/list", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function updateArduinoParams(payload: ArduinoUpdateParamsRequest): Promise<ArduinoUpdateParamsResponse> {
  return request<ArduinoUpdateParamsResponse>("/arduino/params/update", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function uploadArduinoCode(payload: ArduinoUploadRequest): Promise<ArduinoUploadResponse> {
  return request<ArduinoUploadResponse>("/arduino/upload", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function getArduinoDevices(): Promise<ArduinoDevicesResponse> {
  return request<ArduinoDevicesResponse>("/arduino/devices", {
    method: "GET",
  });
}

export async function getArduinoCode(sketchPath = "outputs/arduino_output.ino"): Promise<ArduinoCodeResponse> {
  return request<ArduinoCodeResponse>(`/arduino/code?sketch_path=${encodeURIComponent(sketchPath)}`, {
    method: "GET",
  });
}

export async function readArduinoSerial(payload: ArduinoSerialReadRequest): Promise<ArduinoSerialReadResponse> {
  return request<ArduinoSerialReadResponse>("/arduino/serial/read", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function parseDiagnosticsLog(payload: DiagnosticParseRequest): Promise<DiagnosticParseResponse> {
  return request<DiagnosticParseResponse>("/diagnostics/parse", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function suggestDiagnosticFix(payload: DiagnosticSuggestFixRequest): Promise<DiagnosticSuggestFixResponse> {
  return request<DiagnosticSuggestFixResponse>("/diagnostics/suggest-fix", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function generateModuleBlueprint(
  payload: GenerateModuleBlueprintRequest,
): Promise<GenerateModuleBlueprintResponse> {
  return request<GenerateModuleBlueprintResponse>("/generate/module-blueprint", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function getSystemState(sessionId: string): Promise<GetSystemStateResponse> {
  return request<GetSystemStateResponse>(`/system-state?session_id=${encodeURIComponent(sessionId)}`, {
    method: "GET",
  });
}

export async function saveSystemState(payload: SaveSystemStateRequest): Promise<GetSystemStateResponse> {
  return request<GetSystemStateResponse>("/system-state/save", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function patchSystemState(payload: PatchSystemStateRequest): Promise<PatchSystemStateResponse> {
  return request<PatchSystemStateResponse>("/system-state/patch", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function decideInteractionMode(payload: DecideModeRequest): Promise<DecideModeResponse> {
  return request<DecideModeResponse>("/interaction/decide-mode", {
    method: "POST",
    body: JSON.stringify(payload),
  });
}

export async function compileSysml(payload: { system_sysml2: string; interface_contract?: InterfaceContract; root?: string; out_dir?: string }): Promise<GenerateResponse> {
  return request<GenerateResponse>("/compile/sysml2", { method: "POST", body: JSON.stringify(payload) });
}

export async function prepareInterfaceContract(payload: {
  requirement: string;
    board?: string;
  pin_definitions: Record<string, string>;
  interface_contract?: InterfaceContract;
  previous_contract?: InterfaceContract;
  model?: string;
}): Promise<{ interface_contract: InterfaceContract; contract_sha256: string; provenance: string; functional_status: string; interface_registry: InterfaceRegistry; registered_relationships: string; registry_paths: { interface_registry: string; serial_registry: string } }> {
  return request("/generate/interface-contract", { method: "POST", body: JSON.stringify(payload) });
}
