"""Independent requirement-only audit of newly authored completion tests."""
import argparse
import copy
import concurrent.futures
import json
from pathlib import Path
import sys
import time

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'experiment/code'))
from run_new_workflow_pceval50 import ModelCalls, STOP
from run_current_comparison import save
from design_behavior_tests import parse, review_prompt


def read(path):return json.loads(path.read_text(encoding='utf-8-sig'))


def audit(folder, model):
    ready=folder/'test_audit.json'
    if ready.exists():return read(ready)
    if not (folder/'test_plans.json').exists():return None
    plans=read(folder/'test_plans.json')
    if not plans:return None
    case=read(folder/'input.json')
    calls=ModelCalls(folder/'test_audit_calls',model,role='independent_test_reference_audit',
                     budget=3*len(plans),reasoning_effort='medium' if 'gpt-6.1' in model else None,max_output_tokens=6000)
    results=[]
    for entry in plans:
        plan=copy.deepcopy(entry['plan'])
        if entry.get('reference_result'):
            reference=Path(entry['reference_result']).parent.with_suffix('.cpp')
            reference.resolve().relative_to(folder.resolve())
            plan['reference_cpp']=reference.read_text(encoding='utf8')
        feedback=''
        for attempt in range(3):
            _,raw=calls(review_prompt(case['effective_requirement'],plan)+
                '\nAudit the actual executable assertions. For an archived supplementary plan, old supported/coverage '
                'metadata can precede its added checks; report that inconsistency without presuming the checks valid. '
                'If reference_cpp is absent, review assertion soundness and coverage only; absence of a reference '
                'is a limitation, not itself an incorrect assertion. Candidate code is never supplied.'+feedback)
            try:
                value=parse(raw)
                assert type(value.get('approved')) is bool and value.get('coverage') in ('full','partial')
                assert isinstance(value.get('issues'),list) and isinstance(value.get('coverage_gaps'),list)
                results.append(dict(plan=entry['name'],**value));break
            except (ValueError,AssertionError,TypeError) as exc:
                feedback='\nInvalid audit response schema; return the requested JSON schema. '+str(exc)
        else:results.append(dict(plan=entry['name'],approved=False,coverage='partial',issues=['Unverified audit response schema']))
    result=dict(case=case['id'],all_plans_approved=bool(results) and all(r['approved'] for r in results),
                reviews=results,scope='Requirements, disclosed conditions, test and reference only; no candidate code supplied')
    save(ready,result)
    return result


def main():
    parser=argparse.ArgumentParser();parser.add_argument('--run',type=Path,required=True)
    parser.add_argument('--model',default='openai/gpt-6.1-sol');parser.add_argument('--once',action='store_true')
    args=parser.parse_args();run=args.run.resolve();run.relative_to(ROOT/'experiment/output')
    while True:
        with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:
            for row in pool.map(lambda folder:audit(folder,args.model),(run/'cases').glob('*')):
                if row:print(row['case'],row['all_plans_approved'],flush=True)
                if STOP.is_set():return
        audits=[read(p) for p in (run/'cases').glob('*/test_audit.json')]
        save(run/'test_audits.json',audits)
        expected=len(read(run/'execution.json')['cases'])
        completed=len(list((run/'cases').glob('*/result.json')))
        if args.once or completed==expected:return
        time.sleep(30)


if __name__=='__main__':main()
