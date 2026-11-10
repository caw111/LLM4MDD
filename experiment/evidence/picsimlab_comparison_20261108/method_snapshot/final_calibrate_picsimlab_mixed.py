"""Independent mixed-peripheral positive/negative control, never a dataset candidate."""
import gzip,hashlib,json,shutil,subprocess,sys
from pathlib import Path
import run_picsimlab_all as sim

CODE=r'''#include <Arduino.h>
#include <Servo.h>
#include <DHT.h>
#include <TM1637Display.h>
Servo actuator;DHT sensor(2,DHT22);TM1637Display display(4,5);
unsigned long last=0,lastDht=0;
void setup(){Serial.begin(9600);pinMode(8,OUTPUT);pinMode(7,INPUT);pinMode(13,OUTPUT);actuator.attach(9);sensor.begin();display.setBrightness(7);tone(6,1000);}
void loop(){
 if(millis()-last<500)return;last=millis();
 int raw=analogRead(A0);actuator.write(map(raw,0,1023,0,180));display.showNumberDec(raw);
 digitalWrite(8,LOW);delayMicroseconds(2);digitalWrite(8,HIGH);delayMicroseconds(10);digitalWrite(8,LOW);
 unsigned long width=pulseIn(7,HIGH,30000);
 Serial.print("ADC:");Serial.println(raw);Serial.print("Distance:");Serial.println(width/58.0,2);
 if(millis()-lastDht>=2000){lastDht=millis();Serial.print("Temperature:");Serial.println(sensor.readTemperature(),2);Serial.print("Humidity:");Serial.println(sensor.readHumidity(),2);}
 digitalWrite(13,(millis()/1000)%2);
}
'''

