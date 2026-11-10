"""New frozen run: bounded model/contract repair, compilation and public behavior tests.

Test references are independently drafted from requirements and never sent to the
generator. Runtime feedback contains only stimuli, assertions and observed values.
SimulIDE observations are a separate evidence layer, never inferred from host tests.
"""
import argparse
import concurrent.futures
import copy
import json
import os
import re
import random
from pathlib import Path
import shutil
import time

import run_current_comparison as workflow
from run_new_workflow_pceval50 import ModelCalls, functional_review_prompt, validate_functional_review, STOP
import design_behavior_tests as design
import behavior_full_host as host
import run_behavior_all296 as test_gate
from app.services.configuration_policy import resolve_configuration
from app.services.verified_generation import generate_verified_system
from operational_profile import acceptance_profile, VERSION as PROFILE_VERSION
from app.services.feedback_regression import check_startup

ROOT=workflow.ROOT


def schedule_cases(cases):
    groups={name:[] for name in sorted({c['dataset'] for c in cases})}
    for case in cases:groups[case['dataset']].append(case)
    rng=random.Random(42)
    for group in groups.values():rng.shuffle(group)
    return [group[index] for index in range(max(map(len,groups.values()),default=0))
            for group in groups.values() if index<len(group)]


def review_final_code(reviewer, requirement, board, code, folder):
    """Malformed reviews are infrastructure failures; execution must still run."""
    prompt = functional_review_prompt(requirement, board, code)
    for attempt in range(3):
        raw = None
        try:
            _, raw = reviewer(prompt)
            judgment = validate_functional_review(raw, requirement, code)
            workflow.save(folder/'code_review.json', judgment)
            return judgment
        except Exception as exc:
            workflow.save(folder/f'code_review_error_{attempt}.json', dict(
                error_type=type(exc).__name__, error=str(exc),
                raw=str(raw) if 'raw' in locals() else None))
            prompt = functional_review_prompt(requirement, board, code) + (
                '\nPrevious review was invalid: '+str(exc)+
                '\nReview the SAME code. Copy exact code/requirement quotes. '
                'Do not infer a pass from a formatting error or invent evidence.')
    return None


def pipeline_outcome(row):
    if row['compile_ok']:
        return 'compiled_' + row['functional_status']
    if any(r.get('compiled', {}).get('compile_ok') is False for r in row.get('rounds', [])):
        return 'compiler_failed'
    return {'ContractPlanningError':'contract_planning_failed',
            'SysMLValidationError':'model_or_semantic_review_failed'}.get(row.get('error_type'),'infrastructure_or_generation_failed')


def public_plan(case):
    drafted=design.one(dict(id=case['id'],dataset=case['dataset'],board=case['fqbn'],
                           effective_requirement=case['effective_requirement']))
    record=test_gate.one(case['id'],drafted,[dict(dataset=case['dataset'],board=case['fqbn'],
                                               compile_ok=False)],test_gate.signature())
    path=host.OUT/'cases'/case['id']/'tested_plan.json'
    if record.get('readiness')!='ready' or not path.exists():
        return None,record
    return json.loads(path.read_text(encoding='utf-8')),record


