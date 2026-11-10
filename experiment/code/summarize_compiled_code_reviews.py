"""Report static-review outcomes conditional on target-board compilation.

Reads frozen experiments without changing any original verdict or artifact.
"""
import argparse
from collections import Counter
from datetime import datetime, timezone
import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
DEFAULT = ROOT / 'experiment/output/code_review_all_20261023'
STATUSES = ('no_identified_defect', 'defect_identified', 'risk_identified',
            'inconclusive', 'review_invalid', 'pending')
LABELS = {
    'ablation_direct': '冻结单轮：直接生成',
    'ablation_direct_feedback': '冻结单轮：直接生成＋编译反馈',
    'ablation_workflow': '冻结单轮：完整工作流',
    'ablation_no_contract_feedback': '消融：关闭接口修复',
    'ablation_no_model_feedback': '消融：关闭模型反馈',
    'ablation_blind_resampling': '控制：无诊断重新采样',
    'ablation_llm_backend': '消融：LLM替换确定性后端',
    'historical_direct': '历史：直接生成',
    'workflow_267': '累计工作流：267题编译通过版本',
    'workflow_280': '累计工作流：280题编译通过版本',
    'direct_gpt61': 'GPT-6.1 Sol单次直接生成',
}


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def aggregate(rows):
    identities = [(r['group'], r['case']) for r in rows]
    assert len(identities) == len(set(identities)), 'Duplicate group/case'
    groups = {}
    for group in sorted({r['group'] for r in rows}):
        planned = [r for r in rows if r['group'] == group]
        assert all(type(r['compile_ok']) is bool for r in planned)
        compiled = [r for r in planned if r['compile_ok']]
        counts = Counter(r['review_status'] for r in compiled)
        assert set(counts) <= set(STATUSES), 'Compiled artifact lacks a review mapping'
        n, clean = len(compiled), counts['no_identified_defect']
        pending = counts['pending']
        # Strictly greater than 95%, including when n*0.95 is an integer.
        required = 95 * n // 100 + 1
        groups[group] = dict(
            planned=len(planned), compiled=n,
            compile_rate=n / len(planned),
            compiled_review_counts={s: counts[s] for s in STATUSES},
            reviewed_compiled=n - pending,
            review_complete=pending == 0,
            review_passes=clean,
            review_pass_rate=clean / n if n and not pending else None,
            provisional_clean_fraction=clean / n if n else None,
            maximum_rate_with_pending_clean=(clean + pending) / n if n else None,
            required_for_over_95_percent=required if n else None,
            over_95_percent=(100 * clean > 95 * n) if n and not pending else None,
            already_cannot_reach_95_without_reassessment_or_repair=(
                100 * (clean + pending) <= 95 * n) if n else None,
            all_record_counts=dict(Counter(r['review_status'] for r in planned)),
        )
        assert sum(groups[group]['compiled_review_counts'].values()) == n
    return groups


