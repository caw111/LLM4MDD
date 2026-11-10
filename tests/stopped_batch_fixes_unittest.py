import copy
import unittest
from unittest.mock import patch
from app.services.expression_normalization import normalize_controller
from app.services.interface_planner import prepare_interface_contract
from app.services.source_behavior import check_source_messages, check_source_behavior


class StoppedBatchFixTests(unittest.TestCase):
    def test_operator_whitespace_is_normalized_without_guessing_unknown_operators(self):
        source={'outputs':{'lamp':{'>= ':['step',5]},'other':{'mystery ':['x',2]}}}
        result,events=normalize_controller(source,str)
        self.assertEqual(result['outputs']['lamp'],{'>=':['step',5]})
        self.assertIn('mystery ',result['outputs']['other'])
        self.assertIn('>= ',source['outputs']['lamp'])
        self.assertEqual(len(events),1)

    def test_power_on_servo_position_does_not_use_cycle_reset_position(self):
        source='Initialization: Both servos (servo1, servo2) should be at their 0-degree position.\nCycle reset: servo2 at 180 degrees.'
        proposal={'interface_requests':[dict(kind='servo_angle',pin=5,initial=0,meaning='Servo1'),
            dict(kind='servo_angle',pin=6,initial=180,meaning='Servo2')]}
        contract,actions=prepare_interface_contract(proposal,source,{})
        self.assertEqual([p['initial'] for p in contract['interfaces']],[0,0])
        self.assertEqual(proposal['interface_requests'][1]['initial'],180)
        self.assertTrue(any('source_servo_startup' in action for action in actions))

    def test_boolean_report_cannot_substitute_for_two_required_messages(self):
        source='Continuously check the button value, and print "Button pressed" if the value is HIGH, or print "Button not pressed" if the value is LOW.'
        with self.assertRaisesRegex(ValueError,'SOURCE_CONDITIONAL_TEXT'):
            check_source_messages({'interfaces':[{'binding':{'kind':'serial_value','label':''}}]},source)
        check_source_messages({'interfaces':[{'binding':{'kind':'serial_text_output','text':text}}
            for text in ['Button pressed','Button not pressed']]},source)

    def test_pair_counter_rejects_extra_segments_and_repeat_on_hold(self):
        outputs=['segment'+str(i) for i in range(10)]
        check=dict(kind='bargraph_pair_count',input='button',outputs=outputs)
        contract=dict(control_period_ms=10,interfaces=[dict(name='button',initial=False,direction='input',binding={'kind':'digital_input'})])
        class Counter:
            def __init__(self,ir):self.mode=ir;self.n=0;self.previous=False
            def step(self,inputs):
                if inputs['button'] and (self.mode=='hold' or not self.previous):self.n=(self.n+1)%6
                self.previous=inputs['button']
                count=self.n*2+(1 if self.mode=='extra' and self.n else 0)
                return {name:index<count for index,name in enumerate(outputs)}
        with patch('app.services.source_behavior.validate_source_checks',return_value=[check]),patch('app.services.ir_execution.IRExecution',Counter):
            self.assertEqual(check_source_behavior('good',contract)[0]['status'],'passed')
            for mode in ['extra','hold']:self.assertEqual(check_source_behavior(mode,contract)[0]['status'],'failed')

    def test_continuous_reporting_requires_startup_low_and_both_stable_states(self):
        contract=dict(control_period_ms=10,interfaces=[dict(name='button',initial=False,direction='input',binding={'kind':'digital_input'})])
        check=dict(kind='button_text_state',input='button',high_output='high',low_output='low',active_high=True)
        class Report:
            def __init__(self,mode):self.mode=mode;self.prev=False
            def step(self,inputs):
                high=inputs['button'];changed=high!=self.prev;self.prev=high
                enabled=self.mode=='continuous' or changed
                return dict(high=enabled and high,low=enabled and not high)
        with patch('app.services.source_behavior.validate_source_checks',return_value=[check]),patch('app.services.ir_execution.IRExecution',Report):
            self.assertEqual(check_source_behavior('continuous',contract)[0]['status'],'passed')
            self.assertEqual(check_source_behavior('edge_only',contract)[0]['status'],'failed')


if __name__=='__main__':unittest.main()
