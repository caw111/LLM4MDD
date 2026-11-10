"""Replay previously observed serial-reference failures locally, without model calls."""
import hashlib
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from app.services.interface_planner import plan_interface_contract, ContractPlanningError, requires_clock
from app.services.interface_registry import assemble_interface_contract, register_contract


def replay(source, out):
    cases = {c['id']: c for c in json.loads((ROOT/'experiment/formal/manifest.json').read_text(encoding='utf-8'))['cases']}
    observed = json.loads((source/'SERIAL_REFERENCE_DIAGNOSTIC_TRIALS.json').read_text(encoding='utf-8'))['cases']
    rows = []
    for item in observed:
        name = item['case']
        path = source/name/'repeat_1/contract_planning.json'
        frozen = path.read_bytes()
        last = json.loads(frozen)['attempts'][-1]
        response = last['response']
        parsed = json.loads(re.sub(r'^\s*```(?:json)?\s*\n|\n```\s*$', '', response.strip()))
        candidate, actions = assemble_interface_contract(parsed, require_clock=requires_clock(cases[name]['requirement']))
        registered = register_contract(candidate)
        # Registration itself must resolve the declared Boolean trigger, even if
        # another independent field still prevents a complete contract acceptance.
        for channel in registered['interface_registry']['serial_channels']:
            assert registered['interface_registry']['ports'][channel['trigger_port']] == dict(direction='output', type='bool')
        row = dict(case=name, historical_file_sha256=hashlib.sha256(frozen).hexdigest(),
                   historical_error=last['error'], construction=actions, registered_relationships='verified',
                   interface_registry=registered['interface_registry'])
        try:
            _, report = plan_interface_contract(lambda _: ('historical-response-replay', response),
                cases[name]['requirement'], {}, target_board=cases[name]['fqbn'], max_repairs=0)
            row.update(complete_contract_accepted=True, contract_sha256=report['contract_sha256'])
        except ContractPlanningError as exc:
            row.update(complete_contract_accepted=False, remaining_error=exc.attempts[-1]['error'],
                       blocking_category=exc.blocking_category)
        assert path.read_bytes() == frozen
        rows.append(row)
    report = dict(scope='Local replay of the last historical response; zero new LLM/API calls; no behavior or workflow success-rate claim',
                  source=str(source.relative_to(ROOT)), cases=len(rows),
                  registered_relationships_verified=sum(r['registered_relationships']=='verified' for r in rows),
                  complete_contract_accepted=sum(r['complete_contract_accepted'] for r in rows), results=rows)
    out.mkdir(parents=True, exist_ok=True)
    (out/'offline_replay.json').write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf-8')
    print(json.dumps({k:v for k,v in report.items() if k!='results'},ensure_ascii=False))
    return report


if __name__ == '__main__':
    replay(ROOT/'experiment/output/interface_repairs_full_20261013', ROOT/'experiment/output/interface_registry_20261014')
