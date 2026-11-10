import copy
import json
from pathlib import Path
import unittest

from app.services.behavior_contract import validate_sysml_contract
from app.services.hardware_adapter import validate_hardware_contract
from app.services.sysml import sysml_bundle_prompt, generate_validated_bundle, SysMLValidationError
from app.services.sysml_assembly import assemble_sysml
from app.services.sysml_compiler import validate_executable_sysml
import sysml_assembly_unittest as assembly_tests


FIXTURE = Path('tools/examples/sysml_prompt_example.json')


class PromptConstraintsTests(unittest.TestCase):
    def test_default_prompt_contains_profile_full_json_and_matching_sysml(self):
        fixture=json.loads(FIXTURE.read_text(encoding='utf-8'))
        validate_hardware_contract(fixture['contract']); validate_sysml_contract(fixture['contract'])
        prompt=sysml_bundle_prompt('Target-specific requirement',contract=fixture['contract'])
        self.assertIn('Complete constraints for THIS executable SysML',prompt)
        for rule in ('always produces Real','immediately','ONE actual driver','NOT a clock','<=32','req_test_map'):
            self.assertIn(rule,prompt)
        self.assertIn(assemble_sysml(fixture['controller'],fixture['contract']),prompt)
        tail=prompt.split('Complete illustrative response example',1)[1]
        example=json.JSONDecoder().raw_decode(tail[tail.index('{'):])[0]
        self.assertEqual(set(example),{'controller','req_test_map'})
        self.assertEqual(example['controller'],fixture['controller'])
        self.assertEqual(example['req_test_map'],fixture['req_test_map'])
        self.assertIn('Target-specific requirement',prompt)
        legacy=sysml_bundle_prompt('Target-specific requirement',contract=fixture['contract'],complete_constraints=False)
        self.assertNotIn('Complete constraints for THIS',legacy)
        self.assertTrue(prompt.startswith(legacy))

    def test_uncontracted_mode_example_uses_its_actual_response_schema(self):
        prompt=sysml_bundle_prompt('Target-specific requirement')
        tail=prompt.split('Complete illustrative response example',1)[1]
        example=json.JSONDecoder().raw_decode(tail[tail.index('{'):])[0]
        self.assertEqual(set(example),{'system_sysml2','req_test_map'})
        self.assertIn('part system : Controller',example['system_sysml2'])

    def test_new_constraints_and_example_remain_in_each_repair_prompt(self):
        fixture=json.loads(FIXTURE.read_text(encoding='utf-8'))
        prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture','{}'
        with self.assertRaises(SysMLValidationError):
            generate_validated_bundle(call,sysml_bundle_prompt('Fixture task',contract=fixture['contract']),
                contract=fixture['contract'],max_repairs=1)
        self.assertEqual(len(prompts),2)
        for prompt in prompts:
            self.assertIn('Complete constraints for THIS executable SysML',prompt)
            self.assertIn('Complete illustrative response example',prompt)
            self.assertIn('For the actual task use ONLY its frozen port names',prompt)

    def test_timing_example_preserves_component_declarations_and_has_matching_sysml(self):
        contract=json.loads(Path('tools/examples/periodic_tone.contract.json').read_text(encoding='utf-8'))
        for paused in (False,True):
            data=copy.deepcopy(contract)
            if paused:
                data['interfaces'].append(dict(name='pressed',direction='input',type='bool',unit='1',meaning='pressed',
                    initial=False,sample_period_ms=10,device='button',hardware='D2',binding=dict(kind='digital_input',pin=2)))
            prompt=sysml_bundle_prompt('Pause/resume periodic sound' if paused else 'Periodic short beeps',contract=data)
            tail=prompt.split('Complete illustrative response example',1)[1]
            example=json.JSONDecoder().raw_decode(tail[tail.index('{'):])[0]
            self.assertEqual(len(example['controller']['components']),2 if paused else 1)
            self.assertIn('component_beep_value',json.dumps(example['controller']['outputs']))
            self.assertNotIn('exampleCount',json.dumps(example['controller']))
            source=prompt.split('Assembler-produced SysML for that same example (reference only; your response stays controller JSON):\n',1)[1].split('\nReturn only')[0]
            self.assertTrue(validate_executable_sysml(source)['ok'])

    def test_complete_example_passes_reference_ir_c_and_independent_trace(self):
        fixture=json.loads(FIXTURE.read_text(encoding='utf-8'))
        assembly_tests.AssemblyTests.execute(self,fixture['controller'],fixture['contract'],'''
int main(){ProgramPage1_Init(&ProgramPage1_instance);
 bool ready=false;int doubled=0;double ratio=0;
 ProgramPage1(true,1023,&ready,&doubled,&ratio);
 assert(!ready && doubled==0 && ratio==0.0);
 for(int step=1;step<=16;++step){
  ProgramPage1(false,1023,&ready,&doubled,&ratio);
  assert(ready && doubled==2*(step%8) && ratio==1.0);
 }
 ProgramPage1(false,511,&ready,&doubled,&ratio);
 assert(!ready && doubled==2 && ratio>0.499 && ratio<0.5);
 ProgramPage1(false,512,&ready,&doubled,&ratio);
 assert(ready && doubled==4);
 ProgramPage1(true,0,&ready,&doubled,&ratio);
 assert(!ready && doubled==0 && ratio==0.0);
}''')

    def test_invalid_examples_still_fail_instead_of_relaxing_validation(self):
        fixture=json.loads(FIXTURE.read_text(encoding='utf-8'))
        bad=copy.deepcopy(fixture['controller'])
        bad['updates']=[dict(assign='exampleReady',value=True)]
        with self.assertRaisesRegex(ValueError,'initialized internal state'):
            assemble_sysml(bad,fixture['contract'])
        bad=copy.deepcopy(fixture['controller']);bad['outputs']['exampleRatio']=True
        result=validate_executable_sysml(assemble_sysml(bad,fixture['contract']))
        self.assertFalse(result['ok'])


if __name__=='__main__':unittest.main()
