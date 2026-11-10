"""Exercise the same public contract through API, compiler and export boundaries."""
import copy
import json
from pathlib import Path
import tempfile
import shutil
import subprocess
import sys
import unittest
from unittest.mock import patch

from fastapi.testclient import TestClient
from app.main import app
from app.services.ir_contract import validate_ir
from app.services.ir_prompt import IR_EXAMPLE
from app.services.sysml_compiler import ir_to_il
from tools.ir_to_tblink import ir_to_tblink


class SharedIRContractTests(unittest.TestCase):
    def test_f32_literal_survives_actual_il_backend(self):
        sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
        from run_formal_experiment import lower
        from tests.ir_execution_unittest import expression_ir
        from app.services.arduino_adapter import compiled_kernel_text
        with tempfile.TemporaryDirectory() as folder:
            base=Path(folder);header,source=lower(expression_ir('x+0.1f','f32'),base/'lowered')
            cpp=base/'check.cpp';exe=base/'check.exe'
            cpp.write_text('#include <cassert>\n'+compiled_kernel_text(header,source)+
                '\nint main(){ProgramPage1_Init(&ProgramPage1_instance);float result=0;ProgramPage1(1.0f,0.0f,&result);assert(result==1.0f+0.1f);}',encoding='utf-8')
            build=subprocess.run([shutil.which('g++'),'-std=c++17',str(cpp),'-o',str(exe)],capture_output=True,text=True,timeout=60)
            self.assertEqual(build.returncode,0,build.stderr)
            run=subprocess.run([str(exe)],capture_output=True,text=True,timeout=10)
            self.assertEqual(run.returncode,0,run.stderr)

    def test_float_suffix_is_f32_at_all_shared_boundaries(self):
        from app.services.ir_contract import expression_type
        from tests.ir_execution_unittest import expression_ir
        self.assertEqual(expression_type('0.1f',{}),'f32')
        self.assertEqual(expression_type('0.1',{}),'f64')
        ir=expression_ir('x+0.1f','f32')
        validate_ir(ir);ir_to_il(ir);ir_to_tblink(ir)
        with TestClient(app) as client:
            response=client.post('/validate/subsystem-ir',json={'ir':ir})
            self.assertTrue(response.json()['ok'],response.text)
        bad=copy.deepcopy(ir);bad['nodes'][0]['params']['Expression']='x+0.1'
        with self.assertRaisesRegex(ValueError,'type mismatch'):validate_ir(bad)

    def test_experiment_import_is_the_production_validator(self):
        from experiment.code.formal_ir import validate_ir as experiment_validator
        self.assertIs(experiment_validator, validate_ir)

    def test_invalid_graph_rejected_at_every_boundary(self):
        candidates = []
        old = copy.deepcopy(IR_EXAMPLE)
        old['version'] = 1
        candidates.append(old)
        missing = copy.deepcopy(IR_EXAMPLE)
        missing['edges'].pop()
        candidates.append(missing)
        unknown = copy.deepcopy(IR_EXAMPLE)
        unknown['nodes'][0]['params']['Expression'] = 'unknown + 1.0'
        candidates.append(unknown)
        duplicate = copy.deepcopy(IR_EXAMPLE)
        duplicate['edges'].append(copy.deepcopy(duplicate['edges'][0]))
        candidates.append(duplicate)
        syntax = copy.deepcopy(IR_EXAMPLE)
        syntax['nodes'][0]['params']['Expression'] = 'arg1 +'
        candidates.append(syntax)
        with TestClient(app) as client:
            for candidate in candidates:
                with self.subTest(candidate=candidate):
                    for boundary in (validate_ir, ir_to_il, ir_to_tblink):
                        with self.assertRaises(ValueError):
                            boundary(candidate)
                    result = client.post('/validate/subsystem-ir', json={'ir': candidate})
                    self.assertEqual(result.status_code, 200)
                    self.assertFalse(result.json()['ok'])
                    self.assertEqual(result.json()['issues'][0]['code'], 'IR_CONTRACT')
                    result = client.post('/convert/ir-to-tblink', json={'ir': candidate})
                    self.assertEqual(result.status_code, 400)

    def test_generation_does_not_save_invalid_ir_and_retry_keeps_candidate(self):
        bad = copy.deepcopy(IR_EXAMPLE)
        bad['version'] = 1
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
            path = Path(folder) / 'ir.json'
            with patch('app.main._call_llm', return_value=('test', json.dumps(bad))):
                result = client.post('/generate/subsystem-ir', json={'prompt': 'retain input', 'out_ir_path': str(path)})
            self.assertEqual(result.status_code, 422)
            self.assertFalse(path.exists())
            with patch('app.main._call_llm', side_effect=[('test', json.dumps(bad)), ('test', json.dumps(IR_EXAMPLE))]) as call:
                result = client.post('/generate/subsystem-ir-iterative', json={
                    'prompt': 'retain input', 'out_ir_path': str(path), 'max_iters': 2})
            self.assertEqual(result.status_code, 200, result.text)
            self.assertTrue(result.json()['ok'])
            self.assertEqual(json.loads(path.read_text(encoding='utf-8')), validate_ir(IR_EXAMPLE))
            self.assertIn(json.dumps(bad, ensure_ascii=False), call.call_args_list[1].args[0])

    def test_passthrough_without_expression_nodes_is_valid(self):
        ir = {'version': 2, 'name': 'Pass', 'page': 'ProgramPage1',
              'inputs': [{'name': 'signal', 'type': 'bool'}],
              'outputs': [{'name': 'value', 'type': 'bool'}], 'nodes': [],
              'edges': [{'src': {'kind': 'input', 'name': 'signal'}, 'dst': {'kind': 'output', 'name': 'value'}}],
              'states': [], 'updates': [], 'execution': 'step-actions-then-outputs'}
        self.assertIn('*value = signal', ir_to_il(ir))
        self.assertEqual(ir_to_tblink(ir).getroot().tag, 'Root')

    def test_state_can_be_exposed_as_output_and_cannot_use_tblink(self):
        ir = {'version': 2, 'name': 'Counter', 'page': 'ProgramPage1', 'inputs': [],
              'outputs': [{'name': 'count', 'type': 'i32'}], 'nodes': [],
              'edges': [{'src': {'kind': 'state', 'name': 'count'}, 'dst': {'kind': 'output', 'name': 'count'}}],
              'states': [{'name': 'count', 'type': 'i32', 'initial': '0'}],
              'updates': [{'kind': 'assign', 'state': 'count', 'expression': 'arg0 + 1',
                           'sources': [{'kind': 'state', 'name': 'count'}]}],
              'execution': 'step-actions-then-outputs'}
        xml = ir_to_il(ir)
        self.assertLess(xml.index('sysml_state_count = sysml_state_count + 1'), xml.index('*count = sysml_state_count'))
        with self.assertRaisesRegex(ValueError, 'stateless'):
            ir_to_tblink(ir)


if __name__ == '__main__':
    unittest.main()
