"""Explicit manual MDD model for the underspecified two-sensor display task.

This is human-assisted offline modeling, not a successful automatic LLM run.
The generated kernel is never patched; all behavior is in the saved controller.
"""
import json
import os
from pathlib import Path
import shutil

import complete_workflow_cases as run
from app.services.interface_planner import plan_interface_contract
from app.services.sysml import generate_validated_bundle


def main():
    os.environ.setdefault('JAVA_TOOL_OPTIONS','-Xmx1024m -XX:+UseSerialGC')
    out=run.ROOT/'experiment/output/workflow_completion_20261107/run14_manual_medium019'
    case=run.prepare(next(c for c in run.read(run.ROOT/'experiment/formal/manifest.json')['cases'] if c['id']=='medium_019'))
    folder=out/'cases'/case['id'];current=folder/'attempt_0'
    folder.mkdir(parents=True,exist_ok=True)
    run.workflow.save(out/'execution.json',dict(cases=[case['id']],model_requests=0,origin='offline_manually_authored_sysml'))
    run.workflow.save(folder/'input.json',case)
    quote=case['original_requirement']
    plan=dict(supported=True,coverage='partial',coverage_explanation='Temperature/distance selection, sensor changes and held-button retention; no physical sensor-failure or pixel/electrical simulation.',
        horizon_ms=14500,input_steps=[dict(at_ms=t,kind=k,pin=p,value=v) for t,k,p,v in [
            (0,'digital',2,1),(3000,'digital',2,0),(3300,'digital',2,1),
            (7500,'digital',2,0),(7800,'digital',2,1),(11000,'digital',2,0),(12800,'digital',2,1),
            (0,'temperature',7,21),(6000,'temperature',7,33),(12000,'temperature',7,42),
            (0,'distance',9,120),(9000,'distance',9,77)]],
        ultrasonic=[dict(trigger_pin=8,echo_pin=9)],checks=[dict(kind='display',id='selection_'+str(t),
            requirement_quote=quote,name='tm:4:5',at_ms=t,expected=str(v),numeric=True) for t,v in
            [(2500,21),(3400,120),(5000,120),(7900,33),(10500,33),(11400,77),(13000,77)]])
    run.workflow.save(folder/'test_plans.json',[dict(name='manual_requirement_core',plan=plan,source='offline_manual_requirement_design')])
    run.workflow.save(folder/'test_audit.json',dict(all_plans_approved=True,review_source='offline_manual',coverage='partial',
        reason='Each button press toggles exactly two required quantities. Distinct temperature and distance stimuli establish selected-source dependence; held input does not cycle repeatedly. Test was fixed before creating the model.'))
    requests=[dict(kind='digital_input',pin=2,pullup=True,active_high=False,initial=False,meaning='Pressed selector button'),
        dict(kind='dht_temperature',pin=7,sensor='DHT11',sample_period_ms=2000,initial=0,meaning='Measured temperature Celsius'),
        dict(kind='dht_valid',pin=7,sensor='DHT11',sample_period_ms=2000,initial=False,meaning='Temperature sample valid'),
        dict(kind='ultrasonic_distance',trigger_pin=8,echo_pin=9,sample_period_ms=60,initial=0,meaning='Measured distance cm'),
        dict(kind='ultrasonic_valid',trigger_pin=8,echo_pin=9,sample_period_ms=60,initial=False,meaning='Distance sample valid'),
        dict(kind='tm1637_number',clk_pin=4,dio_pin=5,initial=0,meaning='Selected measured quantity')]
    proposal=dict(board=case['fqbn'],control_period_ms=5,interface_requests=requests,serial_channels=[])
    try:
        contract,_=plan_interface_contract(lambda _:('offline_manual',json.dumps(proposal)),case['effective_requirement'],{},
            max_repairs=0,target_board=case['fqbn'],configuration_resolution=case['configuration_resolution'])
    except Exception as exc:
        if hasattr(exc,'attempts'):run.workflow.save(folder/'planning_failure.json',exc.attempts)
        raise
    controller=dict(states=[dict(name='distanceMode',type='bool',initial=False),
        dict(name='previousPressed',type='bool',initial=False),dict(name='selectedValue',type='i32',initial=0)],
        updates=[],outputs=dict(io_5='selectedValue'))
    controller['updates']=[{'if':'io_0 and not previousPressed','then':[dict(assign='distanceMode',value='not distanceMode')]},
        dict(assign='previousPressed',value='io_0'),
        {'if':'distanceMode and io_4','then':[dict(assign='selectedValue',value='RealFunctions::floor(io_3)')],
         'else':[{'if':'not distanceMode and io_2','then':[dict(assign='selectedValue',value='RealFunctions::floor(io_1)')]}]}]
    response=dict(controller=controller,req_test_map=dict(requirements_to_testpoints=[dict(requirement_id=r['id'],
        test_point_id=r['assertion_ids'][0],test_method='host_trace',stimulus='Changed temperature/distance and repeated button edges',
        oracle='The display retains only the selected valid sensor value') for r in contract['requirements']]))
    run.workflow.save(folder/'manual_controller.json',response)
    accepted={}
    def validator(source):
        checked=run.workflow.validate_candidate(source)
        if checked['ok']:accepted.update(source=source,ir=checked['ir'])
        return checked
    row=dict(case=case['id'],dataset=case['dataset'],origin='offline_manually_authored_sysml',model_requests=0,
        compile_ok=False,core_status='unverified',attempts=[],simulide_status='not_run')
    try:
        _,model,validation=generate_validated_bundle(lambda _:('offline_manual',json.dumps(response)),
            case['effective_requirement'],max_repairs=0,validator=validator,contract=contract)
        for name,data in [('contract.json',contract),('model.json',model),('validation.json',validation)]:run.workflow.save(current/name,data)
        run.workflow.save(folder/'manual_model_review.json',dict(origin='offline_manual',
            review='Mode starts on temperature; only a new press toggles it. Reads actual native device values, holds prior display on invalid sample, and performs no sensor substitution or kernel patch. Automated independent semantic gate was unavailable.'))
        generated=run.workflow.lower_verified(accepted['source'],contract,accepted['ir'],current/'lowered')
        code=run.workflow.adapt_verified(generated,contract,current/'lowered')
        os.environ['WINDOWS_ARDUINO_CLI_PATH']=str(run.workflow.CLI)
        compiled,_=run.workflow.compile_candidate(case,current/'arduino',code,run.read(current/'lowered/arduino_provenance.json'))
        row.update(compile_ok=compiled['compile_ok'],artifact=compiled['artifact'],artifact_sha256=compiled['artifact_sha256'],selected_attempt=0)
        detail=dict(attempt=0,compilation=compiled,behavior=[])
        if compiled['compile_ok']:
            run.pilot.OUT=run.BUNDLE/'host_templates';run.host.OUT=folder/'host';run.host.FIXTURE=run.host.OUT/'fixture'
            run.base.GXX=Path(shutil.which('g++'));run.host.fixture();run.host.case_files(case['id'],plan,case['fqbn'])
            result=run.host.execute(current/'arduino/build/sketch/sketch.ino.cpp',case['fqbn'],case['id'],plan,current/'behavior/manual_requirement_core')
            detail['behavior']=[dict(name='manual_requirement_core',**result)]
            row['core_status']='core_scenarios_passed' if result['status']=='passed' else 'failed' if result['status']=='failed' else 'unverified'
        row['attempts']=[detail]
    except Exception as exc:
        row.update(error_type=type(exc).__name__,error=str(exc)[:3000])
        if hasattr(exc,'attempts'):run.workflow.save(folder/'validation_failures.json',exc.attempts)
    run.workflow.save(folder/'result.json',row);run.workflow.save(out/'summary.json',row)
    print(case['id'],row['compile_ok'],row['core_status'])


if __name__=='__main__':main()
