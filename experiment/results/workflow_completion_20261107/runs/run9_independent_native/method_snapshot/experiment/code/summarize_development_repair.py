"""Portable development-only statistics, with optional paired before/after."""
import argparse
import json
from pathlib import Path
try:
    from .experiment_metrics import summarize, paired
except ImportError:
    from experiment_metrics import summarize, paired


def read_run(root):
    rows = [r for p in root.glob('*/repeat_*/results.json') for r in json.loads(p.read_text(encoding='utf-8'))]
    reports = [json.loads(p.read_text(encoding='utf-8')) for p in root.glob('*/repeat_*/model_validation.json')]
    return rows, reports


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--before', type=Path)
    args = parser.parse_args()
    rows, reports = read_run(args.out)
    execution = json.loads((args.out/'execution.json').read_text(encoding='utf-8'))
    expected = {(case, arm, repeat) for case in execution['cases'] for arm in execution['arms'] for repeat in range(1, execution['repetitions'] + 1)}
    actual = {(r['case'], r['arm'], r['repeat']) for r in rows}
    if not actual <= expected:
        raise SystemExit('Unexpected records outside development execution')
    result = summarize(rows, reports, expected_keys={(case, repeat, arm) for case, arm, repeat in expected})
    result['expected_records'] = len(expected)
    result['generation_complete'] = actual == expected and not any(r.get('infrastructure_error') for r in rows)
    result['scope'] = 'Development only. Missing records are unobserved, not method failures; observed rates are lower bounds.'
    calls = [e for p in args.out.glob('*/repeat_*/*_calls/calls.json') for e in json.loads(p.read_text(encoding='utf-8'))]
    result['physical_usage'] = {'calls': len(calls), 'tokens': sum((e.get('usage') or {}).get('total_tokens', 0) for e in calls),
                              'reported_cost_usd': sum((e.get('usage') or {}).get('cost', 0) or 0 for e in calls),
                              'seconds': sum(e.get('seconds', 0) for e in calls)}
    result['job_seconds'] = sum(json.loads(p.read_text(encoding='utf-8'))['job_seconds'] for p in args.out.glob('*/repeat_*/timing.json'))
    if args.before:
        before, previous_reports = read_run(args.before)
        result['before'] = summarize(before, previous_reports)
        result['before_after_paired_compile'] = {}
        for arm in result['arms']:
            merged = [dict(r, arm=version) for version, group in [('before', before), ('after', rows)] for r in group if r['arm'] == arm]
            result['before_after_paired_compile'][arm] = paired(merged, 'after', 'before', 'compile')
    (args.out/'audited_summary.json').write_text(json.dumps(result, ensure_ascii=False, indent=2), encoding='utf-8')
    print(json.dumps({'records': len(rows), 'generation_complete': result['generation_complete'], 'model': result['model']}, ensure_ascii=False))


if __name__ == '__main__':
    main()
