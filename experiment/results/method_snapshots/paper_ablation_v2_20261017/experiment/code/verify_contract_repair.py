"""Real-model interface-only verification of all 87 failures plus source-balanced controls."""
import argparse
import hashlib
import json
from collections import Counter
from concurrent.futures import ThreadPoolExecutor, wait, FIRST_COMPLETED
from pathlib import Path

from run_current_comparison import ROOT, ResumableCalls, save, metrics
from app.services.interface_planner import plan_interface_contract, ContractPlanningError

SOURCE = ROOT / 'experiment/output/configured_audit_v1_20261015'


def main(out, prior=None, selected_cases=None):
    OUT = (ROOT / out).resolve()
    OUT.relative_to(ROOT / 'experiment/output')
    previous = json.loads((SOURCE/'execution.json').read_text(encoding='utf-8'))
    failures = json.loads((SOURCE/'failure_analysis.json').read_text(encoding='utf-8'))['failures']
    failed_ids = {r['case'] for r in failures if r['stage'] == 'interface_contract'}
    previous_digest = None
    if prior:
        prior_path = (ROOT/prior).resolve()/'summary.json'
        prior_report = json.loads(prior_path.read_text(encoding='utf-8'))
        assert prior_report['complete'], 'Finish the preceding verification before selecting remaining failures'
        failed_ids = {r['case'] for r in prior_report['failures'] if r['group'] == 'prior_failure'}
        previous_digest = hashlib.sha256(prior_path.read_bytes()).hexdigest()
    if selected_cases:
        assert set(selected_cases) <= failed_ids, 'Only select documented prior failures'
        failed_ids = set(selected_cases)
    controls = set()
    for dataset in sorted({c['dataset'] for c in previous['cases']}):
        for c in [c for c in previous['cases'] if c['dataset'] == dataset]:
            rows = json.loads((SOURCE/c['id']/'repeat_1/results.json').read_text(encoding='utf-8'))
            if any(r['arm'] == 'workflow' and r['compile_ok'] for r in rows):
                controls.add(c['id'])
                if sum(x in controls for x in [v['id'] for v in previous['cases'] if v['dataset'] == dataset]) == 4:
                    break
    cases = []
    for c in previous['cases']:
        if c['id'] in failed_ids | controls:
            original = json.loads((SOURCE/c['id']/'repeat_1/configuration_resolution.json').read_text(encoding='utf-8'))
            cases.append(dict(id=c['id'], board=c['fqbn'], dataset=c['dataset'],
                              requirement=original['original_requirement'], group='prior_failure' if c['id'] in failed_ids else 'control'))
    files = [Path(__file__), ROOT/'app/main.py', *sorted((ROOT/'app/services').glob('*.py'))]
    config = dict(scope='Interface planning only; deliberately selected prior failures plus 4 prior compiled controls per source. Not a new end-to-end success-rate experiment.',
                  cases=cases, model=previous['model'], temperature=.2, max_tokens=6000, max_repairs=2,
                  prior_verification_summary_sha256=previous_digest,
                  hashes={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
    OUT.mkdir(parents=True, exist_ok=True)
    if (OUT/'execution.json').exists():
        assert json.loads((OUT/'execution.json').read_text(encoding='utf-8')) == config, 'Verification method changed; use a new version'
    else:
        save(OUT/'execution.json', config)
    def evaluate(case):
        folder = OUT/case['id']; result = folder/'result.json'
        if result.exists():
            row = json.loads(result.read_text(encoding='utf-8'))
            if not row.get('infrastructure_error'):
                return row
        calls = ResumableCalls(folder/'calls', config['model'])
        row = dict(case=case['id'], dataset=case['dataset'], group=case['group'], accepted=False)
        try:
            contract, report = plan_interface_contract(calls, case['requirement'], {}, target_board=case['board'], max_repairs=2)
            save(folder/'contract.json', contract); save(folder/'planning.json', report)
            row.update(accepted=True, attempts=len(report['attempts']))
        except ContractPlanningError as exc:
            save(folder/'planning.json', dict(ok=False, attempts=exc.attempts, blocking_category=exc.blocking_category))
            row.update(attempts=len(exc.attempts), blocking_category=exc.blocking_category, error=exc.attempts[-1]['error'])
        except Exception as exc:
            row.update(infrastructure_error=type(exc).__name__)
        row.update(metrics(calls.entries))
        save(result, row)
        return row
    iterator = iter(cases); rows = []; stop = False
    with ThreadPoolExecutor(max_workers=6) as pool:
        pending = {pool.submit(evaluate, c) for c in list(cases[:6])}
        iterator = iter(cases[6:])
        while pending:
            done, pending = wait(pending, return_when=FIRST_COMPLETED)
            for task in done:
                row = task.result(); rows.append(row)
                stop |= bool(row.get('infrastructure_error'))
                save(OUT/'progress.json', dict(completed=len(rows), planned=len(cases), accepted=sum(r['accepted'] for r in rows), infrastructure_stop=stop))
                if not stop and (case := next(iterator, None)) is not None:
                    pending.add(pool.submit(evaluate, case))
    changed = [p for p,h in config['hashes'].items() if hashlib.sha256((ROOT/p).read_bytes()).hexdigest() != h]
    summary = dict(scope=config['scope'], complete=len(rows)==len(cases) and not stop and not changed,
                   planned=len(cases), records=len(rows), source_changed=changed,
                   groups={g:dict(n=sum(r['group']==g for r in rows), accepted=sum(r['group']==g and r['accepted'] for r in rows)) for g in ('prior_failure','control')},
                   failures=[r for r in rows if not r['accepted']],
                   usage={k:sum(r.get(k,0) or 0 for r in rows) for k in ('llm_calls','tokens','reported_cost_usd')})
    save(OUT/'summary.json', summary)
    print(json.dumps({k:v for k,v in summary.items() if k != 'failures'},ensure_ascii=False), flush=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--out', required=True)
    parser.add_argument('--prior')
    parser.add_argument('--case', action='append')
    args = parser.parse_args()
    main(args.out, args.prior, args.case)
