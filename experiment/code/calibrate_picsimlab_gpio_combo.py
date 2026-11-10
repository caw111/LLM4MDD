"""Independent Mega control: PWM and ordinary GPIO on the same timer port."""
import json,os,queue,sys
import run_picsimlab_reference_preflight as ref
sim=ref.sim
CODE='#include <Arduino.h>\nvoid setup(){Serial.begin(9600);pinMode(2,INPUT_PULLUP);pinMode(5,OUTPUT);pinMode(6,OUTPUT);pinMode(7,OUTPUT);pinMode(8,OUTPUT);Serial.print("ADC:");Serial.println(analogRead(A0));Serial.print("BUTTON:");Serial.println(digitalRead(2));}void loop(){analogWrite(5,32);analogWrite(6,0);analogWrite(7,0);digitalWrite(8,HIGH);delay(5);}\n'
def main():
 os.nice(15);version=sys.argv[1];sim.VERSION=version;sim.APP=sim.BASE/'engines'/version/'picsimlab_NOGUI';sim.RUN=sim.BASE/'gpio_combo_calibration'/version
 case=dict(id='calibration_gpio_combo',dataset='calibration',fqbn='arduino:avr:mega',requirement='Independent PWM and GPIO calibration',source_sha256='independent')
 checks=[dict(id='gpio_'+str(t),kind='gpio',pin=8,at_ms=t,expected=1,requirement_quote='Ordinary GPIO remains HIGH while neighboring PWM is active') for t in [500,1500,2500]]
 checks += [dict(id='pwm',kind='pwm',pin=5,at_ms=1500,expected=32,tolerance=2,requirement_quote='Independent analogWrite32 control'),dict(id='adc',kind='serial',from_ms=0,to_ms=500,regex='^ADC:512$',min_count=1,requirement_quote='ADC initial input is 512'),dict(id='button',kind='serial',from_ms=0,to_ms=500,regex='^BUTTON:1$',min_count=1,requirement_quote='Initial physical button is released HIGH')]
 plan=dict(horizon_ms=3000,input_steps=[dict(at_ms=0,kind='analog',pin='A0',value=512),dict(at_ms=0,kind='digital',pin=2,value=1)],checks=checks,reference_cpp=CODE)
 slots=queue.Queue();slots.put(140);row=ref.check(case,0,dict(name='independent_gpio_pwm_adc_startup',plan=plan),slots)
 sim.save(sim.OUT/'calibration/gpio_pwm_combo'/version/'result.json',row);print(json.dumps(dict(status=row['status'],checks=row.get('checks'),error=row.get('error')),indent=2),flush=True)
if __name__=='__main__':main()
