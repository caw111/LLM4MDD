"""Offline field-failure replay and supplied receive fixtures; never calls an API model."""
import hashlib
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from app.services.interface_planner import plan_interface_contract, contract_digest
from run_contract_development import INTERPRETATIONS


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2), encoding='utf-8')


def validate(out):
    cases = {c['id']: c for c in json.loads((ROOT/'experiment/formal/manifest.json').read_text(encoding='utf-8'))['cases']}
    rows = []
    for cid in ('simple_004', 'simple_019', 'simple_027'):
        path = ROOT/'experiment/output/interface_registry_full_20261014'/cid/'repeat_1/contract_planning.json'
        frozen = path.read_bytes()
        requirement = cases[cid]['requirement'] + '\n' + INTERPRETATIONS.get(cid, '')
        for i, attempt in enumerate(json.loads(frozen)['attempts'], 1):
            contract, report = plan_interface_contract(lambda _: ('recorded-response', attempt['response']),
                requirement, {}, target_board=cases[cid]['fqbn'], max_repairs=0)
            artifact = out/cid/f'attempt_{i}.contract.json'
            save(artifact, contract)
            rows.append(dict(case=cid, historical_attempt=i, historical_file_sha256=hashlib.sha256(frozen).hexdigest(),
                historical_error=attempt['error'], kind='exact_historical_response_replay', interface_accepted=True,
                construction=report['attempts'][0]['construction'], contract_sha256=contract_digest(contract),
                artifact=str(artifact.relative_to(ROOT))))
        assert path.read_bytes() == frozen
    fixtures = []
    for cid, name in [('simple_009', 'serial_command'), ('simple_011', 'serial_integer')]:
        path = ROOT/'tools/examples'/f'{name}.proposal.json'
        original = path.read_bytes()
        # Provided examples exercise the newly implemented operation. Historical
        # responses explicitly reported unsupported and are not rewritten/scored.
        contract, report = plan_interface_contract(lambda _: ('supplied-receive-fixture', original.decode('utf-8')),
            cases[cid]['requirement'], {}, target_board=cases[cid]['fqbn'], max_repairs=0)
        artifact = out/cid/'supplied.contract.json'
        save(artifact, contract)
        fixtures.append(dict(case=cid, kind='supplied_receive_fixture_not_model_generation', interface_accepted=True,
            fixture_sha256=hashlib.sha256(original).hexdigest(), contract_sha256=contract_digest(contract),
            registry=contract['interface_registry'], construction=report['attempts'][0]['construction'],
            artifact=str(artifact.relative_to(ROOT))))
    report = dict(scope='Zero paid/model API calls. Nine exact historical field-error responses and two supplied receiver fixtures; not a new LLM/workflow success-rate experiment.',
        historical_response_count=len(rows), historical_response_accepted=sum(r['interface_accepted'] for r in rows),
        historical_task_count=3, supplied_receive_fixtures=fixtures, replays=rows,
        method_sha256={str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest() for p in [
            ROOT/'app/main.py', ROOT/'app/services/interface_registry.py', ROOT/'app/services/interface_planner.py',
            ROOT/'app/services/hardware_adapter.py', ROOT/'app/services/serial_receiver.py']})
    save(out/'offline_replay.json', report)
    print(json.dumps({k: v for k, v in report.items() if k not in ('replays', 'supplied_receive_fixtures', 'method_sha256')}, ensure_ascii=False))
    return report


if __name__ == '__main__':
    validate(ROOT/'experiment/output/interface_hardening_20261014')
