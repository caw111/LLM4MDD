import copy
import json
import unittest

from app.services.interface_planner import (prepare_interface_contract, add_public_behavior_checks,
    check_explicit_requirements, plan_interface_contract)
from app.services.source_behavior import add_source_checks, check_source_behavior, validate_source_checks
from app.services.sysml_assembly import assemble_sysml
from app.services.sysml_compiler import compile_sysml
from app.services.behavior_contract import check_contract
from app.services.sysml import generate_validated_bundle


TEXT = '每 2 秒向串口监视器输出一次 Hello Arduino。'
COMMAND = '通过串口（9600 波特率）接收字符串 ON 或 OFF，控制 D13 LED 开关。'
PWM = '读取连接在 A0 引脚的电位器，根据数值控制 D9 PWM LED 的亮度。'


def proposal(requests):
    return dict(version=1, board='arduino:avr:uno', control_period_ms=5,
                interface_requests=requests, serial_channels=[], planning_notes=[])


def contract(requests, text):
    c, _ = prepare_interface_contract(proposal(requests), text, {})
    c['requirements'] = [dict(id='R1', text=text, assertion_ids=['source_requirement_R1'])]
    return add_public_behavior_checks(c, text)


def lower(c, model):
    return compile_sysml(assemble_sysml(model, c))


class SourceBehaviorTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.periodic = contract([dict(kind='serial_text_output', initial=False, meaning='Required message',
                                      baud=9600, text='Hello Arduino')], TEXT)
        cls.commands = contract([dict(kind='serial_command_input', initial=0, meaning='ON/OFF commands',
                baud=9600, commands={'ON':1, 'OFF':2}),
                dict(kind='digital_output', initial=False, meaning='LED state', pin='D13')], COMMAND)
        cls.pwm = contract([dict(kind='analog_input', initial=0, meaning='ADC', pin='A0'),
                dict(kind='pwm_output', initial=0, meaning='PWM', pin='D9')], PWM)

    def periodic_model(self, correct):
        c = self.periodic
        clock = next(p['name'] for p in c['interfaces'] if p['binding']['kind']=='elapsed_ms')
        states = [dict(name='timer', type='i32', initial=0), dict(name='due', type='bool', initial=False)]
        return dict(states=states, updates=[dict(assign='timer', value='timer + '+clock),
                dict(assign='due', value='timer >= 2000'),
                {'if':'due', 'then':[dict(assign='timer', value='timer - 2000')]}],
                outputs={'io_0':'due' if correct else 'timer >= 2000'})

    def test_periodic_reset_order_counterexample_and_positive_control(self):
        for correct in [False, True]:
            with self.subTest(correct=correct):
                result=check_contract(lower(self.periodic,self.periodic_model(correct)),self.periodic)
                self.assertEqual(result['source_behavior']['status'], 'passed' if correct else 'failed')
                self.assertEqual(result['functional_status'],'unverified')

    def test_one_step_serial_event_requires_persistent_command_state(self):
        c=self.commands
        on, valid=c['source_behavior_checks'][0]['input'],c['source_behavior_checks'][0]['valid']
        for correct in [False, True]:
            model=dict(states=[dict(name='led',type='bool',initial=False)],
                updates=[{'if':valid,'then':[dict(assign='led',value=on+' == 1')]}],
                outputs={'io_1':'led' if correct else '(if '+valid+' ? '+on+' == 1 else false)'})
            with self.subTest(correct=correct):
                report, issues=check_source_behavior(lower(c,model),c)
                self.assertEqual(report['status'],'passed' if correct else 'failed')
                if not correct:self.assertEqual(report['counterexample']['trace'][-1]['scenario'],'on')

    def test_raw_adc_cannot_rely_on_pwm_saturation(self):
        for expression, expected in [('io_0','failed'), ('io_0 * 255.0 / 1023.0','passed')]:
            model=dict(states=[],updates=[],outputs={'io_1':expression})
            report,_=check_source_behavior(lower(self.pwm,model),self.pwm)
            self.assertEqual(report['status'],expected)

    def test_fixed_text_duplicate_is_rejected_before_model_generation(self):
        c=copy.deepcopy(self.periodic)
        c['interfaces'].append(dict(name='extra',binding={'kind':'serial_value'}))
        with self.assertRaisesRegex(ValueError,'duplicate'):
            add_source_checks(c,TEXT)

    def test_source_matching_does_not_invent_conditional_obligations(self):
        c=copy.deepcopy(self.periodic);c.pop('source_behavior_checks')
        for text in ['温度过高时，'+TEXT, '不要'+TEXT, '每2秒输出一个状态值。']:
            self.assertNotIn('source_behavior_checks',add_source_checks(c,text))
        self.assertEqual(add_source_checks(self.periodic,TEXT),self.periodic)

    def test_configuration_prompt_suffix_does_not_hide_original_requirement(self):
        c=copy.deepcopy(self.periodic);c.pop('source_behavior_checks')
        c['configuration_resolution']={'original_requirement':TEXT}
        result=add_source_checks(c,TEXT+'\nResolved hardware configuration: {}')
        self.assertEqual(result['source_behavior_checks'],self.periodic['source_behavior_checks'])

    def test_sensor_report_period_is_extracted_without_guessing_thresholds(self):
        text='每 5 秒读取 DHT11（D7）温度并串口输出'
        c,_=prepare_interface_contract(proposal([dict(kind='dht_temperature',pin='D7',sensor='DHT11',initial=0,meaning='temperature')]),text,{})
        # A report channel is mandatory; reading the sensor alone is insufficient.
        with self.assertRaisesRegex(ValueError,'temperature report'):
            add_source_checks(c,text)

    def test_tampered_source_assertion_is_rejected(self):
        c=copy.deepcopy(self.periodic);c['source_behavior_checks'][0]['period_ms']=1
        with self.assertRaisesRegex(ValueError,'exact source'):
            validate_source_checks(c)

    def test_supports_serial_commands_requires_a_receiver(self):
        c=copy.deepcopy(self.pwm)
        for text in ['系统串口支持 ARM、DISARM、STATUS 和 RESET 命令。',
                     '系统支持串口命令 ON、OFF、BLINK。']:
            with self.subTest(text=text),self.assertRaisesRegex(ValueError,'receive'):
                check_explicit_requirements(c,text,{})

    def test_planner_assembles_exact_fixed_message_before_model_generation(self):
        good=proposal([dict(kind='serial_text_output',initial=False,meaning='message',baud=9600,text='Hello Arduino')])
        bad=copy.deepcopy(good);bad['interface_requests'][0]['text']='wrong message'
        replies=[bad,good];prompts=[]
        def call(prompt):
            prompts.append(str(prompt));return 'offline-fixture',json.dumps(replies.pop(0))
        c, report=plan_interface_contract(call,TEXT,{},max_repairs=1,resolve_defaults=False)
        self.assertEqual(len(report['attempts']),1)
        self.assertEqual(next(p['binding']['text'] for p in c['interfaces'] if p['binding']['kind']=='serial_text_output'),'Hello Arduino')
        self.assertTrue(c['source_behavior_checks'])

    def test_behavior_counterexample_reaches_model_repair_and_revalidation(self):
        c=self.periodic; prompts=[]; replies=[self.periodic_model(False),self.periodic_model(True)]
        def call(prompt):
            prompts.append(str(prompt));return 'offline-fixture',json.dumps(dict(controller=replies.pop(0),
                req_test_map={'requirements_to_testpoints':[dict(requirement_id='R1', requirement_text=TEXT,
                    test_point_id='T1',test_method='trace',stimulus='elapsed time',oracle='periodic report')]}))
        def validator(source):
            return dict(ok=True,issues=[],ir=compile_sysml(source))
        _,_,report=generate_validated_bundle(call,'Generate controller',contract=c,validator=validator,max_repairs=1)
        self.assertEqual(len(report['attempts']),2)
        self.assertIn('SOURCE_BEHAVIOR',prompts[1])
        self.assertEqual(report['coverage']['source_behavior']['status'],'passed')


if __name__=='__main__':
    unittest.main()
