import shutil
import subprocess
import tempfile
import unittest
from unittest.mock import patch
from fastapi.testclient import TestClient
from pathlib import Path
from app.services.arduino_adapter import generate_adapter, compiled_kernel_text, adapter_issues
from app.services.sysml_compiler import compile_sysml, ir_to_il
from app.main import app, convert_il_to_c, ConvertIlToCRequest

class StatefulArduinoTests(unittest.TestCase):
    def test_nested_branch_sequence_preserves_update_order_and_reset(self):
        source = Path('tools/sysml/conditional_updates.sysml').read_text(encoding='utf-8')
        ir = compile_sysml(source)
        with tempfile.TemporaryDirectory(prefix='nested-branch-') as folder:
            base = Path(folder); il = base/'model.xml'
            il.write_text(ir_to_il(ir), encoding='utf-8')
            generated = convert_il_to_c(ConvertIlToCRequest(il_file_path=str(il), out_c_dir=str(base/'c')))
            code = compiled_kernel_text(generated.contents['define.h'], generated.contents['ProgramPage1.c'])
            code += '''
#include <cassert>
int main(){int32_t result; ProgramPage1_Init(&ProgramPage1_instance);
 ProgramPage1(false,&result); assert(result==2);
 ProgramPage1(false,&result); assert(result==4);
 ProgramPage1(true,&result); assert(result==0);
 ProgramPage1(false,&result); assert(result==2);}
'''
            cpp=base/'check.cpp'; cpp.write_text(code, encoding='utf-8'); binary=base/'check.exe'
            build=subprocess.run([shutil.which('g++'), '-std=c++17', str(cpp), '-o', str(binary)], capture_output=True, text=True)
            self.assertEqual(build.returncode,0,build.stderr)
            run=subprocess.run([str(binary)],capture_output=True,text=True)
            self.assertEqual(run.returncode,0,run.stderr)

    def test_environment_report_independent_boundary_trace(self):
        import json
        from app.services.behavior_contract import check_contract
        source = Path('tools/sysml/environment_report.sysml').read_text(encoding='utf-8')
        contract = json.loads(Path('tools/examples/environment_report.contract.json').read_text(encoding='utf-8'))
        ir = compile_sysml(source)
        coverage = check_contract(ir, contract)
        self.assertEqual(coverage['status'], 'structural_pass', coverage['issues'])
        # Expected outcomes are specified independently of emitted expressions.
        with tempfile.TemporaryDirectory(prefix='environment-trace-') as folder:
            base = Path(folder); il = base/'model.xml'
            il.write_text(ir_to_il(ir), encoding='utf-8')
            generated = convert_il_to_c(ConvertIlToCRequest(il_file_path=str(il), out_c_dir=str(base/'c')))
            code = compiled_kernel_text(generated.contents['define.h'], generated.contents['ProgramPage1.c'])
            code += '''
#include <cassert>
int main() {
 double temperature, humidity; bool due, high;
 ProgramPage1_Init(&ProgramPage1_instance);
 ProgramPage1(30.0, 42.0, 4999, &temperature, &humidity, &due, &high);
 assert(temperature==30.0 && humidity==42.0 && !due && !high);
 ProgramPage1(30.1, 51.0, 1, &temperature, &humidity, &due, &high);
 assert(temperature==30.1 && humidity==51.0 && due && high);
 ProgramPage1(20.0, 60.0, 100, &temperature, &humidity, &due, &high);
 assert(!due && !high);
 ProgramPage1(20.0, 60.0, 4999, &temperature, &humidity, &due, &high);
 assert(due);
 ProgramPage1(20.0, 60.0, 4900, &temperature, &humidity, &due, &high);
 assert(!due);
 ProgramPage1(20.0, 60.0, 1, &temperature, &humidity, &due, &high);
 assert(due);
 ProgramPage1_Init(&ProgramPage1_instance);
 ProgramPage1(10.0, 33.0, 1, &temperature, &humidity, &due, &high);
 assert(temperature==10.0 && humidity==33.0 && !due && !high);
}
'''
            cpp = base/'check.cpp'; cpp.write_text(code, encoding='utf-8'); binary = base/'check.exe'
            build = subprocess.run([shutil.which('g++'), '-std=c++17', str(cpp), '-o', str(binary)], capture_output=True, text=True)
            self.assertEqual(build.returncode, 0, build.stderr)
            run = subprocess.run([str(binary)], capture_output=True, text=True)
            self.assertEqual(run.returncode, 0, run.stderr)

    def test_generated_stateful_kernel_survives_adapter_and_reset(self):
        source = Path('tools/sysml/counter.sysml').read_text(encoding='utf-8-sig')
        ir = compile_sysml(source)
        self.assertEqual(ir['execution'], 'step-actions-then-outputs')
        with tempfile.TemporaryDirectory(prefix='sysml-e2e-') as folder:
            base = Path(folder)
            il = base/'counter.il.xml'
            il.write_text(ir_to_il(ir),encoding='utf-8')
            generated = convert_il_to_c(ConvertIlToCRequest(il_file_path=str(il), out_c_dir=str(base/'c')))
            header, c = generated.contents['define.h'], generated.contents['ProgramPage1.c']
            good = '''bool hardware_reset = false;
int32_t observed = -1;
void setup() { ProgramPage1_Init(&ProgramPage1_instance); }
void loop() { ProgramPage1(hardware_reset, &observed); }
'''
            responses = iter(['void ProgramPage1(bool x, int32_t* y) { *y=99; }', good])
            prompts = []
            def call(prompt, *_):
                prompts.append(prompt)
                return 'test-model',next(responses)
            with patch('app.main._call_llm', side_effect=call), patch('app.main._llm_api_key', 'test-only'), TestClient(app) as api:
                response = api.post('/convert/c-to-arduino', json={
                    'allow_legacy_adapter': True, 'c_code': c, 'compiled_kernel': {'header': header, 'source': c},
                    'user_requirements': 'count each tick; reset when requested', 'out_path': str(base/'counter.ino')})
                self.assertEqual(response.status_code, 200, response.text)
                sketch = response.json()['arduino_code']
                self.assertEqual((base/'counter.ino').read_text(encoding='utf-8'), sketch)
            self.assertEqual(len(prompts),2)
            self.assertIn('Do not redeclare',prompts[1])
            self.assertIn(compiled_kernel_text(header,c),sketch)
            self.assertTrue(sketch.startswith('#ifdef ARDUINO\n#include <Arduino.h>\n#endif\n'))
            self.assertIn('int32_t sysml_state_count',sketch)
            harness = '''
#include <cassert>
int main() {
 setup(); loop(); assert(observed==1);
 loop(); assert(observed==2);
 hardware_reset=true; loop(); assert(observed==0);
 hardware_reset=false; loop(); assert(observed==1);
 setup(); loop(); assert(observed==1);
 return 0;
}
'''
            cpp=base/'check.cpp'; cpp.write_text(sketch+harness,encoding='utf-8')
            compiler=shutil.which('g++')
            self.assertIsNotNone(compiler,'g++ is required for this end-to-end check')
            binary=base/'check.exe'
            build=subprocess.run([compiler,'-std=c++17','-Wall','-Wextra',str(cpp),'-o',str(binary)],capture_output=True,text=True)
            self.assertEqual(build.returncode,0,build.stderr)
            run=subprocess.run([str(binary)],capture_output=True,text=True)
            self.assertEqual(run.returncode,0,run.stderr)

    def test_explicit_sequential_updates_use_new_state(self):
        source = Path('tools/sysml/ordered_updates.sysml').read_text(encoding='utf-8-sig')
        ir = compile_sysml(source)
        self.assertEqual(len(ir['updates'][0]['steps']), 2)
        with tempfile.TemporaryDirectory(prefix='sysml-sequence-') as folder:
            base=Path(folder); il=base/'model.xml'
            il.write_text(ir_to_il(ir),encoding='utf-8')
            generated=convert_il_to_c(ConvertIlToCRequest(il_file_path=str(il),out_c_dir=str(base/'c')))
            code=compiled_kernel_text(generated.contents['define.h'],generated.contents['ProgramPage1.c'])
            code += '\n#include <cassert>\nint main(){int32_t value; ProgramPage1_Init(&ProgramPage1_instance); ProgramPage1(&value); assert(value==3); ProgramPage1(&value); assert(value==4);}'
            cpp=base/'check.cpp'; cpp.write_text(code,encoding='utf-8'); binary=base/'check.exe'
            subprocess.run([shutil.which('g++'),'-std=c++17',str(cpp),'-o',str(binary)],check=True,capture_output=True)
            subprocess.run([str(binary)],check=True,capture_output=True)

    def test_integer_division_preserves_fraction_through_real_backend(self):
        source = """package Arithmetic {
          private import ScalarValues::*;
          part def Controller {
            in attribute numerator : Integer;
            in attribute denominator : Integer;
            out attribute quotient : Real = numerator / denominator;
          }
          part system : Controller;
        }"""
        ir = compile_sysml(source)
        from experiment.code.formal_ir import validate_ir
        validate_ir(ir)
        with tempfile.TemporaryDirectory(prefix='sysml-division-') as folder:
            base = Path(folder)
            il = base/'model.xml'
            il.write_text(ir_to_il(ir), encoding='utf-8')
            generated = convert_il_to_c(ConvertIlToCRequest(il_file_path=str(il), out_c_dir=str(base/'c')))
            code = compiled_kernel_text(generated.contents['define.h'], generated.contents['ProgramPage1.c'])
            code += '\n#include <cassert>\nint main(){ double value; ProgramPage1_Init(&ProgramPage1_instance); ProgramPage1(3,2,&value); assert(value==1.5); ProgramPage1(-3,2,&value); assert(value==-1.5); ProgramPage1(0,2,&value); assert(value==0.0); }'
            cpp = base/'check.cpp'
            cpp.write_text(code, encoding='utf-8')
            binary = base/'check.exe'
            subprocess.run([shutil.which('g++'), '-std=c++17', str(cpp), '-o', str(binary)], check=True, capture_output=True)
            subprocess.run([str(binary)], check=True, capture_output=True)

    def test_nested_arithmetic_in_state_update_runs_correctly(self):
        source = """package Arithmetic {
          private import ScalarValues::*;
          part def Controller {
            in attribute low : Real; in attribute high : Real; in attribute gain : Real;
            attribute stored : Real := 0.0;
            perform action tick { assign stored := low + gain * (high - low); }
            out attribute result : Real = stored;
          }
          part system : Controller;
        }"""
        ir = compile_sysml(source)
        with tempfile.TemporaryDirectory(prefix='sysml-nested-') as folder:
            base=Path(folder); il=base/'model.xml'
            xml=ir_to_il(ir)
            self.assertIn('sysml_arithmetic_', xml)
            il.write_text(xml, encoding='utf-8')
            generated=convert_il_to_c(ConvertIlToCRequest(il_file_path=str(il), out_c_dir=str(base/'c')))
            code=compiled_kernel_text(generated.contents['define.h'], generated.contents['ProgramPage1.c'])
            code += '\n#include <cassert>\nint main(){double result; ProgramPage1_Init(&ProgramPage1_instance); ProgramPage1(2,10,0.5,&result); assert(result==6.0); ProgramPage1(-2,6,0.25,&result); assert(result==0.0);}'
            cpp=base/'check.cpp'; cpp.write_text(code, encoding='utf-8'); binary=base/'check.exe'
            subprocess.run([shutil.which('g++'), '-std=c++17', str(cpp), '-o', str(binary)], check=True, capture_output=True)
            subprocess.run([str(binary)], check=True, capture_output=True)

    def test_adapter_cannot_access_model_state(self):
        self.assertTrue(adapter_issues('void setup(){} void loop(){sysml_state_count=10;}'))
        self.assertTrue(adapter_issues('void setup(){ProgramPage1_Init(&ProgramPage1_instance);} void loop(){ProgramPage1(false, &ProgramPage1_instance.sound);}'))
        with self.assertRaisesRegex(ValueError,'structural checks'):
            generate_adapter(lambda _: ('test','void setup(){} void loop(){}'),
                             'void ProgramPage1(int* value);', 'void ProgramPage1(int* value){*value=1;}', '',{},max_repairs=0)

if __name__=='__main__':
    unittest.main()
