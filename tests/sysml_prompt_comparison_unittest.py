import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
import run_sysml_prompt_comparison as runner


class PromptComparisonTests(unittest.TestCase):
    def test_infrastructure_pairs_are_excluded_and_real_model_failures_remain(self):
        with tempfile.TemporaryDirectory() as folder:
            root=Path(folder)
            cases=['fixture_a','fixture_b']
            for case in cases:
                for variant in runner.VARIANTS:
                    path=root/case/variant;path.mkdir(parents=True)
                    row=dict(case=case,variant=variant,initial_model_ok=False,model_ok=False,compile_ok=False,functional_status='not_generated_or_compile_failed')
                    if case=='fixture_a' and variant==runner.VARIANTS[1]:
                        row.update(initial_model_ok=True,model_ok=True,compile_ok=True,functional_status='passed')
                    if case=='fixture_b' and variant==runner.VARIANTS[1]:
                        row.update(infrastructure_error='APITimeoutError',functional_status='unverified')
                    (path/'result.json').write_text(json.dumps(row))
                    (path/'calls').mkdir();(path/'calls/calls.json').write_text(json.dumps([dict(seconds=1,usage=dict(total_tokens=10,cost=.01))]))
            with patch.object(runner,'CASES',cases):
                report=runner.summarize(root,dict(hashes={},scope='fixture',host_oracle_tasks=cases))
            self.assertTrue(report['generation_complete'])
            self.assertEqual(report['comparable_tasks'],1)
            self.assertEqual(report['infrastructure_records'],1)
            self.assertEqual(report['variants'][runner.VARIANTS[0]]['compile_passes'],0)
            self.assertEqual(report['variants'][runner.VARIANTS[1]]['compile_passes'],1)
            self.assertEqual(report['recovered'],['fixture_a'])
            self.assertEqual(report['variants'][runner.VARIANTS[1]]['usage']['reported_cost_usd'],.02)

    def test_method_tampering_prevents_a_pooled_report(self):
        with tempfile.TemporaryDirectory() as folder:
            with self.assertRaisesRegex(ValueError,'Method changed'):
                runner.summarize(Path(folder),dict(hashes={'tools/examples/sysml_prompt_example.json':'wrong'},scope='fixture',host_oracle_tasks=[]))


if __name__=='__main__':unittest.main()
