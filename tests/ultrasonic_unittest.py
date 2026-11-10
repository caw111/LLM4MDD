"""Frozen sensor pair, blocking-time accounting, and independent electrical tests."""
import copy
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

from fastapi.testclient import TestClient
from app.main import app
from app.services.hardware_adapter import validate_hardware_contract, generate_hardware_adapter
from app.services.behavior_contract import validate_contract
from app.services.interface_planner import check_explicit_requirements, contract_digest
from app.services.interface_semantics import model_boundary_prompt
from app.services.sysml_assembly import assemble_sysml
from app.services.sysml_compiler import validate_executable_sysml
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
from run_contract_development import behavior, lower_verified, adapt_verified, INTERPRETATIONS
from run_controlled_experiment import compile_code
from run_formal_experiment import CLI


def inventory(alarm=True):
    return json.loads(Path('tools/examples/ultrasonic_'+('alarm' if alarm else 'distance')+'.contract.json').read_text(encoding='utf-8'))


DIRECT='''#include <Arduino.h>
#include <TM1637Display.h>
TM1637Display display(4,5);unsigned long last=0;bool started=false;
void setup(){pinMode(8,OUTPUT);digitalWrite(8,LOW);pinMode(9,INPUT);pinMode(13,OUTPUT);digitalWrite(13,LOW);display.setBrightness(7,true);display.clear();}
void loop(){unsigned long now=millis();if(started && (unsigned long)(now-last)<100)return;started=true;last=now;
digitalWrite(8,LOW);delayMicroseconds(2);digitalWrite(8,HIGH);delayMicroseconds(10);digitalWrite(8,LOW);
unsigned long width=pulseIn(9,HIGH,25000UL);
bool valid=width>=116UL && width<=23200UL;double cm=width/58.0;
if(valid)display.showNumberDec((int)cm,false);else display.clear();
digitalWrite(13,valid && cm<10.0?HIGH:LOW);}
'''
POLLING='''unsigned long begin=micros(),rise=0,width=0;int level;
do{level=digitalRead(9);}while(level==LOW && (unsigned long)(micros()-begin)<25000UL);
if(level==HIGH){rise=micros();while(digitalRead(9)==HIGH && (unsigned long)(micros()-begin)<25000UL){}
if((unsigned long)(micros()-begin)<25000UL)width=(unsigned long)(micros()-rise);}
'''


