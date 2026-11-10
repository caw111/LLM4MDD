"""A valid v2 graph alone does not establish correspondence with its source model."""
import copy
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from fastapi.testclient import TestClient
from app.main import app
from app.services.ir_contract import validate_ir
from app.services.sysml_compiler import compile_sysml


SOURCE = '''package Model {
 private import ScalarValues::*;
 part def Controller {
   in attribute sample : Real;
   out attribute doubled : Real = sample * 2.0;
 }
 part system : Controller;
}'''


class CorrespondenceTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.ir = compile_sysml(SOURCE)

    def test_valid_but_different_computation_is_rejected(self):
        changed = copy.deepcopy(self.ir)
        changed['nodes'][0]['params']['Expression'] = '(arg0 + 2.0)'
        validate_ir(changed)  # This is valid v2, but not the supplied source's graph.
        with TestClient(app) as client:
            result = client.post('/validate/subsystem-ir', json={'ir':changed, 'step1_sysml2':SOURCE})
        self.assertEqual(result.status_code, 200, result.text)
        self.assertFalse(result.json()['ok'])
        self.assertIn('IR_SOURCE_MISMATCH', [i['code'] for i in result.json()['issues']])

    def test_matching_compiler_artifact_is_accepted(self):
        with TestClient(app) as client:
            result = client.post('/validate/subsystem-ir', json={'ir':self.ir, 'step1_sysml2':SOURCE})
        self.assertEqual(result.status_code, 200, result.text)
        self.assertTrue(result.json()['ok'], result.text)

    def test_source_based_generation_cannot_ask_llm_for_another_ir(self):
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
            for endpoint in ('subsystem-ir', 'subsystem-ir-iterative'):
                with self.subTest(endpoint=endpoint):
                    target = Path(folder)/(endpoint+'.json')
                    with patch('app.main._call_llm', side_effect=AssertionError('Source model must use deterministic compiler')):
                        result = client.post('/generate/'+endpoint, json={
                            'step1_sysml2':SOURCE, 'out_ir_path':str(target)})
                    self.assertEqual(result.status_code, 200, result.text)
                    self.assertEqual(result.json()['model'], 'deterministic-sysml-compiler')
                    self.assertEqual(result.json()['contents']['ir_origin'], 'compiled_sysml')
                    self.assertEqual(json.loads(target.read_text(encoding='utf-8')), self.ir)

    def test_invalid_source_never_falls_back_to_direct_generation(self):
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
            for endpoint in ('subsystem-ir','subsystem-ir-iterative'):
                target = Path(folder)/(endpoint+'.json')
                with patch('app.main._call_llm', side_effect=AssertionError('No fallback')):
                    response = client.post('/generate/'+endpoint, json={
                        'step1_sysml2':'', 'prompt':'generate a replacement', 'out_ir_path':str(target)})
                if endpoint == 'subsystem-ir':
                    self.assertEqual(response.status_code, 422)
                else:
                    self.assertFalse(response.json()['ok'])
                self.assertFalse(target.exists())

    def test_source_generation_preserves_contract_gate(self):
        contract = {'version':1, 'control_period_ms':10, 'interfaces':[
            dict(name=name, direction=direction, type='f64', unit='1', meaning=name,
                 sample_period_ms=10, initial=0.0, hardware='test', device='test')
            for name,direction in [('sample','input'),('differentOutput','output')]],
            'requirements':[dict(id='R1',text='double input',assertion_ids=['double'])]}
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
            target = Path(folder)/'ir.json'
            with patch('app.main._call_llm', side_effect=AssertionError('No fallback')):
                response = client.post('/generate/subsystem-ir-iterative', json={
                    'step1_sysml2':SOURCE, 'interface_contract':contract, 'out_ir_path':str(target)})
            self.assertFalse(response.json()['ok'])
            self.assertIn('INTERFACE_COVERAGE', [i['code'] for i in response.json()['issues']])
            self.assertFalse(target.exists())


if __name__ == '__main__': unittest.main()
