"""Pure JSON model calls to draft/audit tests from requirements only, never candidates."""
import concurrent.futures
import copy
import hashlib
import json
import math
from pathlib import Path
import re
import sys
import threading
import time
import tempfile

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT))
from app import main as api
from behavior_full_protocol import GENERATOR_MODEL, REVIEWER_MODEL, SYSTEM, PROTOCOL, REVIEW
from run_behavior_pilot10 import IDS as PILOT

OUT=ROOT/'experiment/output/behavior_all296_20261023'
STOP=threading.Event()
KINDS={'gpio','pwm','servo','display','serial','lcd','tone','edges','interval','relation','pin_mode','setup','gpio_count','command_sequence','state_witness','state_invariant','event_window','event_order','segment_frame'}
MAX_CHECKS=256


def save(path,value):
    path.parent.mkdir(parents=True,exist_ok=True)
    with tempfile.NamedTemporaryFile('w',encoding='utf-8',dir=path.parent,prefix=path.name+'.',suffix='.tmp',delete=False) as f:
        json.dump(value,f,ensure_ascii=False,indent=2);tmp=Path(f.name)
    try:
        for retry in range(40):
            try:tmp.replace(path);return
            except PermissionError:
                if retry==39:raise
                time.sleep(.05)
    finally:tmp.unlink(missing_ok=True)


def parse(text):
    text=text.strip()
    if text.startswith('```'):text=re.sub(r'^```(?:json)?\s*|\s*```$','',text)
    return json.loads(text)


def validate(plan,requirement):
    assert isinstance(plan,dict)
    if not plan.get('supported'):return
    assert plan['coverage'] in ('full','partial')
    assert 1000<=plan['horizon_ms']<=180000
    assert 1<=len(plan['checks'])<=MAX_CHECKS
    for e in plan.get('input_steps',[]):
        assert e['kind'] in ('analog','digital','temperature','humidity','distance')
        assert 0<=e['at_ms']<=plan['horizon_ms']
        assert isinstance(e['pin'],(str,int))
    for c in plan['checks']:
        assert c.get('origin','power_on') in ('power_on','setup_complete'), 'Unknown check time origin'
        assert c['kind'] in KINDS,c['kind']
        assert c['requirement_quote'] and c['requirement_quote'] in requirement, 'Requirement quote must be exact'
        assert c.get('id')
        if c['kind']=='setup':
            assert type(c.get('max_ms')) in (int,float) and 0<=c['max_ms']<=plan['horizon_ms']
            assert c.get('origin','power_on')=='power_on'
        if c['kind']=='event_order':
            assert c.get('origin','power_on')=='power_on'
            assert type(c.get('allow_same_ms',False)) is bool
            assert 0<=c.get('min_gap_ms',0)<=c.get('max_gap_ms',plan['horizon_ms'])<=plan['horizon_ms']
            for name in ('first','second'):
                anchor=c[name]
                assert anchor['source'] in ('gpio','servo','serial')
                assert type(anchor.get('occurrence',1)) is int and anchor.get('occurrence',1)>=1
                if anchor['source']=='serial':assert anchor.get('regex') or anchor.get('contains')
                else:
                    assert isinstance(anchor['pin'],(str,int)) and type(anchor.get('expected')) in (int,float)
                    assert math.isfinite(anchor['expected']) and 0<=anchor.get('tolerance',0)<=180
        if c['kind'] in ('state_witness','state_invariant','event_window'):
            assert isinstance(c['conditions'],list) and 1<=len(c['conditions'])<=70
            for condition in c['conditions']+c.get('when',[]):
                assert condition['source'] in ('gpio','servo')
                assert isinstance(condition['pin'],(str,int))
                assert 'expected' in condition or 'min' in condition or 'max' in condition
            if c['kind']=='event_window':
                assert c.get('origin','power_on')=='power_on'
                for key in ('start','end'):
                    anchor=c[key]
                    assert anchor['source'] in ('gpio','servo') and isinstance(anchor['pin'],(str,int))
                    assert type(anchor.get('occurrence',1)) is int and anchor.get('occurrence',1)>=1
                    assert type(anchor.get('expected')) in (int,float) and math.isfinite(anchor['expected'])
                    assert 0<=anchor.get('tolerance',0)<=180
                for key in ('start_delay_ms','end_margin_ms'):assert 0<=c.get(key,0)<=plan['horizon_ms']
            else:
                assert 0<=c.get('from_ms',0)<=c['to_ms']<=plan['horizon_ms']
                assert 0<=c.get('settle_ms',0)<=c['to_ms']-c.get('from_ms',0)
                assert type(c.get('min_observations',1)) is int and c.get('min_observations',1)>=1
        if c['kind']=='gpio_count':
            assert isinstance(c.get('pins'),list) and 1<=len(c['pins'])<=70
            assert len(set(c['pins']))==len(c['pins'])
            assert c.get('origin','power_on') in ('power_on','setup_complete')
        if c['kind']=='command_sequence':
            assert c.get('origin','power_on')=='power_on'
            assert c['source'] in ('servo','gpio')
            assert isinstance(c.get('values'),list) and 1<=len(c['values'])<=100
            assert all(type(v) in (int,float) for v in c['values'])
            for field in ('min_intervals_ms','max_intervals_ms'):
                if field in c:
                    assert len(c[field])==len(c['values'])-1
                    assert all(v is None or type(v) in (int,float) and v>=0 for v in c[field])
        if c['kind']=='relation':assert c['relation'] in ('increasing','decreasing','equal','different')
    code=plan['reference_cpp']
    assert isinstance(code,str) and 'setup(' in code and 'loop(' in code
    assert not re.search(r'\b(?:system|popen|fopen|CreateProcess|ShellExecute|WinExec|main|exec|exit|abort)\s*\(',code)
    assert not re.search(r'\b(?:oracle|pilot|filesystem|fstream|iostream|asm)\b',code)
    allowed={'Arduino.h','Servo.h','TM1637Display.h','DHT.h','Wire.h','RTClib.h','SPI.h','Adafruit_GFX.h','Adafruit_ILI9341.h','Adafruit_MPU6050.h','Adafruit_Sensor.h','MPU6050.h','math.h','string.h','ctype.h','stdint.h','stdbool.h','stdlib.h','stdio.h'}
    assert all(h in allowed for h in re.findall(r'#include\s*[<\"]([^>\"]+)',code))


