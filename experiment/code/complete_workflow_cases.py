"""Regenerate the remaining workflow cases with immutable public host tests.

All candidates go through contract, SysML, IR/IL/C and deterministic hardware
adaptation. No direct-code fallback and no mutation of historical evidence.
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
import traceback

import run_current_comparison as workflow
import behavior_full_host as host
import run_behavior_pilot10 as pilot
import run_workflow267_host_smoke as base
from run_new_workflow_pceval50 import ModelCalls, STOP
from app.services.configuration_policy import resolve_configuration
from app.services.verified_generation import generate_verified_system
from app.services.llm_json import json_prompt
from operational_profile import acceptance_profile
import design_behavior_tests as design
from behavior_full_protocol import PROTOCOL

ROOT = workflow.ROOT
BUNDLE = ROOT / 'experiment/results'


def read(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))


def prepare(case):
    case = copy.deepcopy(case)
    disclosed = {
        'pceval_serial_LCD_display': 'Disclosed finite transport domain for this Uno bench: each input is '
            'a nonempty printable-ASCII line of at most63 bytes, excluding its CR/LF delimiter. Preserve '
            'all received spaces and tabs, including at the beginning and end, in display and echo. '
            'Frames exceeding the buffer bound or containing NUL/non-ASCII are rejected without partial '
            'display or echo; arbitrary-length and non-ASCII text are outside this disclosed bench scope.',
        'pceval_dday_counter': 'Disclosed day-count convention where the source is silent: subtract the '
            'captured setup-time RTC Unix seconds from the accepted ISO target Unix seconds, divide by86400 '
            'and truncate toward zero. Use the same timezone for both. Past targets yield negative days. '
            'Capture RTC only once; do not replace it with the example timestamps.',
        'pceval_piano_keyboard': 'Disclosed resolution of the source component-value inconsistency: '
            'the explicit prose and Attributes instruction specifying10kOhm button pull-down resistors '
            'take precedence over the contradictory resistor value fields in the embedded circuit. '
            'Preserve every connection and MCU pin. This bench variant uses10kOhm external pull-downs, '
            'buttons active HIGH and INPUT without internal pull-ups; only one button is held at a time.',
        'pceval_exercise_counter': 'Disclosed representation choice for the single decimal digit: retain '
            'the logical total count in the model and display its decimal ones digit (total modulo10). '
            'Crossing9 must not stop or reset the logical total. This resolves unspecified visual overflow; '
            'it does not change the required motion threshold or rearming condition.',
        'advanced_001': 'Disclosed configurable bench choices for otherwise unspecified parameters: automatic fan '
            'mode at startup, relay outputs active HIGH, integer Celsius temperature and raw ADC light/soil display. '
            'Manual mode initially selects fan; each D3 press selects light,pump,fan in that order. '
            'A manual update changes only its selected actuator and retains the others until selected or auto mode resumes. '
            'PWM ramps from 0 at 25C to 255 at 35C, light turns on below ADC400, pump turns on below soil ADC400. '
            'D2 toggles auto/manual; D3 cycles fan,light,pump in manual, and A2 maps 0..1023 to PWM0..255 for fan '
            'or OFF/ON at ADC512 for the selected digital output. Display cycles temperature/light/soil every 2s. '
            'Sensor fault means invalid DHT acquisition or analog inputs railed at 0/1023; these analog endpoints '
            'are declared fault sentinels in this bench variant. Fault drives D6 and a Serial fault report. '
            'The existing pin conflict resolver and its recorded remaps are authoritative.',
        'advanced_004': 'Disclosed bench UI/transport: one serial_packet_input at 9600 baud, commands '
            'TIME,SET1,SET2,SET3 with codes1,2,3,4; each frame is COMMAND INTEGER, e.g. TIME 930 or SET1 945. '
            'Values0..2359 encode HHMM; reject a minute field >=60 in the model. Advance the simulated current '
            'time using elapsed milliseconds after TIME. Default alarms07:00,12:00,18:00, editable by SET1..3. '
            'D2 confirms the active reminder, D3/D4 browse previous/next reminder status; live HHMM is the '
            'default page. At each daily alarm start intermittent 500ms sound/LED; confirmation stops it; '
            'after30s unconfirmed emit missed dose once and increment a retained missed counter. '
            'All business clock/alarm/UI logic belongs in SysML.',
        'advanced_010': 'Disclosed bench parameters and transport: item prices1,2,3 and initial stock2 each. '
            'D2/D3/D4 select items1/2/3; short D7 release confirms a sale. A 2s D7 hold toggles admin mode '
            'without dispensing. In admin, serial_packet_input commands RESTOCK1,RESTOCK2,RESTOCK3 '
            'with codes1,2,3 accept COMMAND INTEGER frames at9600baud, payload0..9999 as added stock; '
            'only admin accepts restocking. Stock is bounded0..9999. Display alternates selected item/price '
            'each second, out-of-stock displays -1, accepted dispense emits dispense:itemX and a100ms beep. '
            'Keep stock, sale guards, hold timing and admin transitions in SysML.',
        'advanced_016': 'Disclosed configurable bench calibration/thresholds: A0 current=ADC*10/1023 amperes; '
            'LM35 A1 Celsius=ADC*500/1023 with5V ADC reference. Overload current>7A, overheat>60C; '
            'vibration is a grounded active-LOW switch on D2 with INPUT_PULLUP, released HIGH. Status0 normal,1 overload, '
            '2 overheat,3 mechanical fault; mechanical fault has highest priority, then overheat. '
            'Fan PWM255 above60C and0 otherwise. Mechanical fault drives audible/LED alarm and Serial fault code3. '
            'TM1637 rotates integer current in amperes, integer temperature Celsius, and status code every2s.',
        'advanced_020': 'Disclosed configurable bench parameters: A0 load=ADC*100/1023 percent; overload>80percent. '
            'Start at floor1. D2/D3/D4 request floors1/2/3; move takes1000ms, then emit movement complete once '
            'at9600baud and latch the arrived floor. Overload rejects new moves and cancels a pending move, '
            'preserving the last arrived floor. Sound/LED reflect overload. Display alternates current floor '
            'and integer load percentage each second.',
        'advanced_021': 'Disclosed configurable bench parameters: LM35 A1 Celsius=ADC*500/1023 under5V ADC; '
            'heater, illumination and pump driver control signals are active HIGH in this bench. '
            'heater hysteresis ON below24C and OFF above26C; illumination ON below light ADC400. '
            'Water A2 below200 is low; low water switches OFF heater and pump and activates buzzer. '
            'At safe water pump ON; normal water restores ordinary temperature control. D2 press cycles '
            'displayed integer temperature, light ADC and water ADC, initially temperature. '
            'Use recorded remapped pins where the source fan/pump and display share D5.',
        'advanced_022': 'Disclosed bench clock/transport: serial_packet_input at9600baud with commands '
            'PIR motion is HIGH on D2, configured INPUT without pull-up; D3 is a grounded reed contact '
            'with INPUT_PULLUP, LOW door-closed and HIGH door-open. '
            'NOW,START,END and codes1,2,3; COMMAND INTEGER frames encode minutes of day0..1439. '
            'NOW synchronizes a clock that thereafter advances by elapsed milliseconds. START/END set the '
            'reservation for this day, START<END. Green LED blinks every500ms during the minute before START. '
            'PIR activity while the door is open latches occupied; D7 releases it. Occupied lights red and '
            'selects scrolling OCCUPIED on TM1637; released selects FREE. After END, occupied causes buzzer '
            'and an overtime use report once. Adapter text scrolling is presentation only; time, occupancy '
            'and report gates remain in SysML.',
        'advanced_025': 'Disclosed configurable bench UI/defaults: initial editing mode; three phases have '
            'targets30C,40C,35C and10s durations. Short D2 cycles the editing phase; D3 raises that phase target '
            'by5C (wrap above95C to30C), D4 raises duration by10s (wrap above120s to10s). Holding D2 for1s '
            'starts the process without a short-press edit. Each phase waits until LM35 reaches its target, '
            'beeps100ms once, then counts the selected duration and advances automatically. LM35 Celsius '
            '=ADC*500/1023 under5V reference. Display alternates phase number1..3 and remaining whole seconds '
            'each second. At completion stop the process and emit a completion log; emit phase entry/reached/exit '
            'logs at9600baud. Temperature acquisition, state retention and timing remain in SysML.',
        'advanced_028': 'Disclosed bench event semantics: grounded active-LOW bed/confirm buttons, one action '
            'per press edge with20ms debounce. FIFO among the three beds; repeated presses from a bed already '
            'pending do not duplicate its call. D7 removes only the oldest pending call, prints processed:bedN '
            'and its waited whole seconds, then keeps later calls pending. Display alternates the oldest bed '
            'number and waited seconds every second; zero when empty. LED/buzzer indicate any pending call. '
            'Transport9600baud; all queue/age/arbitration logic remains in SysML.',
        'advanced_029': 'Disclosed configurable bench parameters: both LM35 Celsius=ADC*500/1023 with5V ADC. '
            'D2 is a grounded reed contact with INPUT_PULLUP: LOW closed, HIGH open. Fan and defrost '
            'relay driver commands are active HIGH. '
            'Fan ON above ambient8C and OFF at/below8C. Ambient>10C is alarm level1. Evaporator<2C starts '
            'defrost and level2; defrost relay stops at evaporator>=8C or after30s. Open door pauses fan/defrost; '
            'door continuously open>10s is level3. Alarm priority door3,defrost2,ambient1,normal0; buzzer active '
            'for any alarm. TM1637 cycles integer ambient,evaporator,alarm level every2s; Serial reports changes. '
            'Do not compare unconverted ADC counts to Celsius thresholds. All decisions/timers belong in SysML.',
        'pceval_button_LCD_display': 'Disclosed core acceptance clarification for the source wording conflict: '
            'the stated project goal and success criterion are to display the current button state. '
            'Display Pressed! while the button is HIGH/held and Not pressed! while LOW/released. '
            'The phrase newly pressed describes when to redraw, rather than limiting visibility to one controller tick. '
            'Retained correct LCD text is valid; repeated identical print calls are not required. '
            'This interpretation is the existing frozen core test criterion; original source text is preserved.',
        'medium_018': 'Disclosed bench completion for unspecified devices: fading/PWM LED on D9, '
            'potentiometer ADC on A0. Buttons D2/D3 connect to GND with INPUT_PULLUP, active LOW. '
            'D2 selects automatic repeated 0..255 brightness ramp, D3 selects manual ADC-to-PWM control. '
            'Ramp duration is configurable; bench uses a linear PWM ramp taking2 seconds in each direction. Initially automatic, '
            'starting at zero and rising; selecting automatic again restarts the rise from zero.',
        'medium_019': 'Disclosed bench completion for unspecified display/wiring: TM1637 CLK=D4,DIO=D5; '
            'DHT11 DATA=D7; HC-SR04 TRIG=D8,ECHO=D9; button D2 to GND, INPUT_PULLUP, active LOW. '
            'Each completed button cycle switches the displayed quantity. Display response within 250ms '
            'of a new available sensor value; sensor acquisition limits remain applicable.',
        'advanced_018': 'Disclosed configurable bench parameters where the source leaves them unspecified: '
            'temperature lower=37 Celsius, upper=38 Celsius, setpoint=37.5 Celsius; ventilation runs '
            '10 seconds every 5 minutes. Heater uses hysteresis between those limits; fan ventilates '
            'and can cool above the upper limit. Temperature outside either bound continuously for '
            '60 seconds raises the alarm; returning inside the band resets that persistence timer. '
            'Display current temperature or setpoint as whole Celsius; every D2 press cycles the mode. '
            'Bench Serial baud is 9600. The existing configuration policy explicitly resolves the '
            'fan/TM1637 D5 pin collision; the original requirement is retained alongside the resolution.',
    }
    original = case['requirement']
    requirement = original + ('\n' + disclosed[case['id']] if case['id'] in disclosed else '')
    resolution = resolve_configuration(requirement, {}, board=case['fqbn'])
    case.update(original_requirement=original, configuration_resolution=resolution,
                effective_requirement=resolution['effective_requirement'] + acceptance_profile(original),
                bench_completion=disclosed.get(case['id']))
    return case


def saved_plans(case):
    if case['id'] in ('medium_018','medium_019','pceval_serial_LCD_display'):
        # Their old supplements fill absent wiring/timing differently. Preserve
        # those records, but design fresh tests for this disclosed bench first.
        return []
    row = next(r for r in read(BUNDLE/'final_cases.json') if r['case'] == case['id'])
    plans = [dict(name=e['stage'], plan=read(BUNDLE/e['plan']), source=str(BUNDLE/e['plan']),
                 recorded_status=e['status'], review_required=e.get('engine') in ('trace_review','reviewed_assertions'))
            for e in row['evaluations'] if e.get('plan') and e.get('engine') != 'pilot10' and
            (e['status']=='passed' or e.get('engine') in ('trace_review','reviewed_assertions')) and
            read(BUNDLE/e['plan']).get('checks')]
    for entry in plans:
        entry['plan'].pop('reference_cpp', None)
    return plans


def public_tests(case, folder, model, *, reuse_saved=True):
    path = folder/'test_plans.json'
    if path.is_file():
        return read(path)
    plans = saved_plans(case) if reuse_saved else []
    # Supplementary plans are separate; retain all old inputs and assertions.
    if plans:
        workflow.save(path, plans)
        return plans
    calls = ModelCalls(folder/'test_design_calls', model, role='independent_requirement_test_design',
                       budget=4, reasoning_effort='medium' if 'gpt-6.1' in model else None, max_output_tokens=12000)
    auditor = ModelCalls(folder/'test_audit_calls',model,role='independent_test_reference_audit',
                         budget=12,reasoning_effort='medium' if 'gpt-6.1' in model else None,max_output_tokens=6000)
    seed=folder/'test_design_seed.json'
    feedback = ('\nPrevious independent requirement-only draft and its audit. Repair these test/reference '
                'issues; no candidate code or candidate observations are provided:\n'+seed.read_text(encoding='utf8')) if seed.exists() else ''
    for attempt in range(4):
        prompt = PROTOCOL + ('\nUse only this requirement and disclosed bench conditions. You have no candidate code. '
            'Cover the principal behavior with multiple changing inputs, repeated button actions, and specified '
            'timing. Do not invent missing business thresholds or enforce arbitrary labels. For declared bench '
            'parameters the appended explicit conditions are authoritative. Return the reference_cpp as a '
            'complete standalone Arduino program using the documented device APIs. The reference is a behavioral '
            'witness, deliberately direct C++; SysML ownership applies to the later candidate, not this reference. '
            'Prefer a sound executable partial suite to invented assumptions. Mark every uncovered core feature '
            'in coverage_explanation. Do not enforce unspecified display units, startup modes, relay polarities, '
            'alarm cadences, arbitrary labels or serial report repetition. For an unspecified audible cadence, '
            'use an interval witness of some audible activity, not one arbitrary instant; leave it uncovered if '
            'no supported check can establish it. Distinguish required behavior from optional reference choices. '
            '\nREQUIREMENT:\n') + case['effective_requirement'] + feedback
        _, raw = calls(json_prompt(prompt))
        try:
            plan = design.parse(raw); design.validate(plan, case['effective_requirement'])
            if not plan.get('supported'):
                raise ValueError('No executable requirement tests proposed: '+str(plan.get('coverage_explanation')))
            audit_feedback=''
            for audit_attempt in range(3):
                _,audit_raw=auditor(design.review_prompt(case['effective_requirement'],plan)+audit_feedback)
                try:
                    audit=design.parse(audit_raw)
                    assert type(audit.get('approved')) is bool and audit.get('coverage') in ('full','partial')
                    assert isinstance(audit.get('issues'),list) and isinstance(audit.get('coverage_gaps'),list)
                    break
                except (ValueError,AssertionError,TypeError):
                    audit_feedback='\nReturn the specified approved/coverage/issues/coverage_gaps/reason JSON schema.'
            else:raise ValueError('Independent test audit response could not be validated')
            workflow.save(folder/f'test_design_audit_{attempt}.json',audit)
            if not audit['approved']:
                raise ValueError('Independent requirement audit rejected this test/reference: '+json.dumps(audit,ensure_ascii=False))
            plan['coverage']='full' if plan['coverage']=='full' and audit['coverage']=='full' else 'partial'
            ref = folder/f'reference_{attempt}.cpp'
            ref.write_text(plan['reference_cpp'], encoding='utf8')
            host.case_files(case['id'], plan, case['fqbn'])
            measured = host.execute(ref,case['fqbn'],case['id'],plan,folder/f'reference_{attempt}',reference=True)
            if measured['status'] != 'passed':
                diagnostic = measured
                stderr = folder/f'reference_{attempt}/build.stderr.txt'
                if stderr.is_file():diagnostic=dict(measured,build_stderr=stderr.read_text(errors='replace')[-4000:])
                raise ValueError('Reference program did not pass its own plan: '+json.dumps(diagnostic,ensure_ascii=False)[-10000:])
            plan.pop('reference_cpp')
            plans=[dict(name='new_core',plan=plan,source='independently_designed_before_generation',
                        reference_result=str(folder/f'reference_{attempt}/result.json'))]
            workflow.save(path,plans)
            workflow.save(folder/'test_audit.json',dict(case=case['id'],all_plans_approved=True,reviews=[audit],
                scope='Independent requirement/test/reference audit completed BEFORE candidate generation'))
            return plans
        except (ValueError,AssertionError,KeyError,TypeError) as exc:
            feedback='\nPrevious design/reference was invalid. Repair it without candidate information:\n'+str(exc)
            workflow.save(folder/f'test_design_failure_{attempt}.json',dict(error=str(exc)))
    return []


def run_case(case, out, model, rounds, initial_feedback=None):
    out=Path(out); folder=out/'cases'/case['id']; folder.mkdir(parents=True,exist_ok=True)
    final=folder/'result.json'
    if final.is_file():return read(final)
    STOP.clear()
    os.environ['WINDOWS_ARDUINO_CLI_PATH']=str(workflow.CLI)
    os.environ['PATH']=str(workflow.CLI.parent)+os.pathsep+os.environ['PATH']
    base.GXX=Path(os.environ.get('CXX') or shutil.which('g++'))
    pilot.OUT=BUNDLE/'host_templates';host.OUT=folder/'host';host.FIXTURE=host.OUT/'fixture';host.fixture()
    generation=ModelCalls(folder/'generation_calls',model,role='contract_and_model',budget=80,
                          reasoning_effort='medium' if 'gpt-6.1' in model else None,max_output_tokens=12000)
    review=ModelCalls(folder/'semantic_review_calls',model,role='independent_semantic_review',budget=50,
                      reasoning_effort='medium' if 'gpt-6.1' in model else None,max_output_tokens=12000)
    row=dict(case=case['id'],dataset=case['dataset'],origin='regenerated_verified_workflow',
             compile_ok=False,core_status='unverified',attempts=[],simulide_status='not_run',model=model)
    workflow.save(folder/'input.json',case)
    started=time.monotonic(); feedback=initial_feedback
    try:
        plans=public_tests(case,folder,model)
        row['test_plans']=len(plans)
        for attempt in range(rounds):
            current=folder/f'attempt_{attempt}';accepted={};detail=dict(attempt=attempt)
            try:
                def validator(source):
                    result=workflow.validate_candidate(source)
                    if result['ok']:accepted.update(source=source,ir=result['ir'])
                    return result
                def persist(revision,record):
                    workflow.save(current/f'contract_revision_{revision}.json',record)
                contract,_,bundle,validation=generate_verified_system(generation,review,case['effective_requirement'],{},
                    target_board=case['fqbn'],configuration_resolution=case['configuration_resolution'],
                    validator=validator,max_model_repairs=4,max_contract_revisions=2,
                    diagnostic_feedback=feedback,persist=persist,semantic_review_scope='model_boundary')
                for name,data in [('contract.json',contract),('model.json',bundle),('validation.json',validation)]:
                    workflow.save(current/name,data)
                assert bundle['system_sysml2']==accepted['source']
                with workflow.TRANSLATOR,workflow.JAVA:
                    generated=workflow.lower_verified(accepted['source'],contract,accepted['ir'],current/'lowered')
                code=workflow.adapt_verified(generated,contract,current/'lowered')
                receipt=read(current/'lowered/arduino_provenance.json')
                compiled,log=workflow.compile_candidate(case,current/'arduino',code,receipt)
                detail['compilation']=compiled
                if not compiled['compile_ok']:
                    feedback=dict(stage='target_compile',diagnostics=log[-10000:])
                else:
                    row.update(compile_ok=True,artifact=compiled['artifact'],artifact_sha256=compiled['artifact_sha256'],selected_attempt=attempt)
                    measured=[]
                    for entry in plans:
                        host.case_files(case['id'],entry['plan'],case['fqbn'])
                        scored=host.execute(current/'arduino/build/sketch/sketch.ino.cpp',case['fqbn'],case['id'],
                                            entry['plan'],current/'behavior'/entry['name'])
                        measured.append(dict(name=entry['name'],**scored))
                    detail['behavior']=measured
                    if not plans:
                        # Test-design failure is not a candidate counterexample.
                        # Preserve the compilable artifact without empty repair feedback.
                        row['core_status']='unverified'
                        row['attempts'].append(detail);break
                    passed=bool(measured) and all(r['status']=='passed' and r.get('checks') for r in measured)
                    row['core_status']='core_scenarios_passed' if passed else 'failed' if any(r['status']=='failed' for r in measured) else 'unverified'
                    if passed:
                        row['attempts'].append(detail);break
                    feedback=dict(stage='executed_core_scenarios',preserve_all_acceptance=True,
                        tests=[dict(plan=e['plan'],observed=[c for c in m.get('checks',[]) if not c['passed']]) for e,m in zip(plans,measured)])
            except Exception as exc:
                detail.update(error_type=type(exc).__name__,error=str(exc)[:3000])
                if isinstance(exc,(NameError,OSError)):
                    (current/'harness_failure.txt').parent.mkdir(parents=True,exist_ok=True)
                    (current/'harness_failure.txt').write_text(traceback.format_exc(),encoding='utf8')
                    row['harness_failure']=True
                if hasattr(exc,'attempts'):workflow.save(current/'generation_failures.json',exc.attempts)
                if hasattr(exc,'generation_rounds'):workflow.save(current/'generation_rounds.json',exc.generation_rounds)
                feedback=dict(stage='model_or_contract_validation',diagnostic=detail)
                if STOP.is_set():row['infrastructure_interrupted']=True
            row['attempts'].append(detail);workflow.save(folder/'progress.json',row)
            if STOP.is_set() or row.get('harness_failure'):break
    except Exception as exc:
        row.update(error_type=type(exc).__name__,error=str(exc)[:2000],infrastructure_interrupted=STOP.is_set())
    row.update(seconds=round(time.monotonic()-started,2),generation_usage=workflow.metrics(generation.entries),
               semantic_review_usage=workflow.metrics(review.entries))
    workflow.save(final,row)
    return row


def summarize(out, cases):
    rows=[read(p) for p in (out/'cases').glob('*/result.json')]
    base_rows=read(BUNDLE/'final_cases.json'); replacement={r['case']:r for r in rows}
    passes=sum((replacement[r['case']]['core_status']=='core_scenarios_passed') if r['case'] in replacement
               else r['confirmed_core_pass'] for r in base_rows)
    result=dict(planned=len(cases),completed=len(rows),compiled=sum(r['compile_ok'] for r in rows),
                core_passed=sum(r['core_status']=='core_scenarios_passed' for r in rows),
                all296_with_new_candidates=passes,all296_rate=passes/296,
                original_delivery_retained=True,scope='Targeted development rerun, not a new independent all296 evaluation')
    result['delivery_with_retained_prior_passes'] = sum(r['confirmed_core_pass'] or
        replacement.get(r['case'],{}).get('core_status')=='core_scenarios_passed' for r in base_rows)
    workflow.save(out/'results.json',rows);workflow.save(out/'summary.json',result)
    return result


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out',default='experiment/output/workflow_completion_20261107/run1')
    parser.add_argument('--case',action='append');parser.add_argument('--workers',type=int,default=2)
    parser.add_argument('--model',default=os.environ.get('OPENAI_MODEL','openai/gpt-5.4-nano'))
    parser.add_argument('--rounds',type=int,default=4)
    args=parser.parse_args()
    if not 1<=args.workers<=4 or not 1<=args.rounds<=6:parser.error('workers 1..4, rounds 1..6')
    out=(ROOT/args.out).resolve();out.relative_to((ROOT/'experiment/output').resolve());out.mkdir(parents=True,exist_ok=True)
    targets={r['case'] for r in read(BUNDLE/'groups.json') if r['group']=='workflow_280' and not r['compile_ok']}
    targets|={r['case'] for r in read(BUNDLE/'summary/remaining_cases.json')}
    if args.case:targets=set(args.case)
    cases=[prepare(r) for r in read(ROOT/'experiment/formal/manifest.json')['cases'] if r['id'] in targets]
    assert len(cases)==len(targets)
    # Interleave device families; each worker owns its test fixture and model client.
    cases.sort(key=lambda r:(r['dataset']!='PCEval',r['id']))
    sources=list((ROOT/'app/services').glob('*.py'))+[Path(__file__)]+[
        ROOT/'experiment/code'/name for name in ('behavior_full_host.py','behavior_full_support.py',
        'behavior_full_protocol.py','operational_profile.py','design_behavior_tests.py','run_new_workflow_pceval50.py')]
    config=dict(cases=cases,model=args.model,rounds=args.rounds,workers=args.workers,
                source_hashes={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources})
    frozen=out/'execution.json'
    if frozen.exists():assert read(frozen)==config,'Source/configuration changed; choose a new output version'
    else:
        workflow.save(frozen,config)
        for source in sources:
            target=out/'method_snapshot'/source.relative_to(ROOT)
            target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(source,target)
    with concurrent.futures.ProcessPoolExecutor(max_workers=args.workers) as pool:
        remaining=iter(cases);futures={};stopped=False
        def submit():
            case=next(remaining,None)
            if case is not None:futures[pool.submit(run_case,case,str(out),args.model,args.rounds)]=case['id']
        for _ in range(args.workers):submit()
        while futures:
            done,_=concurrent.futures.wait(futures,return_when=concurrent.futures.FIRST_COMPLETED)
            for future in done:
                case_id=futures.pop(future)
                try:row=future.result()
                except Exception as exc:
                    workflow.save(out/'harness_failure.json',dict(case=case_id,error_type=type(exc).__name__,error=str(exc)[:1000]))
                    stopped=True;continue
                print(row['case'],row['compile_ok'],row['core_status'],flush=True)
                stopped |= bool(row.get('infrastructure_interrupted') or row.get('harness_failure'))
                if not stopped:submit()
            print(json.dumps(summarize(out,cases)),flush=True)
        if stopped:workflow.save(out/'interruption.json',dict(reason='Infrastructure/harness failure; unscheduled cases remain pending'))
    print(json.dumps(summarize(out,cases)),flush=True)


if __name__=='__main__':main()