def main():
 version=sys.argv[1];cid='calibration_mixed_uno'
 sim.VERSION=version;sim.APP=sim.BASE/'engines'/version/'picsimlab_NOGUI';sim.RUN=sim.BASE/'mixed_calibration_workers'/version
 out=sim.OUT/'cases'/cid;out.mkdir(parents=True,exist_ok=True)
 work=sim.BASE/'mixed_calibration_compiler';sketch=work/'sketch';sketch.mkdir(parents=True,exist_ok=True)
 (sketch/'sketch.ino').write_text(CODE);(out/'reference_source.ino').write_text(CODE)
 arduino=Path.home()/'.cache/llm4mdd-arduino';stamp=work/'compiled_source.txt'
 if not (work/'build/sketch.ino.hex').exists() or not stamp.exists() or stamp.read_text()!=CODE:
  result=subprocess.run([str(arduino/'arduino-cli'),'--config-file',str(arduino/'native_config.yaml'),'compile','--fqbn','arduino:avr:uno','--jobs','1','--build-path',str(work/'build'),str(sketch)],capture_output=True,timeout=240)
  (out/'compilation.log').write_bytes(result.stdout+result.stderr);assert result.returncode==0;stamp.write_text(CODE)
 nm=arduino/'data/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-nm'
 (out/'symbols.txt').write_bytes(subprocess.run([str(nm),'-n',str(work/'build/sketch.ino.elf')],capture_output=True,check=True).stdout)
 for name in ['sketch.ino.hex','sketch.ino.elf']:shutil.copyfile(work/'build'/name,out/name)
 digest=hashlib.sha256(CODE.encode()).hexdigest();hexsha=hashlib.sha256((out/'sketch.ino.hex').read_bytes()).hexdigest()
 case=dict(id=cid,dataset='calibration',fqbn='arduino:avr:uno',requirement='Independent DHT22 A0 Servo TM1637 CLK=D4 DIO=D5 HC-SR04 calibration',source_sha256=digest,_symbols_path=str(out/'symbols.txt'),_firmware_path=str(out/'sketch.ino.hex'),_compiled_override=dict(compile_ok=True,source_sha256=digest,hex=dict(sha256=hexsha)))
 checks=[];inputs=[]
 def add(kind,**kw):checks.append(dict(id='calibration_'+str(len(checks)),kind=kind,requirement_quote='Independent calibrated fixture input must match actual peripheral output',**kw))
 for i,(adc,dist,temp,hum) in enumerate([(512,34,21.2,55.5),(1023,80,32.5,65),(0,120,15,40)]):
  t=i*4000
  inputs.extend([dict(at_ms=t,kind='analog',pin='A0',value=adc),dict(at_ms=t,kind='distance',pin=7,value=dist),dict(at_ms=t,kind='temperature',pin=2,value=temp),dict(at_ms=t,kind='humidity',pin=2,value=hum)])
  add('servo',pin=9,at_ms=t+3000,expected=int(adc*180/1023),tolerance=3)
  add('display',name='tm:4:5',at_ms=t+3000,expected=adc,numeric=True)
  # pulseIn's cycle-counting loop excludes time spent in the active tone/Servo
  # interrupts. Validate the independent sensor pulse below, and disclose a 3%
  # library-output allowance for this intentionally interrupt-loaded sketch.
  for prefix,value,tol in [('ADC:',adc,1),('Distance:',dist,max(1,dist*.03)),('Temperature:',temp,.11),('Humidity:',hum,.11)]:add('serial',from_ms=t+1500,to_ms=t+3900,prefix=prefix,expected=value,tolerance=tol,min_count=1)
 add('tone',pin=6,at_ms=1700,expected=1000,tolerance=20)
 plan=dict(horizon_ms=12500,input_steps=inputs,ultrasonic=[dict(trigger_pin=8,echo_pin=7)],checks=checks)
 row=sim.run_plan(case,dict(name='independent_mixed_positive_control',plan=plan),0,0)
 result=dict(version=version,positive_status=row['status'],failed_checks=[c for c in row.get('checks',[]) if not c['passed']])
 if row.get('result_directory'):
  folder=sim.OUT/row['result_directory']
  with gzip.open(folder/'observations.jsonl.gz','rt') as f:trace=[json.loads(line) for line in f]
  echo=[];rise=None
  for e in trace:
   if e['type']!='gpio' or e['pin']!=7:continue
   if e['value']:rise=e['at_us']
   elif rise is not None:
    if e['at_ms']>100:echo.append(dict(at_ms=e['at_ms'],pulse_us=e['at_us']-rise))
    rise=None
  for e in echo:
   distance=34 if e['at_ms']<4000 else 80 if e['at_ms']<8000 else 120
   e['nominal_us']=distance*58.0720092915;e['error_us']=e['pulse_us']-e['nominal_us']
  result['electrical_echo_widths_passed']=bool(echo) and all(abs(e['error_us'])<=max(2,e['nominal_us']*.01) for e in echo)
  result['maximum_echo_relative_error']=max((abs(e['error_us'])/e['nominal_us'] for e in echo),default=None)
  result['echo_samples']=echo
  result['distance_reporting_allowance']='max(1 cm,3%); standard pulseIn cycle counting under concurrent tone and Servo interrupts'
  negative=dict(plan,checks=[dict(id='wrong_servo',kind='servo',pin=9,at_ms=3000,expected=180,tolerance=3,requirement_quote='Deliberately false control'),dict(id='wrong_display',kind='display',name='tm:4:5',at_ms=3000,expected=9999,numeric=True,requirement_quote='Deliberately false control'),dict(id='wrong_serial',kind='serial',from_ms=2200,to_ms=3900,prefix='ADC:',expected=9999,min_count=1,requirement_quote='Deliberately false control')])
  rejected=sim.evaluate(negative,trace,case['fqbn']);result['negative_controls_all_rejected']=all(not c['passed'] and not c.get('unverified') for c in rejected['checks'])
  sim.save(folder/'negative_control_results.json',rejected)
 sim.save(sim.OUT/'calibration/mixed_peripherals'/version/'result.json',result);print(json.dumps(result,indent=2),flush=True)
if __name__=='__main__':main()