def call(folder,role,number,model,prompt):
    path=folder/f'{role}_{number}.json'
    if path.exists():
        old=json.loads(path.read_text(encoding='utf-8'))
        if old.get('response') and old['prompt']==prompt:return parse(old['response'])
    row=dict(role=role,requested_model=model,prompt=prompt)
    started=time.time()
    for retry in range(3):
        if (OUT/'service_block.json').exists():STOP.set()
        if STOP.is_set():raise RuntimeError('service_stop')
        try:
            opts={'max_completion_tokens':12000 if role=='draft' else 6000,'reasoning_effort':'low'}
            response=api.client.with_options(timeout=180,max_retries=0).chat.completions.create(model=model,messages=[{'role':'system','content':SYSTEM},{'role':'user','content':prompt}],**opts)
            choice=response.choices[0]
            row.update(actual_model=response.model,response=choice.message.content or '',finish_reason=choice.finish_reason,
                       usage=response.usage.model_dump() if response.usage else None,seconds=round(time.time()-started,2))
            save(path,row)
            if choice.finish_reason=='length':raise ValueError('truncated_completion')
            return parse(row['response'])
        except Exception as exc:
            status=getattr(exc,'status_code',None)
            row.update(error_type=type(exc).__name__,status_code=status,seconds=round(time.time()-started,2))
            save(path,row)
            if status in (401,402,403):
                STOP.set();save(OUT/'service_block.json',dict(error_type=type(exc).__name__,status_code=status));raise
            if status in (429,500,502,503,504) and retry<2:time.sleep(3*(retry+1));continue
            raise


