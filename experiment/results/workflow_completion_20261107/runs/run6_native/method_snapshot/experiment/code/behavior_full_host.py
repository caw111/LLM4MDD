"""Compile, execute and compare structured requirement tests against host API traces."""
import hashlib
import bisect
import collections
import json
import math
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import time

import run_behavior_pilot10 as pilot10
import run_workflow267_host_smoke as base
import behavior_full_support as support

ROOT=base.ROOT
OUT=ROOT/'experiment/output/behavior_all296_20261023'
FIXTURE=OUT/'fixture'


def save(path,value):
    path.parent.mkdir(parents=True,exist_ok=True)
    with tempfile.NamedTemporaryFile('w',encoding='utf-8',dir=path.parent,prefix=path.name+'.',suffix='.tmp',delete=False) as f:
        json.dump(value,f,ensure_ascii=False,indent=2);tmp=Path(f.name)
    try:
        for retry in range(40):
            try:tmp.replace(path);return
            except PermissionError:
                if retry==39:raise
                time.sleep(.05)
    finally:tmp.unlink(missing_ok=True)


def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def pin(value,board,analog=False):
    a0=54 if 'mega' in board else 14
    if isinstance(value,str) and value.upper().startswith('A'):return a0+int(value[1:])
    if isinstance(value,str) and value.upper().startswith('D'):return int(value[1:])
    n=int(value)
    return n+a0 if analog and 0<=n<(16 if a0==54 else 6) else n


MAIN = r'''
#include <Arduino.h>
#include <Servo.h>
#include <TM1637Display.h>
#include <Adafruit_ILI9341.h>
#include <fstream>
void setup();void loop();
extern "C" void yield() __attribute__((weak));extern "C" void yield(){}
std::string esc(const std::string&s){std::string r="\"";const char*h="0123456789abcdef";for(unsigned char c:s){if(c=='"'||c=='\\'){r+='\\';r+=c;}else if(c<32){r+="\\u00";r+=h[c>>4];r+=h[c&15];}else r+=c;}return r+'"';}
std::string tmtext(const oracle::DisplayEvent&e){std::string t;for(auto seg:e.segments)t+=e.on?pilot::digit(seg&127):' ';return t;}
int main(int argc,char**argv){bool initialized=false;uint64_t loops=0,setup_at=0;std::string status="completed";
 try{setup();initialized=true;setup_at=oracle::now;while(oracle::now<pilot::horizon){loop();++loops;delayMicroseconds(100);}}
 catch(const std::exception&e){status=e.what();}
 std::ofstream out(argv[1]);
 out<<"{\"type\":\"reset_state\",\"gpio_latch\":0,\"at_ms\":0}\n";
 for(auto&e:oracle::events)out<<"{\"type\":\"gpio\",\"sequence\":"<<e.sequence<<",\"at_ms\":"<<e.at<<",\"at_us\":"<<e.micro_at<<",\"pin\":"<<e.pin<<",\"value\":"<<e.level<<",\"pwm\":"<<(e.pwm?"true":"false")<<"}\n";
 for(auto&e:pilot::reads){out<<"{\"type\":\"input_read\",\"at_ms\":"<<e.at<<",\"pin\":"<<e.pin<<",\"kind\":"<<e.kind<<",\"value\":";if(std::isfinite(e.value))out<<e.value;else out<<"null";out<<"}\n";}
 for(auto&e:oracle::serial_lines)out<<"{\"type\":\"serial\",\"sequence\":"<<e.sequence<<",\"at_ms\":"<<e.at<<",\"baud\":"<<e.baud<<",\"text\":"<<esc(e.text)<<"}\n";
 for(auto&e:oracle::servos)out<<"{\"type\":\"servo\",\"sequence\":"<<e.sequence<<",\"at_ms\":"<<e.at<<",\"pin\":"<<e.pin<<",\"angle\":"<<e.angle<<",\"pulse_us\":"<<e.pulse<<",\"operation\":"<<esc(e.operation)<<",\"active\":"<<(e.active?"true":"false")<<"}\n";
 for(auto&e:oracle::tones)out<<"{\"type\":\"tone\",\"at_ms\":"<<e.at<<",\"pin\":"<<e.pin<<",\"frequency\":"<<e.frequency<<",\"until_ms\":"<<e.until<<"}\n";
 for(auto&e:oracle::displays){out<<"{\"type\":\"display\",\"at_ms\":"<<e.at<<",\"name\":\"tm:"<<e.clk<<":"<<e.dio<<"\",\"text\":"<<esc(tmtext(e))<<",\"enabled\":"<<(e.on?"true":"false")<<",\"segments\":[";bool first=true;for(auto v:e.segments){if(!first)out<<",";first=false;out<<(int)v;}out<<"]}\n";}
 for(auto&e:pilot::frames)out<<"{\"type\":\"display\",\"at_ms\":"<<e.at<<",\"name\":"<<esc(e.name)<<",\"text\":"<<esc(e.text)<<"}\n";
 for(auto&e:pilot::lcd)out<<"{\"type\":\"lcd\",\"at_ms\":"<<e.at<<",\"text\":"<<esc(e.text)
 <<",\"initialized\":"<<(e.initialized?"true":"false")<<",\"rotation\":"<<e.rotation<<",\"background\":"<<e.background
 <<",\"x\":"<<e.x<<",\"y\":"<<e.y<<",\"color\":"<<e.color<<",\"size\":"<<e.size
 <<",\"cs\":"<<e.cs<<",\"dc\":"<<e.dc<<",\"rst\":"<<e.rst<<"}\n";
 for(int i=0;i<PIN_COUNT;++i)out<<"{\"type\":\"pin_mode\",\"pin\":"<<i<<",\"value\":"<<oracle::modes[i]<<"}\n";
 out<<"{\"type\":\"execution\",\"setup_returned\":"<<(initialized?"true":"false")<<",\"setup_at_ms\":"<<setup_at<<",\"loop_returns\":"<<loops<<",\"virtual_ms\":"<<oracle::now<<",\"status\":"<<esc(status)<<"}\n";
 return status=="completed"||status=="capture-end"?0:3;
}
'''


