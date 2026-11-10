"""Direct sketch edits and unchanged-test replays; separate from workflow results."""
import argparse
import copy
import difflib
import hashlib
import json
import os
from pathlib import Path
import sys
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'experiment/code'))
import behavior_full_host as host
import run_current_comparison as workflow

BASE = ROOT / 'experiment/output'
PRIMARY = BASE / 'core_feedback_regeneration_round1'
OUT = BASE / 'manual_code_repairs_20261106'
LEDGER = BASE / 'core_reassessment_20261106_pending217/decision_round/results_296.json'


class NoGeneratedCode(RuntimeError):
    pass


def read(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))


def digest_text(text):
    return hashlib.sha256(text.encode('utf-8')).hexdigest()


def replace_once(code, old, new):
    assert code.count(old) == 1, (old, code.count(old))
    return code.replace(old, new, 1)


def specific_patch(case, code):
    from manual_sketch_patch_rules import repair, wiring_buttons
    changed = repair(case, code)
    if changed is not None:
        revised,notes=wiring_buttons(case,changed[0])
        return revised,changed[1]+ ('; '+'; '.join(notes) if notes else '')
    if case == 'pceval_distance_sensor':
        code = replace_once(code,
            'hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 <= 23200UL',
            'hw_ultrasonic_pulse_0 >= 116UL && hw_ultrasonic_pulse_0 < 25000UL')
        return code, 'Accept non-timeout echoes up to the existing acquisition deadline; the former 400cm cutoff discarded the tested 401cm echo. Physical accuracy beyond nominal range is not established.'
    if case == 'pceval_humidity_sensor_basic':
        start = code.index('    int32_t sysml_conditional_0 = 0;', code.index('void ProgramPage1(double io_0', code.index('/* Code of ProgramPage1 */')))
        end = code.index('    bool sysml_value_0 = 0;', start)
        code = code[:start] + ('    sysml_state_reportDue = io_1;\n'
                '    if (io_1) sysml_state_cachedHumidity = io_0;\n') + code[end:]
        code = replace_once(code, '  uint32_t hw_dht_0_now = (uint32_t)millis();',
            '  hw_value_io_1 = false; // A valid fresh acquisition is a one-step event.\n'
            '  uint32_t hw_dht_0_now = (uint32_t)millis();')
        return code, 'Report each successful fresh DHT acquisition immediately; clear the freshness flag between acquisitions. Removes the independent reporting clock that delayed new readings.'
    if case == 'pceval_7_segment_display_counter':
        # millis() begins at Arduino timer initialization, not physical power-on.
        # This recovers setup elapsed time, without claiming bootloader coverage.
        old = 'uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;'
        assert code.count(old) == 1
        code = code.replace(old, old.replace(': 0;', ': hw_now;'))
        code = replace_once(code,
            '  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;',
            '  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : hw_now;')
        return code, 'Include the first observable millis() interval in the counter. Physical bootloader time remains outside Arduino millis() and is not proven by host execution.'
    if case == 'complex_003':
        return code, 'No code edit: the recorded static objection concerns sub-2cm readings outside the HC-SR04 nominal range. Preserve supported-range behavior and retest the actual welcome, flashing, beeps and 5s standby requirements.'
    revised,notes=wiring_buttons(case,code)
    if notes:return revised,'; '.join(notes)
    raise KeyError('No reviewed detail correction yet: '+case)


def patch(case,code):
    from manual_sketch_patch_rules import wiring_buttons,previous_input_order,internal_button_defaults
    notes=[]
    try:revised,why=specific_patch(case,code);notes.append(why)
    except KeyError:revised=code
    revised,wiring=wiring_buttons(case,revised);notes+=wiring
    revised,defaults=internal_button_defaults(case,revised);notes+=defaults
    revised,ordered=previous_input_order(revised)
    if ordered:notes.append('Defer input-history assignments until all edge-dependent reads finish: '+', '.join(ordered))
    if revised==code and case!='complex_003':
        raise KeyError('No reviewed detail correction yet: '+case)
    return revised,'; '.join(notes)


