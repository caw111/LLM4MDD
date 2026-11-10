"""Read-only analysis of paired records; never pool different method snapshots."""
import argparse
import hashlib
import json
from collections import Counter, defaultdict
from pathlib import Path


def analyze(root, repo):
    config = json.loads((root / 'execution.json').read_text(encoding='utf-8'))
    rows = [r for p in root.glob('*/repeat_*/results.json') for r in json.loads(p.read_text(encoding='utf-8'))]
    keys = [(r['case'], r['repeat'], r['arm']) for r in rows]
    assert len(keys) == len(set(keys)), 'Duplicate records'
    expected = {(c['id'], repeat, arm) for c in config['cases']
                for repeat in range(1, config['repetitions'] + 1) for arm in config['arms']}
    assert set(keys) <= expected, 'Unexpected records'
    changed = [p for p, h in config['hashes'].items()
               if not (repo / p).is_file() or hashlib.sha256((repo / p).read_bytes()).hexdigest() != h]
    grouped = defaultdict(dict)
    for r in rows:
        grouped[r['case'], r['repeat']][r['arm']] = r
    paired = [g for g in grouped.values() if set(g) == set(config['arms'])
              and not any(r.get('infrastructure_error') for r in g.values())]
    counts = {a: sum(g[a]['compile_ok'] for g in paired) for a in config['arms']}
    stages = Counter()
    errors = Counter()
    details = []
    for g in paired:
        r = g['workflow']
        if r['compile_ok']:
            continue
        stage = r.get('failure_stage', r.get('compile_status', 'unknown'))
        stages[stage] += 1
        folder = root / r['case'] / f"repeat_{r['repeat']}"
        diagnostics = []
        for filename in ('contract_planning.json', 'model_validation.json'):
            path = folder / filename
            if not path.exists():
                continue
            report = json.loads(path.read_text(encoding='utf-8'))
            attempts = report.get('attempts', [])
            if attempts:
                last = attempts[-1]
                if last.get('error'):
                    diagnostics.append(str(last['error']))
                diagnostics += [str(i.get('code', '')) + ': ' + str(i.get('message', ''))
                                for i in last.get('issues', []) if i.get('severity', 'error') == 'error']
        diagnostics = list(dict.fromkeys(diagnostics)) or [r.get('error', stage)]
        for diagnostic in diagnostics:
            errors[diagnostic] += 1
        details.append({'case': r['case'], 'repeat': r['repeat'], 'dataset': r['dataset'],
                        'stage': stage, 'blocking_category': r.get('blocking_category'),
                        'diagnostics': diagnostics, 'direct_compiles': g['direct']['compile_ok'],
                        'direct_feedback_compiles': g['direct_feedback']['compile_ok']})
    complete = set(keys) == expected and not changed and not any(r.get('infrastructure_error') for r in rows)
    result = dict(complete=complete, planned_records=len(expected), recorded_records=len(rows),
                  comparable_pairs=len(paired), current_source_changed=changed, compile_passes=counts,
                  workflow_failure_stages=dict(stages), failures=details,
                  functional_status={a: dict(Counter(g[a].get('functional_status', 'unknown') for g in paired)) for a in config['arms']},
                  by_dataset={d: {a: {'n':sum(g[a]['dataset']==d for g in paired),
                                     'compile':sum(g[a]['dataset']==d and g[a]['compile_ok'] for g in paired)}
                                  for a in config['arms']} for d in sorted({r['dataset'] for r in rows})})
    (root / 'failure_analysis.json').write_text(json.dumps(result, ensure_ascii=False, indent=2), encoding='utf-8')
    lines = ['# 当前版本对比与失败诊断' + ('' if complete else '（未完成，不能作为全量结论）'), '',
             f"模型 `{config['model']}`，计划 {len(config['cases'])} 题 × {config['repetitions']} 轮；当前 {len(rows)}/{len(expected)} 条记录。",
             f'无基础设施异常的完整配对：{len(paired)}；方法文件变化：{len(changed)}。', '',
             '| 方法 | 配对题目编译通过 |', '|---|---:|']
    for a, n in counts.items():
        lines.append(f'| {a} | {n}/{len(paired)}' + (f'（{n/len(paired):.1%}）' if paired else '') + ' |')
    lines += ['', '## 工作流失败阶段', '', '| 阶段 | 数量 |', '|---|---:|']
    lines += [f'| {s} | {n} |' for s, n in stages.most_common()]
    lines += ['', '## 原始诊断（按涉及失败记录数）', '']
    lines += [f'- {n} 次：{e.replace(chr(10), " ")[:700]}' for e, n in errors.most_common(20)]
    actions = {
        'interface_contract': '优先检查契约规划诊断：区分硬件能力确实缺失、字段/类型错误、引脚映射错误和需求规则误拒绝。支持能力缺失应扩展并测试适配器；契约形状错误应约束结构化输出，不应放松需求检查。',
        'sysml_generation': '检查最后一轮模型诊断，区分语言语法、绑定/作用域、项目可执行子集和需求契约错误；反馈需针对具体构造，不能通过删除行为过关。',
        'sysml_to_ir': '用人工核实的等价模型复现转换失败；合法且属于支持范围的模型应修复转换器，并添加行为回归。',
        'ir_to_code': '固定失败 IR，复现确定性后端；检查类型、表达式和状态执行顺序。',
        'hardware_adapter': '核对契约端口、设备驱动与模型 ABI；修复适配映射并以设备桩执行验证。',
        'arduino_compile': '从真实编译日志区分代码缺陷、依赖或板型配置问题，保留不同类别。'}
    lines += ['', '## 后续优化优先级', '']
    for s, n in stages.most_common():
        lines.append(f'- {s}（{n} 条）：' + actions.get(s, '抽查逐题原始日志后再定位，不凭阶段名假定根因。'))
    lines += ['', '编译通过不等于功能正确。功能状态详见 failure_analysis.json；无独立 oracle 的题目不计为已证明功能成功。',
              '本次为现有、曾用于开发的数据上的诊断评测，不应称为完全未见测试。仅一轮不能证明重复采样稳定性。',
              '直接生成与直接反馈共享首稿，费用不能重复相加；三组总调用限制不同，不能宣称严格等成本。',
              '没有修改被测生产代码、提示或原始结果；基础设施异常不混入方法失败，缺失记录不从计划分母中删除。']
    (root / 'DIAGNOSIS_ZH.md').write_text('\n'.join(lines)+'\n', encoding='utf-8')
    print(json.dumps({k: result[k] for k in ('complete', 'recorded_records', 'comparable_pairs', 'compile_passes', 'workflow_failure_stages')}))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--out', required=True)
    args = parser.parse_args()
    repo = Path(__file__).resolve().parents[2]
    root = (repo / args.out).resolve()
    root.relative_to((repo / 'experiment/output').resolve())
    analyze(root, repo)