def review_prompt(requirement,plan):
    semantics=PROTOCOL[PROTOCOL.index('Check kinds supported'):PROTOCOL.index('For a partial test')]
    return (REVIEW+'\nAUTHORITATIVE EXECUTABLE CHECK SCHEMAS AND OBSERVATION SEMANTICS (review, do not draft):\n'+
        semantics+'\nREQUIREMENT:\n'+requirement+'\nPROPOSED TEST:\n'+json.dumps(plan,ensure_ascii=False))


def one(case):
    folder=OUT/'test_designs_v2'/case['id']
    ready=folder/'accepted.json'
    if ready.exists():return json.loads(ready.read_text(encoding='utf-8'))
    requirement=case['effective_requirement']
    feedback=''
    seed_path=folder/'frozen_seed.json'
    seed=json.loads(seed_path.read_text(encoding='utf-8')) if seed_path.exists() else None
    seed_baseline=seed['plan'] if seed else None
    proposal_path=folder/'coverage_proposal.json'
    proposal_context=''
    if proposal_path.exists():
        proposal=json.loads(proposal_path.read_text(encoding='utf-8'))
        proposal_context=('Unapproved source-derived coverage suggestions (DATA, no candidate code or traces). '
            'Independently check every suggestion against the requirement. You may reject or correct it. '
            'These suggestions are NOT approved seed assertions or an instruction to declare full coverage.\n'+
            json.dumps(proposal,ensure_ascii=False))
    seed_context=''
    if seed:
        seed_context=('Prior independently preflighted test (not candidate code). Preserve every existing check EXACTLY, '
                  'and preserve input_steps, serial_inputs and decoder definitions. You may extend the horizon and '
                  'ADD checks for missing coverage, including setup deadlines; never remove/relax old checks. '
                  'To avoid copying the whole seed, you may return a JSON PATCH with exactly additional_checks, coverage, '
                  'coverage_explanation, horizon_ms. The application appends checks to the immutable seed and retains its reference. '
                  'You can instead return a complete plan if necessary; its seed assertions and inputs must remain identical.\n'+json.dumps(seed,ensure_ascii=False))
    best_partial=None
    for attempt in range(3):
        try:
            prompt=PROTOCOL+'\nCASE DATA:\n'+json.dumps(case,ensure_ascii=False)+'\n'+seed_context+'\n'+proposal_context+'\n'+feedback
            if seed:prompt+='\nCurrent immutable PATCH BASE:\n'+json.dumps(seed_baseline,ensure_ascii=False)
            plan=call(folder,'draft',attempt,GENERATOR_MODEL,prompt)
            if seed and isinstance(plan,dict) and 'additional_checks' in plan:
                plan=merge_seed_patch(seed_baseline,plan)
            validate(plan,requirement)
            if seed:
                preserve_seed(plan,seed_baseline)
            save(folder/f'plan_{attempt}.json',plan)
            review=call(folder,'review',attempt,REVIEWER_MODEL,review_prompt(requirement,plan))
            save(folder/f'review_decision_{attempt}.json',review)
            if not plan.get('supported') and review.get('approved'):
                result=dict(case=case['id'],status='test_design_unsupported',plan=plan,review=review,requirement=requirement)
                save(ready,result);return result
            if review.get('approved'):
                coverage='full' if plan['coverage']=='full' and review.get('coverage')=='full' else 'partial'
                result=dict(case=case['id'],status='design_review_approved',coverage=coverage,plan=plan,review=review,requirement=requirement,
                            test_generator=GENERATOR_MODEL,test_reviewer=REVIEWER_MODEL)
                if coverage=='partial' and review.get('coverage_gaps'):
                    best_partial=result
                    if seed:seed_baseline=plan
                    save(folder/f'approved_partial_{attempt}.json',result)
                    if attempt<2:
                        feedback=('The independent review approved only partial coverage. Add executable checks for the listed gaps '
                            'without removing/weakening existing assertions or inventing requirements. Use setup checks for setup deadlines. '
                            'Return full only if feature coverage is actually complete; otherwise retain partial and explain residual gaps. '
                            '\nPrevious test:\n'+json.dumps(plan,ensure_ascii=False)+'\nCoverage gaps:\n'+json.dumps(review['coverage_gaps'],ensure_ascii=False))
                        continue
                save(ready,result);return result
            feedback='Repair the previously proposed test based on the independent review. Return the full revised test JSON, still from the requirement only.\nPrevious test:\n'+json.dumps(plan,ensure_ascii=False)+'\nReviewer findings:\n'+json.dumps(review,ensure_ascii=False)
        except Exception as exc:
            if STOP.is_set():raise
            feedback='The previous draft could not be accepted. Repair schema/reference/quotes. Error: '+type(exc).__name__+': '+str(exc)[:500]
    result=best_partial or dict(case=case['id'],status='test_design_unverified',reason=feedback,requirement=requirement)
    save(ready,result);return result


