"""Six-arm boundary checks: shared contract, costs, rejection and no adapter LLM."""
import copy
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'experiment/code'))
import run_formal_experiment as runner
from app.services.sysml_assembly import assemble_sysml
from app.services.sysml_compiler import compile_sysml


def contract():
    return dict(version=1, board='arduino:avr:uno', control_period_ms=10,
        interfaces=[dict(name='lamp', direction='output', type='bool', unit='1', initial=False,
            sample_period_ms=10, meaning='LED output', device='LED', hardware='D13',
            binding=dict(kind='digital_output', pin=13, active_high=True))],
        requirements=[dict(id='R1', text='Turn on LED D13', assertion_ids=['on'])])


CONTROLLER = dict(states=[], updates=[], outputs={'lamp':'true'})
BUNDLE = dict(controller=CONTROLLER, req_test_map={'requirements_to_testpoints':[
    dict(requirement_id='R1', requirement_text='Turn on LED D13', test_point_id='on',
         test_method='observe', stimulus='startup', oracle='LED on')]})
CASE = dict(id='fixture', dataset='internal', stratum='simple', requirement='Turn on LED D13', fqbn='arduino:avr:uno')
HEADER = '''struct ProgramPage1_state {};
extern struct ProgramPage1_state ProgramPage1_instance;
void ProgramPage1_Init(struct ProgramPage1_state* self);
void ProgramPage1(bool* lamp);
'''
SOURCE = '''struct ProgramPage1_state ProgramPage1_instance;
void ProgramPage1_Init(struct ProgramPage1_state* self) {}
void ProgramPage1(bool* lamp) {*lamp=true;}
'''


class FixtureCalls:
    instances=[]
    responses={}
    def __init__(self, folder, model):
        self.folder=folder;folder.mkdir(parents=True,exist_ok=True)
        self.entries=[];self.cursor=0;self.instances.append(self)
    def __call__(self, prompt):
        response=self.responses[self.folder.name]  # Any adapter call fails the test.
        if callable(response): response=response(prompt, len(self.entries))
        self.entries.append(dict(prompt=prompt,response=response,seconds=1,usage={'total_tokens':10,'cost':.1}))
        self.cursor=len(self.entries)
        return 'fixture',response


class FormalContractTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.ir=compile_sysml(assemble_sysml(CONTROLLER,contract()))

    def setUp(self):
        FixtureCalls.instances=[]
        FixtureCalls.responses={'direct_calls':'void setup(){} void loop(){}',
            'contract_calls':json.dumps(contract()), 'model_calls':json.dumps(BUNDLE),
            'ir_calls':json.dumps(self.ir), 'kernel_calls':SOURCE}

    def run_case(self, root, case=None, reject_first_model=False):
        if reject_first_model:
            FixtureCalls.responses['model_calls']=lambda prompt,index: '{}' if index==0 else json.dumps(BUNDLE)
        # Real planning validation, SysML assembly/reference validation/IR checks,
        # and deterministic adapter; replace only LLM and expensive code tools.
        with patch.object(runner,'ReplayCalls',FixtureCalls), \
             patch.object(runner,'lower',return_value=(HEADER,SOURCE)), \
             patch.object(runner,'compile_attempt',return_value=({'compile_ok':True,'compile_status':'passed'},'')):
            return runner.evaluate(case or CASE,1,root,'fixture')

    def test_six_arms_share_contract_and_charge_planning(self):
        with tempfile.TemporaryDirectory(dir=runner.ROOT) as folder:
            root=Path(folder);rows=self.run_case(root)
            self.assertEqual(len(rows),6)
            self.assertTrue(all(r['compile_ok'] for r in rows), rows)
            self.assertEqual({r['arm']:r['llm_calls'] for r in rows},
                dict(direct=1,direct_feedback=1,workflow=2,no_sysml_feedback=2,direct_ir=2,llm_kernel=3))
            for r in rows:
                if r['arm'] in runner.CONTRACT_ARMS:
                    frozen=json.loads((root/'fixture'/'repeat_1'/'contract.json').read_text(encoding='utf-8'))
                    self.assertEqual(frozen,runner.freeze_model_semantics(contract()))
                    self.assertEqual(r['contract_sha256'],runner.contract_digest(frozen))
            prompts={c.folder.name:c.entries for c in FixtureCalls.instances}
            self.assertIn('Frozen interface',prompts['ir_calls'][0]['prompt'])
            self.assertIn('"lamp"',prompts['ir_calls'][0]['prompt'])
            self.assertEqual(prompts['adapter_calls'],[])
            self.assertEqual(prompts['ir_adapter_calls'],[])
            self.assertNotIn('Frozen interface',prompts['direct_calls'][0]['prompt'])

    def test_failed_first_model_does_not_gain_feedback_in_ablation(self):
        with tempfile.TemporaryDirectory(dir=runner.ROOT) as folder:
            rows={r['arm']:r for r in self.run_case(Path(folder),reject_first_model=True)}
        self.assertTrue(rows['workflow']['compile_ok'])
        self.assertEqual(rows['workflow']['llm_calls'],3)
        self.assertFalse(rows['no_sysml_feedback']['compile_ok'])
        self.assertEqual(rows['no_sysml_feedback']['llm_calls'],2)

    def test_planner_failure_is_retained_for_all_four_arms(self):
        FixtureCalls.responses['contract_calls']=json.dumps({'unsupported':['unimplemented device']})
        with tempfile.TemporaryDirectory(dir=runner.ROOT) as folder:
            rows=self.run_case(Path(folder))
        self.assertEqual(len(rows),6)
        for r in rows:
            if r['arm'] in runner.CONTRACT_ARMS:
                self.assertEqual(r['failure_stage'],'interface_contract')
                self.assertEqual(r['llm_calls'],3)
                self.assertFalse(r['compile_ok'])
        self.assertEqual({c.folder.name for c in FixtureCalls.instances},{'direct_calls','contract_calls'})

    def test_invalid_public_contract_does_not_silently_replan(self):
        case=copy.deepcopy(CASE);case['interface_contract']=contract()
        case['interface_contract']['board']='arduino:avr:mega'
        with tempfile.TemporaryDirectory(dir=runner.ROOT) as folder:
            rows=self.run_case(Path(folder),case)
        self.assertTrue(all(r.get('failure_stage')=='interface_contract' for r in rows if r['arm'] in runner.CONTRACT_ARMS))
        self.assertEqual(sum(len(c.entries) for c in FixtureCalls.instances if c.folder.name=='contract_calls'),0)

    def test_supplied_contract_freezes_same_semantics_without_mutating_input(self):
        case=copy.deepcopy(CASE);case['interface_contract']=contract()
        original=copy.deepcopy(case)
        with tempfile.TemporaryDirectory(dir=runner.ROOT) as folder:
            root=Path(folder);rows=self.run_case(root,case)
            frozen=json.loads((root/'fixture'/'repeat_1'/'contract.json').read_text(encoding='utf-8'))
        self.assertEqual(case,original)
        self.assertEqual(frozen,runner.freeze_model_semantics(contract()))
        for row in rows:
            if row['arm'] in runner.CONTRACT_ARMS:
                self.assertEqual(row['contract_sha256'],runner.contract_digest(frozen))
        self.assertEqual(sum(len(c.entries) for c in FixtureCalls.instances if c.folder.name=='contract_calls'),0)

    def test_compile_failure_cannot_trigger_free_form_adapter_repair(self):
        case={**CASE,'interface_contract':contract()}
        with tempfile.TemporaryDirectory(dir=runner.ROOT) as folder:
            root=Path(folder);calls=FixtureCalls(root/'must_not_call','fixture')
            with patch.object(runner,'compile_attempt',return_value=({'compile_ok':False,'compile_status':'failed'},'compiler error')) as compiler:
                result=runner.finish_code(case,root,calls,'immutable sketch',2,True)
        self.assertEqual(compiler.call_count,1)
        self.assertFalse(result['compile_ok'])
        self.assertEqual(calls.entries,[])


if __name__=='__main__': unittest.main()
