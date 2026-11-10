import json
from pathlib import Path
import tempfile
import unittest

from experiment.code.audit_contract_stages import audit, candidate_evidence, rejection


class StageAuditTests(unittest.TestCase):
    def test_behavior_rejection_is_not_parser_failure(self):
        attempt = dict(ok=False, stages=dict(sysml_generation=True, sysml_to_ir=True, ir_to_il=True),
                       coverage=dict(status='failed'), issues=[dict(code='CONTRACT_PERIODIC', stage='requirements')])
        self.assertEqual(rejection(attempt), 'public_requirements')
        self.assertEqual(candidate_evidence(attempt)['reference_validation'], 'passed')
        attempt['stages']['sysml_to_ir'] = False
        attempt['stages']['ir_to_il'] = None
        attempt['coverage']['status'] = 'not_run'
        self.assertEqual(rejection(attempt), 'sysml_to_ir')
        self.assertEqual(candidate_evidence(attempt)['ir_to_il'], 'not_reached')

    def test_missing_evidence_and_assembly_rejections_are_distinct(self):
        self.assertEqual(candidate_evidence(None)['reference_validation'], 'unrecorded')
        attempt = dict(ok=False, stages={}, issues=[dict(code='EXPRESSION_SYNTAX')])
        self.assertEqual(rejection(attempt), 'bundle_or_controller_assembly')
        self.assertEqual(candidate_evidence(attempt)['reference_validation'], 'not_reached')

    def test_all_planned_denominator_and_final_regression(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root/'execution.json').write_text(json.dumps(dict(cases=[dict(id='a'), dict(id='b')], repetitions=1)))
            folder = root/'a/repeat_1'
            folder.mkdir(parents=True)
            (folder/'sysml_generation_failures.json').write_text(json.dumps(dict(attempts=[
                dict(ok=False, stages=dict(sysml_generation=True, sysml_to_ir=True, ir_to_il=True), coverage=dict(status='failed')),
                dict(ok=False, stages={}, issues=[dict(code='BUNDLE_FORMAT')], coverage=dict(status='not_run'))])))
            report = audit(root)
            self.assertEqual(report['gates_all_planned_tasks']['first_candidate']['reference_validation']['observed_pass_rate'], .5)
            final = report['gates_all_planned_tasks']['final_candidate']['reference_validation']
            self.assertEqual((final['denominator'], final['passed'], final['failed'], final['not_reached'], final['unrecorded']), (2, 0, 0, 1, 1))
            self.assertTrue(report['cases'][0]['any_candidate_passed_reference'])
            self.assertEqual(report['accepted_models'], 0)


if __name__ == '__main__':
    unittest.main()
