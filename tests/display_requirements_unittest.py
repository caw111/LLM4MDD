"""Source-backed display formatting obligations are checked before freezing."""
import copy
import json
import unittest
from app.services.interface_planner import check_explicit_requirements, plan_interface_contract
from tests.hardware_shift595_unittest import inventory as shift_inventory
from tests.hardware_display_unittest import inventory as tm_inventory


class DisplayRequirementTests(unittest.TestCase):
    def test_explicit_leading_zero_obligation_checks_both_numeric_bindings(self):
        for factory in (shift_inventory,tm_inventory):
            for text in ('数码管保留前导零。','Preserve leading zeros.','Leading zeros must be preserved.',
                         'Leading zeros kept.'):
                with self.subTest(factory=factory.__name__,text=text):
                    c=factory();c['interfaces'][1]['binding']['leading_zero']=False
                    before=copy.deepcopy(c)
                    with self.assertRaisesRegex(ValueError,r'binding.leading_zero must be true'):
                        check_explicit_requirements(c,text,{})
                    self.assertEqual(c,before)
                    c['interfaces'][1]['binding']['leading_zero']=True
                    check_explicit_requirements(c,text,{})

    def test_absent_negated_or_ambiguous_multi_display_text_does_not_invent_obligation(self):
        c=shift_inventory();c['interfaces'][1]['binding']['leading_zero']=False
        for text in ('Display the value.','不要保留前导零。','Do not preserve leading zeros.'):
            check_explicit_requirements(c,text,{})
        other=copy.deepcopy(c['interfaces'][1]);other['name']='secondDisplay'
        c['interfaces'].append(other)
        # This only checks source matching, not hardware validity of this probe.
        check_explicit_requirements(c,'Only the first display must preserve leading zeros.',{})

    def test_planning_feedback_repairs_formatting_without_changing_model_interfaces(self):
        good=shift_inventory();bad=copy.deepcopy(good);bad['interfaces'][1]['binding']['leading_zero']=False
        replies=iter([bad,good]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',json.dumps(next(replies))
        result,report=plan_interface_contract(call,'数码管保留前导零。',{},max_repairs=1,target_board='arduino:avr:mega')
        self.assertEqual([a['ok'] for a in report['attempts']],[False,True])
        self.assertIn('displayNumber.binding.leading_zero',prompts[1])
        self.assertEqual(result['required_dependencies'],good['required_dependencies'])
        self.assertTrue(result['interfaces'][1]['binding']['leading_zero'])

if __name__=='__main__':unittest.main()
