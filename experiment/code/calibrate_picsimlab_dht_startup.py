"""Independent check that the time-zero sensor value precedes a startup read."""
import json,os,queue,sys
import run_picsimlab_reference_preflight as ref
sim=ref.sim
CODE='#include <Arduino.h>\n#include <DHT.h>\nDHT dht(7,DHT11);void setup(){Serial.begin(9600);dht.begin();Serial.print("INITIAL:");Serial.println(dht.readTemperature(),2);}void loop(){}\n'
def main():
 os.nice(15);version=sys.argv[1];sim.VERSION=version;sim.APP=sim.BASE/'engines'/version/'picsimlab_NOGUI';sim.RUN=sim.BASE/'startup_dht_calibration'/version
 case=dict(id='calibration_dht_startup',dataset='calibration',fqbn='arduino:avr:uno',requirement='DHT11 data D7',source_sha256='independent_control')
 plan=dict(horizon_ms=3000,input_steps=[dict(at_ms=0,kind='temperature',pin=7,value=32),dict(at_ms=0,kind='humidity',pin=7,value=50)],checks=[dict(id='initial_value',kind='serial',from_ms=0,to_ms=800,regex='^INITIAL:32[.]00$',min_count=1,requirement_quote='Time-zero physical temperature is 32 C before the first read')],reference_cpp=CODE)
 slots=queue.Queue();slots.put(0);row=ref.check(case,0,dict(name='startup_DHT_initial_state',plan=plan),slots)
 sim.save(sim.OUT/'calibration/dht_startup'/version/'result.json',row);print(json.dumps(dict(status=row['status'],checks=row.get('checks'),error=row.get('error')),indent=2),flush=True)
if __name__=='__main__':main()
