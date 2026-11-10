import copy
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from fastapi.testclient import TestClient
from app.main import app
from app.services.interface_planner import add_public_behavior_checks, plan_interface_contract
from app.services.behavior_contract import validate_contract, check_contract
from app.services.periodic_contract import reference_trace
from app.services.sysml_assembly import assemble_sysml, expression, ExpressionSyntaxError
from app.services.sysml_compiler import compile_sysml, validate_executable_sysml
from app.services.sysml import generate_validated_bundle
import tests.sysml_assembly_unittest as assembly_tests

TEXT=('D2按钮控制D13 LED；每次按下沿切换运行/停止，长按或松开不切换。'
      '初始停止且D13熄灭；启动时从熄灭开始，每370ms翻转；停止立即熄灭，再启动重新计时。'
      '响应及翻转时间容差20ms。')


def inventory():
    return dict(version=1,control_period_ms=10,interfaces=[
        dict(name='pressed',direction='input',type='bool',unit='1',meaning='pressed',sample_period_ms=10,
             initial=False,device='button',hardware='D2',binding=dict(kind='digital_input',pin=2,pullup=True,active_high=False)),
        dict(name='dt',direction='input',type='i32',unit='ms',meaning='elapsed',sample_period_ms=10,
             initial=0,device='clock',hardware='millis',binding=dict(kind='elapsed_ms')),
        dict(name='lamp',direction='output',type='bool',unit='1',meaning='LED on',sample_period_ms=10,
             initial=False,device='LED',hardware='D13',binding=dict(kind='digital_output',pin=13))],
        requirements=[dict(id='R1',text=TEXT,assertion_ids=['timing'])])


def controller():
    return dict(states=[],updates=[],components=[
        dict(kind='edge_toggle',name='run',input='pressed',edge='rising',initial=False,previous_input=False),
        dict(kind='periodic_pulse',name='blink',delta='dt',enabled='component_run_value',
             period_ms=740,pulse_ms=370,first_pulse='after_silence',pause='reset')],
        outputs={'lamp':'component_blink_value'})


class ToggleTests(unittest.TestCase):
    execute=assembly_tests.AssemblyTests.execute

    def test_extracts_only_complete_unambiguous_public_toggle_requirements(self):
        raw=inventory();before=copy.deepcopy(raw)
        c=add_public_behavior_checks(raw,TEXT);validate_contract(c)
        self.assertEqual(raw,before)
        self.assertEqual(c['periodic_checks'][0]['period_ms'],740)
        self.assertEqual(c['periodic_checks'][0]['first_pulse'],'after_silence')
        self.assertEqual(add_public_behavior_checks(c,TEXT),c)
        for text in [TEXT.replace('每370ms翻转','不要每370ms翻转'),TEXT+'每400ms翻转。',
                     TEXT.replace('再启动重新计时','恢复旧计时'),TEXT.replace('从熄灭开始','立即点亮'),
                     TEXT.replace('每370ms翻转','如果温度升高，每370ms翻转'),TEXT.replace('容差20ms','容差适当')]:
            with self.subTest(text=text):self.assertNotIn('periodic_checks',add_public_behavior_checks(raw,text))
        ambiguous=copy.deepcopy(raw);ambiguous['interfaces'].append({**raw['interfaces'][-1],'name':'other'})
        self.assertNotIn('periodic_checks',add_public_behavior_checks(ambiguous,TEXT))
        bad=copy.deepcopy(c);bad['periodic_checks'][0]['period_ms']=370
        with self.assertRaisesRegex(ValueError,'conflicts'):add_public_behavior_checks(bad,TEXT)
        plain=inventory();plain['interfaces']=plain['interfaces'][1:]
        check=add_public_behavior_checks(plain,'初始熄灭，370ms时首次点亮，此后每370ms翻转。时间容差20ms。')['periodic_checks'][0]
        self.assertNotIn('pause_input',check)
        english=('Initially stopped; starts from off; toggles every 370ms. Press-edge toggles run/stop. '
                 'Stopping immediately turns LED off; restarting resets timing. Timing tolerance 20ms.')
        self.assertEqual(add_public_behavior_checks(raw,english)['periodic_checks'][0]['period_ms'],740)

    def test_first_delay_reference_and_generated_code_use_full_on_off_period(self):
        c=add_public_behavior_checks(inventory(),TEXT);check=c['periodic_checks'][0]
        trace,_=reference_trace(check,10)
        values={t:v for t,_,_,v in trace}
        # Initial press at 30ms, then a 370ms OFF interval before first ON.
        self.assertEqual([values[t] for t in [0,30,390,400,760,770,1130,1140]],
                         [False,False,False,True,True,False,False,True])
        self.execute(controller(),c,'''
int main(){
 ProgramPage1_Init(&ProgramPage1_instance);bool lamp=true;
 ProgramPage1(false,0,&lamp);assert(!lamp);
 ProgramPage1(true,10,&lamp);assert(!lamp);
 ProgramPage1(false,369,&lamp);assert(!lamp);
 ProgramPage1(false,1,&lamp);assert(lamp);
 ProgramPage1(false,370,&lamp);assert(!lamp);
 ProgramPage1(false,370,&lamp);assert(lamp);
 ProgramPage1(true,10,&lamp);assert(!lamp);
 ProgramPage1(false,91,&lamp);assert(!lamp);
 ProgramPage1(true,2147483647,&lamp);assert(!lamp);
 ProgramPage1(false,369,&lamp);assert(!lamp);
 ProgramPage1(false,1,&lamp);assert(lamp);
}''')

    def test_asymmetric_first_silence_handles_large_delta_without_overflow(self):
        c=inventory();c['interfaces']=c['interfaces'][1:]
        spec=controller();spec['components']=spec['components'][1:]
        spec['components'][0].update(enabled='true',period_ms=997,pulse_ms=113,pause='preserve')
        self.execute(spec,c,'''
int main(){
 ProgramPage1_Init(&ProgramPage1_instance);bool lamp=true;long long elapsed=0;
 int deltas[]={0,883,1,112,1,3100,2147483647,17,2147483647};
 for(int delta:deltas){elapsed+=delta;ProgramPage1(delta,&lamp);
 assert(lamp==(elapsed>=884&&(elapsed-884)%997<113));}
}''')

    def test_wrong_period_phase_and_reset_are_rejected_before_codegen(self):
        c=add_public_behavior_checks(inventory(),TEXT)
        result=check_contract(compile_sysml(assemble_sysml(controller(),c)),c)
        self.assertEqual(result['issues'],[])
        for changes in [dict(period_ms=370,pulse_ms=185),dict(first_pulse='immediate'),dict(pause='preserve')]:
            spec=controller();spec['components'][1].update(changes)
            with self.subTest(changes=changes):
                checked=check_contract(compile_sysml(assemble_sysml(spec,c)),c)
                self.assertIn('CONTRACT_PERIODIC',[i['code'] for i in checked['issues']])
        spec=controller();spec['components'][1]['pulse_ms']=740
        with self.assertRaisesRegex(ValueError,'pulse_ms < period_ms'):assemble_sysml(spec,c)

    def test_planning_and_supplied_contract_share_the_new_api_gate(self):
        planned,_=plan_interface_contract(lambda _:('fixture',json.dumps(inventory())),TEXT,{},max_repairs=0)
        with tempfile.TemporaryDirectory() as folder,TestClient(app) as client,patch('app.main._call_llm',side_effect=AssertionError('No LLM')):
            response=client.post('/generate/interface-contract',json=dict(requirement=TEXT,interface_contract=inventory(),out_path=str(Path(folder)/'contract.json')))
            self.assertEqual(response.status_code,200,response.text)
            self.assertEqual(response.json()['interface_contract'],planned)
            bad=controller();bad['components'][1].update(period_ms=370,pulse_ms=185)
            response=client.post('/compile/sysml2',json=dict(system_sysml2=assemble_sysml(bad,planned),
                interface_contract=planned,out_dir=str(Path(folder)/'rejected')))
            self.assertEqual(response.status_code,422,response.text)
            self.assertIn('CONTRACT_PERIODIC',[i['code'] for i in response.json()['detail']['issues']])
            self.assertFalse((Path(folder)/'rejected').exists())


