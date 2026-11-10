"""Collect the recorded comparison and repair evidence into a portable bundle."""
import gzip
import hashlib
import json
from pathlib import Path
import shutil

ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / 'experiment/output'
DEST = ROOT / 'experiment/results'


def read(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def artifact(path):
    if not path or not Path(path).is_file():
        return None
    path = Path(path)
    path.resolve().relative_to(ROOT)
    target = DEST / 'artifacts' / (digest(path) + path.suffix)
    target.parent.mkdir(parents=True, exist_ok=True)
    if not target.exists():
        shutil.copyfile(path, target)
    assert digest(path) == digest(target)
    return target.relative_to(DEST).as_posix()


def evidence(result_path, target):
    """Preserve original JSON and compress its trace without changing the bytes."""
    data = read(result_path)
    target.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(result_path, target / 'recorded_result.json')
    trace = result_path.parent / 'trace.jsonl'
    if trace.is_file():
        if not (target / 'trace.jsonl.gz').is_file():
            with trace.open('rb') as source, (target / 'trace.jsonl.gz').open('wb') as raw:
                with gzip.GzipFile(filename='', mode='wb', fileobj=raw, mtime=0) as compressed:
                    shutil.copyfileobj(source, compressed)
        data['packaged_trace_sha256'] = digest(trace)
    for name in ('build.stderr.txt', 'stderr.txt', 'stdout.txt'):
        source = result_path.parent / name
        if source.is_file() and source.stat().st_size:
            shutil.copyfile(source, target / name)
    return data


def generation_files(source):
    """Preserve nearby contracts, models and compiler inputs, without build caches."""
    if not source:
        return {}
    source = Path(source)
    files = {}
    names = ('contract.json', 'model.json', 'model_validation.json', 'interface_registry.json',
             'serial_registry.json', 'configuration_resolution.json', 'contract_planning.json')
    for parent in list(source.parents)[1:4]:
        for name in names:
            path = parent / name
            if path.is_file():
                files[path.relative_to(OUTPUT).as_posix()] = artifact(path)
        for directory in ('lowered', 'ir', 'c'):
            folder = parent / directory
            if folder.is_dir():
                for path in folder.rglob('*'):
                    if path.is_file() and path.suffix.lower() in ('.json', '.sysml', '.xml', '.c', '.h'):
                        files[path.relative_to(OUTPUT).as_posix()] = artifact(path)
    return files


def baseline_plan(case, result_path, measured):
    checks = [check['test'] for check in measured.get('checks', []) if 'test' in check]
    wanted = {check['id'] for check in checks}
    candidates = []
    for argument in measured.get('build_command', []):
        if argument.startswith('-I'):
            candidates.append(Path(argument[2:]) / 'test_spec.json')
    for parent in ('core_reassessment_20261106_pending217', 'core_reassessment_20261106',
                   'core_reassessment_20261106_remaining', 'core_reassessment_20261106_reviewed'):
        folder = OUTPUT / parent
        candidates.extend(folder.glob(f'*plans/{case}.json'))
        candidates.extend(folder.glob(f'test_inputs/{measured.get("case", case)}/test_spec.json'))
        record = folder / 'cases' / f'{case}.json'
        if record.is_file():
            for run in read(record).get('runs', []):
                if Path(run.get('result', '_missing_')) == result_path:
                    candidates.insert(0, Path(run['plan']))
    candidates.extend((OUTPUT / 'behavior_all296_20261023').glob(f'cases/{case}/tested_plan.json'))
    candidates.extend((OUTPUT / 'behavior_all296_20261023').glob(f'embed_native/{case}/native_plan.json'))
    for path in candidates:
        if not path.is_file():
            continue
        data = read(path)
        plan = data.get('plan', data)
        if {check['id'] for check in plan.get('checks', [])} == wanted:
            return plan
    return None


def main():
    DEST.mkdir(parents=True, exist_ok=True)
    groups = read(OUTPUT / 'all_groups_host_smoke_20261023/frozen_manifest.json')
    ledger = read(OUTPUT / 'core_reassessment_20261106_pending217/decision_round/results_296.json')
    repairs = {r['case']: r for r in read(OUTPUT / 'manual_code_repairs_20261106/merged_results.json')}
    requirements = {r['id']: r for r in read(ROOT / 'experiment/formal/manifest.json')['cases']}
    original = {r['case']: r for r in groups if r['group'] == 'workflow_267'}
    reviews = {r['case']: r for r in read(OUTPUT / 'core_reassessment_20261106_pending217/review_217.json')}
    reviews.update({r['case']: r for r in read(OUTPUT / 'core_reassessment_20261106_pending217/decision_round/review_119.json')})
    portable_groups = []
    for row in groups:
        source = Path(row.get('source') or '_missing_')
        portable_groups.append(dict(row, source=artifact(source),
            cpp=artifact(source.parent.parent / 'build/sketch/sketch.ino.cpp') if source.is_file() else None,
            original_source=row.get('source')))
    assert len(portable_groups) == 13 * 296
    save(DEST / 'groups.json', portable_groups)
    save(DEST / 'generation_artifacts.json', [dict(group=r['group'], case=r['case'],
         files=generation_files(r.get('source'))) for r in groups])
    final = []
    for old in ledger:
        case = old['case']
        repaired = repairs.get(case, {})
        passed_repair = repaired.get('core_verdict') == 'core_scenarios_passed'
        passed = old['core_status'] == 'core_pass' or passed_repair
        source = repaired.get('artifact') if passed_repair else original[case].get('source')
        item = dict(case=case, dataset=old['dataset'], board=requirements[case]['fqbn'],
            requirement=requirements[case]['requirement'], confirmed_core_pass=passed,
            source_group=repaired.get('source_group', 'workflow_267') if passed_repair else 'workflow_267',
            code_changed=repaired.get('code_changed', False) if passed_repair else False,
            source=artifact(source), original_source=artifact(original[case].get('source')),
            evaluations=[])
        folder = DEST / 'cases' / case
        save(folder / 'baseline_decision.json', old)
        if case in reviews:
            save(folder / 'requirement_review.json', reviews[case])
        if passed_repair:
            recorded = Path(repaired['result_record'])
            origin = recorded.parent
            save(folder / 'repair_decision.json', repaired)
            for name in ('changes.diff', 'repair_record.json'):
                if (origin / name).is_file():
                    shutil.copyfile(origin / name, folder / name)
            for stage, plan_name in (('after', 'frozen_test_plan.json'), ('supplement', 'supplement_plan.json')):
                result_path = origin / stage / 'result.json'
                plan_path = origin / plan_name
                if result_path.is_file() and plan_path.is_file():
                    measured = evidence(result_path, folder / stage)
                    save(folder / stage / 'plan.json', read(plan_path))
                    item['evaluations'].append(dict(stage=stage, status=measured['status'],
                        cpp=artifact(measured['cpp']), plan=f'cases/{case}/{stage}/plan.json',
                        evidence=f'cases/{case}/{stage}/recorded_result.json'))
            if (origin / 'arduino/compile.log').is_file():
                shutil.copyfile(origin / 'arduino/compile.log', folder / 'compile.log')
        elif passed and Path(old.get('evidence', '_missing_')).is_file():
            result_path = Path(old['evidence'])
            if result_path.suffix == '.md':
                shutil.copyfile(result_path, folder / 'circuit_report.md')
                item['evaluations'].append(dict(stage='baseline', status='passed', engine='recorded_circuit_observation',
                    cpp=None, plan=None, evidence=f'cases/{case}/circuit_report.md'))
                final.append(item)
                continue
            measured = evidence(result_path, folder / 'baseline')
            plan = baseline_plan(case, result_path, measured)
            plan_name = None
            if plan:
                save(folder / 'baseline/plan.json', plan)
                plan_name = f'cases/{case}/baseline/plan.json'
            item['evaluations'].append(dict(stage='baseline', status=measured['status'],
                engine=('trace_review' if measured['status'] == 'unverified_no_checks' else
                        'reviewed_assertions' if measured['status'] == 'failed' else 'host'),
                cpp=artifact(measured.get('cpp')), plan=plan_name,
                evidence=f'cases/{case}/baseline/recorded_result.json'))
        elif passed and Path(old.get('evidence', '_missing_')).is_dir():
            result_path = Path(old['evidence']) / 'result.json'
            measured = evidence(result_path, folder / 'baseline')
            engine = 'host'
            if 'preprocessed_sha256' in measured:
                engine = 'pilot10'
                plan = read(OUTPUT / f'behavior_pilot10_20261023/test_data/{case}/test_spec.json')
            else:
                plan = baseline_plan(case, result_path, measured)
            plan_name = None
            if plan:
                save(folder / 'baseline/plan.json', plan)
                plan_name = f'cases/{case}/baseline/plan.json'
            cpp = measured.get('cpp') or str(Path(source).parent.parent / 'build/sketch/sketch.ino.cpp')
            item['evaluations'].append(dict(stage='baseline', engine=engine, status=measured['status'],
                cpp=artifact(cpp), plan=plan_name, evidence=f'cases/{case}/baseline/recorded_result.json'))
        final.append(item)
        print(case, 'pass' if passed else 'unverified', flush=True)
    assert len(final) == 296 and sum(r['confirmed_core_pass'] for r in final) == 284
    save(DEST / 'final_cases.json', final)
    sources = {
        'smoke_summary.json': 'all_groups_host_smoke_20261023/summary.json',
        'smoke_results.json': 'all_groups_host_smoke_20261023/mapped_results.json',
        'behavior_summary.json': 'behavior_all296_20261023/summary.json',
        'behavior_results.json': 'behavior_all296_20261023/mapped_results.json',
        'fixed16_summary.json': 'runtime_validation_20261023/summary.json',
        'fixed16_results.json': 'runtime_validation_20261023/results.json',
        'ablation_summary.json': 'paper_ablation_v2_20261017/single_round/summary.json',
        'ablation_results.json': 'paper_ablation_v2_20261017/single_round/results.json',
        'ablation_audit.json': 'paper_ablation_v2_20261017/single_round/analysis_audit.json',
        'repair_summary.json': 'manual_code_repairs_20261106/merged_summary.json',
        'repair_results.json': 'manual_code_repairs_20261106/merged_results.json',
        'remaining_cases.json': 'manual_code_repairs_20261106/remaining_cases.json',
    }
    for name, source in sources.items():
        target = DEST / 'summary' / name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(OUTPUT / source, target)
    for name in ('Arduino.h', 'Servo.h', 'TM1637Display.h'):
        target = DEST / 'host_templates/fixture' / name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(OUTPUT / 'behavior_pilot10_20261023/fixture' / name, target)
        target = DEST / 'smoke_templates/fixture' / name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(OUTPUT / 'workflow267_host_smoke_20261023/fixture' / name, target)
    files = {str(p.relative_to(DEST).as_posix()): digest(p) for p in DEST.rglob('*') if p.is_file() and p.name != 'checksums.json'}
    save(DEST / 'package_report.json', dict(cases=296, confirmed=284, groups=len(portable_groups),
        portable_plan_cases=sum(any(e['plan'] for e in r['evaluations']) for r in final),
        files=len(files), bytes=sum(p.stat().st_size for p in DEST.rglob('*') if p.is_file())))
    files['package_report.json'] = digest(DEST / 'package_report.json')
    save(DEST / 'checksums.json', files)


if __name__ == '__main__':
    main()
