import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
from fastapi.testclient import TestClient
from app.main import app, _sanitize_system

class PipelineApiTests(unittest.TestCase):
    def test_legacy_c_endpoint_uses_compiler_without_llm(self):
        source = Path('tools/sysml/noise_monitor.sysml').read_text(encoding='utf-8-sig')
        with tempfile.TemporaryDirectory(prefix='sysml-legacy-') as folder, TestClient(app) as client:
            with patch('app.main._call_llm', side_effect=AssertionError('LLM must not generate model C')):
                response = client.post('/generate/c-from-step1', json={
                    'step1_sysml2': source, 'out_dir': str(Path(folder)/'c')})
            self.assertEqual(response.status_code,200,response.text)
            self.assertEqual(response.json()['model'],'deterministic-sysml-compiler')
            self.assertIn('mic * mic',response.json()['contents']['generated_model_c'])

    def test_incremental_context_keeps_model_and_requirement_map(self):
        source = {'modules': [], 'connections': [], 'system_sysml2': 'package P;',
                  'req_test_map': {'requirements_to_testpoints': [{'requirement_id': 'R1'}]}}
        saved = _sanitize_system(source)
        self.assertEqual(saved['system_sysml2'], source['system_sysml2'])
        self.assertEqual(saved['req_test_map'], source['req_test_map'])
        saved['req_test_map']['requirements_to_testpoints'].clear()
        self.assertEqual(len(source['req_test_map']['requirements_to_testpoints']), 1)

    def test_exhausted_repair_stops_before_saving_model(self):
        with tempfile.TemporaryDirectory(prefix='sysml-stop-') as folder, TestClient(app) as client:
            target = Path(folder) / 'system.sysml'
            with patch('app.main._call_llm', return_value=('test', '{}')) as llm:
                response = client.post('/generate/sysml2-bundle', json={
                    'prompt': 'retain requirements', 'max_repair_attempts': 1,
                    'out_sysml2_path': str(target), 'out_req_test_map_path': str(Path(folder) / 'map.json')})
            self.assertEqual(response.status_code, 422)
            self.assertEqual(llm.call_count, 2)
            self.assertFalse(target.exists())
            self.assertFalse((Path(folder) / 'map.json').exists())
            report = json.loads(target.with_suffix('.validation.json').read_text(encoding='utf-8'))
            self.assertFalse(report['ok'])
            self.assertEqual(len(report['attempts']), 2)

    def test_compiler_diagnostic_repairs_and_compiles_via_api(self):
        good = Path('tools/sysml/modular_monitor.sysml').read_text(encoding='utf-8-sig')
        bad = 'package P { private import ScalarValues::*; part def A { out attribute alarm : Boolean; } }'
        def bundle(model):
            return json.dumps({'system_sysml2': model, 'req_test_map': {'requirements_to_testpoints': [
                {'requirement_id':'R1','requirement_text':'square and threshold','test_point_id':'T1',
                 'test_method':'unit-test','stimulus':'mic=3 threshold=8','oracle':'alarm=true'}]}})
        with tempfile.TemporaryDirectory(prefix='sysml-api-') as folder, TestClient(app) as client:
            target = Path(folder)/'system.sysml'
            with patch('app.main._call_llm', side_effect=[('test',bundle(bad)),('test',bundle(good))]) as llm:
                response=client.post('/generate/sysml2-bundle',json={'prompt':'square and threshold','out_sysml2_path':str(target),
                    'out_req_test_map_path':str(Path(folder)/'map.json'),'max_repair_attempts':1})
                self.assertEqual(response.status_code,200,response.text)
                self.assertEqual(llm.call_count,2)
                self.assertEqual(response.json()["contents"]["validation"]["repairs"], 1)
                self.assertEqual([a["ok"] for a in response.json()["contents"]["validation"]["diagnostics"]], [False, True])
                self.assertIn('SYSML_LOWERING',llm.call_args_list[1].args[0])
                self.assertEqual(target.read_text(encoding='utf-8'),good)
            response=client.post('/compile/sysml2',json={'system_sysml2':good,'out_dir':str(Path(folder)/'compiled')})
            self.assertEqual(response.status_code,200,response.text)
            for name in ('subsystem_ir','il','tblink'):
                self.assertTrue(Path(response.json()['files'][name]).is_file())
            invalid=client.post('/compile/sysml2',json={'system_sysml2':bad,'out_dir':str(Path(folder)/'invalid')})
            self.assertEqual(invalid.status_code,422)
            self.assertFalse((Path(folder)/'invalid').exists())

if __name__=='__main__':
    unittest.main()
