"""Requirement-derived pilot: timestamped host API traces and explicit assertions."""
import collections
import concurrent.futures
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import time

import run_workflow267_host_smoke as base

ROOT = base.ROOT
OUT = ROOT/'experiment/output/behavior_pilot10_20261023'
INPUT = ROOT/'experiment/output/llm_review_handoff_20261023/original_requirements.json'
MANIFEST = ROOT/'experiment/output/all_groups_host_smoke_20261023/frozen_manifest.json'
IDS = ['simple_004', 'simple_010', 'simple_012', 'simple_017', 'simple_024',
       'simple_027', 'pceval_led_RGB_basic', 'pceval_potentiometer_servo_motor',
       'embedbench_1', 'embedbench_5']
BASE_FIXTURE = ROOT/'experiment/output/workflow267_host_smoke_20261023/fixture'


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2), encoding='utf-8')


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def numbers(text):
    # Arduino print(label); print(value) need not insert a separator.
    return [float(n) for n in re.findall(r'[-+]?\d+(?:\.\d+)?', text)]


def specification():
    requirements = {r['id']:r for r in json.loads(INPUT.read_text())}
    specs = {}
    analog = [(0, 0), (1000, 256), (2000, 512), (3000, 768), (4000, 1023), (5000, 512), (6000, 0), (7000, 1023)]
    for case in IDS:
        specs[case] = dict(case=case, requirement=requirements[case]['requirement'],
                           board=requirements[case]['fqbn'], inputs=[], horizon_ms=9000,
                           origin='Manually specified from the original requirement and supplied circuit, not generated code',
                           scope='Host API behavior; command traces, not physical electrical/shaft/display behavior',
                           assumptions=['Host scheduling is not AVR instruction timing.'])
    def adc(case, values):
        specs[case]['inputs'] = [dict(at_ms=t, kind='analog', pin='A0', value=v) for t,v in values]
    adc('simple_004', analog)
    specs['simple_004'].update(check='pwm_increases', expected_rule='D9 PWM must increase with A0; equal input on return must reproduce brightness. No exact linear formula is imposed because the requirement does not specify it.', assumptions=['Input change allowed 300ms settling time; larger ADC is interpreted as a larger potentiometer setting.'])
    adc('simple_010', [(0,123),(2000,777),(4000,0),(6000,1023)])
    specs['simple_010'].update(check='serial_adc', expected_rule='Serial output contains the currently supplied raw A0 value; numeric reports occur at 1000ms intervals, tolerance 100ms; startup phase and labels unrestricted.')
    specs['simple_012'].update(check='serial_hello', horizon_ms=9500, expected_rule='Serial lines contain Hello Arduino at 2000ms intervals, tolerance 100ms; startup phase unrestricted.')
    specs['simple_017'].update(check='display_1234', horizon_ms=3000, expected_rule='TM1637 CLK2/DIO3 visibly commands segments for 1234 at 500ms and 2500ms.', assumptions=['300ms startup allowance; API-level TM1637 observation. A bit-banged display without decoded frames is unverified, not automatically failed.'])
    adc('simple_024', [(0,0),(1000,499),(2000,500),(3000,501),(4000,1023),(5000,500),(6000,499),(7000,0)])
    specs['simple_024'].update(check='adc_threshold', expected_rule='D13 HIGH iff A0 > 500, including exact threshold 500; check 300ms and 700ms after each input step.', assumptions=['300ms settling allowance, no exact response bound supplied in original requirement.'])
    specs['simple_027'].update(check='two_led_period', horizon_ms=6500, expected_rule='D12 and D13 each switch digital state once every 1000ms, tolerance 100ms. Initial phase, initial level, and relation between LEDs are unrestricted.')
    specs['pceval_led_RGB_basic'].update(check='blue_common_anode', horizon_ms=3000, expected_rule='At 100, 1000 and 2500ms R=D4 HIGH, G=D3 HIGH, B=D2 LOW, all pins OUTPUT. Common anode connected to +5V, so LOW lights a channel.')
    adc('pceval_potentiometer_servo_motor', analog)
    specs['pceval_potentiometer_servo_motor'].update(check='servo_mapping', expected_rule='Servo command on D9 follows A0*180/1023 within 1.1 degrees after 300ms; serial reports the commanded/read angle in each stable input stage.', assumptions=['300ms settling allowance. Observe Servo API pulse commands using default 544..2400us calibration; physical rotation and smoothness are not tested.'])
    specs['embedbench_1'].update(check='led_sequence', horizon_ms=32000, expected_rule='D2/D3 initially off, LED1 on after 5s, both on after 10s, both off after 15s, remain off during following 5s delay, then repeat.', assumptions=['Sample first cycle at 100/4500/5300/9500/10300/14500/15300/19000ms. Allow repeated LED1 activation in 20..25s because wording of restart phase is ambiguous; require LED2 and shutdown again in later trace.'])
    specs['embedbench_5'].update(check='button_toggle', horizon_ms=9000, expected_rule='D3 LED initially on. Each falling edge of D2 toggles once; holding or releasing must not toggle.', inputs=[dict(at_ms=t,kind='digital',pin=2,value=v) for t,v in [(0,1),(1000,0),(2200,1),(3000,0),(3100,1),(4200,0),(5500,1),(6500,0),(7600,1)]], assumptions=['Supplied button is wired D2 to GND; INPUT_PULLUP supplies released HIGH. 150ms response allowance; stable holds also checked.'])
    specs['simple_024']['expected_samples']=[dict(at_ms=e['at_ms']+d,pin=13,value=int(e['value']>500)) for e in specs['simple_024']['inputs'] for d in (300,700)]
    specs['pceval_led_RGB_basic']['expected_samples']=[dict(at_ms=t,pin=pin,pwm_equivalent=value) for t in (100,1000,2500) for pin,value in [(4,255),(3,255),(2,0)]]
    specs['pceval_potentiometer_servo_motor']['expected_samples']=[dict(at_ms=e['at_ms']+300,pin=9,angle_deg=e['value']*180/1023,tolerance_deg=1.1) for e in specs['pceval_potentiometer_servo_motor']['inputs']]
    specs['simple_017']['expected_samples']=[dict(at_ms=t,clk=2,dio=3,on=True,segments=[6,91,79,102]) for t in (500,2500)]
    specs['simple_010']['expected_samples']=[dict(window_ms=[e['at_ms']+200,e['at_ms']+1800],serial_numeric_value=e['value']) for e in specs['simple_010']['inputs']]
    specs['simple_012']['expected_samples']=dict(serial_text='Hello Arduino',interval_ms=2000,tolerance_ms=100,minimum_reports=3)
    specs['simple_027']['expected_samples']=dict(pins=[12,13],transition_interval_ms=1000,tolerance_ms=100,minimum_transitions_per_pin=5)
    specs['simple_004']['expected_samples']=dict(adc=[256,512,768,1023],observe_at_ms=[1300,2300,3300,4300],output_pin=9,relation='strictly increasing PWM; no imposed absolute formula')
    specs['embedbench_1']['expected_samples']=[dict(at_ms=t,pins=[2,3],values=[a,b]) for t,a,b in [(100,0,0),(4500,0,0),(5300,1,0),(9500,1,0),(10300,1,1),(14500,1,1),(15300,0,0),(19000,0,0)]]
    specs['embedbench_5']['expected_samples']=[dict(at_ms=t,pin=3,value=v) for t,v in [(200,1),(800,1),(1150,0),(1800,0),(2350,0),(3150,1),(3900,1),(4350,0),(5100,0),(5650,0),(6650,1),(7300,1),(7800,1),(8600,1)]]
    return specs


