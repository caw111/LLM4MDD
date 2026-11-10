import copy
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

from app.services.interface_registry import assemble_interface_contract, normalize_interface_proposal, register_contract
from app.services.interface_planner import plan_interface_contract, prepare_interface_contract, ContractPlanningError
from app.services.hardware_adapter import validate_hardware_contract

ROOT = Path(__file__).resolve().parents[1]


def receive_proposal(command=True, board='arduino:avr:uno', framing='line_or_idle'):
    receiver = dict(kind='serial_command_input' if command else 'serial_integer_input',
                    initial=0, meaning='Received transport token/value', baud=9600, framing=framing)
    if command:
        receiver['commands'] = {'ON': 1, 'OFF': 2}
    else:
        receiver.update(min=0, max=255)
    return dict(board=board, control_period_ms=10, interface_requests=[receiver,
        dict(kind='digital_output' if command else 'pwm_output', pin=13 if command else 9,
             initial=False if command else 0, meaning='Actuator command')])


def prepared(draft):
    data, _ = assemble_interface_contract(draft)
    data['requirements'] = [dict(id='R1', text='Serial controls output', assertion_ids=['serial_trace'])]
    validate_hardware_contract(data)
    return register_contract(data)


class InterfaceHardeningTests(unittest.TestCase):
    def test_all_nine_observed_field_failures_are_accepted_without_another_model_call(self):
        sys.path.insert(0, str(ROOT/'experiment/code'))
        from run_contract_development import INTERPRETATIONS
        cases = {c['id']: c for c in json.loads((ROOT/'experiment/formal/manifest.json').read_text(encoding='utf-8'))['cases']}
        for cid in ('simple_004', 'simple_019', 'simple_027'):
            path = ROOT/'experiment/output/interface_registry_full_20261014'/cid/'repeat_1/contract_planning.json'
            original = path.read_bytes()
            for i, attempt in enumerate(json.loads(original)['attempts']):
                with self.subTest(case=cid, attempt=i):
                    req = cases[cid]['requirement'] + '\n' + INTERPRETATIONS.get(cid, '')
                    call = unittest.mock.Mock(return_value=('offline', attempt['response']))
                    contract, report = plan_interface_contract(call, req, {}, max_repairs=0, target_board=cases[cid]['fqbn'])
                    call.assert_called_once()
                    self.assertTrue(report['attempts'][0]['ok'])
                    self.assertTrue(report['attempts'][0]['construction'])
                    validate_hardware_contract(contract)
            self.assertEqual(path.read_bytes(), original)

    def test_equivalent_shapes_do_not_mutate_input_or_accept_conflicts(self):
        draft = dict(interface_requests=[dict(kind='digital_output', pin=13, initial=False, meaning='LED',
                                              optional={'active_high': 1}, type='bool', direction='output')])
        original = copy.deepcopy(draft)
        data, actions = assemble_interface_contract(draft)
        self.assertIs(data['interfaces'][0]['binding']['active_high'], True)
        self.assertTrue(actions)
        self.assertEqual(draft, original)
        for extra, error in [({'active_high': False}, 'conflicts'), ({'type': 'i32'}, 'catalog'),
                             ({'optional': {'evil': 1}}, 'unknown fields'), ({'raw_unit': 'volts'}, 'calibration')]:
            bad = copy.deepcopy(draft); bad['interface_requests'][0].update(extra)
            with self.subTest(extra=extra), self.assertRaisesRegex(ValueError, error):
                assemble_interface_contract(bad)

    def test_explicit_sampling_and_initial_values_are_preserved(self):
        draft = dict(interface_requests=[dict(kind='analog_input', pin='A0', initial=0, meaning='ADC', sample_period_ms=0),
                                        dict(kind='digital_output', pin=13, initial=False, meaning='LED')])
        with self.assertRaises(ContractPlanningError):
            plan_interface_contract(lambda _:('fixture', json.dumps(draft)), 'A0 sampling period 20ms drives D13', {}, max_repairs=0)
        draft['interface_requests'][0]['sample_period_ms'] = 20
        good, _ = plan_interface_contract(lambda _:('fixture', json.dumps(draft)), 'A0 sampling period 20ms drives D13', {}, max_repairs=0)
        self.assertEqual(good['interfaces'][0]['sample_period_ms'], 20)
        draft['interface_requests'][1]['initial'] = 0
        with self.assertRaisesRegex(ValueError, 'Initial value'):
            prepared(draft)

    def test_unique_source_pins_are_restored_and_contradictions_and_ambiguity_stay_errors(self):
        draft = dict(interface_requests=[dict(kind='analog_input', initial=0, meaning='ADC'),
                                        dict(kind='pwm_output', initial=0, meaning='PWM')])
        good, actions = prepare_interface_contract(draft, 'A0 potentiometer drives D9 PWM', {})
        self.assertEqual([p['binding']['pin'] for p in good['interfaces']], ['A0', 'D9'])
        self.assertEqual(len([a for a in actions if 'restored' in a]), 2)
        draft['interface_requests'][0]['pin'] = 'A1'
        with self.assertRaisesRegex(ValueError, 'confirmed wiring'):
            prepare_interface_contract(draft, 'A0 potentiometer drives D9 PWM', {})
        draft['interface_requests'][0].pop('pin')
        ambiguous, _ = prepare_interface_contract(draft, 'A0 and A1 potentiometers drive D9 PWM', {})
        self.assertNotIn('pin', ambiguous['interfaces'][0]['binding'])

    def test_circuit_roles_and_repair_snapshot_restore_confirmed_pins(self):
        circuit = dict(parts=[dict(id='uno', type='wokwi-arduino-uno'), dict(id='pot', type='wokwi-potentiometer')],
                       connections=[['pot:SIG', 'uno:A2']])
        draft = dict(interface_requests=[dict(kind='analog_input', initial=0, meaning='ADC')])
        data, _ = prepare_interface_contract(draft, json.dumps(circuit), {})
        self.assertEqual(data['interfaces'][0]['binding']['pin'], 'A2')
        data, _ = prepare_interface_contract(draft, 'Read confirmed potentiometer', {}, confirmed={'analog_input': {'pin': 'A3'}})
        self.assertEqual(data['interfaces'][0]['binding']['pin'], 'A3')

    def test_deadline_clock_removal_retains_other_ids_and_never_drops_dependencies(self):
        draft = dict(interface_requests=[dict(kind='elapsed_ms', initial=0, meaning='clock'),
            dict(kind='analog_input', pin='A0', initial=0, meaning='ADC'),
            dict(kind='pwm_output', pin=9, initial=0, meaning='PWM', required_inputs=['io_1'])])
        data, _ = prepare_interface_contract(draft, 'A0 drives D9, response within 50ms', {})
        self.assertEqual([p['name'] for p in data['interfaces']], ['io_1', 'io_2'])
        self.assertEqual(data['required_dependencies']['io_2'], ['io_1'])
        draft['interface_requests'][2]['required_inputs'].append('io_0')
        with self.assertRaisesRegex(ValueError, 'explicit dependencies'):
            prepare_interface_contract(draft, 'A0 drives D9, response within 50ms', {})

    def test_field_diagnostics_name_every_missing_and_unknown_key_on_the_object(self):
        with self.assertRaisesRegex(ValueError, "missing fields.*initial.*meaning.*unknown fields.*bogus.*other"):
            assemble_interface_contract(dict(interface_requests=[dict(kind='digital_output', bogus=1, other=2)]))
        with self.assertRaises(ValueError) as caught:
            assemble_interface_contract(dict(interface_requests=[dict(kind='analog_input', bogus=1), dict(kind='pwm_output', other=2)]))
        self.assertIn('interface_requests[0]', str(caught.exception))
        self.assertIn('interface_requests[1]', str(caught.exception))
        for kind in ({}, [], None):
            with self.subTest(kind=kind), self.assertRaisesRegex(ValueError, 'kind'):
                prepare_interface_contract(dict(interface_requests=[dict(kind=kind)]), 'D13 LED', {})

    def test_serial_registration_and_real_capability_conflicts(self):
        data = prepared(receive_proposal())
        rx = data['interface_registry']['serial_inputs'][0]
        self.assertEqual((rx['value_port'], rx['valid_port']), ('io_0', 'io_0_valid'))
        self.assertEqual(data['interface_registry']['ports']['io_0_valid'], dict(direction='input', type='bool'))
        for change, error in [('baud', 'baud'), ('bounds', 'bounds'), ('commands', 'ASCII'), ('gpio', 'D0/D1'),
                              ('multiple', 'one receive'), ('framing', 'framing'), ('valid', 'matching'), ('tx_baud', 'same baud')]:
            draft = receive_proposal(command=change != 'bounds')
            if change == 'baud': draft['interface_requests'][0]['baud'] = True
            if change == 'bounds': draft['interface_requests'][0].update(min=255, max=0)
            if change == 'commands': draft['interface_requests'][0]['commands'] = {'ON\n': 1}
            if change == 'gpio': draft['interface_requests'][1]['pin'] = 0
            if change == 'framing': draft['interface_requests'][0]['framing'] = 'anything'
            if change == 'multiple': draft['interface_requests'].append(copy.deepcopy(draft['interface_requests'][0]))
            if change == 'tx_baud': draft['serial_channels'] = [dict(baud=115200, trigger=dict(initial=False, meaning='report'),
                values=[dict(type='i32', unit='1', initial=0, meaning='received', label='value: ')])]
            with self.subTest(change=change), self.assertRaisesRegex(ValueError, error):
                candidate, _ = assemble_interface_contract(draft)
                candidate['requirements'] = data['requirements']
                if change == 'valid': candidate['interfaces'][1]['binding']['value'] = 'io_1'
                validate_hardware_contract(candidate)

    def test_serial_receive_cannot_be_silently_omitted_or_use_wrong_baud_or_tokens(self):
        req = 'Serial receive ON or OFF at 9600 baud controls D13'
        for mode in ('absent', 'baud', 'tokens'):
            draft = receive_proposal()
            if mode == 'absent': draft['interface_requests'].pop(0)
            if mode == 'baud': draft['interface_requests'][0]['baud'] = 115200
            if mode == 'tokens': draft['interface_requests'][0]['commands'].pop('OFF')
            with self.subTest(mode=mode), self.assertRaises(ContractPlanningError):
                plan_interface_contract(lambda _:('fixture', json.dumps(draft)), req, {}, max_repairs=0)

    def test_negated_serial_receive_does_not_invent_a_receiver(self):
        draft = dict(interface_requests=[dict(kind='digital_output', pin=13, initial=False, meaning='LED')])
        for req in ('D13 LED; no serial input is required.', 'D13 LED，串口无需接收。', 'D13 LED; Serial does not receive commands.'):
            with self.subTest(requirement=req):
                data, _ = plan_interface_contract(lambda _:('fixture', json.dumps(draft)), req, {}, max_repairs=0)
                self.assertNotIn('serial_inputs', data['interface_registry'])

    def test_supplied_api_and_planner_share_normalization_and_receiver_registry(self):
        from fastapi.testclient import TestClient
        from app.main import app
        draft = receive_proposal()
        draft['interface_requests'][1]['optional'] = {'active_high': 1}
        req = 'Serial receive ON or OFF at 9600 baud controls D13'
        planned, _ = plan_interface_contract(lambda _:('fixture', json.dumps(draft)), req, {}, max_repairs=0)
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client, patch('app.main._call_llm', side_effect=AssertionError('No model calls')):
            response = client.post('/generate/interface-contract', json=dict(requirement=req, interface_contract=draft,
                                    out_path=str(Path(folder)/'contract.json')))
            self.assertEqual(response.status_code, 200, response.text)
            self.assertEqual(response.json()['interface_contract'], planned)
            serial = json.loads(Path(response.json()['registry_paths']['serial_registry']).read_text(encoding='utf-8'))
            self.assertEqual(serial['serial_inputs'], planned['interface_registry']['serial_inputs'])

    def test_receiver_full_i32_limits_line_framing_and_idle_across_millis_wrap(self):
        from app.services.serial_receiver import receiver_code
        draft = receive_proposal(False, framing='line')
        draft['interface_requests'][0].update(min=-2147483648, max=2147483647)
        data = prepared(draft)
        lines, _ = receiver_code(data['interfaces'][0])
        checks = r'''
int main(){
 int32_t value=0;
 Serial.feed("-2147483648\n2147483647\n2147483648\n-2147483649\n");
 assert(hw_rx_poll(0,&value) && value==INT32_MIN);
 assert(hw_rx_poll(10,&value) && value==INT32_MAX);
 assert(!hw_rx_poll(20,&value) && value==INT32_MAX);
 assert(!hw_rx_poll(30,&value) && value==INT32_MAX);
 Serial.feed("42");assert(!hw_rx_poll(40,&value));assert(!hw_rx_poll(1000,&value));
 Serial.feed("\n");assert(hw_rx_poll(1010,&value) && value==42);
}'''
        with tempfile.TemporaryDirectory() as folder:
            self.compile_parser(Path(folder), lines+checks)
        data['interfaces'][0]['binding']['framing'] = 'line_or_idle'
        lines, _ = receiver_code(data['interfaces'][0])
        checks = r'''
int main(){
 int32_t value=0;Serial.feed("-5");assert(!hw_rx_poll(4294967290U,&value));
 assert(!hw_rx_poll(5U,&value));assert(hw_rx_poll(14U,&value) && value==-5);
 assert(!hw_rx_poll(24U,&value));
}'''
        with tempfile.TemporaryDirectory() as folder:
            self.compile_parser(Path(folder), lines+checks)

    def compile_parser(self, base, code):
        (base/'Arduino.h').write_text(ARDUINO, encoding='utf-8')
        cpp, binary = base/'parser.cpp', base/'parser.exe'
        cpp.write_text('#include <Arduino.h>\n#include <cassert>\n'+code, encoding='utf-8')
        build = subprocess.run([shutil.which('g++'), '-std=c++17', '-I', str(base), str(cpp), '-o', str(binary)], capture_output=True, text=True)
        self.assertEqual(build.returncode, 0, build.stderr)
        run = subprocess.run([str(binary)], capture_output=True, text=True, timeout=10)
        self.assertEqual(run.returncode, 0, run.stderr)

    def test_serial_command_and_integer_through_sysml_ir_c_host_and_uno_mega_compilation(self):
        for command in (True, False):
            for board in ('arduino:avr:uno', 'arduino:avr:mega'):
                with self.subTest(command=command, board=board):
                    self.verify_pipeline(command, board)

    def verify_pipeline(self, command, board):
        from fastapi.testclient import TestClient
        from app.main import app
        from app.services.sysml_assembly import assemble_sysml
        draft = receive_proposal(command, board)
        coexist = not command and board == 'arduino:avr:mega'
        if coexist:
            draft['serial_channels'] = [dict(baud=9600, trigger=dict(initial=False, meaning='Report newly received values'),
                values=[dict(type='i32', unit='1', initial=0, meaning='Received value', label='RX: ')])]
        data = prepared(draft)
        state_type, initial = ('bool', False) if command else ('f64', 0.0)
        update = ({'if': 'io_0_valid and io_0 == 1', 'then': True,
                   'else': {'if': 'io_0_valid and io_0 == 2', 'then': False, 'else': 'held'}}) if command else (
                   {'if': 'io_0_valid', 'then': 'io_0 + 0.0', 'else': 'held'})
        controller = dict(states=[dict(name='held', type=state_type, initial=initial)], updates=[dict(assign='held', value=update)],
                          outputs={'io_1': 'held'}, components=[])
        if coexist:
            controller['outputs'].update(serial_0_trigger='io_0_valid', serial_0_value_0='io_0')
        source = assemble_sysml(controller, data)
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
            base = Path(folder)
            compiled = client.post('/compile/sysml2', json=dict(system_sysml2=source, interface_contract=data, out_dir=str(base/'ir')))
            self.assertEqual(compiled.status_code, 200, compiled.text)
            generated = client.post('/convert/il-to-c', json=dict(interface_contract=data,
                provenance=compiled.json()['contents']['provenance'], il_file_path=compiled.json()['files']['il'], out_c_dir=str(base/'c')))
            self.assertEqual(generated.status_code, 200, generated.text)
            contents = generated.json()['contents']
            adapted = client.post('/convert/c-to-arduino', json=dict(provenance=generated.json()['provenance'], c_code=contents['ProgramPage1.c'],
                interface_contract=data, compiled_kernel=dict(header=contents['define.h'], source=contents['ProgramPage1.c']), out_path=str(base/'serial.ino')))
            self.assertEqual(adapted.status_code, 200, adapted.text)
            code = Path(adapted.json()['saved_path']).read_text(encoding='utf-8')
            run_host(self, base, code, command)
            result = client.post('/arduino/compile', json=dict(sketch_path=adapted.json()['saved_path'], fqbn=board))
            self.assertEqual(result.status_code, 200, result.text)
            self.assertTrue(result.json()['success'], result.text)


