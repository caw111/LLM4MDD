import {
  Bot,
  Cable,
  ChevronRight,
  Cpu,
  FileCode,
  FolderOpen,
  Save,
  Send,
  Server,
  Sidebar,
  Sparkles,
  Trash2,
  X,
} from "lucide-react";
import { useEffect, useMemo, useRef, useState, type MouseEvent as ReactMouseEvent } from "react";
import toast, { Toaster } from "react-hot-toast";
import {
  type ArduinoParamItem,
  compileArduinoCode,
  decideInteractionMode,
  convertCToArduino,
  compileSysml,
  convertIlToC,
  type DiagnosticEvent,
  type DiagnosticSuggestFixResponse,
  getArduinoCode,
  getArduinoDevices,
  generateModuleBlueprint,
  generateSysml2Bundle,
  prepareInterfaceContract,
  type InterfaceContract,
  type ProvenanceReceipt,
  getSystemState,
  listArduinoParams,
  parseDiagnosticsLog,
  getPrompt,
  health,
  readArduinoSerial,
  saveSystemState,
  suggestDiagnosticFix,
  uploadArduinoCode,
  updateArduinoParams,
} from "./lib/api";
import { CodeBlock } from "./components/CodeBlock";
import { cn } from "./lib/cn";
import { executeDynamicModule } from "./lib/moduleRegistry";
import {
  autoLayoutModules,
  DEFAULT_BLUEPRINT,
  type FrontendModuleSpec,
  normalizeBlueprint,
  tryParseBlueprint,
  type ModuleBlueprint,
  type ModuleId,
} from "./lib/moduleBlueprint";

type CardStatus = "idle" | "running" | "success" | "error";

type ChatRole = "user" | "assistant";

type FlowCard = {
  id: ModuleId;
  title: string;
  subtitle: string;
  moduleType?: string;
  executor?: string;
  pinDefinition: string;
  x: number;
  y: number;
  w: number;
  h: number;
  status: CardStatus;
  bottomActivity: string;
  logs: string[];
  code: string;
};

type Connection = {
  from: ModuleId;
  to: ModuleId;
  relation?: string;
  logic?: string;
};

type ChatMessage = {
  id: string;
  role: ChatRole;
  content: string;
};

type AiHistoryMessage = {
  role: "user" | "assistant" | "system";
  content: string;
};

const ENGLISH_TEXT: Record<string, string> = {
  "收起AI": "Hide AI",
  "展开AI": "Show AI",
  "打开系统": "Open System",
  "保存系统": "Save System",
  "清空画布": "Clear Canvas",
  "后台生成": "Generate in Background",
  "查看代码": "View Code",
  "编译代码": "Compile Code",
  "部署到板子": "Deploy to Board",
  "模块参数": "Module Parameters",
  "应用中...": "Applying...",
  "应用参数": "Apply Parameters",
  "模块当前后台活动": "Current Module Activity",
  "AI 交互框": "AI Panel",
  "MVP 诊断回传": "MVP Diagnostics",
  "开始串口回填": "Start Serial Fill",
  "停止串口回填": "Stop Serial Fill",
  "清空日志": "Clear Log",
  "解析中...": "Parsing...",
  "解析日志": "Parse Log",
  "生成中...": "Generating...",
  "生成修复建议": "Generate Fix Suggestions",
  "故障摘要": "Fault Summary",
  "查看建议提示词": "View Suggested Prompt",
  "展开AI交互框": "Show AI Panel",
  "板型 FQBN": "Board FQBN",
  "串口": "Port",
  "刷新中...": "Refreshing...",
  "刷新设备": "Refresh Devices",
  "未检测到串口": "No Port Detected",
  "开始部署": "Start Deploy",
  "部署中...": "Deploying...",
  "输入采集模块": "Input Module",
  "数据处理模块": "Processing Module",
  "决策控制模块": "Decision Module",
  "结果输出模块": "Output Module",
  "执行器模块": "Actuator Module",
  "等待执行": "Waiting",
  "未执行": "Not Run",
  "数据流": "Data Flow",
  "控制流": "Control Flow",
  "触发": "Trigger",
  "反馈": "Feedback",
  "依赖": "Dependency",
  "请根据需求自动生成可运行的嵌入式控制系统，并输出 Arduino 代码。":
    "Please generate a runnable embedded control system from the requirements and output Arduino code.",
  "我是AI助手。你可以说: 帮我优化Step2参数, 或者 执行全流程并汇总测试结果。":
    "I am the AI assistant. You can say: optimize Step2 parameters, or run the full pipeline and summarize test results.",
  "由需求自动生成的模块": "Module generated from requirements",
  "已加载历史系统 v": "Loaded previous system v",
  "已按增量模式执行 Step1~Step5（跳过 Step0）。":
    "Incremental mode completed Step1~Step5 (Step0 skipped).",
  "模式判定失败": "Mode detection failed",
  "读取文件失败": "Failed to read file",
  "系统已打开": "System opened",
  "系统已保存": "System saved",
  "画布已清空": "Canvas cleared",
  "请输入需求后再执行": "Please enter requirements before running",
  "正在应用模块参数...": "Applying module parameters...",
  "模块蓝图已更新": "Module blueprint updated",
  "修复建议已生成": "Fix suggestions generated",
  "后端规划失败": "Backend planning failed",
  "后端蓝图生成失败": "Backend blueprint generation failed",
  "系统文件解析失败": "Failed to parse system file",
  "编译失败，但没有返回详细日志。": "Compilation failed, but no detailed logs were returned.",
  "部署失败，但没有返回详细日志。": "Deployment failed, but no detailed logs were returned.",
  "部署失败，请查看日志": "Deployment failed, please check logs",
  "正在编译 Arduino 代码...": "Compiling Arduino code...",
  "Arduino 代码编译成功": "Arduino code compiled successfully",
  "Arduino 代码编译失败": "Arduino code compilation failed",
  "正在部署到开发板...": "Deploying to board...",
  "请先粘贴串口日志": "Please paste serial logs first",
  "正在解析诊断日志...": "Parsing diagnostic logs...",
  "正在生成修复建议...": "Generating fix suggestions...",
  "请先解析日志，再生成修复建议": "Parse logs before generating fix suggestions",
  "流程中断: 无法从后端获取模块蓝图": "Flow interrupted: failed to fetch module blueprint from backend",
  "增量模式: 跳过 Step0，直接执行 Step1~Step5":
    "Incremental mode: skip Step0 and run Step1~Step5 directly",
  "已按重生成模式执行 Step0~Step5。": "Regeneration mode completed Step0~Step5.",
  "模块规划完成": "Module planning completed",
  "输入设备 A0": "Input Device A0",
  "输出设备 D3": "Output Device D3",
  "执行器 D8": "Actuator D8",
  "未选择": "Not Selected",
  "波特率": "Baud Rate",
  "状态": "Status",
  "读取中": "Reading",
  "已开启": "Enabled",
  "未开启": "Disabled",
  "请先选择或输入串口号，例如 COM4": "Please select or enter a port, e.g., COM4",
  "串口日志会自动回填到这里，也可以手动粘贴，例如: [DIAG]|ts=...|lvl=ERROR|code=...|module=...|msg=...":
    "Serial logs will autofill here; you can also paste them manually, e.g., [DIAG]|ts=...|lvl=ERROR|code=...|module=...|msg=...",
  "输入你的需求，例如: 小车根据按键前后左右运动并在异常时蜂鸣告警":
    "Enter your requirements, e.g., the car moves with buttons and beeps on exceptions",
  "模式由大模型自动判定: regenerate 跑 Step0~5；incremental 跳过 Step0，仅跑 Step1~5。":
    "Mode is decided automatically: regenerate runs Step0~5; incremental skips Step0 and runs Step1~5.",
  "当前会优先读取 Windows 侧能看到的串口并提供下拉选择，仍然保留手动输入端口作为兜底。":
    "Ports visible in Windows are prioritized for the dropdown, with manual entry as a fallback.",
  "负责输入准备与初始化。": "Handles input preparation and initialization.",
  "负责中间处理逻辑。": "Handles intermediate processing logic.",
  "负责执行告警或执行器控制动作。": "Handles alerts or actuator control actions.",
  "负责执行器或外设动作。": "Handles actuators or peripheral actions.",
  "负责规则判断与状态决策输出。": "Handles rule checks and state decision output.",
  "负责把状态映射为可视化输出。": "Maps state to visual output.",
  "负责告警策略和通知输出。": "Handles alert strategy and notification output.",
  "通用模块，等待按需求补全实现。": "Generic module, awaiting requirement-specific implementation.",
  "负责采集原始输入并完成基础预处理。": "Collects raw input and performs basic preprocessing.",
  "负责提取特征并输出可判定的数据。": "Extracts features and outputs decision-ready data.",
  "负责决策与状态更新。": "Handles decision-making and state updates.",
  "负责输出与结果呈现。": "Handles output and result presentation.",
  "负责把需求结构化为系统模型与需求映射，作为后续代码生成输入。":
    "Structures requirements into system models and requirement maps for later code generation.",
  "负责信号/数据特征提取，输出分析结果供后续决策。":
    "Extracts signal/data features and outputs analysis for downstream decisions.",
  "负责根据条件与规则做状态判定，并输出控制结果。":
    "Determines state based on conditions and rules, and outputs control results.",
  "负责将告警等级映射为数码管显示内容，并执行显示刷新逻辑。":
    "Maps alert levels to seven-seg display content and refreshes the display.",
  "负责将告警等级转换为蜂鸣器频率与鸣叫节奏策略。":
    "Converts alert levels to buzzer frequency and cadence strategy.",
  "负责输出中间表示（IL/XML），用于后续代码生成与跨工具链处理。":
    "Outputs intermediate representation (IL/XML) for later codegen and toolchain processing.",
  "负责把关键状态输出到显示或交互界面。": "Outputs key states to display or UI.",
  "负责采集或接收输入信号，并输出标准化输入。": "Collects/receives input signals and outputs normalized input.",
  "负责进行特征计算、状态估计或中间结果生成。": "Performs feature computation, state estimation, or intermediate output.",
  "负责根据规则/条件做判断并输出控制决策。": "Evaluates rules/conditions and outputs control decisions.",
  "模块参数已映射到 Arduino 输出文件": "Module parameters mapped to the Arduino output file",
  "从已有 Arduino 产物自动提取的模块代码": "Module code auto-extracted from existing Arduino outputs",
  "正在调用大模型规划模块...": "Calling the model to plan modules...",
  "负责读取传感器/按键/通信输入并做基础归一化": "Collects sensor/button/communication input and normalizes it.",
  "执行特征提取、状态计算或控制前处理": "Performs feature extraction, state computation, or control pre-processing.",
  "基于规则/条件输出控制决策": "Outputs control decisions based on rules/conditions.",
  "将决策结果映射到显示/通信/日志输出": "Maps decisions to display/communication/log output.",
  "根据决策驱动执行器（电机/继电器/蜂鸣器等）": "Drives actuators based on decisions (motor/relay/buzzer, etc.).",
  "由大模型规划生成": "Planned by the model",
  "将该提示词投喂给增量修复流程": "Feed this prompt to the incremental fix flow.",
  "完整的 Arduino 生成代码": "Full generated Arduino code",
  "按模块即时生成的代码（无需完整流程）": "Module code generated on demand (no full pipeline required)",
  "麦克风输入模块": "Microphone Input Module",
  "噪声映射模块": "Noise Mapping Module",
  "阈值判断模块": "Threshold Check Module",
  "蜂鸣器警报模块": "Buzzer Alert Module",
  "显示模块": "Display Module",
  "读取麦克风输入的噪声信号": "Reads noise signal from microphone input.",
  "将噪声大小映射到1-100之间": "Maps noise level to 1-100.",
  "判断噪声是否超过70": "Checks whether noise exceeds 70.",
  "发出警报声以提示用户": "Emits an alert tone to notify the user.",
  "显示当前噪声级别": "Displays the current noise level.",
  "麦克风输入的噪声流入噪声映射模块": "Microphone noise input flows into the Noise Mapping Module.",
  "映射后的噪声级别流入阈值判断模块": "Mapped noise level flows into the Threshold Check Module.",
  "当噪声超过70时触发警报": "Trigger the alert when noise exceeds 70.",
  "映射后的噪声级别流入显示模块": "Mapped noise level flows into the Display Module.",
  "当前模块暂无可展示的具体代码。\n// 可先执行一次代码生成，或确保 outputs/arduino_output.ino 已存在。":
    "No code available for this module yet.\n// Run code generation once, or ensure outputs/arduino_output.ino exists.",
  "确定要清空当前画布吗？这会移除当前板块和连线。":
    "Are you sure you want to clear the canvas? This removes current modules and connections.",
};

const ENGLISH_TEXT_PATTERNS: Array<[RegExp, string]> = [
  [/^Backend:\s*/i, "Backend: "],
  [/^AI\s+.*/, "AI Panel"],
  [/^MVP\s+.*/, "MVP Diagnostics"],
  [/^Port|^串口|^涓插彛/, "Port"],
  [/^波特率|^娉㈢壒鐜?/, "Baud Rate"],
  [/^状态|^鐘舵€?/, "Status"],
  [/读取中|璇诲彇涓?/, "Reading"],
  [/已开启|宸插紑鍚?/, "Enabled"],
  [/未开启|鏈紑鍚?/, "Disabled"],
  [/未选择|鏈€夋嫨/, "Not Selected"],
  [/参数|鍙傛暟/, "Parameter"],
  [/日志|鏃ュ織/, "Log"],
  [/诊断|璇婃柇/, "Diagnostics"],
  [/需求|闇€姹?/, "Requirement"],
  [/提示词|鎻愮ず璇?/, "Prompt"],
  [/生成|鐢熸垚/, "Generate"],
  [/部署|閮ㄧ讲/, "Deploy"],
  [/编译|缂栬瘧/, "Compile"],
  [/查看|鏌ョ湅/, "View"],
  [/清空|娓呯┖/, "Clear"],
  [/打开|鎵撳紑/, "Open"],
  [/保存|淇濆瓨/, "Save"],
  [/刷新|鍒锋柊/, "Refresh"],
  [/应用|搴旂敤/, "Apply"],
];

