import copy
import json
from pathlib import Path
import unittest
from app.services.sysml import generate_validated_bundle, SysMLValidationError
from app.services.behavior_contract import check_contract, validate_contract


def bundle(source):
    return json.dumps({'system_sysml2': source, 'req_test_map': {'requirements_to_testpoints': [
        dict(requirement_id='R1', requirement_text='keep sensors', test_point_id='T1', test_method='trace', stimulus='input', oracle='output')]}})


class RepairTests(unittest.TestCase):
    def test_array_test_map_is_losslessly_wrapped_and_fully_validated(self):
        good=json.loads(bundle('complete source'))
        bad=copy.deepcopy(good);bad['req_test_map']=bad['req_test_map']['requirements_to_testpoints']
        prompts=[];validated=[]
        def call(prompt):
            prompts.append(prompt);return 'fixture',json.dumps(bad)
        def validator(source):
            validated.append(source);return {'ok':True,'issues':[]}
        _,accepted,report=generate_validated_bundle(call,'all original requirements',validator=validator,max_repairs=1)
        self.assertEqual(len(prompts),1)
        self.assertEqual(accepted,good)
        self.assertEqual(validated,['complete source'])
        self.assertEqual(report['attempts'][0]['normalizations'],['req_test_map_array_to_object'])
        self.assertEqual(json.loads(report['attempts'][0]['response']),bad)

    def test_test_map_normalization_never_discards_bad_rows_or_skips_model_validation(self):
        candidate=json.loads(bundle('invalid source'))
        candidate['req_test_map']=candidate['req_test_map']['requirements_to_testpoints']
        def reject(source):return {'ok':False,'issues':[dict(code='Syntax',severity='error',message='invalid source')]}
        with self.assertRaises(SysMLValidationError) as caught:
            generate_validated_bundle(lambda _:('fixture',json.dumps(candidate)),'requirements',validator=reject,max_repairs=0)
        self.assertEqual(caught.exception.attempts[0]['issues'][0]['code'],'Syntax')
        candidate['req_test_map'].append({'requirement_id':'R2'})
        with self.assertRaises(SysMLValidationError) as caught:
            generate_validated_bundle(lambda _:('fixture',json.dumps(candidate)),'requirements',max_repairs=0)
        self.assertIn('Every requirement mapping',caught.exception.attempts[0]['issues'][0]['message'])

    def test_invalid_test_map_type_gets_actionable_nesting_diagnostic(self):
        candidate=json.loads(bundle('complete source'));candidate['req_test_map']='not a map'
        with self.assertRaises(SysMLValidationError) as caught:
            generate_validated_bundle(lambda _:('fixture',json.dumps(candidate)),'requirements',max_repairs=0)
        self.assertIn('req_test_map must be an OBJECT',caught.exception.attempts[0]['issues'][0]['message'])

    def test_adapter_repeated_prototypes_get_local_removal_guidance(self):
        from app.services.arduino_adapter import generate_adapter, adapter_issues
        header = 'void ProgramPage1_Init(struct ProgramPage1_state* self);\nvoid ProgramPage1(bool enabled, bool* led);'
        source = 'void ProgramPage1(bool enabled, bool* led){*led=enabled;}'
        bad = header + '\nvoid setup(){ProgramPage1_Init(&ProgramPage1_instance);}\nvoid loop(){bool led; ProgramPage1(sampled,&led);digitalWrite(13,led);}'
        good = bad.replace(header, '')
        responses = iter([bad, bad, good]); prompts = []
        def call(prompt):
            prompts.append(prompt)
            return 'fixture', next(responses)
        _, sketch = generate_adapter(call, header, source, 'sample sensor and drive LED', {}, max_repairs=2)
        self.assertIn('Line 1:', prompts[1])
        self.assertIn('NO PROGRESS', prompts[2])
        self.assertNotIn('void ProgramPage1(', prompts[0])
        self.assertIn('sample sensor and drive LED', prompts[2])
        self.assertEqual(adapter_issues(good), [])
        self.assertTrue(adapter_issues(bad))  # Rejected prototypes remain rejected.
        self.assertIn(good, sketch)

    def test_syntax_focus_repeated_error_and_complete_revalidation(self):
        prompts, validated = [], []
        syntax = dict(code='Syntax', severity='error', message='expected ;', line=3)
        cascade = dict(code='Linking', severity='error', message='missing fakeCascade')
        def call(prompt):
            prompts.append(prompt)
            return 'fixture', bundle('line1\nline2\nbroken\nline4')
        def validator(source):
            validated.append(source)
            return {'ok': False, 'issues': [cascade, syntax]}
        with self.assertRaises(SysMLValidationError) as caught:
            generate_validated_bundle(call, 'all original requirements', validator=validator, max_repairs=2)
        self.assertEqual(len(validated), 3)
        self.assertIn('   3: broken', prompts[1])
        self.assertNotIn('fakeCascade', prompts[1])
        self.assertIn('NO PROGRESS', prompts[2])
        self.assertTrue(caught.exception.attempts[1]['no_progress'])
        self.assertEqual(len(caught.exception.attempts[2]['issues']), 2)

    def test_missing_contract_is_unverified(self):
        _, _, report = generate_validated_bundle(lambda _: ('fixture', bundle('valid')), 'requirement',
                                                 validator=lambda _: {'ok': True, 'issues': []})
        self.assertEqual(report['coverage']['status'], 'unverified')

    def test_non_string_model_retains_failed_candidates(self):
        with self.assertRaises(SysMLValidationError) as caught:
            generate_validated_bundle(lambda _: ('fixture', '{"system_sysml2":42}'), 'requirements', max_repairs=1)
        self.assertEqual(len(caught.exception.attempts), 2)
        self.assertEqual(caught.exception.attempts[-1]['issues'][0]['code'], 'BUNDLE_FORMAT')

    def test_independent_inventory_blocks_missing_requirement_mapping(self):
        contract = {'version': 1, 'control_period_ms': 100, 'interfaces': [
            {'name': 'sample', 'direction': 'input', 'type': 'f64', 'unit': 'V', 'meaning': 'sensor',
             'sample_period_ms': 100, 'initial': 0.0, 'hardware': 'A0', 'device': 'sensor'}],
            'requirements': [{'id': 'R2', 'text': 'independent requirement', 'assertion_ids': ['external']}]}
        ir = {'inputs': [{'name': 'sample', 'type': 'f64'}], 'outputs': [], 'nodes': [], 'edges': [], 'states': [], 'updates': []}
        candidate = json.loads(bundle('valid'))
        del candidate['system_sysml2']
        candidate['controller'] = {'states': [], 'updates': [], 'outputs': {}}
        with self.assertRaises(SysMLValidationError) as caught:
            generate_validated_bundle(lambda _: ('fixture', json.dumps(candidate)), 'requirements', contract=contract, max_repairs=0,
                                      validator=lambda _: {'ok': True, 'issues': [], 'ir': ir})
        self.assertEqual(caught.exception.attempts[-1]['failure_stage'], 'requirements')
        self.assertEqual(caught.exception.attempts[-1]['issues'][0]['code'], 'REQUIREMENT_MAPPING')

    def test_constant_sensor_argument_is_rejected(self):
        from app.services.arduino_adapter import adapter_issues
        header = 'void ProgramPage1(double sensor, bool* alarm);'
        contract = {'interfaces': [{'name': 'sensor', 'direction': 'input'}]}
        code = 'void setup(){ProgramPage1_Init(&ProgramPage1_instance);} void loop(){bool alarm; ProgramPage1(25.0, &alarm);}'
        self.assertTrue(any('constant' in i for i in adapter_issues(code, header=header, contract=contract)))
        self.assertFalse(adapter_issues(code.replace('25.0', 'sampled'), header=header, contract=contract))


