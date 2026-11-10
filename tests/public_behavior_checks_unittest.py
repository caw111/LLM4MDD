import copy
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from fastapi.testclient import TestClient
from app.main import app
from app.services.interface_planner import add_public_behavior_checks, check_explicit_requirements, plan_interface_contract, has_only_response_deadline
from app.services.behavior_contract import validate_contract, check_contract
from app.services.sysml_assembly import assemble_sysml
from app.services.sysml_compiler import compile_sysml, validate_executable_sysml
from app.services.sysml import generate_validated_bundle
from app.services.interface_registry import register_contract


REQUIREMENT='D6蜂鸣器每3000ms响一次，初始运行且静音。D2按钮按下沿切换暂停/恢复，暂停立即静音。'


def contract():
    data=json.loads(Path('tools/examples/periodic_tone.contract.json').read_text(encoding='utf-8'))
    data['interfaces'].append(dict(name='pressed',direction='input',type='bool',unit='1',meaning='Normalized button',
        sample_period_ms=10,initial=False,device='button',hardware='D2',binding=dict(kind='digital_input',pin=2,active_high=False,pullup=True)))
    data['required_dependencies']={'frequency':['deltaMs','pressed']}
    return add_public_behavior_checks(data,REQUIREMENT)


def controller():
    return dict(states=[],updates=[],components=[
        dict(kind='edge_toggle',name='run',input='pressed',edge='rising',initial=True,previous_input=False),
        dict(kind='periodic_pulse',name='beep',delta='deltaMs',enabled='component_run_value',period_ms=3000,pulse_ms=100,first_pulse='after_period',pause='preserve')],
        outputs={'frequency':{'if':'component_beep_value','then':'1000','else':'0'}})