def evaluate(case,out,generation_model,review_model):
    if STOP.is_set():
        return None
    folder=out/'cases'/case['id'];folder.mkdir(parents=True,exist_ok=True)
    path=folder/'result.json'
    if path.exists():return json.loads(path.read_text(encoding='utf-8'))
    row=dict(case=case['id'],dataset=case['dataset'],compile_ok=False,functional_status='unverified',
             evaluation_partition=case.get('evaluation_partition','unspecified'),
             simulide_status='not_run',rounds=[],started=time.time())
    attempts=case.get('runtime_attempts',2)
    generation=ModelCalls(folder/'generation_calls',generation_model,role='generation_and_repair',budget=9*attempts)
    repair=ModelCalls(folder/'repair_calls',review_model,role='counterexample_repair',budget=6*attempts,reasoning_effort='medium',max_output_tokens=12000)
    reviewer=ModelCalls(folder/'semantic_review_calls',review_model,role='independent_semantic_review',budget=6*attempts,reasoning_effort='medium',max_output_tokens=12000)
    final_reviewer=ModelCalls(folder/'code_review_calls',review_model,role='independent_code_review',budget=3*attempts,reasoning_effort='medium',max_output_tokens=12000)
    try:
        try:
            plan,readiness=public_plan(case)
        except Exception as exc:
            plan=None;readiness=dict(readiness='unverified_test_infrastructure',error_type=type(exc).__name__)
        row['test_readiness']=readiness.get('readiness')
        row['test_coverage']=plan['coverage'] if plan else 'unverified'
        assumptions=plan['plan'].get('assumptions',[]) if plan else []
        requirement=case['effective_requirement']
        if assumptions:
            requirement+='\nFrozen public validation assumptions (declared before candidate generation):\n'+json.dumps(assumptions,ensure_ascii=False)
        workflow.save(folder/'public_generation_input.json',dict(requirement=requirement,assumptions=assumptions,
            configuration_resolution=case['configuration_resolution'],test_readiness=readiness))
        feedback=copy.deepcopy(case.get('repair_feedback'))
        for attempt in range(attempts):
            current=folder/f'attempt_{attempt}'
            accepted={}
            def validator(source):
                result=workflow.validate_candidate(source)
                if result['ok']:accepted.update(source=source,ir=result.get('ir'))
                return result
            def persist(revision,record):
                workflow.save(current/f'contract_revision_{revision}.json',record)
            contract,_,bundle,validation=generate_verified_system(generation,reviewer,requirement,{},
                target_board=case['fqbn'],configuration_resolution=case['configuration_resolution'],
                validator=validator,diagnostic_feedback=feedback,persist=persist,
                semantic_review_scope='model_boundary',max_model_repairs=4,repair_call=repair)
            for name,data in [('contract.json',contract),('model.json',bundle),('validation.json',validation)]:
                workflow.save(current/name,data)
            source=bundle['system_sysml2']
            if source!=accepted.get('source'):raise ValueError('Validated model identity mismatch')
            with workflow.TRANSLATOR,workflow.JAVA:
                generated=workflow.lower_verified(source,contract,accepted['ir'],current/'lowered')
            code=workflow.adapt_verified(generated,contract,current/'lowered')
            receipt=json.loads((current/'lowered/arduino_provenance.json').read_text(encoding='utf-8'))
            compiled,log=workflow.compile_candidate(case,current/'arduino',code,receipt)
            record=dict(attempt=attempt,compiled=compiled,contract_revisions=validation['contract_revisions'])
            row['rounds'].append(record)
            if not compiled['compile_ok']:
                feedback=dict(stage='arduino_compile',compiler_diagnostics=log[-12000:],
                              instruction='Locate model versus adapter failure; preserve original behavior and wiring.')
                continue
            row.update(compile_ok=True,artifact=compiled['artifact'],artifact_sha256=compiled['artifact_sha256'],
                       selected_compiled_attempt=attempt,artifact_selection='last_compiled_candidate')
            judgment=review_final_code(final_reviewer,requirement,case['fqbn'],code,current)
            record['code_review']=judgment['verdict'] if judgment else 'review_infrastructure_error'
            if plan:
                cpp=current/'arduino/build/sketch/sketch.ino.cpp'
                measured=host.execute(cpp,case['fqbn'],case['id'],plan['plan'],current/'behavior')
                record['behavior']=measured
                # Explicit startup delays override the disclosed default; their
                # exact deadline remains the independent requirement test's job.
                explicit_startup=re.search(r'warm.?up|delayed startup|(?:startup|initializ\w*|上电|初始化|预热).{0,80}(?:delay|wait|等待|延迟|预热)',case['requirement'],re.I|re.S)
                startup=check_startup(measured.get('execution'),None if explicit_startup else 500)
                record['startup_check']=startup
                if startup['status']=='failed':
                    row['functional_status']='failed'
                    feedback=dict(stage='executed_startup',failure=startup)
                    workflow.save(current/'repair_counterexample.json',feedback)
                    continue
                if measured['status']=='failed':
                    feedback=dict(stage='executed_behavior',failures=[c for c in measured['checks'] if not c['passed']][:12],
                                  public_assumptions=assumptions)
                    workflow.save(current/'repair_counterexample.json',dict(feedback,cpp_sha256=measured.get('cpp_sha256'),
                        frozen_test_plan=plan['plan'],acceptance_changed=False))
                    row['functional_status']='failed'
                    continue
                row['functional_status']=('passed_public_scenarios' if plan['coverage']=='full' else
                    'partial_scenarios_passed') if measured['status']=='passed' else measured['status']
                if startup['status']=='unverified' and row['functional_status'] in ('passed_public_scenarios','partial_scenarios_passed'):
                    row['functional_status']='unverified_startup'
            else:
                row['functional_status']='unverified_no_executable_tests'
            row['executed_functional_status']=row['functional_status']
            if judgment is None or judgment['verdict']=='uncertain':
                row['functional_status']='unverified_code_review'
                break
            if judgment['verdict']=='judged_fail':
                feedback=dict(stage='independent_code_review',defects=judgment['defects'])
                row['functional_status']='static_defect_identified'
                continue
            break
        row['status']='completed'
    except Exception as exc:
        row.update(status='generation_or_validation_failed',error_type=type(exc).__name__,error=str(exc)[:1000])
        if hasattr(exc,'attempts'):workflow.save(folder/'failure_attempts.json',exc.attempts)
        if hasattr(exc,'generation_rounds'):workflow.save(folder/'failure_generation_rounds.json',exc.generation_rounds)
        if STOP.is_set():
            row['infrastructure_interrupted']=True
    row.update(seconds=round(time.time()-row['started'],2),generation_usage=workflow.metrics(generation.entries),
               repair_usage=workflow.metrics(repair.entries),repair_model=review_model,
               semantic_review_usage=workflow.metrics(reviewer.entries),code_review_usage=workflow.metrics(final_reviewer.entries))
    row['pipeline_outcome']=pipeline_outcome(row)
    workflow.save(path,row)
    return row