class ExpressionLocationTests(unittest.TestCase):
    def test_c_style_and_non_scalar_expressions_keep_their_original_location(self):
        for value in ('!pressed', 'pressed && true', 'pressed ? 1 : 0', None, [], ['pressed'], '',
                      '>=(count + 37, 100)', '>(count, 99)', '+(count, 37)', '>=', 'and'):
            spec=controller()
            spec['updates']=[{'if':value,'then':[]}]
            before=copy.deepcopy(spec)
            with self.subTest(value=value),self.assertRaises(ExpressionSyntaxError) as caught:
                assemble_sysml(spec,inventory())
            self.assertEqual(caught.exception.path,'/controller/updates/0/if')
            self.assertEqual(spec,before)
        spec=controller();spec['outputs']['lamp']='not pressed'
        self.assertIn('not pressed',assemble_sysml(spec,inventory()))

    def test_invalid_conditional_and_remainder_report_exact_original_json_pointer(self):
        for value,part in [('if remaining > 0 then remaining - dt else 0','conditional'),('remaining % 3000','remainder')]:
            spec=controller();spec['states']=[dict(name='remaining',type='i32',initial=0)]
            spec['updates']=[{'if':'pressed','then':[{'assign':'remaining','value':value}]}]
            with self.subTest(value=value),self.assertRaises(ExpressionSyntaxError) as caught:
                assemble_sysml(spec,inventory())
            self.assertEqual(caught.exception.path,'/controller/updates/0/then/0/value')
            self.assertIn(part,str(caught.exception))
        self.assertEqual(expression('(if pressed ? 1 else 0)'),'(if pressed ? 1 else 0)')

    def test_repeated_expression_feedback_keeps_full_model_and_revalidates(self):
        c=inventory();good=controller()
        good['states']=[dict(name='count',type='i32',initial=0)]
        good['updates']=[dict(assign='count',value={'if':'pressed','then':'count + 1','else':'count'})]
        bad=copy.deepcopy(good);bad['updates'][0]['value']='if pressed then count + 1 else count'
        specs=iter([bad,bad,good]);prompts=[]
        mapping={'requirements_to_testpoints':[dict(requirement_id='R1',requirement_text=TEXT,
                 test_point_id='T1',test_method='trace',stimulus='button',oracle='timing')]}
        def call(prompt):prompts.append(prompt);return 'fixture',json.dumps(dict(controller=next(specs),req_test_map=mapping))
        _,accepted,report=generate_validated_bundle(call,TEXT,contract=c,validator=validate_executable_sysml,max_repairs=2)
        self.assertEqual([a['ok'] for a in report['attempts']],[False,False,True])
        self.assertTrue(report['attempts'][1]['no_progress'])
        self.assertIn('/controller/updates/0/value',prompts[1])
        self.assertIn('NO PROGRESS',prompts[2])
        self.assertIn(TEXT,prompts[2])
        self.assertEqual(accepted['controller'],good)


if __name__=='__main__':unittest.main()
