import copy
import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
from unittest.mock import patch

from fastapi.testclient import TestClient
from app.main import app
from app.services.arduino_adapter import generate_adapter
from app.services.hardware_adapter import validate_hardware_contract, generate_hardware_adapter
from app.services.interface_planner import contract_digest


def port(name, direction, typ, unit, period, initial, binding):
    return dict(name=name, direction=direction, type=typ, unit=unit, sample_period_ms=period,
                initial=initial, meaning=name, device=binding['kind'], hardware='test circuit', binding=binding)


def contract():
    return {'version': 1, 'control_period_ms': 100, 'interfaces': [
        port('sample', 'input', 'f64', 'adc_count', 200, 0.0, {'kind': 'analog_input', 'pin': 'A0'}),
        port('elapsed', 'input', 'i32', 'ms', 100, 0, {'kind': 'elapsed_ms'}),
        port('alarm', 'output', 'bool', '1', 100, False, {'kind': 'digital_output', 'pin': 13, 'active_high': True}),
        port('reportDue', 'output', 'bool', '1', 100, False, {'kind': 'report_trigger'}),
        port('measured', 'output', 'f64', 'adc_count', 100, 0.0,
             {'kind': 'serial_value', 'trigger': 'reportDue', 'label': 'Value: ', 'baud': 115200}),
    ], 'requirements': [{'id': 'R1', 'text': 'Sample every 200 ms; threshold 500; report every 300 ms',
                        'assertion_ids': ['sampling', 'threshold', 'report_period']}]}


HEADER = '''struct ProgramPage1_state {};
extern struct ProgramPage1_state ProgramPage1_instance;
void ProgramPage1_Init(struct ProgramPage1_state* self);
void ProgramPage1(double sample, int elapsed, bool* alarm, bool* reportDue, double* measured);
'''
KERNEL = '''struct ProgramPage1_state ProgramPage1_instance;
static int accumulated = 0;
void ProgramPage1_Init(struct ProgramPage1_state* self) { accumulated = 0; }
void ProgramPage1(double sample, int elapsed, bool* alarm, bool* reportDue, double* measured) {
 accumulated += elapsed; *reportDue = accumulated >= 300;
 if (*reportDue) accumulated -= 300;
 *alarm = sample >= 500; *measured = sample;
}
'''
ARDUINO = '''#pragma once
#include <stdint.h>
#include <vector>
enum { LOW=0, HIGH=1, INPUT=0, OUTPUT=1, INPUT_PULLUP=2, A0=14 };
static uint32_t now = 0;
static int reading = 100, reads = 0, levels[20] = {};
static unsigned long millis() { return now; }
static void pinMode(int,int) {}
static int analogRead(int pin) { if(pin != A0) __builtin_abort(); ++reads; return reading; }
static void digitalWrite(int pin, int value) { levels[pin] = value; }
struct SerialStub {
 std::vector<double> values;
 void begin(int baud) { if(baud != 115200) __builtin_abort(); }
 void print(const char*) {}
 void println(double value) { values.push_back(value); }
};
static SerialStub Serial;
'''


