"""Mixed-device isolation, coherent DHT sampling and real production boundaries."""
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
from app.services.hardware_adapter import validate_hardware_contract, generate_hardware_adapter
from app.services.interface_planner import contract_digest, check_explicit_requirements
from app.services.interface_semantics import model_boundary_prompt
from app.services.sysml_compiler import validate_executable_sysml
from tests.ultrasonic_unittest import lower_verified, adapt_verified, compile_code, CLI


def inventory():
    return json.loads(Path('tools/examples/mixed_environment.contract.json').read_text(encoding='utf-8'))


SOURCE=Path('tools/sysml/mixed_environment.sysml').read_text(encoding='utf-8')

# Test-controlled sensor transport. Acquisition consumes time; one DHT frame
# is cached for 2000ms as in Adafruit 1.4.7. This is not an electrical emulator.
ARDUINO='''#pragma once
#include <stdint.h>
#include <math.h>
#include <vector>
#include <string>
enum {LOW=0,HIGH=1,INPUT=0,OUTPUT=1,INPUT_PULLUP=2};
inline uint32_t now=0;inline int levels[70]={},pulse_reads=0;
inline unsigned long pulse=464;
inline unsigned long millis(){return now;}
inline void pinMode(int,int){}
inline void digitalWrite(int pin,int value){levels[pin]=value;}
inline void delayMicroseconds(unsigned int){}
inline unsigned long pulseIn(int pin,int state,unsigned long timeout){
 if(pin!=9||state!=HIGH||timeout!=25000)__builtin_abort();++pulse_reads;now+=pulse?2:25;return pulse;}
struct SerialEvent{uint32_t at;std::string label;double value;};
inline std::vector<SerialEvent> serial_events;
struct SerialStub{std::string label;void begin(int b){if(b!=115200)__builtin_abort();}
 void print(const char* s){label=s;}void println(double v){serial_events.push_back({now,label,v});}};
inline SerialStub Serial;
'''
DHT='''#pragma once
#include <Arduino.h>
enum{DHT11=11,DHT22=22};
inline bool transport_valid=false;inline float temperature=33,humidity=65;
inline int dht_reads=0;inline std::vector<uint32_t> dht_starts;
class DHT {uint32_t last=0;bool begun=false,cached_valid=false;float t=0,h=0;
 public:DHT(int pin,int type){if(pin!=2||type!=DHT22)__builtin_abort();}
 void begin(){begun=true;last=now-2000;}
 bool read(bool force=false){if(!begun)__builtin_abort();if(!force&&(uint32_t)(now-last)<2000)return cached_valid;
  last=now;dht_starts.push_back(now);++dht_reads;now+=7;cached_valid=transport_valid;t=temperature;h=humidity;return cached_valid;}
 float readTemperature(bool fahrenheit=false,bool force=false){if(!read(force))return NAN;return fahrenheit?t*1.8+32:t;}
 float readHumidity(bool force=false){return read(force)?h:NAN;}
};
'''