HARNESS = r'''
#include <Arduino.h>
#include <TM1637Display.h>
#include <Servo.h>
#include <fstream>
void setup(); void loop();
extern "C" void yield() __attribute__((weak));
extern "C" void yield() {}
std::string esc(const std::string&s){std::string r="\"";const char*h="0123456789abcdef";for(unsigned char c:s){if(c=='"'||c=='\\'){r+='\\';r+=c;}else if(c<32){r+="\\u00";r+=h[c>>4];r+=h[c&15];}else r+=c;}return r+'"';}
int main(int argc,char**argv){
 bool init=false;uint64_t loops=0;std::string status="completed";
 try{setup();init=true;while(oracle::now<pilot::horizon){loop();++loops;delayMicroseconds(100);}}
 catch(const std::exception&e){status=e.what();}
 std::ofstream out(argv[1]);
 int levels[PIN_COUNT],kinds[PIN_COUNT];for(int i=0;i<PIN_COUNT;++i){levels[i]=-99999;kinds[i]=-1;}
 for(const auto&e:oracle::events){if(e.pin<0||e.pin>=PIN_COUNT)continue;if(levels[e.pin]==e.level&&kinds[e.pin]==int(e.pwm))continue;levels[e.pin]=e.level;kinds[e.pin]=e.pwm;
 out<<"{\"type\":\"gpio\",\"at_ms\":"<<e.at<<",\"pin\":"<<e.pin<<",\"value\":"<<e.level<<",\"pwm\":"<<(e.pwm?"true":"false")<<"}\n";}
 for(const auto&e:pilot::reads)out<<"{\"type\":\"input_read\",\"at_ms\":"<<e.at<<",\"pin\":"<<e.pin<<",\"value\":"<<e.value<<",\"analog\":"<<(e.analog?"true":"false")<<"}\n";
 for(const auto&e:oracle::serial_lines)out<<"{\"type\":\"serial\",\"at_ms\":"<<e.at<<",\"baud\":"<<e.baud<<",\"text\":"<<esc(e.text)<<"}\n";
 for(const auto&e:oracle::servos)out<<"{\"type\":\"servo\",\"at_ms\":"<<e.at<<",\"pin\":"<<e.pin<<",\"pulse_us\":"<<e.pulse<<",\"active\":"<<(e.active?"true":"false")<<"}\n";
 for(const auto&e:oracle::displays){out<<"{\"type\":\"display\",\"at_ms\":"<<e.at<<",\"clk\":"<<e.clk<<",\"dio\":"<<e.dio<<",\"on\":"<<(e.on?"true":"false")<<",\"segments\":[";for(int i=0;i<4;++i){if(i)out<<',';out<<int(e.segments[i]);}out<<"]}\n";}
 for(int i=0;i<PIN_COUNT;++i)out<<"{\"type\":\"pin_mode_final\",\"pin\":"<<i<<",\"mode\":"<<oracle::modes[i]<<"}\n";
 out<<"{\"type\":\"execution\",\"setup_returned\":"<<(init?"true":"false")<<",\"loop_returns\":"<<loops<<",\"virtual_ms\":"<<oracle::now<<",\"raw_gpio_calls\":"<<oracle::events.size()<<",\"status\":"<<esc(status)<<"}\n";
 return status=="completed"||status=="capture-end"?0:3;
}
'''


