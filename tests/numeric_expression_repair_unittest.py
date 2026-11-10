"""Numeric syntax repair must preserve executable semantics and frozen types."""
import copy
import json
from pathlib import Path
import unittest

from app.services.sysml import generate_validated_bundle, repair_guidance, sysml_bundle_prompt
from app.services.sysml_assembly import assemble_sysml, expression, ExpressionSyntaxError
from app.services.sysml_compiler import validate_executable_sysml
from tests import sysml_assembly_unittest as helpers


class NumericRepairTests(unittest.TestCase):
    execute = helpers.AssemblyTests.execute

    def test_structured_floor_and_parity_use_real_parser_backend_and_negative_semantics(self):
        spec=dict(states=[],updates=[],outputs={
            'rounded':{'floor':{'if':'enabled','then':'sample','else':0.0}},
            'qualified':{'RealFunctions::floor':'sample'},
            'scaled':{'floor':{'/':[{'*':['sample',3.0]},2.0]}},
            'even':'integer == 2 * RealFunctions::floor(integer / 2)',
            'remainder':'nonnegative - 7 * RealFunctions::floor(nonnegative / 7)'})
        c=helpers.contract([('enabled','bool'),('sample','f64'),('integer','i32'),('nonnegative','i32')],
                           [('rounded','i32'),('qualified','i32'),('scaled','i32'),('even','bool'),('remainder','i32')])
        before=copy.deepcopy(spec)
        self.execute(spec,c,'''
int main(){ProgramPage1_Init(&ProgramPage1_instance);
 double samples[]={-3.5,-0.25,0.0,2.75,8.0};int expected[]={-4,-1,0,2,8};int scaledExpected[]={-6,-1,0,4,12};
 for(int i=0;i<5;++i)for(int enabled=0;enabled<2;++enabled)for(int n=-99;n<=99;++n){
 int rounded=0,qualified=0,scaled=0,remainder=0;bool even=false;int positive=n+99;
 ProgramPage1(enabled,samples[i],n,positive,&rounded,&qualified,&scaled,&even,&remainder);
 assert(rounded==(enabled?expected[i]:0));assert(qualified==expected[i]);assert(scaled==scaledExpected[i]);
 assert(even==(n%2==0));assert(remainder==positive%7);}}
''')
        self.assertEqual(spec,before)

    def test_floor_does_not_bypass_shape_reference_or_type_checks(self):
        for value in ({'floor':[]},{'floor':['sample']},{'floor':None},
                      {'floor':'sample','extra':1},{'ceil':'sample'}):
            with self.subTest(value=value),self.assertRaises(ValueError):expression(value)
        for operand in ('true','missingSample'):
            spec=dict(states=[],updates=[],outputs={'result':{'floor':operand}})
            checked=validate_executable_sysml(assemble_sysml(spec,helpers.contract([],[('result','i32')])))
            self.assertFalse(checked['ok'],checked)

    def test_remainder_diagnostic_is_arithmetic_specific_and_preserves_candidate(self):
        for value in ('n % 2',{'mod':['n',2]},{'%':['n',2]},{'remainder':['n',2]}):
            before=copy.deepcopy(value)
            with self.subTest(value=value),self.assertRaises(ExpressionSyntaxError) as caught:
                assemble_sysml(dict(states=[],updates=[],outputs={'result':value}),
                               helpers.contract([('n','i32')],[('result','i32')]))
            message=str(caught.exception)
            self.assertIn('/controller/outputs/result',message)
            self.assertIn('n == 2 * RealFunctions::floor(n / 2)',message)
            self.assertIn('negative dividends',message)
            guidance=repair_guidance([{'code':'EXPRESSION_SYNTAX','message':message}],contract_assembly=True)
            self.assertNotIn('periodic_pulse',guidance)
            self.assertIn('stateless arithmetic',guidance)
            self.assertEqual(value,before)

    def test_numeric_example_selection_keeps_other_profiles_and_compiles(self):
        c=json.loads(Path('tools/examples/shift595_parity.contract.json').read_text(encoding='utf-8'))
        prompt=sysml_bundle_prompt('Map ADC to a number; even and odd choose different outputs.',contract=c)
        self.assertIn('STATELESS sampled arithmetic',prompt)
        self.assertNotIn('For constant ON with output lamp',prompt)
        self.assertNotIn('Example of an initially-off output lamp',prompt)
        ordinary=sysml_bundle_prompt('Map ADC to a displayed number.',contract=c)
        self.assertNotIn('STATELESS sampled arithmetic',ordinary)
        # Verify the exact documented full example, not a separately hand-written equivalent.
        template=Path('tools/prompts/contract_controller.prompt.md').read_text(encoding='utf-8')
        block=template.split('<!-- example:numeric_parity -->')[1].split('<!-- /example -->')[0]
        spec,_=json.JSONDecoder().raw_decode(block[block.index('{'):])
        example_contract=helpers.contract([('numberSample','f64'),('levelSample','f64')],
                                           [('number','i32'),('drive','f64')])
        self.execute(spec,example_contract,'''
int main(){ProgramPage1_Init(&ProgramPage1_instance);int number=0;double drive=0;
 ProgramPage1(0,1023,&number,&drive);assert(number==0&&drive==200.0);
 ProgramPage1(512,1023,&number,&drive);assert(number==9&&drive==0.0);
 ProgramPage1(539,1023,&number,&drive);assert(number==10&&drive==200.0);
 ProgramPage1(1023,0,&number,&drive);assert(number==19&&drive==0.0);}
''')

    def test_type_feedback_repairs_expression_without_changing_frozen_interface(self):
        c=helpers.contract([('sample','f64'),('enabled','bool')],[('result','i32')])
        bad=dict(states=[],updates=[],outputs={'result':{'if':'enabled','then':'sample * 200.0 / 1023.0','else':0}})
        good=copy.deepcopy(bad);good['outputs']['result']['then']={'floor':'sample * 200.0 / 1023.0'}
        original=copy.deepcopy(c);replies=iter([bad,good]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',helpers.candidate(next(replies))
        _,_,report=generate_validated_bundle(call,'Enabled sample maps to 0..200, rounded down.',
            contract=c,max_repairs=1,validator=validate_executable_sysml)
        self.assertEqual([a['ok'] for a in report['attempts']],[False,True])
        self.assertIn('interface contract is frozen',prompts[1])
        self.assertIn('both branches must be Integer',prompts[1])
        self.assertTrue(all(report['attempts'][-1]['stages'].values()))
        self.assertEqual(c,original)


if __name__=='__main__':unittest.main()
