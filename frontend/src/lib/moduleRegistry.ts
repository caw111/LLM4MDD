import type { FrontendModuleSpec } from "./moduleBlueprint";
import { inferExecutorByType, inferModuleType } from "./moduleBlueprint";

export type ModuleExecutionResult = {
    activity: string;
    log: string;
    code: string;
    subtitle: string;
};

export type ModuleTemplate = {
    type: string;
    defaultSubtitle: string;
    defaultCodeSeed: string;
    executor: string;
};

const MODULE_TEMPLATES: Record<string, ModuleTemplate> = {
    source: {
        type: "source",
        defaultSubtitle: "负责采集原始输入并完成基础预处理。",
        defaultCodeSeed: "# Source Module\nread_input()\nnormalize()",
        executor: "data-source",
    },
    analysis: {
        type: "analysis",
        defaultSubtitle: "负责提取特征并输出可判定的数据。",
        defaultCodeSeed: "# Analysis Module\nextract_features()\ncompute_metrics()",
        executor: "analysis",
    },
    decision: {
        type: "decision",
        defaultSubtitle: "负责规则判断与状态决策输出。",
        defaultCodeSeed: "# Decision Module\napply_rules()\nemit_decision()",
        executor: "decision",
    },
    display: {
        type: "display",
        defaultSubtitle: "负责把状态映射为可视化输出。",
        defaultCodeSeed: "# Display Module\nrender_view()\nrefresh_output()",
        executor: "display",
    },
    alert: {
        type: "alert",
        defaultSubtitle: "负责告警策略和通知输出。",
        defaultCodeSeed: "# Alert Module\nnotify()\nthrottle_alert()",
        executor: "alert",
    },
    custom: {
        type: "custom",
        defaultSubtitle: "通用模块，等待按需求补全实现。",
        defaultCodeSeed: "# Custom Module\n// TODO: implement",
        executor: "generic",
    },
};

export function resolveModuleTemplate(module: Pick<FrontendModuleSpec, "id" | "title" | "subtitle" | "moduleType" | "executor" | "codeSeed">): ModuleTemplate {
    const inferredType = (module.moduleType || inferModuleType(`${module.id} ${module.title} ${module.subtitle || ""}`)).toLowerCase();
    const base = MODULE_TEMPLATES[inferredType] ?? MODULE_TEMPLATES.custom;
    return {
        ...base,
        executor: module.executor || base.executor || inferExecutorByType(inferredType),
        defaultCodeSeed: module.codeSeed || base.defaultCodeSeed,
    };
}

export async function executeDynamicModule(
    module: Pick<FrontendModuleSpec, "id" | "title" | "subtitle" | "moduleType" | "executor" | "codeSeed">,
    requirement: string,
): Promise<ModuleExecutionResult> {
    const tpl = resolveModuleTemplate(module);

    await new Promise((resolve) => setTimeout(resolve, 400));

    return {
        activity: `AI: ${tpl.type} 模块执行完成`,
        log: `执行器 ${tpl.executor} 已完成模块生成`,
        code: [
            `# ${module.title}`,
            `# type: ${tpl.type}`,
            `# executor: ${tpl.executor}`,
            `# requirement: ${requirement}`,
            "",
            tpl.defaultCodeSeed,
            "",
            "// TODO: 替换为真实大模型生成实现",
        ].join("\n"),
        subtitle: module.subtitle || tpl.defaultSubtitle,
    };
}

export function getModuleTemplateRegistry(): Record<string, ModuleTemplate> {
    return MODULE_TEMPLATES;
}
