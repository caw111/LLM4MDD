"""Independent full Mega bargraph GPIO bank under simultaneous RGB PWM writes."""
import json,os,queue
import run_picsimlab_reference_preflight as ref
sim=ref.sim
CODE=r'''#include <Arduino.h>
const uint8_t pins[]={8,9,10,11,12,13,44,45};
void setup(){for(uint8_t p:pins)pinMode(p,OUTPUT);pinMode(5,OUTPUT);pinMode(6,OUTPUT);pinMode(7,OUTPUT);}
void loop(){unsigned phase=millis()/2000;uint8_t level=phase==0?32:phase==1?64:phase==2?224:phase==3?255:0;analogWrite(5,level);analogWrite(6,0);analogWrite(7,0);unsigned count=level==255?8:level/32;for(unsigned i=0;i<8;i++)digitalWrite(pins[i],i<count?HIGH:LOW);delay(5);}
'''
def main():
 os.nice(15);sim.VERSION='native-electrical-v14';sim.APP=sim.BASE/'engines'/sim.VERSION/'picsimlab_NOGUI';sim.RUN=sim.BASE/'gpio_bar_bank_reference'
 case=dict(id='calibration_full_bar_bank',dataset='calibration',fqbn='arduino:avr:mega',requirement='Independent full eight GPIO bank with RGB PWM',source_sha256='independent')
 checks=[]
 for t,n in [(1000,1),(3000,2),(5000,7),(7000,8),(9000,0)]:
  for i,p in enumerate([8,9,10,11,12,13,44,45]):checks.append(dict(id=f'bank_{t}_{p}',kind='gpio',pin=p,at_ms=t,expected=int(i<n),requirement_quote='Independent known GPIO bar length'))
 plan=dict(horizon_ms=10000,input_steps=[],checks=checks,reference_cpp=CODE)
 slots=queue.Queue();slots.put(143);row=ref.check(case,0,dict(name='full_bar_bank_independent_PWM',plan=plan),slots)
 sim.save(sim.OUT/'calibration/full_bar_bank'/sim.VERSION/'result.json',row);print(json.dumps(dict(status=row['status'],checks=row.get('checks'),error=row.get('error')),indent=2),flush=True)
if __name__=='__main__':main()