const ENGLISH_TEXT_PARTIALS: Array<[RegExp, string]> = [
  [/执行模块:\s*/g, "Executing module: "],
  [/模块执行器:\s*/g, "Module executor: "],
  [/执行失败:\s*/g, "Execution failed: "],
  [/按需生成失败:\s*/g, "On-demand generation failed: "],
  [/按需生成代码:\s*/g, "On-demand code generation: "],
  [/模块执行完成/g, "module execution completed"],
  [/已完成模块生成/g, "module generation completed"],
  [/^执行器\s*/g, "Executor "],
  [/模块按需代码生成完成/g, "On-demand module code generated"],
  [/双击触发按需代码生成/g, "Double-click to trigger on-demand code generation"],
  [/模块代码已生成:\s*/g, "Module code generated: "],
  [/模块逻辑已生成完成/g, "Module logic generation completed"],
  [/参数应用失败:\s*/g, "Parameter apply failed: "],
  [/清空后保存失败:\s*/g, "Save after clear failed: "],
  [/设备检测失败:\s*/g, "Device detection failed: "],
  [/日志解析失败:\s*/g, "Log parsing failed: "],
  [/建议生成失败:\s*/g, "Suggestion generation failed: "],
];

const EN_MODULE_TITLE_BY_ID: Record<string, string> = {
  step1: "Input Module",
  step2: "Processing Module",
  step3: "Decision Module",
  step4: "Output Module",
  sim: "Actuator Module",
};

const EN_MODULE_TITLE_BY_TYPE: Record<string, string> = {
  source: "Input Module",
  analysis: "Processing Module",
  decision: "Decision Module",
  display: "Output Module",
  alert: "Actuator Module",
  custom: "Custom Module",
};

const EN_MODULE_SUBTITLE_BY_ID: Record<string, string> = {
  step1: "Handles input preparation and initialization.",
  step2: "Handles intermediate processing logic.",
  step3: "Handles decision-making and state updates.",
  step4: "Handles output and result presentation.",
  sim: "Handles actuators or peripheral actions.",
};

const EN_MODULE_SUBTITLE_BY_TYPE: Record<string, string> = {
  source: "Collects/receives input signals and outputs normalized input.",
  analysis: "Performs feature computation, state estimation, or intermediate output.",
  decision: "Determines state based on rules/conditions and outputs control decisions.",
  display: "Outputs key states to the display or UI.",
  alert: "Handles alerts or actuator control actions.",
  custom: "Generic module, awaiting requirement-specific implementation.",
};

const EDGE_LOGIC_PATTERNS: Array<[RegExp, string]> = [
  [/^(.+?)流入(.+?)模块$/g, "$1 flows into the $2 module"],
  [/^映射后的(.+?)流入(.+?)模块$/g, "Mapped $1 flows into the $2 module"],
  [/^当(.+?)时触发(.+)$/g, "Trigger $2 when $1"],
];

const EDGE_WORD_REPLACEMENTS: Array<[RegExp, string]> = [
  [/麦克风输入/g, "microphone input"],
  [/噪声级别/g, "noise level"],
  [/噪声/g, "noise"],
  [/映射后/g, "mapped"],
  [/映射/g, "mapping"],
  [/流入/g, "flows into"],
  [/阈值判断/g, "threshold check"],
  [/阈值/g, "threshold"],
  [/判断/g, "check"],
  [/超过/g, "exceeds"],
  [/触发/g, "trigger"],
  [/警报/g, "alarm"],
  [/蜂鸣器/g, "buzzer"],
  [/显示/g, "display"],
  [/模块/g, "module"],
];

function hasNonEnglishText(text: string) {
  return /[\u4e00-\u9fff�€]/.test(text);
}

function translateUiText(text: string) {
  const leading = text.match(/^\s*/)?.[0] ?? "";
  const trailing = text.match(/\s*$/)?.[0] ?? "";
  const value = text.trim();
  if (!value) return text;
  if (ENGLISH_TEXT[value]) return `${leading}${ENGLISH_TEXT[value]}${trailing}`;
  for (const [pattern, replacement] of ENGLISH_TEXT_PATTERNS) {
    if (pattern.test(value)) return `${leading}${replacement}${trailing}`;
  }
  let replaced = value;
  for (const [pattern, replacement] of ENGLISH_TEXT_PARTIALS) {
    replaced = replaced.replace(pattern, replacement);
  }
  if (replaced !== value) return `${leading}${replaced}${trailing}`;
  return text;
}

function resolveEnglishModuleTitle(card: Pick<FlowCard, "id" | "title" | "moduleType">): string {
  const raw = (card.title || "").trim();
  if (ENGLISH_TEXT[raw]) return ENGLISH_TEXT[raw];
  const byId = EN_MODULE_TITLE_BY_ID[card.id];
  const byType = EN_MODULE_TITLE_BY_TYPE[(card.moduleType || "").toLowerCase()];
  const fallback = byId || byType || raw;
  if (!raw || raw.toLowerCase() === "module" || raw === "模块") return fallback;
  if (hasNonEnglishText(raw)) return byId || byType || EN_MODULE_TITLE_BY_TYPE.custom;
  return raw;
}

function resolveEnglishModuleSubtitle(card: Pick<FlowCard, "id" | "subtitle" | "moduleType">): string {
  const raw = (card.subtitle || "").trim();
  if (ENGLISH_TEXT[raw]) return ENGLISH_TEXT[raw];
  const byId = EN_MODULE_SUBTITLE_BY_ID[card.id];
  const byType = EN_MODULE_SUBTITLE_BY_TYPE[(card.moduleType || "").toLowerCase()];
  const fallback = byId || byType || raw;
  if (!raw) return fallback;
  if (hasNonEnglishText(raw)) return byId || byType || EN_MODULE_SUBTITLE_BY_TYPE.custom;
  return raw;
}

function translateConnectionLabel(edge: Connection, cardMap: Map<ModuleId, FlowCard>): string {
  let result = formatConnectionLabel(edge);
  result = translateUiText(result);

  for (const card of cardMap.values()) {
    if (!card.title || !hasNonEnglishText(card.title)) continue;
    const replacement = resolveEnglishModuleTitle(card);
    if (replacement) {
      result = result.split(card.title).join(replacement);
    }
  }

  for (const [pattern, replacement] of EDGE_LOGIC_PATTERNS) {
    result = result.replace(pattern, replacement);
  }

  for (const [pattern, replacement] of EDGE_WORD_REPLACEMENTS) {
    result = result.replace(pattern, replacement);
  }

  result = result.replace(/[\u4e00-\u9fff]/g, "").replace(/\s+/g, " ").trim();
  return result || translateUiText(formatConnectionLabel(edge));
}

const originalTextMap = new WeakMap<Text, string>();

function translateElementToEnglish(root: ParentNode = document.body) {
  const skipTags = new Set(["SCRIPT", "STYLE", "TEXTAREA", "INPUT", "CODE", "PRE"]);
  const walker = document.createTreeWalker(root, NodeFilter.SHOW_TEXT, {
    acceptNode(node) {
      const parent = node.parentElement;
      if (!parent || skipTags.has(parent.tagName)) return NodeFilter.FILTER_REJECT;
      return hasNonEnglishText(node.nodeValue || "") ? NodeFilter.FILTER_ACCEPT : NodeFilter.FILTER_SKIP;
    },
  });

  const nodes: Text[] = [];
  while (walker.nextNode()) nodes.push(walker.currentNode as Text);
  nodes.forEach((node) => {
    if (!originalTextMap.has(node)) {
      originalTextMap.set(node, node.nodeValue || "");
    }
    node.nodeValue = translateUiText(node.nodeValue || "");
  });

  root.querySelectorAll?.("[placeholder], [title], [aria-label]").forEach((node) => {
    ["placeholder", "title", "aria-label"].forEach((attr) => {
      const value = node.getAttribute(attr);
      if (!value || !hasNonEnglishText(value)) return;
      const cacheKey = `data-i18n-original-${attr}`;
      if (!node.getAttribute(cacheKey)) node.setAttribute(cacheKey, value);
      node.setAttribute(attr, translateUiText(value));
    });
  });
}

function restoreTranslatedText(root: ParentNode = document.body) {
  const skipTags = new Set(["SCRIPT", "STYLE", "TEXTAREA", "INPUT", "CODE", "PRE"]);
  const walker = document.createTreeWalker(root, NodeFilter.SHOW_TEXT, {
    acceptNode(node) {
      const parent = node.parentElement;
      if (!parent || skipTags.has(parent.tagName)) return NodeFilter.FILTER_REJECT;
      return originalTextMap.has(node as Text) ? NodeFilter.FILTER_ACCEPT : NodeFilter.FILTER_SKIP;
    },
  });

  const nodes: Text[] = [];
  while (walker.nextNode()) nodes.push(walker.currentNode as Text);
  nodes.forEach((node) => {
    const original = originalTextMap.get(node);
    if (original !== undefined) {
      node.nodeValue = original;
      originalTextMap.delete(node);
    }
  });

  root.querySelectorAll?.("[data-i18n-original-placeholder],[data-i18n-original-title],[data-i18n-original-aria-label]").forEach((node) => {
    ["placeholder", "title", "aria-label"].forEach((attr) => {
      const cacheKey = `data-i18n-original-${attr}`;
      const original = node.getAttribute(cacheKey);
      if (original) {
        node.setAttribute(attr, original);
        node.removeAttribute(cacheKey);
      }
    });
  });
}

