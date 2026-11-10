"""Replay unchanged historical failures against new deterministic source checks."""
import hashlib
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from app.services.source_behavior import add_source_checks, check_source_behavior
from app.services.sysml_compiler import compile_sysml


def main():
    out = ROOT / 'experiment/analysis/source_behavior_repair_round3_20261029'
    out.mkdir(parents=True, exist_ok=True)
    rows = []
    for case in ['simple_004', 'simple_009', 'simple_012', 'medium_027',
                 'simple_006', 'simple_030', 'simple_028',
                 'pceval_humidity_sensor_basic', 'pceval_temperature_sensor_basic']:
        folder = ROOT / 'experiment/output/configured_audit_v1_20261015' / case / 'repeat_1'
        paths = [folder/'contract.json', folder/'model.json']
        before = {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths}
        c, m = [json.loads(p.read_text(encoding='utf-8')) for p in paths]
        requirement = c['configuration_resolution']['original_requirement']
        try:
            checked = add_source_checks(c, requirement)
            report, issues = check_source_behavior(compile_sysml(m['system_sysml2']), checked)
            assert report['status'] == 'failed', (case, report)
            row = dict(case=case, detected_stage='model_behavior', report=report, issues=issues)
        except ValueError as exc:
            # Only the known duplicate fixed-text contract is expected to stop before IR execution.
            expected = 'SOURCE_FIXED_TEXT' if case == 'simple_012' else 'SOURCE_SENSOR_FORMAT'
            assert case in ['simple_012','pceval_humidity_sensor_basic','pceval_temperature_sensor_basic'] and expected in str(exc), (case, exc)
            row = dict(case=case, detected_stage='contract_planning', error=str(exc))
        assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in before.items())
        row.update(original_sha256=before, originals_unchanged=True)
        rows.append(row)
        print(case, row['detected_stage'], 'rejected', flush=True)
    (out/'historical_replay.json').write_text(json.dumps(rows,ensure_ascii=False,indent=2),encoding='utf-8')


if __name__ == '__main__':
    main()
