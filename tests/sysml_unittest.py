import unittest
import json
from app.services.sysml import validate_sysml, generate_validated_bundle, SysMLValidationError, SysMLToolError

class ReferenceValidatorTests(unittest.TestCase):
    def test_reference_validator_and_diagnostics(self):
        valid = validate_sysml('package P { private import ScalarValues::*; part def Sensor { out attribute level : Real; } part sensor : Sensor; }')
        self.assertTrue(valid['ok'], valid['issues'])
        self.assertTrue(any(e['kind'] == 'PartDefinition' and e['name'] == 'Sensor' for e in valid['elements']))
        syntax = validate_sysml('package P { part def Broken {')
        self.assertFalse(syntax['ok'])
        self.assertTrue(any(e['syntax'] for e in syntax['issues']))
        unresolved = validate_sysml('package P { part p : MissingType; }')
        self.assertFalse(unresolved['ok'])
        self.assertTrue(any('MissingType' in e['message'] for e in unresolved['issues']))

class RepairLoopTests(unittest.TestCase):
    def bundle(self, source):
        return json.dumps({'system_sysml2': source, 'req_test_map': {'requirements_to_testpoints': [
            {'requirement_id': 'R1', 'requirement_text': 'threshold alarm', 'test_point_id': 'T1',
             'test_method': 'unit-test', 'stimulus': 'input=80', 'oracle': 'alarm=true'}]}})

    def test_actual_diagnostics_are_sent_back_and_repair_is_validated(self):
        prompts = []
        responses = iter([self.bundle('package Broken {'), self.bundle(
            'package P { private import ScalarValues::*; part def Controller { '
            'in attribute input : Real; out attribute alarm : Boolean = input > 70.0; } }')])
        def call(prompt):
            prompts.append(prompt)
            return 'test-model', next(responses)
        _, bundle, report = generate_validated_bundle(call, 'threshold alarm', max_repairs=1)
        self.assertEqual(len(prompts), 2)
        self.assertIn('package Broken {', prompts[1])
        self.assertIn('Diagnostics', prompts[1])
        self.assertFalse(report['attempts'][0]['ok'])
        self.assertTrue(report['attempts'][1]['ok'])
        self.assertIn('Controller', bundle['system_sysml2'])

    def test_json_fence_is_unwrapped_but_model_still_validated(self):
        calls = []
        def validator(source):
            calls.append(source)
            return {'ok': False, 'issues': [{'code': 'SYNTAX', 'message': 'bad model'}]}
        with self.assertRaises(SysMLValidationError) as caught:
            generate_validated_bundle(lambda _: ('test', '```json\n' + self.bundle('broken') + '\n```'),
                                      'p', max_repairs=0, validator=validator)
        self.assertEqual(calls, ['broken'])
        self.assertEqual(caught.exception.attempts[0]['issues'][0]['code'], 'SYNTAX')

    def test_lowering_failure_includes_actionable_feedback_and_revalidates(self):
        prompts, validated = [], []
        def call(prompt):
            prompts.append(prompt)
            return 'test', self.bundle('bad sequence' if len(prompts) == 1 else 'fixed sequence')
        def validator(source):
            validated.append(source)
            return {'ok': source == 'fixed sequence', 'issues': [] if source == 'fixed sequence' else [
                {'code': 'SYSML_LOWERING', 'message': 'All performed statements must belong to a single chain beginning at start.'}]}
        _, bundle, report = generate_validated_bundle(call, 'keep timing and both outputs', max_repairs=1, validator=validator)
        self.assertEqual(validated, ['bad sequence', 'fixed sequence'])
        self.assertIn('keep timing and both outputs', prompts[1])
        self.assertIn('bad sequence', prompts[1])
        self.assertIn('first start; then assign', prompts[1])
        self.assertEqual([item['ok'] for item in report['attempts']], [False, True])
        self.assertEqual(bundle['system_sysml2'], 'fixed sequence')

    def test_invalid_repair_budget_does_not_call_model(self):
        def call(_):
            self.fail('Invalid budget must fail before a model call')
        for budget in (-1, 6, True, 1.5):
            with self.subTest(budget=budget), self.assertRaises(ValueError):
                generate_validated_bundle(call, 'p', max_repairs=budget)

    def test_retry_budget_and_tool_failures(self):
        calls = []
        def invalid(prompt):
            calls.append(prompt)
            return 'test', '{}'
        with self.assertRaises(SysMLValidationError) as ctx:
            generate_validated_bundle(invalid, 'p', max_repairs=1)
        self.assertEqual(len(ctx.exception.attempts), 2)
        self.assertEqual(len(calls), 2)
        def unavailable(_):
            raise SysMLToolError('unavailable')
        with self.assertRaises(SysMLToolError):
            generate_validated_bundle(lambda _: ('test', self.bundle('package P;')), 'p', validator=unavailable)

if __name__ == '__main__':
    unittest.main()
