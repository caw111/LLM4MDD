import copy
import unittest

from source_behavior_unittest import contract, lower, proposal
from app.services.interface_planner import prepare_interface_contract, add_public_behavior_checks
from app.services.source_behavior import check_source_behavior, add_source_checks


COUNTER = '使用 TM1637 + 按钮（D2）：每按一次计数+1并显示'
SERIAL_COUNTER = '使用 D2 按钮进行计数，每按一次计数加 1，并通过串口输出当前计数值。'
SWITCHES = '使用两个按钮控制 D13 LED，其中 D2 按钮用于开灯，D3 按钮用于关灯。'
TOGGLE = '使用 D2 按钮控制 D13 LED，每按一次切换开关状态。'
TONE = '让 D6 引脚的蜂鸣器每 2 秒响 200ms。'


def button(pin='D2'):
    return dict(kind='digital_input',pin=pin,initial=False,meaning='Normalized pressed')


class SourceBehaviorRound2Tests(unittest.TestCase):
    def assert_behavior(self, c, model, status):
        report, issues = check_source_behavior(lower(c,model),c)
        self.assertEqual(report['status'],status,report)
        return report

    def test_counter_rejects_overwritten_edge_memory_and_counting_while_held(self):
        c=contract([button(),dict(kind='tm1637_number',clk_pin='D4',dio_pin='D5',
                                  initial=0,meaning='Count')],COUNTER)
        for mode in ['correct','overwritten','level']:
            updates=[{'if':'io_0' if mode=='level' else 'io_0 and not previous',
                      'then':[dict(assign='count',value='count + 1')]}]
            memory=dict(assign='previous',value='io_0')
            updates=([memory]+updates) if mode=='overwritten' else (updates+[memory])
            model=dict(states=[dict(name='count',type='i32',initial=0),
                               dict(name='previous',type='bool',initial=False)],
                       updates=updates,outputs={'io_1':'count'})
            with self.subTest(mode=mode):
                self.assert_behavior(c,model,'passed' if mode=='correct' else 'failed')

    def test_serial_counter_requires_an_observable_report_after_each_press(self):
        p=proposal([button()]);p['serial_channels']=[dict(baud=9600,
            trigger=dict(initial=False,meaning='Press event'),values=[dict(type='i32',unit='count',
            initial=0,meaning='Count',label='counter')])]
        c,_=prepare_interface_contract(p,SERIAL_COUNTER,{})
        c['requirements']=[dict(id='R1',text=SERIAL_COUNTER,assertion_ids=['source_requirement_R1'])]
        c=add_source_checks(c,SERIAL_COUNTER)
        for correct in [False,True]:
            model=dict(states=[dict(name='count',type='i32',initial=0),
                dict(name='previous',type='bool',initial=False),dict(name='pressEvent',type='bool',initial=False)],
                updates=[dict(assign='pressEvent',value='io_0 and not previous'),
                    {'if':'pressEvent','then':[dict(assign='count',value='count+1')]},
                    dict(assign='previous',value='io_0')],
                outputs={'serial_0_trigger':'pressEvent' if correct else 'io_0 and not previous',
                         'serial_0_value_0':'count'})
            with self.subTest(correct=correct):
                self.assert_behavior(c,model,'passed' if correct else 'failed')

    def test_reserved_keyword_is_rejected_before_reference_parser_cascades(self):
        from app.services.sysml_assembly import model_identifier
        for name in ['event','part','action','if']:
            with self.subTest(name=name),self.assertRaisesRegex(ValueError,'Reserved SysML keyword'):
                model_identifier(name)
        self.assertEqual(model_identifier('pressEvent'),'pressEvent')

    def test_two_button_led_requires_persistent_state(self):
        c=contract([button(),button('D3'),dict(kind='digital_output',pin='D13',initial=False,meaning='LED')],SWITCHES)
        for correct in [False,True]:
            model=dict(states=[dict(name='led',type='bool',initial=False)],
                updates=[{'if':'io_0','then':[dict(assign='led',value=True)]},
                         {'if':'io_1','then':[dict(assign='led',value=False)]}],
                outputs={'io_2':'led' if correct else '(if io_0 ? true else false)'})
            with self.subTest(correct=correct):
                self.assert_behavior(c,model,'passed' if correct else 'failed')

    def test_toggle_requires_one_change_per_press_and_retains_on_release(self):
        c=contract([button(),dict(kind='digital_output',pin='D13',initial=False,meaning='LED')],TOGGLE)
        for correct in [False,True]:
            model=dict(states=[dict(name='led',type='bool',initial=False),
                               dict(name='previous',type='bool',initial=False)],
                updates=[{'if':'io_0 and not previous' if correct else 'io_0',
                          'then':[dict(assign='led',value='not led')]},
                         dict(assign='previous',value='io_0')],outputs={'io_1':'led'})
            with self.subTest(correct=correct):
                self.assert_behavior(c,model,'passed' if correct else 'failed')

    def test_buzzer_recurrence_checks_width_without_inventing_pitch_or_start_phase(self):
        c=contract([dict(kind='tone_output',pin='D6',initial=0,meaning='Buzzer')],TONE)
        clock=next(p['name'] for p in c['interfaces'] if p['binding']['kind']=='elapsed_ms')
        for mode in ['correct','wrong_width','silence','one_pulse','step_counter']:
            updates=[dict(assign='phase',value='phase+'+('5' if mode=='step_counter' else clock))]
            if mode!='one_pulse':
                updates.append({'if':'phase >= 2000','then':[dict(assign='phase',value='phase-2000')]})
            output='0' if mode=='silence' else '(if phase < '+('500' if mode=='wrong_width' else '200')+' ? 440 else 0)'
            model=dict(states=[dict(name='phase',type='i32',initial=0)],updates=updates,outputs={'io_0':output})
            with self.subTest(mode=mode):
                self.assert_behavior(c,model,'passed' if mode=='correct' else 'failed')

    def test_large_scenario_is_reported_incomplete_not_a_model_defect(self):
        c=contract([dict(kind='tone_output',pin='D6',initial=0,meaning='Buzzer')],TONE)
        c['control_period_ms']=1
        for p in c['interfaces']:p['sample_period_ms']=1
        c['source_behavior_checks'][0]['evidence']='让 D6 引脚的蜂鸣器每 60 秒响 200ms。'
        c=add_source_checks({k:v for k,v in c.items() if k!='source_behavior_checks'},c['source_behavior_checks'][0]['evidence'])
        model=dict(states=[],updates=[],outputs={'io_0':'0'})
        report,issues=check_source_behavior(lower(c,model),c)
        self.assertEqual(report['status'],'incomplete');self.assertEqual(issues,[])

    def test_conditional_and_ambiguous_buttons_do_not_get_invented_checks(self):
        c=contract([button(),dict(kind='digital_output',pin='D13',initial=False,meaning='LED')],TOGGLE)
        c.pop('source_behavior_checks')
        for text in ['如果报警，则'+TOGGLE, '不要'+TOGGLE, '按钮改变显示模式。']:
            self.assertNotIn('source_behavior_checks',add_source_checks(c,text))


if __name__=='__main__':
    unittest.main()
