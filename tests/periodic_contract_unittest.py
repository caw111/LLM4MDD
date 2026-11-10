import copy
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from fastapi.testclient import TestClient
from app.main import app
from app.services.behavior_contract import validate_contract, check_contract
from app.services.interface_planner import add_public_behavior_checks
from app.services.periodic_contract import reference_trace
from app.services.sysml_assembly import assemble_sysml, expression, ExpressionSyntaxError
from app.services.sysml_compiler import compile_sysml, validate_executable_sysml
from app.services.sysml import generate_validated_bundle

TEXT=('音调1000Hz，每次持续100ms；初始运行且静音，累计运行1000ms后首次发声，'
      '此后每累计运行1000ms开始一次。按下沿切换暂停/恢复，暂停立即静音，'
      '保留计时和剩余鸣叫时长。响应及鸣叫时间容差20ms。')


def contract():
    c=json.loads(Path('tools/examples/periodic_tone.contract.json').read_text(encoding='utf-8'))
    c['interfaces'].append(dict(name='pressed',direction='input',type='bool',unit='1',meaning='Normalized press',
        sample_period_ms=10,initial=False,hardware='D2',device='button',binding=dict(kind='digital_input',pin=2,active_high=False,pullup=True)))
    c['required_dependencies']={'frequency':['deltaMs','pressed']}
    return add_public_behavior_checks(c,TEXT)


def controller():
    return dict(states=[],updates=[],components=[
        dict(kind='edge_toggle',name='run',input='pressed',edge='rising',initial=True,previous_input=False),
        dict(kind='periodic_pulse',name='pulse',delta='deltaMs',enabled='component_run_value',period_ms=1000,pulse_ms=100,first_pulse='after_period',pause='preserve')],
        outputs={'frequency':{'if':'component_pulse_value','then':'1000','else':'0'}})


