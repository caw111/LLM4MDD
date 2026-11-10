"""Read-only inventory. Never merge historical runs, fixtures and reference tests."""
import argparse
from collections import Counter
import csv
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from app.services.failure_analysis import classify_issue


def audit(root):
    records, inventory = [], []
    def add(path, cohort, case, stage, reason, fix, evidence):
        records.append(dict(source=str(path.relative_to(root)), cohort=cohort, case=case,
                            stage=stage, reason=reason, fix=fix, evidence=str(evidence)[:4000]))
    paths = sorted(set(root.glob('experiment/output/run_report_*.json')) |
                   set(root.glob('experiment/runs/*/report.json')) |
                   set(root.glob('experiment/output/*/*/repeat_*/results.json')))
    for path in paths:
        data = json.loads(path.read_text(encoding='utf-8-sig'))
        rows = data if isinstance(data, list) else data.get('results', [])
        if isinstance(rows, dict):
            for case, result in rows.items():
                for issue in result.get('issues', []):
                    add(path, 'fixture', case, *classify_issue(issue), issue)
            continue
        cohort = ('legacy_generation' if path.name.startswith('run_report_') else
                  'reference_oracle' if isinstance(data, dict) and 'mode' in data else 'six_arm')
        inventory.append({'source': str(path.relative_to(root)), 'cohort': cohort, 'records': len(rows)})
        for index, row in enumerate(rows):
            case = row.get('case', row.get('task_id', row.get('id', str(index))))
            if cohort == 'legacy_generation':
                for arm in ('flow', 'direct'):
                    if row.get(arm + '_ok') is False:
                        error = row.get(arm + '_error', '')
                        # Old HTTP failures do not carry reliable stage provenance.
                        if any(s in error.lower() for s in ('timed out', 'connection', 'connecterror', 'winerror 10061', 'winerror 10054')):
                            stage, reason, fix = 'infrastructure', 'transport', '恢复连接并重跑开发样例'
                        elif '/generate/c-from-step1' in error and 'C bundle JSON' in error:
                            stage, reason, fix = 'legacy_llm_c', 'invalid_c_bundle', '旧版LLM生成C的JSON格式失败；当前确定性后端需单独验证，不能计作IR后端失败'
                        elif 'HTTP 422 /convert/c-to-arduino' in error:
                            stage, reason, fix = 'hardware_adapter', 'adapter_quality_check', '核对显示/串口输入等设备能力与需求，保留原始质量诊断'
                        else:
                            stage, reason, fix = 'unknown_legacy', 'missing_stage_evidence', '补齐逐阶段诊断；旧记录不能推断模型失败位置'
                        add(path, cohort, case + ':' + arm, stage, reason, fix, error)
            elif cohort == 'reference_oracle':
                status = row.get('simulation_status', 'not_run')
                if status not in ('passed', 'not_run'):
                    add(path, cohort, case, 'infrastructure' if status.startswith(('blocked', 'infrastructure')) else 'functional',
                        status, '先恢复独立测试基础设施；参考程序结果不计入生成方法通过率', status)
                if row.get('compile_status') not in ('passed', 'not_run', None):
                    add(path, cohort, case, 'hardware_adapter', row['compile_status'], '核查依赖和编译日志', row.get('compile', {}))
            elif not row.get('compile_ok'):
                stage = row.get('failure_stage', 'target_compile')
                add(path, cohort, case + ':' + row['arm'], stage, row.get('error', row.get('compile_status', 'unknown')),
                    '结合该任务 model_validation.json 和 compile.log 定位，不能将编译失败一概归因于适配层', row)
    # Separate per-attempt diagnostics from terminal task failures.
    validations = sorted(set(root.glob('experiment/output/*/*/repeat_*/model_validation.json')) |
                         set(root.glob('experiment/runs/*/repair/attempt_*/validation.json')))
    for path in validations:
        data = json.loads(path.read_text(encoding='utf-8-sig'))
        for attempt in data.get('attempts', [data]):
            for issue in attempt.get('issues', []):
                if issue.get('severity', 'error') == 'error':
                    add(path, 'attempt_diagnostic', str(attempt.get('attempt', path.parent.name)), *classify_issue(issue), issue)
    return {'inventory': inventory, 'counts': dict(Counter(r['cohort'] + '/' + r['stage'] + '/' + r['reason'] for r in records)),
            'limitations': ['Counts are evidence records, not unique tasks or pass rates.',
                           'Legacy runs, fixtures, reference oracles and six-arm runs remain separate.',
                           'No six-arm low-pass-rate explanation is possible without its raw run directory.'], 'records': records}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--root', type=Path, default=ROOT)
    parser.add_argument('--out', type=Path, default=ROOT/'experiment/analysis/failure_audit')
    args = parser.parse_args()
    result = audit(args.root)
    args.out.mkdir(parents=True, exist_ok=True)
    (args.out/'audit.json').write_text(json.dumps(result, ensure_ascii=False, indent=2), encoding='utf-8')
    with (args.out/'failure_table.csv').open('w', encoding='utf-8-sig', newline='') as stream:
        writer = csv.DictWriter(stream, fieldnames=['source', 'cohort', 'case', 'stage', 'reason', 'fix', 'evidence'])
        writer.writeheader(); writer.writerows(result['records'])
    print(json.dumps({'sources': len(result['inventory']), 'counts': result['counts']}, ensure_ascii=False))


if __name__ == '__main__':
    main()