def make_fixture(specs):
    f = OUT/'fixture'
    f.mkdir(parents=True, exist_ok=True)
    h = (BASE_FIXTURE/'Arduino.h').read_text()
    h = h.replace('#include <string>', '#include <string>\n#include <cstring>\n#include <algorithm>\nusing std::min; using std::max;')
    # Stream timeout affects reads; the selected serial cases only use output.
    h = h.replace('struct SerialStub {', 'struct SerialStub {\n unsigned long timeout_ms=1000; void setTimeout(unsigned long ms){timeout_ms=ms;}')
    # New pilot-specific environment; none of the previous fixture/results is changed.
    env = r'''
#include "case_input.h"
namespace pilot {
struct Read{uint64_t at;int pin,value;bool analog;};inline std::vector<Read> reads;
inline int last_analog[PIN_COUNT]={},last_digital[PIN_COUNT]={};
inline bool seen_analog[PIN_COUNT]={},seen_digital[PIN_COUNT]={};
inline int input(int pin,bool analog){
 int value=analog?0:HIGH;
 for(const auto&e:inputs)if(e.pin==pin&&e.analog==analog&&e.at<=oracle::now)value=e.value;
 auto*last=analog?last_analog:last_digital;auto*seen=analog?seen_analog:seen_digital;
 if(!seen[pin]||last[pin]!=value){reads.push_back({oracle::now,pin,value,analog});seen[pin]=true;last[pin]=value;}
 return value;
}
}
'''
    h = h.replace('inline int analogRead(int pin) {', env+'\ninline int analogRead(int pin) {')
    h = base.replace_function(h, 'inline int analogRead(int pin)', '''
 oracle::advance_clock();oracle::progress();if(pin>=0&&pin<ADC_COUNT)pin+=A0;
 if(pin<A0||pin>=A0+ADC_COUNT)throw std::runtime_error("adc-pin-range");return pilot::input(pin,true);
''')
    h = base.replace_function(h, 'inline int digitalRead(int pin)', '''
 oracle::advance_clock();oracle::progress();if(pin<0||pin>=PIN_COUNT)throw std::runtime_error("digital-pin-range");
 if(oracle::modes[pin]==OUTPUT){for(auto e=oracle::events.rbegin();e!=oracle::events.rend();++e)if(e->pin==pin)return e->level?HIGH:LOW;return LOW;}return pilot::input(pin,false);
''')
    # The horizon declaration must precede Arduino's progress() definition.
    h = h.replace('inline void progress() {', '#include "case_horizon.h"\ninline void progress() {')
    h = h.replace('if(now > 10000) throw std::runtime_error("trace-end");', 'if(now > PILOT_HORIZON){now=PILOT_HORIZON;sub_ms=0;throw std::runtime_error("capture-end");}')
    (f/'Arduino.h').write_text(h)
    for name in ['Servo.h', 'TM1637Display.h']:
        shutil.copyfile(BASE_FIXTURE/name, f/name)
    (f/'main.cpp').write_text(HARNESS)
    for case,s in specs.items():
        folder = OUT/'test_data'/case
        save(folder/'test_spec.json', s)
        (folder/'case_horizon.h').write_text(f"#define PILOT_HORIZON {s['horizon_ms']}UL\n")
        entries = ','.join('{'+f"{e['at_ms']},{e['pin']},{e['value']},{str(e['kind']=='analog').lower()}"+'}' for e in s['inputs'])
        (folder/'case_input.h').write_text('#pragma once\nnamespace pilot {struct Stimulus{uint64_t at;int pin,value;bool analog;}; inline std::vector<Stimulus> inputs={'+entries+'}; inline constexpr uint64_t horizon='+str(s['horizon_ms'])+'; }\n')
    save(OUT/'fixture_hashes.json', {x.name:sha(x) for x in f.iterdir()})
    return f


