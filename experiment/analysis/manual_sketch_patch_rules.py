"""Explicit per-task sketch corrections. No expected outputs or reference code used."""
import re
import json
from pathlib import Path


def once(code, old, new):
    assert code.count(old)==1,(old,code.count(old))
    return code.replace(old,new,1)


def body(code, replacement):
    start=code.index('void ProgramPage1(',code.index('/* Code of ProgramPage1 */'))
    lo=code.index('{',start)
    depth=1;hi=lo+1
    while depth:
        depth += (code[hi]=='{')-(code[hi]=='}');hi+=1
    return code[:lo+1]+'\n'+replacement+'\n'+code[hi-1:]


def append_body(code, extra):
    start=code.index('void ProgramPage1(',code.index('/* Code of ProgramPage1 */'))
    lo=code.index('{',start);depth=1;hi=lo+1
    while depth:
        depth+=(code[hi]=='{')-(code[hi]=='}');hi+=1
    return code[:hi-1]+extra+'\n'+code[hi-1:]


def debounce_inputs(code, milliseconds, release_immediate=False, grounded=()):
    for pin in grounded:
        code=code.replace(f'digitalRead({pin}) == HIGH',f'digitalRead({pin}) == LOW')
        code=code.replace(f'pinMode({pin}, INPUT);',f'pinMode({pin}, INPUT_PULLUP);')
    helper='''static bool manual_debounce_input(bool raw, uint8_t index) {
  static bool previous[8]={}, stable[8]={};
  static uint32_t changed[8]={};
  uint32_t now=millis();
  if(raw!=previous[index]) {previous[index]=raw;changed[index]=now;}
  if((uint32_t)(now-changed[index])>=DEBOUNCE_MS) stable[index]=raw;
  return stable[index];
}
'''.replace('DEBOUNCE_MS',str(milliseconds)+'UL')
    if release_immediate:
        helper=helper.replace('  return stable[index];','  if(!raw) stable[index]=false;\n  return stable[index];')
    pattern=r'(hw_value_io_\d+) = (\(digitalRead\([^)]+\) == (?:HIGH|LOW)\));'
    found=list(re.finditer(pattern,code));assert 0<len(found)<=8
    for index,match in reversed(list(enumerate(found))):
        code=code[:match.start()]+match[1]+' = manual_debounce_input('+match[2]+', '+str(index)+');'+code[match.end():]
    return once(code,'void setup() {',helper+'\nvoid setup() {')


def loop_body(code,replacement,name='loop'):
    start=code.index(f'void {name}() {{');lo=code.index('{',start);depth=1;hi=lo+1
    while depth:
        depth+=(code[hi]=='{')-(code[hi]=='}');hi+=1
    return code[:lo+1]+'\n'+replacement+'\n'+code[hi-1:]


