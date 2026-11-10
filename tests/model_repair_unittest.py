import copy
import json
import unittest
from app.services.model_repair import obligation_keys, update_locations, periodic_example
from app.services.sysml import generate_validated_bundle
from app.services.sysml_assembly import assemble_sysml
from app.services.sysml_compiler import compile_sysml, validate_executable_sysml
from app.services.behavior_contract import check_contract
from tests.periodic_contract_unittest import contract, controller, TEXT


def failure(c, at=6030):
    return dict(periodic=dict(status='failed',counterexample=dict(check=copy.deepcopy(c['periodic_checks'][0]),
                history=[dict(at_ms=at,actual=0,expected=1000)])))


class ModelRepairTests(unittest.TestCase):
    def test_same_obligation_is_stable_across_counterexamples_but_not_changed_requirements(self):
        c=contract();a=failure(c);b=failure(c,6070)
        self.assertEqual(obligation_keys(a,c),obligation_keys(b,c))
        b['periodic']['counterexample']['check']['period_ms']=1100
        self.assertEqual(obligation_keys(b,c),[])
        self.assertEqual(obligation_keys({'periodic':{'status':'passed'}},c),[])
        self.assertEqual(obligation_keys(a,None),[])

    def test_local_update_advice_preserves_source_and_distinguishes_correct_guard(self):
        spec=dict(updates=[{'if':'running','then':[{'assign':'remaining','value':'remaining - delta'},
                            {'if':'remaining - delta <= 0','then':[]}]}])
        before=copy.deepcopy(spec);locations=update_locations(spec)
        self.assertEqual(locations[0]['assignment'],'/controller/updates/0/then/0')
        self.assertEqual(locations[0]['guard'],'/controller/updates/0/then/1/if')
        self.assertEqual(spec,before)
        spec['updates'][0]['then'][1]['if']='remaining <= 0'
        self.assertEqual(update_locations(spec),[])
        spec['updates'][0]['then'][1]['if']='another_remaining - delta <= 0'
        self.assertEqual(update_locations(spec),[])

    def test_typed_fragment_preserves_extra_sensor_behavior_and_uses_frozen_parameters(self):
        c=contract()
        c['interfaces'] += [dict(name='sample',direction='input',type='f64',unit='adc_count',meaning='real sensor',
            sample_period_ms=10,initial=0.0,hardware='A0',device='sensor',binding=dict(kind='analog_input',pin='A0')),
            dict(name='measured',direction='output',type='f64',unit='adc_count',meaning='sensor copy',sample_period_ms=10,
                 initial=0.0,hardware='test channel',device='observer')]
        c['required_dependencies']['measured']=['sample']
        c['periodic_checks'][0].update(period_ms=1370,pulse_ms=130,active_value=750)
        spec=dict(states=[],updates=[],outputs={'frequency':'0','measured':'sample'},calculations=[dict(name='repairPulse0',type='i32',expression='0')])
        before=copy.deepcopy(spec)
        example=periodic_example(failure(c),c,spec)
        self.assertEqual(spec,before)
        fragment=example['controller_fragment']
        self.assertEqual(fragment['components'][-1]['name'],'repairPulse1')
        self.assertEqual(fragment['components'][-1]['period_ms'],1370)
        self.assertEqual(fragment['outputs']['frequency']['then'],750)
        # Integrate only the fragment; retain independently required sampled output.
        spec['components']=fragment['components'];spec['outputs'].update(fragment['outputs'])
        ir=compile_sysml(assemble_sysml(spec,c))
        checked=check_contract(ir,c)
        self.assertEqual(checked['issues'],[])
        from app.services.ir_execution import IRExecution
        self.assertEqual(IRExecution(ir).step(dict(deltaMs=0,pressed=False,sample=17.5))['measured'],17.5)

    def test_actual_feedback_revalidates_complete_model_after_same_requirement_moves(self):
        c=contract();a=controller();b=controller()
        # A late pulse fails on its rising edge; an overlong pulse fails later
        # on its falling edge. Two different late periods fail at the SAME
        # first sample, which correctly also triggers exact no-progress.
        a['components'][1]['period_ms']=1100;b['components'][1]['pulse_ms']=200
        example=periodic_example(failure(c),c,b)
        good=dict(states=[],updates=[],**example['controller_fragment'])
        mapping={'requirements_to_testpoints':[dict(requirement_id='R1',requirement_text=TEXT,test_point_id='T1',test_method='trace',stimulus='inputs',oracle='public timing')]}
        replies=iter([a,b,good]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',json.dumps(dict(controller=next(replies),req_test_map=mapping))
        _,_,report=generate_validated_bundle(call,TEXT,contract=c,max_repairs=2,validator=validate_executable_sysml)
        self.assertEqual([x['ok'] for x in report['attempts']],[False,False,True])
        self.assertTrue(report['attempts'][1]['repeated_requirement'])
        self.assertFalse(report['attempts'][1]['no_progress'])
        self.assertIn('SAME REQUIREMENT STILL FAILING',prompts[2])
        self.assertIn('"period_ms": 1000',prompts[2])
        self.assertIn('partial controller fragment',prompts[2])
        self.assertTrue(all(report['attempts'][-1]['stages'].values()))
        self.assertEqual(report['coverage']['periodic']['status'],'passed')
        self.assertEqual(report['coverage']['functional_status'],'unverified')


if __name__=='__main__':unittest.main()