def evaluate(spec, trace):
    checks = []
    def check(label, expected, actual, ok):
        checks.append(dict(label=label, expected=expected, actual=actual, passed=bool(ok)))
    def rows(kind):
        return [e for e in trace if e['type']==kind]
    def at(events, t):
        values = [e for e in events if e.get('at_ms',0)<=t]
        return values[-1] if values else None
    def gpio(pin,t,pwm=False):
        e=at([x for x in rows('gpio') if x['pin']==pin],t)
        if e is None:return None
        return e['value'] if e['pwm'] or not pwm else (255 if e['value'] else 0)
    def mode(pin):
        return next((e['mode'] for e in rows('pin_mode_final') if e['pin']==pin),None)
    def number_match(text, expected, tolerance=0):
        return any(abs(n-expected)<=tolerance for n in numbers(text))
    name=spec['check']
    if name=='pwm_increases':
        values=[gpio(9,1300+i*1000,True) for i in range(4)]
        check('D9 configured OUTPUT',1,mode(9),mode(9)==1)
        check('ADC256/512/768/1023 produce increasing PWM','strictly increasing',values,all(v is not None for v in values) and all(a<b for a,b in zip(values,values[1:])))
        for t,prior in [(5300,2300),(6300,300),(7300,4300)]:
            a,b=gpio(9,t,True),gpio(9,prior,True)
            check(f'repeated input at {t}ms reproduces PWM',b,a,a is not None and b is not None and abs(a-b)<=1)
    elif name=='adc_threshold':
        check('D13 configured OUTPUT',1,mode(13),mode(13)==1)
        for e in spec['inputs']:
            for delta in [300,700]:
                t=e['at_ms']+delta; expected=int(e['value']>500);actual=gpio(13,t)
                check(f'A0={e["value"]} at {t}ms',expected,actual,actual==expected)
    elif name in ('serial_adc','serial_hello'):
        serial=rows('serial')
        if name=='serial_adc':
            numeric=[e for e in serial if re.search(r'\d',e['text'])]
            for inp in spec['inputs']:
                lo=inp['at_ms']+200;hi=inp['at_ms']+1800
                selected=[e for e in numeric if lo<=e['at_ms']<=hi]
                correct=[e for e in selected if number_match(e['text'],inp['value'])]
                check(f'A0 raw value {inp["value"]} reported in {lo}..{hi}ms',inp['value'],selected,bool(correct))
                check(f'no incorrect ADC report in {lo}..{hi}ms','every numeric report contains raw ADC',selected,all(number_match(e['text'],inp['value']) for e in selected))
            reports=numeric;period=1000
        else:
            reports=[e for e in serial if 'Hello Arduino' in e['text']];period=2000
        check('at least three reports', '>=3',len(reports),len(reports)>=3)
        for a,b in zip(reports,reports[1:]):
            dt=b['at_ms']-a['at_ms'];check(f'report interval ending {b["at_ms"]}ms',f'{period} +/-100ms',dt,abs(dt-period)<=100)
    elif name=='display_1234':
        displays=[e for e in rows('display') if e['clk']==2 and e['dio']==3]
        if not displays and rows('gpio'):
            return dict(status='unverified_observer',checks=[],reason='No TM1637 API frames; bit-banged display decoding is not implemented')
        for t in [500,2500]:
            e=at(displays,t);expected=[0x06,0x5b,0x4f,0x66]
            check(f'display visible 1234 at {t}ms',expected,e,e is not None and e['on'] and e['segments']==expected)
    elif name=='two_led_period':
        for pin in [12,13]:
            check(f'D{pin} configured OUTPUT',1,mode(pin),mode(pin)==1)
            events=[e for e in rows('gpio') if e['pin']==pin and e['at_ms']>100]
            check(f'D{pin} >=5 state transitions','>=5',len(events),len(events)>=5)
            for a,b in zip(events,events[1:]):
                dt=b['at_ms']-a['at_ms'];check(f'D{pin} toggle at {b["at_ms"]}ms','1000 +/-100ms',dt,abs(dt-1000)<=100 and a['value']!=b['value'])
    elif name=='blue_common_anode':
        for pin,level in [(4,1),(3,1),(2,0)]:
            check(f'D{pin} configured OUTPUT',1,mode(pin),mode(pin)==1)
            for t in [100,1000,2500]:
                value=gpio(pin,t,True);expected=255 if level else 0
                check(f'D{pin} common-anode at {t}ms',expected,value,value==expected)
    elif name=='servo_mapping':
        servos=[e for e in rows('servo') if e['pin']==9]
        if not servos and any(e['pin']==9 for e in rows('gpio')):
            return dict(status='unverified_observer', checks=[], reason='Servo API frames absent; software servo pulse decoding is not implemented')
        for inp in spec['inputs']:
            t=inp['at_ms']+300;expected=inp['value']*180/1023;e=at(servos,t)
            actual=(e['pulse_us']-544)*180/(2400-544) if e else None
            check(f'D9 servo angle at {t}ms',round(expected,3),actual,e is not None and e['active'] and abs(actual-expected)<=1.1)
            serial=[e for e in rows('serial') if inp['at_ms']+200<=e['at_ms']<=inp['at_ms']+850]
            check(f'serial angle reported during ADC={inp["value"]}',round(expected,3),serial[:3],any(number_match(e['text'],expected,1.1) for e in serial))
    elif name=='led_sequence':
        for pin in [2,3]:check(f'D{pin} configured OUTPUT',1,mode(pin),mode(pin)==1)
        for t,a,b in [(100,0,0),(4500,0,0),(5300,1,0),(9500,1,0),(10300,1,1),(14500,1,1),(15300,0,0),(19000,0,0)]:
            actual=[gpio(2,t),gpio(3,t)];check(f'LED1/LED2 at {t}ms',[a,b],actual,actual==[a,b])
        for pin,lo,hi in [(2,19900,25300),(3,24900,30300)]:
            found=[e for e in rows('gpio') if e['pin']==pin and lo<=e['at_ms']<=hi and e['value']>0]
            check(f'D{pin} repeats in {lo}..{hi}ms','HIGH command',found,bool(found))
    elif name=='button_toggle':
        check('D2 INPUT_PULLUP for button-to-GND',2,mode(2),mode(2)==2)
        check('D3 configured OUTPUT',1,mode(3),mode(3)==1)
        for t,want in [(200,1),(800,1),(1150,0),(1800,0),(2350,0),(3150,1),(3900,1),(4350,0),(5100,0),(5650,0),(6650,1),(7300,1),(7800,1),(8600,1)]:
            actual=gpio(3,t);check(f'button edge/hold/release at {t}ms',want,actual,actual==want)
    else:
        raise ValueError(name)
    execution=rows('execution')[-1]
    if execution['status'] not in ('completed','capture-end'):
        return dict(status='unverified_runtime',checks=checks,reason=execution['status'])
    return dict(status='passed' if checks and all(c['passed'] for c in checks) else 'failed',checks=checks,
                failed_assertions=sum(not c['passed'] for c in checks))