def repair(case, code):
    why=None
    if case=='advanced_003':
        code=body(code,'''    static uint8_t selection=0;
    static bool previous=false;
    static int count=0;
    uint32_t now=millis();
    if (io_6_valid && io_6==1) selection=1;
    if (io_6_valid && io_6==2) selection=2;
    if (io_6_valid && io_6==3) {count=0;selection=0;}
    bool night=selection==0 && io_2<300;
    bool armed=selection==1 || night;
    bool alarm=armed && io_0 && io_1;
    if (alarm && !previous && count<9999) ++count;
    previous=alarm;
    if (io_6_valid && io_6==4) {
        Serial.print("mode: ");
        Serial.println(night ? "NIGHT" : (armed ? "ARMED" : "DISARMED"));
        Serial.print("events: ");
        Serial.println(count);
    }
    *io_3=alarm && now%1000<450 ? 1000 : 0;
    *io_4=alarm;
    *io_5=count;''')
        why='Honor the four parsed commands, count each armed door/motion coincidence once, and distinguish automatic nighttime arming from explicit modes.'
    elif case=='advanced_005':
        code=debounce_inputs(code,25,grounded=(2,3))
        code=body(code,'''    static bool previousRoom=false,previousMetric=false,manual=false;
    static bool previousLivingAlarm=false,previousBedroomAlarm=false;
    static uint8_t room=0,metric=0;
    static uint32_t changed=0,lastReport=0;
    uint32_t now=millis();
    if (manual && (uint32_t)(now-changed)>=8000) manual=false;
    if (!manual) {
        unsigned page=now/2000UL%6;
        room=page/3;
        metric=page%3;
    }
    if (io_8 && !previousRoom) {room=1-room;manual=true;changed=now;}
    if (io_9 && !previousMetric) {metric=(metric+1)%3;manual=true;changed=now;}
    previousRoom=io_8;
    previousMetric=io_9;
    bool living=io_2 && io_0>32 && io_6>700;
    bool bedroom=io_5 && io_3>32 && io_7>700;
    *io_10=living;
    *io_11=bedroom;
    double data[][3]={{io_0,io_1,io_6},{io_3,io_4,io_7}};
    *io_12=metric==2 || (room ? io_5 : io_2) ? (int32_t)data[room][metric] : -1000;
    *serial_0_trigger=(uint32_t)(now-lastReport)>=5000;
    if (*serial_0_trigger) lastReport=now;
    *serial_0_value_0=io_2 ? io_0 : NAN;
    *serial_0_value_1=io_2 ? io_1 : NAN;
    *serial_0_value_2=io_6;
    *serial_0_value_3=io_5 ? io_3 : NAN;
    *serial_0_value_4=io_5 ? io_4 : NAN;
    *serial_0_value_5=io_7;
    *serial_1_trigger=(living && !previousLivingAlarm) || (bedroom && !previousBedroomAlarm);
    *serial_1_value_0=living && !previousLivingAlarm;
    *serial_1_value_1=bedroom && !previousBedroomAlarm;
    previousLivingAlarm=living;
    previousBedroomAlarm=bedroom;''')
        why='Cycle two rooms and three metrics, allow debounced manual selection, emit all six measurements every five seconds, and raise independent room-level alarms.'
    elif case=='advanced_007':
        code=once(code,'pinMode(2, INPUT);','pinMode(2, INPUT_PULLUP);')
        code=body(code,'''    static bool cooling=false,reported=false;
    static uint32_t opened=0,lastReport=0;
    uint32_t now=millis();
    double temperature=io_0*500.0/1023.0;
    if (temperature>=8) cooling=true;
    if (temperature<=6) cooling=false;
    if (io_1 && !sysml_state_prevDoorOpen) opened=now;
    sysml_state_prevDoorOpen=io_1;
    uint32_t duration=io_1 ? (uint32_t)(now-opened) : 0;
    bool alarm=duration>10000;
    bool critical=io_1 && temperature>8;
    *serial_0_trigger=critical && (!reported || (uint32_t)(now-lastReport)>=1000);
    if (*serial_0_trigger) lastReport=now;
    reported=critical;
    *serial_0_value_0=temperature;
    *io_2=cooling;
    *io_3=alarm ? 1000 : 0;
    *io_4=now/2000%2 ? (int32_t)min(duration/1000,(uint32_t)9999) : (int32_t)temperature;
    *io_5=alarm && (duration-10000)%1000<500;''')
        code=once(code,'Serial.print("temperature");','Serial.print("critical cooling loss: ");')
        why='Convert LM35 ADC to Celsius, apply 6–8 Celsius hysteresis, measure elapsed open time, and implement both alarm paths. D2 is closed to ground when the door is closed.'
    elif case=='advanced_008':
        code=debounce_inputs(code,25,grounded=(2,3,7))
        code=body(code,'''    static bool previous[3]={},running=false,active=false,complete=false;
    static uint8_t selected=0,stage=0,pulses=0;
    static uint32_t duration[3]={2000,2000,2000},measured[3]={},last=0,beganBeep=0;
    uint32_t now=millis();
    uint32_t elapsed=(uint32_t)(now-last);
    last=now;
    bool report=false;
    if (running) {
        measured[stage]+=elapsed;
        if (measured[stage]>=duration[stage]) {
            pulses=stage+1;
            beganBeep=now;
            if (++stage==3) {
                active=running=false;
                complete=report=true;
            }
        }
    }
    if (!active) {
        if (io_1 && !previous[0] && duration[selected]<9999000UL) {duration[selected]+=1000;complete=false;}
        if (io_2 && !previous[1]) {selected=(selected+1)%3;complete=false;}
    }
    if (io_3 && !previous[2]) {
        if (!active) {
            for (uint8_t i=0;i<3;++i) measured[i]=0;
            stage=0;
            complete=false;
            active=running=true;
        } else running=!running;
    }
    previous[0]=io_1;
    previous[1]=io_2;
    previous[2]=io_3;
    uint32_t beepTime=(uint32_t)(now-beganBeep);
    if (beepTime>=pulses*300UL) pulses=0;
    *io_0=complete ? 0 : (int32_t)((active ? duration[stage]-measured[stage]+999 : duration[selected])/1000);
    *io_4=pulses && beepTime%300<150 ? 1000 : 0;
    *io_5=complete;
    *serial_0_trigger=report;
    if (report) Serial.println("process complete");
    *serial_0_value_0=measured[0];
    *serial_0_value_1=measured[1];
    *serial_0_value_2=measured[2];''')
        why='Set and run three independent stages, pause the elapsed-time accounting, signal each transition with its own pulse count, and report all three measured durations.'
    elif case=='advanced_011':
        code=body(code,'''    static bool previous=false;
    static uint32_t last=0;
    uint32_t now=millis();
    double temperature=io_0*500.0/1023.0;
    bool hot=temperature>=50,smoky=io_1>=550;
    int level=io_2 || (hot && smoky) ? 2 : (hot || smoky ? 1 : 0);
    bool alarm=level==2;
    *io_3=alarm && now%1000<500 ? 1000 : 0;
    *io_4=alarm;
    *io_5=level==0;
    unsigned page=now/2000%3;
    *io_6=page==0 ? level : (page==1 ? (int32_t)temperature : (int32_t)io_1);
    *serial_0_trigger=alarm && (!previous || (uint32_t)(now-last)>=1000);
    if (*serial_0_trigger) {
        last=now;
        Serial.println("fire alert");
    }
    previous=alarm;
    *serial_0_value_0=temperature;
    *serial_0_value_1=io_1;
    *serial_0_value_2=io_2;
    *serial_0_value_3=level;''')
        why='Calibrate temperature, distinguish one exceeded sensor from a confirmed alarm, and make flame detection independently escalate.'
    elif case=='advanced_013':
        code=body(code,'''    static uint32_t vacantSince[3]={},filledAt=0;
    static bool reported[3]={},wasFull=false,beeping=false;
    bool occupied[]={io_0,io_1,io_2};
    uint32_t now=millis();
    int count=(int)io_0+(int)io_1+(int)io_2;
    bool full=count==3;
    bool reset=io_7_valid && io_7==2;
    *io_8=false;
    for (uint8_t i=0; i<3; ++i) {
        if (occupied[i] || reset) {vacantSince[i]=now;reported[i]=false;}
        else if (!reported[i] && (uint32_t)(now-vacantSince[i])>60000) {
            *io_8=true;
            reported[i]=true;
        }
    }
    if (full && !wasFull) {filledAt=now;beeping=true;}
    if (!full || (uint32_t)(now-filledAt)>=250) beeping=false;
    wasFull=full;
    *io_3=count;
    *io_4=beeping ? 1000 : 0;
    *io_5=full;
    *serial_0_trigger=io_7_valid && io_7==1;
    *serial_0_value_0=io_0;
    *serial_0_value_1=io_1;
    *serial_0_value_2=io_2;''')
        why='Track the three vacancy durations independently, suppress duplicate restock messages, preserve the 60-second timeout, and use the physical full state for the capacity lock and short buzzer cue.'
    elif case=='advanced_014':
        code=debounce_inputs(code,25,grounded=(3,))
        code=body(code,'''    static uint8_t phase=1;
    static bool previous=false,acknowledged=false,reported=false,moving=false;
    static uint32_t idleSince=0;
    uint32_t now=millis();
    bool water=io_5>=350;
    if (io_4 || moving!=io_4) idleSince=now;
    moving=io_4;
    if (phase==1 && water && io_4) phase=2;
    if (phase==2 && !water && io_4) phase=3;
    if (phase==3 && !io_4 && (uint32_t)(now-idleSince)>=2000) phase=4;
    if (phase==4 && io_0 && !previous) acknowledged=true;
    previous=io_0;
    bool fault=phase!=4 && water && !io_4 && (uint32_t)(now-idleSince)>=8000;
    *serial_0_trigger=fault && !reported;
    reported=fault;
    *serial_0_value_0=phase;
    bool alarm=phase==4 && !acknowledged;
    *io_1=alarm && now%1000<500;
    *io_2=alarm && now%1000<500 ? 1000 : 0;
    *io_3=phase;''')
        why='Recognize fill, wash, spin and end in order; hold completion alarm until acknowledgment and report prolonged abnormal high water without vibration.'
    elif case=='advanced_015':
        code=body(code,'''    static bool previous=false,open=false,overflow=false;
    static uint32_t opened=0;
    static uint8_t used=0;
    static char command[7];
    static int uses=0;
    uint32_t now=millis();
    double percent=io_3 ? constrain((45-io_2)*2.5,0.0,100.0) : 0;
    bool low=!io_3 || percent<20;
    if (low || (uint32_t)(now-opened)>=3000) open=false;
    if (io_0 && !previous && !low && !open) {
        open=true;
        opened=now;
        if (uses<9999) ++uses;
    }
    previous=io_0;
    *serial_0_trigger=false;
    for (uint8_t budget=0;budget<32 && Serial.available();++budget) {
        char c=(char)Serial.read();
        if (c=='\\r' || c=='\\n') {
            command[used]='\\0';
            if (!overflow && strcmp(command,"STATUS")==0) *serial_0_trigger=true;
            used=0;
            overflow=false;
        } else if (used<sizeof(command)-1) command[used++]=c;
        else overflow=true;
    }
    *io_1=open;
    *io_4=now/2000%2 ? uses : (int32_t)percent;
    *io_5=low && now%1000<400 ? 1000 : 0;
    *io_6=low;
    *serial_0_value_0=uses;
    *serial_0_value_1=percent;''')
        code=code.replace('#include <Arduino.h>','#include <Arduino.h>\n#include <string.h>',1)
        why='Start one three-second dispense per approach, block or cancel it on unsafe water readings, and expose the counter and level to serial queries.'
    elif case=='advanced_017':
        code=debounce_inputs(code,25,grounded=(2,3,7,8,9))
        code=body(code,'''    static bool previous[4]={},clearDone=false,beeping=false;
    static uint32_t wins[4]={},resetStart=0,winTime=0;
    bool buttons[]={io_0,io_1,io_2,io_3};
    uint32_t now=millis();
    *serial_0_trigger=false;
    if (io_4) {
        if (!sysml_state_resetHeld) {
            resetStart=now;
            sysml_state_playerId=0;
            beeping=false;
            clearDone=false;
        }
        if (!clearDone && (uint32_t)(now-resetStart)>=3000) {
            for (uint8_t i=0; i<4; ++i) wins[i]=0;
            Serial.println("stats cleared");
            clearDone=true;
        }
    } else if (sysml_state_playerId==0) {
        for (uint8_t i=0; i<4; ++i) if (buttons[i] && !previous[i]) {
            sysml_state_playerId=i+1;
            ++wins[i];
            Serial.print("player wins: ");
            Serial.println(wins[i]);
            winTime=now;
            beeping=true;
            *serial_0_trigger=true;
            break;
        }
    }
    for (uint8_t i=0; i<4; ++i) previous[i]=buttons[i];
    sysml_state_resetHeld=io_4;
    if (beeping && (uint32_t)(now-winTime)>=200) beeping=false;
    *io_5=sysml_state_playerId!=0;
    *io_6=beeping ? 1000 : 0;
    *io_7=sysml_state_playerId;
    *serial_0_value_0=(int32_t)winTime;
    *serial_0_value_1=sysml_state_playerId;''')
        why='Capture the first debounced winner once, report both timestamp and ID, lock other buttons, pulse the buzzer, and clear the cumulative win counters only on a three-second reset.'
    elif case=='advanced_019':
        code=body(code,'''    static bool previous=false,candidate=false;
    static uint32_t entered=0,firstOverlap=0;
    uint32_t now=millis();
    bool overlap=io_1 && io_3 && io_0<50 && io_2<30;
    if (overlap && !candidate) firstOverlap=now;
    candidate=overlap;
    bool risk=overlap && (uint32_t)(now-firstOverlap)>=120;
    int level=risk ? (io_0<25 && io_2<15 ? 2 : 1) : 0;
    if (risk && !previous) entered=firstOverlap;
    *serial_0_trigger=!risk && previous;
    if (*serial_0_trigger) *serial_0_value_0=(int32_t)(now-entered);
    previous=risk;
    *io_4=risk && now%1000<500;
    *io_5=!risk;
    *io_6=risk ? 1000 : 0;
    *io_7=level;
    *io_8=false;''')
        why='Require both valid ultrasonic distances for a hazard, derive the warning level, and report elapsed time only when the hazard ends.'
    elif case=='advanced_023':
        code=code.replace('#include <Arduino.h>','#include <Arduino.h>\n#include <stdlib.h>',1)
        code=code.replace('/* Code of ProgramPage1 */','''static double manualLatitude=0,manualLongitude=0;
static bool manualLocationValid=false;
/* Code of ProgramPage1 */''',1)
        code=once(code,'  if (!*begin) return false;','''  if (!*begin) return false;
  if (strncmp(begin,"GPS:",4)==0) {
    char* middle;
    char* last;
    double latitude=strtod(begin+4,&middle);
    if (middle==begin+4 || *middle!=',') return false;
    double longitude=strtod(middle+1,&last);
    if (last==middle+1 || *last || !isfinite(latitude) || !isfinite(longitude) ||
        latitude < -90 || latitude > 90 || longitude < -180 || longitude > 180) return false;
    manualLatitude=latitude;
    manualLongitude=longitude;
    manualLocationValid=true;
    *value=0;
    return true;
  }''')
        code=body(code,'''    static bool prevWarm=false,prevDoor=false,prevShock=false;
    static int events=0;
    uint32_t now=millis();
    double temperature=io_0*500.0/1023.0;
    bool warm=temperature>8;
    bool rising[]={warm && !prevWarm,io_2 && !prevDoor,io_1 && !prevShock};
    const char *labels[]={"temperature high","box opened","shock"};
    for (uint8_t i=0;i<3;++i) if (rising[i]) {
        if (events<9999) ++events;
        Serial.print(labels[i]);
        Serial.print(" GPS: ");
        if (manualLocationValid) {
            Serial.print(manualLatitude,4);
            Serial.print(",");
            Serial.println(manualLongitude,4);
        } else Serial.println("unknown");
    }
    prevWarm=warm;
    prevDoor=io_2;
    prevShock=io_1;
    *io_3=now/2000%2 ? events : (int32_t)temperature;
    *io_4=warm || io_2 ? 1000 : 0;
    *io_5=warm || io_2;
    *serial_0_trigger=false;
    *serial_0_value_0=temperature;
    *serial_0_value_1=events;''')
        why='Track independent temperature, door and shock events, preserve the latest valid GPS coordinates, and attach them to timestamped-by-trace alarm reports.'
    elif case=='advanced_026':
        code=body(code,'''    static bool lastLeft=false,lastRight=false,wasLit=false,detected=false;
    static uint8_t from=0;
    static uint32_t since=0,motion=0,last=0,lastReport=0,total=0;
    static int count=0;
    uint32_t now=millis();
    if (wasLit) total+=(uint32_t)(now-last);
    last=now;
    bool left=io_0 && !lastLeft,right=io_1 && !lastRight;
    lastLeft=io_0;
    lastRight=io_1;
    if (from && (uint32_t)(now-since)>4000) from=0;
    if (left!=right) {
        uint8_t side=left ? 1 : 2;
        if (from && from!=side) {if (count<9999) ++count;from=0;}
        else {from=side;since=now;}
    }
    if (io_0 || io_1) {detected=true;motion=now;}
    wasLit=io_2<350 && detected && (uint32_t)(now-motion)<3000;
    *io_3=0;
    *io_4=count;
    *io_5=wasLit ? 255 : 0;
    *serial_0_trigger=(uint32_t)(now-lastReport)>=5000;
    if (*serial_0_trigger) lastReport=now;
    *serial_0_value_0=(int32_t)total;''')
        why='Gate lighting on darkness, extend the hold while there is motion, count completed directional crossings and integrate actual on-time for reports.'
    elif case=='advanced_027':
        code=debounce_inputs(code,25,grounded=(2,))
        code=body(code,'''    static uint32_t lowSince=0;
    static bool low=false,reported=false;
    static int angle=90;
    uint32_t now=millis();
    if (io_6 && !sysml_state_prevButton) sysml_state_autoManual=!sysml_state_autoManual;
    sysml_state_prevButton=io_6;
    if (!sysml_state_autoManual) {
        double balance=io_1-io_0;
        angle=balance<-35 ? 45 : (balance>35 ? 135 : 90);
    }
    bool lowNow=io_2<614;
    if (lowNow && !low) lowSince=now;
    if (!lowNow) reported=false;
    low=lowNow;
    bool alarm=low && (uint32_t)(now-lowSince)>=10000;
    *io_3=angle;
    *io_4=now/2000%2 ? (int32_t)!low : (int32_t)(io_2*50.0/1023.0);
    *io_5=alarm ? 1000 : 0;
    *serial_0_trigger=alarm && !reported;
    if (*serial_0_trigger) reported=true;
    *serial_0_value_0=1;''')
        code=once(code,'{ Serial.print("low_charge_code"); Serial.println(hw_value_serial_0_value_0); }','Serial.println("low charge");')
        why='Track left, center and right in automatic mode, retain the calibrated angle in manual mode, and alarm only after ten continuous seconds below the disclosed three-volt threshold.'
    elif case=='complex_001':
        code=body(code,'''    static uint32_t lastHot=0,lastNear=0;
    static bool previousHot=false,previousNear=false;
    uint32_t now=millis();
    bool hot=io_2 && io_0>32;
    bool near=io_4 && io_3<15;
    bool distancePage=now/2000%2;
    *io_5=distancePage ? (io_4 ? (int32_t)io_3 : -1) : (io_2 ? (int32_t)io_0 : -1);
    *io_6=(hot || near) && now%1000<400 ? 1000 : 0;
    *serial_0_value_0=hot && (!previousHot || (uint32_t)(now-lastHot)>=1000);
    *serial_0_value_1=near && (!previousNear || (uint32_t)(now-lastNear)>=1000);
    if (*serial_0_value_0) lastHot=now;
    if (*serial_0_value_1) lastNear=now;
    previousHot=hot;
    previousNear=near;
    *serial_0_trigger=*serial_0_value_0 || *serial_0_value_1;''')
        code=once(code,'if (hw_value_serial_0_trigger) { Serial.print("ALARM_TEMP"); Serial.println(hw_value_serial_0_value_0); }',
                      'if (hw_value_serial_0_value_0) Serial.println("ALARM_TEMP");')
        code=once(code,'if (hw_value_serial_0_trigger) { Serial.print("ALARM_DISTANCE"); Serial.println(hw_value_serial_0_value_1); }',
                      'if (hw_value_serial_0_value_1) Serial.println("ALARM_DISTANCE");')
        why='Keep temperature and distance validity separate, blank the invalid page, and report only each active alarm reason on entry and once per second.'
    elif case=='complex_006':
        code=body(code,'''    uint32_t now=millis();
    int threshold=(int)(io_0*50.0/1023.0);
    *io_1=io_4 && io_3>threshold;
    *io_2=*io_1 && now%1000<400 ? 1000 : 0;
    *io_5=now/2000%2 ? threshold : (io_4 ? (int)io_3 : -1000);''')
        why='Map the threshold to 0–50 Celsius, alternate measured temperature and threshold, and pulse the buzzer during an alarm.'
    elif case=='complex_007':
        code=code.replace('#include <Arduino.h>','#include <Arduino.h>\n#include <string.h>\n#include <stdlib.h>',1)
        code=debounce_inputs(code,25,grounded=(2,))
        code=body(code,r'''    static char command[24];
    static uint8_t used=0,mode=0;
    static bool overflow=false,buzzing=false;
    static uint32_t blinkStart=0,buzzStart=0,lastByte=0;
    uint32_t now=millis();
    bool report=io_3 && !sysml_state_prevPressed;
    sysml_state_prevPressed=io_3;
    for (uint8_t budget=0; budget<64; ++budget) {
        char c='\n';
        if (Serial.available()) { c=(char)Serial.read(); lastByte=now; }
        else if (!(used || overflow) || (uint32_t)(now-lastByte)<20) break;
        if (c!='\n' && c!='\r') {
            if (used<sizeof(command)-1) command[used++]=c;
            else overflow=true;
            continue;
        }
        command[used]='\0';
        if (!overflow) {
            if (strcmp(command,"ON")==0) mode=1;
            else if (strcmp(command,"OFF")==0) mode=0;
            else if (strcmp(command,"BLINK")==0) { mode=2; blinkStart=now; }
            else if (strcmp(command,"BUZZ")==0) { buzzing=true; buzzStart=now; }
            else if (strcmp(command,"STATUS")==0) report=true;
            else if (strncmp(command,"PWM:",4)==0) {
                char *end;
                long value=strtol(command+4,&end,10);
                if (end!=command+4 && *end=='\0' && value>=0 && value<=255) sysml_state_pwmValue=value;
            }
        }
        used=0;
        overflow=false;
    }
    if (buzzing && (uint32_t)(now-buzzStart)>=250) buzzing=false;
    *io_0=mode==1 || (mode==2 && (uint32_t)(now-blinkStart)%1000<500);
    *io_1=sysml_state_pwmValue;
    *io_2=buzzing ? 1000 : 0;
    *serial_0_trigger=report;
    *serial_0_value_0=*io_0;
    *serial_0_value_1=(int32_t)*io_1;''')
        why='Restore the absent command decoder, independently retain PWM and LED state, and report status on command or debounced press.'
    elif case=='complex_012':
        code=debounce_inputs(code,25)
        code=body(code,'''    static uint32_t started=0;
    uint32_t now=millis();
    int period=100+(int)(io_0*900.0/1023.0);
    if (io_1 && !sysml_state_previousPressed) {
        sysml_state_running=!sysml_state_running;
        started=now;
    }
    sysml_state_previousPressed=io_1;
    *io_2=sysml_state_running && (uint32_t)(now-started)%(unsigned)period<(unsigned)period/2;
    *io_3=period;''')
        why='Use elapsed time and the displayed full-cycle period, keep configuration visible when stopped, and debounce the run toggle. The unspecified period is set to 100–1000 ms.'
    elif case=='complex_018':
        code=code.replace('#include <Arduino.h>','#include <Arduino.h>\n#include <stdlib.h>',1)
        code=once(code,'  if (!*begin) return false;','''  if (!*begin) return false;
  bool threshold=strncmp(begin,"SET_THRESHOLD:",14)==0;
  bool minimum=strncmp(begin,"SET_FAN_MIN:",12)==0;
  if (threshold || minimum) {
    char* number=begin+(threshold ? 14 : 12);
    char* tail;
    long parsed=strtol(number,&tail,10);
    if (number==tail || *tail || parsed<0 || parsed>(threshold ? 100 : 255)) return false;
    *value=(threshold ? 1000 : 2000)+parsed;
    return true;
  }''')
        code=body(code,'''    if (io_3_valid) {
        if (io_3>=2000) sysml_state_fanMinPwm=io_3-2000;
        else if (io_3>=1000) sysml_state_thresholdC=io_3-1000;
        else if (io_3==10) {sysml_state_thresholdC=30;sysml_state_fanMinPwm=0;}
    }
    double temperature=io_0*500.0/1023.0;
    double difference=temperature-sysml_state_thresholdC;
    double speed=sysml_state_fanMinPwm+max(0.0,difference)*25.0;
    *io_1=min(255.0,speed);
    *io_2=difference>=5.0;
    *serial_0_trigger=*io_2 && !sysml_state_prevOverheatCond;
    *serial_0_value_0=temperature;
    sysml_state_prevOverheatCond=*io_2;''')
        why='Parse numeric threshold and minimum PWM settings, calibrate the LM35 input, vary speed with temperature, and report an overheat on threshold crossing.'
    elif case=='complex_019':
        code=debounce_inputs(code,25,grounded=(2,3))
        code=body(code,'''    static uint32_t tick=0,beepStart=0;
    uint32_t now=millis();
    if (io_0 && !sysml_state_prevAddPressed && sysml_state_remainingSeconds<=9989)
        sysml_state_remainingSeconds+=10;
    if (io_1 && !sysml_state_prevStartPressed && !sysml_state_running && sysml_state_remainingSeconds>0) {
        sysml_state_running=true;
        tick=now;
    }
    sysml_state_prevAddPressed=io_0;
    sysml_state_prevStartPressed=io_1;
    if (sysml_state_running) {
        uint32_t steps=(uint32_t)(now-tick)/1000;
        tick+=steps*1000;
        if (steps>=(uint32_t)sysml_state_remainingSeconds) {
            sysml_state_remainingSeconds=0;
            sysml_state_running=false;
            sysml_state_beeping=true;
            beepStart=now;
        } else sysml_state_remainingSeconds-=steps;
    }
    if (sysml_state_beeping && (uint32_t)(now-beepStart)>=2000) sysml_state_beeping=false;
    *io_2=sysml_state_beeping ? 1000 : 0;
    *io_3=sysml_state_remainingSeconds;''')
        why='Count whole seconds from the start press and sound an uninterrupted two-second completion alarm, with one ten-second increment per press.'
    elif case=='complex_021':
        code=loop_body(code,'''  static bool valid=false;
  static uint32_t lastRead=0,phaseStart=0;
  uint32_t now=millis();
  if (!hw_dht_0_attempted || (uint32_t)(now-lastRead)>=2000) {
    bool previous=valid;
    bool first=!hw_dht_0_attempted;
    hw_dht_0_attempted=true;
    lastRead=now;
    valid=hw_dht_0.read();
    if (valid) {
      hw_value_io_1=hw_dht_0.readTemperature();
      hw_value_io_2=hw_dht_0.readHumidity();
      valid=isfinite(hw_value_io_1) && isfinite(hw_value_io_2);
    }
    if (first || previous!=valid) {
      phaseStart=now;
      if (!valid) Serial.println("sensor error");
    }
  }
  uint32_t phase=(uint32_t)(now-phaseStart);
  digitalWrite(13,!valid && phase%1000<500 ? HIGH : LOW);
  int value=valid ? (int)(phase/2000%2 ? hw_value_io_2 : hw_value_io_1) : 0;
  if (value!=hw_display_previous_io_3 || (phase<20 && now==phaseStart)) {
    hw_display_io_3.showNumberDec(value,!valid,4,0);
    hw_display_previous_io_3=value;
  }''')
        why='Use the DHT validity signal, show 0000 and blink during errors, report the error text, and resume alternating fresh temperature and humidity after recovery.'
    elif case=='complex_022':
        code=debounce_inputs(code,25,grounded=(2,3))
        code=body(code,'''    static bool prevMode=false,prevParameter=false;
    static uint8_t mode=0,brightness=64;
    static uint16_t period=2000;
    static uint32_t started=0;
    uint32_t now=millis();
    if (io_0 && !prevMode) { mode=(mode+1)%3; started=now; }
    if (io_1 && !prevParameter) {
        if (mode==1) brightness=brightness>=240 ? 16 : brightness+16;
        if (mode==2) { period=period<=500 ? 2000 : period-500; started=now; }
    }
    prevMode=io_0;
    prevParameter=io_1;
    unsigned phase=(uint32_t)(now-started)%period;
    unsigned ramp=phase<=period/2 ? phase : period-phase;
    *io_2=mode==0 ? 0 : (mode==1 ? brightness : ramp*510UL/period);
    *io_3=mode;
    *io_4=mode!=0;''')
        why='Use clean button edges, separate brightness and breathing speed, and calculate breathing from an elapsed-time triangle wave.'
    elif case=='complex_026':
        code=debounce_inputs(code,25)
        code=body(code,'''    static uint32_t changed=0,lastReport=0;
    uint32_t now=millis();
    if (io_5_valid && io_5!=sysml_state_mode) {
        sysml_state_mode=io_5;
        changed=now;
    }
    *serial_0_trigger=false;
    if (io_0 && !sysml_state_previousPressed) {
        sysml_state_serialEnabled=!sysml_state_serialEnabled;
        if (sysml_state_serialEnabled) lastReport=now-1000;
    }
    sysml_state_previousPressed=io_0;
    bool humidity=sysml_state_mode==2 || (sysml_state_mode==1 && (uint32_t)(now-changed)/2000%2);
    *io_4=io_3 ? (int32_t)(humidity ? io_2 : io_1) : -1000;
    if (sysml_state_serialEnabled && io_3 && (uint32_t)(now-lastReport)>=1000) {
        lastReport=now;
        *serial_0_trigger=true;
    }
    *serial_0_value_0=io_1;
    *serial_0_value_1=io_2;
    *serial_0_value_2=sysml_state_mode==1;''')
        code=once(code,'  if (hw_value_serial_0_trigger) { Serial.print("BOTH"); Serial.println(hw_value_serial_0_value_2); }\n','')
        why='Honor all three parsed command values, alternate current valid readings in BOTH mode, and gate periodic serial output on a debounced button toggle.'
    elif case=='complex_027':
        code=body(code,'''    static uint32_t last=0;
    static bool started=false;
    static uint8_t closeCount=0;
    uint32_t now=millis();
    bool report=!started || (uint32_t)(now-last)>=3000;
    *serial_1_trigger=false;
    if (report) {
        started=true;
        last=now;
        if (!io_5 || io_4>=20) closeCount=0;
        else if (closeCount<3) *serial_1_trigger=++closeCount==3;
    }
    *io_0=closeCount>=3;
    *serial_0_trigger=report;
    *serial_0_value_0=io_3 ? io_1 : NAN;
    *serial_0_value_1=io_5 ? io_4 : NAN;
    *serial_1_value_0=*io_0;''')
        code=once(code,'{ Serial.print("PersistentDetectedGate"); Serial.println(hw_value_serial_1_value_0); }',
                       'Serial.println("persistent object detected");')
        why='Use one three-second poll event for both reports and the consecutive-distance counter; invalidate missing echoes and announce the third close reading.'
    elif case=='complex_024':
        code=debounce_inputs(code,25)
        code=body(code,'''    static bool previous=false,beeping=false;
    static uint32_t beepStart=0;
    bool pressed=io_0 && !previous;
    if (pressed) {
        ++sysml_state_sampleCount;
        if (sysml_state_sampleCount%5==0) {beepStart=millis();beeping=true;}
    }
    if (beeping && (uint32_t)(millis()-beepStart)>=200) beeping=false;
    *io_3=beeping;
    *serial_0_trigger=pressed;
    *serial_0_value_0=io_1;
    *serial_0_value_1=io_2*500.0/1023.0;
    *serial_0_value_2=sysml_state_sampleCount;
    previous=io_0;''')
        why='Debounce each press, report the contemporaneous light and calibrated LM35 readings, and hold the buzzer on for 200ms after every fifth sample.'
    elif case=='embedbench_10':
        code=debounce_inputs(code,25)
        code=body(code,'''    static bool previous=false,reverse=false;
    if (io_1 && !previous) reverse=!reverse;
    previous=io_1;
    int count=(int)(io_0*10.0/1023.0);
'''+''.join(f'    *io_2_segment_{i}=count>(reverse ? {i} : {9-i});\n' for i in range(10)))
        why='Scale ADC to ten segments and reverse only the fill direction on each debounced press.'
    elif case=='embedbench_11':
        code=body(code,'''    *io_0=millis()/2000UL%3;''')
        code=once(code,'static int32_t hw_value_io_0 = 0;','static int32_t hw_value_io_0 = -1;')
        code=once(code,'static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};',
                       'static const uint8_t digits[3] = {0x3f,0x77,0x73};')
        code=once(code,'value < 10L','value < 3L')
        why='Latch a blank on boot, then drive the correct common-anode glyphs for 0, A and P at two-second intervals.'
    elif case=='embedbench_13':
        code=body(code,'''    static uint32_t randomState=0,last=0;
    static uint8_t leds=0;
    uint32_t now=millis();
    if (!randomState) {
        randomState=0xA23B4537UL;
        for (int i=0;i<8;++i) randomState=(randomState<<3)^(randomState>>2)^analogRead(A15);
        if (!randomState) randomState=1;
    }
    if ((uint32_t)(now-last)>=2000) {
        last=now;
        randomState^=randomState<<13;
        randomState^=randomState>>17;
        randomState^=randomState<<5;
        leds=randomState&7;
    }
    *io_0=leds&1;
    *io_1=leds&2;
    *io_2=leds&4;
    *io_3=(int)*io_0+(int)*io_1+(int)*io_2;''')
        why='Choose a new three-bit LED pattern every two seconds and drive the seven-segment display from its population count.'
    elif case=='embedbench_14':
        code=body(code,'''    uint32_t seconds=millis()/1000UL;
    *io_1=seconds ? (seconds*2)%10 : -1;''')
        code=once(code,'static int32_t hw_value_io_1 = 0;','static int32_t hw_value_io_1 = -1;')
        why='Keep the display blank before the first second and show the units of twice the elapsed real seconds thereafter.'
    elif case=='embedbench_24':
        code=body(code,'''    int minimum=(int)(io_0*90.0/1023.0);
    int maximum=90+(int)(io_1*90.0/1023.0);
    *io_3=minimum+io_2*(maximum-minimum)/1023.0;''')
        why='Calculate the lower and upper servo limits from separate knobs and interpolate between them using the third knob.'
    elif case=='embedbench_26':
        code=body(code,'''    uint32_t phase=millis()/2000UL;
    int index=-1;
    if (phase) {
        unsigned step=(phase-1)%18;
        index=step<=9 ? step : 18-step;
    }
'''+''.join(f'    *io_{i}=index=={i};\n' for i in range(10))+'''    *io_10=false;''')
        why='Scan the wired pins in both directions every two seconds. The second bar is wired one output behind the first.'
    elif case=='embedbench_28':
        code=body(code,'''    unsigned phase=millis()/2000UL%4;
    *io_0=phase>=1 ? 180 : 0;
    *io_1=phase>=2 ? 180 : 0;
    *io_2=phase>=3 ? 180 : 0;
    int count=phase==3 ? 10 : phase*3;
'''+''.join(f'    *io_3_segment_{i}=count>{i};\n' for i in range(10)))
        why='Advance the servos cumulatively with the required 0/3/6/10 bar pattern and reset every four two-second phases.'
    elif case=='embedbench_29':
        code=body(code,'''    int first=(int)(io_0*3.0/1023.0);
    int second=(int)(io_1*3.0/1023.0);
    int third=(int)(io_2*4.0/1023.0);
'''+''.join(f'    *io_3_segment_{start+i}={value}>{i};\n'
            for start,size,value in ((0,3,'first'),(3,3,'second'),(6,4,'third')) for i in range(size)))
        why='Scale each potentiometer separately for its wired 3/3/4-segment group and light the group from its first LED.'
    elif case=='embedbench_30':
        code=body(code,'''    int first=(int)(io_0*9.0/1023.0);
    int last=(int)(io_1*9.0/1023.0);
    if (first>last) { int swap=first; first=last; last=swap; }
    int count=(int)(io_2*(last-first+1)/1023.0);
'''+''.join(f'    *io_{i+3}={i}>=first && {i}<first+count;\n' for i in range(10)))
        why='Sort the mapped endpoints, include both bounds and apply the fill percentage only within the selected range.'
    elif case=='embedbench_34':
        code=body(code,'''    uint32_t tick=millis()/2000UL;
    int digit=tick%10;
    int base=digit/3*45;
    bool moving=tick!=0 && digit!=9;
    *io_0=digit;
    *io_1=moving ? base : 0;
    *io_2=moving ? base+45 : 0;
    *io_3=moving ? base+90 : 0;''')
        why='Drive the three servo targets from the shown digit and keep the special all-zero startup and digit-9 states.'
    elif case=='embedbench_50':
        code=body(code,'''    uint32_t tick=millis()/2000UL;
    int index=tick ? (int)((tick-1)%8) : -1;
    bool first=index>=0 && index<4;
    bool second=index>=4;
'''+''.join(f'    *io_{i}=index=={i};\n' for i in range(8))+'''    *io_8=first;
    *io_9=second;
    *io_10=false;
    *io_11=*io_12=second;
    *io_13=first;''')
        why='Tie both RGB patterns to the same single active bar segment and keep the eight two-second stages synchronized.'
    elif case=='embedbench_54':
        code=body(code,'''    static double rgb[3]={};
    rgb[millis()/5000UL%3]=io_0*255.0/1023.0;
    *io_1=rgb[0];
    *io_2=rgb[1];
    *io_3=rgb[2];
    int count=(int)(io_0*10.0/1023.0);
'''+''.join(f'    *io_{4+i}=count>{i};\n' for i in range(10)))
        why='Write only the selected channel per five-second phase, retain the other channel intensities, and update the bar independently.'
    elif case=='embedbench_76':
        code=debounce_inputs(code,25)
        code=body(code,'''    static bool previous[3]={},started=false;
    static uint8_t count=0;
    bool buttons[]={io_3,io_4,io_5};
    for (uint8_t i=0; i<3; ++i) {
        if (buttons[i] && !previous[i]) {
            count+=i+1;
            if (count>9) count=0;
            started=true;
        }
        previous[i]=buttons[i];
    }
    *io_0=count;
    *io_1=count>=5 ? 90 : 0;
    *io_2=count>=5 ? 180 : (started ? 90 : 0);''')
        why='Use the three specified increments and reset any overflow to zero, keeping initialization distinct from the 0–4 servo range after interaction.'
    elif case=='embedbench_77':
        code=debounce_inputs(code,25)
        code=body(code,'''    static bool previous=false;
    static uint8_t count=0;
    if (io_0 && !previous) count=(count+1)%10;
    previous=io_0;
    *io_1=count;
    *io_2=*io_3=count%2 ? 90 : 0;''')
        why='Tie both servo angles to the parity of the displayed single digit and wrap at ten presses.'
    elif case in ('embedbench_78','embedbench_79'):
        code=debounce_inputs(code,25,release_immediate=True)
        init='(int)(io_2*9.0/1023.0)*10+(int)(io_3*9.0/1023.0)' if case=='embedbench_78' else '0'
        increment_pot='io_2' if case=='embedbench_78' else 'io_3'
        reset='    if (io_2) { number=0; direction=0; }\n' if case=='embedbench_79' else ''
        code=body(code,'''    static int number=INITIAL;
    static int previous=0;
    static uint32_t last=0;
    uint32_t now=millis();
    int direction=(int)io_0-(int)io_1;
RESET    uint32_t interval=100+(uint32_t)((direction>0 ? INC_POT : io_3)*900.0/1023.0);
    if (direction && (direction!=previous || (uint32_t)(now-last)>=interval)) {
        number=(number+direction+100)%100;
        last=now;
    }
    previous=direction;
    *io_4=number;'''.replace('INITIAL',init).replace('RESET',reset).replace('INC_POT',increment_pot))
        code=code.replace('valid && (value >= 10L) ? digits[(value / 10L) % 10]','valid ? digits[(value / 10L) % 10]')
        why='Apply one change per press, repeat at the potentiometer-selected 100–1000 ms interval while held, and preserve decimal wrap and leading zeros.'
    elif case=='pceval_clock':
        code=once(code,'const uint8_t RTC_ADDRESS = 0x68;  // DS1307','RTC_DS1307 rtc;')
        code=once(code,'#include <Wire.h>','#include <Wire.h>\n#include <RTClib.h>')
        start=code.index('bool decodeBcd(')
        end=code.index('void updateTimeDisplay()',start)
        code=code[:start]+'''bool readRtcTime(uint8_t &hours, uint8_t &minutes) {
  if (!rtc.begin()) return false;
  DateTime current=rtc.now();
  hours=current.hour();
  minutes=current.minute();
  return current.isValid();
}

'''+code[end:]
        why='Read the real-time clock using RTClib while retaining the existing four-digit refresh and colon wiring.'
    elif case=='pceval_calendar_display':
        code=code.replace('SECONDS_PER_DAY','CALENDAR_SECONDS_PER_DAY')
        code=once(code,'''  tft.fillRect(10, 30, tft.width() - 10, 24, ILI9341_BLACK);
  tft.fillRect(10, 80, tft.width() - 10, 24, ILI9341_BLACK);''','  tft.fillScreen(ILI9341_BLACK);')
        why='Resolve the identifier collision with RTClib and clear stale date text before drawing the new weekday and date.'
    elif case=='pceval_4_digit_7_segment_display':
        code=loop_body(code,'''  static int previous=-1;
  const uint8_t patterns[]={0x06,0x5b,0x4f,0x66};
  int digit=millis()/1000UL%4;
  if (digit==previous) return;
  for (int pin=10; pin<=13; ++pin) digitalWrite(pin,LOW);
  for (int bit=0; bit<7; ++bit) digitalWrite(2+bit,patterns[digit]&(1U<<bit) ? LOW : HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(10+digit,HIGH);
  previous=digit;''')
        why='Apply the common-anode segment polarity and select one of the four digit pins on each one-second step.'
    elif case=='pceval_LCD_display_basic':
        code='#include <SPI.h>\n#include <Adafruit_GFX.h>\n#include <Adafruit_ILI9341.h>\nstatic Adafruit_ILI9341 screen(10,9,8);\n'+code
        code=loop_body(code,'''  screen.begin();
  screen.setRotation(1);
  screen.fillScreen(ILI9341_BLACK);
  screen.setCursor(0,0);
  screen.setTextColor(ILI9341_WHITE);
  screen.setTextSize(2);
  screen.print("Hello World!");''','setup')
        code=loop_body(code,'')
        why='Initialize the actual ILI9341 over hardware SPI, using the specified CS, D/C and reset pins and display settings.'
    elif case in ('pceval_multiplexer_photoresistor','pceval_multiplexer_potentiometer'):
        channels=6 if case=='pceval_multiplexer_photoresistor' else 5
        code=loop_body(code,'''  static bool started=false;
  static uint32_t last=0;
  uint32_t now=millis();
  if (started && (uint32_t)(now-last)<250) return;
  started=true;
  last=now;
  for (uint8_t channel=0; channel<CHANNELS; ++channel) {
    for (uint8_t bit=0; bit<4; ++bit) digitalWrite(2+bit,channel&(1U<<bit) ? HIGH : LOW);
    delayMicroseconds(20);
    int value=analogRead(A0);
    Serial.print("Sensor ");
    Serial.print(channel);
    Serial.print(": ");
    Serial.println(value);
  }'''.replace('CHANNELS',str(channels)))
        why='Set the physical selector before sampling A0, scan only the wired channels, and report each channel and reading together.'
    elif case=='pceval_photoresistor_bar_led':
        code=body(code,'''    int count=(int)((1023-io_0)*10.0/1023.0);
'''+''.join(f'    *io_{i+1}=count>{i};\n' for i in range(10)))
        why='Reverse the raw ADC mapping so brighter light produces a larger cumulative bar, including both endpoints.'
    elif case=='pceval_binary_led':
        code=body(code,'''    static int32_t value=0;
    if (io_0_valid && io_0>=0 && io_0<=31) value=io_0;
'''+''.join(f'    *io_{i+1}=(value & {1<<i})!=0;\n' for i in range(5)))
        why='Persist the received five-bit value and drive D2 through D6 from least- to most-significant bit.'
    elif case=='pceval_serial_bar_led':
        code=body(code,'''    static int32_t count=0;
    if (io_0_valid && io_0>=0 && io_0<=10) count=io_0;
'''+''.join(f'    *io_1_segment_{i}=count>{i};\n' for i in range(10)))
        why='Persist the integer threshold and light exactly the corresponding number of segments beginning at D2.'
    elif case=='embedbench_42':
        code=body(code,'''    int number=millis()/2000UL%100;
    int units=number%10;
    *io_1=number;
    *io_3=units*18;
    *io_4=units ? min(180,units*18+60) : 0;
    *io_5=units ? min(180,units*18+120) : 0;
'''+''.join(f'    *io_2_segment_{i}=number/10>{i};\n' for i in range(10)))
        code=code.replace('valid && (value >= 10L) ? digits[(value / 10L) % 10]','valid ? digits[(value / 10L) % 10]')
        why='Synchronize the full two-digit count, tens bar and units angles. Clamp physical angles at 180 and reset the servo trio after each units digit 9.'
    elif case=='embedbench_43':
        code=body(code,'''    static uint32_t previous=0;
    static uint8_t digit=0;
    uint32_t now=millis();
    uint32_t period=2000+(uint32_t)((io_1+io_2)*1000.0/1023.0);
    if ((uint32_t)(now-previous)>=period) {previous=now;digit=(digit+1)%10;}
    *io_3=digit;
'''+''.join(f'    *io_4_segment_{i}=digit>{i};\n' for i in range(10)))
        for i,pin in enumerate(range(2,12)):
            code=code.replace(f'digitalWrite({pin}, hw_value_io_4_segment_{i} ? HIGH : LOW);',
                              f'analogWrite({pin}, hw_value_io_4_segment_{i} ? (int)(hw_value_io_0*255.0/1023.0) : 0);')
        why='Combine the two 1–2 second timing controls and apply live PWM only to the number of bar segments indicated by the digit.'
    elif case=='embedbench_44':
        code=body(code,'''    int value=(int)(io_1*99.0/1023.0);
    *io_0=value;
    int count=value/10;
'''+''.join(f'    *io_2_segment_{i}=count>{i};\n' for i in range(10)))
        why='Scale ADC to 00–99 before deriving the bar count from the tens digit.'
    elif case=='embedbench_47':
        code=body(code,'''    const double adc[]={io_0,io_1};
    double *outputs[][3]={{io_2,io_3,io_4},{io_5,io_6,io_7}};
    const uint8_t peak[]={0,1,1,2,2,0}, ramp[]={1,0,2,1,0,2};
    for (uint8_t i=0;i<2;++i) {
        double position=adc[i]*6.0/1023.0;
        unsigned interval=(unsigned)position;
        unsigned sector=interval%6;
        double fraction=position-interval;
        for (uint8_t color=0;color<3;++color) *outputs[i][color]=0;
        *outputs[i][peak[sector]]=255;
        *outputs[i][ramp[sector]]=255*(sector%2 ? 1-fraction : fraction);
    }''')
        why='Convert both independent full-saturation hue inputs through all six HSV sectors using interpolated PWM.'
    elif case=='embedbench_51':
        code=body(code,'''    unsigned phase=millis()/2000UL%5;
    bool white=phase==4;
    *io_0=(white || phase==0 || phase==3) ? 255 : 0;
    *io_4=(white || phase==1 || phase==3) ? 255 : 0;
    *io_8=(white || phase==2) ? 255 : 0;
    *io_1=*io_2=*io_3=*io_5=*io_6=*io_7=white ? 255 : 0;
    *io_10=*io_11=false;
'''+''.join(f'    *io_9_segment_{i}=phase>={i};\n' for i in range(5)))
        why='Implement the five explicit RGB phases and cumulative bar states on a shared two-second tick.'
    elif case=='embedbench_52':
        code=body(code,'''    unsigned step=millis()/2000UL%7;
    *io_0=*io_1=step*30;
    *io_2=step>0;
    *io_3=*io_4=*io_5=*io_6=false;
    *io_7=step>3;
'''+''.join(f'    *io_{9+i}=step>{i};\n' for i in range(6)))
        why='Use seven two-second phases, hold 180 degrees before resetting, and derive both RGB and cumulative bar states from the current angle.'
    elif case=='embedbench_53':
        code=body(code,'''    unsigned phase=millis()/2000UL%6;
    bool red=phase>=1 && phase<=3;
    bool green=phase>=2 && phase<=4;
    bool blue=phase>=3;
    *io_0=red ? 180 : 0;
    *io_1=green ? 180 : 0;
    *io_2=blue ? 180 : 0;
    *io_3=red ? 255 : 0;
    *io_4=green ? 255 : 0;
    *io_5=blue ? 255 : 0;
    int count=((int)red+(int)green+(int)blue)*8/3;
'''+''.join(f'    *io_6_segment_{i}=count>{i};\n' for i in range(8)))
        why='Toggle one servo per two-second step, derive the matching RGB values and scale their average to eight bar segments.'
    elif case=='embedbench_55':
        code=body(code,'''    *io_2=io_0<341;
    *io_3=io_0>=341 && io_0<682;
    *io_4=io_0>=682;
    *io_5=io_1<341;
    *io_6=io_1>=341 && io_1<682;
    *io_7=io_1>=682;
    int count=(int)((io_0+io_1)*4.0/1023.0);
'''+''.join(f'    *io_8_segment_{i}=count>{i};\n' for i in range(8)))
        why='Apply independent primary-color boundaries and use the mean of the two ADC values for the eight-segment bar.'
    elif case=='embedbench_56':
        code=body(code,'''    const uint8_t colors[]={1,2,4,3,6,5,7};
    static uint32_t tick=0;
    static uint8_t colorIndex=0, number=0;
    uint32_t elapsed=(uint32_t)(millis()-tick);
    if (elapsed>=2000) {
        uint32_t steps=elapsed/2000;
        tick+=steps*2000;
        colorIndex=(colorIndex+steps)%7;
        number=(number+steps)%100;
    }
    uint8_t color=colors[colorIndex];
    *io_0=*io_3=color&1;
    *io_1=*io_4=color&2;
    *io_2=*io_5=color&4;
    *io_6=number;''')
        code=once(code,'valid && (value >= 10L) ? digits[(value / 10L) % 10]','valid ? digits[(value / 10L) % 10]')
        why='Advance the RGB sequence and decimal counter on the same elapsed-time ticks, with independent wrap periods and visible leading zero.'
    elif case=='embedbench_57':
        code=body(code,'''    unsigned step=millis()/2000UL%100;
    unsigned phase=step%6;
    *io_0=99-step;
    *io_1=phase==0 || phase==3;
    *io_2=false;
    *io_3=phase==3;
    *io_4=phase==4;
    *io_5=phase==1 || phase==4;
    *io_6=*io_7=false;
    *io_8=phase==5;
    *io_9=phase==2 || phase==5;''')
        code=code.replace('valid && (value >= 10L) ? digits[(value / 10L) % 10]','valid ? digits[(value / 10L) % 10]')
        why='Reset the six-color sequence with the two-digit countdown, preserving the displayed zero and every simultaneous transition.'
    elif case=='embedbench_58':
        code=body(code,'''    unsigned step=millis()/2000UL%10;
    const uint8_t colors[]={1,2,4,3,6,5,7};
    uint8_t color=step ? colors[(step-1)%7] : 0;
    *io_0=color&1 ? 255 : 0;
    *io_1=color&2 ? 255 : 0;
    *io_2=color&4 ? 255 : 0;
    *io_3=step*20;
    *io_4=9-step;''')
        why='Use one tick for the countdown, servo angle and full color order, retaining the dark nine at startup and restart.'
    elif case=='embedbench_59':
        code=body(code,'''    uint32_t tick=millis()/2000UL;
    const uint8_t colors[]={1,2,6,5,4,3,7};
    uint8_t color=colors[tick%7];
    *io_0=*io_3=*io_6=color&1 ? 255 : 0;
    *io_1=*io_4=*io_7=color&2 ? 255 : 0;
    *io_2=*io_5=*io_8=color&4 ? 255 : 0;
    int digit=9-tick%10;
    *io_9=digit;
    *io_10=*io_11=*io_12=digit==0 ? 180 : ((9-digit)/3)*60;''')
        why='Use the specified shared color order and countdown-derived servo angle bands, both updated at the same two-second ticks.'
    elif case=='embedbench_62':
        code=body(code,'''    uint32_t tick=millis()/2000UL;
    int number=tick%100;
    *io_0=*io_3=tick && number<=5 ? 255 : 0;
    *io_1=*io_4=number>=6 && number<=9 ? 255 : 0;
    *io_2=*io_5=number>=10 ? 255 : 0;
    *io_7=number;
'''+''.join(f'    *io_6_segment_{i}=number/10>{i};\n' for i in range(8)))
        code=code.replace('valid && (value >= 10L) ? digits[(value / 10L) % 10]','valid ? digits[(value / 10L) % 10]')
        why='Display the actual two-digit count, cap the tens bar at its eight physical segments and apply the exact two-RGB value ranges.'
    elif case=='embedbench_67':
        code=debounce_inputs(code,25)
        code=body(code,'''    static bool previous=false,started=false,reverse=false;
    static uint32_t began=0;
    static double fromA=0,fromB=0,targetA=0,targetB=0;
    uint32_t now=millis();
    double fraction=min(1.0,(uint32_t)(now-began)/1000.0);
    double a=fromA+(targetA-fromA)*fraction,b=fromB+(targetB-fromB)*fraction;
    if (io_0 && !previous) {
        fromA=a;
        fromB=b;
        began=now;
        reverse=started && !reverse;
        started=true;
        targetA=reverse ? 180 : 0;
        targetB=180-targetA;
    }
    previous=io_0;
    *io_1=a;
    *io_2=b;''')
        why='Debounce one toggle at a time and interpolate from the current servo angles to complementary targets.'
    elif case=='embedbench_75':
        code=debounce_inputs(code,150,release_immediate=True)
        code=body(code,'''    static bool previous=false;
    static uint8_t selected=0;
    if (io_0 && !previous) selected=selected==1 ? 2 : 1;
    previous=io_0;
    int count=selected ? (int)((selected==1 ? io_1 : io_2)*10.0/1023.0) : 0;
'''+''.join(f'    *io_3_segment_{i}=count>{i};\n' for i in range(10)))
        why='Start with neither source selected, require a 150 ms press, and update the cumulative bar from the selected potentiometer.'
    elif case=='embedbench_81':
        code=debounce_inputs(code,25)
        code=body(code,'''    static bool previous[3]={};
    static int number=0;
    if (io_0 && !previous[0]) ++number;
    if (io_1 && !previous[1]) number+=5;
    if (number>99 || (io_2 && !previous[2])) number=0;
    previous[0]=io_0;
    previous[1]=io_1;
    previous[2]=io_2;
    *io_3=number;
'''+''.join(f'    *io_4_segment_{i}=number/10>{i};\n' for i in range(10)))
        code=code.replace('valid && (value >= 10L) ? digits[(value / 10L) % 10]','valid ? digits[(value / 10L) % 10]')
        why='Debounce the three commands, clear every overflow or reset to 00, and update the tens bar and display from the same state.'
    elif case=='embedbench_82':
        code=debounce_inputs(code,25)
        code=body(code,'''    static bool previous[3]={};
    static uint8_t color=0,angle1=0,angle2=0;
    static uint32_t changed=0;
    uint32_t now=millis();
    bool buttons[]={io_2,io_3,io_4};
    for (int i=2; i>=0; --i) if (buttons[i] && !previous[i] && (!color || (uint32_t)(now-changed)>=2000)) {
        color=i+1;
        if (i==0) angle1=90;
        else if (i==1) angle2=90;
        else angle1=angle2=0;
        changed=now;
        break;
    }
    for (uint8_t i=0; i<3; ++i) previous[i]=buttons[i];
    *io_0=angle1;
    *io_1=angle2;
    *io_5=color==1;
    *io_6=color==2;
    *io_7=color==3;''')
        why='Preserve both servo commands until explicitly changed, enforce the two-second state dwell, and reset both on the blue-button transition.'
    elif case=='embedbench_83':
        code=debounce_inputs(code,25)
        code=body(code,'''    static bool prevLeft=false,prevRight=false,visible=false;
    static uint8_t mode=0;
    static uint32_t changed=0;
    uint32_t now=millis();
    bool left=io_2 && !prevLeft, right=io_3 && !prevRight;
    if (visible && (uint32_t)(now-changed)>=2000) visible=false;
    if (!visible && (left || right)) {
        if ((left && right) || (left && mode==2) || (right && mode==1)) mode=3;
        else mode=left ? 1 : 2;
        visible=true;
        changed=now;
    }
    prevLeft=io_2;
    prevRight=io_3;
    *io_0=mode==1 ? 90 : 0;
    *io_1=mode==2 ? 180 : 0;
    *io_4=mode==1 && (visible || io_2);
    *io_5=mode==3 && visible;
    *io_6=mode==2 && (visible || io_3);''')
        why='Retain the selected servo after release, recognize the other button as the combined reset, and maintain each RGB indication for two seconds.'
    elif case=='embedbench_84':
        code=debounce_inputs(code,25)
        code=body(code,'''    static int mode=0;
    static bool previousNext=false,previousReset=false;
    if (io_4 && !previousReset) mode=0;
    else if (io_3 && !previousNext) mode=mode%4+1;
    previousNext=io_3;
    previousReset=io_4;
    *io_5=mode==1 || mode==4 ? io_0*255.0/1023.0 : 0;
    *io_6=mode==2 || mode==4 ? io_1*255.0/1023.0 : 0;
    *io_7=mode==3 || mode==4 ? io_2*255.0/1023.0 : 0;''')
        why='Keep startup and reset dark, start with red on the next press, and retain proportional per-channel PWM in each of the four modes.'
    elif case=='embedbench_85':
        code=debounce_inputs(code,25)
        code=body(code,'''    static int group=1,index=0;
    static bool previousPrimary=false,previousSecondary=false;
    bool primary=io_0 && !previousPrimary,secondary=io_1 && !previousSecondary;
    int selection=primary ? 1 : (secondary ? 2 : 0);
    previousPrimary=io_0;
    previousSecondary=io_1;
    if (primary && secondary) group=0;
    else if (selection) {
        if (selection==group) index=(index+1)%3;
        else if (group==0) { group=selection; index=0; }
        else group=0;
    }
    const uint8_t colors[][3]={{0,0,0},{1,2,4},{3,6,5}};
    uint8_t color=colors[group][index];
    *io_3=color&1;
    *io_4=color&2;
    *io_5=color&4;''')
        for i,pin in enumerate((4,5,6),3):
            code=code.replace(f'digitalWrite({pin}, hw_value_io_{i} ? HIGH : LOW);',
                              f'analogWrite({pin}, hw_value_io_{i} ? (int)(hw_value_io_2*255.0/1023.0) : 0);')
        why='Implement independent primary and secondary cycles with the off transition between groups and live PWM brightness on each active channel.'
    elif case=='embedbench_86':
        code=debounce_inputs(code,150,release_immediate=True)
        code=body(code,'''    static bool previous=false;
    static uint8_t mode=0;
    if (io_0 && !previous) mode=(mode+1)%8;
    previous=io_0;
    const uint8_t colors[][2]={{0,0},{1,4},{2,6},{3,5},{6,3},{5,6},{7,3},{0,7}};
    uint8_t a=colors[mode][0],b=colors[mode][1];
    *io_1=a&1 ? 255 : 0;
    *io_2=a&2 ? 255 : 0;
    *io_3=a&4 ? 255 : 0;
    *io_4=b&1 ? 255 : 0;
    *io_5=b&2 ? 255 : 0;
    *io_6=b&4 ? 255 : 0;
'''+''.join(f'    *io_7_segment_{i}=mode>{i};\n' for i in range(7)))
        why='Apply the full pairwise RGB table and matching seven-segment fill after each qualified press, including the dark wrap.'
    elif case=='embedbench_88':
        code=debounce_inputs(code,25)
        code=body(code,'''    static bool previousUp=false,previousDown=false,active=false;
    static int number=0;
    bool up=io_1 && !previousUp,down=io_2 && !previousDown;
    if (up!=down) {number=(number+(up ? 1 : 99))%100;active=true;}
    previousUp=io_1;
    previousDown=io_2;
    *io_0=number;
    *io_3=active && number<=33;
    *io_4=active && number>=34 && number<=66;
    *io_5=active && number>=67;''')
        code=code.replace('valid && (value >= 10L) ? digits[(value / 10L) % 10]','valid ? digits[(value / 10L) % 10]')
        why='Debounce each decimal change, support bidirectional wrap, and use mutually exclusive inclusive color ranges with a dark startup.'
    elif case=='embedbench_90':
        code=body(code,'''    uint32_t tick=millis()/2000UL;
    unsigned phase=tick%15;
    *io_0=phase>0 && phase<=6;
    *io_1=phase>=7 && phase<=13;
    *io_2=phase==14;
    *io_3=phase<=6 ? phase*30 : 180;
    *io_4=phase<=7 ? (tick<7 ? 0 : 180) : (phase<=13 ? (13-phase)*30 : 0);''')
        why='Separate the two stepped servo stages from the final two-second indicator and reset both servo targets before repeating.'
    elif case=='embedbench_94':
        code=body(code,'''    static int angle=0;
    static uint32_t last=0;
    uint32_t now=millis();
    unsigned interval=10+(unsigned)(io_2*90.0/1023.0);
    int target=(int)(io_0*180.0/1023.0);
    if (angle==target) last=now;
    else if ((uint32_t)(now-last)>=interval) {
        angle+=target>angle ? 1 : -1;
        last=now;
    }
    *io_3=angle;
    *io_4=false;''')
        code=code.replace('digitalWrite(2, hw_value_io_4 ? HIGH : LOW);','analogWrite(2, (int)(hw_value_io_1*255.0/1023.0));')
        why='Use the third knob to pace individual servo steps and output proportional PWM, rather than a binary LED signal, on the wired D2.'
    elif case=='embedbench_95':
        code=body(code,'''    uint32_t tick=millis()/2000UL;
    unsigned count=tick%11;
    *io_2=*io_3=false;
    *io_4=tick && tick%2;
    *io_5=tick && !(tick%2);
'''+''.join(f'    *io_{io}=count>{index};\n' for index,io in enumerate((6,7,8,9,10,11,0,1,12,13))))
        code=re.sub(r'(?m)^\s*(?:pinMode|digitalWrite)\(A(?:1|10),[^;]+;\n','\n',code)
        why='Correct the display pin order to match D4–D13 and run a cumulative ten-step loading animation with two alternating indicators.'
    elif case=='embedbench_96':
        code=body(code,'''    uint32_t tick=millis()/2000UL;
    int phase=tick ? (tick-1)%4 : -1;
    *io_1=phase==0;
    *io_2=phase==1;
    *io_3=phase==2;
    *io_4_segment_0=phase>=0;
'''+''.join(f'    *io_4_segment_{index}=phase==3;\n' for index in range(1,10)))
        why='Hold each individual LED for two seconds with a one-segment bar, then light all segments for two seconds before repeating.'
    elif case=='embedbench_97':
        code=body(code,'''    unsigned stage=millis()/2000UL%4;
    *io_0=stage>=1 ? 60 : 0;
    *io_1=stage>=2 ? 120 : 0;
    *io_2=stage>=3 ? 180 : 0;
    *io_3=stage>=1;
    *io_4=stage>=2;
    *io_5=stage>=3;
    const int count[]={0,2,5,8};
'''+''.join(f'    *io_6_segment_{i}=count[stage]>{i};\n' for i in range(8)))
        why='Synchronize the 60/120/180 servo sequence and the accumulating indicator groups, with a complete two-second reset.'
    elif case=='embedbench_98':
        code=body(code,'''    uint32_t tick=millis()/2000UL;
    int angle=tick ? ((tick-1)%19)*10 : 0;
    *io_0=angle;
    *io_1=tick ? 180-angle : 0;
    *io_2=*io_3=tick && angle<=90;
    int count=angle/18;
'''+''.join(f'    *io_{4+i}=count>{i};\n' for i in range(10)))
        why='Use mirrored ten-degree commands and the shared inclusive 90-degree indicator boundary, with proportional bar fill and a dark zero-angle boot.'
    elif case=='embedbench_103':
        code=body(code,'''    uint32_t now=millis();
    uint32_t phase=now%12000;
    uint32_t tick=now/2000UL;
    double angle=phase<4000 ? phase*180.0/4000.0 :
        (phase<6000 ? 180.0 : (phase<10000 ? (10000-phase)*180.0/4000.0 : 0.0));
    *io_0=tick%2;
    *io_1=!*io_0;
    *io_2=tick%10;
    *io_3=angle;
    *io_4=180-angle;''')
        why='Use mirrored four-second one-way sweeps with two-second endpoint dwells, while the LEDs and digit use their independent two-second tick.'
    elif case=='embedbench_105':
        code=body(code,'''    int value=(int)(io_0*99.0/1023.0);
    *io_1=value;
    *io_2=true;
    *io_3=value>=34;
    *io_4=value>=67;''')
        why='Use the displayed numeric range to select one, two or three cumulative indicators, preserving the last state while the input is unchanged.'
    elif case=='embedbench_106':
        code=body(code,'''    uint32_t tick=millis()/2000UL;
    int number=tick%100;
    *io_1=number;
    *io_2=false;
    *io_3=tick && tick%2;
    *io_4=tick && !(tick%2);
    int count=number/10+1;
'''+''.join(f'    *io_{5+i}=count>{i};\n' for i in range(10)))
        code=re.sub(r'(?m)^\s*(?:pinMode|digitalWrite)\(A10,[^;]+;\n','\n',code)
        why='Derive the ten correctly wired bar outputs from tens plus one, retain a leading zero, and alternate the discrete LEDs each two-second tick.'
    elif case=='embedbench_108':
        code=body(code,'''    uint32_t tick=millis()/2000UL;
    const uint8_t colors[]={1,2,4,3,6,5,7};
    uint8_t color=colors[tick%7];
    *io_0=tick%2;
    *io_1=*io_4=*io_7=color&1;
    *io_2=*io_5=*io_8=color&2;
    *io_3=*io_6=*io_9=color&4;''')
        why='Cycle all three RGB LEDs through the full color table while the status LED blinks on its independent two-state cycle.'
    elif case=='embedbench_109':
        code=body(code,'''    uint32_t now=millis();
    uint32_t phase=now%8000;
    uint32_t endpoints=(now+2000UL)/4000UL;
    double angle=phase<2000 ? phase*180.0/2000.0 :
        (phase<4000 ? 180.0 : (phase<6000 ? (6000-phase)*180.0/2000.0 : 0.0));
    unsigned next=(endpoints+2)%3;
    unsigned rgb=endpoints%3;
    *io_0=endpoints && next==0;
    *io_1=endpoints && next==1;
    *io_2=endpoints && next==2;
    *io_3=rgb==0;
    *io_4=rgb==1;
    *io_5=rgb==2;
    *io_6=*io_7=angle;''')
        why='Synchronize both servo sweeps and dwell periods, and advance the indicator and RGB color only when a sweep reaches an endpoint.'
    elif case=='embedbench_113':
        code=body(code,'''    unsigned stage=millis()/2000UL%4;
    *io_0=stage%2;
    const uint16_t states[]={1,34,140,511};
    uint16_t mask=states[stage];
'''+''.join(f'    *io_{i+1}=(mask & {1<<i})!=0;\n' for i in range(9))+
          ''.join(f'    *io_10_segment_{i}=stage>={i};\n' for i in range(4)))
        why='Use the four explicit RGB masks and bar levels, with the discrete indicator active only in stages two and four.'
    elif case=='embedbench_114':
        code=body(code,'''    uint32_t tick=millis()/2000UL;
    unsigned stage=tick ? (tick-1)%3+1 : 0;
    bool first=stage==1 || stage==3,second=stage==2 || stage==3;
    *io_0=first;
    *io_1=second;
    *io_2=stage==1;
    *io_3=*io_4=stage==3;
    *io_5=false;
    *io_6=second;
    *io_7=stage==3;
'''+''.join(f'    *io_8_segment_{i}={"first" if i<3 else "second"};\n' for i in range(6)))
        why='Hold the startup state dark, then synchronize the RGB, discrete LEDs and first/last bar segments in all three stages.'
    elif case=='embedbench_116':
        code=body(code,'''    uint32_t tick=millis()/2000UL;
    const uint8_t colors[]={1,2,4,3,6,5,7};
    uint8_t rgb=colors[tick%7];
    *io_0=tick%2;
    *io_1=rgb&1;
    *io_2=rgb&2;
    *io_3=rgb&4;
    *io_4=tick%100;''')
        code=code.replace('valid && (value >= 10L) ? digits[(value / 10L) % 10]','valid ? digits[(value / 10L) % 10]')
        why='Use separate blink, color and decimal cycle lengths on the same two-second clock and preserve 00 through 99.'
    elif case=='embedbench_122':
        code=debounce_inputs(code,150,release_immediate=True)
        code=body(code,'''    static bool previous[3]={};
    static uint8_t bits=0;
    bool buttons[]={io_3,io_4,io_5};
    for (uint8_t i=0; i<3; ++i) {
        if (buttons[i] && !previous[i]) bits^=1U<<i;
        previous[i]=buttons[i];
    }
    *io_0=bits&1;
    *io_1=bits&2;
    *io_2=bits&4;
'''+''.join(f'    *io_6_segment_{i}=bits=={i+1};\n' for i in range(8))+'''    *io_7=*io_8=*io_9=false;''')
        why='Toggle each corresponding bit once after the 150 ms qualification and select exactly one numbered bar segment, with zero remaining dark.'
    elif case=='embedbench_125':
        code=debounce_inputs(code,25)
        code=body(code,'''    static bool previous=false,started=false;
    static uint8_t mode=0;
    static uint32_t changed=0;
    uint32_t now=millis();
    if (io_0 && !previous && (!started || (uint32_t)(now-changed)>=2000)) {
        mode=(mode+1)%4;
        started=true;
        changed=now;
    }
    previous=io_0;
    *io_1=mode==1;
    *io_2=mode==2;
    *io_3=mode==3;
    double *out[]={io_4,io_5,io_6,io_7,io_8,io_9,io_10,io_11,io_12};
    const uint8_t rainbow[]={3,6,5};
    uint32_t time=(uint32_t)(now-changed);
    for (uint8_t led=0;led<3;++led) {
        for (uint8_t channel=0;channel<3;++channel) {
            double value=0;
            if (mode==1 && channel==led) value=255;
            if (mode==3) value=192;
            if (mode==2) {
                unsigned phase=(time/1000+led)%3;
                double from=(rainbow[phase]&(1U<<channel)) ? 255 : 0;
                double to=(rainbow[(phase+1)%3]&(1U<<channel)) ? 255 : 0;
                value=from+(to-from)*(time%1000)/1000.0;
            }
            *out[3*led+channel]=value;
        }
    }''')
        why='Enforce each two-second mode dwell and interpolate the rainbow colors continuously while keeping all three discrete indicators synchronized.'
    elif case=='embedbench_60':
        code=body(code,'''    double red=io_0*255.0/1023.0;
    double green=io_1*255.0/1023.0;
    double blue=io_2*255.0/1023.0;
    *io_3=*io_6=red;
    *io_4=*io_7=green;
    *io_5=*io_8=blue;
    *io_9=10*(int)(io_0*9.0/1023.0)+(int)(io_2*9.0/1023.0);''')
        code=once(code,'valid && (value >= 10L) ? digits[(value / 10L) % 10]','valid ? digits[(value / 10L) % 10]')
        why='Map all RGB channels to PWM and pack the specified red and blue digits, keeping both zeros visible.'
    elif case=='embedbench_115':
        code=body(code,'''    unsigned step=millis()/2000UL;
    unsigned phase=step%10;
    unsigned digit=9-phase;
    bool active=step!=0 && digit!=0;
    unsigned color=step<10 ? (phase ? (phase-1)%3 : 0) : phase%3;
    *io_0=digit*11;
    *io_1=active && color==0;
    *io_2=active && color==1;
    *io_3=active && color==2;
    *io_4=*io_7=digit==0 || *io_1;
    *io_5=*io_8=digit==0 || *io_2;
    *io_6=*io_9=digit==0 || *io_3;''')
        code=once(code,'valid && (value >= 10L) ? digits[(value / 10L) % 10]','valid ? digits[(value / 10L) % 10]')
        why='Mirror each two-second countdown digit, preserve startup darkness, then show 00 with both RGB LEDs white before restart.'
    elif case=='embedbench_3':
        code=debounce_inputs(code,25)
        code=body(code,'    static uint32_t phase=0,last=millis();\n    static bool hold=false,off=false,previous[3]={};\n    uint32_t now=millis(),dt=now-last;last=now;\n    if(!hold&&!off) phase=(phase+dt)%6000UL;\n    if(io_3&&!previous[0]) hold=true;\n    if(io_4&&!previous[1]) {hold=false;off=false;}\n    if(io_5&&!previous[2]) off=true;\n    *io_0=!off&&phase<2000 ? 255 : 0;\n    *io_1=!off&&phase>=2000&&phase<4000 ? 255 : 0;\n    *io_2=!off&&phase>=4000 ? 255 : 0;\n    previous[0]=io_3;previous[1]=io_4;previous[2]=io_5;')
        why='Use real two-second color phases, preserve phase on hold/resume, and retain the OFF mode until resumed.'
    elif case=='embedbench_9':
        code=debounce_inputs(code,25)
        code=body(code,'    if(io_1&&!sysml_state_previousPressed) sysml_state_phaseIndex=(sysml_state_phaseIndex+1)%6;\n    sysml_state_previousPressed=io_1;\n    int count=sysml_state_phaseIndex*2;\n'+''.join(f'    *io_0_segment_{i}=count>{9-i};\n' for i in range(10)))
        why='Start empty, add exactly two LEDs per debounced press, and fill from the specified A10 end.'
    elif case=='embedbench_15':
        code=body(code,'    *io_0=(millis()/1000UL*6UL)%100UL;')
        matches=list(re.finditer(r'uint8_t segments = valid && .*?;',code));assert len(matches)==2
        for index,match in reversed(list(enumerate(matches))):
            expr='true' if index==0 else 'value >= 10L';divisor=1 if index==0 else 10
            code=code[:match.start()]+f'uint8_t segments = valid && ({expr}) ? digits[(value / {divisor}L) % 10] : 0;'+code[match.end():]
        why='Advance by six each real second, wrap to the last two digits, and map the blankable tens and always-visible units to the correct displays.'
    elif case=='embedbench_27':
        code=body(code,'    unsigned step=millis()/2000UL%20UL;\n    int count=step<=10 ? step : 20-step;\n    *io_0=count*18;*io_1=count*18;*io_2=count*18;\n'+''.join(f'    *io_3_segment_{i}=count>{i};\n' for i in range(10)))
        why='Advance all three servos together by 18 degrees each two seconds, reverse at endpoints, and derive the cumulative bar level from their sum.'
    elif case=='embedbench_36':
        code=body(code,'    *io_2=((int)(io_0*9.0/1023.0)+(int)(io_1*9.0/1023.0))%10;')
        why='Add the two independently mapped digits and retain only the units digit instead of blanking sums above nine.'
    elif case=='embedbench_40':
        code=body(code,'    int count=millis()/2000UL%10UL;\n    *io_0=count;\n'+''.join(f'    *io_{i+1}=count>{i};\n' for i in range(10)))
        why='Clock the shared digit/bar count once per two seconds instead of once per controller invocation.'
    elif case=='embedbench_48':
        code=append_body(code,'    *io_13=millis()/2000UL%2UL ? 180 : 0;')
        why='Supply the missing independent two-second alternation for the third servo.'
    elif case=='embedbench_80':
        code=debounce_inputs(code,25)
        code=append_body(code,''.join(f'    *io_2_segment_{i}=sysml_state_digit>{i};\n' for i in range(10)))
        why='Replace the one-hot bar indication with the cumulative number of segments matching the displayed digit.'
    elif case=='embedbench_89':
        code=debounce_inputs(code,25)
        code=append_body(code,'    int n=sysml_state_count;\n    *io_9=n%10;\n    *io_3=n>0&&(n<5||n>=10) ? 255 : 0;\n    *io_4=n>=5&&n<10 ? 255 : 0;\n    *io_5=n>=10 ? 255 : 0;\n    *io_6=n>=5&&n<10 ? 255 : 0;\n    *io_7=n>=5 ? 255 : 0;\n    *io_8=n>0&&(n<5||n>=10) ? 255 : 0;')
        why='Display the ones digit of the retained 0..15 count without changing the full count used for RGB range selection.'
    elif case=='embedbench_91':
        code=append_body(code,'    *io_0=millis()/2000UL%2UL==0;\n    *io_1=!*io_0;')
        why='Synchronize the LED alternation to the existing two-second servo stages instead of one-second LED halves.'
    elif case=='embedbench_93':
        code=body(code,'    static int previous=0,direction=0;\n    static uint32_t changed=0;\n    int angle=(int)(io_3*180.0/1023.0);\n    if(angle!=previous) {direction=angle>previous ? 1 : -1;changed=millis();previous=angle;}\n    if((uint32_t)(millis()-changed)>=2000) direction=0;\n    *io_0=direction>0;*io_1=direction<0;*io_2=angle;')
        why='Determine direction before updating the previous commanded angle, retain the indicator for the specified two seconds, then return to the stationary state.'
    elif case=='embedbench_99':
        code=body(code,'    static bool previousMax=false;\n    static uint32_t began=0;\n    bool maximum=io_1>=1023;\n    if(maximum&&!previousMax) began=millis();\n    *io_0=maximum ? (((millis()-began)/2000UL%2UL==0) ? 255 : 0) : io_1*255.0/1023.0;\n    previousMax=maximum;\n    int count=(int)(io_1*10.0/1023.0);\n'+''.join(f'    *io_2_segment_{i}=count>{i};\n' for i in range(10)))
        why='Calibrate proportional PWM and all ten bar segments, and use an actual two-seconds-on/two-seconds-off blink at full-scale input.'
    elif case=='embedbench_100':
        code=body(code,'    *io_0=io_3*255.0/1023.0;*io_1=io_4*255.0/1023.0;\n    int count=(int)(io_5*10.0/1023.0);\n'+''.join(f'    *io_2_segment_{i}=count>{i};\n' for i in range(10)))
        why='Independently calibrate both LED PWM commands and map the third raw ADC channel proportionally to zero through ten bar segments.'
    elif case=='advanced_006':
        code=debounce_inputs(code,25)
        code=body(code,'    static bool automatic=true,previous=false,forced=false;\n    static uint32_t began=0;\n    if(io_7&&!previous) {\n      if(automatic) {automatic=false;forced=false;}\n      else if(!forced) {forced=true;began=millis();}\n      else {forced=false;automatic=true;}\n    }\n    if(forced&&(uint32_t)(millis()-began)>=5000) forced=false;\n    bool water=io_2>=512;\n    bool pump=water && (automatic ? io_0<512 && io_1>=512 : forced);\n    *io_3=pump;*io_4=pump||!water;*io_5=water ? 0 : 1000;\n    unsigned page=millis()/2000UL%3;\n    *io_6=page==0 ? (int32_t)io_0 : (page==1 ? (int32_t)io_2 : (automatic ? 0 : 1));\n    previous=io_7;')
        why='Separate entering manual mode from the next press that requests a five-second run, enforce low-water inhibition in both modes, and rotate the display pages. Preserve the original ADC thresholds. An additional press during a forced run cancels it and returns to automatic mode; this manual-repair interaction convention is disclosed.'
    elif case=='complex_009':
        code=body(code,'    static uint32_t elapsed=0,last=millis(),beepStart=0;\n    static bool running=false,previous[2]={},beeping=false;\n    uint32_t now=millis(),dt=now-last;last=now;\n    uint32_t old=elapsed;\n    if(running) elapsed+=dt;\n    if(old<60000UL && elapsed>=60000UL) {beeping=true;beepStart=now;}\n    if(io_0&&!previous[0]) running=!running;\n    if(io_1&&!previous[1]) {elapsed=0;running=false;beeping=false;}\n    if(beeping && (uint32_t)(now-beepStart)>=1000) beeping=false;\n    *io_2=beeping ? 1000 : 0;*io_3=elapsed/1000UL;\n    previous[0]=io_0;previous[1]=io_1;')
        why='Count real elapsed time across start/pause/resume, reset cleanly, and retain the 60-second reminder tone for a full second.'
    elif case=='complex_013':
        assert '"LIGHT_ON") == 0) { *value = 6L;' in code
        code=body(code,'    static bool previous[2]={},beeping=false;\n    static uint32_t beepStart=0;\n    if(io_0&&!previous[0]) {sysml_state_lightOn=true;++sysml_state_count;}\n    if(io_1&&!previous[1]) {sysml_state_lightOn=false;++sysml_state_count;}\n    if(io_5_valid) {\n      if(io_5==6) {sysml_state_lightOn=true;++sysml_state_count;}\n      else if(io_5==5) {sysml_state_lightOn=false;++sysml_state_count;}\n      else if(io_5==1) {beeping=true;beepStart=millis();++sysml_state_count;}\n      else if(io_5==3) sysml_state_count=0;\n    }\n    if(beeping && (uint32_t)(millis()-beepStart)>=200) beeping=false;\n    *io_2=sysml_state_lightOn;*io_3=beeping ? 1000 : 0;*io_4=sysml_state_count;\n    previous[0]=io_0;previous[1]=io_1;')
        why='Connect all existing parsed commands to the retained light state and operation counter, and implement the missing BEEP pulse.'
    elif case=='medium_001':
        code=body(code,'    static bool previous=false,beeping=false;\n    static uint32_t began=0;\n    bool strong=io_0>800;\n    if(strong&&!previous) {began=millis();beeping=true;}\n    if(!strong || (uint32_t)(millis()-began)>=200) beeping=false;\n    *io_1=(1023.0-io_0)*255.0/1023.0;*io_2=strong;*io_3=beeping ? 1000 : 0;previous=strong;')
        why='Scale inverse brightness to 8-bit PWM and issue a short tone on entry to the explicitly defined strong-light range.'
    elif case=='medium_020':
        code=debounce_inputs(code,25,grounded=(2,))
        code=body(code,'''    static bool previous=false;
    if (io_1 && !previous) sysml_state_mode=(sysml_state_mode+1)%3;
    previous=io_1;
    *io_2=sysml_state_mode==0 ? (int32_t)(millis()/1000UL%10000UL) : (sysml_state_mode==1 ? (int32_t)io_0 : 1234);''')
        why='Replace the constant-zero count page with an advancing counter. With no specified counting source or period, this implementation uses elapsed seconds, disclosed as a manual-repair choice.'
    elif case=='medium_025':
        code=body(code,'    *io_1=io_0>0;\n    *io_2=(int32_t)(io_0*255.0/1023.0);')
        code,n=re.subn(r'digitalWrite\(9, hw_value_io_1 \? HIGH : LOW\);','analogWrite(9, (int)(hw_value_io_0*255.0/1023.0));',code)
        assert n==2,n
        why='Use proportional PWM instead of a Boolean LED command, and display its 0..255 brightness command.'
    elif case=='simple_013':
        code=body(code,'    sysml_state_accumulatedMs+=io_1;\n    *serial_0_trigger=sysml_state_accumulatedMs>=1000;\n    if(*serial_0_trigger) {sysml_state_accumulatedMs%=1000;sysml_state_lastTempForReport=io_0;}\n    *serial_0_value_0=sysml_state_lastTempForReport;')
        why='Capture the one-second DHT reporting event before wrapping its elapsed-time accumulator.'
    elif case=='medium_014':
        assert '"BUZZ"' in code
        code=body(code,'    static bool active=false;\n    static unsigned long began=0;\n    if(io_1_valid) {began=millis();active=true;}\n    if(active && (unsigned long)(millis()-began)>=500) active=false;\n    *io_0=active ? 1000 : 0;')
        why='Connect the parsed BUZZ event to a retriggerable 500ms tone instead of a constant-zero output.'
    elif case=='pceval_button_pulldown':
        code=body(code,'    static bool initialized=false,previous=false;\n    static unsigned long last=0;\n    bool report=!initialized || io_0!=previous || (unsigned long)(millis()-last)>=250;\n    if(report) {initialized=true;last=millis();}\n    *serial_0_trigger=report&&io_0;*serial_1_trigger=report&&!io_0;\n    *serial_0_value_0=io_0;*serial_1_value_0=!io_0;previous=io_0;')
        code=once(code,'if (hw_value_serial_0_trigger) { Serial.print("Button pressed"); Serial.println(hw_value_serial_0_value_0); }','if (hw_value_serial_0_trigger) Serial.println("Button pressed");')
        code=once(code,'if (hw_value_serial_1_trigger) { Serial.print("Button not pressed"); Serial.println(hw_value_serial_1_value_0); }','if (hw_value_serial_1_trigger) Serial.println("Button not pressed");')
        why='Continuously report the actual externally pulled-down button state, including stable states, using the required messages.'
    elif case=='pceval_button_duration':
        code=debounce_inputs(code,25)
        code=body(code,'    static bool previous=false;\n    static unsigned long began=0;\n    if(io_0&&!previous) began=millis();\n    *serial_0_trigger=!io_0&&previous;\n    *serial_0_value_0=(unsigned long)(millis()-began)/1000.0;\n    previous=io_0;')
        code=once(code,'if (hw_value_serial_0_trigger) { Serial.print("duration_s"); Serial.println(hw_value_serial_0_value_0); }','if (hw_value_serial_0_trigger) { Serial.print("Button pressed duration: "); Serial.print(hw_value_serial_0_value_0); Serial.println("s"); }')
        why='Measure elapsed milliseconds between debounced press and release rather than repeatedly assigning zero duration; emit the requested seconds format.'
    elif case=='pceval_humidity_sensor':
        code=body(code,'    static bool initialized=false;\n    static unsigned long last=0;\n    bool report=io_1 && (!initialized || (unsigned long)(millis()-last)>=250);\n    if(report) {initialized=true;last=millis();}\n    *serial_0_trigger=report&&io_0<50;*serial_1_trigger=report&&io_0>=50;\n    *serial_0_value_0=true;*serial_1_value_0=true;')
        code=once(code,'if (hw_value_serial_0_trigger) { Serial.print("below_threshold"); Serial.println(hw_value_serial_0_value_0); }','if (hw_value_serial_0_trigger) Serial.println("Humidity is below the threshold");')
        code=once(code,'if (hw_value_serial_1_trigger) { Serial.print("above_threshold"); Serial.println(hw_value_serial_1_value_0); }','if (hw_value_serial_1_trigger) Serial.println("Humidity is above the threshold");')
        why='Compare each valid sensor reading to the declared 50% threshold without requiring an unrelated serial-input event.'
    elif case=='pceval_temperature_sensor':
        code=body(code,'    static bool initialized=false;\n    static unsigned long last=0;\n    bool report=io_2 && (!initialized || (unsigned long)(millis()-last)>=250);\n    if(report) {initialized=true;last=millis();}\n    *serial_0_trigger=report;*serial_0_value_0=io_0>30;')
        code=once(code,'if (hw_value_serial_0_trigger) { Serial.print("above_threshold"); Serial.println(hw_value_serial_0_value_0); }','if (hw_value_serial_0_trigger) Serial.println(hw_value_serial_0_value_0 ? "Temperature is above the threshold" : "Temperature is below the threshold");')
        why='Report both current above-threshold and below-threshold states, including recovery after cooling.'
    elif case=='pceval_traffic_light':
        code=body(code,'    static bool previous=false,active=false;\n    static unsigned long began=0;\n    if(io_0&&!previous&&!active) {began=millis();active=true;}\n    unsigned long elapsed=millis()-began;\n    if(active&&elapsed>=3000) active=false;\n    *io_1=active&&elapsed>=1000;*io_2=active&&elapsed<1000;*io_3=!active;\n    previous=io_0;')
        why='Advance yellow and red states by elapsed time, retaining their required 1000ms and 2000ms intervals instead of exhausting all states in one update.'
    elif case=='pceval_7_segment_display_serial':
        code=body(code,'    static uint8_t digit=0;\n    if(io_8_valid&&io_8>=0&&io_8<=9) digit=io_8;\n    const uint8_t patterns[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};\n'+''.join(f'    *io_{i}=(patterns[digit]&{1<<i})!=0;\n' for i in range(7))+'    *io_7=false;*serial_0_trigger=io_8_valid;*serial_0_value_0=digit;')
        for pin in range(2,10):
            code,n=re.subn(r'digitalWrite\('+str(pin)+r', (hw_value_io_\d+) \? (?:HIGH : LOW|LOW : HIGH)\);',r'digitalWrite('+str(pin)+r', \1 ? LOW : HIGH);',code)
            assert n==2,(pin,n)
        why='Retain the most recent valid serial digit and apply its correct common-anode segment pattern between serial frames.'
    elif case=='pceval_serial_RGB_led':
        code='#include <string.h>\n'+code
        code=loop_body(code,'''  static char line[96];
  static uint8_t size=0,colors=0;
  static bool overflow=false;
  while(Serial.available()>0) {
    int ch=Serial.read();
    if(ch=='\\r') continue;
    if(ch=='\\n') {
      if(!overflow && size) {
        line[size]='\\0';colors=0;
        if(strstr(line,"Red")) colors|=1;
        if(strstr(line,"Green")) colors|=2;
        if(strstr(line,"Blue")) colors|=4;
      }
      size=0;overflow=false;
    } else if(size<sizeof(line)-1) line[size++]=(char)ch;
    else overflow=true;
  }
  digitalWrite(4,colors&1 ? LOW : HIGH);
  digitalWrite(3,colors&2 ? LOW : HIGH);
  digitalWrite(2,colors&4 ? LOW : HIGH);''')
        for pin in (2,3,4):
            code,n=re.subn(r'digitalWrite\('+str(pin)+r', hw_value_io_\d+ \? (?:HIGH : LOW|LOW : HIGH)\);',f'digitalWrite({pin}, HIGH);',code)
            assert n==1,(pin,n)
        why='Add the missing comma-list serial input path, retain selected channels, and drive the supplied common-anode RGB wiring correctly.'
    elif case=='pceval_serial_monitor':
        code=loop_body(code,'''  static char line[256];
  static unsigned size=0;
  static bool overflow=false;
  static unsigned long lastByte=0;
  while(Serial.available()>0) {
    int ch=Serial.read();lastByte=millis();
    if(ch=='\\r') continue;
    if(ch=='\\n') {
      if(!overflow && size) {line[size]='\\0';Serial.print("Received: ");Serial.println(line);}
      size=0;overflow=false;
    } else if(size<sizeof(line)-1) line[size++]=(char)ch;
    else overflow=true;
  }
  if(size && (unsigned long)(millis()-lastByte)>=20UL) {
    if(!overflow) {line[size]='\\0';Serial.print("Received: ");Serial.println(line);}
    size=0;overflow=false;
  }''')
        why='Replace constant unsolicited zero messages with bounded line-or-idle serial reception that echoes the actual received value.'
    elif case=='embedbench_64':
        code=debounce_inputs(code,25)
        code=body(code,'    static unsigned index=0;\n    static bool previous[2]={};\n    if(io_0&&!previous[0]) index=(index+1)%8;\n    if(io_1&&!previous[1]) index=(index+7)%8;\n    previous[0]=io_0;previous[1]=io_1;\n    const uint8_t colors[8]={0,1,2,4,3,6,5,7};\n    *io_2=colors[index]&1;*io_3=colors[index]&2;*io_4=colors[index]&4;')
        why='Use a debounced persistent color index with forward/backward navigation and correct RGB combinations.'
    elif case=='embedbench_65':
        code=debounce_inputs(code,150)
        code=body(code,'    static unsigned index=0,pending=0;\n    static unsigned long began=0;\n    static bool previous=false,started=false;\n    if(io_0&&!previous) pending=1;\n    if(pending && (!started || (unsigned long)(millis()-began)>=2000)) {index=(index+1)%8;pending=0;started=true;began=millis();}\n    previous=io_0;\n    const uint8_t colors[8]={0,1,2,4,3,6,5,7};\n    *io_1=colors[index]&1 ? 255 : 0;*io_2=colors[index]&2 ? 255 : 0;*io_3=colors[index]&4 ? 255 : 0;')
        why='Qualify stable 150ms presses, retain each color for at least two seconds, and emit full-scale PWM values.'
    elif case=='embedbench_66':
        code=debounce_inputs(code,25)
        code=body(code,'    static unsigned index=0;\n    static bool previous=false,pending=false,started=false;\n    static unsigned long began=0;\n    if(io_0&&!previous) pending=true;\n    if(pending && (!started || (unsigned long)(millis()-began)>=2000)) {index=(index+1)%5;pending=false;started=true;began=millis();}\n    previous=io_0;\n    const int angles[5][3]={{0,0,0},{90,45,135},{180,90,0},{45,135,90},{0,0,0}};\n    *io_1=angles[index][0];*io_2=angles[index][1];*io_3=angles[index][2];')
        why='Advance debounced servo states using elapsed milliseconds and enforce the two-second hold.'
    elif case=='embedbench_68':
        code=debounce_inputs(code,150,release_immediate=True)
        code=body(code,'    if(io_0) sysml_state_redLocked=io_3*255.0/1023.0;\n    if(io_1) sysml_state_greenLocked=io_4*255.0/1023.0;\n    if(io_2) sysml_state_blueLocked=io_5*255.0/1023.0;\n    *io_6=sysml_state_redLocked;*io_7=sysml_state_greenLocked;*io_8=sysml_state_blueLocked;')
        why='After the required 150ms stable press, update each held channel continuously with calibrated PWM and retain its last intensity on release.'
    elif case=='embedbench_69':
        code=debounce_inputs(code,25)
        code=body(code,'    if(io_1&&!sysml_state_previousPressed) sysml_state_selectedChannel=sysml_state_selectedChannel%3+1;\n    sysml_state_previousPressed=io_1;\n    double level=io_0*255.0/1023.0;\n    *io_2=sysml_state_selectedChannel==1 ? level : 0;\n    *io_3=sysml_state_selectedChannel==2 ? level : 0;\n    *io_4=sysml_state_selectedChannel==3 ? level : 0;')
        why='Start with all channels off, select red on the first debounced press, and continuously map ADC to PWM for only the selected channel.'
    elif case=='embedbench_70':
        code=body(code,'    static bool previous[3]={};\n    bool inputs[3]={io_1,io_2,io_3};\n    for(int i=0;i<3;i++) {if(inputs[i]&&!previous[i]) sysml_state_activePot=i+1;previous[i]=inputs[i];}\n    double values[3]={io_4,io_5,io_6};\n    *io_0=sysml_state_activePot ? values[sysml_state_activePot-1]*180.0/1023.0 : 0;')
        why='Use the most recent button edge to select the potentiometer, and divide its raw ADC reading by 1023 before commanding servo degrees.'
    elif case=='embedbench_73':
        code=debounce_inputs(code,25)
        code=body(code,'    static bool previous[3]={},started=false;\n    static int state=0,pending=0;\n    static unsigned long began=0;\n    bool inputs[3]={io_0,io_1,io_2};\n    for(int i=0;i<3;i++) {if(inputs[i]&&!previous[i]) pending=i+1;previous[i]=inputs[i];}\n    if(pending && (!started || (unsigned long)(millis()-began)>=2000)) {state=pending;pending=0;started=true;began=millis();}\n    *io_3=state*30;\n    int count=state==3 ? 10 : state*3;\n'+''.join(f'    *io_4_segment_{i}=count>{i};\n' for i in range(10)))
        why='Latch each debounced button-selected servo angle and cumulative bar pattern, holding it for two seconds before a queued change.'
    elif case=='embedbench_71':
        code=debounce_inputs(code,25)
        why='Debounce both button inputs before the existing latched servo-angle updates.'
    elif case=='embedbench_87':
        code=debounce_inputs(code,25)
        for color in ('red','green','blue'):
            code=once(code,f'sysml_state_{color}Intensity + 32;',f'(sysml_state_{color}Intensity <= 223 ? sysml_state_{color}Intensity + 32 : 255);')
        code=append_body(code,'    int latest=sysml_state_selectedColor==0 ? sysml_state_redIntensity : (sysml_state_selectedColor==1 ? sysml_state_greenIntensity : sysml_state_blueIntensity);\n    int segments=latest==255 ? 8 : latest/32;\n'+''.join(f'    *io_6_segment_{i}=segments>{i};\n' for i in range(8)))
        why='Filter mechanical button bounce before the existing per-channel increment and bar logic.'
    elif case=='complex_001':
        code=once(code,'sysml_state_alarmPhaseMs = (sysml_state_alarmPhaseMs + io_7);','sysml_state_alarmPhaseMs = (sysml_state_alarmPhaseMs + io_7)%1000;')
        why='Wrap the intermittent alarm phase so sustained alarms continue to produce repeated on/off bursts.'
    elif case=='complex_002':
        code=once(code,'((io_0 * 100.0))','((io_0 * 500.0))')
        why='Correct the LM35 calibration from ADC*100 to ADC*500 for the existing 5V reference.'
    elif case=='complex_008':
        code=append_body(code,'    unsigned long period = sysml_state_lastValidDistanceCm < 8 ? 250UL : 1000UL;\n    *io_4 = *io_4 && millis()%period < 100UL ? 1000 : 0;')
        why='Gate the existing near-distance tone into slower or faster recurring bursts instead of a continuous low-frequency carrier.'
    elif case=='complex_010':
        code=once(code,'sysml_state_lastTempC >= 30.0','sysml_state_lastTempC > 30.0')
        why='Honor the explicitly strict temperature-above-30 boundary.'
    elif case=='complex_011':
        code=append_body(code,'    bool phase = millis()%1000UL < 500UL;\n    *io_1 = *io_1 && phase;\n    *io_2 = *io_2 && phase ? 2000 : 0;')
        why='Apply a recurring shared alarm phase to the existing dark-and-near LED and buzzer outputs.'
    elif case=='complex_014':
        code=body(code,'    static unsigned long deadline=0;\n    if (io_0>sysml_state_maxAdc) sysml_state_maxAdc=io_0;\n    if (io_1 && !sysml_state_previousPressed) {deadline=millis()+3000UL;sysml_state_showMax=true;}\n    if (sysml_state_showMax && (int32_t)(millis()-deadline)>=0) sysml_state_showMax=false;\n    sysml_state_previousPressed=io_1;\n    *io_2=(int32_t)(sysml_state_showMax ? sysml_state_maxAdc : io_0);\n    *serial_0_trigger=true;\n    *serial_0_value_0=io_0; *serial_0_value_1=sysml_state_maxAdc;')
        code=once(code,'hw_value_io_0 = (analogRead(A1) * 1.0 + 0.0);','hw_value_io_0 = analogRead(A1)*500.0/1023.0;')
        why='Latch the historical maximum, enter its display mode on a press, expire it using real milliseconds after three seconds, and calibrate the LM35 input.'
    elif case=='complex_020':
        code=once(code,'sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs + io_3);','bool reportDue = sysml_state_elapsedAccumMs + io_3 >= 1000;\n    sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs + io_3);')
        code=append_body(code,'    *serial_0_trigger=reportDue;\n    *serial_1_trigger=*serial_1_trigger && reportDue;')
        code=once(code,'hw_value_io_1 = (analogRead(A1) * 1.0 + 0.0);','hw_value_io_1 = analogRead(A1)*500.0/1023.0;')
        why='Emit the data report once per second using a pre-wrap timer event, and convert the LM35 channel to Celsius. Original unspecified warning thresholds are retained.'
    elif case=='complex_022':
        code=once(code,'sysml_state_accumMs = (sysml_state_accumMs + io_5);','sysml_state_accumMs = (sysml_state_accumMs + io_5) % (sysml_state_breathSpeed > 0 ? sysml_state_breathSpeed : 1);')
        why='Wrap the breathing phase at the configured period so breathing repeats while the selected mode remains active.'
    elif case=='complex_023':
        code=append_body(code,'    *io_4 *= 255.0;')
        why='Convert normalized distance brightness to the adapter\'s 8-bit PWM command range.'
    elif case=='complex_026':
        code=append_body(code,'    if (sysml_state_mode==3) *io_4=(int32_t)(millis()/2000UL%2 ? sysml_state_dispHumi : sysml_state_dispTemp);')
        why='Alternate temperature and humidity in BOTH mode while preserving the existing serial command and button logic.'
    elif case=='complex_028':
        code=append_body(code,'    static bool wasHigh=false;\n    static unsigned long beepStart=0;\n    bool high=sysml_state_fanLevel==3;\n    if (high && !wasHigh) beepStart=millis();\n    wasHigh=high;\n    if (!high || (unsigned long)(millis()-beepStart)>=200UL) *io_3=0;')
        why='Limit the high-speed-entry tone to a 200ms short beep and rearm it after leaving high speed.'
    elif case=='complex_030':
        code=once(code,'sysml_state_reportAccumMs = (sysml_state_reportAccumMs + io_6);','bool reportDue = sysml_state_reportAccumMs + io_6 >= 2000;\n    sysml_state_reportAccumMs = (sysml_state_reportAccumMs + io_6);')
        code=append_body(code,'    *serial_0_trigger=reportDue;')
        why='Preserve the two-second report event before the accumulator is reduced below its threshold.'
    elif case=='embedbench_1':
        code=body(code,'    unsigned long phase = millis()%20000UL;\n    *io_0 = phase >= 5000 && phase < 15000;\n    *io_1 = phase >= 10000 && phase < 15000;')
        why='Keep LED1 on when LED2 joins at 10s; retain the five-second dark interval after 15s.'
    elif case=='embedbench_2':
        code=body(code,'    if (io_3 && !sysml_state_previousPressed) sysml_state_colorIndex = (sysml_state_colorIndex+1)%4;\n    sysml_state_previousPressed = io_3;\n    *io_0 = sysml_state_colorIndex==1 ? 255 : 0;\n    *io_1 = sysml_state_colorIndex==2 ? 255 : 0;\n    *io_2 = sysml_state_colorIndex==3 ? 255 : 0;')
        why='Align the off/red/green/blue state indices and use full-scale PWM commands.'
    elif case=='embedbench_4':
        code=body(code,'    unsigned phase = millis()/3000UL%4;\n    *io_0 = phase<3;\n    *io_1 = phase==1 || phase==2;\n    *io_2 = phase==2;')
        why='Replace constant OFF outputs with the required red/yellow/white/off three-second sequence.'
    elif case=='embedbench_7':
        code=body(code,'    *io_3 = io_0 >= io_1 ? (io_1 + (io_0-io_1)*io_2/1023.0)*255.0/1023.0 : 0;')
        why='Interpolate the third potentiometer between normalized brightness bounds without division by a zero bound span.'
    elif case=='embedbench_17':
        code=body(code,'    bool red = millis()/2000UL%2==0;\n    *io_0=red; *io_1=!red; *io_2=red; *io_3=false; *io_4=!red;')
        why='Drive the blue state as well as the red initialization, at two-second intervals.'
    elif case=='embedbench_12':
        code=body(code,'    static bool raw=false,stable=false;\n    static unsigned long changed=0;\n    if (io_1!=raw) {raw=io_1;changed=millis();}\n    if ((unsigned long)(millis()-changed)>=25 && stable!=raw) {\n      stable=raw; if (stable) sysml_state_count=(sysml_state_count+3)%12;\n    }\n    *io_0=sysml_state_count;')
        why='Debounce stable button changes for 25ms and advance exactly once through 0,3,6,9 per press.'
    elif case=='embedbench_19':
        code=body(code,'    unsigned long phase=millis()%9000UL;\n    *io_0=phase<1000UL || (phase>=3000UL && phase<6000UL);')
        why='Use one nine-second phase for the distinct 1s ON, 2s OFF, 3s ON, 3s OFF stages.'
    elif case=='embedbench_20':
        code=body(code,'    unsigned step=millis()/2000UL%36;\n    *io_0=(step<=18 ? step : 36-step)*10.0;')
        why='Remove duplicated endpoint dwell from the existing ten-degree sweep.'
    elif case=='embedbench_21':
        code=once(code,'            sysml_conditional_0 = 0;','            sysml_conditional_0 = 1;')
        why='Repeat from the required second sequence step instead of inserting an extra all-zero initialization step.'
    elif case in ('embedbench_33','embedbench_38','embedbench_41','embedbench_102'):
        assert code.count('digits[(value / 10L) % 10]')==1
        assert code.count('digits[(value / 1L) % 10]')==1
        code=code.replace('digits[(value / 10L) % 10]','digits[(value / SWAPL) % 10]').replace('digits[(value / 1L) % 10]','digits[(value / 10L) % 10]').replace('digits[(value / SWAPL) % 10]','digits[(value / 1L) % 10]')
        why='Swap only the two serialized decimal digit selections to match the actual register-to-display wiring.'
    elif case=='embedbench_35':
        code=body(code,'    *io_1=(int32_t)(io_0*99.0/1023.0);')
        assert code.count('digits[(value / 10L) % 10]')==1
        code=code.replace('digits[(value / 10L) % 10]','digits[(value / SWAPL) % 10]').replace('digits[(value / 1L) % 10]','digits[(value / 10L) % 10]').replace('digits[(value / SWAPL) % 10]','digits[(value / 1L) % 10]')
        why='Map the ADC endpoints to 00..99 and correct the two display digit assignments.'
    elif case=='embedbench_37':
        assert code.count('io_1 * 10.0')==3
        code=code.replace('io_1 * 10.0','io_1 * 9.0')
        code=code.replace('sysml_value_3 = (180.0 + ((double)((io_2 * 90.0)) / 1023.0));','sysml_value_3 = 180.0;')
        why='Map the single digit to 0..9, and clamp the unsupported third-servo angle range at 180 degrees as explicitly permitted.'
    elif case=='embedbench_101':
        code=body(code,'    *io_0=millis()/2000UL%2==0;\n    *io_1=!*io_0;\n    *io_2=9-(millis()/4000UL%10);')
        why='Keep zero visible for its countdown dwell and start the alternating LEDs in the required LED1 phase.'
    elif case=='embedbench_111':
        assert code.count('((double)(io_0 * 255.0)) / 1023.0')==3
        code=code.replace('((double)(io_0 * 255.0)) / 1023.0','255.0')
        why='Use full primary-color intensity in each selected range, as required by the task, rather than scaling the selected color again.'
    elif case=='embedbench_123':
        code=body(code,'    static bool raw=false,stable=false;\n    static unsigned long changed=0;\n    if(io_0!=raw) {raw=io_0;changed=millis();}\n    if((unsigned long)(millis()-changed)>=150 && stable!=raw) {\n      stable=raw;\n      if(stable && ++sysml_state_currentCounter>10) {\n        sysml_state_currentCounter=0;sysml_state_overflowCounter=(sysml_state_overflowCounter+1)%4;\n      }\n    }\n'+''.join(f'    *io_{i+1}=sysml_state_currentCounter>{i};\n' for i in range(10))+'    *io_11=(sysml_state_overflowCounter&1)!=0;\n    *io_12=(sysml_state_overflowCounter&2)!=0;')
        why='Increment overflow only on the eleventh press, after the ten-segment state has remained observable.'
    elif case=='embedbench_118':
        code=body(code,'    static double target=90,start=90,current=90;\n    static unsigned long began=0;\n    static bool moving=false;\n    bool up=io_1&&!sysml_state_previousBtn1,down=io_2&&!sysml_state_previousBtn2;\n    double next=target;\n    if(up && target<180) next=target+30;\n    if(down && target>0) next=target-30;\n    if(next!=target) {start=current;target=next;began=millis();moving=true;}\n    unsigned long elapsed=millis()-began;\n    if(moving) {if(elapsed>=2000) {current=target;moving=false;} else current=start+(target-start)*elapsed/2000.0;}\n    *io_0=moving;*io_3=current;\n    sysml_state_previousBtn1=io_1;sysml_state_previousBtn2=io_2;')
        why='Detect button edges before history updates, clamp 30-degree targets, and interpolate each motion over two seconds while holding its LED on.'
    elif case=='embedbench_119':
        code=body(code,'    static int mode=0,pending=0;\n    static unsigned long began=0;\n    bool first=io_3&&!sysml_state_previousBtn1,second=io_4&&!sysml_state_previousBtn2;\n    if(first) pending=1; if(second) pending=2;\n    if(first&&second) pending=3;\n    if(pending && (mode==0 || (unsigned long)(millis()-began)>=2000)) {\n      int event=pending;pending=0;\n      mode=event==3 || (mode==1&&event==2) || (mode==2&&event==1) ? 3 : event;\n      began=millis();\n    }\n    *io_0=mode==1 ? 90 : 0; *io_1=mode==2 ? 180 : 0;\n    *io_2=mode==1 || (mode==3 && (millis()-began)%4000UL<2000UL);\n    sysml_state_previousBtn1=io_3;sysml_state_previousBtn2=io_4;')
        why='Use persistent button-selected servo modes, a two-second state hold, and the explicit both-buttons reset/slow-flash state.'
    elif case=='medium_010':
        code=once(code,'digitalRead(2) == HIGH','digitalRead(2) == LOW')
        why='Normalize the existing pull-up override button so the default page is displayed while released.'
    elif case=='simple_006':
        code=body(code,'    sysml_state_cycleAccumMs = (sysml_state_cycleAccumMs + io_0) % 2000;\n    *io_1 = sysml_state_cycleAccumMs < 200 ? 1000 : 0;')
        why='Wrap the recurring buzzer phase modulo 2000ms; remove the ever-true total-elapsed branch.'
    elif case=='simple_009':
        code=body(code,'    static bool retained = false;\n    if (io_1_valid) retained = io_1 == 2;\n    *io_0 = retained;')
        why='Retain the last ON/OFF command between serial frames.'
    elif case=='simple_011':
        code=body(code,'    static double retained = 0;\n    if (io_1_valid) retained = io_1;\n    *io_0 = retained;\n    *serial_0_trigger = io_1_valid;\n    *serial_0_value_0 = io_1;')
        why='Retain the last valid PWM command until another valid command arrives.'
    elif case=='simple_015':
        code=once(code,'hw_value_io_0 = (analogRead(A2) * 1.0 + 0.0);','hw_value_io_0 = analogRead(A2) * 500.0 / 1023.0;')
        why='Convert LM35 ADC counts to Celsius with the declared 5V reference.'
    elif case=='simple_020':
        old='showNumberDec((int)hw_value_io_1, false, 4, 0)'
        assert code.count(old)==2
        code=code.replace(old,old.replace('false','true'))
        why='Enable four-digit leading zeros for the explicitly requested 0000 output.'
    elif case=='simple_021':
        code=body(code,'    *io_0 = millis() % 1000UL < 100UL ? 1000 : 0;')
        why='Replace the constant-zero tone command with the required recurring 100ms pulse.'
    elif case=='simple_025':
        code=once(code,'    sysml_value_0 = false;','    sysml_value_0 = sysml_state_flashing && (millis() % 1000UL < 500UL);')
        code=once(code,'digitalRead(2) == HIGH','digitalRead(2) == LOW')
        code=code.replace('pinMode(2, INPUT);','pinMode(2, INPUT_PULLUP);')
        why='Connect the existing toggled flashing state to a recurring LED phase.'
    elif case=='simple_028':
        code=body(code,'    static bool retained = false;\n    if (io_0) retained = true;\n    if (io_1) retained = false;\n    *io_2 = retained;')
        for pin in (2,3):code=once(code,f'digitalRead({pin}) == HIGH',f'digitalRead({pin}) == LOW')
        why='Normalize both pull-up buttons as active LOW and retain the commanded LED state.'
    elif case=='simple_030':
        code=body(code,'    bool pressed = io_0 && !sysml_state_previousIo0;\n    if (pressed) ++sysml_state_counter;\n    *serial_0_trigger = pressed;\n    *serial_0_value_0 = sysml_state_counter;\n    sysml_state_previousIo0 = io_0;')
        code=once(code,'digitalRead(2) == HIGH','digitalRead(2) == LOW')
        code=code.replace('pinMode(2, INPUT);','pinMode(2, INPUT_PULLUP);')
        why='Capture the press edge before updating the previous-button state; use that same edge to report the count.'
    elif case=='medium_003':
        code=once(code,'if (io_0 > 30.0)','if (io_2 * 500.0 / 1023.0 > 30.0)')
        code=once(code,'sysml_conditional_0 = 1.0;','sysml_conditional_0 = 255.0;')
        why='Use the connected A1 LM35 channel, convert Celsius and command full PWM instead of duty 1/255.'
    elif case=='medium_006':
        code=once(code,'hw_value_io_0 = (analogRead(A0) * 1.0 + 0.0);','hw_value_io_0 = analogRead(A0) * 9999.0 / 1023.0;')
        why='Scale the 10-bit potentiometer range to the specified 0..9999 display range.'
    elif case=='medium_008':
        code=once(code,'digitalRead(2) == HIGH','digitalRead(2) == LOW')
        why='Normalize the INPUT_PULLUP button as pressed when LOW; prevent a false startup count.'
    elif case=='medium_011':
        code=once(code,'        sysml_state_mode = io_1;','        sysml_state_mode = io_1;\n        sysml_state_phaseMs = 0;')
        code=once(code,'sysml_state_phaseMs = (sysml_state_phaseMs + io_2);','sysml_state_phaseMs = (sysml_state_phaseMs + io_2) % 2000;')
        why='Restart the fade phase on each mode command and wrap it continuously instead of exhausting a finite six-second sequence.'
    elif case=='medium_016':
        # Existing state ordering is preserved; only the missing flash output is supplied.
        start=code.index('    bool sysml_value_0 = 0;',code.index('/* Code of'))
        end=code.index('    *io_1 = sysml_value_0;',start)
        code=code[:start]+'    bool sysml_value_0 = sysml_state_mode == 1 ? millis()%1000UL < 500UL : sysml_state_mode == 2;\n'+code[end:]
        why='Give the flashing mode a recurring phase; keep the existing three-mode button transition logic.'
    elif case=='medium_026':
        code=body(code,'    unsigned long period = 200UL + (unsigned long)(io_0 * 1800.0 / 1023.0);\n    *io_1 = millis() % period < period / 2;')
        why='Implement the missing full-cycle period mapping from 0.2 to 2 seconds.'
    elif case=='medium_027':
        code=body(code,'    if (io_2) sysml_state_lastValidTemp = io_1;\n    sysml_state_accumulatedMs += io_0;\n    *serial_0_trigger = sysml_state_accumulatedMs >= 5000;\n    *serial_0_value_0 = sysml_state_lastValidTemp;\n    sysml_state_accumulatedMs %= 5000;')
        why='Capture the five-second report event before wrapping the timer, so the trigger remains reachable.'
    elif case=='pceval_smart_led_system':
        code,n=re.subn(r'(io_[0-4]) < 500\.0',r'\1 > 500.0',code)
        assert n==5,n
        why='Apply the explicit raw ADC >500 threshold independently to each corresponding LED.'
    elif case=='pceval_button_led':
        code=once(code,'digitalRead(2) == HIGH','digitalRead(2) == LOW')
        code=code.replace('pinMode(2, INPUT);','pinMode(2, INPUT_PULLUP);')
        why='Match the supplied ground-connected button wiring with active LOW sensing.'
    elif case=='pceval_button_pullup':
        # Supplied wiring is read from the existing adapter, not guessed from the task name.
        matches=re.findall(r'digitalRead\((\d+)\) == HIGH',code)
        assert len(matches)==1
        code=once(code,f'digitalRead({matches[0]}) == HIGH',f'digitalRead({matches[0]}) == LOW')
        code=body(code,'    static bool initialized=false,previous=false;\n    static uint32_t last=0;\n    bool report=!initialized||io_0!=previous||(uint32_t)(millis()-last)>=250;\n    if(report) {initialized=true;last=millis();}\n    *serial_0_trigger=report;*serial_0_value_0=io_0;previous=io_0;')
        code=once(code,'if (hw_value_serial_0_trigger) { Serial.print("button_pressed"); Serial.println(hw_value_serial_0_value_0); }','if (hw_value_serial_0_trigger) Serial.println(hw_value_serial_0_value_0 ? "Button pressed" : "Button not pressed");')
        why='Continuously report pressed for the externally pulled-up input when LOW, retaining the required external-resistor input mode.'
    elif case=='pceval_led_RGB_basic':
        code,n=re.subn(r'digitalWrite\(([234]), (hw_value_io_\d+) \? HIGH : LOW\)',r'digitalWrite(\1, \2 ? LOW : HIGH)',code)
        assert n==6,n
        why='Invert each output for the explicitly wired common-anode RGB LED.'
    elif case=='pceval_parking_spot_monitor':
        code=code.replace('digitalWrite(2,','digitalWrite(99,').replace('digitalWrite(4,','digitalWrite(2,').replace('digitalWrite(99,','digitalWrite(4,')
        why='Correct the swapped red and blue adapter pins while preserving the existing distance threshold logic.'
    elif case=='pceval_potentiometer_bar_led':
        code=body(code,'    int count = (int)(io_10 * 10.0 / 1023.0);\n'+''.join(f'    *io_{i} = count > {i};\n' for i in range(10)))
        why='Replace ten always-true comparisons with proportional cumulative bar thresholds, including both endpoints.'
    elif case=='pceval_7_segment_display_basic':
        # Keep the adapter and correct the actual electrical level for each named segment.
        for i,pin in enumerate(range(2,10)):
            on=bool(0x6d & (1<<i))
            code,n=re.subn(r'digitalWrite\('+str(pin)+r', [^;]+\);',f'digitalWrite({pin}, {"LOW" if on else "HIGH"});',code)
            assert n==2,(pin,n)
        why='Use the common-anode segment pattern for digit 5 and keep the decimal point off.'
    if why is not None:return code,why
    return None


