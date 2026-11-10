"""Independent Arduino analogWrite calibration on all Uno/Mega PWM outputs."""
import gzip,hashlib,json,os,shutil,statistics,subprocess,sys
from pathlib import Path
import run_picsimlab_all as sim

def main():
 version=sys.argv[1];board=sys.argv[2] if len(sys.argv)>2 else 'mega'
 sim.VERSION=version;sim.APP=sim.BASE/'engines'/version/'picsimlab_NOGUI';sim.RUN=sim.BASE/'pwm_calibration_workers'/version/board
 pins=list(range(2,14))+[44,45,46] if board=='mega' else [3,5,6,9,10,11]
 fqbn='arduino:avr:'+board
 cid='calibration_pwm_'+board
 out=sim.OUT/'cases'/cid;out.mkdir(parents=True,exist_ok=True)
 work=sim.BASE/'pwm_calibration_compiler'/board;sketch=work/'sketch';sketch.mkdir(parents=True,exist_ok=True)
 code='#include <Arduino.h>\nint pins[]={'+','.join(map(str,pins))+'};\nvoid setup(){for(int p:pins)pinMode(p,OUTPUT);}\nvoid loop(){int values[]={64,128,192,255};for(int value:values){unsigned long started=millis();while(millis()-started<1000){for(int p:pins)analogWrite(p,value);}}}\n'
 (sketch/'sketch.ino').write_text(code);(out/'reference_source.ino').write_text(code)
 arduino=Path.home()/'.cache/llm4mdd-arduino'
 stamp=work/'compiled_source.txt'
 if not (work/'build/sketch.ino.hex').exists() or not stamp.exists() or stamp.read_text()!=code:
  result=subprocess.run([str(arduino/'arduino-cli'),'--config-file',str(arduino/'native_config.yaml'),'compile','--fqbn',fqbn,'--jobs','1','--build-path',str(work/'build'),str(sketch)],capture_output=True,timeout=240)
  (out/'compilation.log').write_bytes(result.stdout+result.stderr);assert result.returncode==0
  stamp.write_text(code)
 nm=arduino/'data/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-nm'
 (out/'symbols.txt').write_bytes(subprocess.run([str(nm),'-n',str(work/'build/sketch.ino.elf')],capture_output=True,check=True).stdout)
 for name in ['sketch.ino.hex','sketch.ino.elf']:shutil.copyfile(work/'build'/name,out/name)
 digest=hashlib.sha256(code.encode()).hexdigest();hexsha=hashlib.sha256((out/'sketch.ino.hex').read_bytes()).hexdigest()
 case=dict(id=cid,dataset='calibration',fqbn=fqbn,requirement='Independent PWM calibration',source_sha256=digest,_symbols_path=str(out/'symbols.txt'),_firmware_path=str(out/'sketch.ino.hex'),_compiled_override=dict(compile_ok=True,source_sha256=digest,hex=dict(sha256=hexsha)))
 checks=[dict(id=f'p{p}_{n}',kind='pwm',pin=p,at_ms=500+i*1000,expected=n,tolerance=3,requirement_quote='Arduino analogWrite duty control') for i,n in enumerate([64,128,192,255]) for p in pins]
 plan=dict(horizon_ms=4200,input_steps=[],checks=checks)
 row=sim.run_plan(case,dict(name='independent_PWM_calibration',plan=plan),0,0)
 result=dict(status=row['status'],failed=[dict(pin=c['test']['pin'],at_ms=c['test']['at_ms'],expected=c['test']['expected'],actual=c['actual']) for c in row.get('checks',[]) if not c['passed']],frequencies=[])
 if row.get('result_directory'):
  folder=sim.OUT/row['result_directory']
  with gzip.open(folder/'observations.jsonl.gz','rt') as f:events=[json.loads(line) for line in f]
  for pin in pins:
   rises=[e['at_ms'] for e in events if e['type']=='gpio' and e['pin']==pin and e['value'] and 400<e['at_ms']<900]
   periods=[b-a for a,b in zip(rises,rises[1:])]
   observed=1000/statistics.median(periods) if periods else None
   fast=pin in ([4,13] if board=='mega' else [5,6]);expected=16000000/(64*(256 if fast else 510))
   result['frequencies'].append(dict(pin=pin,expected_hz=expected,observed_hz=observed,within_one_percent=observed is not None and abs(observed-expected)<=expected*.01))
  result['frequency_calibration_passed']=all(r['within_one_percent'] for r in result['frequencies'])
 sim.save(sim.OUT/'calibration'/('pwm_'+board)/version/'result.json',result)
 print(json.dumps(result,indent=2),flush=True)
if __name__=='__main__':main()