def report(out,cases):
    import collections
    ids={c['id'] for c in cases}
    all_rows=[json.loads(p.read_text(encoding='utf-8')) for p in (out/'cases').glob('*/result.json')]
    all_rows=[r for r in all_rows if r['case'] in ids]
    rows=[r for r in all_rows if not r.get('infrastructure_interrupted')]
    summary=dict(planned=len(cases),completed=len(rows),pending=len(cases)-len(rows),datasets={},
        infrastructure_interrupted=sum(bool(r.get('infrastructure_interrupted')) for r in all_rows),
        pipeline_outcomes=dict(collections.Counter(pipeline_outcome(r) for r in rows)))
    compiled=sum(r['compile_ok'] for r in rows)
    verified=sum(r['compile_ok'] and r['functional_status']=='passed_public_scenarios' for r in rows)
    n=len(cases)
    summary['acceptance_gate']=dict(denominator=n,compiled=compiled,runtime_verified=verified,
        compile_rate=compiled/n if n else 0,runtime_rate=verified/n if n else 0,
        partial_scenarios_passed=sum(r['functional_status']=='partial_scenarios_passed' for r in rows),
        threshold='both rates strictly greater than 0.90',
        state='collecting' if summary['pending'] else 'passed' if n and compiled/n>0.9 and verified/n>0.9 else 'repair_required',
        scope='Host execution with full declared scenario coverage; not hardware/SimulIDE proof')
    summary['partitions']={}
    for partition in sorted({c.get('evaluation_partition','unspecified') for c in cases}):
        rr=[r for r in rows if r.get('evaluation_partition','unspecified')==partition]
        summary['partitions'][partition]=dict(planned=sum(c.get('evaluation_partition','unspecified')==partition for c in cases),
            completed=len(rr),compiled=sum(r['compile_ok'] for r in rr),
            functional=dict(collections.Counter(r['functional_status'] for r in rr)))
    for ds in ['internal','PCEval','EmbedBench']:
        rr=[r for r in rows if r['dataset']==ds];total=sum(c['dataset']==ds for c in cases)
        summary['datasets'][ds]=dict(planned=total,completed=len(rr),compiled=sum(r['compile_ok'] for r in rr),
            pipeline_outcomes=dict(collections.Counter(pipeline_outcome(r) for r in rr)),
            functional=dict(collections.Counter(r['functional_status'] for r in rr)),
            simulide=dict(collections.Counter(r['simulide_status'] for r in rr)))
    workflow.save(out/'results.json',rows);workflow.save(out/'summary.json',summary)
    return summary


