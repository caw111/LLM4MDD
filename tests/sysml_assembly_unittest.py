import copy
import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

from app.services.sysml_assembly import assemble_sysml
from app.services.sysml import generate_validated_bundle, SysMLValidationError
from app.services.sysml_compiler import compile_sysml, ir_to_il, validate_executable_sysml


def contract(inputs, outputs):
    return {'version': 1, 'control_period_ms': 10, 'interfaces': [
        dict(name=name, direction=direction, type=typ, unit='1', meaning=name,
             sample_period_ms=10, initial=False if typ == 'bool' else 0,
             hardware='test harness', device='test')
        for direction, ports in [('input', inputs), ('output', outputs)] for name, typ in ports],
        'requirements': [dict(id='R1', text='independent trace', assertion_ids=['trace'])]}


def candidate(controller):
    return json.dumps({'controller': controller, 'req_test_map': {'requirements_to_testpoints': [
        dict(requirement_id='R1', requirement_text='trace', test_point_id='T1',
             test_method='execution', stimulus='input sequence', oracle='expected trace')]}})


class AssemblyTests(unittest.TestCase):
    def test_json_literals_preserve_types_and_order_in_executable_model(self):
        spec={'states':[{'name':'count','type':'i32','initial':-2},
                        {'name':'memory','type':'bool','initial':False}],
              'updates':[{'if':'enabled','then':[{'assign':'memory','value':True}]},
                         {'assign':'count','value':{'+':['count',2]}}],
              'outputs':{'flag':{'and':['memory',{'not':False}]},
                         'value':{'+':['count',{'if':'enabled','then':2.5,'else':-1.25}]}}}
        self.execute(spec,contract([('enabled','bool')],[('flag','bool'),('value','f64')]),'''
int main(){bool flag=false;double value=0;ProgramPage1_Init(&ProgramPage1_instance);
 ProgramPage1(false,&flag,&value);assert(!flag&&value==-1.25);
 ProgramPage1(true,&flag,&value);assert(flag&&value==4.5);
 ProgramPage1(false,&flag,&value);assert(flag&&value==2.75);}
''')

    def test_json_literals_do_not_coerce_types_or_accept_nonfinite_values(self):
        from app.services.sysml_assembly import expression, ExpressionSyntaxError
        self.assertEqual(expression(1e20), '100000000000000000000.0')
        self.assertEqual(expression(1e-7), '0.0000001')
        for value in (float('nan'),float('inf'),float('-inf')):
            with self.subTest(value=value),self.assertRaises(ExpressionSyntaxError):expression(value)
        with self.assertRaises(ValueError):expression(None)
        # A JSON true cannot turn into Integer 1, nor a numeric 1 into Boolean true.
        for typ,value in [('i32',True),('bool',1)]:
            spec={'states':[],'updates':[],'outputs':{'result':value}}
            self.assertFalse(validate_executable_sysml(assemble_sysml(spec,contract([],[('result',typ)])))['ok'])

    def execute(self, spec, inventory, harness):
        from app.main import convert_il_to_c, ConvertIlToCRequest
        source = assemble_sysml(spec, inventory)
        ir = compile_sysml(source)  # Real reference parser and semantic model.
        with tempfile.TemporaryDirectory() as folder:
            base = Path(folder)
            xml = base/'model_IL.xml'
            xml.write_text(ir_to_il(ir), encoding='utf-8')
            generated = convert_il_to_c(ConvertIlToCRequest(il_file_path=str(xml),
                out_c_dir=str(base/'c'), codegen_root=str(Path('CodeGenerator2/CodeGenerator2').resolve()),
                optimize=False, target='Intel'))
            program = '#include <cassert>\n'+generated.contents['define.h']+'\n'+generated.contents['ProgramPage1.c'].replace('#include "define.h"', '')+'\n'+harness
            (base/'test.cpp').write_text(program, encoding='utf-8')
            binary = base/'test.exe'
            build = subprocess.run([shutil.which('g++'), '-std=c++17', str(base/'test.cpp'), '-o', str(binary)], capture_output=True, text=True)
            self.assertEqual(build.returncode, 0, build.stderr)
            run = subprocess.run([str(binary)], capture_output=True, text=True, timeout=10)
            self.assertEqual(run.returncode, 0, run.stderr)
        return ir

    def test_structured_operators_preserve_operands_precedence_and_boolean_truth(self):
        spec={'states':[],'updates':[],'outputs':{
            'both':{'and':['a',{'not':'b'},{'>=':['sample','2']}]},
            'either':{'or':['a','b',{'==':['sample','3']}]},
            'parity':{'xor':['a','b',{'<':['sample','0']}]},
            'value':{'*':[{'+':['sample','2']},{'-':['sample','1']}]}}}
        self.execute(spec,contract([('a','bool'),('b','bool'),('sample','i32')],
            [('both','bool'),('either','bool'),('parity','bool'),('value','i32')]),'''
int main(){ProgramPage1_Init(&ProgramPage1_instance);
 for(int a=0;a<2;++a)for(int b=0;b<2;++b)for(int x=-2;x<=3;++x){
 bool both=false,either=false,parity=false;int value=0;
 ProgramPage1(a,b,x,&both,&either,&parity,&value);
 assert(both==(a&&!b&&x>=2));assert(either==(a||b||x==3));
 assert(parity==((bool(a)!=bool(b))!=(x<0)));assert(value==(x+2)*(x-1));}}
''')

    def test_explicit_noop_branches_keep_guards_state_and_following_order(self):
        spec={'states':[{'name':'count','type':'i32','initial':'5'}],
              'updates':[{'if':'enabled','then':[],
                          'else':[{'assign':'count','value':'count + 1'}]},
                         {'if':'count > 0','then':[],'else':[]},
                         {'assign':'count','value':'count * 2'}],
              'outputs':{'value':'count'}}
        ir=self.execute(spec,contract([('enabled','bool')],[('value','i32')]),'''
int main(){int value=0;ProgramPage1_Init(&ProgramPage1_instance);
 ProgramPage1(true,&value);assert(value==10);
 ProgramPage1(false,&value);assert(value==22);
 ProgramPage1(true,&value);assert(value==44);}
''')
        steps=ir['updates'][0]['steps']
        self.assertEqual(steps[0]['then'],{'kind':'sequence','steps':[]})
        self.assertEqual(steps[1]['kind'],'if')
        # 空分支不能令非法引用或非 Boolean 条件绕过完整校验。
        for guard in ('undeclaredFlag','42'):
            invalid=copy.deepcopy(spec);invalid['updates'][1]['if']=guard
            self.assertFalse(validate_executable_sysml(assemble_sysml(invalid,
                contract([('enabled','bool')],[('value','i32')])))['ok'])

    def test_structured_operator_shapes_and_operand_types_are_checked(self):
        from app.services.sysml_assembly import expression, ExpressionSyntaxError
        for value in ({'and':[]},{'or':['true']},{'==':['1','1','1']},
                      {'+':'1 + 2'},{'and':['true','false'],'extra':'true'},
                      {'unknown':['true','false']}):
            with self.subTest(value=value),self.assertRaises(ExpressionSyntaxError):
                expression(value)
        for operand in ('42','unknownInput'):
            spec={'states':[],'updates':[],'outputs':{'lamp':{'and':['pressed',operand]}}}
            self.assertFalse(validate_executable_sysml(assemble_sysml(spec,
                contract([('pressed','bool')],[('lamp','bool')])))['ok'])

    def test_structured_negation_preserves_boolean_semantics_and_checks_types(self):
        spec={'states':[],'updates':[],'outputs':{'lamp':{'not':'pressed'}}}
        self.execute(spec,contract([('pressed','bool')],[('lamp','bool')]),'''
int main(){bool lamp=false;ProgramPage1_Init(&ProgramPage1_instance);
 ProgramPage1(false,&lamp);assert(lamp);ProgramPage1(true,&lamp);assert(!lamp);}
''')
        invalid={'states':[],'updates':[],'outputs':{'lamp':{'not':'42'}}}
        checked=validate_executable_sysml(assemble_sysml(invalid,contract([],[('lamp','bool')])))
        self.assertFalse(checked['ok'])
        invalid['outputs']['lamp']={'not':'true','extra':'false'}
        with self.assertRaises(ValueError):assemble_sysml(invalid,contract([],[('lamp','bool')]))

    def test_stateful_timer_keeps_order_initialization_and_reset(self):
        spec = {'states': [{'name':'elapsed','type':'i32','initial':'0'}, {'name':'lit','type':'bool','initial':'false'}],
                'updates': [{'assign':'elapsed','value':'elapsed + delta'},
                            {'if':'elapsed >= 500','then':[{'assign':'elapsed','value':'elapsed - 500'}, {'assign':'lit','value':'not lit'}]}],
                'outputs': {'lamp':'lit'}}
        self.execute(spec, contract([('delta','i32')],[('lamp','bool')]), '''
int main() {
 bool lamp = true; ProgramPage1_Init(&ProgramPage1_instance);
 ProgramPage1(0,&lamp); assert(!lamp);
 ProgramPage1(499,&lamp); assert(!lamp);
 ProgramPage1(1,&lamp); assert(lamp);
 ProgramPage1(500,&lamp); assert(!lamp);
 ProgramPage1_Init(&ProgramPage1_instance);
 ProgramPage1(0,&lamp); assert(!lamp);
 ProgramPage1(500,&lamp); assert(lamp);
}''')

    def test_repeated_module_instances_preserve_connections(self):
        spec = {'states': [], 'updates': [], 'outputs': {'result':'left.y + right.y'},
                'definitions': [{'name':'Square','inputs':[{'name':'x','type':'f64'}],
                    'output_ports':[{'name':'y','type':'f64'}],
                    'body':{'states':[],'updates':[],'outputs':{'y':'x*x'}}}],
                'parts':[{'name':'left','definition':'Square'},{'name':'right','definition':'Square'}],
                'bindings':[{'left':'a','right':'left.x'},{'left':'right.x','right':'b'}]}
        self.execute(spec, contract([('a','f64'),('b','f64')],[('result','f64')]), '''
int main() {
 double result=0; ProgramPage1_Init(&ProgramPage1_instance);
 ProgramPage1(3.0,4.0,&result); assert(result==25.0);
 ProgramPage1(-2.0,1.0,&result); assert(result==5.0);
}''')

    def test_explicit_floor_preserves_integer_contract_and_negative_rounding(self):
        spec = {'states':[], 'updates':[], 'outputs':{'result':'RealFunctions::floor(sample * 255.0 / 1023.0)'}}
        ir = self.execute(spec, contract([('sample','f64')],[('result','i32')]), '''
int main() {
 int result=0; ProgramPage1_Init(&ProgramPage1_instance);
 ProgramPage1(1023.0,&result); assert(result==255);
 ProgramPage1(511.0,&result); assert(result==127);
 ProgramPage1(-1.0,&result); assert(result==-1);
 ProgramPage1(-1023.0,&result); assert(result==-255);
}''')
        self.assertEqual(ir['outputs'][0]['type'], 'i32')

    def test_structured_conditional_preserves_nested_selection_and_floor(self):
        spec = {'states':[], 'updates':[], 'outputs':{'result':{
            'if':'enabled', 'then':{'if':'sample >= 0.0', 'then':'RealFunctions::floor(sample * 255.0 / 1023.0)', 'else':'0'}, 'else':'0'}}}
        self.execute(spec, contract([('enabled','bool'),('sample','f64')],[('result','i32')]), '''
int main() {
 int result=-1; ProgramPage1_Init(&ProgramPage1_instance);
 ProgramPage1(false,1023.0,&result); assert(result==0);
 ProgramPage1(true,1023.0,&result); assert(result==255);
 ProgramPage1(true,511.0,&result); assert(result==127);
 ProgramPage1(true,-1.0,&result); assert(result==0);
}''')

    def test_conditional_expression_cannot_contain_action_arrays(self):
        from app.services.sysml_assembly import expression
        with self.assertRaises(ValueError):
            expression({'if':'enabled','then':[{'assign':'state','value':'0'}],'else':'0'})
        value = '0'
        for _ in range(34): value={'if':'enabled','then':value,'else':'0'}
        with self.assertRaisesRegex(ValueError,'nesting'):
            expression(value)

    def test_branch_local_arithmetic_and_short_circuit_do_not_pre_evaluate(self):
        spec={'states':[], 'updates':[], 'outputs':{
            'selected':{'if':'enabled','then':'sample / (sample - 1.0)','else':'0.0'},
            'both':'enabled and ((sample / (sample - 1.0)) > 1.0)',
            'either':'not enabled or ((sample / (sample - 1.0)) > 1.0)'}}
        self.execute(spec,contract([('enabled','bool'),('sample','f64')],[('selected','f64'),('both','bool'),('either','bool')]),'''
#include <cfenv>
int main() {
 double selected=0; bool both=false,either=false; ProgramPage1_Init(&ProgramPage1_instance);
 std::feclearexcept(FE_ALL_EXCEPT);
 ProgramPage1(false,1.0,&selected,&both,&either); assert(selected==0 && !both && either);
 assert(!std::fetestexcept(FE_DIVBYZERO | FE_INVALID));
 ProgramPage1(false,2.0,&selected,&both,&either); assert(selected==0 && !both && either);
 ProgramPage1(true,2.0,&selected,&both,&either); assert(selected==2 && both && either);
}''')

    def test_exact_triangle_wave_backend_regression(self):
        spec={'states':[],'updates':[],'outputs':{'value':{'if':'elapsed <= 1000',
            'then':'1.0 * elapsed * 255.0 / 1000.0','else':'1.0 * (2000 - elapsed) * 255.0 / 1000.0'}}}
        self.execute(spec,contract([('elapsed','i32')],[('value','f64')]),'''
int main(){double value=0;ProgramPage1_Init(&ProgramPage1_instance);
 ProgramPage1(0,&value);assert(value==0);ProgramPage1(1000,&value);assert(value==255);
 ProgramPage1(1500,&value);assert(value==127.5);ProgramPage1(2000,&value);assert(value==0);}
''')

    def test_illegal_interface_and_action_redefinitions_fail(self):
        base = {'states': [], 'updates': [], 'outputs': {'lamp':'pressed'}}
        inventory = contract([('pressed','bool')],[('lamp','bool')])
        variants = []
        v = copy.deepcopy(base); v['states']=[{'name':'lamp','type':'bool','initial':'false'}]; variants.append(v)
        v = copy.deepcopy(base); v['outputs']['extra']='false'; variants.append(v)
        v = copy.deepcopy(base); v['updates']=[{'assign':'lamp','value':'true'}]; variants.append(v)
        v = copy.deepcopy(base); v['outputs']['lamp']='true; } part system : Controller;'; variants.append(v)
        v = copy.deepcopy(base); v['system_sysml2']='package Extra {}'; variants.append(v)
        v = copy.deepcopy(base); v['outputs']['lamp']='pressed ? true :'; variants.append(v)
        for spec in variants:
            with self.subTest(spec=spec), self.assertRaises(ValueError):
                assemble_sysml(spec, inventory)

    def test_feedback_retains_rendered_source_and_runs_full_reference_validation(self):
        inventory = contract([('pressed','bool')],[('lamp','bool')])
        inventory['required_dependencies'] = {'lamp':['pressed']}
        prompts, sources = [], []
        replies = iter([candidate({'states':[],'updates':[],'outputs':{'lamp':'missingInput'}}),
                        candidate({'states':[],'updates':[],'outputs':{'lamp':'pressed'}})])
        def call(prompt):
            prompts.append(prompt)
            return 'fixture', next(replies)
        def validate(source):
            sources.append(source)
            return validate_executable_sysml(source)
        _, bundle, report = generate_validated_bundle(call, 'Button controls LED', contract=inventory, validator=validate, max_repairs=1)
        self.assertEqual([a['ok'] for a in report['attempts']], [False, True])
        self.assertEqual(len(sources), 2)
        self.assertIn(sources[0], prompts[1])
        self.assertEqual(report['attempts'][0]['rendered_sysml2'], sources[0])
        self.assertEqual(bundle['system_sysml2'], sources[1])
        self.assertIn('exactly controller and req_test_map', prompts[1])
        with self.assertRaises(SysMLValidationError):
            generate_validated_bundle(lambda _: ('fixture', json.dumps({'system_sysml2':sources[1]})),
                'requirement', contract=inventory, max_repairs=0)


if __name__ == '__main__': unittest.main()
