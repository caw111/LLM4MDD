"""Transport format, interrupted completion and replay acceptance boundaries."""
import json
from pathlib import Path
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import Mock, patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'experiment/code'))
from app import main
from app.services.llm_json import JSONPrompt, CompletionText, json_prompt, check_completion, JSONCompletionError
from app.services.sysml import generate_validated_bundle, SysMLValidationError
from app.services.interface_planner import plan_interface_contract, ContractPlanningError
import run_controlled_experiment as controlled
import run_formal_experiment as formal


def response(text='{}', reason='stop', refusal=None):
    return SimpleNamespace(model='fixture', usage=None, choices=[SimpleNamespace(
        finish_reason=reason, message=SimpleNamespace(content=text, refusal=refusal))])


def bundle():
    return json.dumps(dict(system_sysml2='full source', req_test_map={'requirements_to_testpoints':[
        dict(requirement_id='R1', requirement_text='keep input', test_point_id='T1',
             test_method='trace', stimulus='sensor', oracle='output')]}))


def contract():
    return dict(version=1, control_period_ms=10, interfaces=[dict(name='lamp',direction='output',
        type='bool',unit='1',meaning='LED',sample_period_ms=10,initial=False,device='LED',hardware='D12',
        binding=dict(kind='digital_output',pin=12))], requirements=[dict(id='R1',text='LED',assertion_ids=['on'])])


