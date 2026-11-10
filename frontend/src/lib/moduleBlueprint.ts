export type ModuleId = string;

export type FrontendModuleSpec = {
    id: ModuleId;
    title: string;
    subtitle: string;
    moduleType?: string;
    executor?: string;
    codeSeed: string;
};

export type ModuleConnection = {
    from: ModuleId;
    to: ModuleId;
    relation?: string;
    logic?: string;
};

export type ModuleBlueprint = {
    modules: FrontendModuleSpec[];
    connections: ModuleConnection[];
};

const DEFAULT_MODULES: FrontendModuleSpec[] = [
    {
        id: "step1",
        title: "输入采集模块",
        subtitle: "读取传感器、按键或通信接口的原始输入。根据需求完成采样、有效性检查和量纲转换，再将输入值传给处理模块。具体设备、采样周期、引脚与有效范围待需求确定。",
        moduleType: "source",
        executor: "data-source",
        codeSeed: "# Input Module\nread_inputs()\nnormalize_inputs()",
    },
    {
        id: "step2",
        title: "数据处理模块",
        subtitle: "接收输入模块提供的数据，按需求完成特征计算、滤波或状态更新，并向决策模块输出计算结果。需要跨周期记忆的值应明确初始化与更新顺序。算法、窗口长度和异常输入处理待需求确定。",
        moduleType: "analysis",
        executor: "analysis",
        codeSeed: "# Processing Module\ncompute_features()\nupdate_state()",
    },
    {
        id: "step3",
        title: "决策控制模块",
        subtitle: "读取处理结果与当前状态，依照阈值、条件和状态转移规则决定控制动作。将决策分别传给显示模块和执行器。阈值、边界条件、复位规则及冲突优先级待需求确定。",
        moduleType: "decision",
        executor: "decision",
        codeSeed: "# Decision Module\nif condition_met() => action_a\nelse => action_b",
    },
    {
        id: "step4",
        title: "结果输出模块",
        subtitle: "接收决策模块的状态与数值，转换为显示内容、通信消息或日志。保持输出格式与需求一致，并按指定周期更新。具体输出设备、显示单位和刷新频率待需求确定。",
        moduleType: "display",
        executor: "display",
        codeSeed: "# Output Module\nrender_status()\npublish_output()",
    },
    {
        id: "sim",
        title: "执行器模块",
        subtitle: "接收控制决策并驱动电机、继电器或蜂鸣器等外设，将逻辑输出映射为设备操作。需要明确启动、停止、复位及异常时的动作。具体设备、驱动引脚、持续时间与校准参数待需求确定。",
        moduleType: "alert",
        executor: "alert",
        codeSeed: "# Actuator Module\napply_actuator_command()",
    },
];

const DEFAULT_CONNECTIONS: ModuleConnection[] = [
    { from: "step1", to: "step2" },
    { from: "step2", to: "step3" },
    { from: "step3", to: "step4" },
    { from: "step3", to: "sim" },
];

export const DEFAULT_BLUEPRINT: ModuleBlueprint = {
    modules: DEFAULT_MODULES,
    connections: DEFAULT_CONNECTIONS,
};

export function normalizeBlueprint(blueprint: ModuleBlueprint): ModuleBlueprint {
    const seen = new Set<string>();
    const modules: FrontendModuleSpec[] = [];

    for (const mod of blueprint.modules) {
        const rawId = (mod.id || "").trim();
        const id = rawId || `module_${modules.length + 1}`;
        if (seen.has(id)) continue;
        seen.add(id);
        modules.push({
            id,
            title: mod.title?.trim() || id,
            subtitle: mod.subtitle?.trim() || "由需求自动生成的模块",
            moduleType: (mod.moduleType || inferModuleType(mod.title || id)).trim(),
            executor: (mod.executor || inferExecutorByType(mod.moduleType || inferModuleType(mod.title || id))).trim(),
            codeSeed: mod.codeSeed?.trim() || `# ${mod.title || id}\n// TODO: implement`,
        });
    }

    const connections = blueprint.connections.filter((edge) => seen.has(edge.from) && seen.has(edge.to));
    return {
        modules,
        connections: connections.length ? connections : buildLinearConnections(modules.map((m) => m.id)),
    };
}

export function inferModuleType(text: string): string {
    const s = text.toLowerCase();
    if (/source|input|采集|输入|传感|mic|sensor/.test(s)) return "source";
    if (/analysis|analy|filter|预处理|分析|特征/.test(s)) return "analysis";
    if (/decision|rule|threshold|判定|阈值|策略/.test(s)) return "decision";
    if (/display|ui|render|show|显示|可视/.test(s)) return "display";
    if (/alert|alarm|buzzer|notify|告警|蜂鸣|通知/.test(s)) return "alert";
    return "custom";
}

export function inferExecutorByType(moduleType: string): string {
    const t = moduleType.toLowerCase();
    if (t === "source") return "data-source";
    if (t === "analysis") return "analysis";
    if (t === "decision") return "decision";
    if (t === "display") return "display";
    if (t === "alert") return "alert";
    return "generic";
}

export function buildLinearConnections(ids: string[]): ModuleConnection[] {
    const edges: ModuleConnection[] = [];
    for (let i = 0; i < ids.length - 1; i += 1) {
        edges.push({
            from: ids[i],
            to: ids[i + 1],
            relation: "data_flow",
            logic: "上一模块输出作为下一模块输入",
        });
    }
    return edges;
}

