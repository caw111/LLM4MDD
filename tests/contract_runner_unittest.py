import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
import run_contract_development as runner


class Calls:
    def __init__(self,*args):self.entries=[]
    def __call__(self,prompt):
        self.entries.append({'seconds':0,'usage':{'total_tokens':0,'cost':0}})
        return 'fixture','void setup(){} void loop(){}'


class RunnerTests(unittest.TestCase):
    def test_backend_rejection_is_retained_and_next_task_continues(self):
        contract={'version':1,'control_period_ms':10,'interfaces':[
            dict(name='led',direction='output',type='bool',unit='1',initial=True,sample_period_ms=10,
                 meaning='LED on',device='LED',hardware='D13',binding={'kind':'digital_output','pin':13})],
            'requirements':[dict(id='R1',text='LED on',assertion_ids=['on'])]}
        def bundle(*args,**kwargs):
            kwargs['validator']('fixture')
            return 'fixture',{'system_sysml2':'fixture'},{'attempts':[{'ok':True}]}
        with tempfile.TemporaryDirectory() as folder:
            root=Path(folder)
            with patch.object(runner,'BudgetCalls',Calls), \
                 patch.object(runner,'compiled',return_value=({'compile_ok':True,'functional_status':'passed'},'')), \
                 patch.object(runner,'plan_interface_contract',return_value=(contract,{'contract_sha256':'fixture'})), \
                 patch.object(runner,'generate_validated_bundle',side_effect=bundle), \
                 patch.object(runner,'validate_executable_sysml',return_value={'ok':True,'ir':{'version':2}}), \
                 patch.object(runner,'lower_verified',side_effect=[runner.BackendFailure('ILParser input rejection'),'kernel']), \
                 patch.object(runner,'adapt_verified',return_value='sketch'):
                case={'id':'simple_001','requirement':'LED','fqbn':'arduino:avr:uno'}
                first=runner.evaluate(case,1,root,'fixture')
                second=runner.evaluate(case,2,root,'fixture')
            self.assertEqual(len(first),3)
            self.assertEqual(first[-1]['failure_stage'],'ir_to_code')
            self.assertFalse(first[-1]['backend_ok'])
            self.assertNotIn('infrastructure_error',first[-1])
            self.assertEqual(json.loads((root/'simple_001/repeat_1/results.json').read_text())[-1],first[-1])
            self.assertEqual(second[-1]['functional_status'],'passed')
            self.assertTrue(second[-1]['backend_ok'])
            self.assertEqual(second[-1]['provenance_status'],'verified_model_chain')


if __name__=='__main__':unittest.main()
