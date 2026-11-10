import copy
import json
from pathlib import Path
import unittest

from app.services.interface_planner import plan_interface_contract, ContractPlanningError
from app.services.planning_feedback import independent_field_diagnostics
from tests.mixed_environment_unittest import INTERPRETATIONS


def inventory():
    return json.loads(Path('tools/examples/environment_alarm.contract.json').read_text(encoding='utf-8'))


class PlanningFeedbackTests(unittest.TestCase):
    def test_independent_errors_are_repaired_together_without_mutating_candidate(self):
        good=inventory();bad=copy.deepcopy(good)
        for p in bad['interfaces']:
            if p['binding']['kind'].startswith(('dht_','ultrasonic_')):p['sample_period_ms']=10
            if p['binding']['kind']=='dht_temperature':p['unit']='%RH'
            if p['binding']['kind']=='serial_value':p['binding']['label']=''
        bad['interfaces']=[p for p in bad['interfaces'] if p['name']!='temperatureReportDue']
        before=copy.deepcopy(bad);prompts=[];responses=iter([bad,good])
        def call(prompt):prompts.append(prompt);return 'fixture',json.dumps(next(responses))
        result,report=plan_interface_contract(call,INTERPRETATIONS['complex_001'],{},max_repairs=1)
        self.assertEqual(bad,before)
        self.assertEqual([a['ok'] for a in report['attempts']],[False,True])
        for fragment in ('>= 2000 ms','>= 60 ms','unit degC','ALARM_TEMP','ALARM_DISTANCE','temperatureReportDue','fix ALL'):
            self.assertIn(fragment,prompts[1])
        self.assertEqual(result['requirements'],good['requirements'])
        self.assertEqual([p['binding'] for p in result['interfaces']],[p['binding'] for p in good['interfaces']])
        self.assertGreaterEqual(len(report['attempts'][0]['field_diagnostics']),7)

    def test_malformed_retry_has_no_stale_field_diagnostics_and_budget_still_fails(self):
        bad=inventory();bad['interfaces'][0]['sample_period_ms']=10
        responses=iter([json.dumps(bad),'{broken'])
        with self.assertRaises(ContractPlanningError) as caught:
            plan_interface_contract(lambda _:('fixture',next(responses)),INTERPRETATIONS['complex_001'],{},max_repairs=1)
        attempts=caught.exception.attempts
        self.assertTrue(attempts[0]['field_diagnostics'])
        self.assertEqual(attempts[1]['field_diagnostics'],[])
        self.assertFalse(any(a['ok'] for a in attempts))
        self.assertEqual(independent_field_diagnostics(None,'anything'),[])
        malformed=inventory()
        for p in malformed['interfaces']:
            if p['name']=='temperatureReportDue':p['binding']=None
        self.assertTrue(independent_field_diagnostics(malformed,INTERPRETATIONS['complex_001']))

    def test_empty_advisory_list_does_not_bypass_hardware_validation(self):
        wrong=inventory()
        for p in wrong['interfaces']:
            if p['name']=='alarmHz':p['binding']['pin']=8
        self.assertEqual(independent_field_diagnostics(wrong,INTERPRETATIONS['complex_001']),[])
        with self.assertRaises(ContractPlanningError) as caught:
            plan_interface_contract(lambda _:('fixture',json.dumps(wrong)),INTERPRETATIONS['complex_001'],{},max_repairs=1)
        self.assertEqual(len(caught.exception.attempts),2)
        self.assertIn('Conflicting hardware',caught.exception.attempts[-1]['error'])
        self.assertTrue(caught.exception.attempts[-1]['no_progress'])


if __name__=='__main__':unittest.main()