def supplement(case, frozen):
    from manual_core_test_plans import make_plan
    authored=make_plan(case,frozen)
    if authored is not None:return authored
    plan = copy.deepcopy(frozen)
    if case == 'simple_015':
        # Preserve and report the old integer-regex failure. The ADC quantization
        # does not justify changing a physically correct 24.93C output to 25.xxC.
        plan['checks']=[dict(kind='serial',id=f'calibrated_{t}',from_ms=t,to_ms=t+1900,
            expected=adc*500.0/1023.0,tolerance=.02,min_count=1,
            requirement_quote='LM35 output in Celsius with a declared 5V ADC reference')
            for t,adc in ((700,51),(3700,82),(6700,36))]
    elif case == 'pceval_humidity_sensor_basic':
        plan['horizon_ms'] = 41000
        plan['input_steps'] = [dict(at_ms=t,kind='humidity',pin=2,value=v)
                              for t,v in ((0,11.5),(4100,88.2),(8200,0),(12300,100),
                                          (16400,25.7),(20500,64.5),(24600,33.3),(28700,77.7))]
        plan['checks'] = [dict(kind='serial', id=f'fresh_{t}', from_ms=t,to_ms=t+3000,
            regex=rf'^Humidity: {v}(?:0+)?$',min_count=1,baud=9600,
            requirement_quote='Continuously report the current measured humidity')
            for t,v in ((0,11.5),(4100,88.2),(8200,0),(12300,100),
                        (16400,25.7),(20500,64.5),(24600,33.3),(28700,77.7))]
        for c in plan['checks']:
            if c['id'] in ('fresh_8200','fresh_12300'):
                c['regex'] = '^Humidity: ' + ('0' if c['id']=='fresh_8200' else '100') + r'(?:\.0+)?$'
    elif case == 'pceval_7_segment_display_counter':
        plan['horizon_ms'] = 21500
        plan['checks'] = [dict(kind='display',id=f'digit_{n}',name='sevseg1',at_ms=n*1000+300,
            expected=str(n%10),requirement_quote='Advance one decimal digit each second') for n in range(21)]
    else:
        return None
    plan.pop('reference_cpp', None)
    return plan


