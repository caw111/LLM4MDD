import json
import sys
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
import run_new_workflow_pceval50 as calls
import run_semantic_repair_validation as runner


class ResumeTests(unittest.TestCase):
    def tearDown(self):calls.STOP.clear()

    def test_transient_connections_retry_and_success_does_not_stop_batch(self):
        Error=type('APIConnectionError',(RuntimeError,),{})
        response=SimpleNamespace(model='fixture',id='test',usage=None,choices=[SimpleNamespace(
            message=SimpleNamespace(content='OK',refusal=None),finish_reason='stop')])
        with tempfile.TemporaryDirectory() as td,patch.object(calls.workflow.api.client,'with_options') as options,patch.object(
                calls,'completion_text',return_value='OK'),patch.object(calls.time,'sleep'):
            create=options.return_value.chat.completions.create
            create.side_effect=[Error(),Error(),Error(),Error(),response]
            caller=calls.ModelCalls(Path(td),'fixture',role='test',budget=1)
            self.assertEqual(caller('request'),('fixture','OK'))
            self.assertEqual(create.call_count,5)
            self.assertFalse(calls.STOP.is_set())
            self.assertEqual(sum('response' in r for r in caller.entries),1)

    def test_stopped_work_does_not_create_a_fake_case_result(self):
        calls.STOP.set()
        with tempfile.TemporaryDirectory() as td:
            self.assertIsNone(runner.evaluate({'id':'pending'},Path(td),'gen','review'))
            self.assertFalse((Path(td)/'cases').exists())

    def test_infrastructure_rows_are_pending_not_functional_failures(self):
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)
            for cid,extra in [('good',{}),('interrupted',{'infrastructure_interrupted':True})]:
                path=out/'cases'/cid/'result.json';path.parent.mkdir(parents=True)
                path.write_text(json.dumps(dict(case=cid,dataset='internal',compile_ok=False,
                    functional_status='unverified',simulide_status='not_run',rounds=[],**extra)))
            result=runner.report(out,[dict(id=c,dataset='internal') for c in ['good','interrupted','not_started']])
            self.assertEqual(result['completed'],1)
            self.assertEqual(result['pending'],2)
            self.assertEqual(result['infrastructure_interrupted'],1)


if __name__=='__main__':unittest.main()
