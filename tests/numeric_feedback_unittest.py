"""Field-specific planning and expression repair feedback, without paid calls."""
import copy
import json
import unittest

from app.services.interface_planner import plan_interface_contract
from app.services.model_repair import update_locations
from app.services.numeric_contract import add_numeric_checks
from app.services.sysml_assembly import expression, ExpressionSyntaxError
from tests.numeric_contract_unittest import fixture
from tests import sysml_assembly_unittest as helpers


class NumericFeedbackTests(unittest.TestCase):
    execute=helpers.AssemblyTests.execute

    def test_wrong_units_feedback_names_port_actual_value_and_legal_raw_binding(self):
        c,_,text=fixture('parity');bad=copy.deepcopy(c)
        port=next(p for p in bad['interfaces'] if p['name']=='numberAdc')
        port.update(unit='1');port['binding']['scale']=99.0
        with self.assertRaises(ValueError) as caught:add_numeric_checks(bad,text)
        message=str(caught.exception)
        for fragment in ('mapped_parity.input','numberAdc','unit=adc_count','scale=1','offset=0','physical pin'):
            self.assertIn(fragment,message)
        self.assertIn('"unit": "1"',message)
        replies=iter([bad,c]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',json.dumps(next(replies))
        result,report=plan_interface_contract(call,text,{},max_repairs=1,target_board='arduino:avr:mega')
        self.assertEqual([a['ok'] for a in report['attempts']],[False,True])
        self.assertIn(message,prompts[1]);self.assertEqual(len(result['numeric_checks']),1)
        self.assertEqual(result['required_dependencies'],c['required_dependencies'])
        self.assertEqual(result['requirements'],c['requirements'])

    def test_scale_feedback_reports_actual_binding_and_preserves_source(self):
        c,_,text=fixture('parity');c['interfaces'][0]['binding']['scale']=255.0;before=copy.deepcopy(c)
        with self.assertRaises(ValueError) as caught:add_numeric_checks(c,text)
        for fragment in ('raw ADC','scale=1','offset=0','255.0','A3'):
            self.assertIn(fragment,str(caught.exception))
        self.assertEqual(c,before)

    def test_post_assignment_constant_guard_has_precise_advisory_without_rewrite(self):
        for value,guard in [('count - 1','count - 1 < 0'),('count + 1','count + 1 > 99'),
                            ('count - 0.5','count - 0.5 < 0'),('count + delta','count + delta > 99')]:
            spec=dict(updates=[{'if':'enabled','then':[{'assign':'count','value':value},{'if':guard,'then':[]}]}])
            before=copy.deepcopy(spec);found=update_locations(spec)
            self.assertEqual(len(found),1)
            self.assertEqual(found[0]['guard'],'/controller/updates/0/then/1/if')
            self.assertIn('already committed',found[0]['advice']);self.assertEqual(spec,before)
        for guard in ('count < 0','othercount - 1 < 0','count - 10 < 0'):
            self.assertEqual(update_locations(dict(updates=[{'assign':'count','value':'count - 1'},
                                                           {'if':guard,'then':[]} ])),[])

    def test_nested_conditional_feedback_example_executes_with_outer_arithmetic_intact(self):
        malformed=('elapsed + (if enabled then delta else 0)',
                   'elapsed + {"if":"enabled","then":"delta","else":0}')
        for value in malformed:
            with self.assertRaises(ExpressionSyntaxError) as caught:expression(value)
            message=str(caught.exception)
            self.assertIn('WHOLE expression as nested objects',message)
            self.assertIn('Never put a JSON object inside an expression string',message)
        # Execute the exact legal example from the diagnostic, including both branches.
        start=message.index('{"+":')
        legal,_=json.JSONDecoder().raw_decode(message[start:])
        self.execute(dict(states=[],updates=[],outputs={'value':legal}),
            helpers.contract([('elapsed','i32'),('enabled','bool'),('delta','i32')],[('value','i32')]),
            '''int main(){ProgramPage1_Init(&ProgramPage1_instance);int value=0;
ProgramPage1(13,true,7,&value);assert(value==20);
ProgramPage1(13,false,7,&value);assert(value==13);}''')


if __name__=='__main__':unittest.main()