class DHTContractTests(unittest.TestCase):
    def test_source_sensor_identity_and_circuit_roles_are_preserved(self):
        c=inventory()
        check_explicit_requirements(c,'DHT22 temperature',{})
        check_explicit_requirements(c,'Use DHT22; do not use DHT11.',{})
        with self.assertRaisesRegex(ValueError,'Explicit DHT11'):
            check_explicit_requirements(c,'DHT11 temperature',{})
        circuit=dict(parts=[dict(id='board',type='wokwi-arduino-uno'),dict(id='sensor',type='wokwi-dht22')],
                     connections=[['sensor:SDA','board:2']])
        check_explicit_requirements(c,json.dumps(circuit),{})
        circuit['connections'][0][1]='board:8'
        with self.assertRaisesRegex(ValueError,'DHT data wiring'):
            check_explicit_requirements(c,json.dumps(circuit),{})

    def run_cpp(self, code, *, wrap=False, succeeds=True):
        with tempfile.TemporaryDirectory() as folder:
            base=Path(folder)
            for name,contents in [('Arduino.h',ARDUINO),('DHT.h',DHT),('test.cpp',code)]:
                (base/name).write_text(contents,encoding='utf-8')
            binary=base/'test.exe'
            build=subprocess.run([shutil.which('g++'),'-std=c++17','-I',str(base),str(base/'test.cpp'),'-o',str(binary)],capture_output=True,text=True)
            self.assertEqual(build.returncode,0,build.stderr)
            for args in ([[],['wrap']] if wrap else [[]]):
                ran=subprocess.run([str(binary),*args],capture_output=True,text=True,timeout=10)
                if succeeds:
                    self.assertEqual(ran.returncode,0,ran.stderr)
                else:
                    self.assertNotEqual(ran.returncode,0,'Independent assertions accepted a behavioral mutant')
                    self.assertIn('Assertion',ran.stderr)

    def test_common_contract_rejects_incoherent_frames_and_mixed_legacy_gating(self):
        good=inventory();validate_contract(good);validate_hardware_contract(good)
        bads=[]
        bad=copy.deepcopy(good);bad['interfaces'][2]['type']='f64';bads.append(bad)
        bad=copy.deepcopy(good);bad['interfaces'][2]['initial']=True;bads.append(bad)
        bad=copy.deepcopy(good);bad['interfaces'][2]['sample_period_ms']=4000;bads.append(bad)
        bad=copy.deepcopy(good);bad['interfaces'][2]['binding']['pin']=7;bads.append(bad)
        bad=copy.deepcopy(good);bad['interfaces'][2]['binding']['sensor']='DHT11';bads.append(bad)
        bad=copy.deepcopy(good);bad['interfaces'][0]['sample_period_ms']=1000;bads.append(bad)
        bad=copy.deepcopy(good);bad['interfaces'][1]['unit']='degC';bads.append(bad)
        bad=copy.deepcopy(good);bad['interfaces'][2]['binding']['invalid_policy']='zero';bads.append(bad)
        bad=copy.deepcopy(good);duplicate=copy.deepcopy(bad['interfaces'][0]);duplicate['name']='duplicate';bad['interfaces'].append(duplicate);bads.append(bad)
        # Remove dependencies too, so this specifically checks the mixed-device gate.
        bad=copy.deepcopy(good);bad['interfaces'].pop(2)
        for deps in bad['required_dependencies'].values():
            if 'environmentValid' in deps:deps.remove('environmentValid')
        bads.append(bad)
        for i,bad in enumerate(bads):
            with self.subTest(i=i),self.assertRaises(ValueError):validate_contract(bad)
        # Physical aliases represent the same sensor; changed types cannot alias.
        for p in good['interfaces'][:3]:p['binding']['pin']='A0'
        good['interfaces'][2]['binding']['pin']=14
        validate_hardware_contract(good)
        self.assertIn('model runs with validity false',model_boundary_prompt(good))

    def test_coherent_sampling_invalid_recovery_clock_and_rollover(self):
        c=inventory()
        header='void ProgramPage1(double temperatureC, double humidityPercent, bool environmentValid, double distanceCm, bool distanceValid, int elapsedMilliseconds, double* measuredTemperature, double* measuredHumidity, bool* reportDue, bool* highTemperature);'
        glue=generate_hardware_adapter(header,c)
        spy='''#include <Arduino.h>
#include <cassert>
int ProgramPage1_instance=0,calls=0,delta=0;uint32_t previous=0;
double observedT=0,observedH=0,observedD=0;bool observedV=false,observedDV=false;
void ProgramPage1_Init(int*){}
void ProgramPage1(double t,double h,bool v,double d,bool dv,int ms,double* ot,double* oh,bool* report,bool* alarm){
 assert(ms==(calls?(int32_t)(now-previous):0));previous=now;++calls;delta=ms;
 observedT=t;observedH=h;observedD=d;observedV=v;observedDV=dv;
 *ot=t;*oh=h;*report=false;*alarm=dv&&d<15;}
'''
        harness='''int main(int argc,char**){if(argc>1)now=UINT32_MAX-100;setup();loop();
assert(calls==1&&dht_reads==1&&pulse_reads==1&&!observedV&&observedDV&&observedD==8&&levels[13]==HIGH);
assert(observedT==0&&observedH==0);uint32_t first=dht_starts[0];
now=first+500;loop();assert(calls==2&&dht_reads==1&&levels[13]==HIGH);
transport_valid=true;temperature=33;humidity=65;now=first+2000;loop();
assert(calls==3&&dht_reads==2&&observedV&&observedT==33&&observedH==65);
temperature=12;humidity=99;now+=100;loop();assert(dht_reads==2&&observedT==33&&observedH==65);
transport_valid=false;now=first+4010;loop();assert(dht_reads==3&&!observedV&&observedT==33&&observedH==65&&levels[13]==HIGH);
transport_valid=true;temperature=NAN;now=first+6020;loop();assert(dht_reads==4&&!observedV&&observedT==33&&observedH==65);
temperature=32;humidity=70;now=first+8030;loop();assert(dht_reads==5&&observedV&&observedT==32&&observedH==70);
for(size_t i=1;i<dht_starts.size();++i)assert((uint32_t)(dht_starts[i]-dht_starts[i-1])>=2000);
}'''
        self.run_cpp(spy+glue+harness,wrap=True)

    def test_legacy_dht_only_retains_explicit_wait_for_first_valid_semantics(self):
        c=json.loads(Path('tools/examples/environment_report.contract.json').read_text(encoding='utf-8'))
        header='void ProgramPage1(double temperatureC, double humidityPercent, int elapsedMilliseconds, double* measuredTemperature, double* measuredHumidity, bool* reportDue, bool* highTemperature);'
        spy='''#include <Arduino.h>
#include <cassert>
int ProgramPage1_instance=0,calls=0;void ProgramPage1_Init(int*){}
void ProgramPage1(double t,double h,int ms,double* ot,double* oh,bool* r,bool* a){++calls;*ot=t;*oh=h;*r=false;*a=t>32;}
'''
        self.run_cpp(spy+generate_hardware_adapter(header,c)+'''int main(){setup();loop();assert(calls==0&&dht_reads==1);
now=2000;transport_valid=true;loop();assert(calls==1&&dht_reads==2&&levels[13]==HIGH);
now=4000;transport_valid=false;temperature=0;loop();assert(calls==2&&dht_reads==3&&levels[13]==HIGH);}''')

    def test_browser_sysml_ir_provenance_board_compile_and_independent_behavior(self):
        with tempfile.TemporaryDirectory() as folder,TestClient(app) as client:
            base=Path(folder);c=inventory()
            with patch('app.main._call_llm',side_effect=AssertionError('No LLM in deterministic conversion')):
                prepared=client.post('/generate/interface-contract',json=dict(requirement=c['requirements'][0]['text'],interface_contract=c,out_path=str(base/'contract.json')))
            self.assertEqual(prepared.status_code,200,prepared.text)
            browser=json.loads(subprocess.run([shutil.which('node'),'-e',
                'let s="";process.stdin.on("data",x=>s+=x);process.stdin.on("end",()=>process.stdout.write(JSON.stringify(JSON.parse(s))));'],
                input=json.dumps(prepared.json()),text=True,capture_output=True,check=True).stdout)
            c=browser['interface_contract'];self.assertEqual(contract_digest(c),browser['contract_sha256'])
            for board in ('arduino:avr:uno','arduino:avr:mega'):
                with self.subTest(board=board):
                    c['board']=board;checked=validate_executable_sysml(SOURCE);self.assertTrue(checked['ok'],checked)
                    target=base/board.split(':')[-1]
                    generated=lower_verified(SOURCE,c,checked['ir'],target/'lowered')
                    code=adapt_verified(generated,c,target/'lowered')
                    compiled,log=compile_code(target/'compiled',code,CLI,board)
                    self.assertTrue(compiled['compile_ok'],log)
                    # Hand-reviewed assertions use public outputs and controlled
                    # sensor input, never the IR's own output as the expected value.
                    harness='''\n#include <cassert>
int main(){setup();loop();assert(levels[13]==HIGH&&serial_events.empty());
uint32_t first=now;now=first+100;pulse=1740;loop();assert(levels[13]==LOW);
transport_valid=true;temperature=33;humidity=65;now=2005;loop();assert(levels[13]==HIGH);
transport_valid=false;now=4010;loop();assert(levels[13]==LOW);
now=first+5000;loop();assert(serial_events.empty()); // invalid report is suppressed by SysML
transport_valid=true;temperature=32;humidity=70;now=6020;loop();assert(levels[13]==LOW); // strict >32
now=8030;loop();now=first+10000;loop();assert(serial_events.size()==2);
assert(serial_events[0].label=="Temperature (C): "&&serial_events[0].value==32);
assert(serial_events[1].label=="Humidity (%RH): "&&serial_events[1].value==70);
assert(serial_events[0].at<first+10100); // clock did not restart after DHT failure
}'''
                    self.run_cpp(code+harness)
                    if board=='arduino:avr:uno':
                        marker='\n  ProgramPage1('
                        self.assertEqual(code.count(marker),1)
                        self.run_cpp(code.replace(marker,'\n  if (!hw_value_environmentValid) return;'+marker)+harness,succeeds=False)
                        marker='hw_value_environmentValid = hw_dht_0_valid;'
                        self.assertEqual(code.count(marker),1)
                        self.run_cpp(code.replace(marker,'hw_value_environmentValid = true;')+harness,succeeds=False)


if __name__=='__main__':unittest.main()
