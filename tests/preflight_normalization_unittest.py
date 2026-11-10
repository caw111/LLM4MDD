"""Normalize equivalent syntax only; preserve conflicts, types and branch behavior."""
import copy
import unittest

from app.services.interface_registry import normalize_interface_proposal, assemble_interface_contract
from app.services.hardware_adapter import validate_hardware_contract
from app.services.expression_normalization import normalize_controller
from app.services.sysml_assembly import expression, assemble_sysml
from app.services.sysml_compiler import validate_executable_sysml
import sysml_assembly_unittest as helpers


class PreflightNormalizationTests(unittest.TestCase):
    def test_redundant_ultrasonic_pin_matches_trigger_and_keeps_echo(self):
        for trigger,alias in [('D8','D8'),(8,'D8'),('D8',8)]:
            proposal=dict(interface_requests=[dict(kind=kind,trigger_pin=trigger,pin=alias,echo_pin='D9',
                initial=initial,meaning='Distance measurement') for kind,initial in
                [('ultrasonic_distance',0.0),('ultrasonic_valid',False)]])
            before=copy.deepcopy(proposal)
            normalized,actions=normalize_interface_proposal(proposal,repair_catalog_fields=True)
            self.assertTrue(actions);self.assertEqual(proposal,before)
            for item in normalized['interface_requests']:
                self.assertNotIn('pin',item);self.assertEqual(item['echo_pin'],'D9')
                self.assertEqual(item['trigger_pin'],trigger)
            data,_=assemble_interface_contract(normalized)
            data['requirements']=[dict(id='R1',text='Measure distance',assertion_ids=['measure'])]
            validate_hardware_contract(data)

    def test_real_pin_conflicts_and_missing_trigger_are_still_rejected(self):
        for extra in (dict(pin='D9',trigger_pin='D8'),dict(pin=True,trigger_pin=1),
                      dict(pin='D8'),dict(pin='08',trigger_pin=8)):
            proposal=dict(interface_requests=[dict(kind='ultrasonic_distance',echo_pin='D9',
                            initial=0.0,meaning='distance',**extra)])
            with self.subTest(extra=extra),self.assertRaises(ValueError):
                normalize_interface_proposal(proposal,repair_catalog_fields=True)

    def test_equivalent_nested_conditionals_preserve_precedence_and_original(self):
        original=dict(states=[],updates=[],outputs={
            'result':'base + (if enabled then (if sample >= 0 then sample else -sample) else 7)',
            'nested':{'if':{'>=':['sample',0],'then':{'if':{'==':['base',0],'then':1,'else':2}},'else':3}}})
        before=copy.deepcopy(original)
        fixed,events=normalize_controller(original,expression)
        self.assertEqual(original,before);self.assertTrue(events)
        self.assertEqual(normalize_controller(fixed,expression),(fixed,[]))
        helpers.AssemblyTests.execute(self,fixed,helpers.contract(
            [('enabled','bool'),('sample','i32'),('base','i32')],[('result','i32'),('nested','i32')]),'''
int main(){ProgramPage1_Init(&ProgramPage1_instance);
 for(int enabled=0;enabled<2;++enabled)for(int sample=-3;sample<=3;++sample)for(int base=0;base<3;++base){
 int result=0,nested=0;ProgramPage1(enabled,sample,base,&result,&nested);
 assert(result==base+(enabled?(sample>=0?sample:-sample):7));
 assert(nested==(sample>=0?(base==0?1:2):3));}}
''')

    def test_aliases_do_not_coerce_types_ignore_operands_or_accept_injection(self):
        for text in ['if a then 1', 'if a then 1 else 2; part P {}','if a then millis() else 2']:
            with self.subTest(text=text),self.assertRaises(ValueError):expression(text)
        for text in ['if 1 then 1 else 2','if missing then 1 else 2','if a then 1.0 else 2']:
            source=assemble_sysml(dict(states=[],updates=[],outputs={'out':text}),
                                  helpers.contract([('a','bool')],[('out','i32')]))
            self.assertFalse(validate_executable_sysml(source)['ok'])


if __name__=='__main__':unittest.main()