def wiring_buttons(case, code):
    """Infer only direct button-to-rail wiring in the supplied original circuit."""
    root=Path(__file__).resolve().parents[2]
    manifest=json.loads((root/'experiment/formal/manifest.json').read_text(encoding='utf-8'))['cases']
    requirement=next(r['requirement'] for r in manifest if r['id']==case)
    marker=requirement.find('Given circuit')
    if marker<0:return code,[]
    start=requirement.find('{',marker)
    if start<0:return code,[]
    circuit,_=json.JSONDecoder().raw_decode(requirement[start:])
    buttons={p['id'] for p in circuit['parts'] if 'pushbutton' in p['type'].lower()}
    boards={p['id'] for p in circuit['parts'] if any(x in p['type'].lower() for x in ('arduino','mega'))}
    parent={}
    def net(x):
        parent.setdefault(x,x)
        if parent[x]!=x:parent[x]=net(parent[x])
        return parent[x]
    def join(a,b):parent[net(a)]=net(b)
    for a,b,*_ in circuit['connections']:join(a,b)
    for button in buttons:
        for terminal in ('1','2'):join(button+':'+terminal+'.l',button+':'+terminal+'.r')
    def external_pullup(pin):
        nodes=[n for n in parent if n.split(':')[0] in boards and n.split(':',1)[1]==pin]
        target={net(n) for n in nodes}
        rails={net(n) for n in parent if n.split(':')[0] in boards and n.split(':',1)[1].lower() in ('5v','3.3v')}
        for part in circuit['parts']:
            if 'resistor' not in part['type'].lower():continue
            a,b=net(part['id']+':1'),net(part['id']+':2')
            if (a in target and b in rails) or (b in target and a in rails):return True
        return False
    notes=[]
    for button in sorted(buttons):
        peers=[]
        for connection in circuit['connections']:
            a,b=connection[:2]
            if a.split(':')[0]==button:peers.append(b)
            if b.split(':')[0]==button:peers.append(a)
        rail=[];pins=[]
        for peer in peers:
            part,pin=peer.split(':',1)
            if part not in boards:continue
            if pin.lower().startswith('gnd'):rail.append('LOW')
            elif pin.lower() in ('5v','3.3v'):rail.append('HIGH')
            elif re.fullmatch(r'\d+|A\d+',pin):pins.append(pin)
        if len(set(rail))!=1:continue
        level=rail[0]
        for pin in pins:
            old=code
            code=re.sub(r'(digitalRead\('+re.escape(pin)+r'\) == )(?:HIGH|LOW)',r'\g<1>'+level,code)
            if level=='LOW' and not external_pullup(pin):code=code.replace(f'pinMode({pin}, INPUT);',f'pinMode({pin}, INPUT_PULLUP);')
            if code!=old:notes.append(f'{button} on {pin}: direct rail wiring requires pressed={level}')
    return code,notes


