import copy
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
from unittest.mock import patch

from fastapi.testclient import TestClient
from app.main import app
from app.services.hardware_adapter import pin, validate_hardware_contract
from app.services.interface_planner import check_explicit_requirements
from app.services.sysml_assembly import assemble_sysml


def inventory():
    def port(name,direction,typ,unit,initial,binding):
        return dict(name=name,direction=direction,type=typ,unit=unit,initial=initial,
                    sample_period_ms=10,meaning=name,device='fixture',hardware='Mega circuit',binding=binding)
    return {'version':1,'board':'arduino:avr:mega','control_period_ms':10,'interfaces':[
        port('pressed','input','bool','1',False,{'kind':'digital_input','pin':22,'pullup':True,'active_high':False}),
        port('adc','input','i32','adc_count',0,{'kind':'analog_input','pin':'A15'}),
        port('duty','output','f64','pwm_count',0.0,{'kind':'pwm_output','pin':44})],
        'requirements':[dict(id='R1',text='Mega D22 button gates A15 brightness on D44',assertion_ids=['gate'])],
        'required_dependencies':{'duty':['pressed','adc']}}


class BoardTests(unittest.TestCase):
    def test_board_specific_alias_ranges_and_pwm_conflicts(self):
        validate_hardware_contract(inventory())
        self.assertEqual(pin('A15','arduino:avr:mega'),'A15')
        self.assertEqual(pin('D69','arduino:avr:mega'),'69')
        for value,board in [('A16','arduino:avr:mega'),('D70','arduino:avr:mega'),('A6','arduino:avr:uno'),('D22','arduino:avr:uno')]:
            with self.subTest(value=value,board=board),self.assertRaises(ValueError):pin(value,board)
        alias = inventory()
        alias['interfaces'].append(dict(name='conflict',direction='output',type='bool',unit='1',initial=False,
            sample_period_ms=10,meaning='conflict',device='LED',hardware='D69',binding={'kind':'digital_output','pin':69}))
        with self.assertRaisesRegex(ValueError,'Conflicting'):
            validate_hardware_contract(alias)
        wrong = inventory(); wrong['interfaces'][2]['binding']['pin']=53
        with self.assertRaisesRegex(ValueError,'PWM capable'):
            validate_hardware_contract(wrong)

    def test_planning_preserves_requested_board_and_high_analog_pin(self):
        check_explicit_requirements(inventory(),'Arduino Mega D22 A15 D44',{})
        with self.assertRaisesRegex(ValueError,'Requested board'):
            check_explicit_requirements(inventory(),'D22 A15 D44',{},target_board='arduino:avr:uno')
        wrong = inventory(); wrong['interfaces'][1]['binding']['pin']='A0'
        with self.assertRaisesRegex(ValueError,'A15'):
            check_explicit_requirements(wrong,'Arduino Mega D22 A15 D44',{})

    def test_full_mega_pipeline_auto_board_and_independent_io_trace(self):
        contract = inventory()
        model = assemble_sysml({'states':[],'updates':[], 'outputs':{'duty':{
            'if':'pressed','then':'adc * 255.0 / 1023.0','else':'0.0'}}},contract)
        with tempfile.TemporaryDirectory() as folder, TestClient(app) as client:
            base = Path(folder)
            compiled = client.post('/compile/sysml2',json={'system_sysml2':model,'interface_contract':contract,'out_dir':str(base/'ir')})
            self.assertEqual(compiled.status_code,200,compiled.text)
            generated = client.post('/convert/il-to-c',json={'il_file_path':compiled.json()['files']['il'],'out_c_dir':str(base/'c'),
                'interface_contract':contract,'provenance':compiled.json()['contents']['provenance']})
            self.assertEqual(generated.status_code,200,generated.text)
            contents=generated.json()['contents']
            with patch('app.main._call_llm',side_effect=AssertionError('No LLM in hardware adaptation')):
                adapter=client.post('/convert/c-to-arduino',json={'c_code':contents['ProgramPage1.c'],
                    'provenance':generated.json()['provenance'],
                    'compiled_kernel':{'header':contents['define.h'],'source':contents['ProgramPage1.c']},
                    'interface_contract':contract,'out_path':str(base/'mega.ino')})
            self.assertEqual(adapter.status_code,200,adapter.text)
            with patch('app.main.OUTPUT_DIR',str(base/'outputs')):
                result=client.post('/arduino/compile',json={'sketch_path':adapter.json()['saved_path']})
            self.assertEqual(result.status_code,200,result.text)
            self.assertEqual(result.json()['fqbn'],'arduino:avr:mega')
            self.assertTrue(result.json()['success'],result.text)
            with patch('app.main.subprocess.run',side_effect=AssertionError('Wrong board must fail before compiler or upload')):
                for endpoint,extra in [('/arduino/compile',{}),('/arduino/upload',{'port':'COM_DO_NOT_USE'})]:
                    wrong=client.post(endpoint,json={'sketch_path':adapter.json()['saved_path'],'fqbn':'arduino:avr:uno',**extra})
                    self.assertEqual(wrong.status_code,422,wrong.text)
            (base/'Arduino.h').write_text('''#pragma once
#include <stdint.h>
#define constrain(x,a,b) ((x)<(a)?(a):((x)>(b)?(b):(x)))
enum {LOW=0,HIGH=1,INPUT=0,OUTPUT=1,INPUT_PULLUP=2,A15=69};
static uint32_t now=0; static int rawButton=HIGH, adcValue=0, pwm=-1, modes[70]={};
inline unsigned long millis(){return now;}
inline void pinMode(int p,int m){modes[p]=m;}
inline int digitalRead(int p){if(p!=22)__builtin_abort();return rawButton;}
inline int analogRead(int p){if(p!=A15)__builtin_abort();return adcValue;}
inline void analogWrite(int p,int v){if(p!=44)__builtin_abort();pwm=v;}
''',encoding='utf-8')
            harness='''
#include <cassert>
int main(){setup();assert(modes[22]==INPUT_PULLUP && modes[44]==OUTPUT && pwm==0);
 loop();assert(pwm==0);rawButton=LOW;adcValue=511;now=10;loop();assert(pwm==127);
 adcValue=1023;now=20;loop();assert(pwm==255);rawButton=HIGH;now=30;loop();assert(pwm==0);}
'''
            source=base/'trace.cpp';source.write_text(adapter.json()['arduino_code']+harness,encoding='utf-8')
            binary=base/'trace.exe'
            build=subprocess.run([shutil.which('g++'),'-std=c++17','-I',str(base),str(source),'-o',str(binary)],capture_output=True,text=True)
            self.assertEqual(build.returncode,0,build.stderr)
            run=subprocess.run([str(binary)],capture_output=True,text=True,timeout=10)
            self.assertEqual(run.returncode,0,run.stderr)


if __name__=='__main__':unittest.main()
