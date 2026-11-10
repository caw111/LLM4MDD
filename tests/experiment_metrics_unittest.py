import unittest
from experiment.code.experiment_metrics import functional_rates, paired, summarize, first_response_entries
from app.services.failure_analysis import classify_issue


class MetricsTests(unittest.TestCase):
    def test_missing_backend_evidence_is_not_a_zero_pass_rate(self):
        rows=[dict(case='a',repeat=1,arm='workflow',compile_ok=True,functional_status='failed')]
        result=summarize(rows)['arms']['workflow']
        self.assertIsNone(result['backend']['rate'])
        self.assertEqual(result['functional_failures_after_compile'],1)
        rows[0]['backend_ok']=True
        result=summarize(rows)['arms']['workflow']
        self.assertEqual(result['backend']['rate'],1)
        self.assertEqual(result['backend']['recorded'],1)
    def test_compilation_failures_remain_in_all_task_denominator(self):
        rows = [dict(compile_ok=True, functional_status='passed'), dict(compile_ok=True, functional_status='simulation_failed'),
                dict(compile_ok=False, functional_status='not_generated_or_compile_failed')]
        result = functional_rates(rows)
        self.assertEqual(result['all_tasks']['rate'], 1/3)
        self.assertEqual(result['compiled_only']['rate'], 1/2)
        rows.append(dict(compile_ok=True, functional_status='no_validated_oracle'))
        self.assertEqual(functional_rates(rows)['unresolved'], 1)

    def test_pair_only_same_task_and_repeat(self):
        rows = [dict(case='a', repeat=1, arm='x', compile_ok=True), dict(case='a', repeat=2, arm='y', compile_ok=False)]
        self.assertEqual(paired(rows, 'x', 'y', 'compile')['task_repeat_pairs'], 0)
        rows.append(dict(case='a', repeat=1, arm='y', compile_ok=False))
        result = paired(rows, 'x', 'y', 'compile')
        self.assertEqual(result['difference'], 1)
        self.assertEqual(result['ci95'], [1, 1])

    def test_repair_success_excludes_initial_passes(self):
        reports = [{'attempts': [{'ok': True}]}, {'attempts': [{'ok': False}, {'ok': True}]}, {'attempts': [{'ok': False}, {'ok': False}]}]
        self.assertEqual(summarize([], reports)['model']['repair_success']['rate'], .5)

    def test_infrastructure_and_untested_behavior_not_scored_as_method_failures(self):
        rows = [dict(case='a', repeat=1, arm='x', compile_ok=True, functional_status='pending'),
                dict(case='a', repeat=1, arm='y', compile_ok=True, functional_status='passed')]
        self.assertEqual(paired(rows, 'x', 'y', 'functional')['task_repeat_pairs'], 0)
        rows[0]['infrastructure_error'] = 'timeout'
        self.assertEqual(paired(rows, 'x', 'y', 'compile')['task_repeat_pairs'], 0)

    def test_language_and_lowering_are_distinct(self):
        self.assertEqual(classify_issue(dict(code='Syntax', message='bad syntax'))[0], 'sysml_generation')
        self.assertEqual(classify_issue(dict(code='SYSML_LOWERING', message='missing driver'))[0], 'sysml_to_ir')
        self.assertEqual(classify_issue(dict(code='IR_BACKEND', message='parser rejection'))[0], 'ir_to_code')

    def test_unstarted_tasks_remain_in_denominator_without_becoming_failures(self):
        rows = [dict(case='a', repeat=1, arm='direct', compile_ok=True, functional_status='passed')]
        result = summarize(rows, expected_keys={('a', 1, 'direct'), ('b', 1, 'direct')})
        self.assertEqual(result['records'], 1)
        self.assertEqual(result['planned_records'], 2)
        self.assertEqual(result['arms']['direct']['compile']['rate'], .5)
        self.assertEqual(result['arms']['direct']['functional']['all_tasks']['rate'], .5)
        self.assertEqual(result['arms']['direct']['functional']['unresolved'], 1)
        self.assertEqual(result['arms']['direct']['failures'], {})
        self.assertEqual(len(rows), 1)  # Do not mutate archived raw records.

    def test_missing_pairs_and_unexpected_records(self):
        result = summarize([], expected_keys={('a', 1, 'direct'), ('a', 1, 'direct_feedback')})
        contrast = result['contrasts'][0]['compile']
        self.assertEqual(contrast['task_repeat_pairs'], 0)
        self.assertEqual(contrast['unresolved_pairs'], 1)
        with self.assertRaises(ValueError):
            summarize([dict(case='test', repeat=1, arm='direct')], expected_keys=set())

    def test_first_candidate_cost_includes_transport_retries_but_no_repairs(self):
        entries = [{'error_type': 'Timeout'}, {'error_type': 'Connection'}, {'response': 'first'}, {'response': 'repair'}]
        self.assertEqual(first_response_entries(entries), entries[:3])
        self.assertEqual(first_response_entries(entries[:2]), entries[:2])


if __name__ == '__main__':
    unittest.main()
