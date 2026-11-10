"""Offline component validation and fault injection, never a real-LLM experiment."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from app.services.sysml_compiler import validate_executable_sysml, ir_to_il
from app.services.behavior_contract import check_contract
from app.services.arduino_adapter import compiled_kernel_text
from app.main import convert_il_to_c, ConvertIlToCRequest


def save(path, data):
    path.write_text(json.dumps(data, ensure_ascii=False, indent=2), encoding='utf-8')


def verify(out):
    source_path = ROOT/'tools/sysml/environment_report.sysml'
    oracle_path = ROOT/'experiment/development/oracles/environment_report.cpp'
    contract_path = ROOT/'tools/examples/environment_report.contract.json'
    original = source_path.read_text(encoding='utf-8')
    oracle = oracle_path.read_text(encoding='utf-8')
    contract = json.loads(contract_path.read_text(encoding='utf-8'))
    candidates = {'baseline': original,
                  'wrong_period': original.replace('5000', '4000'),
                  'wrong_threshold_boundary': original.replace('temperatureC > 30.0', 'temperatureC >= 30.0'),
                  'constant_sensor_output': original.replace('= humidityPercent;', '= 50.0;'),
                  'missing_report_trigger': original.replace('reportPulse := accumulated >= 5000', 'reportPulse := false')}
    compiler = shutil.which('g++')
    if compiler is None:
        raise RuntimeError('g++ is required for the independent host behavior oracle')
    out.mkdir(parents=True, exist_ok=False)
    files = [source_path, oracle_path, contract_path, Path(__file__), ROOT/'app/main.py',
             ROOT/'app/services/sysml.py', ROOT/'app/services/sysml_compiler.py',
             ROOT/'app/services/arduino_adapter.py', ROOT/'app/services/behavior_contract.py']
    report = {'scope': 'Offline deterministic component and oracle sensitivity only; not LLM pass rates or hardware validation',
              'live_model_calls': 0, 'formal_experiment_started': False, 'complete': False,
              'source_sha256': {str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest() for p in files}, 'cases': []}
    save(out/'report.json', report)
    for name, source in candidates.items():
        started = time.monotonic()
        folder = out/name; folder.mkdir()
        (folder/'system.sysml').write_text(source, encoding='utf-8')
        row = {'name': name, 'expected_behavior': 'passed' if name == 'baseline' else 'failed'}
        try:
            validation = validate_executable_sysml(source)
            row.update(model_ok=validation['ok'], stages=validation['stages'], issues=validation['issues'])
            if validation['ok']:
                ir = validation['ir']
                row['coverage'] = check_contract(ir, contract)
                save(folder/'ir.json', ir)
                il = folder/'model.il.xml'; il.write_text(ir_to_il(ir), encoding='utf-8')
                generated = convert_il_to_c(ConvertIlToCRequest(il_file_path=str(il), out_c_dir=str(folder/'c')))
                kernel = compiled_kernel_text(generated.contents['define.h'], generated.contents['ProgramPage1.c'])
                cpp = folder/'oracle.cpp'; cpp.write_text(kernel + '\n' + oracle, encoding='utf-8')
                binary = folder/'oracle.exe'
                build = subprocess.run([compiler, '-std=c++17', str(cpp), '-o', str(binary)], capture_output=True, text=True, timeout=60)
                (folder/'compile.log').write_text(build.stdout + build.stderr, encoding='utf-8')
                row['compile_ok'] = build.returncode == 0
                if row['compile_ok']:
                    run = subprocess.run([str(binary)], capture_output=True, text=True, timeout=10)
                    (folder/'behavior.log').write_text(run.stdout + run.stderr, encoding='utf-8')
                    row.update(behavior_status='passed' if run.returncode == 0 else 'failed',
                               exit_code=run.returncode,
                               failed_assertions=sorted(set(line[5:] for line in run.stdout.splitlines() if line.startswith('FAIL '))))
                row['oracle_cpp_sha256'] = hashlib.sha256(cpp.read_bytes()).hexdigest()
            # Deliberate mutants undergo the full language/compiler checks. Static coverage
            # failures are recorded but do not substitute for independent oracle execution.
            row['expectation_met'] = bool(row.get('compile_ok') and
                row.get('behavior_status') == row['expected_behavior'] and
                (name == 'baseline' or row.get('failed_assertions')))
        except Exception as exc:
            row.update(error_type=type(exc).__name__, error=str(exc), expectation_met=False)
        row['seconds'] = round(time.monotonic() - started, 3)
        report['cases'].append(row)
        save(out/'report.json', report)
        print(name, row.get('behavior_status', row.get('error_type', 'not_run')), flush=True)
    report['complete'] = len(report['cases']) == len(candidates)
    report['all_expectations_met'] = all(row['expectation_met'] for row in report['cases'])
    save(out/'report.json', report)
    return report


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--out', type=Path, default=ROOT/'experiment/output/component_oracle_v1')
    args = parser.parse_args()
    result = verify(args.out.resolve())
    raise SystemExit(0 if result['all_expectations_met'] else 1)