ARDUINO = '''#pragma once
#include <stdint.h>
#include <string>
#include <vector>
enum { LOW=0,HIGH=1,INPUT=0,OUTPUT=1,INPUT_PULLUP=2 };
static uint32_t now = 0;
static int levels[70] = {};
static unsigned long millis(){return now;}
static void pinMode(int,int){}
static void digitalWrite(int pin,int value){levels[pin]=value;}
static void analogWrite(int pin,int value){levels[pin]=value;}
template<class T> T constrain(T value,T low,T high){return value<low?low:(value>high?high:value);}
struct SerialStub {
 std::string input; size_t cursor=0;
 std::vector<double> values;
 void begin(int baud){if(baud!=9600)__builtin_abort();}
 int available(){return (int)(input.size()-cursor);}
 int read(){return available()?(unsigned char)input[cursor++]:-1;}
 void feed(const std::string& s){input+=s;}
 void print(const char*){}
 void println(double value){values.push_back(value);}
};
static SerialStub Serial;
'''


def run_host(test, base, code, command):
    if command:
        checks = '''
 setup(); loop(); assert(levels[13]==0 && !hw_value_io_0_valid);
 Serial.feed("O"); now=10;loop();assert(levels[13]==0);
 Serial.feed("N\\r\\nOFF\\nON\\n");now=20;loop();assert(levels[13]==1 && hw_value_io_0_valid);
 now=30;loop();assert(levels[13]==0 && hw_value_io_0_valid);
 now=40;loop();assert(levels[13]==1 && hw_value_io_0_valid);
 now=50;loop();assert(levels[13]==1 && !hw_value_io_0_valid);
 Serial.feed("BAD\\n");now=60;loop();assert(levels[13]==1 && !hw_value_io_0_valid);
 Serial.feed(" OFF ");now=70;loop();assert(levels[13]==1);
 now=80;loop();assert(levels[13]==1);now=90;loop();assert(levels[13]==0 && hw_value_io_0_valid);
 Serial.feed(std::string(70,'X')+"ON\\nON\\n");
 for(now=100;now<=120;now+=10)loop();assert(levels[13]==0 && !hw_value_io_0_valid);
 now=130;loop();assert(levels[13]==1 && hw_value_io_0_valid);
 Serial.feed(std::string("OFF",3)+std::string(1,'\\0')+"\\n");now=140;loop();assert(levels[13]==1 && !hw_value_io_0_valid);
 Serial.feed("off\\n");now=150;loop();assert(levels[13]==1 && !hw_value_io_0_valid);
'''
    else:
        checks = '''
 setup();loop();assert(levels[9]==0);
 Serial.feed("255\\n0\\r\\n128\\n");now=10;loop();assert(levels[9]==255 && hw_value_io_0_valid);
 now=20;loop();assert(levels[9]==0);now=30;loop();assert(levels[9]==128);
 const char* bad[]={"256\\n","-1\\n","12x\\n","2147483648\\n","-2147483649\\n","\\n","+\\n","1 2\\n"};
 for(auto value:bad){Serial.feed(value);now+=10;loop();assert(levels[9]==128 && !hw_value_io_0_valid);}
 Serial.feed(" +64 ");now+=10;loop();assert(levels[9]==128);
 now+=20;loop();assert(levels[9]==64 && hw_value_io_0_valid);
 now+=10;loop();assert(levels[9]==64 && !hw_value_io_0_valid);
 Serial.feed(std::string(70,'9')+"\\n32\\n");
 now+=10;loop();now+=10;loop();now+=10;loop();assert(levels[9]==64);
 now+=10;loop();assert(levels[9]==32);
'''
    (base/'Arduino.h').write_text(ARDUINO, encoding='utf-8')
    test_cpp = base/'test.cpp'
    test_cpp.write_text(code+'\n#include <cassert>\nint main(){\n'+checks+'}\n', encoding='utf-8')
    binary = base/'test.exe'
    built = subprocess.run([shutil.which('g++'), '-std=c++17', '-I', str(base), str(test_cpp), '-o', str(binary)], capture_output=True, text=True)
    test.assertEqual(built.returncode, 0, built.stderr)
    result = subprocess.run([str(binary)], capture_output=True, text=True, timeout=10)
    test.assertEqual(result.returncode, 0, result.stderr)


if __name__ == '__main__': unittest.main()
