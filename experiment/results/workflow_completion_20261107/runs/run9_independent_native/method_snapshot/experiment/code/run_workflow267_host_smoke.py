"""Execute frozen workflow267 sources with host API doubles, NOT AVR simulation."""
import collections
import concurrent.futures
import hashlib
import json
from pathlib import Path
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'experiment/output/workflow267_host_smoke_20261023'
GXX = Path('E:/mingw64/mingw64/bin/g++.exe')
ORACLES = ROOT / 'experiment/development/oracles'


def replace_function(text, signature, body):
    start = text.index(signature)
    opening = text.index('{', start)
    depth = 1
    end = opening + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[:opening + 1] + body + text[end - 1:]


def fixtures():
    target = OUT / 'fixture'
    target.mkdir(parents=True, exist_ok=True)
    h = (ORACLES / 'gpio_arduino.h').read_text()
    h = h.replace('inline int task=0;', 'inline int task=0; inline int scenario=0;')
    h = h.replace('(task==301?18500:task==29?15500:6500)', '10000')
    # Host clock advances at observation points so polling code can make progress.
    h = h.replace('oracle::require_interrupts(); oracle::progress(); return (uint32_t)',
                  'oracle::require_interrupts(); oracle::advance_clock(); oracle::progress(); return (uint32_t)')
    h = h.replace('inline void progress() {', 'inline void advance_clock(){ if(++sub_ms>=1000){++now;sub_ms=0;} }\ninline void progress() {')
    h = replace_function(h, 'inline int analogRead(int pin)', '''
 oracle::require_interrupts(); oracle::advance_clock(); oracle::progress();
 if(pin>=0 && pin<ADC_COUNT)pin+=A0;
 if(pin<A0 || pin>=A0+ADC_COUNT)throw std::runtime_error("pin-range");
 return oracle::scenario==0?0:oracle::scenario==1?1023:int((oracle::now/1000)%3)*511;
''')
    h = replace_function(h, 'inline int digitalRead(int pin)', '''
 oracle::advance_clock(); oracle::progress();
 if(pin<0 || pin>=PIN_COUNT)throw std::runtime_error("pin-range");
 if(oracle::modes[pin]==OUTPUT){for(auto i=oracle::events.rbegin();i!=oracle::events.rend();++i)if(i->pin==pin)return i->level?HIGH:LOW;return LOW;}
 return oracle::scenario==0?LOW:oracle::scenario==1?HIGH:((oracle::now/500)%2?HIGH:LOW);
''')
    h = replace_function(h, 'inline unsigned long pulseIn(', '''
 oracle::require_interrupts();
 if(pin>=PIN_COUNT)throw std::runtime_error("pin-range");
 unsigned long width=oracle::scenario==0?0:oracle::scenario==1?1740:580;
 delayMicroseconds(width && width<=timeout?width:1000);
 return width<=timeout?width:0;
''')
    h = h.replace('unsigned long baud=0;std::string pending;', '''unsigned long baud=0;std::string pending;
 int available(){return 0;} int read(){return -1;} int peek(){return -1;}
''')
    h = h.replace('oracle::trigger_write(pin,level);', 'if(pin<0||pin>=PIN_COUNT)throw std::runtime_error("pin-range"); oracle::trigger_write(pin,level);')
    (target / 'Arduino.h').write_text(h)
    for name in ('Servo.h', 'TM1637Display.h'):
        (target / name).write_text((ORACLES / name).read_text())
    dht = (ORACLES / 'DHT.h').read_text()
    dht = dht.replace('if(oracle::task!=301)throw std::runtime_error("dht-scenario-unverified");', '')
    dht = dht.replace('result=begun&&pin==7&&type==DHT11&&start>=2000&&!(start>=6000&&start<10000);',
                      'result=begun&&(type==DHT11||type==DHT22);')
    (target / 'DHT.h').write_text(dht)
    harness = r'''
#include <Arduino.h>
#include <Servo.h>
#include <DHT.h>
#include <TM1637Display.h>
void setup(); void loop();
extern "C" void yield() __attribute__((weak));
extern "C" void yield() {}
int main(int argc,char**argv){
 oracle::scenario=argc>1?atoi(argv[1]):0;
 bool initialized=false; unsigned long calls=0; std::string status="completed";
 try {setup();initialized=true;while(oracle::now<10000){loop();++calls;delayMicroseconds(1000);}}
 catch(const std::exception&e){status=e.what();}
 size_t outputs=oracle::events.size()+oracle::tones.size()+oracle::serial_lines.size()+oracle::servos.size()+oracle::displays.size()+(!Serial.pending.empty());
 bool pass=initialized&&calls>=2&&oracle::now>=10000&&outputs>0&&(status=="completed"||status=="trace-end");
 std::cout<<"{\"setup_returned\":"<<(initialized?"true":"false")<<",\"loop_returns\":"<<calls<<",\"virtual_ms\":"<<oracle::now<<",\"outputs\":"<<outputs<<",\"status\":\""<<status<<"\",\"pass\":"<<(pass?"true":"false")<<"}";
 return pass?0:3;
}
'''
    (target / 'main.cpp').write_text(harness)
    return target