def self_check():
    # Compilation failures must never inflate/deflate the conditional denominator;
    # uncertain reviews cannot be silently counted as passes, and pending is not final.
    rows = [dict(group='x', case=str(i), compile_ok=i < 20,
                 review_status=('no_identified_defect' if i < 19 else
                                'risk_identified' if i == 19 else 'no_code_generated'))
            for i in range(23)]
    result = aggregate(rows)['x']
    assert result['compiled'] == 20 and result['review_passes'] == 19
    assert result['review_pass_rate'] == .95 and not result['over_95_percent']
    assert result['required_for_over_95_percent'] == 20
    rows[19]['review_status'] = 'pending'
    assert aggregate(rows)['x']['review_pass_rate'] is None
    rows[19]['review_status'] = 'review_invalid'
    assert aggregate(rows)['x']['review_pass_rate'] == .95
    assert aggregate([dict(group='x', case='1', compile_ok=False,
                           review_status='no_code_generated')])['x']['review_pass_rate'] is None
    try:
        aggregate(rows + [rows[0]])
    except AssertionError:
        pass
    else:
        raise AssertionError('Duplicate records were accepted')
    print('Conditional-denominator, uncertainty, strict-threshold and uniqueness checks passed')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--out', type=Path, default=DEFAULT)
    parser.add_argument('--self-check', action='store_true')
    args = parser.parse_args()
    if args.self_check:
        self_check()
        return
    out = args.out.resolve()
    source_paths = [out / name for name in ('execution.json', 'results.json', 'summary.json')]
    before = {str(p): digest(p) for p in source_paths}
    execution, rows, original_summary = map(read, source_paths)
    assert len(rows) == len(execution['rows']) == 3256
    assert {(r['group'], r['case'], r['compile_ok'], r.get('review_key')) for r in rows} == {
        (r['group'], r['case'], r['compile_ok'], r.get('review_key')) for r in execution['rows']}
    changed = [p for p, h in execution['hashes'].items() if digest(ROOT / p) != h]
    assert not changed, 'Frozen source/input changed'
    groups = aggregate(rows)
    assert len(groups) == 11 and all(g['planned'] == 296 for g in groups.values())
    assert all(digest(Path(p)) == h for p, h in before.items()), 'Snapshot changed during reading'
    summary = dict(
        updated_utc=datetime.now(timezone.utc).isoformat(),
        all_experiments_review_complete=original_summary['complete'],
        all_compiled_reviews_complete=all(g['review_complete'] for g in groups.values()),
        records=len(rows), groups=groups, source_sha256=before,
        frozen_originals_unchanged=True, review_usage=original_summary['usage'],
        pass_definition='Existing no_identified_defect verdict only; risks, uncertain/invalid reviews are not passes; pending means the final rate is unavailable.',
        denominator_definition='Compilation: all 296 planned cases per group. Review: only target-board compile_ok=true cases of that same group.',
        limitations='LLM-assisted static review, not functional correctness or hardware execution; historical cumulative groups are not same-version/equal-budget ablations.',
    )
    lines = ['# 编译通过率与编译后代码审查结果', '',
        '编译通过率＝目标板编译通过题数／296；审查通过率＝审查未发现明确缺陷且检查信息完整的题数／本组编译通过题数。',
        '沿用原冻结审查标准：风险、信息不足、格式异常均不计审查通过。未完成的组不发布最终审查通过率，也不把未完成题从分母删除。',
        '这里的“审查通过”仅指LLM辅助静态审查未发现问题，不代表实物运行或功能正确。', '',
        '| 实验组 | 编译通过 | 编译通过率 | 已审查／编译通过 | 审查通过数 | 最终审查通过率 |',
        '|---|---:|---:|---:|---:|---:|']
    for group, g in groups.items():
        rate = f"{g['review_pass_rate']:.1%}" if g['review_pass_rate'] is not None else '未完成'
        lines.append(f"| {LABELS[group]} | {g['compiled']}/296 | {g['compile_rate']:.1%} | "
                     f"{g['reviewed_compiled']}/{g['compiled']} | {g['review_passes']} | {rate} |")
    lines += ['', '## 编译通过代码的审查状态', '',
        '| 实验组 | 明确缺陷 | 风险 | 信息不足 | 格式异常 | 待审查 |',
        '|---|---:|---:|---:|---:|---:|']
    for group, g in groups.items():
        c = g['compiled_review_counts']
        lines.append('| ' + LABELS[group] + ' | ' + ' | '.join(str(c[k]) for k in
            ('defect_identified', 'risk_identified', 'inconclusive', 'review_invalid', 'pending')) + ' |')
    latest = groups['workflow_280']
    lines += ['', f"最新累计工作流的审查分母为{latest['compiled']}，超过95%需至少"
        f"{latest['required_for_over_95_percent']}题通过审查。该目标并非已测得的结果。",
        '冻结单轮完整工作流、历史267题版本和后续280题版本分别报告，不合并成一次独立对照实验。',
        '如果新增缺陷修复，应保存为另一个版本，重新编译并用相同标准重新审查，另报调用预算；不能覆盖本轮原始结果。', '',
        '全部审查是否完成：' + str(original_summary['complete']),
        '审查实际调用消耗：' + json.dumps(original_summary['usage'], ensure_ascii=False)]
    (out / 'compiled_review_metrics.json').write_text(json.dumps(summary, ensure_ascii=False, indent=2), encoding='utf-8')
    (out / 'REPORT_COMPILE_REVIEW_ZH.md').write_text('\n'.join(lines) + '\n', encoding='utf-8')
    print(json.dumps(dict(records=len(rows), groups=len(groups), complete=summary['all_compiled_reviews_complete'],
                         report=str(out / 'REPORT_COMPILE_REVIEW_ZH.md')), ensure_ascii=True))


if __name__ == '__main__':
    main()
