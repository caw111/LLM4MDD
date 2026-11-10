"""Independent mixed DHT/sonar/display/tone/serial acceptance and fault controls."""
import json
from pathlib import Path
import tempfile
import unittest

from tests.ultrasonic_unittest import behavior,lower_verified,adapt_verified,compile_code,CLI
from app.services.sysml_compiler import validate_executable_sysml
from app.services.interface_planner import check_explicit_requirements
from run_contract_development import INTERPRETATIONS


DIRECT='''#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
DHT sensor(7,DHT11);TM1637Display display(4,5);
bool attemptedT=false,attemptedD=false,tv=false,dv=false,previous=false,oldHot=false,oldNear=false;
double temperature=0,distance=0;uint32_t sampledT=0,sampledD=0,alarmStart=0,reported=0,control=0;
int visible=10000,frequency=-1;
void setup(){sensor.begin();pinMode(8,OUTPUT);digitalWrite(8,LOW);pinMode(9,INPUT);
display.setBrightness(7);display.clear();Serial.begin(115200);noTone(6);}
void loop(){uint32_t now=millis();if(now-control<10)return;
 if(!attemptedT||(uint32_t)(now-sampledT)>=2000){sampledT=now;attemptedT=true;
  float t=sensor.readTemperature();tv=isfinite(t);if(tv)temperature=t;}
 now=millis();if(!attemptedD||(uint32_t)(now-sampledD)>=100){sampledD=now;attemptedD=true;
  digitalWrite(8,LOW);delayMicroseconds(2);digitalWrite(8,HIGH);delayMicroseconds(10);digitalWrite(8,LOW);
  unsigned long width=pulseIn(9,HIGH,25000);dv=width>=116&&width<=23200;if(dv)distance=width/58.0;}
 now=millis();bool hot=tv&&temperature>32,near=dv&&distance<15,active=hot||near;
 if(active&&!previous)alarmStart=now;
 int hz=active&&((uint32_t)(now-alarmStart)%1000)<500?1000:0;
 if(hz!=frequency){frequency=hz;if(hz)tone(6,hz);else noTone(6);}
 if(active&&(!previous||hot!=oldHot||near!=oldNear||(uint32_t)(now-reported)>=1000)){
  if(hot)Serial.println("ALARM_TEMP");if(near)Serial.println("ALARM_DISTANCE");reported=now;}
 previous=active;oldHot=hot;oldNear=near;
 bool distancePage=(now/2000)%2;
 int number=distancePage?(dv?(int)distance:10000):(tv?(int)temperature:10000);
 if(number!=visible){visible=number;if(number==10000)display.clear();else display.showNumberDec(number,false);}
 control=millis();
}
'''


