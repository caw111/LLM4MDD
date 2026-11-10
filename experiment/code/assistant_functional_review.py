"""Local file tools for the assistant's code review. No API client or model calls."""
import argparse
from collections import Counter
from datetime import datetime, timezone
import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'experiment/output/code_review_all_20261023/execution.json'
OUT = ROOT / 'experiment/output/assistant_functional_review_20261023'


def read(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2), encoding='utf-8')


def plan():
    original = read(SOURCE)
    expected = dict(source=str(SOURCE), source_sha256=sha(SOURCE),
                    scope='All eleven original comparison/ablation groups, 296 cases each; review compiled code only.',
                    method='Assistant reads code and saved local test evidence; no new external API calls; no code changes.',
                    statuses=['judged_pass', 'judged_fail', 'uncertain'],
                    limitation='Code-based functional judgment is not physical execution or proof of general functional correctness.')
    if (OUT / 'execution.json').exists():
        assert read(OUT / 'execution.json') == expected
    else:
        save(OUT / 'execution.json', expected)
    return original


def case_keys(config, case):
    return list(dict.fromkeys(r['review_key'] for r in config['rows']
                             if r['case'] == case and r['compile_ok']))


def record(config, path):
    jobs = {j['key']: j for j in config['jobs']}
    for note in read(path):
        key = case_keys(config, note['case'])[note['candidate'] - 1]
        job = jobs[key]
        source = ROOT / job['artifact']
        assert sha(source) == job['code_sha256']
        code = source.read_text(encoding='utf-8')
        assert note['verdict'] in ('judged_pass', 'judged_fail', 'uncertain')
        assert note['reason'] and note['quotes']
        assert all(q and q in code for q in note['quotes']), 'Unsupported evidence quote'
        result = dict(review_key=key, code_sha256=job['code_sha256'], reviewer='assistant',
                      case=note['case'], verdict=note['verdict'], reason=note['reason'],
                      evidence=[dict(code_quote=q, line=code[:code.index(q)].count('\n') + 1)
                                for q in note['quotes']],
                      limitations='Code-based judgment; hardware execution was not established by this review.')
        target = OUT / 'decisions' / (key + '.json')
        if target.exists():
            assert read(target) == result, 'Existing assistant judgment cannot be silently overwritten'
        else:
            save(target, result)


