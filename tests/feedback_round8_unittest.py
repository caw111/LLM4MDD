import copy
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import Mock, patch
from contextlib import ExitStack

from app.services.source_behavior import check_source_messages, derive_checks
from app.services.interface_planner import plan_interface_contract, prepare_interface_contract
from app.services.wiring_inventory import circuit_inventory

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
import run_semantic_repair_validation as runner


class FeedbackRound8Tests(unittest.TestCase):
    def test_compiler_failure_is_not_confused_with_upstream_rejection(self):
        for error,expected in [('ContractPlanningError','contract_planning_failed'),
                               ('SysMLValidationError','model_or_semantic_review_failed')]:
            self.assertEqual(runner.pipeline_outcome(dict(compile_ok=False,error_type=error,rounds=[])),expected)
        self.assertEqual(runner.pipeline_outcome(dict(compile_ok=False,rounds=[{'compiled':{'compile_ok':False}}])),
                         'compiler_failed')
    def test_configuration_metadata_is_never_a_serial_message(self):
        source='每 2 秒读取 DHT22（D2）温度并串口输出'
        requirement=source+'\nResolved hardware configuration (authoritative pin assignments; defaults)'
        contract={'interfaces':[], 'configuration_resolution':{'original_requirement':source}}
        check_source_messages(contract,requirement)
        # An explicit user message with identical words must still be enforced.
        contract['configuration_resolution']['original_requirement']='串口输出 Resolved hardware configuration'
        with self.assertRaisesRegex(ValueError,'SOURCE_MESSAGE'):
            check_source_messages(contract,requirement)

    def test_serial_direction_and_label_errors_receive_structural_feedback(self):
        source='通过串口（9600 波特率）接收字符串 ON 或 OFF，控制 D13 LED 开关。'
        base={'interface_requests':[dict(kind='digital_output',pin='D13',initial=False,meaning='LED')]}
        bad=copy.deepcopy(base)
        bad['serial_channels']=[dict(baud=9600,trigger=dict(initial=False,meaning='report'),values=[
            dict(type='i32',unit='1',initial=0,meaning='value')])]
        good=copy.deepcopy(base)
        good['interface_requests'].append(dict(kind='serial_command_input',initial=0,meaning='Received ON/OFF',
            baud=9600,commands={'ON':1,'OFF':2}))
        proposals=iter([bad,base,good]);prompts=[]
        def call(prompt):
            prompts.append(str(prompt));return 'fixture',json.dumps(next(proposals))
        contract,report=plan_interface_contract(call,source,{},max_repairs=2)
        self.assertIn('SERIAL VALUE SCHEMA REPAIR',prompts[1])
        self.assertIn('SERIAL DIRECTION REPAIR',prompts[2])
        self.assertEqual([a['ok'] for a in report['attempts']],[False,False,True])
        self.assertTrue(any(p['binding']['kind']=='serial_command_input' for p in contract['interfaces']))
        self.assertIn('command_hold',[c['kind'] for c in derive_checks(contract,source)])

    def rgb(self):
        return dict(parts=[dict(id='uno',type='wokwi-arduino-uno'),dict(id='rgb',type='wokwi-rgb-led')]+
            [dict(id='r'+str(i),type='wokwi-resistor') for i in range(3)],connections=[['rgb:COM','uno:5V']]+
            [wire for i,color in enumerate('RGB') for wire in [[f'rgb:{color}',f'r{i}:1'],[f'r{i}:2',f'uno:{4-i}']]])

    def test_common_anode_series_resistors_restore_actual_polarity_and_pins(self):
        circuit=self.rgb();before=copy.deepcopy(circuit)
        inventory=circuit_inventory([circuit],'arduino:avr:uno')
        rgb=next(d for d in inventory if d['device']=='rgb')
        self.assertEqual(rgb['signals'],{'R':['D4'],'G':['D3'],'B':['D2']})
        self.assertEqual(rgb['rails']['COM'],['5V'])
        proposal={'interface_requests':[dict(kind='digital_output',device='rgb',pin=pin,active_high=True,
            initial=False,meaning=color) for pin,color in zip([4,3,2],'RGB')]}
        contract,actions=prepare_interface_contract(proposal,json.dumps(circuit),{})
        self.assertTrue(all(p['binding']['active_high'] is False for p in contract['interfaces']))
        self.assertEqual(circuit,before)

    def test_malformed_code_review_retries_same_candidate_and_never_becomes_a_pass(self):
        with tempfile.TemporaryDirectory() as tmp,patch.object(runner,'functional_review_prompt',return_value='same code'),patch.object(
                runner,'validate_functional_review',side_effect=[ValueError('unsupported quote'),{'verdict':'judged_pass'}]):
            reviewer=Mock(return_value=('fixture','raw'))
            result=runner.review_final_code(reviewer,'requirement','uno','code',Path(tmp))
            self.assertEqual(result['verdict'],'judged_pass')
            self.assertEqual(reviewer.call_count,2)
            self.assertTrue((Path(tmp)/'code_review_error_0.json').exists())
        with tempfile.TemporaryDirectory() as tmp,patch.object(runner,'functional_review_prompt',return_value='same code'),patch.object(
                runner,'validate_functional_review',side_effect=ValueError('unsupported quote')):
            reviewer=Mock(return_value=('fixture','raw'))
            self.assertIsNone(runner.review_final_code(reviewer,'requirement','uno','code',Path(tmp)))
            self.assertEqual(reviewer.call_count,3)

    def test_review_infrastructure_failure_does_not_skip_execution_or_claim_acceptance(self):
        with tempfile.TemporaryDirectory() as tmp,ExitStack() as stack:
            folder=Path(tmp)
            case=dict(id='fixture',dataset='internal',fqbn='arduino:avr:uno',
                requirement='LED on',effective_requirement='LED on',configuration_resolution={})
            plan=dict(coverage='partial',plan=dict(assumptions=[],checks=[]))
            def generated(*args,**kwargs):
                kwargs['validator']('model')
                return {},None,{'system_sysml2':'model'},{'contract_revisions':0}
            def lowered(source,contract,ir,out):
                out.mkdir(parents=True,exist_ok=True)
                (out/'arduino_provenance.json').write_text('{}')
                return 'generated'
            for name,value in [('ModelCalls',Mock(return_value=Mock(entries=[]))),
                ('public_plan',Mock(return_value=(plan,{'readiness':'ready'}))),
                ('generate_verified_system',Mock(side_effect=generated)),('review_final_code',Mock(return_value=None))]:
                stack.enter_context(patch.object(runner,name,value))
            for name,value in [('validate_candidate',Mock(return_value={'ok':True,'ir':{}})),
                ('lower_verified',Mock(side_effect=lowered)),('adapt_verified',Mock(return_value='code')),
                ('compile_candidate',Mock(return_value=({'compile_ok':True,'artifact':'code','artifact_sha256':'hash'},''))),
                ('metrics',Mock(return_value={}))]:
                stack.enter_context(patch.object(runner.workflow,name,value))
            execute=stack.enter_context(patch.object(runner.host,'execute',return_value=dict(status='passed',
                checks=[],execution=dict(setup_returned=True,setup_at_ms=0))))
            result=runner.evaluate(case,folder,'generation','review')
            execute.assert_called_once()
            self.assertTrue(result['compile_ok'])
            self.assertEqual(result['executed_functional_status'],'partial_scenarios_passed')
            self.assertEqual(result['functional_status'],'unverified_code_review')


if __name__=='__main__':unittest.main()
