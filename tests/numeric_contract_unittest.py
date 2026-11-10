"""Public numeric assertions: source extraction, real lowering, and fault controls."""
import copy
import json
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import patch

from fastapi.testclient import TestClient
from app.main import app
from app.services.behavior_contract import check_contract, validate_contract
from app.services.interface_planner import add_public_behavior_checks, contract_digest
from app.services.numeric_contract import add_numeric_checks, check_numeric
from app.services.sysml import generate_validated_bundle, SysMLValidationError
from app.services.sysml_assembly import assemble_sysml
from app.services.sysml_compiler import compile_sysml, validate_executable_sysml


def fixture(name):
    base=Path('tools/examples')
    contract=json.loads((base/f'shift595_{name}.contract.json').read_text(encoding='utf-8'))
    controller=json.loads((base/f'shift595_{name}.controller.json').read_text(encoding='utf-8'))
    return contract,controller,contract['requirements'][0]['text']


def candidate(controller,c):
    return json.dumps(dict(controller=controller,req_test_map=dict(requirements_to_testpoints=[
        dict(requirement_id=r['id'],requirement_text=r['text'],test_point_id='T'+r['id'],
             test_method='trace',stimulus='public input sequence',oracle='public output assertion')
        for r in c['requirements']])))


class NumericContractTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.models={}
        for name in ('parity','repeat'):
            c,spec,text=fixture(name);c=add_public_behavior_checks(c,text)
            cls.models[name]=(c,spec,compile_sysml(assemble_sysml(spec,c)),text)

    def test_reference_models_pass_without_claiming_functional_proof(self):
        for name,(c,_,ir,_) in self.models.items():
            with self.subTest(name=name):
                report=check_contract(ir,c)
                self.assertEqual(report['issues'],[])
                self.assertEqual(report['numeric']['status'],'passed')
                self.assertGreater(report['numeric']['checked'],10)
                self.assertEqual(report['functional_status'],'unverified')

    def test_extraction_is_nonmutating_idempotent_and_uses_public_parameters(self):
        for name in ('parity','repeat'):
            c,_,text=fixture(name);before=copy.deepcopy(c)
            extracted=add_numeric_checks(c,text)
            self.assertEqual(c,before)
            self.assertEqual(add_numeric_checks(extracted,text),extracted)
            self.assertEqual(len(extracted['numeric_checks']),1)
        c,_,text=fixture('parity')
        text=text.replace('99','37').replace('50ms','40ms')
        check=add_numeric_checks(c,text)['numeric_checks'][0]
        self.assertEqual((check['display_max'],check['response_ms']),(37,40))
        c['interfaces'][0]['name']='ownLevel'
        self.assertEqual(add_numeric_checks(c,text)['numeric_checks'][0]['even_input'],'ownLevel')

    def test_incomplete_negated_conditional_and_duplicate_source_do_not_invent_checks(self):
        c,_,text=fixture('parity')
        for altered in (text.replace('变化50ms内反映','持续变化'),
                        text.replace('A5映射','不要A5映射'),
                        text.replace('A5映射','如果A5映射'),text+' A5映射floor(ADC*99/1023)得到显示整数0..99。'):
            with self.subTest(text=altered):self.assertNotIn('numeric_checks',add_numeric_checks(c,altered))
        c,_,text=fixture('repeat')
        self.assertNotIn('numeric_checks',add_numeric_checks(c,text.replace('清零优先并重置重复计时','清零')))

    def test_contradictions_and_adapter_side_task_mapping_are_rejected(self):
        c,_,text=fixture('parity')
        for field,value in [('scale',0.1),('offset',1)]:
            bad=copy.deepcopy(c);bad['interfaces'][0]['binding'][field]=value
            with self.subTest(field=field),self.assertRaisesRegex(ValueError,'raw ADC'):
                add_numeric_checks(bad,text)
        with self.assertRaisesRegex(ValueError,'Contradictory'):
            add_numeric_checks(c,text.replace('D2按A3','D2按A4'))
        frozen=add_numeric_checks(c,text);frozen['numeric_checks'][0]['display_max']=98
        with self.assertRaisesRegex(ValueError,'contradict'):add_numeric_checks(frozen,text)
        c,_,text=fixture('repeat');c['interfaces'][0]['binding']['active_high']=True
        with self.assertRaisesRegex(ValueError,'active_high=false'):add_numeric_checks(c,text)

    def test_schema_rejects_malformed_or_unbounded_assertions(self):
        c=copy.deepcopy(self.models['parity'][0])
        for key,value in [('adc_max',True),('response_ms',0),('input','levelA'),('evidence',''),('pwm_max',999),('extra',1)]:
            bad=copy.deepcopy(c);bad['numeric_checks'][0][key]=value
            with self.subTest(key=key),self.assertRaises(ValueError):validate_contract(bad)
        bad=copy.deepcopy(c);bad['numeric_checks']*=2
        with self.assertRaisesRegex(ValueError,'Duplicate'):validate_contract(bad)
        c=copy.deepcopy(self.models['repeat'][0]);c['numeric_checks'][0]['maximum']=1001
        with self.assertRaisesRegex(ValueError,'budget'):validate_contract(c)

    def test_raw_and_inverted_parity_fail_and_feedback_tracks_same_obligation(self):
        c,good,_,text=self.models['parity'];before=copy.deepcopy(c)
        raw=copy.deepcopy(good);raw['calculations'][1]['expression']='numberAdc == 2 * RealFunctions::floor(numberAdc / 2.0)'
        inverted=copy.deepcopy(good);inverted['calculations'][1]['expression']='shown != 2 * RealFunctions::floor(shown / 2.0)'
        replies=iter([raw,inverted,good]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',candidate(next(replies),c)
        _,_,report=generate_validated_bundle(call,text,contract=c,max_repairs=2,validator=validate_executable_sysml)
        attempts=report['attempts']
        self.assertEqual([a['ok'] for a in attempts],[False,False,True])
        self.assertTrue(attempts[1]['repeated_requirement'])
        self.assertNotEqual(attempts[0]['diagnostic_fingerprint'],attempts[1]['diagnostic_fingerprint'])
        self.assertIn('SAME REQUIREMENT STILL FAILING',prompts[2])
        self.assertIn(attempts[0]['rendered_sysml2'],prompts[1])
        self.assertIn(text,prompts[1]);self.assertEqual(c,before)

    def test_counter_wrong_wrap_reset_and_simultaneous_press_have_counterexamples(self):
        c,good,_,_=self.models['repeat']
        cases={
            'wrap':json.loads(json.dumps(good).replace('"value": 99','"value": -1')),
            'reset':copy.deepcopy(good),
            'simultaneous':copy.deepcopy(good),
        }
        cases['reset']['updates'][0]['then'][0]['value']='count'
        cases['simultaneous']['calculations'][0]['expression']['if']='up'
        for name,spec in cases.items():
            with self.subTest(name=name):
                ir=compile_sysml(assemble_sysml(spec,c));report,issues=check_numeric(ir,c)
                self.assertEqual(report['status'],'failed',report)
                self.assertEqual(issues[0]['code'],'CONTRACT_NUMERIC')
                last=report['counterexample']['history'][-1]
                self.assertNotEqual(last['expected'],last['actual'])

    def test_continuous_sampling_and_sensor_ownership_are_checked(self):
        c,good,_,_=self.models['parity']
        wrong=copy.deepcopy(good);wrong['outputs']['brightnessA']['then']='levelB * 255.0 / 1023.0'
        latched=copy.deepcopy(good)
        latched['states']=[dict(name='saved',type='f64',initial=0.0),dict(name='ready',type='bool',initial=False)]
        latched['updates']=[{'if':'not ready','then':[{'assign':'saved','value':'numberAdc'}, {'assign':'ready','value':True}]}]
        latched['calculations'][0]['expression']='RealFunctions::floor(saved * 99.0 / 1023.0)'
        for spec in (wrong,latched):
            ir=compile_sysml(assemble_sysml(spec,c));report,issues=check_numeric(ir,c)
            self.assertEqual(report['status'],'failed',report);self.assertEqual(issues[0]['code'],'CONTRACT_NUMERIC')

    def test_api_preserves_frozen_checks_and_rejects_invalid_behavior_without_llm(self):
        c,spec,_,text=self.models['parity'];bad=copy.deepcopy(spec)
        bad['calculations'][1]['expression']='shown != 2 * RealFunctions::floor(shown / 2.0)'
        source=assemble_sysml(bad,c);ir=compile_sysml(source)
        with tempfile.TemporaryDirectory() as folder,TestClient(app) as client,patch('app.main._call_llm',side_effect=AssertionError('No LLM')):
            prepared=client.post('/generate/interface-contract',json=dict(requirement=text,interface_contract=c,
                board='arduino:avr:mega',out_path=str(Path(folder)/'contract.json')))
            self.assertEqual(prepared.status_code,200,prepared.text)
            result=prepared.json();frozen=result['interface_contract']
            self.assertEqual(frozen['numeric_checks'],c['numeric_checks'])
            browser=json.loads(subprocess.run(['node','-e','let s="";process.stdin.on("data",x=>s+=x);process.stdin.on("end",()=>process.stdout.write(JSON.stringify(JSON.parse(s))));'],
                input=json.dumps(frozen),capture_output=True,text=True,check=True).stdout)
            self.assertEqual(contract_digest(browser),result['contract_sha256'])
            target=Path(folder)/'rejected'
            response=client.post('/compile/sysml2',json=dict(system_sysml2=source,interface_contract=browser,out_dir=str(target)))
            self.assertEqual(response.status_code,422,response.text)
            self.assertIn('CONTRACT_NUMERIC',response.text);self.assertFalse(target.exists())
            response=client.post('/validate/subsystem-ir',json=dict(ir=ir,interface_contract=browser))
            self.assertFalse(response.json()['ok']);self.assertIn('CONTRACT_NUMERIC',response.text)

    def test_budget_exhaustion_retains_failure(self):
        c,good,_,text=self.models['parity'];bad=copy.deepcopy(good)
        bad['calculations'][1]['expression']='shown != 2 * RealFunctions::floor(shown / 2.0)'
        with self.assertRaises(SysMLValidationError) as caught:
            generate_validated_bundle(lambda _:('fixture',candidate(bad,c)),text,contract=c,max_repairs=0,validator=validate_executable_sysml)
        self.assertEqual(len(caught.exception.attempts),1)
        self.assertFalse(caught.exception.attempts[0]['ok'])
        self.assertIn('counterexample',caught.exception.attempts[0]['coverage']['numeric'])


if __name__=='__main__':unittest.main()