def fixture():
    FIXTURE.mkdir(parents=True,exist_ok=True)
    h=(pilot10.OUT/'fixture/Arduino.h').read_text()
    h=h.replace('struct Event {', 'inline uint64_t event_sequence=0;\nstruct Event {',1)
    h=h.replace('uint64_t micro_at=0; };','uint64_t micro_at=0; uint64_t sequence=++event_sequence; };',1)
    h=h.replace('struct SerialEvent {uint64_t at;unsigned long baud;std::string text;};',
                'struct SerialEvent {uint64_t at;unsigned long baud;std::string text;uint64_t sequence=++event_sequence;};',1)
    h=h.replace('namespace oracle {\n// AVR-libc', 'namespace pilot {inline void gpio(int,int,bool);inline void mode(int,int);inline unsigned long pulse(uint8_t,uint8_t,unsigned long);}\nnamespace oracle {\n// AVR-libc',1)
    start=h.index('#include "case_input.h"');end=h.index('inline int analogRead',start)
    h=h[:start]+support.ENVIRONMENT+'\n'+h[end:]
    h=base.replace_function(h,'inline void digitalWrite(int pin,int level)', 'oracle::gpio_operation();pilot::gpio(pin,level,false);')
    h=base.replace_function(h,'inline void pinMode(int pin,int mode)', 'oracle::gpio_operation();if(pin<0||pin>=PIN_COUNT)throw std::runtime_error("pin-range");pilot::mode(pin,mode);')
    h=base.replace_function(h,'inline void analogWrite(int pin,int level)', '''
 if(level<0||level>255)throw std::runtime_error("pwm-range-unverified");pinMode(pin,OUTPUT);
 bool capable=false;
 #if defined(ARDUINO_AVR_MEGA2560)
 capable=(pin>=2&&pin<=13)||(pin>=44&&pin<=46);
 #else
 capable=pin==3||pin==5||pin==6||pin==9||pin==10||pin==11;
 #endif
 if(capable)pilot::gpio(pin,level,true);else pilot::gpio(pin,level>=128?HIGH:LOW,false);
''')
    h=base.replace_function(h,'inline unsigned long pulseIn(', 'return pilot::pulse(pin,state,timeout);')
    h=h.replace('return pilot::input(pin,false);','return pilot::digital(pin);')
    h=base.replace_function(h,'inline int digitalRead(int pin)', '''
 oracle::advance_clock();oracle::progress();if(pin<0||pin>=PIN_COUNT)throw std::runtime_error("digital-pin-range");
 if(oracle::modes[pin]==OUTPUT)return pilot::levels[pin]?HIGH:LOW;return pilot::digital(pin);
''')
    start=h.index('struct SerialStub {');end=h.index('inline SerialStub Serial;',start)
    old=h[start:end];tail=old[old.index(' void begin'):]
    h=h[:start]+support.ARDUINO_STRING+'\nstruct SerialStub {\n unsigned long baud=0;std::string pending;\n'+support.SERIAL_METHODS+tail+h[end:]
    h=h.replace('size_t write(uint8_t c){','size_t write(uint8_t c){\n if(!baud)return 0;')
    h+='\n#define PROGMEM\n#define PSTR(x) (x)\n#define pgm_read_byte(p) (*(const uint8_t*)(p))\n#define pgm_read_word(p) (*(const uint16_t*)(p))\n#ifndef PI\n#define PI 3.14159265358979323846\n#endif\n#define TWO_PI (2*PI)\n#define DEG_TO_RAD (PI/180.0)\n#define RAD_TO_DEG (180.0/PI)\n#define radians(x) ((x)*DEG_TO_RAD)\n#define degrees(x) ((x)*RAD_TO_DEG)\n#define sq(x) ((x)*(x))\n'
    (FIXTURE/'Arduino.h').write_text(h)
    servo=(pilot10.OUT/'fixture/Servo.h').read_text().replace('int pulse; bool active;', 'int pulse; bool active;double angle=0;const char*operation="detach";uint64_t sequence=++event_sequence;')
    servo=servo.replace('void record()', 'void record(const char*operation="write")')
    servo=servo.replace('pin_,pulse_,true}', 'pin_,pulse_,true,(pulse_-low_)*180.0/(high_-low_),operation}')
    servo=servo.replace('pinMode(pin,OUTPUT);record();return index;', 'pinMode(pin,OUTPUT);record("attach");return index;')
    if 'record("attach")' not in servo:raise ValueError('Servo fixture template does not expose attach/write operations')
    (FIXTURE/'Servo.h').write_text(servo)
    shutil.copyfile(pilot10.OUT/'fixture/TM1637Display.h',FIXTURE/'TM1637Display.h')
    for name,content in {'DHT.h':support.DHT,'Wire.h':support.WIRE,'RTClib.h':support.RTC,'Adafruit_ILI9341.h':support.LCD,'Adafruit_MPU6050.h':support.MPU}.items():(FIXTURE/name).write_text(content)
    for name in ('Adafruit_Sensor.h','MPU6050.h'):(FIXTURE/name).write_text('#pragma once\n#include <Adafruit_MPU6050.h>\n')
    (FIXTURE/'Adafruit_GFX.h').write_text('#pragma once\n#include <Arduino.h>\n')
    (FIXTURE/'SPI.h').write_text('#pragma once\n#include <Arduino.h>\nenum{SPI_MODE0=0,SPI_MODE1=1,SPI_MODE2=2,SPI_MODE3=3};struct SPISettings{SPISettings(unsigned long,uint8_t,uint8_t){}};struct SPIClass{void begin(){}void beginTransaction(SPISettings){}void endTransaction(){}uint8_t transfer(uint8_t){throw std::runtime_error("raw-spi-transfer-unverified");}};inline SPIClass SPI;\n')
    (FIXTURE/'main.cpp').write_text(MAIN)
    save(OUT/'fixture_hashes.json',{p.name:sha(p) for p in FIXTURE.iterdir()})
    return FIXTURE


