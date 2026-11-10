"""Explicit denominators and task-cluster paired statistics for development runs."""
from collections import Counter, defaultdict
import random
import statistics

CONTRASTS = [('direct_feedback', 'direct', '迭代贡献'), ('workflow', 'no_sysml_feedback', '模型反馈价值'),
             ('workflow', 'direct_ir', 'SysML独立价值'), ('workflow', 'llm_kernel', '确定性后端价值')]
CALL_BUDGETS = dict(direct=1, direct_feedback=3, workflow=8, no_sysml_feedback=6, direct_ir=8, llm_kernel=9)


def first_response_entries(entries):
    """A first candidate includes every recorded transport attempt before its response."""
    end = next((i + 1 for i, entry in enumerate(entries) if 'response' in entry), len(entries))
    return entries[:end]


def ci95(values):
    if not values:
        return None
    rng = random.Random(20261012)
    draws = sorted(statistics.mean(rng.choices(values, k=len(values))) for _ in range(2000))
    return [draws[49], draws[1949]]


def rate(passes, n):
    return dict(passes=passes, denominator=n, rate=passes / n if n else None)


def functional_rates(rows):
    compiled = [r for r in rows if r.get('compile_ok')]
    passed = sum(r.get('functional_status') == 'passed' for r in rows)
    unknown = sum(r.get('functional_status') not in ('passed', 'simulation_failed', 'failed') for r in compiled)
    unknown += sum(not r.get('compile_ok') and bool(r.get('infrastructure_error') or r.get('record_missing')) for r in rows)
    return dict(all_tasks=rate(passed, len(rows)), compiled_only=rate(passed, len(compiled)),
                unresolved=unknown, complete=unknown == 0,
                interpretation='Observed success fraction; lower bound if unresolved > 0. Untested artifacts are not proven failures.')


def paired(rows, left, right, metric):
    groups = {arm: {(r['case'], r['repeat']): r for r in rows if r['arm'] == arm} for arm in (left, right)}
    shared = sorted(groups[left].keys() & groups[right].keys())
    by_task = defaultdict(list)
    used_pairs = 0
    def value(r):
        return float(r.get('compile_ok', False) if metric == 'compile' else r.get('functional_status') == 'passed')
    for key in shared:
        pair = [groups[left][key], groups[right][key]]
        if any(r.get('infrastructure_error') or r.get('record_missing') for r in pair):
            continue
        if metric == 'functional' and any(r.get('compile_ok') and r.get('functional_status') not in ('passed', 'simulation_failed', 'failed') for r in pair):
            continue
        used_pairs += 1
        by_task[key[0]].append(value(groups[left][key]) - value(groups[right][key]))
    means = [statistics.mean(v) for v in by_task.values()]
    return dict(task_repeat_pairs=used_pairs, unresolved_pairs=len(shared)-used_pairs, tasks=len(means), difference=statistics.mean(means) if means else None,
                ci95=ci95(means), unit='task-cluster paired difference')


def summarize(rows, reports=(), *, expected_keys=None):
    recorded_count = len(rows)
    keys = [(r['case'], r['repeat'], r['arm']) for r in rows]
    if len(keys) != len(set(keys)):
        raise ValueError('Duplicate case/repeat/arm records')
    rows = list(rows)
    if expected_keys is not None:
        expected_keys = set(expected_keys)
        if not set(keys) <= expected_keys:
            raise ValueError('Unexpected case/repeat/arm records')
        rows += [dict(case=case, repeat=repeat, arm=arm, record_missing=True,
                      compile_ok=False, compile_status='not_run', functional_status='not_run')
                 for case, repeat, arm in sorted(expected_keys - set(keys))]
    arms = {}
    for arm in sorted({r['arm'] for r in rows}):
        group = [r for r in rows if r['arm'] == arm]
        repeats = sorted({r['repeat'] for r in group})
        repeat_rates = [statistics.mean(float(r.get('compile_ok', False)) for r in group if r['repeat'] == i) for i in repeats]
        functional_repeats = [statistics.mean(float(r.get('functional_status') == 'passed') for r in group if r['repeat'] == i) for i in repeats]
        backend_recorded = sum(type(r.get('backend_ok')) is bool for r in group)
        backend = {**rate(sum(r.get('backend_ok') is True for r in group),len(group)),
                   'unrecorded_or_not_reached':len(group)-backend_recorded,
                   'recorded':backend_recorded,
                   'interpretation':'Observed successes over all planned tasks; absent backend evidence is not a measured backend failure.'}
        if not backend_recorded:
            backend['rate'] = None
        arms[arm] = {'compile': rate(sum(r.get('compile_ok', False) for r in group), len(group)),
                     'missing_records': sum(bool(r.get('record_missing')) for r in group),
                     'rate_interpretation': 'Observed success fraction over planned records; lower bound while records are missing or unresolved.',
                     'backend': backend,
                     'functional': functional_rates(group), 'infrastructure': sum(bool(r.get('infrastructure_error')) for r in group),
                     'failures': dict(Counter(r.get('failure_stage', r.get('compile_status', 'unknown')) for r in group if not r.get('compile_ok') and not r.get('record_missing') and not r.get('infrastructure_error'))),
                     'functional_failures_after_compile':sum(bool(r.get('compile_ok')) and r.get('functional_status') in ('failed','simulation_failed') for r in group),
                     'compile_repeat_rates': repeat_rates, 'compile_repeat_sd': statistics.stdev(repeat_rates) if len(repeat_rates) > 1 else None,
                     'functional_repeat_rates': functional_repeats, 'functional_repeat_sd': statistics.stdev(functional_repeats) if len(functional_repeats) > 1 else None,
                     'allocated_usage': {key: sum(r.get(key, 0) or 0 for r in group) for key in ('llm_calls', 'tokens', 'reported_cost_usd', 'llm_seconds', 'compile_seconds')}}
    attempts = [p['attempts'] for p in reports if p.get('attempts')]
    failed_initial = [a for a in attempts if not a[0]['ok']]
    stages = {}
    for stage in ('sysml_generation', 'sysml_to_ir', 'ir_to_il'):
        observed = [a[-1]['stages'][stage] for a in attempts if stage in a[-1].get('stages', {})]
        stages[stage] = {**rate(sum(v is True for v in observed), len(observed)),
                         'not_reached': sum(v is None for v in observed), 'unrecorded': len(attempts) - len(observed)}
    return {'records': recorded_count, 'planned_records': len(rows), 'missing_records': len(rows)-recorded_count, 'arms': arms, 'model': {
        'first_pass': rate(sum(a[0]['ok'] for a in attempts), len(attempts)),
        'eventual_pass': rate(sum(a[-1]['ok'] for a in attempts), len(attempts)),
        'repair_success': rate(sum(a[-1]['ok'] for a in failed_initial), len(failed_initial)),
        'attempts': sum(len(a) for a in attempts), 'stages': stages},
        'contrasts': [dict(left=left, right=right, question=question, compile=paired(rows, left, right, 'compile'),
                           functional_observed=paired(rows, left, right, 'functional')) for left, right, question in CONTRASTS],
        'max_method_calls_excluding_transport_retries': CALL_BUDGETS,
        'budget_caveat': 'Unequal compute budgets; shared prefixes allocated to each arm. Do not sum allocated usage as physical cost.'}
