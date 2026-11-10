"""Regenerate diagnosed workflow artifacts from repaired model/contract pairs offline."""
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
import json

import behavior_full_host as host
import run_current_comparison as workflow
from app.services.interface_registry import register_contract
from app.services.interface_semantics import freeze_model_semantics

ROOT = workflow.ROOT
BASE = ROOT / 'experiment/output'
OUT = BASE / 'core_repair_offline_round1'
PREVIOUS = BASE / 'core_reassessment_20261106_pending217'
ARCHIVE = BASE / 'configured_audit_v1_20261015'
CASES = ('embedbench_38', 'embedbench_41', 'embedbench_102',
         'simple_020', 'pceval_led_RGB_basic', 'medium_003')


def read(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))


def sample(plan, kind, time, key, expected, quote, tolerance=None):
    check = dict(id=f'{kind}_{key}_{time}', kind=kind, at_ms=time, expected=expected,
                 requirement_quote=quote)
    check['name' if kind=='display' else 'pin'] = key
    if tolerance is not None:
        check['tolerance'] = tolerance
    plan['checks'].append(check)


def prepare(case, contract, source):
    if case.startswith('embedbench'):
        binding = next(p['binding'] for p in contract['interfaces'] if p['binding']['kind']=='shift595_number')
        digits = {'sevseg1': 0, 'sevseg2': 1}
        for register in binding['registers']:
            register['digit'] = digits[register['display']]
    elif case=='simple_020':
        next(p['binding'] for p in contract['interfaces'] if p['binding']['kind']=='tm1637_number')['leading_zero']=True
    elif case=='pceval_led_RGB_basic':
        for port in contract['interfaces']:
            port['binding']['active_high']=False
    else:
        old='(if ((io_0) > (30.0)) ? 1.0 else 0.0)'
        assert source.count(old)==1
        source=source.replace(old,'(if ((io_2 * 500.0 / 1023.0) > 30.0) ? 255.0 else 0.0)')
    for port in contract['interfaces']:
        if 'hardware' in port:
            port['hardware']=json.dumps(port['binding'],ensure_ascii=False,sort_keys=True)
        port.pop('model_semantics', None)
    contract.pop('interface_registry',None)
    return freeze_model_semantics(register_contract(contract)),source


def test_plan(case):
    if case=='pceval_led_RGB_basic':
        return read(PREVIOUS/'additional_plans'/f'{case}.json')
    if case=='medium_003':
        plan=dict(horizon_ms=7000, checks=[], input_steps=[
            dict(at_ms=t,kind='analog',pin=pin,value=value)
            for t,pin,value in [(0,'A0',1023),(0,'A1',30),(1000,'A1',61),
                                (2000,'A1',62),(3500,'A1',95),(5000,'A1',30)]])
        for t,value in [(500,0),(1500,0),(2700,255),(4200,255),(5700,0)]:
            sample(plan,'pwm',t,5,value,'Only A1 LM35 temperatures greater than 30 Celsius enable D5')
        return plan
    if case in ('embedbench_41','embedbench_102'):
        plan=read(BASE/'behavior_all296_20261023/embed_native'/case/'native_plan.json')
        plan.update(horizon_ms=45000,checks=[])
        for count in (0,1,2,3,9,10,11,20):
            for name,value in zip(('sevseg1','sevseg2'),f'{count:02}'):
                sample(plan,'display',count*2000+250,name,value,'Show the counter in the designated decimal digits')
        return plan
    plan=read(PREVIOUS/'decision_plans'/f'{case}.json')
    if case=='simple_020':
        for t,v in [(500,'0000'),(1100,'1111'),(1700,'0000'),(2900,'1111'),(3800,'0000')]:
            sample(plan,'display',t,'tm:4:5',v,'D2 pressed shows 1111 and released shows 0000')
    else:
        for t,n,second in [(500,0,0),(2700,12,0),(4600,12,77),(5700,49,77),(7700,49,99),(8700,81,99)]:
            for name,value in zip(('sevseg1','sevseg2'),f'{n:02}'):
                sample(plan,'display',t,name,value,'Display servo 1 angle with the correct decimal digit assignment')
            sample(plan,'servo',t,2,n,'Potentiometer 1 sets servo 1 angle',1)
            sample(plan,'servo',t,3,second,'Potentiometer 2 independently sets servo 2 angle',1)
    return plan


def run(case):
    original=ARCHIVE/case/'repeat_1'
    target=OUT/case
    plan=test_plan(case)
    host.save(target/'test_plan.json',plan)
    source=read(original/'model.json')['system_sysml2']
    contract,source=prepare(case,read(original/'contract.json'),source)
    host.save(target/'contract.json',contract)
    (target/'model.sysml').write_text(source,encoding='utf-8')
    board=contract['board']
    host.case_files(case,plan,board)
    row=dict(case=case,board=board,stage='model_validation',case_scope=(
        'display_regression_only' if case in ('embedbench_41','embedbench_102') else 'diagnosed_core_behaviors'))
    try:
        accepted=workflow.validate_candidate(source)
        workflow.save(target/'model_validation.json',accepted)
        if not accepted['ok']:
            return row
        row['stage']='lowering'
        with workflow.TRANSLATOR, workflow.JAVA:
            generated=workflow.lower_verified(source,contract,accepted['ir'],target/'lowered')
        code=workflow.adapt_verified(generated,contract,target/'lowered')
        receipt=read(target/'lowered/arduino_provenance.json')
        row['stage']='target_compile'
        compiled,_=workflow.compile_candidate(dict(id=case,fqbn=board),target/'arduino',code,receipt)
        row.update(compiled)
        if not compiled['compile_ok']:
            return row
        row['stage']='behavior'
        cpp=target/'arduino/build/sketch/sketch.ino.cpp'
        measured=host.execute(cpp,board,case,plan,target/'behavior')
        row.update(replay=measured['status'],checks=len(measured.get('checks',[])),
                   failed_checks=[c for c in measured.get('checks',[]) if not c['passed']])
    except Exception as error:
        row.update(error_type=type(error).__name__,error=str(error)[:2000])
    return row


def main():
    host.OUT=OUT
    host.FIXTURE=OUT/'fixture'
    host.fixture()
    rows=[]
    with ThreadPoolExecutor(max_workers=2) as pool:
        jobs={pool.submit(run,c):c for c in CASES}
        for future in as_completed(jobs):
            try:
                row=future.result()
            except Exception as error:
                row=dict(case=jobs[future],stage='preparation',error_type=type(error).__name__,error=str(error))
            rows.append(row)
            workflow.save(OUT/'summary.json',dict(planned=len(CASES),rows=rows,model_calls=0))
            print(row['case'],row.get('stage'),row.get('compile_ok'),row.get('replay'),row.get('error'),flush=True)


if __name__=='__main__':
    main()