def execute(row, spec, fixture, key):
    folder=OUT/'executions'/key
    folder.mkdir(parents=True, exist_ok=True)
    source=Path(row['source']);cpp=source.parent.parent/'build/sketch/sketch.ino.cpp'
    assert sha(source)==row['source_sha256']
    result=dict(case=spec['case'],source=str(source),source_sha256=sha(source),preprocessed_sha256=sha(cpp),evidence=str(folder))
    macro='ARDUINO_AVR_MEGA2560' if 'mega' in row['board'] else 'ARDUINO_AVR_UNO'
    command=[str(base.GXX),'-std=c++17','-O0','-D'+macro,'-I'+str(fixture),'-I'+str(OUT/'test_data'/spec['case']),str(cpp),str(fixture/'main.cpp'),'-o',str(folder/'program.exe')]
    result['build_command']=command
    try:
        build=subprocess.run(command,capture_output=True,timeout=60)
        (folder/'build.stderr.txt').write_bytes(build.stderr)
        if build.returncode:
            result.update(status='unverified_host_build',reason='Host API/compile interface limitation; original Arduino compile was successful')
        else:
            run=subprocess.run([str(folder/'program.exe'),str(folder/'trace.jsonl')],capture_output=True,timeout=10)
            (folder/'stdout.txt').write_bytes(run.stdout);(folder/'stderr.txt').write_bytes(run.stderr)
            trace=[json.loads(line) for line in (folder/'trace.jsonl').read_text().splitlines()]
            result.update(evaluate(spec,trace),process_exit_code=run.returncode,trace_rows=len(trace))
    except subprocess.TimeoutExpired:
        result.update(status='unverified_timeout')
    save(folder/'result.json',result)
    return result