class MixedEnvironmentTests(unittest.TestCase):
    def test_missing_initialization_and_unmodeled_display_are_distinguished(self):
        opaque='''class TM1637Display{public:TM1637Display(int,int){}void setBrightness(int){}
void clear(){pinMode(4,OUTPUT);digitalWrite(4,LOW);}void showNumberDec(int,bool){clear();}};'''
        none=opaque.replace('pinMode(4,OUTPUT);digitalWrite(4,LOW);','')
        cases=[(DIRECT.replace('sensor.begin();','pinMode(7,INPUT_PULLUP);').replace('sensor.readTemperature()','sensor.readTemperature(false,true)'),'failed'),
               (DIRECT.replace('#include <TM1637Display.h>',opaque),'oracle_runtime_unverified'),
               (DIRECT.replace('#include <TM1637Display.h>',none),'failed')]
        with tempfile.TemporaryDirectory() as folder:
            for i,(code,expected) in enumerate(cases):
                with self.subTest(i=i):
                    target=Path(folder)/str(i);source=target/'build/sketch/sketch.ino.cpp'
                    source.parent.mkdir(parents=True);source.write_text(code,encoding='utf-8')
                    result=behavior(target,301)
                    self.assertEqual(result['functional_status'],expected,result)

    def test_output_tolerance_does_not_rewrite_sensor_acquisition_period(self):
        c=json.loads(Path('tools/examples/environment_alarm.contract.json').read_text(encoding='utf-8'))
        check_explicit_requirements(c,INTERPRETATIONS['complex_001'],{})
        for port in c['interfaces']:
            if port['binding']['kind'].startswith('ultrasonic_'):port['sample_period_ms']=400
        with self.assertRaisesRegex(ValueError,'Distance sampling period'):
            check_explicit_requirements(c,INTERPRETATIONS['complex_001'],{})
        with self.assertRaisesRegex(ValueError,'sample_period_ms plus bounded acquisition'):
            check_explicit_requirements(c,'HC-SR04 输入变化250ms内反映。',{})
        c=json.loads(Path('tools/examples/environment_alarm.contract.json').read_text(encoding='utf-8'))
        for port in c['interfaces']:
            if port['binding']['kind'].startswith('dht_'):port['sample_period_ms']=4000
        with self.assertRaisesRegex(ValueError,'DHT sampling period'):
            check_explicit_requirements(c,INTERPRETATIONS['complex_001'],{})
        c=json.loads(Path('tools/examples/environment_alarm.contract.json').read_text(encoding='utf-8'))
        for port in c['interfaces']:
            if port['binding']['kind']=='serial_value':port['binding']['label']='Value: '
        with self.assertRaisesRegex(ValueError,'serial reason token'):
            check_explicit_requirements(c,INTERPRETATIONS['complex_001'],{})

    def test_independent_reference_and_behavioral_faults(self):
        mutants=[('reference',DIRECT,'passed'),
            ('global_dht_gate',DIRECT.replace('now=millis();bool hot=', 'if(!tv)return;now=millis();bool hot='),'failed'),
            ('ignore_validity',DIRECT.replace('tv&&temperature>32','temperature>32'),'failed'),
            ('temperature_boundary',DIRECT.replace('temperature>32','temperature>=32'),'failed'),
            ('distance_boundary',DIRECT.replace('distance<15','distance<=15'),'failed'),
            ('rounded_threshold',DIRECT.replace('distance<15','round(distance)<15'),'failed'),
            ('continuous_tone',DIRECT.replace('((uint32_t)(now-alarmStart)%1000)<500','true'),'failed'),
            ('wrong_period',DIRECT.replace('%1000)<500','%2000)<500'),'failed'),
            ('no_rotation',DIRECT.replace('(now/2000)%2','false'),'failed'),
            ('wrong_page_period',DIRECT.replace('(now/2000)%2','(now/3000)%2'),'failed'),
            ('missing_reason',DIRECT.replace('if(hot)Serial.println("ALARM_TEMP");',''),'failed'),
            ('false_reason',DIRECT.replace('if(hot)Serial.println','if(active)Serial.println'),'failed'),
            ('constant_distance',DIRECT.replace('distance=width/58.0','distance=30'),'failed'),
            ('wrong_sensor_type',DIRECT.replace('sensor(7,DHT11)','sensor(7,DHT22)'),'failed'),
            ('no_timeout',DIRECT.replace('pulseIn(9,HIGH,25000)','pulseIn(9,HIGH)'),'failed')]
        with tempfile.TemporaryDirectory() as folder:
            for name,code,expected in mutants:
                with self.subTest(name=name):
                    target=Path(folder)/name;source=target/'build/sketch/sketch.ino.cpp'
                    source.parent.mkdir(parents=True);source.write_text(code,encoding='utf-8')
                    result=behavior(target,301)
                    log=target/'host_oracle/trace.log'
                    self.assertEqual(result['functional_status'],expected,log.read_text() if log.exists() else (target/'host_oracle/build.log').read_text())

    def test_full_reference_model_real_backend_and_board(self):
        c=json.loads(Path('tools/examples/environment_alarm.contract.json').read_text(encoding='utf-8'))
        source=Path('tools/sysml/environment_alarm.sysml').read_text(encoding='utf-8')
        checked=validate_executable_sysml(source);self.assertTrue(checked['ok'],checked)
        with tempfile.TemporaryDirectory() as folder:
            target=Path(folder)
            generated=lower_verified(source,c,checked['ir'],target/'lowered')
            code=adapt_verified(generated,c,target/'lowered')
            compiled,log=compile_code(target/'compiled',code,CLI,'arduino:avr:uno')
            self.assertTrue(compiled['compile_ok'],log)
            measured=behavior(target/'compiled',301)
            self.assertEqual(measured['functional_status'],'passed',
                (target/'compiled/host_oracle/trace.log').read_text() if (target/'compiled/host_oracle/trace.log').exists() else measured)


if __name__=='__main__':unittest.main()
