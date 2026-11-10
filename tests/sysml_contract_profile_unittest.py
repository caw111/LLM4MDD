import copy
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
from fastapi.testclient import TestClient

from app.main import app
from app.services.interface_planner import plan_interface_contract
from app.services.interface_registry import register_contract
from app.services.hardware_adapter import validate_hardware_contract, generate_hardware_adapter
from app.services.behavior_contract import validate_sysml_contract, check_contract
from app.services.sysml import generate_validated_bundle
from app.services.sysml_assembly import assemble_sysml
from app.services.ir_contract import validate_ir


def inventory():
    return dict(version=1,control_period_ms=10,interfaces=[
        dict(name='sample',direction='input',type='f32',unit='adc_count',meaning='sensor',sample_period_ms=10,initial=0.0,
             device='potentiometer',hardware='A0',binding=dict(kind='analog_input',pin='A0')),
        dict(name='brightness',direction='output',type='f32',unit='pwm_count',meaning='PWM',sample_period_ms=10,initial=0.0,
             device='LED',hardware='D9',binding=dict(kind='pwm_output',pin=9))],
        requirements=[dict(id='R1',text='Use A0 to drive D9 brightness',assertion_ids=['response'])])


class SysMLContractProfileTests(unittest.TestCase):
    def test_planner_repairs_representability_before_freezing(self):
        bad=inventory();good=copy.deepcopy(bad)
        for p in good['interfaces']:p['type']='f64'
        replies=iter([bad,good]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',json.dumps(next(replies))
        accepted,report=plan_interface_contract(call,'A0 controls D9 brightness',{},max_repairs=1)
        self.assertEqual(accepted,register_contract(good))
        self.assertIn('sample, brightness',prompts[1])
        self.assertIn('Real to f64',prompts[1])
        self.assertEqual([a['ok'] for a in report['attempts']],[False,True])
        self.assertEqual(bad['interfaces'][0]['type'],'f32')

    def test_supplied_sysml_contract_fails_before_calls_tools_or_artifact_writes(self):
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
            base=Path(folder)
            with patch('app.main._call_llm',side_effect=AssertionError('No LLM')), \
                 patch('app.main.validate_sysml',side_effect=AssertionError('No parser before interface gate')):
                requests=[('/generate/interface-contract',dict(requirement='A0 controls D9',interface_contract=inventory(),out_path=str(base/'contract.json'))),
                          ('/compile/sysml2',dict(system_sysml2='model',interface_contract=inventory(),out_dir=str(base/'compiled'))),
                          ('/generate/sysml2-bundle',dict(prompt='A0 controls D9',interface_contract=inventory(),out_sysml2_path=str(base/'model.sysml')))]
                for endpoint,payload in requests:
                    response=client.post(endpoint,json=payload)
                    self.assertEqual(response.status_code,422,response.text)
                    self.assertIn('f32',response.text)
            self.assertEqual(list(base.iterdir()),[])
        with self.assertRaisesRegex(ValueError,'f32'):
            generate_validated_bundle(lambda _:self.fail('No LLM'),'prompt',contract=inventory())

    def test_local_states_are_not_silently_widened(self):
        contract=inventory()
        for p in contract['interfaces']:p['type']='f64'
        controller=dict(states=[dict(name='stored',type='f32',initial='0.0')],updates=[],outputs={'brightness':'stored'})
        with self.assertRaisesRegex(ValueError,'f32'):
            assemble_sysml(controller,contract)

    def test_direct_ir_and_hardware_keep_f32_support(self):
        contract=inventory();validate_hardware_contract(contract)
        ir=dict(version=2,name='FloatPass',page='ProgramPage1',inputs=[dict(name='sample',type='f32')],
                outputs=[dict(name='brightness',type='f32')],nodes=[],
                edges=[dict(src=dict(kind='input',name='sample'),dst=dict(kind='output',name='brightness'))],
                states=[],updates=[],execution='step-actions-then-outputs')
        validate_ir(ir);self.assertEqual(check_contract(ir,contract)['issues'],[])
        code=generate_hardware_adapter('void ProgramPage1(float sample,float* brightness);',contract)
        self.assertIn('static float hw_value_sample',code)
        with TestClient(app) as client:
            result=client.post('/validate/subsystem-ir',json={'ir':ir,'interface_contract':contract})
            self.assertTrue(result.json()['ok'],result.text)
        with self.assertRaisesRegex(ValueError,'f32'):validate_sysml_contract(contract)


if __name__=='__main__':unittest.main()