def report(config):
    jobs = {j['key']: j for j in config['jobs']}
    compiled_keys = {r['review_key'] for r in config['rows'] if r['compile_ok']}
    decisions = {}
    for path in (OUT / 'decisions').glob('*.json'):
        d = read(path)
        key = d['review_key']
        assert key in compiled_keys and path.stem == key
        assert d['verdict'] in ('judged_pass', 'judged_fail', 'uncertain')
        assert d['reason'] and d['evidence'] and d['reviewer'] == 'assistant'
        job = jobs[key]
        code_path = ROOT / job['artifact']
        assert sha(code_path) == job['code_sha256'] == d['code_sha256']
        code = code_path.read_text(encoding='utf-8')
        for evidence in d['evidence']:
            assert evidence['code_quote'] and evidence['code_quote'] in code
        assert key not in decisions
        decisions[key] = d
    rows = [dict(r, functional_judgment=('not_buildable' if not r['compile_ok'] else
                  decisions.get(r['review_key'], {}).get('verdict', 'pending')))
            for r in config['rows']]
    groups = {}
    for group in sorted({r['group'] for r in rows}):
        selected = [r for r in rows if r['group'] == group]
        counts = Counter(r['functional_judgment'] for r in selected)
        compiled = sum(r['compile_ok'] for r in selected)
        groups[group] = dict(planned=len(selected), compiled=compiled, compile_rate=compiled / len(selected),
            counts=dict(counts), complete=counts['pending'] == 0,
            code_judgment_pass_rate=(counts['judged_pass'] / compiled if compiled and not counts['pending'] else None))
    summary = dict(complete=len(decisions) == len(compiled_keys), records=len(rows),
        unique_compiled_artifacts=len(compiled_keys), reviewed_unique=len(decisions),
        new_external_api_calls=0, groups=groups, updated_utc=datetime.now(timezone.utc).isoformat(),
        interpretation='judged_pass is an assistant code-based judgment, not an independently executed full-task pass.')
    save(OUT / 'results.json', rows)
    save(OUT / 'summary.json', summary)
    lines = ['# 本线程助手直接审查：功能实现判断', '',
        '未新增外部模型API调用，未修改生成代码。按任务的有效需求、目标板和完整源代码直接判断；同一代码及上下文在不同组间复用判断。',
        '“判断可通过”表示已阅读的代码实现了明确需求且未发现功能矛盾，属于静态判断；不能写成已经通过实物或全量运行测试。',
        '存在明确功能矛盾列为失败；选型、接线或需求不足以判断时列为无法确认。尚未阅读的代码单列待审阅，不冒充无法确认或通过。', '',
        f'独立代码已审阅{len(decisions)}/{len(compiled_keys)}份；完整审查完成：{summary["complete"]}。', '',
        '| 实验组 | 编译通过／296 | 编译率 | 判断可通过 | 明确功能缺陷 | 无法确认 | 待审阅 | 最终代码判断通过率 |',
        '|---|---:|---:|---:|---:|---:|---:|---:|']
    for group, g in groups.items():
        counts = g['counts']
        rate = f"{g['code_judgment_pass_rate']:.1%}" if g['code_judgment_pass_rate'] is not None else '未完成'
        lines.append(f"| {group} | {g['compiled']}/296 | {g['compile_rate']:.1%} | " +
                     ' | '.join(str(counts.get(k, 0)) for k in ('judged_pass', 'judged_fail', 'uncertain', 'pending')) +
                     ' | ' + rate + ' |')
    lines += ['', '判断通过率的分母为本组编译通过题数；未编译通过者保留为not_buildable，不计功能通过。',
        '逐份判断、代码原文和行号在decisions/；结果映射在results.json。旧API审查与本次助手判断分别保存，不能合并为同一审查实验。',
        '历史累计267、280题版本与冻结单轮完整工作流分别报告，不能合并为同版本等预算消融。']
    (OUT / 'REPORT_ZH.md').write_text('\n'.join(lines) + '\n', encoding='utf-8')
    print(json.dumps(summary, ensure_ascii=True))


def packet(config, case, candidate=None, groups_filter=None):
    jobs = {j['key']: j for j in config['jobs']}
    selected = [r for r in config['rows'] if r['case'] == case and r['compile_ok']]
    by_key = {}
    for row in selected:
        by_key.setdefault(row['review_key'], []).append(row['group'])
    print('CASE', case)
    seen_requirements = set()
    for index, (key, groups) in enumerate(by_key.items(), 1):
        if candidate is not None and index != candidate:
            continue
        if groups_filter and not set(groups).intersection(groups_filter):
            continue
        j = jobs[key]
        path = ROOT / j['artifact']
        assert sha(path) == j['code_sha256']
        print('\nCANDIDATE', index, key, ','.join(groups), 'BOARD', j['board'])
        if j['requirement'] not in seen_requirements:
            print('REQUIREMENT', j['requirement'])
            seen_requirements.add(j['requirement'])
        print('ARTIFACT', j['artifact'])
        # Omit only whole-line comments and blank lines; keep all executable code.
        print('\n'.join(f'{n}: {line}' for n, line in enumerate(path.read_text(encoding='utf-8').splitlines(), 1)
                        if line.strip() and not line.lstrip().startswith('//')
                        and not (line.strip().startswith('/*') and line.strip().endswith('*/'))))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--case')
    parser.add_argument('--candidate', type=int)
    parser.add_argument('--group', action='append')
    parser.add_argument('--record', type=Path)
    args = parser.parse_args()
    config = plan()
    if args.record:
        record(config, args.record)
        report(config)
    elif args.case:
        packet(config, args.case, args.candidate, args.group)
    else:
        report(config)


if __name__ == '__main__':
    main()
