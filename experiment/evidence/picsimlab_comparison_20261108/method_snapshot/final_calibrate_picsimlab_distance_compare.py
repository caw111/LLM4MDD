"""Standard Arduino integer pulseIn conversion on the same comparison inputs."""
import json,os,queue
import run_picsimlab_reference_preflight as ref
sim=ref.sim
CODE=r'''#include <Arduino.h>
void setup(){Serial.begin(9600);pinMode(3,OUTPUT);pinMode(2,INPUT);}
void loop(){if(!Serial.available())return;String input=Serial.readStringUntil('\n');long target=input.toInt();digitalWrite(3,LOW);delayMicroseconds(2);digitalWrite(3,HIGH);delayMicroseconds(10);digitalWrite(3,LOW);long distance=pulseIn(2,HIGH,30000)/58;Serial.print("REFERENCE_DISTANCE:");Serial.println(distance);if(distance>target)Serial.println("Measured distance is greater");else if(distance==target)Serial.println("Measured distance is equal to the target");else Serial.println("Measured distance is less");}
'''
def main():
 os.nice(15);sim.VERSION='native-electrical-v14';sim.APP=sim.BASE/'engines'/sim.VERSION/'picsimlab_NOGUI';sim.RUN=sim.BASE/'distance_compare_reference'
 case=dict(id='calibration_distance_compare',dataset='calibration',fqbn='arduino:avr:uno',requirement='Independent integer-centimeter distance comparison',source_sha256='independent')
 plan=sim.load(sim.OUT/'binary_review_34_20261108/pceval_distance_comparison.json')[0]['plan'];plan['reference_cpp']=CODE
 slots=queue.Queue();slots.put(141);row=ref.check(case,0,dict(name='standard_pulseIn_integer_conversion',plan=plan),slots)
 sim.save(sim.OUT/'calibration/distance_integer_comparison'/sim.VERSION/'result.json',row);print(json.dumps(dict(status=row['status'],checks=row.get('checks'),error=row.get('error')),indent=2),flush=True)
if __name__=='__main__':main()