class PeriodicTests(unittest.TestCase):
    def test_pause_remainder_cannot_hide_between_tolerance_windows(self):
        c=contract()
        # This controller clears an active pulse on pause and never restores
        # its remaining sound on resume. The old midpoint-only probe misses it.
        spec=dict(states=[dict(name=name,type=typ,initial=value) for name,typ,value in [
            ('run','bool',True),('elapsed','i32',0),('remaining','i32',0),
            ('beeping','bool',False),('previous','bool',False)]],
            updates=[{'if':'pressed and not previous','then':[{'assign':'run','value':'not run'}]},
                     {'assign':'previous','value':'pressed'},
                     {'if':'not run','then':[{'assign':'beeping','value':False}]},
                     {'if':'run','then':[
                         {'if':'beeping','then':[{'assign':'remaining','value':'remaining-deltaMs'},
                             {'if':'remaining<=deltaMs','then':[{'assign':'beeping','value':False}]}]},
                         {'if':'not beeping and elapsed+deltaMs>=1000','then':[
                             {'assign':'elapsed','value':'elapsed+deltaMs-1000'},
                             {'assign':'beeping','value':True},{'assign':'remaining','value':100}],
                          'else':[{'assign':'elapsed','value':'elapsed+deltaMs'}]}]}],
            outputs={'frequency':{'if':'beeping','then':1000,'else':0}})
        ir=compile_sysml(assemble_sysml(spec,c))
        def midpoint(check,step,**kwargs):
            kwargs['pause_offset_ms']=check['pulse_ms']//2
            return reference_trace(check,step,**kwargs)
        with patch('app.services.periodic_contract.reference_trace',side_effect=midpoint):
            self.assertEqual(check_contract(ir,c)['periodic']['status'],'passed')
        result=check_contract(ir,c)
        self.assertEqual(result['periodic']['status'],'failed')
        self.assertEqual(result['periodic']['counterexample']['pause_offset_ms'],25)
        self.assertIn('CONTRACT_PERIODIC',[i['code'] for i in result['issues']])

    def test_equivalent_function_style_is_normalized_and_still_fully_validated(self):
        for value in ['and(a,b)','and(not(a), b)','x or (y and and(not(a), b))','or((a>b),not(c))','xor(a,b)']:
            with self.subTest(value=value):self.assertIsInstance(expression(value),str)
        for value in ['a and (b or c)','not(a)','(a and not previous)','a or (b xor (not c))']:
            self.assertEqual(expression(value),value)
        spec=controller();spec['outputs']['frequency']['if']='and(not(false), component_pulse_value)'
        mapping=dict(requirements_to_testpoints=[dict(requirement_id='R1',requirement_text=TEXT,test_point_id='T1',test_method='trace',stimulus='input',oracle='timing')])
        responses=iter([json.dumps(dict(controller=s,req_test_map=mapping)) for s in [spec,controller()]]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',next(responses)
        _,_,report=generate_validated_bundle(call,TEXT,contract=contract(),validator=validate_executable_sysml,max_repairs=1)
        self.assertEqual([a['ok'] for a in report['attempts']],[True])
        self.assertEqual(len(prompts),1)
        self.assertEqual(report['attempts'][0]['normalizations'][0]['path'],'/controller/outputs/frequency/if')
        self.assertIn('and(not(false)',report['attempts'][0]['response'])
        for value in ['and(a)','and(a,b,)','and(a,b); call()']:
            with self.subTest(value=value),self.assertRaises(ExpressionSyntaxError):expression(value)

    def test_only_complete_unambiguous_public_parameters_are_extracted(self):
        c=contract();self.assertEqual(len(c['periodic_checks']),1)
        self.assertEqual(add_public_behavior_checks(c,TEXT),c)
        raw=copy.deepcopy(c);raw.pop('periodic_checks')
        for text in [TEXT.replace('音调1000Hz','有声音'),TEXT.replace('每次持续100ms','短促鸣叫'),TEXT.replace('保留计时和剩余鸣叫时长','恢复时重新计时'),
                     TEXT.replace('此后每累计运行1000ms开始一次','不要此后每累计运行1000ms开始一次'),TEXT+'音调500Hz。',TEXT.replace('累计运行1000ms后首次','累计运行2000ms后首次')]:
            with self.subTest(text=text):self.assertNotIn('periodic_checks',add_public_behavior_checks(raw,text))
        bad=copy.deepcopy(c);bad['periodic_checks'][0]['period_ms']=2000
        with self.assertRaisesRegex(ValueError,'conflicts'):add_public_behavior_checks(bad,TEXT)
        english=('Initially running and silent. First pulse after 1000ms of active time; then every 1000ms of active time. '
                 'Each pulse lasts 100ms; pitch 1000Hz. Press-edge toggles pause/resume; pause immediately silences; '
                 'preserve phase and remaining pulse duration. Timing tolerance 20ms.')
        self.assertEqual(add_public_behavior_checks(raw,english)['periodic_checks'][0]['period_ms'],1000)

    def test_malformed_or_vacuous_assertions_reject_before_model_execution(self):
        for key,value in [('output',[]),('delta','frequency'),('period_ms',True),('pulse_ms',0),('pulse_ms',1000),
                          ('active_value',True),('inactive_value',1000),('tolerance_ms',50),('first_pulse','later'),
                          ('initial_running',1),('pause_policy','ignore'),('evidence','')]:
            c=contract();c['periodic_checks'][0][key]=value
            with self.subTest(key=key,value=value),self.assertRaises(ValueError):validate_contract(c)
        c=contract();c['periodic_checks'][0]['period_ms']=2147483647
        with self.assertRaisesRegex(ValueError,'verification budget'):reference_trace(c['periodic_checks'][0],10)

    def test_reference_timing_edges_are_exact_even_with_jitter(self):
        check=contract()['periodic_checks'][0]
        trace,edges=reference_trace(check,10)
        values={t:v for t,dt,pressed,v in trace}
        self.assertEqual([values[t] for t in [0,990,1000,1090,1100,1990,2000]],[0,0,1000,1000,0,0,1000])
        _,jitter_edges=reference_trace(check,10,jitter=True)
        self.assertEqual(edges[:6],[1000,1100,2000,2100,3000,3100])
        self.assertEqual(jitter_edges[:6],edges[:6])
        jitter_trace,_=reference_trace(check,10,jitter=True)
        self.assertEqual({dt for _,dt,_,_ in jitter_trace},{0,10,20})
        check['tolerance_ms']=0
        trace,_=reference_trace(check,10,jitter=True)
        self.assertEqual({dt for _,dt,_,_ in trace},{0,10})

    def test_direct_ir_float_assertion_obeys_declared_precision(self):
        c=contract();c.pop('startup_checks');c.pop('pause_checks')
        c['interfaces']=[p for p in c['interfaces'] if p['name']!='pressed']
        c['required_dependencies']={'frequency':['deltaMs']}
        out=next(p for p in c['interfaces'] if p['name']=='frequency');out.update(type='f32',initial=0.0)
        check=c['periodic_checks'][0]
        for field in ('pause_input','initial_running','pause_policy'):check.pop(field)
        check.update(active_value=0.1,inactive_value=0.0)
        ir=dict(version=2,name='FloatPulse',page='ProgramPage1',execution='step-actions-then-outputs',
            inputs=[dict(name='deltaMs',type='i32')],outputs=[dict(name='frequency',type='f32')],
            states=[dict(name='elapsed',type='i32',initial='0')],
            updates=[dict(kind='assign',state='elapsed',expression='arg0+arg1',sources=[dict(kind='state',name='elapsed'),dict(kind='input',name='deltaMs')])],
            nodes=[dict(id='pulse',type='Expression',inports=[dict(name='time',type='i32')],outports=[dict(name='out',type='f32')],
                        params={'Expression':'(time>=1000 && time%1000<100) ? 0.1f : 0.0f'})],
            edges=[dict(src=dict(kind='state',name='elapsed'),dst=dict(kind='node',id='pulse',port='time')),
                   dict(src=dict(kind='node',id='pulse',port='out'),dst=dict(kind='output',name='frequency'))])
        result=check_contract(ir,c)
        self.assertEqual(result['issues'],[]);self.assertEqual(result['periodic']['status'],'passed')
        check.update(active_value=1.00000001,inactive_value=1.00000002)
        with self.assertRaisesRegex(ValueError,'distinguish'):validate_contract(c)

    def test_period_width_and_resume_failures_have_executed_counterexamples(self):
        c=contract();good=controller()
        ir=compile_sysml(assemble_sysml(good,c));result=check_contract(ir,c)
        self.assertEqual(result['issues'],[]);self.assertEqual(result['periodic']['status'],'passed')
        self.assertGreater(result['periodic']['checked'],1000);self.assertEqual(result['functional_status'],'unverified')
        for field,value in [('period_ms',1100),('pulse_ms',200),('pause','reset')]:
            bad=controller();bad['components'][1][field]=value
            with self.subTest(field=field):
                result=check_contract(compile_sysml(assemble_sysml(bad,c)),c)
                self.assertIn('CONTRACT_PERIODIC',[i['code'] for i in result['issues']])
                self.assertEqual(result['startup']['status'],'passed');self.assertEqual(result['pause']['status'],'passed')
                self.assertIn('history',result['periodic']['counterexample'])

    def test_assertion_gate_is_shared_by_api_and_feedback(self):
        c=contract();bad=controller();bad['components'][1]['period_ms']=1100
        source=assemble_sysml(bad,c);ir=compile_sysml(source)
        with tempfile.TemporaryDirectory() as folder,TestClient(app) as client,patch('app.main._call_llm',side_effect=AssertionError('No LLM')):
            target=Path(folder)/'compiled'
            response=client.post('/compile/sysml2',json=dict(system_sysml2=source,interface_contract=c,out_dir=str(target)))
            self.assertEqual(response.status_code,422,response.text);self.assertIn('CONTRACT_PERIODIC',response.text);self.assertFalse(target.exists())
            response=client.post('/validate/subsystem-ir',json=dict(ir=ir,interface_contract=c))
            self.assertFalse(response.json()['ok']);self.assertIn('CONTRACT_PERIODIC',response.text)
        mapping=dict(requirements_to_testpoints=[dict(requirement_id='R1',requirement_text=TEXT,test_point_id='T1',test_method='trace',stimulus='input',oracle='timing')])
        responses=iter([json.dumps(dict(controller=s,req_test_map=mapping)) for s in [bad,controller()]]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',next(responses)
        _,_,report=generate_validated_bundle(call,TEXT,contract=c,validator=validate_executable_sysml,max_repairs=1)
        self.assertEqual([a['ok'] for a in report['attempts']],[False,True]);self.assertIn('start-to-start',prompts[1])
        self.assertIn('CONTRACT_PERIODIC',prompts[1]);self.assertIn(source,prompts[1])

    def test_generic_boolean_reset_pulse_with_independent_manual_state_machine(self):
        c=contract();c.pop('startup_checks');c.pop('pause_checks')
        for p in c['interfaces']:p['sample_period_ms']=5
        c['control_period_ms']=5
        out=next(p for p in c['interfaces'] if p['name']=='frequency')
        out.update(type='bool',initial=False,unit='1',device='LED',binding=dict(kind='digital_output',pin=6))
        c['periodic_checks']=[dict(delta='deltaMs',output='frequency',period_ms=715,pulse_ms=75,
            first_pulse='immediate',active_value=True,inactive_value=False,tolerance_ms=5,evidence='Caller-specified LED pulse schedule',
            pause_input='pressed',initial_running=False,pause_policy='reset')]
        spec=dict(states=[dict(name='run',type='bool',initial='false'),dict(name='previous',type='bool',initial='false'),dict(name='phase',type='i32',initial='0')],
            updates=[{'if':'run','then':[{'assign':'phase','value':'phase+deltaMs'},
                       {'if':'phase>=715','then':[{'assign':'phase','value':'phase-715'}]}]},
                     {'if':'pressed and not previous','then':[{'assign':'run','value':'not run'},
                       {'if':'not run','then':[{'assign':'phase','value':'0'}]}]},
                     {'assign':'previous','value':'pressed'}],outputs={'frequency':'run and phase<75'})
        result=check_contract(compile_sysml(assemble_sysml(spec,c)),c)
        self.assertEqual(result['issues'],[]);self.assertEqual(result['periodic']['status'],'passed')


if __name__=='__main__':unittest.main()