def self_check(specs,fixture):
    assert numbers('servo_angle_deg45.04') == [45.04]
    assert numbers('A0 potentiometer123.00') == [0.0,123.0]
    # Known-correct references exercise capture + assertions; deliberately wrong polarity must fail.
    programs={
      'simple_024':'void setup(){pinMode(13,OUTPUT);}void loop(){digitalWrite(13,analogRead(A0)>500);}',
      'pceval_led_RGB_basic':'void setup(){for(int p=2;p<=4;++p)pinMode(p,OUTPUT);digitalWrite(4,HIGH);digitalWrite(3,HIGH);digitalWrite(2,LOW);}void loop(){}',
      'embedbench_5':'bool state=true,prev=false;void setup(){pinMode(2,INPUT_PULLUP);pinMode(3,OUTPUT);digitalWrite(3,HIGH);}void loop(){bool pressed=digitalRead(2)==LOW;if(pressed&&!prev)state=!state;prev=pressed;digitalWrite(3,state);}',
      'simple_017':'TM1637Display display(2,3);void setup(){display.setBrightness(7);display.showNumberDec(1234);}void loop(){}',
      'pceval_potentiometer_servo_motor':'Servo s;uint32_t last=0;void setup(){s.attach(9);Serial.begin(9600);}void loop(){s.write(map(analogRead(A0),0,1023,0,180));if(millis()-last>=100){last=millis();Serial.print("servo_angle_deg");Serial.println(s.read());}}',
      'simple_004':'void setup(){}void loop(){analogWrite(9,map(analogRead(A0),0,1023,0,255));}',
      'simple_010':'uint32_t last=0;void setup(){Serial.begin(9600);}void loop(){if(millis()-last>=1000){last=millis();Serial.print("A0 potentiometer");Serial.println(analogRead(A0));}}',
      'simple_012':'uint32_t last=0;void setup(){Serial.begin(9600);}void loop(){if(millis()-last>=2000){last=millis();Serial.println("Hello Arduino");}}',
      'simple_027':'bool level=false;uint32_t last=0;void setup(){pinMode(12,OUTPUT);pinMode(13,OUTPUT);digitalWrite(12,LOW);digitalWrite(13,LOW);}void loop(){if(millis()-last>=1000){last=millis();level=!level;digitalWrite(12,level);digitalWrite(13,level);}}',
      'embedbench_1':'void setup(){pinMode(2,OUTPUT);pinMode(3,OUTPUT);}void loop(){auto t=millis()%20000;digitalWrite(2,t>=5000&&t<15000);digitalWrite(3,t>=10000&&t<15000);}',
    }
    results={}
    for case,body in programs.items():
        folder=OUT/'self_check'/case
        source=folder/'sketch/sketch.ino';source.parent.mkdir(parents=True,exist_ok=True)
        code='#include <Arduino.h>\n#include <TM1637Display.h>\n#include <Servo.h>\n'+body
        source.write_text(code);cpp=folder/'build/sketch/sketch.ino.cpp';cpp.parent.mkdir(parents=True,exist_ok=True);cpp.write_text(code)
        row=dict(source=str(source),source_sha256=sha(source),board=specs[case]['board'])
        result=execute(row,specs[case],fixture,'reference_'+case)
        assert result['status']=='passed', (case,result)
        results[case]=result['status']
    bad=OUT/'self_check/wrong_threshold/sketch/sketch.ino'
    bad.parent.mkdir(parents=True,exist_ok=True)
    bad.write_text(programs['simple_024'].replace('>500','<=500'))
    cpp=bad.parent.parent/'build/sketch/sketch.ino.cpp';cpp.parent.mkdir(parents=True,exist_ok=True);cpp.write_text('#include <Arduino.h>\n'+bad.read_text())
    wrong=execute(dict(source=str(bad),source_sha256=sha(bad),board=specs['simple_024']['board']),specs['simple_024'],fixture,'reference_wrong_threshold')
    assert wrong['status']=='failed'
    results['wrong_threshold_rejected']=True
    save(OUT/'self_check/results.json',results)


