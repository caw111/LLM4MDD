"""Independent RTC date at midnight; no floating timestamp conversion."""
import json,os,queue
import run_picsimlab_reference_preflight as ref
sim=ref.sim
CODE=r'''#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
RTC_DS3231 rtc;
void setup(){Serial.begin(9600);Wire.begin();rtc.begin();}
void loop(){DateTime now=rtc.now();Serial.print("DATE:");Serial.print(now.year());Serial.print('-');if(now.month()<10)Serial.print('0');Serial.print(now.month());Serial.print('-');if(now.day()<10)Serial.print('0');Serial.println(now.day());Serial.print("RAW:");Serial.println(now.unixtime());DateTime f((uint32_t)(double)now.unixtime());Serial.print("FLOAT_DAY:");Serial.println(f.day());delay(250);}
'''
def main():
 os.nice(15);sim.VERSION='native-electrical-v14';sim.APP=sim.BASE/'engines'/sim.VERSION/'picsimlab_NOGUI';sim.RUN=sim.BASE/'rtc_boundary_reference'
 original=next(c for c in sim.load(sim.OUT/'manifest.json')['cases'] if c['id']=='pceval_calendar_display')
 case=dict(original,id='calibration_calendar_boundary',source_sha256='independent')
 plan=dict(horizon_ms=4000,rtc_epoch_s=1709164798,input_steps=[dict(at_ms=0,kind='digital',pin=2,value=0),dict(at_ms=0,kind='digital',pin=3,value=0)],checks=[dict(id='date_before_midnight',kind='serial',from_ms=250,to_ms=750,contains='DATE:2024-02-28',min_count=1,requirement_quote='RTC starts on 2024-02-28 at 23:59:58'),dict(id='date_after_midnight',kind='serial',from_ms=2500,to_ms=3500,contains='DATE:2024-02-29',min_count=1,requirement_quote='RTC advances to the leap day after midnight')],reference_cpp=CODE)
 slots=queue.Queue();slots.put(142);row=ref.check(case,0,dict(name='integer_RTC_midnight_boundary',plan=plan),slots)
 sim.save(sim.OUT/'calibration/calendar_midnight_boundary'/sim.VERSION/'result.json',row);print(json.dumps(row,indent=2),flush=True)
if __name__=='__main__':main()