function splitArduinoCodeByModules(
  arduinoCode: string,
  modules: Array<Pick<FlowCard, "id" | "title" | "subtitle" | "moduleType">>,
): Record<string, string> {
  const code = (arduinoCode || "").trim();
  if (!modules.length) return {};

  if (!code) {
    const empty: Record<string, string> = {};
    modules.forEach((item) => {
      empty[item.id] = "";
    });
    return empty;
  }

  const lines = code.split(/\r?\n/);
  const controlRe = /^\s*(if|for|while|switch|else|do)\b/;
  const fnNameRe = /([A-Za-z_][A-Za-z0-9_]*)\s*\([^;{}]*\)\s*\{/;
  const fnBlocks: Array<{ name: string; text: string; signature: string }> = [];
  const preamble: string[] = [];

  const countBraceDelta = (text: string) => {
    let delta = 0;
    for (const ch of text) {
      if (ch === "{") delta += 1;
      else if (ch === "}") delta -= 1;
    }
    return delta;
  };

  let i = 0;
  while (i < lines.length) {
    const line = lines[i];
    const looksLikeSig = line.includes("(") && !controlRe.test(line) && !line.trim().endsWith(";");
    let fnStart = -1;
    let fnName = "";

    if (looksLikeSig) {
      for (let j = i; j < Math.min(i + 6, lines.length); j += 1) {
        const maybe = lines.slice(i, j + 1).join(" ");
        const m = maybe.match(fnNameRe);
        if (m) {
          fnStart = i;
          fnName = m[1];
          break;
        }
      }
    }

    if (fnStart >= 0) {
      let end = fnStart;
      let depth = 0;
      let opened = false;
      while (end < lines.length) {
        const d = countBraceDelta(lines[end]);
        if (d > 0) opened = true;
        depth += d;
        end += 1;
        if (opened && depth <= 0) break;
      }
      const text = lines.slice(fnStart, end).join("\n").trim();
      fnBlocks.push({
        name: fnName.toLowerCase(),
        text,
        signature: lines[fnStart]?.trim() || fnName,
      });
      i = end;
      continue;
    }

    if (fnBlocks.length === 0) preamble.push(line);
    i += 1;
  }

  const result: Record<string, string> = {};
  const fullArduino = code.trim();
  const preambleText = preamble.join("\n").trim();
  const assigned = new Set<number>();

  const scoreByKeywords = (text: string, keywords: string[]) => {
    const lower = text.toLowerCase();
    return keywords.reduce((sum, kw) => sum + (lower.includes(kw) ? 1 : 0), 0);
  };

  const pickFunction = (keywords: string[]) => {
    let bestIdx = -1;
    let bestScore = -1;
    for (let idx = 0; idx < fnBlocks.length; idx += 1) {
      if (assigned.has(idx)) continue;
      const fn = fnBlocks[idx];
      const score = scoreByKeywords(`${fn.name}\n${fn.signature}\n${fn.text}`, keywords);
      if (score > bestScore) {
        bestScore = score;
        bestIdx = idx;
      }
    }
    if (bestIdx >= 0) {
      assigned.add(bestIdx);
      return fnBlocks[bestIdx];
    }
    return null;
  };

  const coreIds = ["step1", "step2", "step3", "step4", "sim"];
  const isCoreFive = coreIds.every((id) => modules.some((m) => m.id === id));

  if (isCoreFive && fnBlocks.length > 0) {
    const strictNames: Record<string, string[]> = {
      step1: ["read_input", "collect_input", "sample_input", "input_update"],
      step2: ["process_data", "analyze_data", "compute_state", "update_state"],
      step3: ["make_decision", "control_decision", "evaluate_rule", "dispatch_action"],
      step4: ["update_output", "render_output", "publish_output", "show_status"],
      sim: ["drive_actuator", "apply_command", "control_actuator", "trigger_action"],
    };

    const pickByStrictName = (names: string[]) => {
      for (let idx = 0; idx < fnBlocks.length; idx += 1) {
        if (assigned.has(idx)) continue;
        const name = fnBlocks[idx].name.toLowerCase();
        if (names.some((n) => name === n || name.includes(n))) {
          assigned.add(idx);
          return fnBlocks[idx];
        }
      }
      return null;
    };

    const mapping: Record<string, string[]> = {
      step1: ["setup", "init", "input", "read", "adc", "sensor", "sample"],
      step2: ["analysis", "process", "compute", "filter", "feature", "state"],
      step3: ["loop", "decision", "rule", "control", "condition", "dispatch"],
      step4: ["display", "output", "render", "publish", "serial", "lcd", "oled"],
      sim: ["actuator", "motor", "relay", "buzzer", "servo", "tone", "apply"],
    };

    modules.forEach((mod) => {
      const fn =
        (strictNames[mod.id] ? pickByStrictName(strictNames[mod.id]) : null) ||
        pickFunction(mapping[mod.id] || [mod.id.toLowerCase(), mod.title.toLowerCase()]);
      if (fn) {
        result[mod.id] = [
          "// Extracted from final Arduino code",
          ...(mod.id === "step1" && preambleText ? ["", "// Global declarations", preambleText] : []),
          "",
          fn.text,
        ].join("\n").trim();
      }
    });
  }

  const remainingFns = fnBlocks.filter((_, idx) => !assigned.has(idx));
  let ptr = 0;
  modules.forEach((mod) => {
    if (result[mod.id]?.trim()) return;

    const fallbackFn = remainingFns[ptr];
    if (fallbackFn) {
      ptr += 1;
      result[mod.id] = [
        "// Extracted from final Arduino code",
        fallbackFn.text,
      ].join("\n\n");
      return;
    }

    result[mod.id] = [
      `// Fallback for module: ${mod.title}`,
      "// No distinct function could be isolated; using full final Arduino code.",
      fullArduino,
    ].join("\n");
  });

  return result;
}


function summarizeCodeRole(code: string, fallback: string) {
  const text = (code || "").toLowerCase();
  if (!text.trim()) return fallback;

  if (text.includes("sysml") || text.includes("requirement") || text.includes("req_test")) {
    return "负责把需求结构化为系统模型与需求映射，作为后续代码生成输入。";
  }
  if (text.includes("bandpass") || text.includes("filter") || text.includes("rms") || text.includes("peak")) {
    return "负责信号/数据特征提取，输出分析结果供后续决策。";
  }
  if (text.includes("threshold") || text.includes("level") || text.includes("if") || text.includes("alarm")) {
    return "负责根据条件与规则做状态判定，并输出控制结果。";
  }
  if (text.includes("segment") || text.includes("display") || text.includes("digit") || text.includes("programpage")) {
    return "负责将告警等级映射为数码管显示内容，并执行显示刷新逻辑。";
  }
  if (text.includes("buzzer") || text.includes("hz") || text.includes("pulse") || text.includes("beep")) {
    return "负责将告警等级转换为蜂鸣器频率与鸣叫节奏策略。";
  }
  if (text.includes("<il") || text.includes(".il") || text.includes("xml")) {
    return "负责输出中间表示（IL/XML），用于后续代码生成与跨工具链处理。";
  }

  return fallback;
}

function buildCardsFromBlueprint(blueprint: ModuleBlueprint): FlowCard[] {
  const normalized = normalizeBlueprint(blueprint);
  const layout = autoLayoutModules(
    normalized.modules.map((item) => item.id),
    normalized.connections,
  );
  return normalized.modules.map((item) => ({
    id: item.id,
    title: item.title,
    subtitle: item.subtitle,
    moduleType: item.moduleType,
    executor: item.executor,
    pinDefinition: defaultPinText(item.id),
    x: layout[item.id]?.x ?? 48,
    y: layout[item.id]?.y ?? 88,
    w: 290,
    h: 260,
    status: "idle",
    bottomActivity: "等待执行",
    logs: ["未执行"],
    code: item.codeSeed,
  }));
}

function statusClass(status: CardStatus) {
  if (status === "success") return "border-emerald-300 bg-emerald-50 text-emerald-700";
  if (status === "error") return "border-rose-300 bg-rose-50 text-rose-700";
  if (status === "running") return "border-amber-300 bg-amber-50 text-amber-700";
  return "border-slate-300 bg-slate-50 text-slate-600";
}

function nowTag() {
  const d = new Date();
  return `${d.getHours().toString().padStart(2, "0")}:${d.getMinutes().toString().padStart(2, "0")}:${d
    .getSeconds()
    .toString()
    .padStart(2, "0")}`;
}

function defaultPinText(moduleId: string): string {
  if (moduleId === "step1") return "输入设备 A0";
  if (moduleId === "step4") return "输出设备 D3";
  if (moduleId === "sim") return "执行器 D8";
  return "";
}

function isPinLikeParam(item: ArduinoParamItem): boolean {
  const joined = `${item.param_id} ${item.name} ${item.zh_name || ""}`.toLowerCase();
  if (joined.includes("引脚")) return true;
  return /(pin|clk|dio|scl|sda|miso|mosi|cs|ss|rx|tx)/.test(joined);
}

function buildModulePinDefinition(params: ArduinoParamItem[]): string {
  const pinItems = (params || []).filter(isPinLikeParam);
  if (!pinItems.length) return "";
  return pinItems.map((item) => `${item.name}=${item.value}`).join(", ");
}

function formatConnectionLabel(edge: Connection): string {
  if (edge.logic?.trim()) return edge.logic.trim();
  const relation = (edge.relation || "data_flow").toLowerCase();
  if (relation === "control_flow") return "控制流";
  if (relation === "trigger") return "触发";
  if (relation === "feedback") return "反馈";
  if (relation === "dependency") return "依赖";
  return "数据流";
}

function buildExecutionOrder(moduleIds: string[], edges: Connection[]): string[] {
  const order: string[] = [];
  const indegree = new Map<string, number>();
  const adjacency = new Map<string, string[]>();

  moduleIds.forEach((id) => {
    indegree.set(id, 0);
    adjacency.set(id, []);
  });

  edges.forEach((edge) => {
    if (!indegree.has(edge.from) || !indegree.has(edge.to) || edge.from === edge.to) return;
    adjacency.get(edge.from)?.push(edge.to);
    indegree.set(edge.to, (indegree.get(edge.to) || 0) + 1);
  });

  const queue: string[] = moduleIds.filter((id) => (indegree.get(id) || 0) === 0);
  while (queue.length) {
    const current = queue.shift() as string;
    order.push(current);
    for (const next of adjacency.get(current) || []) {
      const remain = (indegree.get(next) || 0) - 1;
      indegree.set(next, remain);
      if (remain === 0) queue.push(next);
    }
  }

  if (order.length === moduleIds.length) return order;
  const visited = new Set(order);
  const rest = moduleIds.filter((id) => !visited.has(id));
  return [...order, ...rest];
}

function groupParamsByModule(items: ArduinoParamItem[] | null | undefined): Record<string, ArduinoParamItem[]> {
  const out: Record<string, ArduinoParamItem[]> = {};
  for (const item of items || []) {
    const moduleId = String(item.module_id || "").trim();
    if (!moduleId) continue;
    if (!out[moduleId]) out[moduleId] = [];
    out[moduleId].push({ ...item });
  }
  return out;
}

export default function App() {
  const systemSessionId = "default";
  const [hasCompleteSystem, setHasCompleteSystem] = useState(false);
  const [backendStatus, setBackendStatus] = useState<"checking" | "ok" | "down">("checking");
  const [cards, setCards] = useState<FlowCard[]>(() => buildCardsFromBlueprint(DEFAULT_BLUEPRINT));
  const [connections, setConnections] = useState<Connection[]>(() => DEFAULT_BLUEPRINT.connections);
  const [promptText, setPromptText] = useState("请根据需求自动生成可运行的嵌入式控制系统，并输出 Arduino 代码。");
  const [modulePromptTemplate, setModulePromptTemplate] = useState("");
  const [incrementalStep1Template, setIncrementalStep1Template] = useState("");
  const [incrementalArduinoTemplate, setIncrementalArduinoTemplate] = useState("");

  const [codeModal, setCodeModal] = useState<{
    title: string;
    code: string;
    summary: string;
    moduleId?: ModuleId;
  } | null>(null);
  const [moduleParams, setModuleParams] = useState<Record<string, ArduinoParamItem[]>>({});
  const [savingParamsModuleId, setSavingParamsModuleId] = useState<string | null>(null);
  const [deployModalOpen, setDeployModalOpen] = useState(false);
  const [deployPort, setDeployPort] = useState("COM4");
  const [deployFqbn, setDeployFqbn] = useState("arduino:avr:uno");
  const [isDeploying, setIsDeploying] = useState(false);
  const [deployPorts, setDeployPorts] = useState<Array<{ address: string; label: string; fqbn?: string | null }>>([]);
  const [deployBoards, setDeployBoards] = useState<Array<{ label: string; fqbn: string }>>([]);
  const [isLoadingDeployDevices, setIsLoadingDeployDevices] = useState(false);
  const [diagnosticRawLog, setDiagnosticRawLog] = useState("");
  const [serialBaudRate, setSerialBaudRate] = useState(115200);
  const [serialAutoFillEnabled, setSerialAutoFillEnabled] = useState(false);
  const [isSerialPolling, setIsSerialPolling] = useState(false);
  const [diagnosticEvents, setDiagnosticEvents] = useState<DiagnosticEvent[]>([]);
  const [diagnosticSummary, setDiagnosticSummary] = useState<
    Array<{ code: string; level: string; module: string; count: number; latest_line_no: number }>
  >([]);
  const [diagnosticFix, setDiagnosticFix] = useState<DiagnosticSuggestFixResponse | null>(null);
  const [isParsingDiagnostics, setIsParsingDiagnostics] = useState(false);
  const [isSuggestingFix, setIsSuggestingFix] = useState(false);
  const [isAiPanelOpen, setIsAiPanelOpen] = useState(true);
  const [englishMode, setEnglishMode] = useState(false);
  const [chatInput, setChatInput] = useState("");
  const [chatMessages, setChatMessages] = useState<ChatMessage[]>([
    {
      id: "a1",
      role: "assistant",
      content: "我是AI助手。你可以说: 帮我优化Step2参数, 或者 执行全流程并汇总测试结果。",
    },
  ]);

  const t = (text: string) => (englishMode ? translateUiText(text) : text);
  const displayModuleTitle = (card: FlowCard) => (englishMode ? resolveEnglishModuleTitle(card) : card.title);
  const displayModuleSubtitle = (card: FlowCard) => card.subtitle || (englishMode ? resolveEnglishModuleSubtitle(card) : "暂无功能说明");
  const moduleDetailsRef = useRef<HTMLDialogElement>(null);
  const [detailModule, setDetailModule] = useState<FlowCard | null>(null);
  const [validationStatus, setValidationStatus] = useState("");
  const [validationDiagnostics, setValidationDiagnostics] = useState("");
  const [contractText, setContractText] = useState("");
  const [useEditedContract, setUseEditedContract] = useState(false);
  const [contractDigest, setContractDigest] = useState("");

  const [dragging, setDragging] = useState<{
    id: ModuleId;
    startMouseX: number;
    startMouseY: number;
    startCardX: number;
    startCardY: number;
  } | null>(null);
  const fileInputRef = useRef<HTMLInputElement | null>(null);

  const [stepOutputs, setStepOutputs] = useState<{
    sysml?: string;
    reqTestMap?: Record<string, unknown>;
    reqMap?: string;
    cFile?: string;
    ilFile?: string;
    regenerated?: string;
    arduinoCode?: string;
  }>({});

  useEffect(() => {
    if (!englishMode) {
      restoreTranslatedText();
      return;
    }

    translateElementToEnglish();
    const observer = new MutationObserver(() => {
      window.requestAnimationFrame(() => translateElementToEnglish());
    });
    observer.observe(document.body, {
      childList: true,
      subtree: true,
      characterData: true,
      attributes: true,
      attributeFilter: ["placeholder", "title", "aria-label"],
    });

    return () => observer.disconnect();
  }, [englishMode]);

  useEffect(() => {
    let canceled = false;
    (async () => {
      try {
        await health();
        if (!canceled) setBackendStatus("ok");
      } catch {
        if (!canceled) setBackendStatus("down");
      }

      try {
        const moduleTemplate = await getPrompt("module_blueprint.prompt.md");
        if (!canceled) setModulePromptTemplate(moduleTemplate);
      } catch {
        // Keep running even if blueprint prompt template cannot be fetched.
      }
      try {
        const step1Template = await getPrompt("incremental_step1.prompt.md");
        if (!canceled) setIncrementalStep1Template(step1Template);
      } catch {
        // Keep running even if incremental step1 template cannot be fetched.
      }

      try {
        const arduinoTemplate = await getPrompt("incremental_c_to_arduino.prompt.md");
        if (!canceled) setIncrementalArduinoTemplate(arduinoTemplate);
      } catch {
        // Keep running even if incremental arduino template cannot be fetched.
      }
      try {
        const state = await getSystemState(systemSessionId);
        if (!canceled && state.system?.modules?.length) {
          applyBlueprint({
            modules: state.system.modules.map((item, idx) => ({
              id: item.id || `module_${idx + 1}`,
              title: item.title || item.id || `module_${idx + 1}`,
              subtitle: item.subtitle || "由需求自动生成的模块",
              moduleType: item.moduleType,
              executor: item.executor,
              codeSeed: item.codeSeed || `# ${item.title || item.id || "module"}\n// TODO: implement`,
            })),
            connections: state.system.connections,
          });
          if (!canceled) {
            setStepOutputs((prev) => ({ ...prev, sysml: state.system.system_sysml2, reqTestMap: state.system.req_test_map }));
            const savedContract = state.system.req_test_map?.interface_contract;
            if (savedContract) setContractText(JSON.stringify(savedContract, null, 2));
            setHasCompleteSystem(state.version > 0);
            toast.success(`已加载历史系统 v${state.version}`);
          }
        }
      } catch {
        // Keep default blueprint if state loading fails.
      }

      try {
        const listed = await listArduinoParams({ sketch_path: "outputs/arduino_output.ino" });
        if (!canceled) {
          setModuleParams(groupParamsByModule(listed.params));
        }
      } catch {
        // Ignore when no sketch or no parameter markers are available yet.
      }

      try {
        const loaded = await getArduinoCode("outputs/arduino_output.ino");
        if (!canceled) {
          setStepOutputs((prev) => ({
            ...prev,
            arduinoCode: loaded.code,
          }));
        }
      } catch {
        // Ignore when sketch file is not present yet.
      }
    })();
    return () => {
      canceled = true;
    };
  }, []);

  useEffect(() => {
    if (!serialAutoFillEnabled || !deployPort.trim()) {
      setIsSerialPolling(false);
      return;
    }

    let canceled = false;
    let timer: number | null = null;

    const appendSerialText = (chunk: string) => {
      if (!chunk) return;
      setDiagnosticRawLog((prev) => {
        const merged = `${prev}${prev && !prev.endsWith("\n") ? "\n" : ""}${chunk}`.replace(/\r\n/g, "\n");
        const lines = merged.split("\n");
        return lines.length > 400 ? lines.slice(-400).join("\n") : merged;
      });
    };

    const poll = async () => {
      if (canceled) return;
      setIsSerialPolling(true);
      try {
        const result = await readArduinoSerial({
          port: deployPort.trim(),
          baud_rate: serialBaudRate,
          read_timeout_ms: 900,
          max_chars: 6000,
        });
        if (canceled) return;
        appendSerialText(String(result.text || ""));
      } catch (err) {
        if (!canceled) {
          setSerialAutoFillEnabled(false);
          toast.error(`串口回填失败: ${String((err as Error)?.message || "unknown")}`, { id: "serial-monitor" });
        }
        return;
      } finally {
        if (!canceled) {
          setIsSerialPolling(false);
        }
      }

      if (!canceled) {
        timer = window.setTimeout(() => {
          void poll();
        }, 500);
      }
    };

    void poll();

    return () => {
      canceled = true;
      setIsSerialPolling(false);
      if (timer !== null) window.clearTimeout(timer);
    };
  }, [serialAutoFillEnabled, deployPort, serialBaudRate]);

  useEffect(() => {
    if (!deployModalOpen) return;
    void loadDeployDevices();
  }, [deployModalOpen]);

  function snapshotSystem() {
    return {
      system_sysml2: stepOutputs.sysml,
      req_test_map: stepOutputs.reqTestMap,
      modules: cards.map((card) => ({
        id: card.id,
        title: card.title,
        subtitle: card.subtitle,
        moduleType: card.moduleType,
        executor: card.executor,
        codeSeed: card.code,
      })),
      connections: connections.map((edge) => ({
        from: edge.from,
        to: edge.to,
        relation: edge.relation,
        logic: edge.logic,
      })),
    };
  }

  function stringifyCurrentSystem() {
    return JSON.stringify(snapshotSystem(), null, 2);
  }

  function buildRecentHistory(nextUserMessage?: string): AiHistoryMessage[] {
    const base = chatMessages
      .filter((msg) => msg.content.trim().length > 0)
      .map((msg) => ({ role: msg.role, content: msg.content.trim() as string }));
    const merged = nextUserMessage?.trim()
      ? [...base, { role: "user" as const, content: nextUserMessage.trim() }]
      : base;
    return merged.slice(-12);
  }

  function injectPlaceholder(template: string, placeholder: string, value: string) {
    const normalizedValue = value.trim();
    if (!template.trim()) return normalizedValue;
    if (template.includes(placeholder)) {
      return template.replace(placeholder, normalizedValue);
    }
    return `${template}\n\n${normalizedValue}`;
  }

  useEffect(() => {
    const onMove = (evt: MouseEvent) => {
      if (!dragging) return;
      const dx = evt.clientX - dragging.startMouseX;
      const dy = evt.clientY - dragging.startMouseY;
      setCards((prev) =>
        prev.map((card) =>
          card.id === dragging.id
            ? {
              ...card,
              x: Math.max(16, dragging.startCardX + dx),
              y: Math.max(72, dragging.startCardY + dy),
            }
            : card,
        ),
      );
    };
    const onUp = () => setDragging(null);

    window.addEventListener("mousemove", onMove);
    window.addEventListener("mouseup", onUp);
    return () => {
      window.removeEventListener("mousemove", onMove);
      window.removeEventListener("mouseup", onUp);
    };
  }, [dragging]);

  useEffect(() => {
    if (!dragging) return;
    const { userSelect, webkitUserSelect } = document.body.style;
    document.body.style.userSelect = "none";
    document.body.style.webkitUserSelect = "none";
    return () => {
      document.body.style.userSelect = userSelect;
      document.body.style.webkitUserSelect = webkitUserSelect;
    };
  }, [dragging]);

  const cardMap = useMemo(() => {
    return new Map(cards.map((c) => [c.id, c]));
  }, [cards]);

  const canvasSize = useMemo(() => {
    const minWidth = 1360;
    const minHeight = 760;
    if (!cards.length) return { width: minWidth, height: minHeight };

    const maxRight = cards.reduce((acc, card) => Math.max(acc, card.x + card.w), 0);
    const maxBottom = cards.reduce((acc, card) => Math.max(acc, card.y + card.h), 0);

    // Reserve additional room so branch edges and labels near boundaries are not clipped.
    const width = Math.max(minWidth, Math.ceil(maxRight + 240));
    const height = Math.max(minHeight, Math.ceil(maxBottom + 80));

    return { width, height };
  }, [cards]);

  function updateCard(id: ModuleId, updater: (card: FlowCard) => FlowCard) {
    setCards((prev) => prev.map((card) => (card.id === id ? updater(card) : card)));
  }

  function setCardStatus(id: ModuleId, status: CardStatus, activity: string) {
    updateCard(id, (card) => ({ ...card, status, bottomActivity: activity }));
  }

  function appendLog(id: ModuleId, message: string) {
    updateCard(id, (card) => ({
      ...card,
      logs: [
        `[${nowTag()}] ${message}`,
        ...card.logs.filter((line) => line !== "未执行" && line !== "尚未开始仿真"),
      ].slice(0, 8),
    }));
  }

  function applyBlueprint(blueprint: ModuleBlueprint) {
    const normalized = normalizeBlueprint(blueprint);
    setCards(buildCardsFromBlueprint(normalized));
    setConnections(normalized.connections);
    setStepOutputs({});
  }

  async function requestBlueprintFromBackend(requirement: string, history?: AiHistoryMessage[]): Promise<ModuleBlueprint> {
    const existing = {
      modules: cards.map((card) => ({
        id: card.id,
        title: card.title,
        subtitle: card.subtitle,
        moduleType: card.moduleType,
        executor: card.executor,
        codeSeed: card.code,
      })),
      connections,
    };

    const res = await generateModuleBlueprint({
      requirement,
      existing_blueprint: existing,
      prompt: modulePromptTemplate.trim()
        ? injectPlaceholder(modulePromptTemplate, "<<USER_REQUIREMENT>>", requirement)
        : undefined,
      history,
      temperature: 0.2,
      max_tokens: 5000,
      max_modules: 12,
    });

    return normalizeBlueprint({
      modules: res.blueprint.modules.map((item, idx) => ({
        id: item.id || `module_${idx + 1}`,
        title: item.title || item.id || `module_${idx + 1}`,
        subtitle: item.subtitle || "由大模型规划生成",
        moduleType: item.moduleType,
        executor: item.executor,
        codeSeed: item.codeSeed || `# ${item.title || item.id || "module"}_${idx + 1}\n// TODO: implement`,
      })),
      connections: res.blueprint.connections,
    });
  }

  async function runBlueprintStep(requirement: string, history?: AiHistoryMessage[]): Promise<ModuleBlueprint> {
    toast.loading("Step0: 正在分析模块与逻辑关系...", { id: "step0" });
    try {
      const planned = await requestBlueprintFromBackend(requirement, history);
      applyBlueprint(planned);
      toast.success(`Step0完成: ${planned.modules.length} 个模块`, { id: "step0" });
      return planned;
    } catch (err) {
      const msg = (err as Error)?.message || "未知错误";
      toast.error(`Step0失败: ${msg}`, { id: "step0" });
      throw err;
    }
  }

  async function runModuleStep(module: FrontendModuleSpec, requirement: string): Promise<boolean> {
    try {
      setCardStatus(module.id, "running", `执行模块: ${module.title}`);
      appendLog(module.id, `模块执行器: ${module.executor || module.moduleType || "generic"}`);
      const result = await executeDynamicModule(module, requirement);
      updateCard(module.id, (card) => ({
        ...card,
        code: result.code || card.code,
        subtitle: result.subtitle || card.subtitle,
      }));
      setCardStatus(module.id, "success", result.activity);
      appendLog(module.id, result.log);
      return true;
    } catch (err) {
      const msg = (err as Error).message || "Unknown error";
      setCardStatus(module.id, "error", `执行失败: ${msg}`);
      appendLog(module.id, msg);
      return false;
    }
  }

  async function runFlow(
    requirementOverride?: string,
    options?: { skipStep0?: boolean; priorSystemJson?: string; history?: AiHistoryMessage[] },
  ) {
    const requirement = (requirementOverride ?? promptText).trim();
    if (!requirement) {
      toast.error("请输入需求后再执行");
      return;
    }

    const skipStep0 = Boolean(options?.skipStep0);
    const priorSystemJson = (options?.priorSystemJson || stringifyCurrentSystem()).trim();
    const history = options?.history ?? [];

    let normalized: ModuleBlueprint;
    if (skipStep0) {
      normalized = normalizeBlueprint({
        modules: cards.map((card) => ({
          id: card.id,
          title: card.title,
          subtitle: card.subtitle,
          moduleType: card.moduleType,
          executor: card.executor,
          codeSeed: card.code,
        })),
        connections,
      });
      toast.success("增量模式: 跳过 Step0，直接执行 Step1~Step5", { id: "step0" });
    } else {
      let plannedBlueprint: ModuleBlueprint;
      try {
        plannedBlueprint = await runBlueprintStep(requirement, history);
      } catch {
        toast.error("流程中断: 无法从后端获取模块蓝图", { id: "pipeline" });
        return;
      }
      normalized = normalizeBlueprint(plannedBlueprint);
    }
    const moduleMap = new Map(normalized.modules.map((m) => [m.id, m]));
    const order = buildExecutionOrder(
      normalized.modules.map((m) => m.id),
      normalized.connections as Connection[],
    );

    toast.loading("后台生成中...", { id: "pipeline" });
    let stage = "模块执行";
    try {
      for (const id of order) {
        const module = moduleMap.get(id);
        if (!module) continue;
        const ok = await runModuleStep(module, requirement);
        if (!ok) {
          const title = module.title ?? id;
          toast.error(`流程中断于 ${title}`, { id: "pipeline" });
          return;
        }
      }

      const pinMap = new Map(
        cards.map((card) => {
          const pinValue = buildModulePinDefinition(moduleParams[card.id] || []);
          return [card.id, String(pinValue || "")];
        }),
      );
      const pinEntries = normalized.modules
        .map((mod) => ({ id: mod.id, title: mod.title, value: String(pinMap.get(mod.id) || "").trim() }))
        .filter((item) => item.value.length > 0);
      const pinDefinitions = Object.fromEntries(pinEntries.map((item) => [item.id, item.value]));

      stage = "接口契约：确定输入输出、采样和硬件映射";
      const preparedContract = await prepareInterfaceContract({
        requirement,
        pin_definitions: pinDefinitions,
        interface_contract: useEditedContract ? JSON.parse(contractText) as InterfaceContract : undefined,
        previous_contract: !useEditedContract && skipStep0 && contractText ? JSON.parse(contractText) as InterfaceContract : undefined,
      });
      const interfaceContract = preparedContract.interface_contract;
      const frozenDigest = preparedContract.contract_sha256;
      setContractText(JSON.stringify(interfaceContract, null, 2));
      setContractDigest(frozenDigest);

      const step1Prompt = skipStep0
        ? injectPlaceholder(
          injectPlaceholder(
            incrementalStep1Template.trim() ||
            [
              "你现在做的是增量更新，不是重建系统。",
              "请基于历史系统做微量修改，尽量保持模块命名、主连接与整体结构稳定。",
              "仅修改与本次用户新需求直接相关的部分。",
              "",
              "历史系统 JSON:",
              "<<PRIOR_SYSTEM_JSON>>",
              "",
              "本次修改需求:",
              "<<USER_REQUIREMENT>>",
            ].join("\n"),
            "<<PRIOR_SYSTEM_JSON>>",
            priorSystemJson,
          ),
          "<<USER_REQUIREMENT>>",
          requirement,
        )
        : requirement;

      stage = "Step1: 生成、校验并自动修复系统模型";
      setValidationDiagnostics("");
      setValidationStatus("正在生成并校验 SysML；失败时自动反馈诊断，最多修复 2 次，通过后继续编译。");
      const step1Res = await generateSysml2Bundle({
        interface_contract: interfaceContract,
        prompt: step1Prompt,
        history,
          temperature: 0.2,
        max_tokens: 6000,
        max_repair_attempts: 2,
        out_sysml2_path: "outputs/system.sysml2",
        out_req_test_map_path: "outputs/req_test_map.json",
      });
      if (step1Res.contents?.contract_sha256 !== frozenDigest) throw new Error("SysML 阶段接口契约发生变化");
      const validation = step1Res.contents?.validation as { attempts?: number; repairs?: number; diagnostics?: { attempt: number; ok: boolean; issues: { message: string }[] }[] } | undefined;
      setValidationDiagnostics((validation?.diagnostics ?? []).map(a => `第 ${a.attempt} 次：${a.ok ? "通过" : "未通过"} ${a.issues.map(issue => issue.message).join("；")}`).join("\n"));
      setValidationStatus(`SysML 校验通过：共 ${validation?.attempts ?? 1} 次生成，${validation?.repairs ?? Math.max(0, (validation?.attempts ?? 1) - 1)} 次修复。继续编译已通过的模型。`);
      setStepOutputs((prev) => ({
        ...prev,
        sysml: String(step1Res.contents?.system_sysml2 ?? ""),
        reqTestMap: step1Res.contents?.req_test_map as Record<string, unknown>,
        reqMap: "outputs/req_test_map.json",
      }));

      stage = "Step2: 编译 SysML 为 IR";
      const compiled = await compileSysml({
        interface_contract: interfaceContract,
        system_sysml2: String(step1Res.contents?.system_sysml2 ?? ""),
        out_dir: "outputs/sysml_compiled",
      });
      if (compiled.contents?.contract_sha256 !== frozenDigest) throw new Error("IR 阶段接口契约发生变化");
      const modelReceipt = compiled.contents?.provenance as ProvenanceReceipt | undefined;
      if (!modelReceipt || modelReceipt.claims.stage !== "sysml_ir") throw new Error("模型编译未返回来源凭据");
      const ilFile = String(compiled.files.il);
      setStepOutputs((prev) => ({ ...prev, ilFile }));

      stage = "Step3: IR 生成 C";
      const step4Res = await convertIlToC({
        provenance: modelReceipt,
        interface_contract: interfaceContract,
        il_file_path: ilFile,
        out_c_dir: "outputs/c_output_codegen",
        codegen_root: (import.meta.env.VITE_CODEGENERATOR2_ROOT as string | undefined) ?? "CodeGenerator2/CodeGenerator2",
        optimize: false,
        target: "Intel",
      });
      if (step4Res.provenance_status !== "verified_model_chain" || !step4Res.provenance ||
          step4Res.contract_sha256 !== frozenDigest ||
          step4Res.provenance.claims.ir_sha256 !== modelReceipt.claims.ir_sha256) {
        throw new Error("C 内核与已校验模型的来源不一致");
      }
      setStepOutputs((prev) => ({ ...prev, cFile: String(step4Res.files.c_output_dir) + "/ProgramPage1.c" }));
      const regenerated = [step4Res.contents?.["define.h"], step4Res.contents?.["main.c"], step4Res.contents?.["ProgramPage1.c"]]
        .filter(Boolean)
        .join("\n\n");
      setStepOutputs((prev) => ({ ...prev, regenerated }));

      const moduleSpecText = normalized.modules
        .map((mod, idx) => `${idx + 1}) ${mod.title}(${mod.id}, type=${mod.moduleType || "custom"})`)
        .join("\n");
      const relationText = normalized.connections
        .map((edge, idx) => `${idx + 1}) ${edge.from} -> ${edge.to} [${edge.relation || "data_flow"}] ${edge.logic || ""}`.trim())
        .join("\n");
      const pinDefinitionText = pinEntries.length
        ? pinEntries.map((item, idx) => `${idx + 1}) ${item.title}(${item.id}): ${item.value}`).join("\n")
        : "未提供模块引脚定义，使用模板默认值。";

      const arduinoRequirement = [
        "必须输出 Arduino C/C++ 代码。",
        "必须按下述模块进行函数拆分，每个模块至少对应一个函数。",
        `模块列表:\n${moduleSpecText}`,
        `模块关系:\n${relationText}`,
        "loop() 中按模块关系顺序组织调用。",
        "每个函数只负责单一模块职责，不要混杂其它模块逻辑。",
        "引脚规则: 仅覆盖用户提供的引脚，未提供字段保持默认值。",
        ...(skipStep0
          ? [
            injectPlaceholder(
              incrementalArduinoTemplate.trim() ||
              [
                "当前为增量更新模式: 需要在历史系统基础上做小幅修改，保持整体结构稳定。",
                "",
                "历史系统JSON:",
                "<<PRIOR_SYSTEM_JSON>>",
              ].join("\n"),
              "<<PRIOR_SYSTEM_JSON>>",
              priorSystemJson,
            ),
          ]
          : []),
        `模块引脚定义:\n${pinDefinitionText}`,
        `用户需求: ${requirement}`,
      ].join("\n");

      stage = "Step5: 生成Arduino代码";
      const arduinoRes = await convertCToArduino({
        provenance: step4Res.provenance,
        interface_contract: interfaceContract,
        compiled_kernel: { header: String(step4Res.contents?.["define.h"] ?? ""), source: String(step4Res.contents?.["ProgramPage1.c"] ?? "") },
        c_code: regenerated || "int main(){ return 0; }",
        user_requirements: arduinoRequirement,
        pin_definitions: Object.keys(pinDefinitions).length ? pinDefinitions : null,
        module_specs: normalized.modules.map((mod) => ({
          id: mod.id,
          title: mod.title,
          subtitle: mod.subtitle,
          moduleType: mod.moduleType,
        })),
        history,
          temperature: 0.2,
      });

      if (arduinoRes.contract_sha256 !== frozenDigest) throw new Error("Arduino 阶段接口契约发生变化");
      if (arduinoRes.provenance_status !== "verified_model_chain" || !arduinoRes.provenance ||
          arduinoRes.provenance?.claims.ir_sha256 !== modelReceipt.claims.ir_sha256) {
        throw new Error("Arduino 产物没有保留已校验模型的来源");
      }

      try {
        const markedParams = arduinoRes.params && arduinoRes.params.length
          ? arduinoRes.params
          : (await listArduinoParams({ sketch_path: "outputs/arduino_output.ino" })).params;
        setModuleParams(groupParamsByModule(markedParams));
      } catch {
        setModuleParams({});
      }

      setStepOutputs((prev) => ({
        ...prev,
        arduinoCode: String(arduinoRes.arduino_code || ""),
      }));

      stage = "Arduino 编译验证";
      const compileResult = await compileArduinoCode({
        provenance: arduinoRes.provenance,
        sketch_path: arduinoRes.saved_path || "outputs/arduino_output.ino",
        fqbn: interfaceContract.board || "arduino:avr:uno",
      });
      if (!compileResult.success) {
        const diagnostics = [compileResult.compile_stdout, compileResult.compile_stderr].filter(Boolean).join("\n");
        setValidationDiagnostics(diagnostics);
        setValidationStatus("Arduino 编译未通过，生成文件与诊断已保留。");
        throw new Error(diagnostics || "Arduino 编译失败");
      }
      if (compileResult.provenance_status !== "verified_model_chain" ||
          compileResult.arduino_sha256 !== arduinoRes.provenance.claims.arduino_sha256) {
        throw new Error("编译的 Arduino 文件与已生成产物不一致");
      }
      setValidationStatus("SysML、接口契约和 Arduino 编译已通过；功能行为仍需独立测试验证。");

      const moduleParts = arduinoRes.module_code_map && Object.keys(arduinoRes.module_code_map).length
        ? arduinoRes.module_code_map
        : splitArduinoCodeByModules(String(arduinoRes.arduino_code || ""), normalized.modules);
      setCards((prev) =>
        prev.map((card) => {
          if (!moduleMap.has(card.id)) return card;
          const nextCode = moduleParts[card.id] || String(arduinoRes.arduino_code || "");
          const nextSubtitle = card.subtitle || summarizeCodeRole(nextCode, "暂无功能说明");
          return {
            ...card,
            code: nextCode,
            subtitle: nextSubtitle,
          };
        }),
      );

      const finalSystem = {
        system_sysml2: String(step1Res.contents?.system_sysml2 ?? ""),
        req_test_map: step1Res.contents?.req_test_map as Record<string, unknown>,
        modules: normalized.modules.map((mod) => {
          const found = moduleParts[mod.id];
          return {
            id: mod.id,
            title: mod.title,
            subtitle: mod.subtitle,
            moduleType: mod.moduleType,
            executor: mod.executor,
            codeSeed: found || mod.codeSeed,
          };
        }),
        connections: normalized.connections,
      };
      await saveSystemState({ session_id: systemSessionId, system: finalSystem });
      setHasCompleteSystem(true);

      toast.success("模块逻辑已生成完成", { id: "pipeline" });
    } catch (err) {
      const msg = (err as Error)?.message || "未知错误";
      if (stage.startsWith("Step1")) setValidationStatus(`SysML 生成或校验未通过，后续步骤已停止：${msg}`);
      toast.error(`${stage} 失败: ${msg}`, { id: "pipeline" });
    }
  }

  function saveSystem() {
    let savedContract: InterfaceContract | null = null;
    try {
      savedContract = contractText ? JSON.parse(contractText) as InterfaceContract : null;
    } catch {
      toast.error("接口约定不是有效 JSON，请修正后保存。");
      return;
    }
    const payload = {
      cards: cards.map(({ id, x, y }) => ({ id, x, y })),
      promptText,
      interface_contract: savedContract,
      modules: cards.map(({ id, title, subtitle, moduleType, executor, code, pinDefinition }) => ({
        id,
        title,
        subtitle,
        moduleType,
        executor,
        code,
        pinDefinition,
      })),
      connections,
      savedAt: new Date().toISOString(),
    };
    const blob = new Blob([JSON.stringify(payload, null, 2)], { type: "application/json" });
    const url = URL.createObjectURL(blob);
    const a = document.createElement("a");
    a.href = url;
    a.download = "llm4mdd-system.json";
    a.click();
    URL.revokeObjectURL(url);
    toast.success("系统已保存");
  }

  function openSystem() {
    fileInputRef.current?.click();
  }

  async function clearCanvas() {
    if (!window.confirm("确定要清空当前画布吗？这会移除当前板块和连线。")) {
      return;
    }

    setCards([]);
    setConnections([]);
    setCodeModal(null);
    setModuleParams({});
    setStepOutputs({});
    setHasCompleteSystem(false);

    try {
      await saveSystemState({
        session_id: systemSessionId,
        system: {
          modules: [],
          connections: [],
        },
      });
      toast.success("画布已清空");
    } catch (err) {
      toast.error(`清空后保存失败: ${String((err as Error)?.message || "unknown")}`);
    }
  }

  function openModuleCodeModal(card: Pick<FlowCard, "id" | "title" | "code" | "subtitle">) {
    const text = String(card.code || "").trim();
    const lower = text.toLowerCase();
    const isTemplatePlaceholder =
      !text ||
      lower.includes("todo: implement") ||
      lower.includes("// todo") ||
      lower === "未执行";

    if (!isTemplatePlaceholder) {
      setCodeModal({
        title: card.title,
        code: card.code,
        summary: `${card.subtitle}`,
        moduleId: card.id,
      });
      return;
    }

    void (async () => {
      const arduinoCode = await ensureArduinoCodeLoaded({ silent: true });
      if (arduinoCode?.trim()) {
        const moduleParts = splitArduinoCodeByModules(
          arduinoCode,
          cards.map((item) => ({
            id: item.id,
            title: item.title,
            subtitle: item.subtitle,
            moduleType: item.moduleType,
          })),
        );
        const resolved = String(moduleParts[card.id] || "").trim();
        if (resolved) {
          updateCard(card.id, (item) => ({
            ...item,
            code: resolved,
            subtitle: item.subtitle || summarizeCodeRole(resolved, "暂无功能说明"),
          }));
          setCodeModal({
            title: card.title,
            code: resolved,
            summary: "从已有 Arduino 产物自动提取的模块代码",
            moduleId: card.id,
          });
          return;
        }
      }

      const runtimeModule = cards.find((item) => item.id === card.id);
      if (runtimeModule) {
        const requirement = promptText.trim();
        toast.loading(`正在按模块生成代码: ${runtimeModule.title}`, { id: `module-code-${runtimeModule.id}` });
        try {
          setCardStatus(runtimeModule.id, "running", `按需生成代码: ${runtimeModule.title}`);
          appendLog(runtimeModule.id, "双击触发按需代码生成");
          const result = await executeDynamicModule(
            {
              id: runtimeModule.id,
              title: runtimeModule.title,
              subtitle: runtimeModule.subtitle,
              moduleType: runtimeModule.moduleType,
              executor: runtimeModule.executor,
              codeSeed: runtimeModule.code,
            },
            requirement || "请按模块职责生成可运行代码",
          );

          const generated = String(result.code || "").trim();
          if (generated) {
            updateCard(runtimeModule.id, (item) => ({
              ...item,
              code: generated,
              subtitle: result.subtitle || summarizeCodeRole(generated, item.subtitle),
              status: "success",
              bottomActivity: result.activity || "模块按需代码生成完成",
            }));
            appendLog(runtimeModule.id, result.log || "按需生成完成");
            toast.success(`模块代码已生成: ${runtimeModule.title}`, { id: `module-code-${runtimeModule.id}` });
            setCodeModal({
              title: runtimeModule.title,
              code: generated,
              summary: "按模块即时生成的代码（无需完整流程）",
              moduleId: runtimeModule.id,
            });
            return;
          }
        } catch (err) {
          const msg = String((err as Error)?.message || "unknown");
          setCardStatus(runtimeModule.id, "error", `按需生成失败: ${msg}`);
          appendLog(runtimeModule.id, `按需生成失败: ${msg}`);
          toast.error(`模块按需生成失败: ${msg}`, { id: `module-code-${runtimeModule.id}` });
        }
      }

      setCodeModal({
        title: card.title,
        code: text || "// 当前模块暂无可展示的具体代码。\n// 可先执行一次代码生成，或确保 outputs/arduino_output.ino 已存在。",
        summary: `${card.subtitle}`,
        moduleId: card.id,
      });
    })();
  }

  function handleParamValueChange(moduleId: string, paramId: string, value: string) {
    setModuleParams((prev) => ({
      ...prev,
      [moduleId]: (prev[moduleId] || []).map((item) =>
        item.param_id === paramId ? { ...item, value } : item,
      ),
    }));
  }

  async function applyModuleParams(moduleId: string) {
    const moduleItems = moduleParams[moduleId] || [];
    if (!moduleItems.length) return;

    setSavingParamsModuleId(moduleId);
    toast.loading("正在应用模块参数...", { id: `save-params-${moduleId}` });
    try {
      const result = await updateArduinoParams({
        sketch_path: "outputs/arduino_output.ino",
        updates: moduleItems.map((item) => ({
          param_id: item.param_id,
          value: item.value,
        })),
      });

      setStepOutputs((prev) => ({
        ...prev,
        arduinoCode: result.arduino_code,
      }));
      setModuleParams(groupParamsByModule(result.params));

      toast.success("模块参数已映射到 Arduino 输出文件", { id: `save-params-${moduleId}` });
    } catch (err) {
      toast.error(`参数应用失败: ${String((err as Error)?.message || "unknown")}`, { id: `save-params-${moduleId}` });
    } finally {
      setSavingParamsModuleId(null);
    }
  }

  async function ensureArduinoCodeLoaded(opts?: { silent?: boolean }): Promise<string | null> {
    const existing = stepOutputs.arduinoCode?.trim();
    if (existing) return stepOutputs.arduinoCode || "";

    try {
      const loaded = await getArduinoCode("outputs/arduino_output.ino");
      setStepOutputs((prev) => ({
        ...prev,
        arduinoCode: loaded.code,
      }));
      return loaded.code;
    } catch (err) {
      if (!opts?.silent) {
        toast.error(`未找到可用 Arduino 代码文件: ${String((err as Error)?.message || "unknown")}`);
      }
      return null;
    }
  }

  async function handleOpenArduinoCode() {
    const code = await ensureArduinoCodeLoaded();
    if (!code?.trim()) return;
    setCodeModal({
      title: "Arduino Output",
      code,
      summary: "完整的 Arduino 生成代码",
    });
  }

  async function handleCompileArduino() {
    const code = await ensureArduinoCodeLoaded({ silent: true });
    if (!code?.trim()) {
      toast.error("未找到 Arduino 代码文件，请先生成或放置 outputs/arduino_output.ino");
      return;
    }

    toast.loading("正在编译 Arduino 代码...", { id: "compile-arduino" });
    try {
      const result = await compileArduinoCode({
        sketch_path: "outputs/arduino_output.ino",
      });
      const compileLog = [result.compile_stdout, result.compile_stderr].filter(Boolean).join("\n\n").trim();
      if (result.success) {
        toast.success("Arduino 代码编译成功", { id: "compile-arduino" });
        if (compileLog) {
          setCodeModal({
            title: "Arduino Compile Log",
            code: compileLog,
            summary: `板型: ${result.fqbn}`,
          });
        }
        return;
      }

      toast.error("Arduino 代码编译失败", { id: "compile-arduino" });
      setCodeModal({
        title: "Arduino Compile Log",
        code: compileLog || "编译失败，但没有返回详细日志。",
        summary: `板型: ${result.fqbn}`,
      });
    } catch (err) {
      toast.error(`编译失败: ${String((err as Error)?.message || "unknown")}`, { id: "compile-arduino" });
    }
  }

  async function handleDeployArduino() {
    const code = await ensureArduinoCodeLoaded({ silent: true });
    if (!code?.trim()) {
      toast.error("未找到 Arduino 代码文件，请先生成或放置 outputs/arduino_output.ino");
      return;
    }
    if (!deployPort.trim()) {
      toast.error("请先输入串口号，例如 COM4");
      return;
    }

    setIsDeploying(true);
    toast.loading("正在部署到开发板...", { id: "deploy-arduino" });
    try {
      const result = await uploadArduinoCode({
        sketch_path: "outputs/arduino_output.ino",
        fqbn: deployFqbn,
        port: deployPort.trim(),
      });

      const logText = [
        `[summary]\n${result.message}`,
        result.compile_command ? `[compile command]\n${result.compile_command}` : "",
        result.compile_stdout ? `[compile stdout]\n${result.compile_stdout}` : "",
        result.compile_stderr ? `[compile stderr]\n${result.compile_stderr}` : "",
        result.upload_command ? `[upload command]\n${result.upload_command}` : "",
        result.upload_stdout ? `[upload stdout]\n${result.upload_stdout}` : "",
        result.upload_stderr ? `[upload stderr]\n${result.upload_stderr}` : "",
      ]
        .filter(Boolean)
        .join("\n\n");

      if (result.success) {
        toast.success(result.message || `部署成功：${result.port}`, { id: "deploy-arduino" });
        setDeployModalOpen(false);
        setSerialAutoFillEnabled(true);
        toast.success(`已开始回填串口日志到诊断框 (${result.port})`, { id: "serial-monitor" });
        if (logText) {
          setCodeModal({
            title: "Arduino Deploy Log",
            code: logText,
            summary: `${result.fqbn} -> ${result.port}`,
          });
        }
        return;
      }

      toast.error(result.message || "部署失败，请查看日志", { id: "deploy-arduino" });
      setCodeModal({
        title: "Arduino Deploy Log",
        code: logText || "部署失败，但没有返回详细日志。",
        summary: `${result.fqbn} -> ${result.port}`,
      });
    } catch (err) {
      toast.error(`部署失败: ${String((err as Error)?.message || "unknown")}`, { id: "deploy-arduino" });
    } finally {
      setIsDeploying(false);
    }
  }

  async function loadDeployDevices() {
    setIsLoadingDeployDevices(true);
    try {
      const result = await getArduinoDevices();
      setDeployPorts(result.ports.map((item) => ({ address: item.address, label: item.label, fqbn: item.fqbn })));
      setDeployBoards(result.boards);
      if (result.ports.length > 0) {
        setDeployPort((prev) => prev || result.ports[0].address);
        const matchedPort = result.ports.find((item) => item.address === deployPort) ?? result.ports[0];
        if (matchedPort?.fqbn) {
          setDeployFqbn(matchedPort.fqbn);
        }
      }
      if (result.boards.length > 0 && !result.boards.some((item) => item.fqbn === deployFqbn)) {
        setDeployFqbn(result.boards[0].fqbn);
      }
    } catch (err) {
      toast.error(`设备检测失败: ${String((err as Error)?.message || "unknown")}`, { id: "deploy-devices" });
    } finally {
      setIsLoadingDeployDevices(false);
    }
  }

  async function handleParseDiagnostics() {
    const raw = diagnosticRawLog.trim();
    if (!raw) {
      toast.error("请先粘贴串口日志");
      return;
    }

    setIsParsingDiagnostics(true);
    toast.loading("正在解析诊断日志...", { id: "diag-parse" });
    try {
      const result = await parseDiagnosticsLog({
        raw_log: raw,
        source: "serial",
        max_events: 200,
      });
      setDiagnosticEvents(result.events || []);
      setDiagnosticSummary(result.summary || []);
      setDiagnosticFix(null);
      toast.success(`解析完成: ${result.parsed_events} 条事件`, { id: "diag-parse" });
    } catch (err) {
      toast.error(`日志解析失败: ${String((err as Error)?.message || "unknown")}`, { id: "diag-parse" });
    } finally {
      setIsParsingDiagnostics(false);
    }
  }

  async function handleSuggestFixFromDiagnostics() {
    if (!diagnosticEvents.length) {
      toast.error("请先解析日志，再生成修复建议");
      return;
    }

    setIsSuggestingFix(true);
    toast.loading("正在生成修复建议...", { id: "diag-fix" });
    try {
      const result = await suggestDiagnosticFix({
        events: diagnosticEvents,
        requirement: promptText,
        current_arduino_code: stepOutputs.arduinoCode || null,
        });
      setDiagnosticFix(result);
      toast.success("修复建议已生成", { id: "diag-fix" });
    } catch (err) {
      toast.error(`建议生成失败: ${String((err as Error)?.message || "unknown")}`, { id: "diag-fix" });
    } finally {
      setIsSuggestingFix(false);
    }
  }

  function onOpenFile(content: string) {
    try {
      const parsed = JSON.parse(content) as {
        cards?: Array<{ id: ModuleId; x: number; y: number }>;
        modules?: Array<{
          id: ModuleId;
          title: string;
          subtitle: string;
          moduleType?: string;
          executor?: string;
          code: string;
          pinDefinition?: string;
        }>;
        connections?: Connection[];
        promptText?: string;
        interface_contract?: InterfaceContract;
      };
      if (parsed.modules?.length) {
        applyBlueprint({
          modules: parsed.modules.map((item) => ({
            id: item.id,
            title: item.title,
            subtitle: item.subtitle,
            moduleType: item.moduleType,
            executor: item.executor,
            codeSeed: item.code,
          })),
          connections: parsed.connections ?? [],
        });
        setCards((prev) =>
          prev.map((card) => {
            const found = parsed.modules?.find((item) => item.id === card.id);
            return found ? { ...card, pinDefinition: found.pinDefinition?.trim() ?? card.pinDefinition } : card;
          }),
        );
      }
      if (parsed.cards?.length) {
        setCards((prev) =>
          prev.map((card) => {
            const found = parsed.cards?.find((x) => x.id === card.id);
            return found ? { ...card, x: found.x, y: found.y } : card;
          }),
        );
      }
      if (parsed.connections?.length) {
        setConnections(parsed.connections);
      }
      if (parsed.interface_contract) setContractText(JSON.stringify(parsed.interface_contract, null, 2));
      if (parsed.promptText) {
        setPromptText(parsed.promptText);
      }
      toast.success("系统已打开");
    } catch {
      toast.error("系统文件解析失败");
    }
  }

  async function sendAiMessage() {
    if (!chatInput.trim()) return;
    const message = chatInput.trim();
    setChatMessages((prev) => [...prev, { id: crypto.randomUUID(), role: "user", content: message }]);
    setChatInput("");

    if (message.startsWith("/modules")) {
      const raw = message.replace("/modules", "").trim();
      const history = buildRecentHistory(raw || message);
      toast.loading("正在生成模块蓝图...", { id: "modules" });
      try {
        const parsed = tryParseBlueprint(raw);
        const planned = parsed ?? (await requestBlueprintFromBackend(raw || promptText, history));
        applyBlueprint(planned);
        toast.success("模块蓝图已更新", { id: "modules" });
        setChatMessages((prev) => [
          ...prev,
          {
            id: crypto.randomUUID(),
            role: "assistant",
            content: `已更新模块蓝图，共 ${planned.modules.length} 个模块。可继续点击“后台生成”执行。`,
          },
        ]);
      } catch (err) {
        toast.error("后端蓝图生成失败", { id: "modules" });
        setChatMessages((prev) => [
          ...prev,
          {
            id: crypto.randomUUID(),
            role: "assistant",
            content: `后端蓝图生成失败: ${String((err as Error)?.message || "unknown")}`,
          },
        ]);
      }
      return;
    }

    if (message.includes("自动生成模块") || message.includes("规划模块")) {
      const history = buildRecentHistory(message);
      toast.loading("正在调用大模型规划模块...", { id: "modules" });
      try {
        const planned = await requestBlueprintFromBackend(message, history);
        applyBlueprint(planned);
        toast.success("模块规划完成", { id: "modules" });
        setChatMessages((prev) => [
          ...prev,
          {
            id: crypto.randomUUID(),
            role: "assistant",
            content: `已根据需求自动规划 ${planned.modules.length} 个模块，并替换到画布。`,
          },
        ]);
      } catch (err) {
        toast.error("后端规划失败", { id: "modules" });
        setChatMessages((prev) => [
          ...prev,
          {
            id: crypto.randomUUID(),
            role: "assistant",
            content: `后端规划失败: ${String((err as Error)?.message || "unknown")}`,
          },
        ]);
      }
      return;
    }

    toast.loading("正在由大模型判断执行模式...", { id: "mode" });
    try {
      const historyForAi = buildRecentHistory(message);
      const modeRes = await decideInteractionMode({
        instruction: message,
        has_complete_system: hasCompleteSystem,
        current_system: snapshotSystem(),
        history: historyForAi,
          temperature: 0.1,
        max_tokens: 300,
      });

      toast.success(`模式判定: ${modeRes.mode}`, { id: "mode" });
      setChatMessages((prev) => [
        ...prev,
        {
          id: crypto.randomUUID(),
          role: "assistant",
          content: `模式判定: ${modeRes.mode}。${modeRes.reason}`,
        },
      ]);

      if (modeRes.mode === "regenerate") {
        setPromptText(message);
        void runFlow(message, { skipStep0: false, history: historyForAi }).then(() => {
          setChatMessages((prev) => [
            ...prev,
            {
              id: crypto.randomUUID(),
              role: "assistant",
              content: "已按重生成模式执行 Step0~Step5。",
            },
          ]);
        });
        return;
      }

      const priorSystemJson = stringifyCurrentSystem();
      void runFlow(message, { skipStep0: true, priorSystemJson, history: historyForAi }).then(() => {
        setChatMessages((prev) => [
          ...prev,
          {
            id: crypto.randomUUID(),
            role: "assistant",
            content: "已按增量模式执行 Step1~Step5（跳过 Step0）。",
          },
        ]);
      });
    } catch (err) {
      toast.error("模式判定失败", { id: "mode" });
      setChatMessages((prev) => [
        ...prev,
        {
          id: crypto.randomUUID(),
          role: "assistant",
          content: `模式判定失败: ${String((err as Error)?.message || "unknown")}`,
        },
      ]);
    }
  }

  return (
    <div className="min-h-screen overflow-y-auto overflow-x-hidden bg-[linear-gradient(140deg,#edf4ff_0%,#fff8ea_50%,#eefcf7_100%)] text-slate-800">
      <Toaster position="top-right" />
      <dialog ref={moduleDetailsRef} className="fixed inset-0 m-auto max-h-[85vh] w-[min(92vw,640px)] overflow-y-auto rounded-2xl border border-slate-200 bg-white p-6 shadow-xl backdrop:bg-slate-900/40" aria-label="模块功能说明">
        <div className="flex items-center justify-between gap-4">
          <h2 className="text-lg font-semibold">{detailModule?.title} · 功能说明</h2>
          <button type="button" onClick={() => moduleDetailsRef.current?.close()} aria-label="关闭功能说明"><X className="h-5 w-5" /></button>
        </div>
        <p className="mt-2 text-xs text-slate-500">以下是需求规划说明；生成代码的实际行为仍需测试验证。</p>
        <h3 className="mt-5 font-medium">职责与处理规则</h3>
        <p className="mt-2 whitespace-pre-wrap text-sm leading-7">{detailModule?.subtitle || "暂无详细说明，请重新规划模块。"}</p>
        <h3 className="mt-5 font-medium">输入与上游依赖</h3>
        <ul className="mt-2 space-y-2 text-sm">{connections.filter(e => e.to === detailModule?.id).map(e => <li key={e.from}>{cards.find(c => c.id === e.from)?.title || e.from}：{e.logic || "未说明具体数据，请补充需求。"}</li>)}</ul>
        {!connections.some(e => e.to === detailModule?.id) && <p className="mt-2 text-sm text-slate-600">无上游模块；外部输入以职责说明和硬件配置为准。</p>}
        <h3 className="mt-5 font-medium">输出与下游用途</h3>
        <ul className="mt-2 space-y-2 text-sm">{connections.filter(e => e.from === detailModule?.id).map(e => <li key={e.to}>{cards.find(c => c.id === e.to)?.title || e.to}：{e.logic || "未说明具体用途，请补充需求。"}</li>)}</ul>
        {!connections.some(e => e.from === detailModule?.id) && <p className="mt-2 text-sm text-slate-600">无下游模块；外设动作或系统输出以职责说明为准。</p>}
      </dialog>
      {validationStatus && <div role="status" className="max-h-32 overflow-auto border-b border-sky-200 bg-sky-50 px-5 py-3 text-sm text-sky-900">{validationStatus}{validationDiagnostics && <details className="mt-2"><summary className="cursor-pointer">查看逐轮校验与修复记录</summary><pre className="mt-2 whitespace-pre-wrap text-xs">{validationDiagnostics}</pre></details>}</div>}
      <details className="border-b border-slate-200 bg-white px-5 py-2 text-sm">
        <summary className="cursor-pointer">接口与硬件约定（生成模型前固定）</summary>
        <p className="my-2 text-xs text-slate-600">流程会先确定设备、输入输出、单位和采样周期，再共用于模型与硬件适配。自动规划的约定仍需独立功能测试验证。</p>
        <p className="my-2 text-xs text-slate-600">数字接口会列出模型 true／false 对应的引脚电平。低电平有效按钮按下时，模型输入为 true；初始值也使用模型逻辑值。</p>
        <label className="flex items-center gap-2 text-xs">
          <input type="checkbox" checked={useEditedContract} onChange={e => setUseEditedContract(e.target.checked)} />
          使用下方编辑后的约定；未勾选时根据本次需求重新规划
        </label>
        <textarea aria-label="接口契约" value={contractText} onChange={e => setContractText(e.target.value)}
          className="mt-2 h-48 w-full rounded border p-2 font-mono text-xs" placeholder="运行后显示接口约定，也可以在此粘贴已核实的约定。" />
        {contractDigest && <p className="break-all text-xs text-slate-500">本次约定指纹：{contractDigest}</p>}
      </details>


      <input
        ref={fileInputRef}
        type="file"
        accept="application/json"
        className="hidden"
        onChange={(evt) => {
          const file = evt.target.files?.[0];
          if (!file) return;
          file.text().then(onOpenFile).catch(() => toast.error("读取文件失败"));
        }}
      />

      <header className="flex h-14 items-center justify-between border-b border-slate-200 bg-white/85 px-3 backdrop-blur sm:px-5">
        <div className="flex items-center gap-3">
          <div className="inline-flex items-center gap-2 rounded-xl border border-cyan-200 bg-cyan-50 px-2.5 py-1 text-xs font-medium text-cyan-700">
            <Cable className="h-3.5 w-3.5" />
            Embedded Logic Flow
          </div>
          <div className="hidden items-center gap-2 text-xs text-slate-600 sm:flex">
            <span className="inline-flex items-center gap-1 rounded-lg border border-slate-300 bg-white px-2 py-1">
              <Server className="h-3.5 w-3.5" />
              Backend: {backendStatus === "ok" ? "Online" : backendStatus === "down" ? "Down" : "Checking"}
            </span>
          </div>
        </div>

        <div className="flex items-center gap-2">
          <button
            type="button"
            className={cn(
              "inline-flex items-center gap-1.5 rounded-xl border px-3 py-1.5 text-xs hover:bg-slate-50",
              englishMode ? "border-cyan-500 bg-cyan-50 text-cyan-800" : "border-slate-300 bg-white",
            )}
            onClick={() => {
              setEnglishMode((prev) => !prev);
              toast.success(englishMode ? "已切换到中文" : "English mode enabled");
            }}
          >
            {englishMode ? "中文" : "English"}
          </button>
          <button
            type="button"
            className="inline-flex items-center gap-1.5 rounded-xl border border-slate-300 bg-white px-3 py-1.5 text-xs hover:bg-slate-50"
            onClick={() => setIsAiPanelOpen((prev) => !prev)}
          >
            <Sidebar className="h-3.5 w-3.5" />
            {isAiPanelOpen ? "收起AI" : "展开AI"}
          </button>
          <button
            type="button"
            className="inline-flex items-center gap-1.5 rounded-xl border border-slate-300 bg-white px-3 py-1.5 text-xs hover:bg-slate-50"
            onClick={openSystem}
          >
            <FolderOpen className="h-3.5 w-3.5" />
            打开系统
          </button>
          <button
            type="button"
            className="inline-flex items-center gap-1.5 rounded-xl border border-slate-300 bg-white px-3 py-1.5 text-xs hover:bg-slate-50"
            onClick={saveSystem}
          >
            <Save className="h-3.5 w-3.5" />
            保存系统
          </button>
          <button
            type="button"
            className="inline-flex items-center gap-1.5 rounded-xl border border-slate-300 bg-white px-3 py-1.5 text-xs hover:bg-slate-50"
            onClick={() => {
              void clearCanvas();
            }}
          >
            <Trash2 className="h-3.5 w-3.5" />
            清空画布
          </button>
          <button
            type="button"
            className="inline-flex items-center gap-1.5 rounded-xl border border-slate-300 bg-white px-3 py-1.5 text-xs hover:bg-slate-50"
            style={{ display: "none" }}
            onClick={() => {
              void runFlow();
            }}
          >
            <Sparkles className="h-3.5 w-3.5" />
            后台生成
          </button>
          <button
            type="button"
            className="inline-flex items-center gap-1.5 rounded-xl border border-slate-300 bg-white px-3 py-1.5 text-xs hover:bg-slate-50 disabled:cursor-not-allowed disabled:opacity-50"
            onClick={() => {
              void handleOpenArduinoCode();
            }}
          >
            <FileCode className="h-3.5 w-3.5" />
            查看代码
          </button>
          <button
            type="button"
            className="inline-flex items-center gap-1.5 rounded-xl border border-slate-300 bg-white px-3 py-1.5 text-xs hover:bg-slate-50 disabled:cursor-not-allowed disabled:opacity-50"
            onClick={() => {
              void handleCompileArduino();
            }}
          >
            <Cpu className="h-3.5 w-3.5" />
            编译代码
          </button>
          <button
            type="button"
            className="inline-flex items-center gap-1.5 rounded-xl border border-slate-300 bg-white px-3 py-1.5 text-xs hover:bg-slate-50 disabled:cursor-not-allowed disabled:opacity-50"
            onClick={() => setDeployModalOpen(true)}
          >
            <Cpu className="h-3.5 w-3.5" />
            部署到板子
          </button>
        </div>
      </header>

      <div className="grid min-h-[calc(100vh-56px)] grid-cols-12">
        <section
          className={cn(
            "col-span-12 bg-white/35",
            isAiPanelOpen ? "border-r border-slate-200 md:col-span-8 xl:col-span-9" : "md:col-span-12 xl:col-span-12",
          )}
        >
          <div className="relative h-full overflow-auto">
            <div className="relative" style={{ width: canvasSize.width, minWidth: canvasSize.width, height: canvasSize.height, minHeight: canvasSize.height }}>
              <svg className="pointer-events-none absolute inset-0 h-full w-full">
                {connections.map((edge) => {
                  const from = cardMap.get(edge.from);
                  const to = cardMap.get(edge.to);
                  if (!from || !to) return null;
                  const x1 = from.x + from.w;
                  const y1 = from.y + from.h / 2;
                  const x2 = to.x;
                  const y2 = to.y + to.h / 2;
                  const relation = (edge.relation || "data_flow").toLowerCase();
                  const edgeColor = "#0ea5e9";
                  const curve = Math.max(72, Math.abs(x2 - x1) * 0.42);
                  const c1x = x1 + curve;
                  const c2x = x2 - curve;
                  const path = `M ${x1} ${y1} C ${c1x} ${y1}, ${c2x} ${y2}, ${x2} ${y2}`;
                  const label = englishMode
                    ? translateConnectionLabel(edge, cardMap)
                    : formatConnectionLabel(edge);
                  const wideGlyphCount = (label.match(/[\u2E80-\u9FFF]/g) || []).length;
                  const narrowGlyphCount = Math.max(0, label.length - wideGlyphCount);
                  const cx = (x1 + x2) / 2;
                  const cy = (y1 + y2) / 2;
                  const labelWidth = Math.max(52, wideGlyphCount * 11 + narrowGlyphCount * 7 + 20);
                  const markerId = `arrow-${edge.from}-${edge.to}`;
                  return (
                    <g key={`${edge.from}-${edge.to}`}>
                      <defs>
                        <marker id={markerId} markerWidth="9" markerHeight="9" refX="8" refY="4.5" orient="auto" markerUnits="userSpaceOnUse">
                          <path d="M0,0 L9,4.5 L0,9 z" fill={edgeColor} />
                        </marker>
                      </defs>
                      <path d={path} stroke="#cbd5e1" strokeOpacity="0.6" strokeWidth="6" fill="none" />
                      <path
                        d={path}
                        stroke={edgeColor}
                        strokeWidth="2.6"
                        fill="none"
                        strokeLinecap="round"
                        markerEnd={`url(#${markerId})`}
                      />
                      <rect
                        x={cx - labelWidth / 2}
                        y={cy - 18}
                        width={labelWidth}
                        height={20}
                        rx={10}
                        fill="white"
                        fillOpacity="0.92"
                        stroke={edgeColor}
                        strokeOpacity="0.28"
                      />
                      <text
                        x={cx}
                        y={cy - 8}
                        textAnchor="middle"
                        dominantBaseline="middle"
                        fontSize="10"
                        fill="#0f172a"
                        className="select-none"
                        fontWeight="600"
                      >
                        {label}
                      </text>
                    </g>
                  );
                })}
              </svg>

              {cards.map((card) => (
                <article
                  key={card.id}
                  className="absolute rounded-2xl border border-slate-300 bg-white shadow-[0_10px_24px_rgba(15,23,42,0.12)]"
                  style={{ left: card.x, top: card.y, width: card.w, minHeight: card.h }}
                  onDoubleClick={() => {
                    openModuleCodeModal(card);
                  }}
                >
                  <header
                    className="flex h-9 cursor-move items-center justify-between rounded-t-2xl border-b border-slate-200 bg-slate-50 px-3"
                    onMouseDown={(evt: ReactMouseEvent<HTMLDivElement>) => {
                      evt.preventDefault();
                      setDragging({
                        id: card.id,
                        startMouseX: evt.clientX,
                        startMouseY: evt.clientY,
                        startCardX: card.x,
                        startCardY: card.y,
                      });
                    }}
                  >
                    <div className="inline-flex items-center gap-2 text-sm font-medium text-slate-800">
                      <Cpu className="h-4 w-4 text-slate-600" />
                      {displayModuleTitle(card)}
                    </div>
                    <span className={cn("rounded-md border px-2 py-0.5 text-[11px]", statusClass(card.status))}>{card.status}</span>
                  </header>

                  <div className="space-y-3 p-3">
                    <p className="line-clamp-3 whitespace-pre-line text-xs leading-5 text-slate-600">{displayModuleSubtitle(card)}</p>
                    <button type="button" className="text-xs font-medium text-sky-700 underline" onDoubleClick={(event) => event.stopPropagation()} onClick={() => { setDetailModule(card); moduleDetailsRef.current?.showModal(); }}>
                      {englishMode ? "Function details" : "查看完整功能说明"}
                    </button>

                    {(moduleParams[card.id] || []).length > 0 ? (
                      <div className="space-y-2 rounded-lg border border-slate-200 bg-slate-50 p-2">
                        <div className="text-[11px] font-medium text-slate-700">{t("模块参数")}</div>
                        {(moduleParams[card.id] || []).map((param) => (
                          <div key={param.param_id} className="grid grid-cols-[1fr_1fr] items-center gap-2">
                            <div className="truncate text-[11px] text-slate-600" title={param.param_id}>
                              {(() => {
                                const rawLabel = String(param.zh_name || param.name || "参数");
                                const fallbackLabel = String(param.name || param.param_id || "Parameter");
                                const label = englishMode && hasNonEnglishText(rawLabel) ? fallbackLabel : rawLabel;
                                return t(label);
                              })()} ({param.type})
                            </div>
                            <input
                              value={param.value}
                              onChange={(evt) => handleParamValueChange(card.id, param.param_id, evt.target.value)}
                              className="w-full rounded-md border border-slate-300 bg-white px-2 py-1 text-[11px] text-slate-800 outline-none focus:border-cyan-500"
                            />
                          </div>
                        ))}
                        <div className="flex justify-end">
                          <button
                            type="button"
                            className="rounded-md border border-emerald-600 bg-emerald-600 px-2.5 py-1 text-[11px] text-white hover:bg-emerald-700 disabled:cursor-not-allowed disabled:opacity-60"
                            onClick={() => {
                              void applyModuleParams(card.id);
                            }}
                            disabled={savingParamsModuleId === card.id}
                          >
                            {savingParamsModuleId === card.id ? t("应用中...") : t("应用参数")}
                          </button>
                        </div>
                      </div>
                    ) : null}

                    <div className="flex items-center justify-end">
                      <button
                        type="button"
                        className="inline-flex items-center gap-1 rounded-lg border border-slate-300 bg-white px-2.5 py-1 text-xs text-slate-700 hover:bg-slate-50"
                        onClick={() => {
                          openModuleCodeModal(card);
                        }}
                      >
                        <FileCode className="h-3.5 w-3.5" />
                        {t("查看代码")}
                      </button>
                    </div>
                  </div>

                  <footer className="rounded-b-2xl border-t border-slate-200 bg-slate-50/80 px-3 py-2">
                    <div className="text-[11px] font-medium text-slate-700">{t("模块当前后台活动")}</div>
                    <div className="mt-1 text-[11px] text-slate-600">{t(card.bottomActivity)}</div>
                    <div className="mt-1 max-h-16 overflow-auto rounded-md border border-slate-200 bg-white px-2 py-1 text-[11px] text-slate-500">
                      {card.logs.map((line, idx) => (
                        <div key={`${card.id}-log-${idx}`}>{t(line)}</div>
                      ))}
                    </div>
                  </footer>
                </article>
              ))}
            </div>
          </div>
        </section>

        {isAiPanelOpen ? (
          <aside className="col-span-12 flex h-full flex-col bg-white md:col-span-4 xl:col-span-3">
            <div className="border-b border-slate-200 px-4 py-3">
              <div className="inline-flex items-center gap-2 text-sm font-semibold text-slate-800">
                <Bot className="h-4 w-4 text-cyan-700" />
                {t("AI 交互框")}
              </div>
            </div>

            <div className="border-b border-slate-200 p-3">
              <div className="text-xs font-semibold text-slate-700">{t("MVP 诊断回传")}</div>
              <div className="mt-2 flex items-center gap-2">
                <input
                  value={String(serialBaudRate)}
                  onChange={(evt) => {
                    const next = Number.parseInt(evt.target.value || "0", 10);
                    setSerialBaudRate(Number.isFinite(next) && next > 0 ? next : 115200);
                  }}
                  className="w-24 rounded-lg border border-slate-300 px-2 py-1 text-[11px] outline-none focus:border-cyan-500"
                  placeholder="115200"
                />
                <button
                  type="button"
                  className="rounded-lg border border-slate-300 bg-white px-2 py-1 text-[11px] text-slate-700 hover:bg-slate-50"
                  onClick={() => {
                    if (!deployPort.trim()) {
                      toast.error("请先选择或输入串口号，例如 COM4");
                      return;
                    }
                    setSerialAutoFillEnabled((prev) => !prev);
                  }}
                >
                  {serialAutoFillEnabled ? t("停止串口回填") : t("开始串口回填")}
                </button>
                <button
                  type="button"
                  className="rounded-lg border border-slate-300 bg-white px-2 py-1 text-[11px] text-slate-700 hover:bg-slate-50"
                  onClick={() => {
                    setDiagnosticRawLog("");
                    setDiagnosticEvents([]);
                    setDiagnosticSummary([]);
                    setDiagnosticFix(null);
                  }}
                >
                  {t("清空日志")}
                </button>
              </div>
              <div className="mt-1 text-[11px] text-slate-500">
                {englishMode
                  ? `Port: ${deployPort || t("未选择")} | Baud Rate: ${serialBaudRate} | Status: ${serialAutoFillEnabled ? (isSerialPolling ? t("读取中") : t("已开启")) : t("未开启")}`
                  : `串口: ${deployPort || "未选择"} | 波特率: ${serialBaudRate} | 状态: ${serialAutoFillEnabled ? (isSerialPolling ? "读取中" : "已开启") : "未开启"}`}
              </div>
              <textarea
                value={diagnosticRawLog}
                onChange={(evt) => setDiagnosticRawLog(evt.target.value)}
                className="mt-2 h-24 w-full resize-none rounded-xl border border-slate-300 px-3 py-2 text-[11px] outline-none focus:border-cyan-500"
                placeholder="串口日志会自动回填到这里，也可以手动粘贴，例如: [DIAG]|ts=...|lvl=ERROR|code=...|module=...|msg=..."
              />
              <div className="mt-2 flex gap-2">
                <button
                  type="button"
                  className="rounded-lg border border-cyan-700 bg-cyan-700 px-2 py-1 text-[11px] text-white hover:bg-cyan-800 disabled:opacity-60"
                  onClick={() => {
                    void handleParseDiagnostics();
                  }}
                  disabled={isParsingDiagnostics}
                >
                  {isParsingDiagnostics ? t("解析中...") : t("解析日志")}
                </button>
                <button
                  type="button"
                  className="rounded-lg border border-emerald-700 bg-emerald-700 px-2 py-1 text-[11px] text-white hover:bg-emerald-800 disabled:opacity-60"
                  onClick={() => {
                    void handleSuggestFixFromDiagnostics();
                  }}
                  disabled={isSuggestingFix || !diagnosticEvents.length}
                >
                  {isSuggestingFix ? t("生成中...") : t("生成修复建议")}
                </button>
              </div>

              {diagnosticSummary.length ? (
                <div className="mt-2 rounded-lg border border-slate-200 bg-slate-50 p-2 text-[11px] text-slate-600">
                  <div className="mb-1 font-medium text-slate-700">{t("故障摘要")}</div>
                  {diagnosticSummary.slice(0, 4).map((item, idx) => (
                    <div key={`${item.code}-${item.module}-${idx}`}>
                      {item.code} | {item.level} | {item.module} | x{item.count}
                    </div>
                  ))}
                </div>
              ) : null}

              {diagnosticFix ? (
                <div className="mt-2 rounded-lg border border-emerald-200 bg-emerald-50 p-2 text-[11px] text-emerald-900">
                  <div className="font-medium">{diagnosticFix.headline}</div>
                  {diagnosticFix.suggestions.slice(0, 2).map((item, idx) => (
                    <div key={`${item.title}-${idx}`} className="mt-1">
                      {idx + 1}. {item.title}: {item.patch_hint}
                    </div>
                  ))}
                  <button
                    type="button"
                    className="mt-2 rounded-md border border-emerald-700 bg-white px-2 py-1 text-[11px] text-emerald-800 hover:bg-emerald-100"
                    onClick={() => {
                      setCodeModal({
                        title: "Diagnostic Suggested Prompt",
                        code: diagnosticFix.recommended_prompt,
                        summary: "将该提示词投喂给增量修复流程",
                      });
                    }}
                  >
                    {t("查看建议提示词")}
                  </button>
                </div>
              ) : null}
            </div>

            <div className="flex-1 space-y-3 overflow-auto p-3">
              {chatMessages.map((msg) => (
                <div
                  key={msg.id}
                  className={cn(
                    "max-w-[92%] rounded-2xl border px-3 py-2 text-xs leading-5",
                    msg.role === "user"
                      ? "ml-auto border-cyan-200 bg-cyan-50 text-cyan-900"
                      : "mr-auto border-slate-200 bg-slate-50 text-slate-700",
                  )}
                >
                  {msg.content}
                </div>
              ))}
            </div>

            <div className="border-t border-slate-200 p-3">
              <div className="flex items-end gap-2">
                <textarea
                  value={chatInput}
                  onChange={(evt) => setChatInput(evt.target.value)}
                  className="h-20 flex-1 resize-none rounded-xl border border-slate-300 px-3 py-2 text-xs outline-none focus:border-cyan-500"
                  placeholder={t("输入你的需求，例如: 小车根据按键前后左右运动并在异常时蜂鸣告警")}
                />
                <button
                  type="button"
                  className="inline-flex h-10 w-10 items-center justify-center rounded-xl border border-cyan-700 bg-cyan-700 text-white hover:bg-cyan-800"
                  onClick={sendAiMessage}
                >
                  <Send className="h-4 w-4" />
                </button>
              </div>
              <div className="mt-2 inline-flex items-center gap-1 text-[11px] text-slate-500">
                <Sparkles className="h-3.5 w-3.5" />
                {t("模式由大模型自动判定: regenerate 跑 Step0~5；incremental 跳过 Step0，仅跑 Step1~5。")}
              </div>
            </div>
          </aside>
        ) : null}
      </div>

      {false && !isAiPanelOpen ? (
        <button
          type="button"
          className="fixed right-3 top-1/2 z-40 -translate-y-1/2 rounded-xl border border-cyan-700 bg-cyan-700 px-3 py-2 text-xs text-white shadow-lg hover:bg-cyan-800"
          onClick={() => setIsAiPanelOpen(true)}
        >
          展开AI交互框
        </button>
      ) : null}

      {deployModalOpen ? (
        <div className="fixed inset-0 z-50 flex items-center justify-center bg-slate-900/55 p-4 backdrop-blur-sm">
          <div className="w-full max-w-lg rounded-2xl border border-slate-300 bg-white shadow-2xl">
            <div className="flex items-center justify-between border-b border-slate-200 px-4 py-2.5">
              <div className="inline-flex items-center gap-1.5 text-sm font-medium text-slate-800">
                <ChevronRight className="h-4 w-4 text-emerald-700" />
                {t("部署到板子")}
              </div>
              <button
                type="button"
                className="rounded-lg border border-slate-300 bg-white p-1.5 text-slate-700 hover:bg-slate-50"
                onClick={() => setDeployModalOpen(false)}
              >
                <X className="h-4 w-4" />
              </button>
            </div>
            <div className="space-y-4 p-4">
              <div className="space-y-1">
                <label className="text-xs font-medium text-slate-700">{t("板型 FQBN")}</label>
                <select
                  value={deployFqbn}
                  onChange={(evt) => setDeployFqbn(evt.target.value)}
                  className="w-full rounded-xl border border-slate-300 px-3 py-2 text-sm outline-none focus:border-emerald-500"
                >
                  {(deployBoards.length ? deployBoards : [{ label: "Arduino Uno", fqbn: "arduino:avr:uno" }]).map((item) => (
                    <option key={item.fqbn} value={item.fqbn}>
                      {item.label} ({item.fqbn})
                    </option>
                  ))}
                </select>
              </div>
              <div className="space-y-1">
                <div className="flex items-center justify-between gap-3">
                  <label className="text-xs font-medium text-slate-700">{t("串口")}</label>
                  <button
                    type="button"
                    className="text-xs text-emerald-700 hover:text-emerald-800 disabled:opacity-60"
                    onClick={() => {
                      void loadDeployDevices();
                    }}
                    disabled={isLoadingDeployDevices}
                  >
                    {isLoadingDeployDevices ? t("刷新中...") : t("刷新设备")}
                  </button>
                </div>
                <select
                  value={deployPort}
                  onChange={(evt) => {
                    const nextPort = evt.target.value;
                    setDeployPort(nextPort);
                    const matched = deployPorts.find((item) => item.address === nextPort);
                    if (matched?.fqbn) setDeployFqbn(matched.fqbn);
                  }}
                  className="w-full rounded-xl border border-slate-300 px-3 py-2 text-sm outline-none focus:border-emerald-500"
                >
                  {deployPorts.length > 0 ? (
                    deployPorts.map((item) => (
                      <option key={item.address} value={item.address}>
                        {item.label}
                      </option>
                    ))
                  ) : (
                    <option value={deployPort}>{deployPort || t("未检测到串口")}</option>
                  )}
                </select>
                <input
                  value={deployPort}
                  onChange={(evt) => setDeployPort(evt.target.value)}
                  className="w-full rounded-xl border border-slate-300 px-3 py-2 text-sm outline-none focus:border-emerald-500"
                  placeholder="COM4"
                />
              </div>
              <div className="rounded-xl border border-emerald-100 bg-emerald-50 px-3 py-2 text-xs text-emerald-800">
                {t("当前会优先读取 Windows 侧能看到的串口并提供下拉选择，仍然保留手动输入端口作为兜底。")}
              </div>
              <div className="flex justify-end">
                <button
                  type="button"
                  className="inline-flex items-center gap-2 rounded-xl border border-emerald-700 bg-emerald-700 px-4 py-2 text-sm text-white hover:bg-emerald-800 disabled:cursor-not-allowed disabled:opacity-60"
                  onClick={() => {
                    void handleDeployArduino();
                  }}
                  disabled={isDeploying}
                >
                  <Cpu className="h-4 w-4" />
                  {isDeploying ? t("部署中...") : t("开始部署")}
                </button>
              </div>
            </div>
          </div>
        </div>
      ) : null}

      {codeModal ? (
        <div className="fixed inset-0 z-50 flex items-center justify-center bg-slate-900/55 p-4 backdrop-blur-sm">
          <div className="w-full max-w-5xl rounded-2xl border border-slate-300 bg-white shadow-2xl">
            <div className="flex items-center justify-between border-b border-slate-200 px-4 py-2.5">
              <div className="inline-flex items-center gap-1.5 text-sm font-medium text-slate-800">
                <ChevronRight className="h-4 w-4 text-cyan-700" />
                {englishMode ? translateUiText(codeModal.title) : codeModal.title}
              </div>
              <div className="flex items-center gap-2">
                <button
                  type="button"
                  className="rounded-lg border border-slate-300 bg-white p-1.5 text-slate-700 hover:bg-slate-50"
                  onClick={() => setCodeModal(null)}
                >
                  <X className="h-4 w-4" />
                </button>
              </div>
            </div>
            <div className="p-4">
              <CodeBlock
                title={englishMode ? translateUiText(codeModal.title) : codeModal.title}
                summary={codeModal.summary ? (englishMode ? translateUiText(codeModal.summary) : codeModal.summary) : undefined}
                content={codeModal.code}
                filename={`${englishMode ? translateUiText(codeModal.title) : codeModal.title}.txt`}
              />
            </div>
          </div>
        </div>
      ) : null}
    </div>
  );
}