def run(case):
    folder = OUT / 'cases' / case
    if (folder/'result.json').exists():
        return read(folder/'result.json')
    metadata = next(c for c in read(PRIMARY/'execution.json')['cases'] if c['id']==case)
    source_group='workflow_267'
    if case in ('pceval_distance_sensor','pceval_humidity_sensor_basic','pceval_7_segment_display_counter','complex_003',
                'medium_028','pceval_button_RGB_led'):
        source_group='core_feedback_regeneration_round1'
        row = read(PRIMARY/'cases'/case/'result.json')
        attempt = PRIMARY/'cases'/case/f"attempt_{row['selected_compiled_attempt']}"
        source = attempt/'arduino/sketch/sketch.ino'
        original_cpp = attempt/'arduino/build/sketch/sketch.ino.cpp'
        planfile = PRIMARY/'validation/cases'/case/'tested_plan.json'
    else:
        archives=read(BASE/'all_groups_host_smoke_20261023/frozen_manifest.json')
        frozen = next(r for r in archives if r['group']=='workflow_267' and r['case']==case)
        if not frozen.get('source') and case in ('advanced_013','embedbench_24','embedbench_26','embedbench_77',
                                                 'embedbench_78','embedbench_79','pceval_binary_led','pceval_serial_bar_led'):
            frozen=next(r for r in archives if r['group']=='workflow_280' and r['case']==case)
            source_group='workflow_280'
        if not frozen.get('source') and case in ('medium_018','medium_019','pceval_button_LCD_display','pceval_dht22_LCD_display',
                                                 'pceval_serial_LCD_display','pceval_piano_keyboard','pceval_RTC_module_basic',
                                                 'pceval_button_RTC_timezone','pceval_multiple_timezone','pceval_alarm','pceval_dday_counter',
                                                 'pceval_clock','pceval_calendar_display','pceval_accelerometer','pceval_gyroscope',
                                                 'pceval_exercise_counter'):
            frozen=next(r for r in archives if r['group']=='direct_gpt61' and r['case']==case)
            source_group='direct_gpt61'
        if not frozen.get('source') or not Path(frozen['source']).is_file():
            raise NoGeneratedCode('No archived generated sketch exists; this task requires generating or implementing code before a detail repair can be tested.')
        source = Path(frozen['source'])
        assert workflow.digest(source)==frozen['source_sha256']
        original_cpp = source.parent.parent/'build/sketch/sketch.ino.cpp'
        plans=[]
        for name in ('decision_plans','repair_plans','boundary_plans','extension_plans',
                     'followup_plans','supplement_plans','additional_plans'):
            p=LEDGER.parent.parent/name/f'{case}.json'
            if p.exists():plans.append(p)
        tested=BASE/'behavior_all296_20261023/cases'/case/'tested_plan.json'
        native=BASE/'behavior_all296_20261023/embed_native'/case/'native_plan.json'
        if tested.exists():plans.append(tested)
        if native.exists():plans.append(native)
        recorded=LEDGER.parent.parent/'cases'/f'{case}.json'
        if recorded.exists():
            for run_record in read(recorded).get('runs',[]):
                p=Path(run_record.get('plan','_missing_'))
                if p.is_file() and p not in plans:plans.append(p)
        if not plans:
            for version in ('test_designs_v2','test_designs'):
                p=BASE/'behavior_all296_20261023'/version/case/'accepted.json'
                if p.is_file() and read(p).get('plan'):plans.append(p)
        if not plans:
            from manual_core_test_plans import make_plan
            authored=make_plan(case,dict(horizon_ms=10000,input_steps=[],checks=[]))
            if authored and authored.get('checks'):
                path=OUT/'authored_frozen_plans'/f'{case}.json'
                host.save(path,authored);plans.append(path)
            else:raise ValueError('No retained executable test plan: '+case)
        planfile=next((p for p in plans if read(p).get('plan',read(p)).get('checks')),plans[0])
    original = source.read_text(encoding='utf-8')
    spec=read(planfile)
    plan = spec.get('plan',spec)
    folder.mkdir(parents=True,exist_ok=True)
    (folder/'original.ino').write_text(original,encoding='utf-8')
    host.save(folder/'frozen_test_plan.json',plan)
    host.case_files(case,plan,metadata['fqbn'])
    before = host.execute(original_cpp, metadata['fqbn'],case,plan,folder/'before')
    try:
        revised, explanation = patch(case,original)
    except KeyError as error:
        if source_group=='workflow_267' or 'No reviewed detail correction yet' not in str(error):
            raise
        revised, explanation = original, 'Recover the archived generated candidate and verify it without source edits.'
    diff = ''.join(difflib.unified_diff(original.splitlines(True),revised.splitlines(True),fromfile='original.ino',tofile='repaired.ino'))
    (folder/'changes.diff').write_text(diff,encoding='utf-8')
    repair_origin='manual_direct_sketch_edit' if revised!=original else 'archived_candidate_verification'
    receipt = dict(case=case,repair_origin=repair_origin,source=str(source),
        source_group=source_group,
        source_sha256=workflow.digest(source),repaired_sha256=digest_text(revised),
        test_plan_source=str(planfile),test_plan_sha256=workflow.digest(planfile),
        explanation=explanation,code_changed=revised!=original,
        scope='Target Arduino compilation plus host API execution; no new SysML generation or SimulIDE evidence')
    host.save(folder/'repair_record.json',receipt)
    compiled, _ = workflow.compile_code(folder/'arduino',revised,workflow.CLI,metadata['fqbn'])
    result=dict(case=case,dataset=metadata['dataset'],before_status=before['status'],
        source_group=source_group,
        before_failed=[c['id'] for c in before.get('checks',[]) if not c['passed']],
        code_changed=revised!=original,explanation=explanation,compile_ok=compiled['compile_ok'],
        artifact=str(folder/'arduino/sketch/sketch.ino'),artifact_sha256=workflow.digest(folder/'arduino/sketch/sketch.ino'),
        artifact_text_sha256=digest_text(revised),
        test_coverage=plan.get('coverage','targeted_core_regression'),simulide_status='not_run')
    if compiled['compile_ok']:
        cpp=folder/'arduino/build/sketch/sketch.ino.cpp'
        after=host.execute(cpp,metadata['fqbn'],case,plan,folder/'after')
        result.update(after_status=after['status'],checks=len(after.get('checks',[])),
            failed=[c['id'] for c in after.get('checks',[]) if not c['passed']])
        extra=supplement(case,plan)
        if extra:
            host.save(folder/'supplement_plan.json',extra)
            host.case_files(case,extra,metadata['fqbn'])
            measured=host.execute(cpp,metadata['fqbn'],case,extra,folder/'supplement')
            result.update(supplement_status=measured['status'],supplement_checks=len(measured.get('checks',[])))
        result['status']='passed_existing_tests' if after['status']=='passed' and result.get('supplement_status','passed')=='passed' else 'needs_further_repair'
        if case=='simple_015' and result.get('supplement_status')=='passed':
            result.update(status='core_pass_test_precision_issue',
                adjudication='Original integer-prefix regex rejects correctly calibrated 24.93C and 17.60C readings. Original failures are retained; the independent numeric Celsius check passes. No rounding edit was made to satisfy the old regex.')
        if after['status']=='unverified_no_checks' and result.get('supplement_status')=='passed':
            result['status']='core_pass_supplement'
        issues={
            'medium_003':'The legacy suite injects already-converted Celsius as raw ADC counts. The separate LM35 5V ADC test verifies low/threshold/high temperatures and the correct A1 pin.',
            'embedbench_37':'The legacy test expects servo3 at zero, whereas the original explicitly permits clamping its 180..270 range at 180. Separate checks verify all servo mappings and single-digit endpoints.',
            'embedbench_41':'The legacy native assertions use an inconsistent time/count mapping. Separate requirements-derived full-core regression checks digits, servos and bar synchronization.',
            'embedbench_102':'The legacy native assertions use an inconsistent time/count mapping. Separate requirements-derived full-core regression checks all main synchronized outputs.',
            'simple_020':'The legacy trace uses an active-HIGH button variant. The disclosed ground-connected INPUT_PULLUP variant is tested separately; both original and revised test evidence are retained.'}
        if case in issues and after['status']!='passed' and result.get('supplement_status')=='passed':
            result.update(status='core_pass_with_retained_test_issue',adjudication=issues[case])
    else:
        result['status']='compile_failed'
    assert workflow.digest(source)==receipt['source_sha256'], 'Original source modified'
    assert workflow.digest(planfile)==receipt['test_plan_sha256'], 'Original test plan modified'
    host.save(folder/'result.json',result)
    return result