def main():
    OUT.mkdir(parents=True,exist_ok=True)
    specs=specification()
    save(OUT/'test_specs_frozen.json',specs)
    fixture=make_fixture(specs)
    self_check(specs,fixture)
    rows=[r for r in json.loads(MANIFEST.read_text()) if r['case'] in IDS]
    assert len(rows)==130
    save(OUT/'frozen_manifest.json',rows)
    save(OUT/'provenance.json',dict(runner_sha256=sha(__file__),requirements_sha256=sha(INPUT),manifest_sha256=sha(MANIFEST),specs_sha256=sha(OUT/'test_specs_frozen.json'),selection='10 purposively selected observable tasks, NOT a random sample or held-out evaluation; none are the old 16 oracle tasks.'))
    jobs={}
    for row in rows:
        if not row['compile_ok']:continue
        cpp=Path(row['source']).parent.parent/'build/sketch/sketch.ino.cpp'
        key=hashlib.sha256(json.dumps([row['case'],row['source_sha256'],row['board'],sha(cpp)]).encode()).hexdigest()
        row['behavior_execution_key']=key;jobs.setdefault(key,row)
    results={};started=time.time()
    print(json.dumps(dict(selected=IDS,groups=13,rows=len(rows),compiled=sum(r['compile_ok'] for r in rows),unique_executions=len(jobs))),flush=True)
    with concurrent.futures.ThreadPoolExecutor(max_workers=8) as pool:
        futures={pool.submit(execute,row,specs[row['case']],fixture,key):key for key,row in jobs.items()}
        for future in concurrent.futures.as_completed(futures):
            key=futures[future];results[key]=future.result()
            if len(results)%10==0:print(json.dumps(dict(completed=len(results),total=len(jobs),counts=dict(collections.Counter(x['status'] for x in results.values())))),flush=True)
    mapped=[]
    for row in rows:
        item=dict(row)
        if not row['compile_ok']:item['behavior_status']='historical_compile_failed_not_run'
        else:
            result=results[row['behavior_execution_key']];item.update(behavior_status=result['status'],evidence=result['evidence'],failed_assertions=result.get('failed_assertions'))
        mapped.append(item)
    groups={g:dict(total=10,compiled=sum(r['compile_ok'] for r in mapped if r['group']==g),counts=dict(collections.Counter(r['behavior_status'] for r in mapped if r['group']==g))) for g in dict.fromkeys(r['group'] for r in mapped)}
    save(OUT/'execution_results.json',results);save(OUT/'mapped_results.json',mapped)
    summary=dict(groups=groups,unique_executions=len(jobs),seconds=round(time.time()-started,1),scope='Requirement-derived host behavioral checks; no SimulIDE/hardware. Purposive pilot, not an estimate for 296 tasks.')
    save(OUT/'summary.json',summary)
    print(json.dumps(summary,ensure_ascii=False),flush=True)


if __name__=='__main__':main()