class ContractTests(unittest.TestCase):
    def setUp(self):
        self.contract = json.loads(Path('tools/examples/environment_report.contract.json').read_text(encoding='utf-8'))

    def test_bad_units_initial_and_duplicate_ports_fail(self):
        for field, value in [('unit', ''), ('initial', 'unknown'), ('sample_period_ms', -1)]:
            bad = copy.deepcopy(self.contract); bad['interfaces'][0][field] = value
            with self.subTest(field=field), self.assertRaises(ValueError):
                validate_contract(bad)
        self.contract['interfaces'].append(self.contract['interfaces'][0])
        with self.assertRaises(ValueError):
            validate_contract(self.contract)

    def test_constant_replacement_and_missing_state_are_rejected(self):
        ir = {'inputs': [{'name': p['name'], 'type': p['type']} for p in self.contract['interfaces'] if p['direction'] == 'input'],
              'outputs': [{'name': p['name'], 'type': p['type']} for p in self.contract['interfaces'] if p['direction'] == 'output'],
              'states': [], 'updates': [], 'nodes': [], 'edges': []}
        result = check_contract(ir, self.contract)
        self.assertEqual(result['status'], 'failed')
        self.assertIn('CONSTANT_OR_MISSING_BEHAVIOR', {i['code'] for i in result['issues']})
        self.assertIn('STATE_COVERAGE', {i['code'] for i in result['issues']})


if __name__ == '__main__':
    unittest.main()