def case_files(case,plan,board):
    folder=OUT/'test_inputs'/case;folder.mkdir(parents=True,exist_ok=True)
    def p(x,analog=False):return pin(x,board,analog)
    def literal(v):return 'NAN' if v is None else repr(float(v))
    kinds={'analog':0,'digital':1,'temperature':2,'humidity':3,'distance':4}
    inputs=','.join('{'+f"{e['at_ms']},{p(e['pin'],e['kind']=='analog')},{literal(e['value'])},{kinds[e['kind']]}"+'}' for e in sorted(plan.get('input_steps',[]),key=lambda e:e['at_ms']))
    serial=','.join('{'+f"{e['at_ms']},{json.dumps(e['text'],ensure_ascii=False)}"+'}' for e in sorted(plan.get('serial_inputs',[]),key=lambda e:e['at_ms']))
    ultra=','.join('{'+f"{p(e['trigger_pin'])},{p(e['echo_pin'])}"+'}' for e in plan.get('ultrasonic',[]))
    direct=','.join('{'+json.dumps(e['name'])+',{'+','.join(str(p(i)) for i in e['pins'])+'},'+str(e.get('common_anode',False)).lower()+'}' for e in plan.get('seven_segments',[]))
    buses=[]
    for b in plan.get('shift_registers',[]):
        displays=','.join('{'+json.dumps(d['name'])+','+str(d['register_index'])+',{'+','.join(map(str,d['segment_bits']))+'},'+str(d.get('common_anode',False)).lower()+'}' for d in b.get('displays',[]))
        buses.append('{'+f"{p(b['data_pin'])},{p(b['clock_pin'])},{p(b['latch_pin'])},{b['registers']},0,"+'{'+displays+'}}')
    multiplexers=[]
    for m in plan.get('analog_multiplexers',[]):
        select=','.join(str(p(pin_number)) for pin_number in m['select_pins'])
        samples=','.join('{'+f"{s['at_ms']},{s['channel']},{s['value']}"+'}' for s in
                         sorted(m['samples'],key=lambda sample:sample['at_ms']))
        multiplexers.append(f"{{{p(m['signal_pin'],True)},{{{select}}},{{{samples}}}}}")
    text='''#pragma once
namespace pilot {
struct Stimulus{uint64_t at;int pin;double value;int kind;};
struct SerialInput{uint64_t at;std::string text;};struct Ultra{int trigger,echo;};
struct DirectDisplay{std::string name;int pins[7];bool anode;};
struct ShiftDisplay{std::string name;int reg;int bits[7];bool anode;};
struct Bus{int data,clock,latch,registers;uint64_t bits;std::vector<ShiftDisplay>displays;};
struct MuxSample{uint64_t at;unsigned channel;int value;};
struct Multiplexer{int signal,select[4];std::vector<MuxSample>samples;};
struct ImuSample{uint64_t at;float acceleration[3],gyro[3];int raw_y;};
'''+f'inline std::vector<Stimulus> inputs={{{inputs}}};\ninline std::vector<SerialInput> serial_inputs={{{serial}}};\ninline std::vector<Ultra>ultrasonic={{{ultra}}};\ninline std::vector<DirectDisplay>direct_displays={{{direct}}};\ninline std::vector<Bus>buses={{{",".join(buses)}}};\ninline constexpr uint64_t horizon={plan["horizon_ms"]};\n'+'}\n'
    rtc_epoch=int(plan.get('rtc_epoch_s',1767225600))
    imu=[]
    for s in sorted(plan.get('imu_samples',[]),key=lambda sample:sample['at_ms']):
        vectors=','.join('{'+','.join(f'{float(value):.8f}f' for value in s.get(field,(0,0,0)))+'}'
                         for field in ('acceleration','gyro'))
        imu.append(f"{{{s['at_ms']},{vectors},{s.get('raw_y',0)}}}")
    text=text.removesuffix('}\n')+f'inline std::vector<Multiplexer>multiplexers={{{",".join(multiplexers)}}};\n'
    text+=f'inline constexpr uint32_t rtc_epoch={rtc_epoch}UL;\n'
    text+=f'inline std::vector<ImuSample>imu_samples={{{",".join(imu)}}};\n}}\n'
    (folder/'case_input.h').write_text(text,encoding='utf-8')
    (folder/'case_horizon.h').write_text(f'#define PILOT_HORIZON {plan["horizon_ms"]}UL\n')
    save(folder/'test_spec.json',plan)
    return folder