def check(row, fixture):
    folder = OUT / 'cases' / row['case']
    folder.mkdir(parents=True, exist_ok=True)
    source = Path(row['source'])
    result = dict(case=row['case'], dataset=row['dataset'], source=str(source),
                  source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
                  historical_compile_ok=True, runtime_status='unverified')
    if result['source_sha256'] != row['sha256']:
        result['reason'] = 'frozen_source_hash_mismatch'
        return result
    cpp = source.parent.parent / 'build/sketch/sketch.ino.cpp'
    result['preprocessed_sha256'] = hashlib.sha256(cpp.read_bytes()).hexdigest()
    macro = 'ARDUINO_AVR_MEGA2560' if 'mega' in row['board'] else 'ARDUINO_AVR_UNO'
    cmd = [str(GXX), '-std=c++17', '-O0', '-D'+macro, '-I'+str(fixture),
           str(cpp), str(fixture / 'main.cpp'), '-o', str(folder / 'smoke.exe')]
    result['build_command'] = cmd
    try:
        build = subprocess.run(cmd, capture_output=True, timeout=60)
        (folder / 'build.stderr.txt').write_bytes(build.stderr)
        if build.returncode:
            result['reason'] = 'host_adapter_build_unsupported'
            return result
        result['scenarios'] = []
        for scenario in range(3):
            try:
                run = subprocess.run([str(folder / 'smoke.exe'), str(scenario)], capture_output=True, timeout=5)
                (folder / f'scenario_{scenario}.stdout.txt').write_bytes(run.stdout)
                (folder / f'scenario_{scenario}.stderr.txt').write_bytes(run.stderr)
                record = json.loads(run.stdout)
                record.update(scenario=scenario, exit_code=run.returncode)
            except subprocess.TimeoutExpired:
                record = dict(scenario=scenario, pass_=False, status='wall_clock_timeout')
            except (ValueError, UnicodeError):
                record = dict(scenario=scenario, pass_=False, status='process_crash_or_invalid_output', exit_code=run.returncode)
            result['scenarios'].append(record)
        passed = sum(x.get('pass', False) for x in result['scenarios'])
        result['runtime_status'] = 'host_smoke_pass' if passed == 3 else 'unverified'
        result['scenarios_passed'] = passed
        result['reason'] = 'all_three_scenarios' if passed == 3 else 'inspect_scenario_evidence'
    except subprocess.TimeoutExpired:
        result['reason'] = 'host_build_timeout'
    (folder / 'result.json').write_text(json.dumps(result, indent=2), encoding='utf-8')
    return result


def self_check(fixture):
    assert replace_function('int a(){if(1){return 1;}}END', 'int a()', 'return 2;') == 'int a(){return 2;}END'
    folder = OUT / 'self_check'
    folder.mkdir(exist_ok=True)
    cases = {'healthy': 'void setup(){pinMode(13,OUTPUT);} void loop(){digitalWrite(13,HIGH);}',
             'no_output': 'void setup(){} void loop(){}',
             'setup_blocked': 'void setup(){while(true)delay(100);} void loop(){}'}
    checks = {}
    for name, code in cases.items():
        src = folder / (name+'.cpp')
        src.write_text('#include <Arduino.h>\n'+code)
        exe = folder / (name+'.exe')
        subprocess.run([str(GXX), '-std=c++17', '-I'+str(fixture), str(src), str(fixture/'main.cpp'), '-o', str(exe)], check=True, capture_output=True)
        run = subprocess.run([str(exe)], capture_output=True, timeout=5)
        checks[name] = json.loads(run.stdout)
    assert checks['healthy']['pass']
    assert not checks['no_output']['pass']
    assert not checks['setup_blocked']['pass']
    (folder/'results.json').write_text(json.dumps(checks, indent=2))


def main():
    OUT.mkdir(exist_ok=True, parents=True)
    rows = [r for r in json.loads((ROOT/'experiment/output/simulide_validation_20261023/old_comparison_execution.json').read_text()) if r['group']=='workflow_267']
    eligible = [r for r in rows if r['historical_compile_ok']]
    assert len(rows)==296 and len(eligible)==267
    (OUT/'frozen_manifest.json').write_text(json.dumps(rows, indent=2))
    fixture = fixtures()
    self_check(fixture)
    results = []
    started = time.time()
    with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
        futures = [pool.submit(check, row, fixture) for row in eligible]
        for f in concurrent.futures.as_completed(futures):
            results.append(f.result())
            results.sort(key=lambda r:r['case'])
            (OUT/'results.json').write_text(json.dumps(results, indent=2))
            count = collections.Counter(r['runtime_status'] for r in results)
            summary = dict(target='frozen_workflow267', total=296, historically_compiled=267,
                           completed=len(results), counts=dict(count), seconds=round(time.time()-started,1),
                           scope='host C++ execution with API doubles; NOT SimulIDE, AVR, physical hardware, or functional correctness',
                           scenarios=['digital LOW/ADC 0', 'digital HIGH/ADC 1023', 'digital toggles/ADC steps'],
                           serial_input='none; serial-dependent paths not covered', virtual_horizon_ms=10000)
            (OUT/'summary.json').write_text(json.dumps(summary, indent=2))
            if len(results)%20==0 or len(results)==267: print(json.dumps(summary), flush=True)


if __name__=='__main__':
    main()
