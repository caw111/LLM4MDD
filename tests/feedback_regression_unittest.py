import copy
import json
import unittest

from app.services.feedback_regression import check_public_button_contract, check_startup
from app.services.interface_planner import plan_interface_contract
from app.services.sysml_semantic_review import build_review_context, candidate_digest, review_prompt


class FeedbackRegressionTests(unittest.TestCase):
    def test_wrong_button_mapping_rejected_and_corrected_contract_replayed(self):
        source='D2 按钮控制 D13 LED。\nOperational acceptance profile operational-acceptance-v1 (public):\nbuttons connect to GND'
        contract={'interfaces':[{'binding':{'kind':'digital_input','pin':'D2','active_high':True,'pullup':False}}]}
        original=copy.deepcopy(contract)
        with self.assertRaisesRegex(ValueError,'BUTTON_ELECTRICAL_REGRESSION'):
            check_public_button_contract(contract,source)
        self.assertEqual(contract,original)
        contract['interfaces'][0]['binding'].update(active_high=False,pullup=True)
        check_public_button_contract(contract,source)
        check_public_button_contract(original,'D2 button active HIGH with external pulldown')

    def test_runtime_evidence_not_reviewer_prose_controls_startup_acceptance(self):
        self.assertEqual(check_startup(None)['status'],'unverified')
        self.assertEqual(check_startup(dict(setup_returned=True,setup_at_ms=500))['status'],'passed')
        self.assertEqual(check_startup(dict(setup_returned=True,setup_at_ms=501))['status'],'failed')
        self.assertEqual(check_startup(dict(setup_returned=False,setup_at_ms=0))['status'],'failed')

    def test_model_review_scope_is_bound_to_candidate_and_not_global_relaxation(self):
        args=dict(original_requirement='Initialize sensor and report.',contract={},system_sysml2='package P {}')
        strict=build_review_context(**args)
        staged=build_review_context(**args,review_scope='model_boundary')
        self.assertNotEqual(candidate_digest(strict),candidate_digest(staged))
        self.assertIn('not final code acceptance',str(review_prompt(staged)))
        self.assertNotIn('not final code acceptance',str(review_prompt(strict)))

    def test_planner_preserves_appended_acceptance_conditions(self):
        from app.services.configuration_policy import resolve_configuration
        resolution=resolve_configuration('点亮 D13 LED。')
        before=copy.deepcopy(resolution)
        requirement=resolution['effective_requirement']+'\nFrozen extra acceptance condition.'
        prompts=[]
        def call(prompt):
            prompts.append(str(prompt))
            return 'fixture',json.dumps(dict(interface_requests=[dict(kind='digital_output',pin='D13',initial=False,meaning='LED')]))
        plan_interface_contract(call,requirement,{},configuration_resolution=resolution,max_repairs=0)
        self.assertIn('Frozen extra acceptance condition.',prompts[0])
        self.assertEqual(resolution,before)

    def test_conditional_profile_devices_do_not_invent_hardware_requirements(self):
        from app.services.configuration_policy import resolve_configuration
        from app.services.interface_planner import check_explicit_requirements
        source='点亮 D13 LED。'
        resolution=resolve_configuration(source)
        def call(prompt):
            return 'fixture',json.dumps(dict(interface_requests=[dict(kind='digital_output',pin='D13',initial=False,meaning='LED')]))
        contract,_=plan_interface_contract(call,source,{},configuration_resolution=resolution,max_repairs=0)
        check_explicit_requirements(contract,resolution['effective_requirement']+
            '\nOperational acceptance profile operational-acceptance-v1 (public):\nservo commands and DHT defaults apply only if required.',{})


if __name__=='__main__':unittest.main()