def evaluate(plan,trace,board):
    by={k:[e for e in trace if e['type']==k] for k in {e['type'] for e in trace}}
    execution=by['execution'][-1]
    state_checks=any(c['kind'] in ('state_witness','state_invariant','event_window') for c in plan['checks'])
    ordered=[e for e in trace if e['type'] in ('gpio','servo')]
    ordered.sort(key=lambda e:(e['at_ms'],e.get('sequence',0)))
    anchor_events=sorted(ordered+by.get('serial',[]),key=lambda e:(e['at_ms'],e.get('sequence',0)))
    ordered_metadata=bool(by.get('reset_state')) and all('sequence' in e for e in ordered)
    snapshots=[]
    if state_checks and ordered_metadata:
        state={('gpio',p):0 for p in range(70 if 'mega' in board else 20)}
        snapshots=[(0,dict(state))]
        for e in ordered:
            key=(e['type'],e['pin'])
            value=int(e['value']>0) if e['type']=='gpio' else e['angle'] if e.get('active') and e.get('operation')=='write' else None
            if state.get(key)!=value:
                state[key]=value;snapshots.append((e['at_ms'],dict(state)))
    def state_match(conditions,state):
        return all(within(condition,state.get((condition['source'],pin(condition['pin'],board)))) for condition in conditions)
    def command_anchor(definition,after=-1):
        previous=None;found=[]
        for i,e in enumerate(anchor_events):
            if e['type']!=definition['source']:continue
            if e['type']=='serial':
                if i>after and matching(definition,e):found.append(i)
                continue
            if e['pin']!=pin(definition['pin'],board):continue
            if e['type']=='servo' and (not e.get('active') or e.get('operation')!='write'):continue
            value=e['angle'] if e['type']=='servo' else int(e['value']>0)
            if previous is not None and abs(value-previous)<=definition.get('tolerance',0):continue
            previous=value
            if i>after and within(definition,value):found.append(i)
        n=definition.get('occurrence',1)
        return found[n-1] if len(found)>=n else None
    def last(events,t):
        events=[e for e in events if e.get('at_ms',0)<=t];return events[-1] if events else None
    def selected(kind,p=None):return [e for e in by.get(kind,[]) if p is None or e.get('pin')==p]
    def scalar(source,c,t):
        p=pin(c.get('pin',0),board)
        if source in ('gpio','pwm'):
            e=last(selected('gpio',p),t)
            if e is None:return None
            return (e['value'] if e['pwm'] else (255 if e['value'] else 0)) if source=='pwm' else int(e['value']>0)
        if source=='servo':
            e=last(selected('servo',p),t);return e['angle'] if e and e['active'] else None
        if source=='display':
            texts=[]
            for name in c['name'].split(','):
                e=last([e for e in by.get('display',[]) if e['name']==name.strip()],t)
                if not e:return None
                texts.append(e['text'])
            return ''.join(texts).strip()
        if source=='tone':
            e=last(selected('tone',p),t)
            if e and e['frequency'] and t<e['until_ms']:return e['frequency']
            events=selected('gpio',p);level=0;rises=[]
            for e in events:
                if e.get('pwm'):continue
                if e['value'] and not level and abs(e.get('at_us',e['at_ms']*1000)-t*1000)<=3000:rises.append(e.get('at_us',e['at_ms']*1000))
                level=int(e['value']>0)
            return (len(rises)-1)*1000000/(rises[-1]-rises[0]) if len(rises)>=2 and rises[-1]>rises[0] else 0
        if source=='pin_mode':return next((e['value'] for e in by.get('pin_mode',[]) if e['pin']==p),None)
        raise ValueError(source)
    def within(c,actual):
        if actual is None:return False
        if c.get('numeric'):
            try:actual=float(actual)
            except (ValueError,TypeError):return False
        if 'expected' in c:
            expected=c['expected']
            if c.get('numeric'):
                try:expected=float(expected)
                except (ValueError,TypeError):return False
            if isinstance(expected,(int,float)):
                try:return math.isfinite(float(actual)) and abs(float(actual)-expected)<=c.get('tolerance',0)
                except (ValueError,TypeError):return False
            return str(actual).strip()==str(expected).strip()
        return ('min' not in c or actual>=c['min']) and ('max' not in c or actual<=c['max'])
    def matching(c,event):
        if 'baud' in c and event.get('baud')!=c['baud']:return False
        if c.get('kind')=='lcd' and any(event.get(k)!=v for k,v in c.get('display_properties',{}).items()):return False
        text=event.get('text','')
        if 'prefix' in c and c['prefix'] not in text:return False
        if 'contains' in c and c['contains'] not in text:return False
        if 'regex' in c and not re.search(c['regex'],text):return False
        if 'expected' in c:
            if isinstance(c['expected'],(int,float)):return any(abs(v-c['expected'])<=c.get('tolerance',0) for v in pilot10.numbers(text))
            return str(c['expected']) in text
        return True
    def edges(c):
        p=pin(c['pin'],board);result=[];level=0
        for e in selected('gpio',p):
            current=int(e['value']>0)
            if current!=level:
                if c.get('edge','both')=='both' or c.get('edge')==('rising' if current else 'falling'):result.append(e)
            level=current
        # tone() represents a timer-driven carrier; reconstruct its ideal host waveform.
        segments=[];active=None
        for e in selected('tone',p):
            t=e['at_ms'];freq=e['frequency'];end=min(e['until_ms'],plan['horizon_ms'])
            if active and active[2]==freq and t<=active[1]:active[1]=end;continue
            if active:
                active[1]=min(active[1],t)
                if active[1]>active[0]:segments.append(active)
            active=[t,end,freq] if freq else None
        if active and active[1]>active[0]:segments.append(active)
        lo=c.get('from_ms',0);hi=c.get('to_ms',plan['horizon_ms'])
        for a,b,freq in segments:
            step=500.0/freq;start=max(0,int(math.ceil((lo-a)/step)));end=int(math.floor((min(b,hi)-a)/step))
            if end-start>500000:raise ValueError('tone edge observation budget exceeded')
            for n in range(start,end+1):
                value=1 if n%2==0 else 0
                if c.get('edge','both')=='both' or c.get('edge')==('rising' if value else 'falling'):result.append(dict(at_ms=a+n*step,value=value))
        result.sort(key=lambda e:e['at_ms'])
        return result
    checks=[]
    for original in plan['checks']:
        c=dict(original)
        if c.get('origin')=='setup_complete':
            if not execution.get('setup_returned') or 'setup_at_ms' not in execution:
                checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,
                                   actual=None,passed=False,unverified='setup_anchor_unavailable'))
                continue
            offset=execution['setup_at_ms']
            if c['kind'] in ('serial','lcd','edges','interval'):
                c.setdefault('from_ms',0);c.setdefault('to_ms',plan['horizon_ms'])
            for key in ('at_ms','from_ms','to_ms'):
                if key in c:
                    c[key]=[t+offset for t in c[key]] if isinstance(c[key],list) else c[key]+offset
            end=max(([c['at_ms']] if not isinstance(c.get('at_ms'),list) else c['at_ms']) if 'at_ms' in c else [c.get('to_ms',offset)])
            if end>execution['virtual_ms']:
                checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,
                                   actual=None,passed=False,unverified='setup_relative_window_not_observed'))
                continue
        observation_end=max(c['at_ms']) if isinstance(c.get('at_ms'),list) else c.get('at_ms',c.get('to_ms',0))
        if observation_end>execution['virtual_ms']:
            checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,
                actual=None,passed=False,unverified='requested_window_not_observed'))
            continue
        kind=c['kind'];ok=False;actual=None
        if kind=='event_order':
            if any('sequence' not in e for e in anchor_events):
                checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,actual=None,
                    passed=False,unverified='event_order_metadata_unavailable'))
                continue
            first=command_anchor(c['first']);second=command_anchor(c['second'])
            actual=dict(first=anchor_events[first] if first is not None else None,second=anchor_events[second] if second is not None else None)
            ok=first is not None and second is not None and first<second
            if first is not None and second is not None:
                gap=anchor_events[second]['at_ms']-anchor_events[first]['at_ms'];actual['gap_ms']=gap
                if c.get('allow_same_ms') and gap==0:ok=True
                ok=ok and gap>=c.get('min_gap_ms',0) and gap<=c.get('max_gap_ms',float('inf'))
        elif kind=='event_window':
            if not ordered_metadata:
                checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,actual=None,
                    passed=False,unverified='ordered_state_metadata_unavailable'))
                continue
            start=command_anchor(c['start']);end=command_anchor(c['end'],start) if start is not None else None
            if start is None or end is None:actual=dict(reason='required_phase_anchor_missing')
            else:
                lo=anchor_events[start]['at_ms']+c.get('start_delay_ms',0)
                hi=anchor_events[end]['at_ms']-c.get('end_margin_ms',0)
                actual=dict(start_at_ms=anchor_events[start]['at_ms'],end_at_ms=anchor_events[end]['at_ms'],observed_from_ms=lo,observed_to_ms=hi)
                if lo<=hi:
                    frames={t:s for t,s in snapshots};times=sorted(frames)
                    probes=sorted({lo,hi}|{t for t in times if lo<=t<=hi})
                    violations=[t for t in probes if not state_match(c['conditions'],frames[times[max(0,bisect.bisect_right(times,t)-1)]])]
                    actual.update(observations=len(probes),violation_times_ms=violations[:20]);ok=not violations
                else:actual['reason']='phase_window_empty_or_reversed'
        elif kind in ('state_witness','state_invariant'):
            lo,hi=c.get('from_ms',0),c.get('to_ms',plan['horizon_ms'])
            if not ordered_metadata or hi>execution['virtual_ms']:
                checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,actual=None,
                    passed=False,unverified='ordered_state_metadata_or_window_unavailable'))
                continue
            times=[t for t,_ in snapshots]
            start=max(0,bisect.bisect_left(times,lo)-1)
            observations=[(lo,snapshots[start][1])]+[(t,s) for t,s in snapshots[start+1:] if lo<=t<=hi]
            if kind=='state_witness':
                matches=[t for t,s in observations if state_match(c['conditions'],s)]
                actual=dict(witness_at_ms=matches[0] if matches else None);ok=bool(matches)
            else:
                # Cross-pin writes within a millisecond are observed as one settled API state.
                settled={t:s for t,s in observations};settled[hi]=snapshots[max(0,bisect.bisect_right(times,hi)-1)][1]
                for t in list(settled):
                    probe=t+c.get('settle_ms',0)
                    if probe<=hi:settled[probe]=snapshots[max(0,bisect.bisect_right(times,probe)-1)][1]
                guarded_since=None;violations=[];count=0
                for t,s in sorted(settled.items()):
                    if not state_match(c.get('when',[]),s):guarded_since=None;continue
                    if guarded_since is None:guarded_since=t
                    if t-guarded_since<c.get('settle_ms',0):continue
                    count+=1
                    if not state_match(c['conditions'],s):violations.append(t)
                actual=dict(observations=count,violation_times_ms=violations[:20])
                ok=count>=c.get('min_observations',1) and not violations
        elif kind=='gpio_count':
            levels=[scalar('gpio',dict(c,pin=p),c['at_ms']) for p in c['pins']]
            actual=None if any(v is None for v in levels) else sum(v==int(c.get('active_high',True)) for v in levels)
            ok=within(c,actual)
        elif kind=='display_gpio_count':
            actual=[]
            for t in c['at_ms']:
                levels=[scalar('gpio',dict(c,pin=p),t) for p in c['pins']]
                actual.append((scalar('display',c,t),None if any(v is None for v in levels) else sum(levels)))
            ok=bool(actual) and all(count is not None and digit==str(count) for digit,count in actual)
        elif kind=='distinct_gpio_patterns':
            patterns={tuple(scalar('gpio',dict(c,pin=p),t) for p in c['pins']) for t in c['at_ms']}
            actual=len({pattern for pattern in patterns if None not in pattern})
            ok=within(c,actual)
        elif kind=='segment_frame':
            frames=[e for e in by.get('display',[]) if e.get('name')==c['name'] and
                    c.get('from_ms',0)<=e['at_ms']<=c.get('to_ms',plan['horizon_ms']) and e.get('segments')==c['segments'] and e.get('enabled',True)]
            actual=dict(count=len(frames),examples=frames[:3]);ok=len(frames)>=c.get('min_count',1)
        elif kind=='multiplex_display':
            glyphs={mask:str(i) for i,mask in enumerate((63,6,91,79,102,109,125,7,127,111))}
            slots=[collections.Counter() for _ in c['digit_pins']]
            for t in range(c['from_ms'],c['to_ms']+1):
                active=[i for i,p in enumerate(c['digit_pins']) if scalar('gpio',dict(c,pin=p),t)==1]
                if len(active)!=1:continue
                segments=[scalar('gpio',dict(c,pin=p),t) for p in c['segment_pins']]
                if None in segments:continue
                bits=sum(1<<i for i,value in enumerate(segments) if value==0)
                slots[active[0]][glyphs.get(bits,'?')]+=1
            actual=''.join(slot.most_common(1)[0][0] if slot else '?' for slot in slots)
            ok=actual==c['expected'] and all(slot[char]>=3 for slot,char in zip(slots,actual))
        elif kind=='command_sequence':
            events=selected(c['source'],pin(c['pin'],board))
            if c['source']=='servo':
                events=[e for e in events if e.get('active')]
                if any('operation' not in e for e in events):
                    checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,actual=None,
                        passed=False,unverified='command_operation_metadata_missing'))
                    continue
                events=[e for e in events if e['operation']=='write']
            values=[]
            for e in events:
                if not c.get('from_ms',0)<=e['at_ms']<=c.get('to_ms',plan['horizon_ms']):continue
                value=e['angle'] if c['source']=='servo' else e['value']
                if not values or abs(value-values[-1]['value'])>c.get('tolerance',0):
                    values.append(dict(value=value,at_ms=e['at_ms']))
            expected=c['values'];prefix=values[:len(expected)]
            ok=len(prefix)==len(expected) and all(abs(v['value']-x)<=c.get('tolerance',0) for v,x in zip(prefix,expected))
            intervals=[b['at_ms']-a['at_ms'] for a,b in zip(prefix,prefix[1:])]
            for field,compare in [('min_intervals_ms',lambda a,b:a>=b),('max_intervals_ms',lambda a,b:a<=b)]:
                if field in c:ok=ok and all(bound is None or compare(dt,bound) for dt,bound in zip(intervals,c[field]))
            actual=dict(commands=prefix,intervals_ms=intervals)
        elif kind=='setup':
            actual=dict(returned=execution.get('setup_returned'),at_ms=execution.get('setup_at_ms'))
            ok=actual['returned'] is True and actual['at_ms'] is not None and actual['at_ms']<=c['max_ms']
        elif kind in ('gpio','pwm','servo','display','tone','pin_mode'):
            actual=scalar(kind,c,c.get('at_ms',plan['horizon_ms']));ok=within(c,actual)
        elif kind in ('serial','lcd'):
            entries=[e for e in by.get(kind,[]) if c.get('from_ms',0)<=e['at_ms']<=c.get('to_ms',plan['horizon_ms']) and matching(c,e)]
            actual=dict(count=len(entries),examples=entries[:3]);ok=len(entries)>=c.get('min_count',1) and ('max_count' not in c or len(entries)<=c['max_count'])
        elif kind=='edges':
            events=[e for e in edges(c) if c.get('from_ms',0)<=e['at_ms']<=c.get('to_ms',plan['horizon_ms'])]
            actual=dict(count=len(events),times_ms=[e['at_ms'] for e in events[:20]])
            ok=len(events)>=c.get('min_count',0) and ('max_count' not in c or len(events)<=c['max_count'])
        elif kind=='interval':
            events=edges(c) if c['source']=='gpio' else [e for e in by.get('serial',[]) if matching(c,e)]
            events=[e for e in events if c.get('from_ms',0)<=e['at_ms']<=c.get('to_ms',plan['horizon_ms'])]
            intervals=[b['at_ms']-a['at_ms'] for a,b in zip(events,events[1:])]
            actual=dict(count=len(events),intervals_ms=intervals[:40]);ok=len(events)>=c.get('min_count',3) and bool(intervals) and all(abs(dt-c['expected_ms'])<=c.get('tolerance_ms',0) for dt in intervals)
        elif kind=='relation':
            actual=[scalar(c['source'],c,t) for t in c['at_ms']]
            try:
                values=list(map(float,actual));pairs=list(zip(values,values[1:]));tol=c.get('tolerance',0)
                ok=bool(pairs) and all(math.isfinite(v) for v in values) and all({'increasing':b>a+tol,'decreasing':b<a-tol,'equal':abs(a-b)<=tol,'different':abs(a-b)>tol}[c['relation']] for a,b in pairs)
            except (ValueError,TypeError):ok=False
        else:raise ValueError('Unsupported check kind '+kind)
        checks.append(dict(id=c['id'],requirement_quote=c['requirement_quote'],test=c,actual=actual,passed=bool(ok)))
    state=('failed' if any(not c['passed'] and not c.get('unverified') for c in checks) else
           'unverified_window' if any(c.get('unverified') for c in checks) else
           'passed' if checks else 'unverified_no_checks')
    # Missing high-level frames can be an observation limitation for a bit-banged driver.
    for c in checks:
        if c['test']['kind']=='servo' and c['actual'] is None:
            p=pin(c['test']['pin'],board)
            if selected('gpio',p):state='unverified_observer'
        if c['test']['kind']=='display' and c['actual'] is None and c['test'].get('name','').startswith('tm:'):
            pins=[int(v) for v in c['test']['name'].split(':')[1:]]
            if any(e['pin'] in pins for e in by.get('gpio',[])):state='unverified_observer'
    if execution['status'] not in ('completed','capture-end'):state='unverified_runtime'
    return dict(status=state,checks=checks,failed_assertions=sum(not c['passed'] for c in checks),execution=execution)


