import copy
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'experiment/code'))
from app.services.interface_registry import assemble_interface_contract, register_contract
from app.services.hardware_adapter import generate_hardware_adapter, validate_hardware_contract
from app.services.extended_devices import format_parts
import behavior_full_host as host
import run_behavior_pilot10 as pilot
import run_workflow267_host_smoke as base


ROOT = Path(__file__).resolve().parents[1]


def contract(requests, channels=()):
    requests = [dict(r, initial=r.get('initial', False if r['kind'].endswith('_valid') else 0),
                     meaning='Required native device operation') for r in requests]
    value, _ = assemble_interface_contract(dict(control_period_ms=5, interface_requests=requests, serial_channels=list(channels)))
    value['requirements'] = [dict(id='R1', text='Native device transport', assertion_ids=['native'])]
    return register_contract(value)


def bus(kind, **fields):
    return dict(kind=kind, sda_pin='A4', scl_pin='A5', **fields)


def lcd(kind, **fields):
    return dict(kind=kind, cs_pin=10, dc_pin=9, rst_pin=8, mosi_pin=11, miso_pin=12, sck_pin=13, **fields)


def kernel(value, body):
    types={'bool':'bool','i32':'int32_t','f64':'double'}
    args=', '.join(types[p['type']]+('* ' if p['direction']=='output' else ' ')+p['name'] for p in value['interfaces'])
    header='void ProgramPage1('+args+');'
    return header, '#include <Arduino.h>\nint ProgramPage1_instance;void ProgramPage1_Init(int*){}\n'+header[:-1]+'{'+body+'}\n'