def previous_input_order(code):
    """Move an input-history update only when that history is read afterward."""
    marker=code.find('/* Code of ProgramPage1 */')
    if marker<0:return code,[]
    start=code.index('void ProgramPage1(',marker);lo=code.index('{',start)
    depth=1;hi=lo+1
    while depth:
        depth+=(code[hi]=='{')-(code[hi]=='}');hi+=1
    content=code[lo+1:hi-1]
    pattern=r'(?m)^\s*(sysml_state_(?:previous|prev)[A-Za-z0-9_]*) = (io_\d+);'
    fixes={}
    for match in re.finditer(pattern,content):
        variable,value=match.group(1,2)
        # Another assignment is not evidence of an edge-dependent read.
        tail=re.sub(pattern,'',content[match.end():])
        if re.search(r'\b'+variable+r'\b',tail):fixes[variable]=value
    for variable,value in fixes.items():
        content=re.sub(r'(?m)^\s*'+re.escape(variable)+r' = '+re.escape(value)+r';','',content)
        content+='\n    '+variable+' = '+value+';\n'
    return code[:lo+1]+content+code[hi-1:],list(fixes)


def internal_button_defaults(case,code):
    """Use the disclosed pull-up default only for ports explicitly called buttons."""
    if not case.startswith(('simple_','medium_','complex_','advanced_')):return code,[]
    root=Path(__file__).resolve().parents[2]
    path=root/'experiment/output/configured_audit_v1_20261015'/case/'repeat_1/contract.json'
    if not path.exists():return code,[]
    notes=[]
    for port in json.loads(path.read_text(encoding='utf-8'))['interfaces']:
        binding=port.get('binding',{})
        if binding.get('kind')!='digital_input' or not re.search('button|push|按钮|按键',port.get('meaning',''),re.I):continue
        pin=str(binding['pin']).removeprefix('D')
        old=code
        code=code.replace(f'digitalRead({pin}) == HIGH',f'digitalRead({pin}) == LOW')
        code=code.replace(f'pinMode({pin}, INPUT);',f'pinMode({pin}, INPUT_PULLUP);')
        if code!=old:notes.append(f'button {pin}: disclosed INPUT_PULLUP/active-LOW default')
    return code,notes