class HardwareContractTests(unittest.TestCase):
    def test_actual_generated_glue_obeys_sampling_outputs_report_and_clock_wrap(self):
        def no_llm(_):
            self.fail('Structured hardware generation must not invoke LLM')
        model, code = generate_adapter(no_llm, HEADER, KERNEL, 'requirements', {}, contract=contract())
        self.assertEqual(model, 'deterministic-hardware-adapter')
        harness = '''
#include <cassert>
int main(int argc, char**) {
 setup(); assert(levels[13] == LOW);
 if (argc > 1) {
   now = 4294967245U; loop(); assert(reads == 1);
   now = 49; loop(); assert(accumulated == 100 && reads == 1);
   now = 149; loop(); assert(accumulated == 200 && reads == 2);
   now = 249; loop(); assert(Serial.values.size() == 1);
   return 0;
 }
 loop(); assert(reads == 1 && levels[13] == LOW);
 reading = 700; now = 99; loop(); assert(reads == 1 && levels[13] == LOW);
 now = 100; loop(); assert(reads == 1 && levels[13] == LOW);
 now = 200; loop(); assert(reads == 2 && levels[13] == HIGH);
 now = 299; loop(); assert(Serial.values.empty());
 now = 300; loop(); assert(Serial.values.size() == 1 && Serial.values[0] == 700);
 loop(); assert(Serial.values.size() == 1);
 reading = 200; now = 400; loop(); assert(reads == 3 && levels[13] == LOW);
 now = 500; loop(); assert(Serial.values.size() == 1);
 now = 600; loop(); assert(Serial.values.size() == 2 && Serial.values[1] == 200);
}
'''
        with tempfile.TemporaryDirectory() as folder:
            base = Path(folder)
            (base/'Arduino.h').write_text(ARDUINO, encoding='utf-8')
            (base/'test.cpp').write_text(code + harness, encoding='utf-8')
            binary = base/'test.exe'
            build = subprocess.run([shutil.which('g++'), '-std=c++17', '-I', str(base), str(base/'test.cpp'), '-o', str(binary)], capture_output=True, text=True)
            self.assertEqual(build.returncode, 0, build.stderr)
            for args in ([], ['wrap']):
                run = subprocess.run([str(binary), *args], capture_output=True, text=True)
                self.assertEqual(run.returncode, 0, run.stderr)

    def test_mapping_errors_fail_before_generating_code(self):
        invalid = []
        wrong = contract(); wrong['interfaces'][0]['binding']['kind'] = 'constant'; invalid.append(wrong)
        wrong = contract(); wrong['interfaces'][0]['binding']['pin'] = 'A0); injected()'; invalid.append(wrong)
        wrong = contract(); wrong['interfaces'][0]['sample_period_ms'] = 150; invalid.append(wrong)
        wrong = contract(); wrong['interfaces'][4]['binding']['trigger'] = 'missing'; invalid.append(wrong)
        wrong = contract(); wrong['interfaces'][2]['binding']['pin'] = 'A0'; invalid.append(wrong)
        wrong = contract(); wrong['interfaces'][0]['unit'] = 'degC'; invalid.append(wrong)
        for wrong in invalid:
            with self.subTest(wrong=wrong), self.assertRaises(ValueError):
                generate_hardware_adapter(HEADER, wrong)
        with self.assertRaisesRegex(ValueError, 'signature'):
            generate_hardware_adapter(HEADER.replace('double sample', 'bool sample'), contract())

    def test_production_api_rejects_unproven_kernel_with_valid_frozen_contract(self):
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
            base = Path(folder)
            with patch('app.main._call_llm', side_effect=AssertionError('No LLM required')), patch('app.main._llm_api_key', ''):
                prepared = client.post('/generate/interface-contract', json={
                    'requirement': 'verified inventory', 'interface_contract': contract(), 'out_path': str(base/'contract.json')})
                self.assertEqual(prepared.status_code, 200, prepared.text)
                result = client.post('/convert/c-to-arduino', json={
                    'c_code': KERNEL, 'compiled_kernel': {'header': HEADER, 'source': KERNEL},
                    'interface_contract': prepared.json()['interface_contract'], 'out_path': str(base/'sketch.ino')})
                self.assertEqual(result.status_code, 422, result.text)
                self.assertIn('provenance',result.text)
                self.assertFalse((base/'sketch.ino').exists())
                legacy = client.post('/convert/c-to-arduino', json={'c_code': KERNEL})
                self.assertEqual(legacy.status_code, 422)

    def test_failed_contract_planning_is_retained(self):
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
            path = Path(folder)/'contract.json'
            with patch('app.main._call_llm', return_value=('test', '{"unsupported":["device-X"]}')) as llm:
                response = client.post('/generate/interface-contract', json={
                    'requirement': 'Use device-X', 'out_path': str(path), 'max_repair_attempts': 1})
            self.assertEqual(response.status_code, 422)
            self.assertEqual(llm.call_count, 2)
            self.assertFalse(path.exists())
            self.assertEqual(len(json.loads(path.with_suffix('.planning.json').read_text())['attempts']), 2)

    def test_environment_model_contract_and_real_arduino_toolchain(self):
        inventory = json.loads(Path('tools/examples/environment_report.contract.json').read_text(encoding='utf-8'))
        source = Path('tools/sysml/environment_report.sysml').read_text(encoding='utf-8')
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
            base = Path(folder)
            compiled = client.post('/compile/sysml2', json={
                'system_sysml2': source, 'interface_contract': inventory, 'out_dir': str(base/'ir')})
            self.assertEqual(compiled.status_code, 200, compiled.text)
            self.assertEqual(compiled.json()['contents']['contract_sha256'], contract_digest(inventory))
            generated = client.post('/convert/il-to-c', json={
                'interface_contract':inventory,'provenance':compiled.json()['contents']['provenance'],
                'il_file_path': compiled.json()['files']['il'], 'out_c_dir': str(base/'c')})
            self.assertEqual(generated.status_code, 200, generated.text)
            contents = generated.json()['contents']
            with patch('app.main._call_llm', side_effect=AssertionError('Hardware glue must be deterministic')):
                adapter = client.post('/convert/c-to-arduino', json={
                    'provenance':generated.json()['provenance'],
                    'c_code': contents['ProgramPage1.c'], 'interface_contract': inventory,
                    'compiled_kernel': {'header': contents['define.h'], 'source': contents['ProgramPage1.c']},
                    'out_path': str(base/'environment.ino')})
            self.assertEqual(adapter.status_code, 200, adapter.text)
            self.assertEqual(adapter.json()['contract_sha256'], contract_digest(inventory))
            result = client.post('/arduino/compile', json={
                'sketch_path': adapter.json()['saved_path'], 'fqbn': 'arduino:avr:uno'})
            self.assertEqual(result.status_code, 200, result.text)
            self.assertTrue(result.json()['success'], result.text)


if __name__ == '__main__':
    unittest.main()
