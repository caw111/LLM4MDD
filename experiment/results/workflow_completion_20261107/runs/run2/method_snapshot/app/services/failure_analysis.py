"""Conservative diagnostic classification shared by repair and experiment audits."""
import re


def classify_issue(issue):
    code = str(issue.get('code', '')).lower()
    text = code + ' ' + str(issue.get('message', '')).lower()
    stage = issue.get('stage')
    if stage == 'requirements':
        return stage, 'requirement_coverage', '恢复独立需求/接口清单中的遗漏项，并执行独立行为断言'
    if code == 'json_incomplete':
        return 'sysml_generation', 'incomplete_response', '保留完成原因和原始候选；在现有预算内重试完整 JSON，不删除需求'
    if code == 'llm_refusal':
        return 'sysml_generation', 'provider_refusal', '保留拒绝结果并停止该生成步骤，不将其计为成功'
    if issue.get('syntax') or 'syntax' in code or 'bundle_format' in code:
        return 'sysml_generation', 'syntax_or_bundle', '修复语法/JSON；使用已验证的可执行子集示例'
    if 'pin_' in code:
        return 'hardware_adapter', 'pin_contract', '核对板型、引脚别名、共享总线与设备映射'
    if stage == 'ir_to_code' or 'ir_backend' in code:
        return 'ir_to_code', 'backend', '修复确定性后端，增加类型、算术及执行顺序回归'
    lowering = stage == 'sysml_to_ir' or any(k in code for k in ('lowering', 'ir_contract'))
    location = 'sysml_to_ir' if lowering else 'sysml_generation'
    for words, reason, fix in (
        (('binding', 'bind ', 'driver'), 'binding', 'bind a = child.x；同类型、唯一驱动，根输入连接子模块输入'),
        (('succession', 'chain', 'order', 'unsequenced'), 'action_order', '在 perform action 内用 first start; then 显式连接全部动作'),
        (('initial', 'persistent', 'assignment', 'state'), 'persistent_state', 'attribute count : Integer := 0；仅在动作中 assign 更新'),
        (('linking', 'resolve', 'reference', 'scope'), 'reference_scope', '修正名称、作用域及 ScalarValues 导入'),
        (('type', 'incompatible'), 'type', '统一 Boolean/Integer/Real 类型并保留所需计算'),
        (('unsupported', 'not yet supported', 'multiplicity'), 'unsupported_semantics', '补充有明确执行语义的转换规则；未支持时保留失败'),
    ):
        if any(word in text for word in words):
            return location, reason, fix
    return location, 'unclassified', '保留原始诊断，人工核实；不推断缺失证据'


def priority(issue):
    """Syntax first, then references, types and executable-subset diagnostics."""
    stage, reason, _ = classify_issue(issue)
    if reason == 'syntax_or_bundle':
        return 0
    if stage in ('sysml_to_ir', 'ir_to_code'):
        return 3
    return {'reference_scope': 1, 'type': 2}.get(reason, 3)


def diagnostic_fingerprint(issues):
    # Ignore line/column movement, but preserve identifiers, constants and messages.
    return sorted(set((str(i.get('code', '')), re.sub(r'\s+', ' ', str(i.get('message', ''))).strip())
                      for i in issues if i.get('severity', 'error') == 'error'))
