import sys
import tempfile
from pathlib import Path
import unittest

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'experiment/code'))
from run_contract_development import behavior


class GPIOOracleTests(unittest.TestCase):
    def test_nonblocking_software_carrier_and_independent_faults(self):
        sketch='''unsigned long previous=0;bool level=false;
void setup(){pinMode(6,OUTPUT);}
void loop(){unsigned long now=millis();
if(now>=1000 && now%1000<100){
 if(micros()-previous>=500){previous=micros();level=!level;digitalWrite(6,level);}
}else{digitalWrite(6,LOW);level=false;}}
'''
        cases=[(sketch,'passed'),
               (sketch.replace('>=500','>=1000'),'failed'),
               (sketch.replace('>=500','>=250'),'failed'),
               (sketch.replace('now%1000<100','now%1000<250'),'failed'),
               (sketch.replace('now>=1000','now>=0'),'failed')]
        with tempfile.TemporaryDirectory() as folder:
            for index,(code,expected) in enumerate(cases):
                with self.subTest(index=index):
                    attempt=Path(folder)/str(index)
                    source=attempt/'build/sketch/sketch.ino.cpp';source.parent.mkdir(parents=True)
                    source.write_text('#include <Arduino.h>\n'+code,encoding='utf-8')
                    result=behavior(attempt,21)
                    self.assertEqual(result['functional_status'],expected,result)

    def test_zero_length_numeric_display_is_no_data_not_a_valid_number(self):
        sketch='''#include <TM1637Display.h>
TM1637Display display(4,5);
void setup(){display.setBrightness(7);}
void loop(){display.showNumberDec((long)analogRead(A0)*9999/1023,4,0);}
'''
        cases=[(sketch,'failed'),
               (sketch.replace('1023,4,0','1023,true,4'),'passed'),
               (sketch.replace('1023,4,0','1023,false,0'),'oracle_runtime_unverified')]
        with tempfile.TemporaryDirectory() as folder:
            for index,(code,expected) in enumerate(cases):
                with self.subTest(index=index):
                    attempt=Path(folder)/str(index)
                    source=attempt/'build/sketch/sketch.ino.cpp';source.parent.mkdir(parents=True)
                    source.write_text('#include <Arduino.h>\n'+code,encoding='utf-8')
                    result=behavior(attempt,19)
                    self.assertEqual(result['functional_status'],expected,result)

    def test_default_yield_user_override_and_default_adc_reference(self):
        on='void setup(){pinMode(13,OUTPUT);} void loop(){yield();digitalWrite(13,HIGH);}'
        custom='void yield(){digitalWrite(13,HIGH);} void setup(){pinMode(13,OUTPUT);} void loop(){yield();}'
        display='''#include <TM1637Display.h>
TM1637Display display(4,5);
void setup(){analogReference(DEFAULT);display.setBrightness(7);}
void loop(){yield();display.showNumberDec((long)analogRead(A0)*9999/1023,false);}'''
        cases=[(1,on,'passed'),(1,custom,'passed'),(1,on.replace('HIGH','LOW'),'failed'),
               (19,display,'passed'),(19,display.replace('analogReference(DEFAULT)','analogReference(EXTERNAL)'),'oracle_runtime_unverified'),
               (19,display.replace('analogRead(A0)','512'),'failed')]
        with tempfile.TemporaryDirectory() as folder:
            for i,(task,code,expected) in enumerate(cases):
                attempt=Path(folder)/str(i);source=attempt/'build/sketch/sketch.ino.cpp';source.parent.mkdir(parents=True)
                source.write_text('#include <Arduino.h>\n'+code,encoding='utf-8')
                with self.subTest(case=i):
                    result=behavior(attempt,task)
                    self.assertEqual(result['functional_status'],expected,
                        (attempt/'host_oracle/build.log').read_text()+'\n'+((attempt/'host_oracle/trace.log').read_text() if (attempt/'host_oracle/trace.log').exists() else str(result)))

    def test_numeric_and_raw_segment_display_with_independent_faults(self):
        number='''#include <TM1637Display.h>
TM1637Display display(4,5);
void setup(){display.setBrightness(7);}
void loop(){display.showNumberDec((long)analogRead(A0)*9999/1023,false);}'''
        raw='''#include <TM1637Display.h>
TM1637Display display(4,5);
void setup(){display.setBrightness(7);}
void loop(){long value=(long)analogRead(A0)*9999/1023;uint8_t data[4]={};int i=3;
do{data[i--]=display.encodeDigit(value%10);value/=10;}while(value&&i>=0);display.setSegments(data);}'''
        cases=[(number,'passed'),(raw,'passed'),(number.replace('1023,false','1023,true'),'passed'),
            (number.replace('analogRead(A0)','512'),'failed'),
            (number.replace('9999/1023','999/1023'),'failed'),
            (number.replace('display(4,5)','display(5,4)'),'failed'),
            (number.replace('setBrightness(7)','setBrightness(7,false)'),'failed'),
            (number.replace('void loop(){','void loop(){delay(200);'),'failed')]
        with tempfile.TemporaryDirectory() as folder:
            for index,(sketch,expected) in enumerate(cases):
                with self.subTest(index=index):
                    attempt=Path(folder)/str(index);source=attempt/'build/sketch/sketch.ino.cpp'
                    source.parent.mkdir(parents=True)
                    source.write_text('#include <Arduino.h>\n'+sketch,encoding='utf-8')
                    result=behavior(attempt,19)
                    self.assertEqual(result['functional_status'],expected,(attempt/'host_oracle/trace.log').read_text() if (attempt/'host_oracle/trace.log').exists() else result)

    def test_tone_reference_and_faults(self):
        beep='''unsigned long last=0;
void setup(){pinMode(6,OUTPUT);}
void loop(){if(millis()-last>=1000){last+=1000;tone(6,1000,100);}}'''
        paused='''bool running=true,previous=false,playing=false; unsigned long last=0,elapsed=0;
void setup(){pinMode(6,OUTPUT);pinMode(2,INPUT_PULLUP);}
void loop(){unsigned long now=millis();bool pressed=digitalRead(2)==LOW;
if(pressed&&!previous)running=!running;
else if(running){elapsed+=now-last;if(elapsed>=3000){elapsed-=3000;playing=true;}
 if(playing&&elapsed>=100)playing=false;}
previous=pressed;last=now;if(running&&playing)tone(6,1000);else noTone(6);}'''
        cases=[(21,beep,'passed'),(29,paused,'passed'),
            (21,beep.replace('tone(6,1000,100);','for(int n=0;n<100;++n){digitalWrite(6,HIGH);delayMicroseconds(500);digitalWrite(6,LOW);delayMicroseconds(500);}'),'passed'),
            (21,beep.replace('tone(6,1000,100);','for(int n=0;n<100;++n){digitalWrite(6,HIGH);delayMicroseconds(250);digitalWrite(6,LOW);delayMicroseconds(250);}'),'failed'),
            (21,beep.replace('tone(6,1000,100)','tone(6,500,100)'),'failed'),
            (21,beep.replace('tone(6,1000,100)','tone(6,1000,250)'),'failed'),
            (21,beep.replace('tone(6,1000,100)','tone(6,1000)'),'failed'),
            (21,beep.replace('tone(6,1000,100)','digitalWrite(6,HIGH)'),'failed'),
            (21,beep.replace('last>=1000','last>=900').replace('last+=1000','last+=900'),'failed'),
            (29,paused.replace('pressed&&!previous','pressed'),'failed'),
            (29,paused.replace('running=!running;','{running=!running;elapsed=0;}'),'failed'),
            (29,paused.replace('if(pressed&&!previous)','if(false)'),'failed')]
        # This fault passes a trace ending after its first beep: accumulating
        # period time only while silent adds 100ms drift to every later cycle.
        drift='''bool running=true,previous=false;unsigned long last=0,phase=0,remaining=0;
void setup(){pinMode(2,INPUT_PULLUP);pinMode(6,OUTPUT);}
void loop(){unsigned long now=millis(),dt=now-last;last=now;bool pressed=digitalRead(2)==LOW;
if(pressed&&!previous)running=!running;previous=pressed;
if(running){if(remaining>0)remaining=remaining>dt?remaining-dt:0;
 if(remaining==0){phase+=dt;if(phase>=3000){phase-=3000;remaining=100;}}}
if(running&&remaining>0)tone(6,1000);else noTone(6);}'''
        cases.append((29,drift,'failed'))
        with tempfile.TemporaryDirectory() as folder:
            for index,(task,sketch,expected) in enumerate(cases):
                with self.subTest(index=index):
                    attempt=Path(folder)/str(index);source=attempt/'build/sketch/sketch.ino.cpp'
                    source.parent.mkdir(parents=True)
                    source.write_text('#include <Arduino.h>\n'+sketch,encoding='utf-8')
                    self.assertEqual(behavior(attempt,task)['functional_status'],expected)

    def test_reference_sketches_and_independent_faults(self):
        on='void setup(){pinMode(13,OUTPUT);digitalWrite(13,HIGH);} void loop(){}'
        blink='bool lit=false; void setup(){pinMode(12,OUTPUT);digitalWrite(12,LOW);} void loop(){delay(500);lit=!lit;digitalWrite(12,lit);}'
        button='void setup(){pinMode(13,OUTPUT);pinMode(2,INPUT_PULLUP);} void loop(){digitalWrite(13,digitalRead(2)==LOW);}'
        fade='void setup(){pinMode(5,OUTPUT);} void loop(){int phase=millis()%2000;analogWrite(5,(phase<=1000?phase:2000-phase)*255L/1000);}'
        adc='void setup(){pinMode(9,OUTPUT);pinMode(2,INPUT_PULLUP);} void loop(){analogWrite(9,digitalRead(2)==LOW?map(analogRead(A0),0,1023,0,255):0);}'
        toggle='''bool running=false,previous=false; unsigned long start=0;
void setup(){pinMode(13,OUTPUT);pinMode(2,INPUT_PULLUP);digitalWrite(13,LOW);}
void loop(){bool pressed=digitalRead(2)==LOW;if(pressed && !previous){running=!running;start=millis();}previous=pressed;
digitalWrite(13,running && ((millis()-start)/500)%2);}'''
        cases=[(1,on,'passed'),(2,blink,'passed'),(3,button,'passed'),
               (2,blink.replace('lit=!lit;digitalWrite(12,lit);','digitalWrite(12,!digitalRead(12));'),'passed'),
               (1,on.replace('void loop(){}','void loop(){digitalRead(7);}'),'oracle_runtime_unverified'),
               (1,on.replace('HIGH','LOW'),'failed'),(2,blink.replace('500','400'),'failed'),
               (3,button.replace('==LOW','==HIGH'),'failed'),(3,button.replace('digitalRead(2)==LOW','true'),'failed'),
               (8,fade,'passed'),(4,adc,'passed'),(28,toggle,'passed'),
               (8,fade.replace('analogWrite(5,(phase<=1000?phase:2000-phase)*255L/1000)','analogWrite(5,128)'),'failed'),
               (8,fade.replace('%2000','%1000'),'failed'),
               (4,adc.replace('analogRead(A0)','512'),'failed'),
               (4,adc.replace('==LOW','==HIGH'),'failed'),
               (28,toggle.replace('pressed && !previous','pressed'),'failed'),
               (28,toggle.replace('running && ','true && '),'failed'),
               (28,toggle.replace('start=millis();','start=0;'),'failed')]
        with tempfile.TemporaryDirectory() as folder:
            for index,(task,sketch,expected) in enumerate(cases):
                with self.subTest(index=index):
                    attempt=Path(folder)/str(index)
                    source=attempt/'build/sketch/sketch.ino.cpp'
                    source.parent.mkdir(parents=True)
                    source.write_text('#include <Arduino.h>\n'+sketch,encoding='utf-8')
                    result=behavior(attempt,task)
                    self.assertEqual(result['functional_status'],expected)


if __name__=='__main__':unittest.main()
