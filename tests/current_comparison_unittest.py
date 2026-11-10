"""Check paired budgets, failure isolation and honest untested denominators."""
import json
from pathlib import Path
import sys
import subprocess
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'experiment/code'))
import run_current_comparison as runner

CASE = dict(id='fixture',dataset='internal',stratum='simple',split='test',fqbn='arduino:avr:uno',requirement='LED on')


class FixtureCalls:
    def __init__(self, folder, model):
        self.folder=folder;self.entries=[]
    def __call__(self, prompt):
        self.entries.append(dict(prompt=prompt,response='void setup(){} void loop(){}',seconds=1,usage={'total_tokens':10,'cost':.01}))
        return 'fixture',self.entries[-1]['response']


class CurrentComparisonTests(unittest.TestCase):
    def test_compile_timeout_retains_partial_stdout_and_stderr(self):
        with tempfile.TemporaryDirectory(dir=runner.ROOT) as folder:
            root=Path(folder)
            timeout=subprocess.TimeoutExpired('arduino-cli',180,output=b'compiler started',stderr=b'partial diagnostic')
            with patch('run_controlled_experiment.subprocess.run',side_effect=timeout):
                result,log=runner.compile_code(root,'void setup(){} void loop(){}',runner.CLI,'arduino:avr:uno')
            self.assertEqual(result['compile_status'],'timeout')
            self.assertIn('compiler started',log)
            self.assertIn('partial diagnostic',log)
            self.assertEqual((root/'compile.log').read_text(encoding='utf-8'),log)

    def test_transient_windows_reader_lock_retries_without_losing_existing_record(self):
        with tempfile.TemporaryDirectory(dir=runner.ROOT) as folder:
            path=Path(folder)/'progress.json'
            path.write_text('{"old":true}',encoding='utf-8')
            replace=Path.replace
            attempts=[]
            def locked(source,target):
                attempts.append(source)
                if len(attempts)<3:
                    self.assertEqual(json.loads(path.read_text(encoding='utf-8')),{'old':True})
                    raise PermissionError('Windows reader holds target')
                return replace(source,target)
            with patch.object(Path,'replace',locked), patch.object(runner.time,'sleep'):
                runner.save(path,{'complete':True})
            self.assertEqual(json.loads(path.read_text(encoding='utf-8')),{'complete':True})
            self.assertEqual(len(attempts),3)
            self.assertEqual(list(Path(folder).glob('*.tmp')),[])

    def test_compile_feedback_uses_same_first_draft_and_exact_six_call_limit(self):
        def checked(case,folder,code,receipt=None):
            return dict(compile_ok=False,compile_status='failed',functional_status='not_generated_or_compile_failed'), 'bad'
        with tempfile.TemporaryDirectory(dir=runner.ROOT) as folder:
            with patch.object(runner,'ResumableCalls',FixtureCalls), patch.object(runner,'compile_candidate',checked), \
                 patch.object(runner,'plan_interface_contract',side_effect=ValueError('unsupported device')):
                rows={r['arm']:r for r in runner.evaluate(CASE,1,Path(folder),'fixture')}
        self.assertEqual(rows['direct']['llm_calls'],1)
        self.assertEqual(rows['direct_feedback']['llm_calls'],6)
        self.assertEqual(rows['direct_feedback']['compile_repairs'],5)
        self.assertEqual(rows['workflow']['failure_stage'],'interface_contract')
        self.assertFalse(any(r.get('infrastructure_error') for r in rows.values()))

    def test_direct_transport_failure_does_not_erase_independent_workflow_failure(self):
        class BrokenCalls(FixtureCalls):
            def __call__(self,prompt):raise RuntimeError('LLM infrastructure failure: APIConnectionError')
        with tempfile.TemporaryDirectory(dir=runner.ROOT) as folder:
            with patch.object(runner,'ResumableCalls',BrokenCalls), \
                 patch.object(runner,'plan_interface_contract',side_effect=ValueError('unsupported device')):
                rows=runner.evaluate(CASE,1,Path(folder),'fixture')
        self.assertEqual(len(rows),3)
        self.assertTrue(rows[0]['infrastructure_error'])
        self.assertEqual(rows[2]['failure_stage'],'interface_contract')
        self.assertNotIn('infrastructure_error',rows[2])

    def test_capability_failure_is_saved_separately_without_fake_registry(self):
        failure=runner.ContractPlanningError([dict(ok=False,code='HARDWARE_CAPABILITY',error='unsupported sensor')])
        with tempfile.TemporaryDirectory(dir=runner.ROOT) as folder:
            root=Path(folder)
            with patch.object(runner,'ResumableCalls',FixtureCalls), \
                 patch.object(runner,'compile_candidate',return_value=(dict(compile_ok=True),'ok')), \
                 patch.object(runner,'plan_interface_contract',side_effect=failure):
                rows={r['arm']:r for r in runner.evaluate(CASE,1,root,'fixture')}
            self.assertEqual(rows['workflow']['blocking_category'],'hardware_capability')
            target=root/'fixture/repeat_1'
            self.assertEqual(json.loads((target/'contract_planning.json').read_text())['blocking_category'],'hardware_capability')
            self.assertFalse((target/'interface_registry.json').exists())

    def test_completed_job_is_replayed_without_new_model_calls(self):
        with tempfile.TemporaryDirectory(dir=runner.ROOT) as folder:
            root=Path(folder)
            expected=[dict(arm=a,compile_ok=False) for a in runner.ARMS]
            runner.save(root/'fixture/repeat_1/results.json',expected)
            with patch.object(runner,'ResumableCalls',side_effect=AssertionError('must not call model')):
                self.assertEqual(runner.evaluate(CASE,1,root,'fixture'),expected)

    def test_missing_records_and_no_oracle_are_not_reported_as_functional_success(self):
        with tempfile.TemporaryDirectory(dir=runner.ROOT) as folder:
            root=Path(folder)
            rows=[dict(case='fixture',dataset='internal',stratum='simple',split='test',repeat=1,arm=a,
                       compile_ok=True,compile_status='passed',functional_status='no_validated_oracle') for a in runner.ARMS]
            runner.save(root/'fixture/repeat_1/results.json',rows)
            config=dict(cases=[CASE],repetitions=2,call_limits=dict(direct=1,direct_feedback=6,workflow=6),hashes={},model='fixture')
            stats=runner.report(root,config)
        self.assertFalse(stats['generation_complete'])
        self.assertEqual(stats['missing_records'],3)
        self.assertEqual(stats['arms']['workflow']['functional']['all_tasks']['passes'],0)
        self.assertEqual(stats['arms']['workflow']['functional']['unresolved'],2)
        self.assertEqual(stats['contrasts'][0]['functional_observed']['task_repeat_pairs'],0)


if __name__=='__main__':unittest.main()