def main():
    parser=argparse.ArgumentParser();parser.add_argument('--out',required=True);parser.add_argument('--case',action='append')
    parser.add_argument('--workers',type=int,default=4);parser.add_argument('--generation-model',default='openai/gpt-5.4-nano')
    parser.add_argument('--review-model',default='openai/gpt-6.1-sol')
    parser.add_argument('--repair-ledger',type=Path)
    parser.add_argument('--runtime-attempts',type=int,choices=range(1,5),default=2)
    args=parser.parse_args()
    out=(ROOT/args.out).resolve();out.relative_to((ROOT/'experiment/output').resolve());out.mkdir(parents=True,exist_ok=True)
    cases=json.loads((ROOT/'experiment/formal/manifest.json').read_text(encoding='utf-8'))['cases']
    failures={}
    if args.repair_ledger:
        ledger=json.loads(args.repair_ledger.read_text(encoding='utf-8-sig'))
        if len({r['case'] for r in ledger})!=len(ledger):
            parser.error('Repair ledger contains duplicate cases')
        failures={r['case']:r for r in ledger if r['core_status'] in ('core_fail','compile_failed')}
        if not set(failures)<={c['id'] for c in cases}:
            parser.error('Repair ledger contains unknown cases')
    development=set(p.parent.name for p in (ROOT/'experiment/output/semantic_repair_validation_20261030/revision7/cases').glob('*/result.json'))
    for c in cases:
        requirement=c['requirement']+('\n'+workflow.INTERPRETATIONS[c['id']] if c['id'] in workflow.INTERPRETATIONS else '')
        resolution=resolve_configuration(requirement,{},board=c['fqbn'])
        profile=acceptance_profile(c['requirement'])
        c.update(configuration_resolution=resolution,operational_profile=profile,
                 evaluation_partition='development_regression' if c['id'] in development else 'not_in_13_case_regression',
                 effective_requirement=resolution['effective_requirement']+profile)
        c['runtime_attempts']=args.runtime_attempts
        if c['id'] in failures:
            previous=failures[c['id']]
            c['repair_feedback']=dict(stage='previous_frozen_artifact_diagnosis',
                previous_status=previous['core_status'],diagnosis=previous.get('failure_reason'),
                instruction='Repair the contract or SysML root cause and regenerate through the production compiler. '
                            'Validate the original requirement independently. Previous diagnoses may be incomplete.')
    selected=[c for c in cases if (not args.case or c['id'] in args.case) and
              (not args.repair_ledger or c['id'] in failures)]
    if args.case and len(selected)!=len(set(args.case)):parser.error('Unknown/duplicate case')
    selected=schedule_cases(selected)
    files=list((ROOT/'app/services').glob('*.py'))+list((ROOT/'tools/prompts').glob('*.md'))+list((ROOT/'experiment/code').glob('*.py'))
    config=dict(cases=cases,generation_model=args.generation_model,review_model=args.review_model,
                repair_model=args.review_model,repair_call_budget=6*args.runtime_attempts,
                max_generation_output_tokens=6000,max_repair_and_review_output_tokens=12000,
                max_test_checks=design.MAX_CHECKS,
                scheduled_case_ids=[c['id'] for c in selected],workers=args.workers,
                scheduling_seed=42,development_case_ids=sorted(development),
                operational_profile_version=PROFILE_VERSION,
                max_runtime_repairs=args.runtime_attempts-1,max_contract_revisions=1,max_model_repairs=4,
                repair_ledger_sha256=workflow.digest(args.repair_ledger) if args.repair_ledger else None,
                hashes={str(p.relative_to(ROOT)):workflow.digest(p) for p in files},
                test_policy='Requirement-only independent design plus reference and mutation preflight; public assumptions frozen before generation')
    path=out/'execution.json'
    if path.exists():
        if json.loads(path.read_text(encoding='utf-8'))!=config:raise SystemExit('Frozen method changed; choose a new output directory')
    else:
        workflow.save(path,config)
        for file in files:
            target=out/'method_snapshot'/file.relative_to(ROOT);target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(file,target)
    design.OUT=host.OUT=test_gate.OUT=out/'validation';host.FIXTURE=host.OUT/'fixture';host.fixture()
    os.environ['PATH']=str(workflow.CLI.parent)+os.pathsep+os.environ['PATH']
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.workers) as pool:
        pending=iter(selected)
        futures={}
        def submit_next():
            if STOP.is_set():return
            case=next(pending,None)
            if case is not None:futures[pool.submit(evaluate,case,out,args.generation_model,args.review_model)]=case['id']
        for _ in range(args.workers):submit_next()
        while futures:
            done,_=concurrent.futures.wait(futures,return_when=concurrent.futures.FIRST_COMPLETED)
            for future in done:
                futures.pop(future)
                row=future.result()
                if row:print(row['case'],row['compile_ok'],row['functional_status'],flush=True)
                submit_next()
            report(out,selected)
        if STOP.is_set():
            workflow.save(out/'run_interruption.json',dict(status='interrupted_infrastructure',
                action='Stopped dispatch; unscheduled cases remain pending, never marked failed.'))
    print(json.dumps(report(out,selected),ensure_ascii=False),flush=True)


if __name__=='__main__':main()
