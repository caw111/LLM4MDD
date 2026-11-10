import copy
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
import behavior_full_host as host
import run_behavior_all296 as gate
from operational_profile import acceptance_profile


class ValidationProtocolTests(unittest.TestCase):
    def test_reference_repair_review_receives_the_same_execution_semantics(self):
        plan=dict(supported=True,coverage='full',horizon_ms=1000,
            checks=[dict(kind='gpio',id='x',requirement_quote='LED',pin=13,at_ms=100,expected=1)],
            reference_cpp='void setup(){}void loop(){}')
        review=dict(approved=True,coverage='full',issues=[],coverage_gaps=[])
        with tempfile.TemporaryDirectory() as td,patch.object(gate,'OUT',Path(td)),patch.object(host,'case_files'),\
             patch.object(host,'execute',side_effect=[dict(status='failed',checks=[]),dict(status='passed',evidence='ref'),dict(status='failed',evidence='mutant')]),\
             patch.object(gate,'call',side_effect=[plan,review]) as call:
            result=gate.one('fixture',dict(status='design_review_approved',requirement='LED',coverage='full',plan=plan),
                [dict(dataset='internal',board='arduino:avr:uno',compile_ok=False)],'test')
        self.assertEqual(result['readiness'],'ready')
        self.assertIn('AFTER that resolved start operation',call.call_args_list[1].args[4])
        self.assertIn('end-of-millisecond API state',call.call_args_list[1].args[4])

    def test_unapproved_coverage_suggestions_cannot_override_independent_partial_review(self):
        import design_behavior_tests as design
        with tempfile.TemporaryDirectory() as td,patch.object(design,'OUT',Path(td)):
            folder=Path(td)/'test_designs_v2/fixture';folder.mkdir(parents=True)
            (folder/'coverage_proposal.json').write_text(json.dumps(dict(checks=['source-derived suggestion'])))
            plan=dict(supported=True,coverage='full',horizon_ms=1000,
                checks=[dict(kind='gpio',id='x',requirement_quote='LED',pin=13,at_ms=100,expected=1)],
                reference_cpp='void setup(){}void loop(){}')
            review=dict(approved=True,coverage='partial',coverage_gaps=[],issues=[])
            with patch.object(design,'call',side_effect=[plan,review]) as call:
                result=design.one(dict(id='fixture',effective_requirement='LED'))
            self.assertEqual(result['coverage'],'partial')
            self.assertIn('Unapproved source-derived coverage suggestions',call.call_args_list[0].args[4])
            reviewer_prompt=call.call_args_list[1].args[4]
            self.assertIn('AUTHORITATIVE EXECUTABLE CHECK SCHEMAS',reviewer_prompt)
            self.assertIn('AFTER that resolved start operation',reviewer_prompt)
            self.assertIn('end-of-millisecond API state',reviewer_prompt)

    def test_serial_assertions_observe_required_baud_instead_of_just_payload(self):
        plan=dict(horizon_ms=12000,checks=[dict(kind='serial',id='baud',requirement_quote='9600 baud',
            contains='Hello',baud=9600,min_count=1)])
        execution=self.trace()[-1]
        for baud,expected in [(9600,'passed'),(115200,'failed')]:
            trace=[dict(type='serial',at_ms=100,text='Hello',baud=baud),execution]
            self.assertEqual(host.evaluate(plan,trace,'arduino:avr:uno')['status'],expected)

    def test_large_frozen_suites_can_add_checks_without_deleting_old_assertions(self):
        from design_behavior_tests import validate,MAX_CHECKS
        plan=dict(supported=True,coverage='full',horizon_ms=1000,input_steps=[],
            checks=[dict(kind='gpio',id=str(i),requirement_quote='LED',pin=13,at_ms=100,expected=1) for i in range(MAX_CHECKS)],
            reference_cpp='void setup(){} void loop(){}')
        validate(plan,'LED')
        plan['checks'].append(dict(plan['checks'][0],id='over_limit'))
        with self.assertRaises(AssertionError):validate(plan,'LED')

    def test_coverage_patch_extends_seed_without_changing_its_inputs_or_reference(self):
        from design_behavior_tests import merge_seed_patch
        seed=dict(supported=True,horizon_ms=12000,checks=[dict(kind='gpio',id='old',pin=13,at_ms=100,expected=1)],
            input_steps=[],reference_cpp='original reference')
        patch=dict(additional_checks=[dict(kind='setup',id='startup',max_ms=500)],coverage='full',
            coverage_explanation='Adds setup check',horizon_ms=12000)
        actual=merge_seed_patch(seed,patch)
        self.assertEqual(actual['checks'][0],seed['checks'][0])
        self.assertEqual(actual['reference_cpp'],seed['reference_cpp'])
        self.assertEqual(len(seed['checks']),1)
        wrong=dict(patch,additional_checks=[dict(seed['checks'][0])])
        with self.assertRaises(ValueError):merge_seed_patch(seed,wrong)

    def test_seeded_round_can_add_coverage_but_cannot_remove_assertions_or_change_stimuli(self):
        from design_behavior_tests import preserve_seed
        seed=dict(supported=True,input_steps=[dict(at_ms=0,kind='digital',pin=2,value=0)],horizon_ms=12000,
            checks=[dict(kind='gpio',id='old',pin=13,at_ms=1000,expected=1)])
        expanded=copy.deepcopy(seed)
        expanded['checks'].append(dict(kind='setup',id='startup',max_ms=500))
        preserve_seed(expanded,seed)
        for field in ['checks','input_steps']:
            wrong=copy.deepcopy(expanded);wrong[field]=[]
            with self.assertRaises(ValueError):preserve_seed(wrong,seed)

    def test_setup_deadline_is_an_executable_assertion_not_a_review_assumption(self):
        plan=dict(horizon_ms=12000,checks=[dict(kind='setup',id='startup',
            requirement_quote='setup returns within 500ms',max_ms=500)])
        self.assertEqual(host.evaluate(plan,self.trace(setup=500),'arduino:avr:uno')['status'],'passed')
        self.assertEqual(host.evaluate(plan,self.trace(setup=501),'arduino:avr:uno')['status'],'failed')
        self.assertEqual(host.evaluate(plan,self.trace(initialized=False),'arduino:avr:uno')['status'],'failed')

    def plan(self):
        return dict(horizon_ms=12000,checks=[dict(kind='gpio',id='on',requirement_quote='on after initialization',
            origin='setup_complete',pin=13,at_ms=20,expected=1),dict(kind='edges',id='hold',requirement_quote='stays on',
            origin='setup_complete',pin=13,from_ms=21,to_ms=100,min_count=0,max_count=0)])

    def trace(self,setup=5000,level=1,initialized=True):
        return [dict(type='gpio',at_ms=setup+15,pin=13,value=level,pwm=False),
            dict(type='execution',setup_at_ms=setup,setup_returned=initialized,virtual_ms=12000,status='completed')]

    def test_setup_relative_deadline_and_hold_do_not_assume_absolute_startup(self):
        plan=self.plan();before=copy.deepcopy(plan)
        self.assertEqual(host.evaluate(plan,self.trace(),'arduino:avr:uno')['status'],'passed')
        self.assertEqual(plan,before)
        self.assertEqual(host.evaluate(plan,self.trace(level=0),'arduino:avr:uno')['status'],'failed')

    def test_unobserved_initialization_or_window_is_unknown_not_a_false_failure(self):
        self.assertEqual(host.evaluate(self.plan(),self.trace(initialized=False),'arduino:avr:uno')['status'],'unverified_window')
        self.assertEqual(host.evaluate(self.plan(),self.trace(setup=11990),'arduino:avr:uno')['status'],'unverified_window')

    def test_all_off_requirement_can_use_an_all_high_negative_control(self):
        with tempfile.TemporaryDirectory() as td,patch.object(gate,'OUT',Path(td)),patch.object(host,'case_files'),patch.object(host,'execute',side_effect=[
            {'status':'passed','evidence':'ref'}, {'status':'passed','evidence':'valid_noop'}, {'status':'failed','evidence':'wrong_high'}]) as execute:
            result=gate.one('off',dict(status='design_review_approved',requirement='LED off',coverage='full',
                plan={'reference_cpp':'void setup(){} void loop(){}'}),[dict(dataset='internal',board='arduino:avr:uno',compile_ok=False)],'test')
            self.assertEqual(result['readiness'],'ready')
            self.assertEqual(execute.call_count,3)
            self.assertTrue((Path(td)/'cases/off/all_high_mutant.cpp').exists())

    def test_profile_is_disclosed_and_mapping_is_not_imposed_on_other_tasks(self):
        self.assertIn('additional public experimental conditions',acceptance_profile('LED remains ON'))
        self.assertNotIn('floor(ADC',acceptance_profile('根据按钮切换温度显示'))
        self.assertIn('floor(ADC',acceptance_profile('读取连接在 A0 引脚的电位器，根据数值控制 D9 PWM LED 的亮度。'))


if __name__=='__main__':unittest.main()
