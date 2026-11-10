"""Package targeted workflow reruns without changing the historical evidence."""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import shutil

ROOT = Path(__file__).resolve().parents[2]
BASE = ROOT / 'experiment/results'
RUNS = ROOT / 'experiment/output/workflow_completion_20261107'


def read(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))


def write(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2)+'\n', encoding='utf8')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runs', nargs='+', required=True)
    parser.add_argument('--out', default='experiment/results/workflow_completion_20261107')
    args = parser.parse_args()
    out = (ROOT / args.out).resolve()
    assert out.parent == BASE and out.name.startswith('workflow_completion_')
    historical = read(BASE/'final_cases.json')
    missing = {r['case'] for r in read(BASE/'groups.json') if r['group']=='workflow_280' and not r['compile_ok']}
    remaining = {r['case'] for r in read(BASE/'summary/remaining_cases.json')}
    targets = missing | remaining
    candidates = {}
    all_rows = []
    for order, run in enumerate(args.runs):
        source = RUNS/run
        assert source.parent == RUNS and (source/'execution.json').is_file()
        for path in sorted((source/'cases').glob('*/result.json')):
            row = read(path)
            if row['case'] not in targets:
                continue
            row = dict(row, run=run, evidence_source=str(path.relative_to(ROOT)), run_order=order)
            audit_file=path.parent/'test_audit.json'
            audit=read(audit_file) if audit_file.is_file() else {}
            row['test_review_source']=audit.get('review_source','independent_model' if audit.get('all_plans_approved') else 'legacy_regression')
            row['test_review_approved']=audit.get('all_plans_approved')
            if row['case'] in remaining and row['core_status']=='core_scenarios_passed' and not audit.get('all_plans_approved'):
                row['development_reported_core_status']=row['core_status']
                row['core_status']='unverified_independent_test_audit'
            all_rows.append(row)
            score = (row['core_status']=='core_scenarios_passed', row['compile_ok'], order)
            previous = candidates.get(row['case'])
            if previous is None or score > previous[0]:
                candidates[row['case']] = (score, row, path.parent)
    assert set(candidates)==targets, 'Incomplete targeted run: '+str(sorted(targets-set(candidates)))
    rows = []
    for case in sorted(targets):
        _, row, folder = candidates[case]
        destination = out/'cases'/case
        for path in folder.rglob('*'):
            if not path.is_file() or path.suffix not in ('.json','.cpp','.ino','.h','.hpp','.sysml','.c','.il','.txt','.log'):
                continue
            relative = path.relative_to(folder)
            if 'fixture' in relative.parts or 'build' in relative.parts:
                continue
            target = destination/relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(path, target)
        # The host executes Arduino's expanded C++ file; retain that exact input too.
        for attempt in row['attempts']:
            build=folder/f"attempt_{attempt['attempt']}"/'arduino/build'
            if not build.is_dir() and row.get('artifact'):
                build=(ROOT/row['artifact']).parent.parent/'build'
            for firmware in build.glob('*.hex'):
                target=destination/f"attempt_{attempt['attempt']}"/'firmware'/firmware.name
                target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(firmware,target)
            for behavior in attempt.get('behavior', []):
                source = Path(behavior['cpp'])
                if source.is_file():
                    expected = behavior.get('cpp_sha256')
                    assert not expected or hashlib.sha256(source.read_bytes()).hexdigest()==expected
                    target = destination/f"attempt_{attempt['attempt']}"/'compiled_candidate.cpp'
                    target.parent.mkdir(parents=True,exist_ok=True)
                    shutil.copyfile(source,target)
        for path in folder.rglob('trace.jsonl'):
            target = destination/path.relative_to(folder)
            target = target.with_suffix('.jsonl.gz')
            target.parent.mkdir(parents=True, exist_ok=True)
            with gzip.GzipFile(filename=str(target), mode='wb',mtime=0) as stream:
                stream.write(path.read_bytes())
        if row.get('artifact'):
            source = ROOT/row['artifact']
            assert source.is_file() and hashlib.sha256(source.read_bytes()).hexdigest()==row['artifact_sha256']
        plans = read(folder/'test_plans.json') if (folder/'test_plans.json').is_file() else []
        row['test_coverage'] = [dict(name=p['name'], coverage=p['plan'].get('coverage','not_declared'),
                                   review_required=p.get('review_required',False)) for p in plans]
        row['historically_ungenerated'] = case in missing
        row['previously_unconfirmed'] = case in remaining
        row['portable_evidence'] = str(destination.relative_to(ROOT))
        rows.append(row)
    for run in args.runs:
        source=RUNS/run
        (out/'runs'/run).mkdir(parents=True,exist_ok=True)
        if (source/'method_snapshot').is_dir():
            shutil.copytree(source/'method_snapshot',out/'runs'/run/'method_snapshot',dirs_exist_ok=True)
        for name in ('execution.json','summary.json','interruption.json'):
            if (source/name).exists():
                shutil.copyfile(source/name,out/'runs'/run/name)
    for name in ('final_regression.stderr.log','verified_native.stderr.log','wiring_final.stderr.log','semantic_final.stderr.log'):
        if (RUNS/name).exists():
            target=out/'verification'/name;target.parent.mkdir(parents=True,exist_ok=True)
            shutil.copyfile(RUNS/name,target)
    for source in (RUNS/'original_integrity_verification.json',
                   ROOT/'experiment/replay-output/workflow_completion_20261107/replay_summary.json'):
        if source.is_file():
            target=out/'verification'/source.name;target.parent.mkdir(parents=True,exist_ok=True)
            shutil.copyfile(source,target)
    passed = {r['case'] for r in rows if r['core_status']=='core_scenarios_passed'}
    old_passed = {r['case'] for r in historical if r['confirmed_core_pass']}
    dataset=[]
    for name in ('internal','PCEval','EmbedBench'):
        old = [r for r in historical if r['dataset']==name]
        selected = [r for r in rows if r['dataset']==name]
        dataset.append(dict(dataset=name,total=len(old),targeted=len(selected),
            targeted_compile=sum(r['compile_ok'] for r in selected),targeted_core=sum(r['case'] in passed for r in selected),
            historical_core=sum(r['confirmed_core_pass'] for r in old),
            retained_delivery_core=sum(r['case'] in old_passed|passed for r in old)))
    manual_models={r['case'] for r in rows if r.get('model_origin',r['origin'])=='offline_manually_authored_sysml'}
    compiled={r['case'] for r in rows if r['compile_ok']}
    historical_workflow=[r for r in read(BASE/'groups.json') if r['group']=='workflow_280']
    history_compiled=sum(r['compile_ok'] for r in historical_workflow)
    compilation_sources=[]
    selected_by_id={r['case']:r for r in rows}
    for original in historical_workflow:
        if original['compile_ok']:
            source=BASE/original['source'];origin='historical_workflow_280'
        else:
            r=selected_by_id[original['case']]
            source=out/'cases'/r['case']/f"attempt_{r['selected_attempt']}"/'arduino/sketch/sketch.ino'
            origin='offline_manual_sysml' if r['case'] in manual_models else 'targeted_workflow_regeneration'
        assert source.is_file(),str(source)
        compilation_sources.append(dict(case=original['case'],dataset=original['dataset'],origin=origin,
            source=str(source.relative_to(ROOT)),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    write(out/'all296_compilation_sources.json',compilation_sources)
    summary=dict(scope='Targeted development repair, not an independent all296 rerun',targeted=len(rows),
        target_compile=sum(r['compile_ok'] for r in rows),target_core=len(passed),
        no_new_candidate=sum(not r['compile_ok'] and not r.get('artifact') for r in rows),
        actual_target_compile_failures=sum(not a['compilation']['compile_ok'] for r in rows for a in r.get('attempts',[]) if 'compilation' in a),
        missing16_compiled=len(compiled&missing),missing16_manual_models=len(manual_models&missing),
        missing16_automatic_workflow_compiled=len((compiled&missing)-manual_models),
        cumulative_workflow_compile=history_compiled+len((compiled&missing)-manual_models),
        cumulative_compile_including_manual_model=history_compiled+len(compiled&missing),
        positive_cases_by_test_review={tier:sum(r['case'] in passed and r['test_review_source']==tier for r in rows)
            for tier in ('independent_model','offline_manual','legacy_regression')},
        workflow_sources_replacing_direct_archive=len((passed&missing)-manual_models),
        mdd_sources_replacing_direct_archive=len(passed&missing),previously_ungenerated=len(missing),
        newly_confirmed=len(passed-old_passed),historical_confirmed=len(old_passed),
        retained_delivery_core=len(old_passed|passed),total=296,datasets=dataset,
        assessment='Host API execution; partial plans and historical human-reviewed evidence remain separately labeled.',
        simulide='Not performed in this development run; no new circuit-simulation claim.',
        original_evidence='The original experiment/results manifest remains unchanged. This subtree has its own checksums.')
    write(out/'cases.json',rows);write(out/'all_development_attempts.json',all_rows);write(out/'summary.json',summary)
    write(out/'remaining.json',[r for r in rows if r['case'] not in passed])
    checksums={str(p.relative_to(out)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest()
               for p in sorted(out.rglob('*')) if p.is_file() and p.name!='checksums.json'}
    write(out/'checksums.json',checksums)
    print(json.dumps(summary,ensure_ascii=False,indent=2))


if __name__=='__main__':
    main()