class PublicChecksTests(unittest.TestCase):
    def test_equivalent_response_deadlines_preserve_business_timing(self):
        for deadline in ('响应不超过 50ms', '响应不大于50毫秒', '响应时间不超过50ms',
                         'response within 50 milliseconds', 'response time at most 50ms'):
            requirement='ADC-to-servo mapping; '+deadline
            with self.subTest(deadline=deadline):
                self.assertTrue(has_only_response_deadline(requirement))
                for timing in ('; 每5秒输出报告', '; 延时500ms启动', '; 累计运行时间',
                               '; use an elapsed_ms clock', '; integrate the input',
                               '; 周期性翻转', '; 持续鸣叫100ms', '; move smoothly',
                               '; 平滑运动', '; ramp to the target', '; slew rate limit'):
                    self.assertFalse(has_only_response_deadline(requirement+timing))

    def test_deadline_clock_is_repaired_before_freezing_without_fake_dependence(self):
        good=json.loads(Path('tools/examples/adc_display.contract.json').read_text(encoding='utf-8'))
        bad=copy.deepcopy(good)
        bad['interfaces'].append(copy.deepcopy(contract()['interfaces'][0]))
        output=next(p['name'] for p in good['interfaces'] if p['direction']=='output')
        bad.setdefault('required_dependencies',{}).setdefault(output,[]).append('deltaMs')
        requirement='A0映射到TM1637，CLK=D4、DIO=D5，输入变化后的显示响应时间不超过50ms，包括总线传输。'
        with self.assertRaisesRegex(ValueError,'response deadline'):check_explicit_requirements(bad,requirement,{})
        replies=iter([bad,good]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',json.dumps(next(replies))
        accepted,report=plan_interface_contract(call,requirement,{},max_repairs=1)
        self.assertEqual(accepted,register_contract(good));self.assertEqual([a['ok'] for a in report['attempts']],[False,True])
        self.assertIn('BEFORE freezing',prompts[1]);self.assertIn(requirement,prompts[1])
        self.assertFalse(has_only_response_deadline(requirement+' 每5秒输出报告。'))
        self.assertFalse(has_only_response_deadline(requirement+' Expose an elapsed_ms clock input.'))
        self.assertFalse(has_only_response_deadline('未明确描述的控制行为'))

    def test_only_explicit_public_obligations_are_added_and_preserved(self):
        data=contract();saved=copy.deepcopy(data)
        self.assertEqual(add_public_behavior_checks(data,REQUIREMENT),data)
        self.assertEqual(data,saved)
        self.assertEqual(data['startup_checks'][0]['expected'],0)
        self.assertTrue(data['pause_checks'][0]['initial_running'])
        raw=json.loads(Path('tools/examples/periodic_tone.contract.json').read_text(encoding='utf-8'))
        self.assertEqual(add_public_behavior_checks(raw,'Sound the buzzer'),raw)
        for text in ['Do not be initially silent; sound immediately.',
                     'Initially silent unless an alarm input is active.',
                     '不要求初始静音，立即响起。', '初始静音，除非报警输入有效。']:
            with self.subTest(text=text):self.assertNotIn('startup_checks',add_public_behavior_checks(raw,text))
        self.assertEqual(add_public_behavior_checks(raw,'Initially silent; then beep.')['startup_checks'][0]['expected'],0)
        bad=copy.deepcopy(data);bad['startup_checks'][0]['expected']=1000
        with self.assertRaisesRegex(ValueError,'contradicts'):add_public_behavior_checks(bad,REQUIREMENT)
        for key in ('startup_checks','pause_checks'):
            bad=copy.deepcopy(data);bad[key][0]['expected']=True
            with self.assertRaises(ValueError):validate_contract(bad)
            bad=copy.deepcopy(data);bad[key][0]['output']=[]
            with self.assertRaises(ValueError):validate_contract(bad)

    def test_pause_after_an_active_prefix_exposes_a_latched_pulse(self):
        inventory=contract()
        spec=controller();spec['components']=spec['components'][:1]
        spec['states']=[dict(name='elapsed',type='i32',initial='0'),dict(name='pulseOn',type='bool',initial='false')]
        spec['updates']=[{'if':'component_run_value','then':[{'assign':'elapsed','value':'elapsed+deltaMs'},
            {'if':'elapsed>=3000','then':[{'assign':'pulseOn','value':'true'}]}]}]
        spec['outputs']['frequency']={'if':'pulseOn','then':'1000','else':'0'}
        ir=compile_sysml(assemble_sysml(spec,inventory))
        result=check_contract(ir,inventory)
        self.assertIn('CONTRACT_PAUSE',[i['code'] for i in result['issues']])
        self.assertEqual(result['startup']['status'],'passed')

    def test_explicit_response_tolerance_is_not_silently_tightened(self):
        inventory=contract();inventory.pop('startup_checks')
        inventory['requirements'][0]['text']='Continuous tone while running; pause silences within 20ms.'
        inventory['pause_checks'][0]['response_tolerance_ms']=20
        spec=controller();spec['components']=spec['components'][:1]
        spec['states']=[dict(name='pausedFor',type='i32',initial='0')]
        spec['updates']=[{'if':'component_run_value','then':[{'assign':'pausedFor','value':'0'}],
                         'else':[{'assign':'pausedFor','value':'pausedFor+deltaMs'}]}]
        spec['outputs']['frequency']={'if':'component_run_value or pausedFor<20','then':'1000','else':'0'}
        ir=compile_sysml(assemble_sysml(spec,inventory))
        self.assertEqual(check_contract(ir,inventory)['issues'],[])
        inventory['pause_checks'][0]['response_tolerance_ms']=0
        self.assertIn('CONTRACT_PAUSE',[i['code'] for i in check_contract(ir,inventory)['issues']])
        derived=add_public_behavior_checks(contract(),REQUIREMENT+' 响应及鸣叫时间容差20ms。')
        # Existing explicit 0ms assertions stay intact; they are caller supplied.
        self.assertEqual(derived['pause_checks'][0]['response_tolerance_ms'],0)

    def test_real_models_distinguish_startup_pause_edge_and_correct_behavior(self):
        inventory=contract()
        variants=[('good',controller(),None)]
        bad=controller();bad['components'][1]['first_pulse']='immediate';variants.append(('startup',bad,'CONTRACT_STARTUP'))
        bad=controller();bad['components'][0]['edge']='falling';variants.append(('edge',bad,'CONTRACT_PAUSE'))
        bad=controller();bad['outputs']['frequency']={'if':'not component_run_value or component_beep_value','then':'1000','else':'0'}
        variants.append(('pause_output',bad,'CONTRACT_PAUSE'))
        for name,spec,code in variants:
            with self.subTest(name=name):
                ir=compile_sysml(assemble_sysml(spec,inventory))
                result=check_contract(ir,inventory)
                if code:self.assertIn(code,[i['code'] for i in result['issues']])
                else:
                    self.assertEqual(result['issues'],[])
                    self.assertEqual(result['startup']['status'],'passed')
                    self.assertEqual(result['pause']['status'],'passed')
                    self.assertGreater(result['pause']['checked'],0)
                    self.assertEqual(result['functional_status'],'unverified')

    def test_startup_gate_is_shared_by_api_and_model_repair(self):
        inventory=contract();bad=controller();bad['components'][1]['first_pulse']='immediate'
        source=assemble_sysml(bad,inventory);ir=compile_sysml(source)
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client, patch('app.main._call_llm',side_effect=AssertionError('No LLM')):
            target=Path(folder)/'compiled'
            response=client.post('/compile/sysml2',json=dict(system_sysml2=source,interface_contract=inventory,out_dir=str(target)))
            self.assertEqual(response.status_code,422,response.text);self.assertIn('CONTRACT_STARTUP',response.text)
            self.assertFalse(target.exists())
            response=client.post('/validate/subsystem-ir',json=dict(ir=ir,interface_contract=inventory))
            self.assertFalse(response.json()['ok']);self.assertIn('CONTRACT_STARTUP',response.text)
        def bundle(spec):return json.dumps(dict(controller=spec,req_test_map=dict(requirements_to_testpoints=[dict(
            requirement_id='R1',requirement_text=REQUIREMENT,test_point_id='T1',test_method='trace',stimulus='button',oracle='silence')])))
        responses=iter([bundle(bad),bundle(controller())]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',next(responses)
        _,accepted,report=generate_validated_bundle(call,REQUIREMENT,contract=inventory,validator=validate_executable_sysml,max_repairs=1)
        self.assertEqual([a['ok'] for a in report['attempts']],[False,True])
        self.assertIn('CONTRACT_STARTUP',prompts[1]);self.assertIn('executed counterexample',prompts[1]);self.assertIn(source,prompts[1])
        self.assertEqual(accepted['controller'],controller())


if __name__=='__main__':unittest.main()
