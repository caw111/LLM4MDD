import copy
import unittest
from unittest.mock import Mock, patch

from app.services.sysml import SysMLValidationError
from app.services.verified_generation import generate_verified_system


def failure(category='contract_semantic_issue', valid=True):
    exc=SysMLValidationError([dict(failure_stage='semantic_review')])
    exc.semantic_review=dict(failure_category=category,rounds=[dict(validation={'valid':valid},
        result={'issues':[dict(target='contract',certainty='confirmed',reason='Wrong active level')]} )])
    return exc


class VerifiedGenerationTests(unittest.TestCase):
    def test_contract_issue_replans_and_revalidates_without_mutating_prior_contract(self):
        first={'interfaces':[{'binding':{'active_high':True}}]};original=copy.deepcopy(first)
        second={'interfaces':[{'binding':{'active_high':False}}]}
        planner=Mock(side_effect=[(first,{}),(second,{})])
        generator=Mock(side_effect=[failure(),('model',{'system_sysml2':'new'}, {'ok':True})])
        snapshots=[]
        with patch('app.services.verified_generation.plan_interface_contract',planner),patch('app.services.verified_generation.generate_validated_bundle',generator),patch('app.services.verified_generation.sysml_bundle_prompt',return_value='prompt'):
            c,_,_,report=generate_verified_system(Mock(),Mock(),'source',{},persist=lambda n,r:snapshots.append(r))
        self.assertEqual(c,second);self.assertEqual(first,original)
        self.assertEqual(report['contract_revisions'],1)
        self.assertEqual(planner.call_args.kwargs['previous'],first)
        self.assertEqual(planner.call_args.kwargs['diagnostic_feedback']['source'],'validated_independent_semantic_review')
        self.assertEqual(generator.call_args.kwargs['contract'],second)
        self.assertEqual(len(snapshots),4)

    def test_requirement_or_infrastructure_issue_does_not_rewrite_contract(self):
        for category in ['requirement_semantic_issue','semantic_review_error','repair_budget_exhausted']:
            with self.subTest(category=category),patch('app.services.verified_generation.plan_interface_contract',return_value=({},{})) as planner,patch('app.services.verified_generation.generate_validated_bundle',side_effect=failure(category)),patch('app.services.verified_generation.sysml_bundle_prompt',return_value='prompt'):
                with self.assertRaises(SysMLValidationError):generate_verified_system(Mock(),Mock(),'source',{})
                self.assertEqual(planner.call_count,1)

    def test_invalid_or_budget_exhausted_findings_never_trigger_unbounded_replanning(self):
        for valid, budget in [(False,1),(True,0)]:
            with patch('app.services.verified_generation.plan_interface_contract',return_value=({},{})) as planner,patch('app.services.verified_generation.generate_validated_bundle',side_effect=failure(valid=valid)),patch('app.services.verified_generation.sysml_bundle_prompt',return_value='prompt'):
                with self.assertRaises(SysMLValidationError):generate_verified_system(Mock(),Mock(),'source',{},max_contract_revisions=budget)
                self.assertEqual(planner.call_count,1)

    def test_runtime_feedback_is_data_and_source_requirement_stays_unchanged(self):
        with patch('app.services.verified_generation.plan_interface_contract',return_value=({},{})) as planner,patch('app.services.verified_generation.generate_validated_bundle',return_value=('m',{},{})) as generator,patch('app.services.verified_generation.sysml_bundle_prompt',return_value='prompt'):
            generate_verified_system(Mock(),Mock(),'source',{},diagnostic_feedback={'actual':0,'expected':1})
            self.assertEqual(planner.call_args.args[1],'source')
            self.assertEqual(generator.call_args.kwargs['original_requirement'],'source')
            self.assertIn('counterexample',generator.call_args.args[1])


if __name__=='__main__':unittest.main()
