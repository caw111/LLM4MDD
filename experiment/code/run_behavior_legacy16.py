"""Re-execute the fixed, previously validated sixteen requirement-specific oracles."""
import concurrent.futures
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

import run_contract_development as legacy
import behavior_full_host as host

ROOT=host.ROOT
OUT=host.OUT/'legacy16'
GXX=Path('E:/mingw64/mingw64/bin/g++.exe')
AR=Path('E:/mingw64/mingw64/bin/ar.exe')

CAPTURE=r'''
#include <fstream>
std::string legacy_trace_path;
std::string trace_esc(const std::string&s){std::string r="\"";for(unsigned char c:s){if(c=='"'||c=='\\'){r+='\\';r+=c;}else if(c=='\t')r+="\\t";else if(c=='\r')r+="\\r";else if(c=='\n')r+="\\n";else if(c>=32)r+=c;}return r+'"';}
struct LegacyTraceDump{~LegacyTraceDump(){if(legacy_trace_path.empty())return;std::ofstream out(legacy_trace_path);
 int level[PIN_COUNT]={};uint16_t shift=0,prev=65535;bool had=false;int clk=0,latch=0,data=0;
 for(auto&e:oracle::events){
  bool bus=oracle::task==116||oracle::task==179||oracle::task==204;
  if(bus){if(e.pin==A0)data=e.level!=0;if(e.pin==A2){if(e.level&&!clk)shift=uint16_t((shift<<1)|data);clk=e.level!=0;}if(e.pin==A1){if(e.level&&!latch&&(!had||shift!=prev)){out<<"{\"type\":\"shift_frame\",\"at_ms\":"<<e.at<<",\"bits\":"<<shift<<"}\n";prev=shift;had=true;}latch=e.level!=0;}if(e.pin==A0||e.pin==A1||e.pin==A2)continue;}
  out<<"{\"type\":\"gpio\",\"at_ms\":"<<e.at<<",\"at_us\":"<<e.micro_at<<",\"pin\":"<<e.pin<<",\"value\":"<<e.level<<",\"pwm\":"<<(e.pwm?"true":"false")<<"}\n";}
 for(auto&e:oracle::serial_lines)out<<"{\"type\":\"serial\",\"at_ms\":"<<e.at<<",\"text\":"<<trace_esc(e.text)<<"}\n";
 for(auto&e:oracle::servos)out<<"{\"type\":\"servo\",\"at_ms\":"<<e.at<<",\"pin\":"<<e.pin<<",\"pulse_us\":"<<e.pulse<<"}\n";
 for(auto&e:oracle::tones)out<<"{\"type\":\"tone\",\"at_ms\":"<<e.at<<",\"pin\":"<<e.pin<<",\"frequency\":"<<e.frequency<<"}\n";
 for(auto&e:oracle::displays){out<<"{\"type\":\"display\",\"at_ms\":"<<e.at<<",\"clk\":"<<e.clk<<",\"dio\":"<<e.dio<<",\"segments\":[";for(int i=0;i<4;++i){if(i)out<<',';out<<int(e.segments[i]);}out<<"]}\n";}
 out<<"{\"type\":\"capture\",\"virtual_ms\":"<<oracle::now<<",\"raw_gpio_calls\":"<<oracle::events.size()<<"}\n";
}};static LegacyTraceDump legacy_trace_dump;
'''


