"""Offline regression on recorded proposals, never a new generation experiment."""
import argparse
import hashlib
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from app.services.interface_planner import plan_interface_contract, ContractPlanningError
from app.services.llm_json import CompletionText


def main(source, out):
    source, out = (ROOT / source).resolve(), (ROOT / out).resolve()
    assert source != out and out.is_relative_to(ROOT / 'experiment/output')
    cfg = json.loads((source / 'execution.json').read_text(encoding='utf-8'))
    rows = []
    for case in cfg['cases']:
        folder = source / case['id'] / 'repeat_1'
        history = json.loads((folder / 'contract_planning.json').read_text(encoding='utf-8'))
        requirement = json.loads((folder / 'configuration_resolution.json').read_text(encoding='utf-8'))['original_requirement']
        attempts = []
        for i, attempt in enumerate(history['attempts']):
            raw = attempt['response']
            metadata = attempt.get('completion', {})
            response = CompletionText(raw, finish_reason=metadata.get('finish_reason'), refusal=metadata.get('refusal'))
            try:
                contract, report = plan_interface_contract(lambda _: ('recorded-proposal', response), requirement,
                    {}, target_board=case['fqbn'], max_repairs=0)
                attempts.append(dict(index=i, accepted=True, actions=report['attempts'][0]['construction']))
                target = out / case['id']; target.mkdir(parents=True, exist_ok=True)
                (target / 'contract.json').write_text(json.dumps(contract, ensure_ascii=False, indent=2), encoding='utf-8')
            except ContractPlanningError as exc:
                attempts.append(dict(index=i, accepted=False, error=exc.attempts[-1]['error']))
            except (ValueError, TypeError, KeyError) as exc:
                attempts.append(dict(index=i, accepted=False, error=type(exc).__name__ + ': ' + str(exc)))
        rows.append(dict(case=case['id'], originally_accepted=bool(history.get('ok', history['attempts'][-1]['ok'])),
                         now_accepted=any(a['accepted'] for a in attempts), attempts=attempts,
                         history_sha256=hashlib.sha256((folder/'contract_planning.json').read_bytes()).hexdigest()))
    summary = dict(scope='Offline revalidation of historical proposals under changed configuration/validators; no model requests, no new compile or functional success-rate claim.',
        tasks=len(rows), previously_accepted=sum(r['originally_accepted'] for r in rows),
        accepted=sum(r['now_accepted'] for r in rows),
        recovered=[r['case'] for r in rows if not r['originally_accepted'] and r['now_accepted']],
        regressions=[r['case'] for r in rows if r['originally_accepted'] and not r['now_accepted']], cases=rows)
    out.mkdir(parents=True, exist_ok=True)
    (out/'offline_revalidation.json').write_text(json.dumps(summary, ensure_ascii=False, indent=2), encoding='utf-8')
    print(json.dumps({k:v for k,v in summary.items() if k != 'cases'}, ensure_ascii=False))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--source', required=True)
    parser.add_argument('--out', required=True)
    args = parser.parse_args()
    main(args.source, args.out)
