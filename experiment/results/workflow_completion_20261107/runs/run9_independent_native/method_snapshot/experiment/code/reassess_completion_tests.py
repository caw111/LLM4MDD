"""Independently rebuild native-device tests, then execute frozen workflow code.

Test authors see only the requirement and public bench, never candidate code.
Failed execution feeds the frozen counterexample back through the MDD pipeline.
"""
import argparse
import concurrent.futures
import copy
import hashlib
import json
import os
from pathlib import Path
import shutil
import time

import complete_workflow_cases as completion

ROOT=completion.ROOT


def one(source, out, model):
    source=Path(source);out=Path(out)
    old=completion.read(source/'result.json');case=completion.read(source/'input.json')
    folder=out/'cases'/case['id'];folder.mkdir(parents=True,exist_ok=True)
    if (folder/'result.json').is_file():return completion.read(folder/'result.json')
    completion.workflow.save(folder/'input.json',case)
    completion.workflow.save(folder/'frozen_candidate.json',old)
    completion.base.GXX=Path(os.environ.get('CXX') or shutil.which('g++'))
    completion.pilot.OUT=completion.BUNDLE/'host_templates'
    completion.host.OUT=folder/'host';completion.host.FIXTURE=completion.host.OUT/'fixture'
    completion.host.fixture();started=time.monotonic()
    plans=completion.public_tests(case,folder,model,reuse_saved=False)
    row=dict(old,origin='workflow_candidate_independently_reassessed',test_plans=len(plans),
        prior_generation_source=str(source.relative_to(ROOT)),attempts=[],core_status='unverified',selected_attempt=0)
    row['test_design_scope']='Fresh requirement-only tests designed after candidate generation, before reexecution; no candidate supplied.'
    attempt=source/f"attempt_{old['selected_attempt']}"
    destination=folder/'attempt_0'
    for part in ('contract.json','model.json','validation.json'):
        if (attempt/part).is_file():
            destination.mkdir(parents=True,exist_ok=True);shutil.copyfile(attempt/part,destination/part)
    for part in ('lowered','arduino/sketch'):
        if (attempt/part).is_dir():shutil.copytree(attempt/part,destination/part,dirs_exist_ok=True)
    if (attempt/'arduino/compile.log').is_file():
        shutil.copyfile(attempt/'arduino/compile.log',destination/'arduino/compile.log')
    cpp=attempt/'arduino/build/sketch/sketch.ino.cpp'
    baseline=folder/'reassessment_baseline';baseline.mkdir(parents=True,exist_ok=True)
    shutil.copyfile(cpp,baseline/'compiled_candidate.cpp')
    measured=[]
    for entry in plans:
        completion.host.case_files(case['id'],entry['plan'],case['fqbn'])
        result=completion.host.execute(cpp,case['fqbn'],case['id'],entry['plan'],baseline/entry['name'])
        measured.append(dict(name=entry['name'],**result))
    passed=bool(measured) and all(r['status']=='passed' and r.get('checks') for r in measured)
    row['attempts']=[dict(attempt=0,compilation=old['attempts'][old['selected_attempt']]['compilation'],behavior=measured)]
    row['core_status']='core_scenarios_passed' if passed else 'failed' if any(r['status']=='failed' for r in measured) else 'unverified'
    row['seconds']=round(time.monotonic()-started,2)
    if passed or not plans:
        completion.workflow.save(folder/'result.json',row);return row
    completion.workflow.save(folder/'reassessment_before_repair.json',row)
    feedback=dict(stage='independent_requirement_core_reexecution',preserve_all_acceptance=True,
        tests=[dict(plan=p['plan'],observed=[c for c in m.get('checks',[]) if not c['passed']]) for p,m in zip(plans,measured)])
    row=completion.run_case(case,str(out),model,4,initial_feedback=feedback)
    row['test_design_scope']='Requirement-only tests frozen before this candidate repair; source candidate and counterexamples retained.'
    completion.workflow.save(folder/'result.json',row)
    return row


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-runs',nargs='+',required=True)
    parser.add_argument('--out',required=True)
    parser.add_argument('--workers',type=int,default=2)
    parser.add_argument('--model',default='openai/gpt-6.1-sol')
    args=parser.parse_args();out=(ROOT/args.out).resolve()
    out.relative_to(ROOT/'experiment/output');assert 1<=args.workers<=3
    selected={}
    for run in args.source_runs:
        for path in (ROOT/'experiment/output/workflow_completion_20261107'/run/'cases').glob('*/result.json'):
            row=completion.read(path)
            if row['dataset']=='PCEval' and row['compile_ok']:selected[row['case']]=path.parent
    assert selected,'No compiled native-device candidates available'
    completion.workflow.save(out/'execution.json',dict(model=args.model,cases=sorted(selected),source_runs=args.source_runs,
        scope='Independent requirement-only test redesign and frozen-candidate reexecution',
        source_hashes={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in
            [Path(__file__),Path(completion.__file__),ROOT/'experiment/code/behavior_full_protocol.py',ROOT/'experiment/code/design_behavior_tests.py']}))
    snapshot=out/'method_snapshot'
    for p in list((ROOT/'app/services').glob('*.py'))+list((ROOT/'experiment/code').glob('*.py')):
        target=snapshot/p.relative_to(ROOT);target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,target)
    with concurrent.futures.ProcessPoolExecutor(max_workers=args.workers) as pool:
        futures=[pool.submit(one,str(p),str(out),args.model) for p in selected.values()]
        for future in concurrent.futures.as_completed(futures):
            row=future.result();print(row['case'],row['core_status'],flush=True)
            rows=[completion.read(p) for p in (out/'cases').glob('*/result.json')]
            completion.workflow.save(out/'summary.json',dict(planned=len(selected),completed=len(rows),
                compiled=sum(r['compile_ok'] for r in rows),core_passed=sum(r['core_status']=='core_scenarios_passed' for r in rows)))


if __name__=='__main__':main()