def one(job):
    key,row=job;folder=OUT/'executions'/key;folder.mkdir(parents=True,exist_ok=True)
    source=Path(row['source']);cpp=source.parent.parent/'build/sketch/sketch.ino.cpp'
    result=dict(case=row['case'],source=str(source),source_sha256=host.sha(source),cpp_sha256=host.sha(cpp),evidence=str(folder),coverage='legacy16_public_acceptance')
    assert result['source_sha256']==row['source_sha256']
    macro='ARDUINO_AVR_MEGA2560' if 'mega' in row['board'] else 'ARDUINO_AVR_UNO'
    command=[str(GXX),'-std=c++17','-D'+macro,'-I'+str(OUT/'fixture'),str(cpp),str(OUT/'fixture/oracle.cpp'),str(OUT/'fixture/ArduinoHooks.a'),'-o',str(folder/'program.exe')]
    try:
        build=subprocess.run(command,capture_output=True,timeout=90);(folder/'build.stderr.txt').write_bytes(build.stderr)
        if build.returncode:result['status']='unverified_host_build'
        else:
            run=subprocess.run([str(folder/'program.exe'),str(legacy.ORACLE_TASKS[row['case']]),str(folder/'trace.jsonl')],capture_output=True,timeout=25)
            (folder/'stdout.txt').write_bytes(run.stdout);(folder/'stderr.txt').write_bytes(run.stderr)
            result.update(status='passed' if run.returncode==0 else 'unverified_runtime' if run.returncode==3 else 'failed',exit_code=run.returncode,
                          assertion_diagnostic=run.stderr.decode('utf-8',errors='replace'))
    except subprocess.TimeoutExpired:result['status']='unverified_timeout'
    host.save(folder/'result.json',result);return result


def main():
    fixture=OUT/'fixture';fixture.mkdir(parents=True,exist_ok=True)
    original=ROOT/'experiment/development/oracles'
    for name in ['TM1637Display.h','Servo.h','DHT.h','mixed_environment_trace.h','ArduinoHooks.cpp']:
        shutil.copyfile(original/name,fixture/name)
    shutil.copyfile(original/'gpio_arduino.h',fixture/'Arduino.h')
    oracle=(original/'gpio_trace.cpp').read_text()
    marker='int main(int argc,char** argv) {'
    oracle=oracle.replace(marker,CAPTURE+'\n'+marker+'\n if(argc>=3)legacy_trace_path=argv[2];').replace('if(argc!=2) return 2;','if(argc<2) return 2;')
    (fixture/'oracle.cpp').write_text(oracle)
    subprocess.run([str(GXX),'-std=c++17','-c',str(fixture/'ArduinoHooks.cpp'),'-o',str(fixture/'ArduinoHooks.o')],check=True,capture_output=True)
    subprocess.run([str(AR),'rcs',str(fixture/'ArduinoHooks.a'),str(fixture/'ArduinoHooks.o')],check=True,capture_output=True)
    host.save(OUT/'test_spec.json',dict(tasks=legacy.ORACLE_TASKS,public_acceptance=legacy.INTERPRETATIONS,
                                      input_fixture_sha256=host.sha(original/'gpio_arduino.h'),assertion_source_sha256=host.sha(original/'gpio_trace.cpp'),capture_does_not_change_assertions=True))
    manifest=[r for r in json.loads((ROOT/'experiment/output/all_groups_host_smoke_20261023/frozen_manifest.json').read_text()) if r['case'] in legacy.ORACLE_TASKS]
    jobs={}
    for row in manifest:
        if not row['compile_ok']:continue
        cpp=Path(row['source']).parent.parent/'build/sketch/sketch.ino.cpp'
        key=hashlib.sha256(json.dumps([row['case'],row['board'],host.sha(cpp)]).encode()).hexdigest()
        row['legacy_execution_key']=key;jobs.setdefault(key,row)
    results={}
    with concurrent.futures.ThreadPoolExecutor(max_workers=6) as pool:
        for key,result in zip(jobs,pool.map(one,jobs.items())):
            results[key]=result;host.save(OUT/'execution_results.json',results)
            if len(results)%10==0:print('Legacy oracle executions completed',len(results),'/',len(jobs),flush=True)
    mapped=[]
    for row in manifest:
        if not row['compile_ok']:mapped.append(dict(row,status='compile_failed_not_run'));continue
        result=results[row['legacy_execution_key']]
        mapped.append(dict(row,status=result['status'],evidence=result['evidence'],test_origin='fixed_validated_legacy16',coverage='legacy16_public_acceptance'))
    host.save(OUT/'mapped_results.json',mapped)
    host.save(OUT/'summary.json',dict(final=True,tasks=16,rows=len(mapped),unique_executions=len(jobs),counts=dict(__import__('collections').Counter(r['status'] for r in mapped))))
    print('Legacy16 replay complete',flush=True)


if __name__=='__main__':main()