export function autoLayoutModules(
    moduleIds: string[],
    connections?: ModuleConnection[],
): Record<string, { x: number; y: number }> {
    const result: Record<string, { x: number; y: number }> = {};
    const gridFallback = () => {
        const cols = 3;
        const xStart = 48;
        const yStart = 88;
        const xGap = 350;
        const yGap = 290;
        moduleIds.forEach((id, idx) => {
            const row = Math.floor(idx / cols);
            const col = idx % cols;
            result[id] = {
                x: xStart + col * xGap,
                y: yStart + row * yGap,
            };
        });
        return result;
    };

    if (!moduleIds.length) return result;
    if (!connections || !connections.length) return gridFallback();

    const indegree = new Map<string, number>();
    const nextMap = new Map<string, string[]>();
    const depth = new Map<string, number>();

    moduleIds.forEach((id) => {
        indegree.set(id, 0);
        nextMap.set(id, []);
        depth.set(id, 0);
    });

    connections.forEach((edge) => {
        if (!indegree.has(edge.from) || !indegree.has(edge.to) || edge.from === edge.to) return;
        nextMap.get(edge.from)?.push(edge.to);
        indegree.set(edge.to, (indegree.get(edge.to) || 0) + 1);
    });

    const queue = moduleIds.filter((id) => (indegree.get(id) || 0) === 0);
    const topo: string[] = [];

    while (queue.length) {
        const current = queue.shift() as string;
        topo.push(current);
        const baseDepth = depth.get(current) || 0;
        for (const nxt of nextMap.get(current) || []) {
            depth.set(nxt, Math.max(depth.get(nxt) || 0, baseDepth + 1));
            const remain = (indegree.get(nxt) || 0) - 1;
            indegree.set(nxt, remain);
            if (remain === 0) queue.push(nxt);
        }
    }

    // Cyclic graph fallback
    if (topo.length !== moduleIds.length) return gridFallback();

    const layers = new Map<number, string[]>();
    topo.forEach((id) => {
        const d = depth.get(id) || 0;
        const layer = layers.get(d) || [];
        layer.push(id);
        layers.set(d, layer);
    });

    const xStart = 64;
    const xGap = 420;
    const centerY = 260;
    const yGap = 280;

    Array.from(layers.keys())
        .sort((a, b) => a - b)
        .forEach((d) => {
            const ids = layers.get(d) || [];
            const n = ids.length;
            ids.forEach((id, idx) => {
                const offset = idx - (n - 1) / 2;
                result[id] = {
                    x: xStart + d * xGap,
                    y: Math.max(88, centerY + offset * yGap),
                };
            });
        });

    return result;
}

function parseJsonContent(raw: string): unknown {
    const fenced = raw.match(/```(?:json)?\s*([\s\S]*?)```/i);
    const content = (fenced?.[1] ?? raw).trim();
    return JSON.parse(content);
}

export function tryParseBlueprint(raw: string): ModuleBlueprint | null {
    const trimmed = raw.trim();
    if (!trimmed) return null;

    try {
        const parsed = parseJsonContent(trimmed) as Partial<ModuleBlueprint>;
        if (!Array.isArray(parsed.modules)) return null;

        const modules: FrontendModuleSpec[] = parsed.modules
            .map((item, idx) => {
                const typed = item as Partial<FrontendModuleSpec>;
                const id = (typed.id || `module_${idx + 1}`).trim();
                const title = (typed.title || id).trim();
                return {
                    id,
                    title,
                    subtitle: (typed.subtitle || "由大模型规划生成").trim(),
                    moduleType: (typed.moduleType || inferModuleType(title)).trim(),
                    executor: (typed.executor || inferExecutorByType(typed.moduleType || inferModuleType(title))).trim(),
                    codeSeed: (typed.codeSeed || `# ${title}\n// TODO: implement`).trim(),
                };
            })
            .filter((item) => Boolean(item.id));

        if (!modules.length) return null;

        const connections: ModuleConnection[] = Array.isArray(parsed.connections)
            ? parsed.connections
                .map((edge) => {
                    const typed = edge as Partial<ModuleConnection>;
                    return {
                        from: (typed.from || "").trim(),
                        to: (typed.to || "").trim(),
                        relation: (typed.relation || "").trim() || undefined,
                        logic: (typed.logic || "").trim() || undefined,
                    };
                })
                .filter((edge) => edge.from && edge.to)
            : [];

        return normalizeBlueprint({
            modules,
            connections,
        });
    } catch {
        return null;
    }
}

export function inferBlueprintFromRequirement(requirement: string): ModuleBlueprint {
    const normalized = requirement
        .replace(/[。！？；]/g, "\n")
        .replace(/,/g, "\n")
        .split(/\n+/)
        .map((s) => s.trim())
        .filter(Boolean);

    const picked = normalized.slice(0, 8);
    if (!picked.length) return DEFAULT_BLUEPRINT;

    const modules: FrontendModuleSpec[] = picked.map((item, idx) => {
        const id = `module_${idx + 1}`;
        return {
            id,
            title: item.length > 20 ? `${item.slice(0, 20)}...` : item,
            subtitle: `来自需求分解: ${item}`,
            moduleType: inferModuleType(item),
            executor: inferExecutorByType(inferModuleType(item)),
            codeSeed: `# ${item}\n// TODO: 由大模型继续生成具体实现`,
        };
    });

    return normalizeBlueprint({
        modules,
        connections: buildLinearConnections(modules.map((m) => m.id)),
    });
}