class JSONRequestTests(unittest.TestCase):
    def test_production_and_experiment_explicit_format_only(self):
        client=Mock();client.with_options.return_value=client
        client.chat.completions.create.return_value=response()
        request=main.GenerateFromPromptRequest(prompt='unused',model='fixture',max_tokens=6000)
        with patch.object(main,'client',client),patch.object(main,'_llm_api_key','fixture'):
            with tempfile.TemporaryDirectory() as folder:
                calls=controlled.Calls(Path(folder),'fixture')
                for invoke in (lambda p:main._call_llm(p,request),calls):
                    _,text=invoke(json_prompt('generate model'))
                    self.assertEqual(client.chat.completions.create.call_args.kwargs['response_format'],{'type':'json_object'})
                    self.assertEqual(text.completion['finish_reason'],'stop')
                    invoke('Generate Arduino code that prints JSON')
                    self.assertNotIn('response_format',client.chat.completions.create.call_args.kwargs)
                self.assertEqual(calls.entries[0]['response_format'],{'type':'json_object'})
                self.assertNotIn('response_format',calls.entries[1])
        self.assertEqual(client.chat.completions.create.call_count,4)

    def test_transport_failure_never_silently_falls_back_to_text(self):
        client=Mock();client.with_options.return_value=client
        client.chat.completions.create.side_effect=ValueError('unsupported format')
        with patch.object(main,'client',client),patch.object(main,'_llm_api_key','fixture'):
            calls=controlled.Calls(Path('unused'),'fixture')
            with self.assertRaisesRegex(RuntimeError,'infrastructure'):
                calls(json_prompt('model'))
        self.assertEqual(client.chat.completions.create.call_count,1)
        self.assertEqual(calls.entries[0]['error_type'],'ValueError')

    def test_incomplete_bundle_then_semantic_failure_both_rejected(self):
        answers=iter([CompletionText(bundle(),finish_reason='length'),CompletionText(bundle(),finish_reason='stop')])
        prompts=[];validator=Mock(return_value={'ok':False,'issues':[dict(code='Syntax',severity='error',message='bad model')]})
        def call(prompt):prompts.append(prompt);return 'fixture',next(answers)
        with self.assertRaises(SysMLValidationError) as caught:
            generate_validated_bundle(call,'keep input',validator=validator,max_repairs=1)
        self.assertEqual([a['issues'][0]['code'] for a in caught.exception.attempts],['JSON_INCOMPLETE','Syntax'])
        validator.assert_called_once_with('full source')
        self.assertTrue(all(isinstance(p,JSONPrompt) for p in prompts))
        self.assertIn('keep input',prompts[1])
        self.assertEqual(caught.exception.attempts[0]['response'],bundle())
        self.assertEqual(caught.exception.attempts[0]['completion']['finish_reason'],'length')

    def test_refusal_stops_model_and_contract_retries(self):
        for generate,error in (
            (lambda c:generate_validated_bundle(c,'requirements',max_repairs=2),SysMLValidationError),
            (lambda c:plan_interface_contract(c,'D12 LED',{},max_repairs=2),ContractPlanningError)):
            call=Mock(return_value=('fixture',CompletionText('{}',finish_reason='stop',refusal='refused')))
            with self.assertRaises(error) as caught:generate(call)
            self.assertEqual(call.call_count,1)
            attempt=caught.exception.attempts[0]
            self.assertFalse(attempt['ok']);self.assertEqual(attempt['completion']['refusal'],'refused')

    def test_planner_retries_incomplete_and_revalidates_wiring(self):
        wrong=contract();wrong['interfaces'][0]['binding']['pin']=13
        answers=iter([CompletionText(json.dumps(contract()),finish_reason='length'),
                      CompletionText(json.dumps(wrong),finish_reason='stop'),
                      CompletionText(json.dumps(contract()),finish_reason='stop')])
        prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',next(answers)
        result,report=plan_interface_contract(call,'D12 LED',{},max_repairs=2)
        self.assertEqual([a['ok'] for a in report['attempts']],[False,False,True])
        self.assertEqual(report['attempts'][0]['code'],'JSON_INCOMPLETE')
        self.assertIn('D12',report['attempts'][1]['error'])
        self.assertEqual(result['interfaces'][0]['binding']['pin'],12)
        self.assertTrue(all(isinstance(p,JSONPrompt) for p in prompts))

    def test_cached_completion_keeps_rejection_and_format_identity(self):
        prompt=json_prompt('model')
        with tempfile.TemporaryDirectory() as folder:
            path=Path(folder)/'calls.json'
            entry=dict(prompt=prompt,response='{}',actual_model='fixture',finish_reason='length',
                       response_format={'type':'json_object'})
            path.write_text(json.dumps([entry]),encoding='utf-8')
            _,text=formal.ReplayCalls(Path(folder),'fixture')(prompt)
            with self.assertRaises(JSONCompletionError):check_completion(text)
            with self.assertRaisesRegex(RuntimeError,'format mismatch'):
                formal.ReplayCalls(Path(folder),'fixture')(str(prompt))

    def test_direct_ir_cannot_accept_incomplete_or_refused_json(self):
        with tempfile.TemporaryDirectory() as folder:
            calls=Mock(return_value=('fixture',CompletionText('{}',finish_reason='stop',refusal='refused')))
            calls.folder=Path(folder)
            with patch.object(formal,'validate_ir') as validate:
                self.assertIsNone(formal.generate_ir(calls,'all requirements'))
            validate.assert_not_called();self.assertEqual(calls.call_count,1)
            self.assertIsInstance(calls.call_args.args[0],JSONPrompt)
            attempts=json.loads((Path(folder)/'validation.json').read_text())
            self.assertEqual(attempts[0]['code'],'LLM_REFUSAL')

    def test_content_filter_is_not_normal_completion(self):
        with self.assertRaises(JSONCompletionError):
            check_completion(CompletionText('{}',finish_reason='content_filter'))

    def test_iterative_ir_api_reports_actual_refusal_attempt(self):
        from fastapi.testclient import TestClient
        with tempfile.TemporaryDirectory() as folder, TestClient(main.app) as client:
            target=Path(folder)/'ir.json'
            with patch.object(main,'_call_llm',return_value=('fixture',
                    CompletionText('{}',finish_reason='stop',refusal='refused'))) as call:
                result=client.post('/generate/subsystem-ir-iterative',json={
                    'prompt':'all requirements','max_iters':3,'out_ir_path':str(target)})
            self.assertEqual(result.status_code,200,result.text)
            self.assertFalse(result.json()['ok'])
            self.assertEqual(result.json()['iteration'],1)
            self.assertEqual(result.json()['issues'][0]['code'],'LLM_REFUSAL')
            self.assertEqual(call.call_count,1)
            self.assertFalse(target.exists())


if __name__=='__main__':unittest.main()
