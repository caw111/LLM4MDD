import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
from resume_contract_development import ResumeCalls
from run_controlled_experiment import Calls
from app.services.llm_json import json_prompt, check_completion, JSONCompletionError


class ResumeTests(unittest.TestCase):
    def fixture(self,folder):
        prompt=json_prompt('full requirements')
        path=Path(folder)/'old.json'
        entry=dict(number=1,prompt=prompt,response='{}',requested_model='fixture',actual_model='fixture',
                   finish_reason='length',response_format={'type':'json_object'},seconds=1,usage={'total_tokens':10})
        path.write_text(json.dumps([entry,dict(number=2,error_type='APIConnectionError')]),encoding='utf-8')
        return path,prompt

    def test_replay_keeps_completion_and_budget_prefix_without_transport(self):
        with tempfile.TemporaryDirectory() as folder:
            old,prompt=self.fixture(folder);before=old.read_bytes();events=[]
            calls=ResumeCalls(Path(folder)/'new','fixture',previous=old,events=events)
            with patch.object(Calls,'__call__',side_effect=AssertionError('must not call provider')):
                _,text=calls(prompt)
            with self.assertRaises(JSONCompletionError):check_completion(text)
            self.assertEqual(len(calls.entries),1)
            self.assertEqual(calls.entries[0]['usage']['total_tokens'],10)
            self.assertEqual(events[0]['kind'],'replayed')
            self.assertEqual(old.read_bytes(),before)

    def test_format_or_prompt_mismatch_never_issues_new_request(self):
        with tempfile.TemporaryDirectory() as folder:
            old,prompt=self.fixture(folder)
            for changed in (str(prompt),json_prompt('changed requirement')):
                calls=ResumeCalls(Path(folder)/'new','fixture',previous=old,events=[])
                with patch.object(Calls,'__call__',side_effect=AssertionError('must not call provider')):
                    with self.assertRaisesRegex(RuntimeError,'Cached request differs'):calls(changed)

    def test_only_missing_response_calls_provider_and_budget_is_preserved(self):
        with tempfile.TemporaryDirectory() as folder:
            old,prompt=self.fixture(folder);events=[]
            calls=ResumeCalls(Path(folder)/'new','fixture',previous=old,events=events)
            calls(prompt)
            def provider(instance,p):
                instance.entries.append(dict(number=len(instance.entries)+1,prompt=p,response='new',seconds=1))
                return 'fixture','new'
            with patch.object(Calls,'__call__',autospec=True,side_effect=provider) as request:
                for _ in range(5):self.assertEqual(calls(json_prompt('repair'))[1],'new')
                with self.assertRaisesRegex(ValueError,'budget exhausted'):calls(json_prompt('repair'))
            self.assertEqual(request.call_count,5)
            self.assertEqual(len(events),6)
            self.assertEqual(events[-1]['kind'],'new_transport')


if __name__=='__main__':unittest.main()
