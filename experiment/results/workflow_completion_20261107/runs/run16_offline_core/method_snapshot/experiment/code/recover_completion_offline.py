"""Finish lowering/compilation of exact cached, semantically accepted models.

No model endpoint is called. This does not bypass an absent/negative semantic
review: both cached acceptance and fresh official validation are required.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import time

import complete_workflow_cases as completion

ROOT=completion.ROOT


def one(source,out):
    case=completion.read(source/'input.json');old=completion.read(source/'result.json')
    folder=out/'cases'/case['id'];current=folder/'attempt_0'
    if (folder/'result.json').exists():return completion.read(folder/'result.json')
    model=completion.read(source/'attempt_0/model.json')
    contract=completion.read(source/'attempt_0/contract.json')
    validation=completion.read(source/'attempt_0/validation.json')
    assert validation['status']=='passed' and validation['validation']['semantic_review']['final_status']=='passed'
    folder.mkdir(parents=True,exist_ok=True);current.mkdir(parents=True,exist_ok=True)
    for name,value in [('input.json',case),('attempt_0/model.json',model),('attempt_0/contract.json',contract),
                       ('attempt_0/validation.json',validation)]:completion.workflow.save(folder/name,value)
    completion.workflow.save(folder/'recovery_provenance.json',dict(source=str(source.relative_to(ROOT)),
        model_sha256=hashlib.sha256((source/'attempt_0/model.json').read_bytes()).hexdigest(),
        contract_sha256=hashlib.sha256((source/'attempt_0/contract.json').read_bytes()).hexdigest(),
        model_requests=0,scope='Resume lowering of an exact cached semantic-pass model after tool timeout'))
    if (source/'test_plans.json').exists():
        shutil.copyfile(source/'test_plans.json',folder/'test_plans.json')
        if (source/'test_audit.json').exists():shutil.copyfile(source/'test_audit.json',folder/'test_audit.json')
    plans=completion.read(folder/'test_plans.json') if (folder/'test_plans.json').exists() else []
    completion.pilot.OUT=completion.BUNDLE/'host_templates'
    completion.host.OUT=folder/'host';completion.host.FIXTURE=completion.host.OUT/'fixture';completion.host.fixture()
    completion.base.GXX=Path(os.environ.get('CXX') or shutil.which('g++'))
    os.environ['WINDOWS_ARDUINO_CLI_PATH']=str(completion.workflow.CLI)
    row=dict(old,origin='cached_semantic_pass_workflow_resumed_offline',compile_ok=False,core_status='unverified',
             attempts=[],test_plans=len(plans),simulide_status='not_run',model_requests_this_recovery=0)
    started=time.monotonic()
    try:
        checked=completion.workflow.validate_candidate(model['system_sysml2'])
        completion.workflow.save(current/'fresh_validation.json',checked)
        assert checked['ok'],'Cached source no longer passes official/model validation'
        generated=completion.workflow.lower_verified(model['system_sysml2'],contract,checked['ir'],current/'lowered')
        code=completion.workflow.adapt_verified(generated,contract,current/'lowered')
        receipt=completion.read(current/'lowered/arduino_provenance.json')
        compiled,_=completion.workflow.compile_candidate(case,current/'arduino',code,receipt)
        detail=dict(attempt=0,compilation=compiled,behavior=[])
        row.update(compile_ok=compiled['compile_ok'],artifact=compiled['artifact'],artifact_sha256=compiled['artifact_sha256'],selected_attempt=0)
        if compiled['compile_ok']:
            for entry in plans:
                completion.host.case_files(case['id'],entry['plan'],case['fqbn'])
                result=completion.host.execute(current/'arduino/build/sketch/sketch.ino.cpp',case['fqbn'],case['id'],
                    entry['plan'],current/'behavior'/entry['name'])
                detail['behavior'].append(dict(name=entry['name'],**result))
            measured=detail['behavior']
            row['core_status']='core_scenarios_passed' if measured and all(m['status']=='passed' for m in measured) else \
                'failed' if any(m['status']=='failed' for m in measured) else 'unverified'
        row['attempts']=[detail]
    except Exception as exc:
        row.update(error_type=type(exc).__name__,error=str(exc)[:3000])
    row['seconds']=round(time.monotonic()-started,2)
    completion.workflow.save(folder/'result.json',row)
    return row


def main():
    os.environ.setdefault('JAVA_TOOL_OPTIONS','-Xmx1024m -XX:+UseSerialGC')
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,action='append',required=True)
    parser.add_argument('--out',type=Path,required=True)
    args=parser.parse_args();out=args.out.resolve();out.relative_to(ROOT/'experiment/output')
    completion.workflow.save(out/'execution.json',dict(sources=[str(p) for p in args.source],model_requests=0))
    for source in args.source:
        row=one(source.resolve(),out);print(row['case'],row['compile_ok'],row['core_status'],flush=True)
        rows=[completion.read(p) for p in (out/'cases').glob('*/result.json')]
        completion.workflow.save(out/'summary.json',dict(planned=len(args.source),completed=len(rows),
            compiled=sum(r['compile_ok'] for r in rows),core_passed=sum(r['core_status']=='core_scenarios_passed' for r in rows)))


if __name__=='__main__':main()
