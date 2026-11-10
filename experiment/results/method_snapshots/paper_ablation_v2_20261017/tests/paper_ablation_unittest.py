"""Ablation interventions must not borrow repaired successes or double-charge calls."""
import copy
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
import run_paper_ablation as run


def entry(text, response='{}'):
    return dict(prompt=text,response=response,actual_model='fixture',seconds=1,
                usage=dict(total_tokens=10,cost=.01))


class AblationTests(unittest.TestCase):
    def setUp(self):
        self.full=dict(case='fixture',repeat=1,arm='workflow',compile_ok=True,compile_status='passed',
            artifact='original.ino',artifact_sha256='digest',functional_status='passed',backend_ok=True)
        self.entries=[entry('plan'),entry('plan repair'),entry('model'),entry('model repair')]

    def test_each_removed_feedback_stops_at_its_own_original_failure(self):
        rows=run.derive_feedback_ablations(self.full,self.entries,
            dict(attempts=[dict(ok=False),dict(ok=True)]),
            dict(attempts=[dict(ok=False,failure_stage='requirements'),dict(ok=True)]))
        plan,model,_=rows
        self.assertFalse(plan['compile_ok']);self.assertFalse(model['compile_ok'])
        self.assertEqual(plan['method_calls'],1);self.assertEqual(model['method_calls'],3)
        self.assertEqual(model['failure_stage'],'requirements')
        for row in (plan,model):
            self.assertNotIn('artifact',row);self.assertNotIn('backend_ok',row)
            self.assertNotEqual(row['functional_status'],'passed')
        self.assertTrue(self.full['compile_ok'])

    def test_valid_first_attempt_shares_exact_downstream_artifact(self):
        plan,model,_=run.derive_feedback_ablations(self.full,[entry('plan'),entry('model')],
            dict(attempts=[dict(ok=True)]),dict(attempts=[dict(ok=True)]))
        for row in (plan,model):
            self.assertTrue(row['compile_ok']);self.assertEqual(row['artifact'],'original.ino')
            self.assertEqual(row['method_calls'],2)

    def test_planning_exhaustion_does_not_invent_model_calls(self):
        full={**self.full,'compile_ok':False,'failure_stage':'interface_contract'}
        plan,model,_=run.derive_feedback_ablations(full,self.entries[:2],dict(attempts=[dict(ok=False)]*2),None)
        self.assertEqual(plan['method_calls'],1);self.assertEqual(model['method_calls'],2)
        self.assertEqual(model['failure_stage'],'interface_contract')

    def test_transport_attempts_are_recorded_without_spending_method_responses(self):
        entries=[dict(error_type='APIConnectionError',seconds=1),entry('same'),entry('second')]
        prefix=run.response_prefix(entries,1)
        self.assertEqual(len(prefix),2);self.assertEqual(run.usage(prefix)['method_calls'],1)
        self.assertEqual(run.usage(prefix)['llm_calls'],2)
        with self.assertRaises(ValueError):run.response_prefix(entries,3)

    def test_blind_sampler_never_sends_diagnostics_or_previous_candidate(self):
        seen=[]
        def fake(self,prompt):
            seen.append(prompt)
            return 'fixture','{}'
        with tempfile.TemporaryDirectory(dir=run.ROOT) as directory:
            with patch.object(run.ExperimentCalls,'__call__',fake):
                calls=run.BlindCalls(Path(directory),'fixture')
                calls('original');calls('previous candidate and secret diagnostic')
        self.assertEqual(seen,['original','original'])

    def test_seed_reused_only_when_actual_prompt_is_identical(self):
        with tempfile.TemporaryDirectory(dir=run.ROOT) as directory:
            with patch.object(run.ExperimentCalls,'__call__',return_value=('fixture','{}')):
                a=run.BlindCalls(Path(directory)/'a','fixture',[entry('original')],'source')
                a('original')
                b=run.BlindCalls(Path(directory)/'b','fixture',[entry('other')],'source')
                b('original')
        self.assertEqual(a.entries[0]['shared_call_ref'],'source')
        self.assertEqual(b.entries,[])

    def test_backend_not_generated_when_upstream_has_no_accepted_model(self):
        full={**self.full,'compile_ok':False,'failure_stage':'sysml_generation'}
        case=dict(id='fixture',dataset='internal',stratum='simple')
        with tempfile.TemporaryDirectory(dir=run.ROOT) as directory:
            folder=Path(directory)
            with patch.object(run.ExperimentCalls,'__call__',side_effect=AssertionError('No request allowed')):
                row=run.llm_backend(case,1,folder/'backend','fixture',full,self.entries,folder)
        self.assertFalse(row['compile_ok']);self.assertEqual(row['arm'],'llm_backend')
        self.assertTrue(row['shared_upstream_failure'])

    def test_llm_backend_can_recover_a_deterministic_backend_failure(self):
        case=dict(id='fixture',dataset='internal',stratum='simple',requirement='public requirement')
        with tempfile.TemporaryDirectory(dir=run.ROOT) as directory:
            folder=Path(directory);run.save(folder/'model.json',dict(system_sysml2='accepted'))
            run.save(folder/'contract.json',dict(interfaces=[]))
            full={**self.full,'compile_ok':False,'failure_stage':'ir_to_code'}
            with patch.object(run.ExperimentCalls,'__call__',return_value=('fixture','void setup(){} void loop(){}')), \
                 patch.object(run.production,'compile_candidate',return_value=(dict(compile_ok=True,compile_status='passed',functional_status='no_validated_oracle'),'')):
                row=run.llm_backend(case,1,folder/'backend','fixture',full,self.entries,folder)
        self.assertTrue(row['compile_ok']);self.assertTrue(row['model_accepted'])

    def test_fixed_cohort_and_repetitions_are_not_selected_by_prior_results(self):
        cases=run.ordered_cases()
        self.assertEqual(len(cases),296)
        self.assertEqual(Counter(c['dataset'] for c in cases),dict(internal=120,PCEval=50,EmbedBench=126))

    def test_complete_audit_checks_all_planned_arms_and_writes_real_counts(self):
        case=dict(id='fixture',dataset='internal',stratum='simple')
        config=dict(cases=[case],repetitions=1,phase='pilot',model='fixture',hashes={})
        with tempfile.TemporaryDirectory(dir=run.ROOT) as directory:
            root=Path(directory)
            rows=[{**run.blank(case,1,a),'llm_calls':0,'tokens':0,'reported_cost_usd':0,'llm_seconds':0,
                   'compile_ok':a=='workflow','compile_status':'passed' if a=='workflow' else 'not_generated',
                   'functional_status':'no_validated_oracle'} for a in run.ARMS]
            run.save(root/'jobs/fixture/repeat_1/ablation_results.json',rows)
            result=run.summarize_run(root,config)
            self.assertTrue(result['complete']);self.assertEqual(result['records'],7)
            self.assertEqual(result['arms']['workflow']['compile']['passes'],1)
            run.save(root/'jobs/fixture/repeat_1/ablation_results.json',rows[:-1])
            result=run.summarize_run(root,config)
            self.assertFalse(result['complete']);self.assertEqual(result['missing_records'],1)


from collections import Counter
if __name__=='__main__':unittest.main()
