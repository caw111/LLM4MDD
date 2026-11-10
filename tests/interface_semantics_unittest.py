import copy
import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
from unittest.mock import patch

from fastapi.testclient import TestClient
from app.main import app
from app.services.behavior_contract import validate_contract
from app.services.hardware_adapter import generate_hardware_adapter
from app.services.interface_planner import plan_interface_contract, contract_digest
from app.services.interface_semantics import freeze_model_semantics, model_boundary_prompt
from app.services.sysml import generate_validated_bundle, SysMLValidationError
from app.services.sysml_compiler import validate_executable_sysml


def inventory(input_high=False, output_high=True):
    return dict(version=1, control_period_ms=10, interfaces=[
        dict(name=name,direction=direction,type='bool',unit='1',meaning='pressed flag' if direction=='input' else 'lamp command',
             sample_period_ms=10,initial=False,device='button' if direction=='input' else 'LED',hardware='electrical wiring',
             binding=dict(kind='digital_'+direction,pin=pin,active_high=high))
        for name,direction,pin,high in [('pressed','input',2,input_high),('lamp','output',13,output_high)]],
        requirements=[dict(id='R1',text='button controls lamp',assertion_ids=['follow_button'])])


class InterfaceSemanticsTests(unittest.TestCase):
    def test_duplicate_output_feedback_targets_declaration_and_preserves_interface(self):
        c=inventory()
        mapping={'requirements_to_testpoints':[dict(requirement_id='R1',requirement_text='button controls lamp',
            test_point_id='T1',test_method='trace',stimulus='button',oracle='lamp') ]}
        bad=dict(states=[dict(name='lamp',type='bool',initial=False)],updates=[],outputs={'lamp':'pressed'})
        good=dict(states=[],updates=[],outputs={'lamp':'pressed'})
        replies=iter([bad,bad,good]);prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture',json.dumps(dict(controller=next(replies),req_test_map=mapping))
        _,bundle,report=generate_validated_bundle(call,'button controls lamp',contract=c,max_repairs=2,validator=validate_executable_sysml)
        self.assertEqual([a['ok'] for a in report['attempts']],[False,False,True])
        self.assertTrue(report['attempts'][1]['no_progress'])
        self.assertIn('controller declaration, not a missing JSON wrapper',prompts[1])
        self.assertIn('rename ONLY that internal declaration',prompts[2])
        self.assertIn('NO PROGRESS',prompts[2])
        self.assertEqual(bundle['controller'],good)

    def test_frozen_mapping_is_derived_without_rewriting_requirements(self):
        original=inventory();before=copy.deepcopy(original)
        frozen=freeze_model_semantics(original)
        self.assertEqual(original,before)
        self.assertEqual(frozen['requirements'],original['requirements'])
        self.assertEqual(frozen['interfaces'][0]['model_semantics'],dict(kind='digital_boolean',true_level='LOW',false_level='HIGH'))
        self.assertEqual(freeze_model_semantics(frozen),frozen)
        for change in ('binding','semantics'):
            bad=copy.deepcopy(frozen)
            if change=='binding':bad['interfaces'][0]['binding']['active_high']=True
            else:bad['interfaces'][0]['model_semantics']['true_level']='HIGH'
            with self.assertRaisesRegex(ValueError,'contradicts'):validate_contract(bad)
            with self.assertRaisesRegex(ValueError,'contradicts'):freeze_model_semantics(bad)

    def test_planning_and_supplied_api_freeze_same_mapping_and_reject_conflicts(self):
        c=inventory();planned,_=plan_interface_contract(lambda _:('fixture',json.dumps(c)), 'D2 button D13 LED', {}, max_repairs=0)
        with tempfile.TemporaryDirectory() as folder,TestClient(app) as client,patch('app.main._call_llm',side_effect=AssertionError('No LLM')):
            path=Path(folder)/'contract.json'
            response=client.post('/generate/interface-contract',json=dict(requirement='D2 button D13 LED',interface_contract=c,out_path=str(path)))
            self.assertEqual(response.status_code,200,response.text)
            supplied=response.json()['interface_contract']
            self.assertEqual(supplied,planned)
            self.assertEqual(response.json()['contract_sha256'],contract_digest(supplied))
            # Real browser JSON serialization must preserve this frozen mapping.
            node=subprocess.run([shutil.which('node'),'-e','let s="";process.stdin.on("data",x=>s+=x);process.stdin.on("end",()=>process.stdout.write(JSON.stringify(JSON.parse(s))));'],
                input=json.dumps(supplied),capture_output=True,text=True,check=True)
            self.assertEqual(contract_digest(json.loads(node.stdout)),contract_digest(supplied))
            bad=copy.deepcopy(supplied);bad['interfaces'][0]['binding']['active_high']=True
            target=Path(folder)/'invalid.json'
            response=client.post('/generate/interface-contract',json=dict(requirement='D2 button D13 LED',interface_contract=bad,out_path=str(target)))
            self.assertEqual(response.status_code,422);self.assertFalse(target.exists())

    def test_boundary_prompt_survives_each_repair_even_with_ambiguous_legacy_prose(self):
        c=inventory();c['interfaces'][0]['hardware']='按下时读到false'
        # Legacy contracts lacking the derived field still get exact binding semantics.
        prompts=[]
        def call(prompt):prompts.append(prompt);return 'fixture','{}'
        with self.assertRaises(SysMLValidationError):generate_validated_bundle(call,'D2 button D13 LED',contract=c,max_repairs=1)
        for prompt in prompts:
            self.assertIn('"true_level": "LOW"',prompt)
            self.assertIn('"initial_model_value": false',prompt)
            self.assertIn('do not reinterpret those levels as model Boolean values',prompt)
        self.assertEqual(model_boundary_prompt(c),model_boundary_prompt(freeze_model_semantics(c)))

    def test_actual_adapter_truth_tables_and_initial_output_for_both_polarities(self):
        header='''struct ProgramPage1_state {}; extern struct ProgramPage1_state ProgramPage1_instance;
void ProgramPage1_Init(struct ProgramPage1_state* self);
void ProgramPage1(bool pressed, bool* lamp);'''
        kernel='''struct ProgramPage1_state ProgramPage1_instance;
static bool seen=false;
void ProgramPage1_Init(struct ProgramPage1_state*){}
void ProgramPage1(bool pressed,bool* lamp){seen=pressed;*lamp=pressed;}
'''
        stub='''#pragma once
#include <stdint.h>
enum{LOW=0,HIGH=1,INPUT=0,OUTPUT=1,INPUT_PULLUP=2};
static uint32_t now=0;static int raw=HIGH,driven=-1;
static unsigned long millis(){return now;}static void pinMode(int,int){}
static int digitalRead(int pin){if(pin!=2)__builtin_abort();return raw;}
static void digitalWrite(int pin,int value){if(pin!=13)__builtin_abort();driven=value;}
'''
        for input_high in (False,True):
            for output_high in (False,True):
                c=freeze_model_semantics(inventory(input_high,output_high))
                code=generate_hardware_adapter(header,c)
                # Independent raw-level expectations, not calculated by production helpers.
                input_true=1 if input_high else 0;output_true=1 if output_high else 0
                harness=f'''#include <cassert>
int main(){{setup();assert(driven=={1-output_true});
 for(int level=0;level<2;++level){{raw=level;now+=10;loop();
 bool expected=(level=={input_true});assert(seen==expected);
 assert(driven==(expected?{output_true}:{1-output_true}));}}}}
'''
                with self.subTest(input_high=input_high,output_high=output_high),tempfile.TemporaryDirectory() as tmp:
                    root=Path(tmp);(root/'Arduino.h').write_text(stub)
                    source=root/'test.cpp';source.write_text('#include "Arduino.h"\n'+header+'\n'+kernel+code+harness)
                    executable=root/'test.exe'
                    built=subprocess.run([shutil.which('g++'),'-std=c++17','-I',str(root),str(source),'-o',str(executable)],capture_output=True,text=True)
                    self.assertEqual(built.returncode,0,built.stderr)
                    run=subprocess.run([str(executable)],capture_output=True,text=True,timeout=10)
                    self.assertEqual(run.returncode,0,run.stderr)


if __name__=='__main__':unittest.main()