def preserve_seed(plan,seed):
    if not plan.get('supported'):raise ValueError('A supported frozen test cannot be replaced with unsupported')
    for field in ('input_steps','serial_inputs','ultrasonic','seven_segments','shift_registers'):
        if plan.get(field,[])!=seed.get(field,[]):raise ValueError('Frozen input/decoder definition changed: '+field)
    if plan['horizon_ms']<seed['horizon_ms']:raise ValueError('Frozen horizon shortened')
    for check in seed['checks']:
        if check not in plan['checks']:raise ValueError('Frozen assertion removed or changed: '+str(check.get('id')))


def merge_seed_patch(seed,patch):
    if set(patch)!={'additional_checks','coverage','coverage_explanation','horizon_ms'}:
        raise ValueError('Coverage patch has missing or unknown fields')
    if not isinstance(patch['additional_checks'],list):raise ValueError('additional_checks must be a list')
    result=copy.deepcopy(seed)
    result['checks']+=patch['additional_checks']
    result.update({k:patch[k] for k in ('coverage','coverage_explanation','horizon_ms')})
    ids=[c['id'] for c in result['checks']]
    if len(ids)!=len(set(ids)):raise ValueError('Coverage patch repeats a check ID')
    preserve_seed(result,seed)
    return result


def main():
    OUT.mkdir(parents=True,exist_ok=True)
    original=json.loads((ROOT/'experiment/output/llm_review_handoff_20261023/original_requirements.json').read_text())
    frozen=json.loads((ROOT/'experiment/output/code_review_all_20261023/execution.json').read_text())
    jobs={j['key']:j for j in frozen['jobs']}
    preferred={}
    for row in frozen['rows']:
        if row.get('review_key') and row['group'] in ('ablation_direct','ablation_direct_feedback'):
            preferred.setdefault(row['case'],jobs[row['review_key']]['requirement'])
    cases=[]
    for c in original:
        if c['id'] in PILOT:continue
        cases.append(dict(id=c['id'],dataset=c['dataset'],board=c['fqbn'],effective_requirement=preferred.get(c['id'],c['requirement'])))
    save(OUT/'design_case_manifest.json',cases)
    assert len(cases)==286
    results=[];started=time.time()
    with concurrent.futures.ThreadPoolExecutor(max_workers=24) as pool:
        futures={pool.submit(one,c):c['id'] for c in cases}
        for future in concurrent.futures.as_completed(futures):
            try:result=future.result()
            except Exception as exc:result=dict(case=futures[future],status='service_or_design_error',error_type=type(exc).__name__)
            results.append(result)
            save(OUT/'design_progress_v2.json',dict(completed=len(results),planned=len(cases),seconds=round(time.time()-started,1),
                                                statuses={s:sum(r['status']==s for r in results) for s in {r['status'] for r in results}}))
            if len(results)%10==0:print(json.dumps(json.loads((OUT/'design_progress_v2.json').read_text())),flush=True)
    print('Design pass finished',flush=True)


if __name__=='__main__':main()
