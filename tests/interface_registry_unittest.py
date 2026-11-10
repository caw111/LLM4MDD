import copy
import json
from pathlib import Path
import tempfile
import shutil
import subprocess
import unittest
from unittest.mock import patch

from app.services.hardware_adapter import validate_hardware_contract
from app.services.interface_planner import plan_interface_contract, requires_clock, ContractPlanningError
from app.services.interface_registry import assemble_interface_contract, register_contract, derive_registry
from app.services.behavior_contract import validate_sysml_contract


def proposal():
    return dict(control_period_ms=10,
        interface_requests=[dict(kind='analog_input', pin='A0', initial=0.0, meaning='Real sampled ADC')],
        serial_channels=[dict(baud=115200, trigger=dict(initial=False, meaning='Report every second'),
            values=[dict(type='f64', unit='adc_count', initial=0.0, meaning='Sampled ADC', label='ADC: ', required_inputs=['io_0'])])])


def legacy():
    data, _ = assemble_interface_contract(proposal())
    data['requirements'] = [dict(id='R1', text='Report ADC', assertion_ids=['report'])]
    data['interfaces'][1]['name'] = 'actualReportTrigger'
    data['interfaces'][2]['binding']['trigger'] = 'reportDue'
    return data


class RegistryTests(unittest.TestCase):
    def test_native_channels_have_only_program_owned_names_and_references(self):
        for index in range(100):
            draft = proposal()
            draft['serial_channels'] = [copy.deepcopy(draft['serial_channels'][0]) for _ in range(1 + index % 4)]
            for channel_index, group in enumerate(draft['serial_channels']):
                group['baud'] = 9600 + index * 300
                value=group['values'][0]
                group['values'] = [dict(value, label=f'case_{index}_channel_{channel_index}_value_{j}: ')
                                   for j in range(1 + index % 5)]
            data, actions = assemble_interface_contract(draft, board='arduino:avr:mega', require_clock=True)
            data['requirements'] = [dict(id='R1', text='Report ADC', assertion_ids=['report'])]
            validate_hardware_contract(data); validate_sysml_contract(data)
            data = register_contract(data)
            registry = data['interface_registry']
            self.assertEqual(registry, derive_registry(data))
            for i, channel in enumerate(registry['serial_channels']):
                self.assertEqual(channel['trigger_port'], f'serial_{i}_trigger')
                self.assertEqual(channel['value_ports'], [f'serial_{i}_value_{j}' for j in range(1 + index % 5)])
                self.assertEqual(registry['ports'][channel['trigger_port']], dict(direction='output', type='bool'))
            self.assertIn('required_elapsed_input_registered', actions)
            self.assertNotIn('interfaces', draft)

    def test_catalog_constructs_types_directions_units_and_sensor_sampling(self):
        requests = [dict(kind='digital_input', pin=2, initial=False), dict(kind='digital_output', pin=13, initial=True),
            dict(kind='analog_input', pin='A0', initial=0.0), dict(kind='pwm_output', pin=3, initial=0.0),
            dict(kind='elapsed_ms', initial=0), dict(kind='dht_temperature', pin=7, sensor='DHT22', initial=0.0),
            dict(kind='dht_humidity', pin=7, sensor='DHT22', initial=0.0), dict(kind='servo_angle', pin=2, initial=90.0),
            dict(kind='tone_output', pin=6, initial=0), dict(kind='tm1637_number', clk_pin=4, dio_pin=5, initial=0)]
        for request in requests:
            request['meaning'] = 'declared physical operation'
            with self.subTest(kind=request['kind']):
                data, _ = assemble_interface_contract(dict(interface_requests=[request], control_period_ms=7))
                data['requirements'] = [dict(id='R1', text='fixture', assertion_ids=['fixture'])]
                validate_hardware_contract(data); validate_sysml_contract(data)
                p = data['interfaces'][0]
                if request['kind'].startswith('dht_'):
                    self.assertEqual(p['type'], 'f64'); self.assertEqual(p['sample_period_ms'], 2002)
                if request['kind'] == 'elapsed_ms':
                    self.assertEqual(p['binding']['semantics'], 'delta_since_previous_step')

    def test_remote_sensor_and_display_drivers_use_native_registered_requests(self):
        temperature = dict(kind='dht_temperature', pin=7, sensor='DHT22', initial=0.0, meaning='Temperature')
        valid = dict(kind='dht_valid', pin=7, sensor='DHT22', initial=False, meaning='Valid DHT frame')
        button = dict(kind='digital_input', pin=2, initial=False, meaning='Independent button')
        ultrasonic = [dict(kind=kind, trigger_pin=8, echo_pin=9, timeout_us=60000,
                           initial=initial, meaning='Distance frame')
                      for kind, initial in [('ultrasonic_distance', 0.0), ('ultrasonic_valid', False)]]
        fixture = json.loads(Path('tools/examples/shift595_counter.contract.json').read_text(encoding='utf-8'))
        display = next(p for p in fixture['interfaces'] if p['binding']['kind'] == 'shift595_number')
        display_request = dict(display['binding'], initial=display['initial'], meaning=display['meaning'], unit=display['unit'])
        for requests in ([temperature, valid, button], ultrasonic, [display_request]):
            with self.subTest(kinds=[r['kind'] for r in requests]):
                data, _ = assemble_interface_contract(dict(interface_requests=requests, control_period_ms=7))
                data['requirements'] = [dict(id='R1', text='Native driver inventory', assertion_ids=['driver'])]
                validate_hardware_contract(data); validate_sysml_contract(data)
                self.assertEqual(register_contract(data)['interface_registry'], derive_registry(data))
                if requests is ultrasonic:
                    self.assertEqual([p['sample_period_ms'] for p in data['interfaces']], [63, 63])
                if requests[0]['kind'] == 'dht_temperature':
                    self.assertEqual([p['sample_period_ms'] for p in data['interfaces']], [2002, 2002, 7])
        data, _ = assemble_interface_contract(dict(interface_requests=[ultrasonic[0]], control_period_ms=7))
        data['requirements'] = [dict(id='R1', text='Incomplete distance frame', assertion_ids=['driver'])]
        with self.assertRaises(ValueError): validate_hardware_contract(data)

    def test_led_bar_graph_expands_to_each_real_wired_pin(self):
        data, _ = assemble_interface_contract(dict(interface_requests=[dict(kind='led_bar_graph',
            pins=list(range(2, 12)), initial=[True]*6+[False]*4, meaning='LED segments')]))
        data['requirements'] = [dict(id='R1', text='threshold six', assertion_ids=['six'])]
        validate_hardware_contract(data)
        self.assertEqual([p['name'] for p in data['interfaces']], [f'io_0_segment_{i}' for i in range(10)])
        self.assertEqual([p['binding']['pin'] for p in data['interfaces']], list(range(2, 12)))
        self.assertEqual([p['initial'] for p in data['interfaces']], [True]*6+[False]*4)

    def test_planner_registers_source_period_and_never_exposes_old_template_name(self):
        prompts=[]
        def call(prompt):
            prompts.append(prompt)
            return 'fixture', json.dumps(proposal())
        data, report=plan_interface_contract(call, 'Every second report A0 ADC value', {}, max_repairs=0)
        self.assertNotIn('trigger:"reportDue"', prompts[0])
        self.assertIn('Do NOT generate interfaces', prompts[0])
        self.assertIn('serial_0_trigger', json.dumps(data['interface_registry']))
        self.assertEqual(report['registered_relationships'], 'verified')
        self.assertEqual(data['requirements'][0]['text'], 'Every second report A0 ADC value')
        self.assertTrue(requires_clock('在 TM1637 每秒递增显示一个数字'))
        self.assertFalse(requires_clock('每次按下按钮递增显示一个数字'))

    def test_legacy_template_alias_is_repaired_only_if_unambiguous(self):
        original = legacy()
        data, actions = assemble_interface_contract(original)
        self.assertEqual(data['interfaces'][2]['binding']['trigger'], 'actualReportTrigger')
        validate_hardware_contract(data)
        self.assertEqual(original['interfaces'][2]['binding']['trigger'], 'reportDue')
        self.assertTrue(actions)
        for kind in ('second_trigger', 'unknown_reference'):
            bad=legacy()
            if kind=='second_trigger':
                bad['interfaces'].append(dict(copy.deepcopy(bad['interfaces'][1]), name='otherTrigger'))
            else:bad['interfaces'][2]['binding']['trigger']='notRegistered'
            candidate, _=assemble_interface_contract(bad)
            with self.assertRaisesRegex(ValueError, 'Boolean model output trigger'):
                validate_hardware_contract(candidate)

    def test_registry_tampering_and_new_free_names_are_rejected(self):
        data, _=assemble_interface_contract(proposal()); data['requirements']=[dict(id='R1', text='report', assertion_ids=['report'])]
        data=register_contract(data)
        for changed in ('registry', 'binding'):
            bad=copy.deepcopy(data)
            if changed=='registry':bad['interface_registry']['serial_channels'][0]['trigger_port']='invented'
            else:bad['interfaces'][2]['binding']['trigger']='invented'
            with self.subTest(changed=changed), self.assertRaisesRegex(ValueError,'interface_registry differs'):
                validate_hardware_contract(bad)
        bad=proposal();bad['serial_channels'][0]['trigger']['name']='invented'
        with self.assertRaisesRegex(ValueError,'serial_channels'):
            assemble_interface_contract(bad)

    def test_capability_gaps_conflicts_and_missing_outputs_are_not_fake_success(self):
        for draft, req in [(dict(interface_requests=[dict(kind='hc_sr04', initial=0, meaning='distance')]), 'D8 D9 distance'),
                           (dict(interface_requests=[dict(kind='analog_input', pin='A0', initial=0.0, meaning='sample')]), 'A0 sensor')]:
            with self.assertRaises(ContractPlanningError) as caught:
                plan_interface_contract(lambda _:('fixture',json.dumps(draft)), req, {}, max_repairs=0)
            self.assertEqual(caught.exception.blocking_category,'hardware_capability')
        bad=proposal();bad['interface_requests']=[dict(kind='digital_output',pin=5,initial=False,meaning='LED'),
            dict(kind='tm1637_number',clk_pin=4,dio_pin=5,initial=0,meaning='display')]
        bad.pop('serial_channels')
        with self.assertRaises(ContractPlanningError) as caught:
            plan_interface_contract(lambda _:('fixture',json.dumps(bad)), 'D5 LED TM1637 CLK=D4 DIO=D5', {}, max_repairs=0, resolve_defaults=False)
        self.assertIn('Conflicting hardware',caught.exception.attempts[0]['error'])

    def test_api_saves_an_isolated_registry_for_each_preparation(self):
        from fastapi.testclient import TestClient
        from app.main import app
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
            requests=[]
            for i in range(2):
                path=Path(folder)/'contract.json'
                response=client.post('/generate/interface-contract',json=dict(requirement='Every second report A0 ADC value',
                    interface_contract=proposal(),out_path=str(path)))
                self.assertEqual(response.status_code,200,response.text)
                result=response.json();requests.append(result)
                registry=json.loads(Path(result['registry_paths']['interface_registry']).read_text(encoding='utf-8'))
                self.assertEqual(registry,result['interface_contract']['interface_registry'])
                serial=json.loads(Path(result['registry_paths']['serial_registry']).read_text(encoding='utf-8'))
                self.assertEqual(serial['serial_channels'],registry['serial_channels'])
            self.assertNotEqual(requests[0]['registry_paths'],requests[1]['registry_paths'])
            self.assertEqual(requests[0]['contract_sha256'],requests[1]['contract_sha256'])
            mega=client.post('/generate/interface-contract',json=dict(requirement='On Arduino Mega every second report A0 ADC value',
                interface_contract=proposal(),out_path=str(Path(folder)/'mega.json')))
            self.assertEqual(mega.status_code,200,mega.text)
            self.assertEqual(mega.json()['interface_contract']['board'],'arduino:avr:mega')

    def test_registry_remains_authoritative_in_model_repair_prompts(self):
        from app.services.sysml import generate_validated_bundle, SysMLValidationError
        data, _=assemble_interface_contract(proposal())
        data['requirements']=[dict(id='R1',text='Report ADC',assertion_ids=['report'])]
        data=register_contract(data)
        prompts=[]
        def call(prompt):
            prompts.append(prompt)
            return 'fixture','{}'
        with self.assertRaises(SysMLValidationError):
            generate_validated_bundle(call,'Report ADC',contract=data,max_repairs=1)
        self.assertEqual(len(prompts),2)
        for prompt in prompts:
            self.assertIn('Application-owned interface registry',prompt)
            self.assertIn('never invent or rename serial ports',prompt)
            self.assertIn('"trigger_port": "serial_0_trigger"',prompt)

    def test_malformed_clock_and_output_sampling_are_rejected(self):
        with self.assertRaisesRegex(ValueError,'control_period_ms'):
            assemble_interface_contract(dict(interfaces=[]),require_clock=True)
        with self.assertRaisesRegex(ValueError,'sample_period_ms'):
            assemble_interface_contract(dict(interface_requests=[dict(kind='digital_output',pin=13,
                initial=False,meaning='LED',sample_period_ms=100)],control_period_ms=10))
        from fastapi.testclient import TestClient
        from app.main import app
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
            response=client.post('/generate/interface-contract',json=dict(requirement='Measure distance',
                interface_contract=dict(interface_requests=[dict(kind='hc_sr04',initial=0,meaning='distance')]),
                out_path=str(Path(folder)/'contract.json')))
            self.assertEqual(response.status_code,422)
            self.assertEqual(response.json()['detail']['blocking_category'],'hardware_capability')
            self.assertFalse((Path(folder)/'contract.json').exists())
            self.assertFalse((Path(folder)/'interface_registries').exists())

    def test_two_channels_through_sysml_ir_c_adapter_and_real_board_compilers(self):
        for board in ('arduino:avr:uno','arduino:avr:mega'):
            with self.subTest(board=board):
                self.verify_two_channels(board)

    def verify_two_channels(self, board):
        from fastapi.testclient import TestClient
        from app.main import app
        from app.services.sysml_assembly import assemble_sysml
        from hardware_contract_unittest import ARDUINO
        draft=proposal()
        draft['serial_channels'].append(dict(baud=115200,
            trigger=dict(initial=False,meaning='Report every two seconds'),
            values=[dict(type='f64',unit='%',initial=0.0,meaning='ADC percentage',label='Percent: ',required_inputs=['io_0'])]))
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client, patch('app.main._call_llm',side_effect=AssertionError('No LLM call')):
            base=Path(folder)
            prepared=client.post('/generate/interface-contract',json=dict(requirement='Every second report A0; every two seconds report A0 percentage',
                board=board,interface_contract=draft,out_path=str(base/'contract.json')))
            self.assertEqual(prepared.status_code,200,prepared.text)
            data=prepared.json()['interface_contract']
            controller=dict(states=[],updates=[],components=[dict(kind='periodic_pulse',name=name,
                delta='elapsedMilliseconds',enabled='true',period_ms=period,pulse_ms=10,first_pulse='after_period',pause='preserve')
                for name,period in [('fast',1000),('slow',2000)]],outputs=dict(
                serial_0_trigger='component_fast_value',serial_0_value_0='io_0',
                serial_1_trigger='component_slow_value',serial_1_value_0='io_0 * 100.0 / 1023.0'))
            source=assemble_sysml(controller,data)
            compiled=client.post('/compile/sysml2',json=dict(system_sysml2=source,interface_contract=data,out_dir=str(base/'ir')))
            self.assertEqual(compiled.status_code,200,compiled.text)
            generated=client.post('/convert/il-to-c',json=dict(interface_contract=data,
                provenance=compiled.json()['contents']['provenance'],il_file_path=compiled.json()['files']['il'],out_c_dir=str(base/'c')))
            self.assertEqual(generated.status_code,200,generated.text)
            contents=generated.json()['contents']
            adapted=client.post('/convert/c-to-arduino',json=dict(provenance=generated.json()['provenance'],
                c_code=contents['ProgramPage1.c'],interface_contract=data,
                compiled_kernel=dict(header=contents['define.h'],source=contents['ProgramPage1.c']),out_path=str(base/'serial.ino')))
            self.assertEqual(adapted.status_code,200,adapted.text)
            code=Path(adapted.json()['saved_path']).read_text(encoding='utf-8')
            (base/'Arduino.h').write_text(ARDUINO,encoding='utf-8')
            harness='''
#include <cassert>
int main(){setup();reading=512;loop();assert(Serial.values.empty());
 now=500;loop();assert(Serial.values.empty());
 now=1000;loop();assert(Serial.values.size()==1 && Serial.values[0]==512);
 now=1010;loop();now=1500;loop();assert(Serial.values.size()==1);
 reading=1023;now=2000;loop();assert(Serial.values.size()==3);
 assert(Serial.values[1]==1023 && Serial.values[2]==100);
 now=2010;loop();now=3000;loop();assert(Serial.values.size()==4 && Serial.values[3]==1023);
 now=3010;loop();assert(Serial.values.size()==4);}
'''
            test=base/'test.cpp';test.write_text(code+harness,encoding='utf-8')
            binary=base/'test.exe'
            built=subprocess.run([shutil.which('g++'),'-std=c++17','-I',str(base),str(test),'-o',str(binary)],capture_output=True,text=True)
            self.assertEqual(built.returncode,0,built.stderr)
            run=subprocess.run([str(binary)],capture_output=True,text=True,timeout=10)
            self.assertEqual(run.returncode,0,run.stderr)
            result=client.post('/arduino/compile',json=dict(sketch_path=adapted.json()['saved_path'],fqbn=board))
            self.assertEqual(result.status_code,200,result.text)
            self.assertTrue(result.json()['success'],result.text)


if __name__=='__main__':unittest.main()