def main():
    global OUT
    parser=argparse.ArgumentParser()
    parser.add_argument('--case',action='append')
    parser.add_argument('--workers',type=int,default=1)
    parser.add_argument('--out',type=Path)
    parser.add_argument('--all',action='store_true')
    args=parser.parse_args()
    if args.out:
        OUT=args.out.resolve();OUT.relative_to(BASE.resolve())
    OUT.mkdir(parents=True,exist_ok=True)
    rows=read(LEDGER)
    selected=[r for r in rows if r['core_status'] in ('core_fail','compile_failed')]
    host.save(OUT/'historical_failure_inventory.json',dict(total=296,selected=len(selected),
        rows=selected,policy='Direct code repairs are reported separately from automatic workflow performance. No-firmware cases require implementation, not a claimed detail fix.'))
    host.OUT=OUT/'validation';host.FIXTURE=host.OUT/'fixture';host.fixture()
    os.environ['PATH']=str(workflow.CLI.parent)+os.pathsep+os.environ['PATH']
    def safe(case):
        try:
            result=run(case)
        except Exception as exc:
            status=('blocked_no_generated_code' if isinstance(exc,NoGeneratedCode) else
                    'needs_task_specific_repair' if isinstance(exc,KeyError) and 'No reviewed detail correction yet' in str(exc) else
                    'repair_infrastructure_error')
            previous=next(r for r in rows if r['case']==case)
            result=dict(case=case,dataset=previous['dataset'],status=status,error_type=type(exc).__name__,error=str(exc),
                        historical_failure_reason=previous.get('failure_reason'),simulide_status='not_run')
            before=OUT/'cases'/case/'before/result.json'
            if before.exists():result.update(before_status=read(before)['status'],before_evidence=str(before))
            host.save(OUT/'cases'/case/'result.json',result)
        return result
    cases=([r['case'] for r in selected] if args.all else args.case or
           ['pceval_distance_sensor','pceval_humidity_sensor_basic','pceval_7_segment_display_counter','complex_003'])
    with ThreadPoolExecutor(max_workers=args.workers) as pool:
      futures={pool.submit(safe,case):case for case in cases}
      for future in as_completed(futures):
        result=future.result()
        print(result['case'],result['status'],flush=True)
        results=[read(p) for p in (OUT/'cases').glob('*/result.json')]
        host.save(OUT/'summary.json',dict(historical_selected=len(selected),tested=len(results),
            pending_review=len(selected)-len(results),rows=results,updated=time.time()))


if __name__=='__main__':main()