class UltrasonicTests(unittest.TestCase):
    def test_pair_types_units_timeouts_alias_conflicts_and_wiring(self):
        c=inventory();validate_hardware_contract(c)
        text=INTERPRETATIONS['medium_022'];check_explicit_requirements(c,text,{})
        self.assertIn('distanceValid',model_boundary_prompt(c))
        for index,field,value in [(0,'unit','mm'),(1,'initial',True),(1,'sample_period_ms',200),(0,'type','i32')]:
            bad=copy.deepcopy(c);bad['interfaces'][index][field]=value
            with self.subTest(field=field),self.assertRaises(ValueError):validate_hardware_contract(bad)
        for field,value in [('timeout_us',True),('timeout_us',1000),('timeout_us',1000000),
                            ('echo_pin',8),('invalid_policy','zero'),('sensor','HC-SR05')]:
            bad=copy.deepcopy(c);bad['interfaces'][0]['binding'][field]=value
            with self.subTest(field=field),self.assertRaises(ValueError):validate_hardware_contract(bad)
        bad=copy.deepcopy(c);bad['interfaces'].pop(1)
        bad['required_dependencies']={k:['distanceCm'] for k in bad['required_dependencies']}
        with self.assertRaisesRegex(ValueError,'requires one'):validate_hardware_contract(bad)
        with self.assertRaisesRegex(ValueError,'requires one'):validate_contract(bad)
        bad=copy.deepcopy(c);bad['interfaces'][-1]['binding']['pin']=9
        with self.assertRaisesRegex(ValueError,'Conflicting'):validate_hardware_contract(bad)
        bad=copy.deepcopy(c)
        for p in bad['interfaces'][:2]:p['binding'].update(trigger_pin=9,echo_pin=8)
        with self.assertRaisesRegex(ValueError,'TRIG wiring'):check_explicit_requirements(bad,text,{})
        circuit={'parts':[{'id':'u','type':'wokwi-arduino-uno'},{'id':'s','type':'wokwi-hc-sr04'}],
                 'connections':[['s:TRIG','u:8'],['s:ECHO','u:9']]}
        with self.assertRaisesRegex(ValueError,'TRIG/ECHO wiring'):
            check_explicit_requirements(bad,json.dumps(circuit),{})
        # Alias sharing applies only to the paired channels of the same sensor.
        for p in c['interfaces'][:2]:p['binding'].update(trigger_pin='A0',echo_pin='A1')
        c['interfaces'][1]['binding'].update(trigger_pin=14,echo_pin=15)
        validate_hardware_contract(c)
        c['interfaces'][-1]['binding']['pin']='D14'
        with self.assertRaisesRegex(ValueError,'Conflicting'):validate_hardware_contract(c)

    def test_explicit_timeout_initial_blank_and_impossible_deadline_are_checked(self):
        c=inventory();text=INTERPRETATIONS['medium_022']
        for p in c['interfaces'][:2]:p['binding']['timeout_us']=30000
        with self.assertRaisesRegex(ValueError,'timeout_us must match'):check_explicit_requirements(c,text,{})
        c=inventory();c['interfaces'][2]['initial']=0
        with self.assertRaisesRegex(ValueError,'initial must request blank'):check_explicit_requirements(c,text,{})
        c=inventory()
        with self.assertRaisesRegex(ValueError,'blocking acquisition'):check_explicit_requirements(c,text+' 响应时间不超过20ms。',{})
        check_explicit_requirements(c,text,{})
        for p in c['interfaces'][:2]:p['sample_period_ms']=500
        with self.assertRaisesRegex(ValueError,'sample_period_ms plus bounded acquisition'):check_explicit_requirements(c,text,{})

    def test_oracle_pulse_width_wait_and_timeout_have_independent_time_assertions(self):
        code='''#include <Arduino.h>
#include <cassert>
void trigger(){digitalWrite(8,HIGH);delayMicroseconds(10);digitalWrite(8,LOW);}
int main(){oracle::task=22;pinMode(8,OUTPUT);pinMode(9,INPUT);oracle::now=1000;
trigger();auto before=micros();assert(pulseIn(9,HIGH,25000)==1740);assert(micros()-before==1940);
trigger();delayMicroseconds(200);before=micros();assert(pulseIn(9,HIGH,25000)==0);assert(micros()-before==25000);
trigger();before=micros();assert(pulseIn(9,HIGH,1000)==0);assert(micros()-before==1000);
oracle::now=4500;oracle::sub_ms=0;trigger();before=micros();assert(pulseIn(9,HIGH,25000)==0);assert(micros()-before==25000);
oracle::now=5500;oracle::sub_ms=0;trigger();before=micros();assert(pulseIn(9,HIGH,25000)==0);assert(micros()-before==25000);}
'''
        with tempfile.TemporaryDirectory() as folder:
            base=Path(folder);shutil.copyfile('experiment/development/oracles/gpio_arduino.h',base/'Arduino.h')
            (base/'test.cpp').write_text(code,encoding='utf-8');binary=base/'test.exe'
            build=subprocess.run([shutil.which('g++'),'-std=c++17','-I',str(base),str(base/'test.cpp'),'-o',str(binary)],capture_output=True,text=True)
            self.assertEqual(build.returncode,0,build.stderr)
            result=subprocess.run([str(binary)],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)

    def test_two_models_browser_contract_and_verified_backend_pass_both_boards(self):
        for alarm,task in [(False,7),(True,22)]:
            with self.subTest(alarm=alarm),tempfile.TemporaryDirectory() as folder,TestClient(app) as client:
                base=Path(folder);c=inventory(alarm);text=INTERPRETATIONS['medium_022' if alarm else 'medium_007']
                with patch('app.main._call_llm',side_effect=AssertionError('No LLM')):
                    response=client.post('/generate/interface-contract',json=dict(requirement=text,interface_contract=c,out_path=str(base/'contract.json')))
                    self.assertEqual(response.status_code,200,response.text)
                browser=json.loads(subprocess.run([shutil.which('node'),'-e',
                    'let s="";process.stdin.on("data",x=>s+=x);process.stdin.on("end",()=>process.stdout.write(JSON.stringify(JSON.parse(s))));'],
                    input=json.dumps(response.json()),text=True,capture_output=True,check=True).stdout)
                c=browser['interface_contract'];self.assertEqual(contract_digest(c),browser['contract_sha256'])
                spec=json.loads(Path('tools/examples/ultrasonic_'+('alarm' if alarm else 'distance')+'.controller.json').read_text(encoding='utf-8'))
                for board in ('arduino:avr:uno','arduino:avr:mega'):
                    c['board']=board;source=assemble_sysml(spec,c);checked=validate_executable_sysml(source)
                    self.assertTrue(checked['ok'],checked)
                    target=base/board.split(':')[-1]
                    generated=lower_verified(source,c,checked['ir'],target/'lowered')
                    code=adapt_verified(generated,c,target/'lowered')
                    result,log=compile_code(target/'compiled',code,CLI,board)
                    self.assertTrue(result['compile_ok'],log)
                    measured=behavior(target/'compiled',task,board)
                    self.assertEqual(measured['functional_status'],'passed',
                        (target/'compiled/host_oracle/trace.log').read_text() if (target/'compiled/host_oracle/trace.log').exists() else measured)

    def test_independent_oracle_accepts_pulse_and_polling_and_rejects_behavioral_mutants(self):
        cases=[('pulse',DIRECT,'passed'),('polling',DIRECT.replace('unsigned long width=pulseIn(9,HIGH,25000UL);',POLLING),'passed'),
            ('blank_by_off',DIRECT.replace('if(valid)display.showNumberDec((int)cm,false);else display.clear();',
                'display.setBrightness(7,valid);display.showNumberDec(valid?(int)cm:0,false);'),'passed'),
            ('constant',DIRECT.replace('double cm=width/58.0','double cm=30.0'),'failed'),
            ('scale',DIRECT.replace('width/58.0','width/29.0'),'failed'),
            ('threshold',DIRECT.replace('cm<10.0','cm<=10.0'),'failed'),
            ('rounded_threshold',DIRECT.replace('cm<10.0','round(cm)<10.0'),'failed'),
            ('timeout_zero',DIRECT.replace('bool valid=width>=116UL && width<=23200UL','bool valid=width<=23200UL'),'failed'),
            ('short_echo',DIRECT.replace('width>=116UL','width>0'),'failed'),
            ('missing_timeout',DIRECT.replace('pulseIn(9,HIGH,25000UL)','pulseIn(9,HIGH)'),'failed'),
            ('short_trigger',DIRECT.replace('delayMicroseconds(10)','delayMicroseconds(5)'),'failed'),
            ('slow_sampling',DIRECT.replace('(now-last)<100','(now-last)<400'),'failed')]
        with tempfile.TemporaryDirectory() as folder:
            for index,(name,code,status) in enumerate(cases):
                with self.subTest(name=name):
                    target=Path(folder)/str(index);cpp=target/'build/sketch/sketch.ino.cpp';cpp.parent.mkdir(parents=True)
                    cpp.write_text(code,encoding='utf-8');result=behavior(target,22)
                    self.assertEqual(result['functional_status'],status,
                        (target/'host_oracle/trace.log').read_text() if (target/'host_oracle/trace.log').exists() else result)

    def test_elapsed_includes_blocking_reads_and_invalid_samples_do_not_stop_model(self):
        c=inventory();c['interfaces'].append(dict(name='elapsed',direction='input',type='i32',unit='ms',initial=0,
            sample_period_ms=10,meaning='actual delta',device='clock',hardware='millis',binding={'kind':'elapsed_ms'}))
        header='void ProgramPage1(double distanceCm, bool distanceValid, int elapsed, int* distanceDisplay, bool* nearby);'
        code=generate_hardware_adapter(header,c)
        arduino='''#pragma once
#include <stdint.h>
#include <math.h>
enum{INPUT=0,OUTPUT=1,LOW=0,HIGH=1};
static uint32_t now=0;static unsigned long width=0;static int reads=0;
inline unsigned long millis(){return now;}inline void pinMode(int,int){}inline void digitalWrite(int,int){}
inline void delayMicroseconds(unsigned int){}
inline unsigned long pulseIn(uint8_t pin,uint8_t level,unsigned long timeout){++reads;now+=width?(width+200+999)/1000:(timeout+999)/1000;return width;}
'''
        kernel='''#include <Arduino.h>
#include <cassert>
int ProgramPage1_instance=0,calls=0,observedDelta=0;double observedDistance=0;bool observedValid=false;
uint32_t previous=0;
void ProgramPage1_Init(int*){}
void ProgramPage1(double cm,bool valid,int elapsed,int* display,bool* led){
assert(elapsed==(calls?(int32_t)(now-previous):0));previous=now;++calls;
observedDelta=elapsed;observedDistance=cm;observedValid=valid;*display=10000;*led=false;}
'''
        harness='''int main(int argc,char**){if(argc>1)now=UINT32_MAX-19;setup();loop();
assert(calls==1&&reads==1&&!observedValid&&observedDistance==0);uint32_t first=now;
now=first+100;width=1740;loop();assert(calls==2&&reads==2&&observedValid&&observedDistance==30&&observedDelta==102);
now+=100;width=0;loop();assert(calls==3&&reads==3&&!observedValid&&observedDistance==30&&observedDelta==125);
now+=100;width=58;loop();assert(calls==4&&!observedValid&&observedDistance==30);
now+=100;width=464;loop();assert(calls==5&&observedValid&&observedDistance==8);}
'''
        with tempfile.TemporaryDirectory() as folder:
            base=Path(folder);(base/'Arduino.h').write_text(arduino,encoding='utf-8')
            (base/'TM1637Display.h').write_text('class TM1637Display{public:TM1637Display(int,int){}void setBrightness(int,bool){}void clear(){}void showNumberDec(int,bool,int,int){}};',encoding='utf-8')
            source=base/'test.cpp';source.write_text(kernel+code+harness,encoding='utf-8');binary=base/'test.exe'
            built=subprocess.run([shutil.which('g++'),'-std=c++17','-I',str(base),str(source),'-o',str(binary)],capture_output=True,text=True)
            self.assertEqual(built.returncode,0,built.stderr)
            for args in ([],['wrap']):
                result=subprocess.run([str(binary),*args],capture_output=True,text=True)
                self.assertEqual(result.returncode,0,result.stderr)


if __name__=='__main__':unittest.main()