class ExtendedDeviceTests(unittest.TestCase):
    def test_multiplex_observer_supports_cathode_and_marks_unknown_scans_unverified(self):
        trace=[]
        for t in range(1,81):
            digit=(t-1)%4
            for i,p in enumerate((2,3,4,5)):
                trace.append(dict(type='gpio',at_ms=t,pin=p,value=int(i==digit),pwm=False))
            for i,p in enumerate(range(6,13)):
                trace.append(dict(type='gpio',at_ms=t,pin=p,value=((6,91,79,102)[digit]>>i)&1,pwm=False))
        trace.append(dict(type='execution',virtual_ms=100,status='completed',setup_returned=True,setup_at_ms=0))
        check=dict(kind='multiplex_display',id='digits',requirement_quote='display',digit_pins=[2,3,4,5],
                   segment_pins=list(range(6,13)),from_ms=1,to_ms=80,expected='1234',common_anode=False)
        plan=dict(horizon_ms=100,checks=[check])
        self.assertEqual(host.evaluate(plan,trace,'arduino:avr:uno')['status'],'passed')
        check['expected']='5678'
        self.assertEqual(host.evaluate(plan,trace,'arduino:avr:uno')['status'],'failed')
        for e in trace:
            if e['type']=='gpio' and e['pin']>=6:e['value']=0
        result=host.evaluate(plan,trace,'arduino:avr:uno')
        self.assertEqual(result['checks'][0]['unverified'],'multiplex_scan_not_decoded_reliably')

    def test_indexed_gpio_observation_retains_last_write_at_the_same_timestamp(self):
        trace=[dict(type='gpio',at_ms=t,pin=13,value=v,pwm=False) for t,v in [(0,0),(10,1),(10,0),(20,1)]]
        trace.append(dict(type='execution',virtual_ms=100,status='completed',setup_returned=True,setup_at_ms=0))
        plan=dict(horizon_ms=100,checks=[dict(kind='gpio',id=str(t),requirement_quote='state',pin=13,
                  at_ms=t,expected=v) for t,v in [(5,0),(10,0),(19,0),(20,1),(99,1)]])
        self.assertEqual(host.evaluate(plan,trace,'arduino:avr:uno')['status'],'passed')

    def test_explicit_serial_input_accepts_all_registered_receiver_types(self):
        from app.services.interface_planner import check_explicit_requirements
        for request in (dict(kind='serial_datetime_input',baud=9600),
                        dict(kind='serial_string_input',baud=9600),
                        dict(kind='serial_packet_input',baud=9600,commands={'NOW':1},min=0,max=1439)):
            with self.subTest(kind=request['kind']):
                value=contract([request])
                check_explicit_requirements(value,'Serial input at 9600 baud',{})

    def test_unobserved_future_output_is_not_inferred_from_last_captured_value(self):
        plan=dict(horizon_ms=10000,checks=[dict(kind='gpio',id='future',requirement_quote='future output',
                                              pin=13,at_ms=9000,expected=1)])
        trace=[dict(type='gpio',at_ms=100,pin=13,value=1,pwm=False),
               dict(type='execution',virtual_ms=5000,status='capture-end',setup_returned=True,setup_at_ms=0)]
        result=host.evaluate(plan,trace,'arduino:avr:uno')
        self.assertEqual(result['status'],'unverified_window')
        self.assertEqual(result['checks'][0]['unverified'],'requested_window_not_observed')

    def test_contract_rejects_wrong_bus_and_arbitrary_format_code(self):
        value=contract([bus('rtc_epoch'),bus('rtc_valid')])
        validate_hardware_contract(value)
        for pin in ('A0',13):
            bad=copy.deepcopy(value);bad['interfaces'][0]['binding']['sda_pin']=pin
            with self.assertRaises(ValueError):validate_hardware_contract(bad)
        for text in ('{0.__class__}','{1}','{0!r}','{0:%n}','{x}'):
            with self.subTest(text=text), self.assertRaises(ValueError):format_parts(text,1)

    def test_frozen_requirement_text_is_restored_without_a_model_echo(self):
        from app.services.sysml import generate_validated_bundle
        from app.services.sysml_compiler import compile_sysml
        value=contract([dict(kind='digital_output',pin=13,initial=False)])
        response=dict(controller=dict(states=[],updates=[],outputs=dict(io_0=False)),req_test_map=dict(
            requirements_to_testpoints=[dict(requirement_id='R1',test_point_id='native',test_method='trace',
                stimulus='startup',oracle='lamp initially off')]))
        def validator(source):
            return dict(ok=True,issues=[],ir=compile_sysml(source))
        _,accepted,_=generate_validated_bundle(lambda _:('fixture',json.dumps(response)), 'requirement',
            validator=validator,contract=value,max_repairs=0)
        self.assertEqual(accepted['req_test_map']['requirements_to_testpoints'][0]['requirement_text'],
                         value['requirements'][0]['text'])

    def test_native_boundary_contains_lcd_setup_and_serial_format_responsibilities(self):
        from app.services.interface_semantics import adapter_boundary_facts
        value=contract([lcd('lcd_number')],[dict(baud=9600,trigger=dict(initial=False,meaning='report'),
            values=[dict(type='i32',unit='s',initial=0,meaning='epoch',label='')],format='{0:iso}')])
        facts=adapter_boundary_facts(value)
        self.assertTrue(any(f['kind']=='native_device_adapter' and 'setRotation' in f['lcd_semantics'] for f in facts))
        self.assertTrue(any(f['kind']=='formatted_serial_report' and f['format']=='{0:iso}' for f in facts))
        self.assertTrue(any(f['kind']=='serial_adapter_transport' and f['baud']==9600 for f in facts))

    def test_native_acquisition_formatting_and_lcd_execute_on_changing_inputs(self):
        value=contract([bus('rtc_epoch'),bus('rtc_valid'),bus('mpu_acceleration',axis='y',address=105),bus('mpu_valid',address=105),
                        lcd('lcd_number',format='Y: {0:.2f}')],
            [dict(baud=9600,trigger=dict(initial=False,meaning='Valid acquired frame'),
                  values=[dict(type='i32',unit='s',initial=0,meaning='RTC epoch',label='')],format='{0:iso}')])
        header,code=kernel(value,'*io_4=io_2;*serial_0_trigger=io_1 && io_3;*serial_0_value_0=io_0;')
        code+=generate_hardware_adapter(header,value)
        plan=dict(horizon_ms=1600,rtc_epoch_s=1732423680,input_steps=[],serial_inputs=[],checks=[
            dict(kind='lcd',id='first',requirement_quote='native',from_ms=0,to_ms=700,contains='Y: 2.50'),
            dict(kind='lcd',id='second',requirement_quote='native',from_ms=800,to_ms=1500,contains='Y: -4.25'),
            dict(kind='serial',id='rtc',requirement_quote='native',from_ms=0,to_ms=1500,contains='2024-11-24T04:48:')],
            imu_samples=[dict(at_ms=0,acceleration=[0,2.5,0],gyro=[0,0,0],raw_y=0),
                         dict(at_ms=800,acceleration=[0,-4.25,0],gyro=[0,0,0],raw_y=0)])
        self.execute(value,code,plan)
        with tempfile.TemporaryDirectory() as folder:
            from run_controlled_experiment import compile_code
            result,log=compile_code(Path(folder),code,ROOT/'.tools/arduino/arduino-cli.exe','arduino:avr:uno')
            self.assertTrue(result['compile_ok'],log)

    def test_string_receiver_preserves_arbitrary_frames_and_rejects_overflow(self):
        value=contract([dict(kind='serial_string_input',baud=9600),lcd('lcd_text',source_input='io_0'),
                        dict(kind='serial_echo',baud=9600,source_input='io_0')])
        header,code=kernel(value,'*io_1=io_0;*io_2=io_0;');code+=generate_hardware_adapter(header,value)
        plan=dict(horizon_ms=2200,input_steps=[],serial_inputs=[dict(at_ms=200,text='  arbitrary first  \n'),
            dict(at_ms=800,text='x'*80+'\n'),dict(at_ms=1500,text='other frame\n')],checks=[
            dict(kind='lcd',id='first',requirement_quote='frame',from_ms=200,to_ms=700,contains='arbitrary first'),
            dict(kind='serial',id='whitespace',requirement_quote='frame',from_ms=200,to_ms=700,
                 regex='^  arbitrary first  $',min_count=1,max_count=1),
            dict(kind='serial',id='second',requirement_quote='frame',from_ms=1500,to_ms=2000,contains='other frame'),
            dict(kind='serial',id='overflow',requirement_quote='frame',from_ms=800,to_ms=1400,contains='xxxx',min_count=0,max_count=0)])
        self.execute(value,code,plan)

    def test_datetime_receiver_rejects_invalid_calendar_dates(self):
        value=contract([dict(kind='serial_datetime_input',baud=9600)],
            [dict(baud=9600,trigger=dict(initial=False,meaning='Accepted date'),values=[
                dict(type='i32',unit='s',initial=0,meaning='Parsed Unix seconds',label='')],format='{0:iso}')])
        header,code=kernel(value,'*serial_0_trigger=io_0_valid;*serial_0_value_0=io_0;');code+=generate_hardware_adapter(header,value)
        plan=dict(horizon_ms=1600,input_steps=[],serial_inputs=[dict(at_ms=100,text='2025-02-29T12:00:00\n'),
            dict(at_ms=400,text='2024-02-29T12:00:00\n'),dict(at_ms=800,text='2038-01-01T00:00:00\n')],checks=[
            dict(kind='serial',id='invalid_leap',requirement_quote='calendar',from_ms=100,to_ms=350,contains='2025',min_count=0,max_count=0),
            dict(kind='serial',id='valid_leap',requirement_quote='calendar',from_ms=400,to_ms=750,contains='2024-02-29T12:00:00',min_count=1,max_count=1),
            dict(kind='serial',id='out_of_contract',requirement_quote='calendar',from_ms=800,to_ms=1500,contains='2038',min_count=0,max_count=0)])
        self.execute(value,code,plan)

    def test_setup_only_rtc_acquisition_is_not_resampled_by_loop(self):
        value=contract([bus('rtc_epoch',acquisition='once'),bus('rtc_valid',acquisition='once')],
            [dict(baud=9600,trigger=dict(initial=False,meaning='Valid captured RTC'),values=[
                dict(type='i32',unit='s',initial=0,meaning='Captured seconds',label='')],format='{0:iso}')])
        header,code=kernel(value,'*serial_0_trigger=io_1;*serial_0_value_0=io_0;');code+=generate_hardware_adapter(header,value)
        plan=dict(horizon_ms=2500,rtc_epoch_s=1732423680,input_steps=[],checks=[
            dict(kind='serial',id='captured',requirement_quote='Read once',from_ms=1500,to_ms=2300,
                 contains='2024-11-24T04:48:00',min_count=1),
            dict(kind='serial',id='not_resampled',requirement_quote='Read once',from_ms=1500,to_ms=2300,
                 regex='2024-11-24T04:48:0[12]',min_count=0,max_count=0)])
        self.execute(value,code,plan)

    def test_exclusive_tones_switch_between_physical_pins(self):
        value=contract([dict(kind='digital_input',pin=2,initial=False),
                        dict(kind='tone_output',pin=9,channel_mode='exclusive'),dict(kind='tone_output',pin=10,channel_mode='exclusive')])
        header,code=kernel(value,'*io_1=io_0?262:0;*io_2=io_0?0:294;');code+=generate_hardware_adapter(header,value)
        plan=dict(horizon_ms=1200,input_steps=[dict(at_ms=0,kind='digital',pin=2,value=0),dict(at_ms=500,kind='digital',pin=2,value=1)],checks=[
            dict(kind='tone',id='first',requirement_quote='tone',pin=10,at_ms=300,expected=294),
            dict(kind='tone',id='second',requirement_quote='tone',pin=9,at_ms=800,expected=262),
            dict(kind='tone',id='off',requirement_quote='tone',pin=10,at_ms=800,expected=0)])
        self.execute(value,code,plan)

    def test_four_digit_scan_outputs_the_whole_number(self):
        value=contract([dict(kind='seven_segment_number',segment_pins=list(range(6,13)),digit_pins=[2,3,4,5],
                             common_anode=True,digit_active_high=True,leading_zero=True)])
        header,code=kernel(value,'*io_0=millis()<500?1234:5678;');code+=generate_hardware_adapter(header,value)
        plan=dict(horizon_ms=1200,input_steps=[],checks=[dict(kind='multiplex_display',id='first',
            requirement_quote='four digits',digit_pins=[2,3,4,5],segment_pins=list(range(6,13)),
            from_ms=100,to_ms=400,expected='1234',common_anode=True),dict(kind='multiplex_display',id='second',
            requirement_quote='four digits',digit_pins=[2,3,4,5],segment_pins=list(range(6,13)),
            from_ms=650,to_ms=1050,expected='5678',common_anode=True)])
        self.execute(value,code,plan)

    def test_packet_receiver_preserves_command_and_payload_without_matching_examples(self):
        value=contract([dict(kind='serial_packet_input',baud=9600,commands={'TIME':1,'SET1':2},min=0,max=2359)],
            [dict(baud=9600,trigger=dict(initial=False,meaning='accepted frame'),values=[
                dict(type='i32',unit='1',initial=0,meaning='packed frame',label='')],format='{0:d}')])
        header,code=kernel(value,'*serial_0_trigger=io_0_valid;*serial_0_value_0=io_0;');code+=generate_hardware_adapter(header,value)
        plan=dict(horizon_ms=1600,input_steps=[],serial_inputs=[dict(at_ms=100,text='SET1 2359\n'),
            dict(at_ms=500,text='SET1 2360\n'),dict(at_ms=900,text='OTHER 12\n')],checks=[
            dict(kind='serial',id='valid',requirement_quote='packet',from_ms=100,to_ms=450,regex='^22359$',min_count=1,max_count=1),
            dict(kind='serial',id='invalid',requirement_quote='packet',from_ms=500,to_ms=1400,regex='.*',min_count=0,max_count=0)])
        self.execute(value,code,plan)

    def test_tm1637_text_scroll_and_retained_word_have_actual_segment_evidence(self):
        value=contract([dict(kind='tm1637_text',clk_pin=4,dio_pin=5,initial=-1,
                             messages={'-1':'OCCUPIED','-2':'FREE'},scroll_ms=300)])
        header,code=kernel(value,'*io_0=millis()<1500?-1:-2;');code+=generate_hardware_adapter(header,value)
        plan=dict(horizon_ms=2400,input_steps=[],checks=[
            dict(kind='segment_frame',id='occupied',requirement_quote='occupied',name='tm:4:5',
                 from_ms=0,to_ms=200,segments=[63,57,57,62]),
            dict(kind='segment_frame',id='scroll',requirement_quote='occupied',name='tm:4:5',
                 from_ms=250,to_ms=650,segments=[57,57,62,115]),
            dict(kind='segment_frame',id='free',requirement_quote='free',name='tm:4:5',
                 from_ms=1500,to_ms=2200,segments=[113,80,121,121])])
        self.execute(value,code,plan)

    def execute(self,value,code,plan):
        with tempfile.TemporaryDirectory() as folder:
            folder=Path(folder);old=(host.OUT,host.FIXTURE,pilot.OUT,base.GXX)
            try:
                host.OUT=folder;host.FIXTURE=folder/'fixture';pilot.OUT=ROOT/'experiment/results/host_templates'
                base.GXX=Path(shutil.which('g++'));host.fixture();host.case_files('native',plan,value.get('board','arduino:avr:uno'))
                source=folder/'native.cpp';source.write_text(code,encoding='utf8')
                result=host.execute(source,value.get('board','arduino:avr:uno'),'native',plan,folder/'run')
                details=result
                if result['status']=='unverified_host_build':details=(folder/'run/build.stderr.txt').read_text(errors='replace')
                self.assertEqual(result['status'],'passed',details)
            finally:host.OUT,host.FIXTURE,pilot.OUT,base.GXX=old


if __name__=='__main__':unittest.main()
