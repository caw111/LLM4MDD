"""Export auditable shared configuration decisions without any model/service calls."""
import argparse
import copy
import hashlib
import json
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from app.services.configuration_policy import resolve_configuration, VERSION
from run_contract_development import INTERPRETATIONS


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--out', default='experiment/configured/configuration_' + VERSION.rsplit('-', 1)[-1])
    args = parser.parse_args()
    out = (ROOT / args.out).resolve()
    out.relative_to((ROOT / 'experiment').resolve())
    if (out/'manifest.json').exists():
        old = json.loads((out/'manifest.json').read_text(encoding='utf-8'))
        if old.get('configuration_policy') != VERSION:
            raise ValueError('Do not overwrite a different configuration policy version; use a new output directory')
    source = ROOT / 'experiment/formal/manifest.json'
    manifest = json.loads(source.read_text(encoding='utf-8'))
    hashes = {p: hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in (
        'app/services/configuration_policy.py', 'app/services/interface_planner.py', 'app/services/hardware_adapter.py',
        'app/services/wiring_inventory.py', 'app/services/interface_registry.py',
        'experiment/code/run_contract_development.py')}
    cases, reasons = [], Counter()
    for original in manifest['cases']:
        case = copy.deepcopy(original)
        public = INTERPRETATIONS.get(case['id'], '')
        task = case['requirement'] + ('\n' + public if public else '')
        policy = resolve_configuration(task, {}, board=case['fqbn'])
        case.update(original_requirement=case['requirement'], requirement=policy['effective_requirement'],
                    configuration_resolution=policy, public_interpretation=public)
        reasons.update(c['reason'] for c in policy['changes'])
        cases.append(case)
    assert len(cases) == len({c['id'] for c in cases}) == len(manifest['cases'])
    out.mkdir(parents=True, exist_ok=True)
    configured = dict(manifest, cases=cases, configuration_policy=VERSION,
                      original_manifest_sha256=hashlib.sha256(source.read_bytes()).hexdigest(), policy_hashes=hashes)
    (out/'manifest.json').write_text(json.dumps(configured, ensure_ascii=False, indent=2), encoding='utf-8')
    summary = dict(cases=len(cases), changed_cases=sum(bool(c['configuration_resolution']['changes']) for c in cases),
                   decisions=dict(reasons), model_calls=0, original_manifest_modified=False,
                   functional_correctness_verified=False, policy_hashes=hashes)
    (out/'summary.json').write_text(json.dumps(summary, ensure_ascii=False, indent=2), encoding='utf-8')
    print(json.dumps(summary))


if __name__ == '__main__':
    main()
