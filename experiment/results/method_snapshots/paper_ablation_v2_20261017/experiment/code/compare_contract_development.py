"""Compare frozen development reports; retain all-task and compiled-only rates."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def compare(baseline_path, run, stages_path):
    baseline, current, stages = read(baseline_path), read(run/'audited_summary.json'), read(stages_path)
    summary = read(run/'summary.json')
    execution = read(run/'execution.json')
    if not current['complete'] or not current['functional_verification_complete']:
        raise ValueError('Current run must have complete artifacts and functional measurements')
    if not baseline.get('replay_complete'):
        raise ValueError('Baseline requires its completed oracle correction audit')
    files = [baseline_path, run/'execution.json', run/'results.json', run/'summary.json', run/'audited_summary.json', stages_path]
    evidence = {str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest() for p in files}
    rows = read(run/'results.json')
    final = {(c['case'], c['repeat']): c for c in stages['cases']}
    failures = []
    for row in rows:
        if row.get('compile_ok') and row.get('functional_status') == 'passed':
            continue
        entry = {key: row.get(key) for key in ('case', 'repeat', 'arm', 'compile_ok', 'functional_status')}
        entry['recorded_failure_stage'] = row.get('failure_stage')
        if row.get('compile_ok'):
            entry['stage'] = 'independent_functional'
            trace = (ROOT/row['artifact']).parents[1]/'host_oracle/trace.log'
            entry['diagnostic'] = trace.read_text(encoding='utf-8').strip() if trace.is_file() else 'unrecorded trace'
            if trace.is_file():
                evidence[str(trace.relative_to(ROOT))] = hashlib.sha256(trace.read_bytes()).hexdigest()
        elif row.get('compile_status') == 'failed':
            entry['stage'] = 'target_compile'
        elif row['arm'] == 'workflow' and final[(row['case'], row['repeat'])]['final_rejection']:
            model = final[(row['case'], row['repeat'])]
            entry.update(stage=model['final_rejection'], diagnostics=model['final_issues'], report=model['report'])
        else:
            entry['stage'] = row.get('failure_stage', 'unclassified')
        failures.append(entry)
    return dict(baseline=str(baseline_path.relative_to(ROOT)), run=str(run.relative_to(ROOT)), evidence_sha256=evidence,
        complete=current['complete'], functional_verification_complete=current['functional_verification_complete'],
        arms={arm: dict(before=baseline['arms'][arm], after=current['arms'][arm]) for arm in current['arms']},
        model=dict(before=baseline['model'], after=current['model']),
        stage_audit=stages['gates_all_planned_tasks'],
        usage=dict(before=baseline['physical_usage'], after=current['physical_usage']),
        wall_seconds=dict(before=baseline['wall_seconds'], after=summary['seconds']),
        method_call_limits=execution['call_limits'], contrasts=current['contrasts'],
        failures=failures, failure_counts=dict(Counter(f['stage'] for f in failures)),
        provenance_verified_rows=sum(r.get('provenance_status') == 'verified_model_chain' for r in rows),
        interpretation='Ten development tasks, three repeats per arm. Before/after runs are separate stochastic samples, '
        'not a controlled attribution of a single change. Current paired intervals compare arms within the same run. '
        'All-task and compiled-only denominators are retained; raw scores are not changed by subsequent source repairs. '
        'Unequal call budgets and shared direct-generation prefixes prevent interpreting gains as compute-matched method gains. '
        'Neither subset performance nor confidence intervals spanning negative differences prove population non-inferiority.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    for name in ('baseline', 'run', 'stages', 'out'):
        parser.add_argument('--'+name, required=True)
    args = parser.parse_args()
    result = compare(ROOT/args.baseline, ROOT/args.run, ROOT/args.stages)
    result['analysis_script_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    with (ROOT/args.out).open('x', encoding='utf-8') as stream:
        json.dump(result, stream, ensure_ascii=False, indent=2)
    print(json.dumps(dict(failure_counts=result['failure_counts'], provenance_verified_rows=result['provenance_verified_rows']), ensure_ascii=False))