def execute(cpp,board,case,plan,folder,reference=False):
    folder.mkdir(parents=True,exist_ok=True)
    config=OUT/'test_inputs'/case
    macro='ARDUINO_AVR_MEGA2560' if 'mega' in board else 'ARDUINO_AVR_UNO'
    cmd=[str(base.GXX),'-std=c++17','-O0','-D'+macro,'-DARDUINO=10819','-DF_CPU=16000000L','-I'+str(FIXTURE),'-I'+str(config),str(cpp),str(FIXTURE/'main.cpp'),'-o',str(folder/'program.exe')]
    result=dict(cpp=str(cpp),cpp_sha256=sha(cpp),board=board,case=case,reference=reference,evidence=str(folder),build_command=cmd)
    try:
        build=subprocess.run(cmd,capture_output=True,timeout=90)
        (folder/'build.stderr.txt').write_bytes(build.stderr)
        if build.returncode:result.update(status='unverified_host_build')
        else:
            run=subprocess.run([str(folder/'program.exe'),str(folder/'trace.jsonl')],capture_output=True,timeout=20)
            (folder/'stdout.txt').write_bytes(run.stdout);(folder/'stderr.txt').write_bytes(run.stderr)
            if not (folder/'trace.jsonl').exists():result.update(status='unverified_process_exit',exit_code=run.returncode)
            else:
                trace=[json.loads(line) for line in (folder/'trace.jsonl').read_text().splitlines()]
                result.update(evaluate(plan,trace,board),process_exit_code=run.returncode,trace_rows=len(trace))
    except subprocess.TimeoutExpired:result.update(status='unverified_timeout')
    except Exception as exc:result.update(status='unverified_harness',error_type=type(exc).__name__,error=str(exc)[:500])
    save(folder/'result.json',result)
    return result
