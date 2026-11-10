"""Check the simulator's observed SPI clock against an independent SPISettings sketch."""
import gzip
import hashlib
import json
import os
from pathlib import Path
import shutil
import statistics
import subprocess
import sys

import run_picsimlab_all as sim


def main():
    version=sys.argv[1] if len(sys.argv)>1 else 'native-electrical-v5'
    interface=sys.argv[2] if len(sys.argv)>2 else 'spi'
    out=sim.OUT/'calibration'/f'{interface}_clock'/version;out.mkdir(parents=True,exist_ok=True)
    work=sim.BASE/f'{interface}_clock_calibration';sketch=work/'sketch';sketch.mkdir(parents=True,exist_ok=True)
    code='''#include <Arduino.h>
#include <SPI.h>
void setup(){SPI.begin();SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE0));pinMode(10,OUTPUT);digitalWrite(10,LOW);}
void loop(){SPI.transfer(0xA5);delayMicroseconds(10);}
'''
    if interface=='i2c':code='''#include <Arduino.h>
#include <Wire.h>
void setup(){Wire.begin();Wire.setClock(100000);}
void loop(){Wire.beginTransmission(0x68);Wire.write(0);Wire.endTransmission();delayMicroseconds(100);}
'''
    (sketch/'sketch.ino').write_text(code);(out/'reference_source.ino').write_text(code)
    arduino=Path.home()/'.cache/llm4mdd-arduino'
    if not (work/'build/sketch.ino.hex').exists():
        command=[str(arduino/'arduino-cli'),'--config-file',str(arduino/'native_config.yaml'),'compile','--fqbn','arduino:avr:uno','--jobs','1','--build-path',str(work/'build'),str(sketch)]
        result=subprocess.run(command,capture_output=True,timeout=240)
        (out/'compilation.log').write_bytes(result.stdout+result.stderr);assert result.returncode==0
    profile=work/'profile';profile.mkdir(exist_ok=True)
    (profile/'picsimlab.ini').write_text('picsimlab_version = "0.9.3"\npicsimlab_lab = "Arduino_Uno"\nArduino_Uno_proc = "atmega328p"\nArduino_Uno_clock = "16.0"\npicsimlab_debug = "0"\nspare_on = "1"\n')
    (work/'fixture.tsv').write_text('H 100 18446744073709551615\n'+('P 19 28\nP 18 27\n' if interface=='i2c' else 'P 13 19\nP 11 17\nP 10 16\n'))
    (work/'circuit.pcf').write_text('version,0,0,0:0.9.3\nspare_on,0,0,0:1\n')
    env=dict(os.environ,DISPLAY='',WAYLAND_DISPLAY='',PULSE_SERVER='',ALSOFT_DRIVERS='null',PICSIMLAB_USER_DATA=str(profile),
             PICSIMLAB_FIXTURE=str(work/'fixture.tsv'),PICSIMLAB_TRACE=str(work/'trace.tsv'))
    app=sim.BASE/'engines'/version/'picsimlab_NOGUI'
    if not app.exists():app.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(sim.BASE/'picsimlab-0.9.3/src/picsimlab_NOGUI',app)
    with (out/'simulator.log').open('wb') as log:
        result=subprocess.run([str(app),'Arduino_Uno','atmega328p',str(work/'build/sketch.ino.hex'),str(work/'circuit.pcf')],env=env,cwd=work,stdout=log,stderr=log,timeout=30)
    assert result.returncode==0
    rises=[];previous=0
    for line in (work/'trace.tsv').read_text().splitlines():
        f=line.split()
        if f[0]=='G' and f[2]==('19' if interface=='i2c' else '13'):
            value=int(f[3]);cycle=int(f[1])
            if value and not previous:rises.append(cycle)
            previous=value
    # Ignore the deliberate inter-byte delay; estimate clock within each SPI byte.
    periods=[b-a for a,b in zip(rises,rises[1:]) if 0<b-a<(2000 if interface=='i2c' else 150)]
    frequency=16000000/statistics.median(periods)
    target=100000 if interface=='i2c' else 1000000
    row=dict(requested_hz=target,observed_hz=frequency,median_cpu_cycles_per_clock=statistics.median(periods),
             within_one_percent=abs(frequency-target)<=target*.01,version=version,period_samples=len(periods))
    with (work/'trace.tsv').open('rb') as a,gzip.open(out/'electrical_trace.tsv.gz','wb') as b:shutil.copyfileobj(a,b)
    sim.save(out/'result.json',row);print(json.dumps(row),flush=True)


if __name__=='__main__':main()
