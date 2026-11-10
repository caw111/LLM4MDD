"""Measure native PICSimLab execution on three existing LED/button test plans.

This pilot reports scenario results, not whole-task functional correctness.
"""
import argparse
import hashlib
from itertools import groupby
import json
import os
from pathlib import Path
import re
import shutil
import socket
import subprocess
import time

from picsimlab_timing_probe import ROOT, OUT, BASE, Remote


def load(path):
    return json.loads(path.read_text(encoding='utf8'))


def gpio_matches(value, output_enabled, expected):
    return value == expected and output_enabled


def scalar_wave(path):
    signals={}
    changes=[]
    tick=0
    for line in path.read_text().splitlines():
        if line.startswith('$var '):
            fields=line.split();signals[fields[3]]=fields[4]
        elif line.startswith('#'):
            tick=int(line[1:])
        elif len(line)>=2 and line[0] in '01':
            changes.append((tick,line[1:],int(line[0])))
    return signals,changes


def waveform_checks(output, checks, output_pins):
    _,clock=scalar_wave(output/'millis.vcd')
    _,ddr=scalar_wave(output/'ddre.vcd')
    _,gpio=scalar_wave(output/'gpio.vcd')
    markers="!$%&[()]+*{}<>'^"
    clock_value=0
    clock_samples=[]
    for tick,group in groupby(clock,key=lambda e:e[0]):
        for _,marker,value in group:
            bit=markers.index(marker)
            clock_value=(clock_value&~(1<<bit))|(value<<bit)
        clock_samples.append((tick,clock_value))
    results=[]
    for check in checks:
        target=check['at_ms']
        tick,actual_ms=next((t,v) for t,v in clock_samples if v>=target)
        assert 0<=actual_ms-target<=2, 'Clock waveform did not cover the requested sample'
        values={}
        for t,marker,value in gpio:
            if t>tick:break
            values[marker]=value
        direction={}
        for t,marker,value in ddr:
            if t>tick:break
            direction[marker]=value
        pin=str(check['pin'])
        value=values[markers[output_pins.index(pin)]]
        output_enabled=bool(direction[markers[4 if pin=='2' else 5]])
        results.append(dict(id=check['id'],pin=check['pin'],target_ms=target,
                            actual_ms=actual_ms,trace_tick=tick,actual=value,
                            expected=check['expected'],output_enabled=output_enabled,
                            timing_valid=True,passed=gpio_matches(value,output_enabled,check['expected'])))
    return results


def run_case(case_id, pin_map):
    folder = OUT / 'cases' / case_id
    output = folder / 'picsimlab_pilot'
    output.mkdir(exist_ok=True)
    compiled = load(folder / 'compilation.json')
    assert compiled['compile_ok'] and compiled['board'] == 'arduino:avr:mega'
    firmware = folder / 'firmware/sketch.ino.hex'
    assert hashlib.sha256(firmware.read_bytes()).hexdigest() == compiled['hex']['sha256']
    plan = load(folder / 'test_plans.json')[0]['plan']
    assert all(check['kind'] == 'gpio' for check in plan['checks'])
    assert all(step['kind'] == 'digital' for step in plan['input_steps'])
    input_pins = sorted({str(step['pin']) for step in plan['input_steps']})
    output_pins = sorted({str(check['pin']) for check in plan['checks']})
    assert len(output_pins) <= 8 and len(input_pins) <= 8
    preferences = [pin_map[pin] for pin in output_pins] + [0] * (8-len(output_pins))
    parts = ['version,0,0,0:0.9.3', 'scale,0,0,0:1.0', 'spare_on,0,0,0:1', 'osc_on,0,0,0:0',
             'LEDs,30,30,0:' + ','.join(map(str, preferences + [1] + [0]*8 + [len(output_pins)]))]
    button_part = None
    if input_pins:
        button_part = 1
        pins = [pin_map[pin] for pin in input_pins] + [0]*(8-len(input_pins))
        # Explicit ideal active-low buttons match these supplied circuits.
        parts.append('Push Buttons,30,100,0:' + ','.join(map(str, pins + [0,1,len(input_pins)])))
    parts.append('VCD Dump,30,200,0:' + ','.join(map(str, preferences+[1])))
    parts.append(f"VCD Dump Memory,30,300,0:{compiled['millis_ram_address']},16,1")
    parts.append('VCD Dump Memory,30,400,0:45,8,1')
    config = output / 'circuit.pcf'
    config.write_text('\n'.join(parts)+'\n', encoding='ascii')
    env = dict(os.environ, DISPLAY='', WAYLAND_DISPLAY='', PULSE_SERVER='', ALSOFT_DRIVERS='null',
               APPDIR=str(BASE/'squashfs-root'), LD_PRELOAD=str(BASE/'profile_compat.so'),
               LD_LIBRARY_PATH=f'{BASE}/squashfs-root/usr/lib:{BASE}/squashfs-root/usr/lib/picsimlab')
    started = time.monotonic()
    remote = None
    row = dict(case=case_id, engine='PICSimLab 0.9.3 Linux64_NOGUI',
               source_sha256=compiled['source_sha256'], firmware_sha256=compiled['hex']['sha256'],
               plan_coverage=plan.get('coverage'), scope='supplied_scenarios_only', checks=[], inputs=[])
    with (output/'simulator.log').open('wb') as log:
        app = subprocess.Popen([str(BASE/'squashfs-root/AppRun'), 'Arduino_Mega', 'atmega2560',
                                str(firmware), str(config)], cwd=BASE, env=env,
                               stdout=log, stderr=log, start_new_session=True)
        try:
            deadline = time.monotonic()+20
            while time.monotonic()<deadline:
                assert app.poll() is None, f'PICSimLab exited {app.returncode}'
                try:
                    remote=Remote()
                    remote.sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
                    break
                except OSError:
                    time.sleep(.1)
            assert remote is not None
            remote.command('sim stop')
            info=remote.command('info')
            assert 'atmega2560' in info and 'LEDs' in info and 'VCD Dump' in info, info
            address=compiled['millis_ram_address']
            events = sorted([(s['at_ms'],0,s) for s in plan['input_steps']] +
                            [(c['at_ms'],1,c) for c in plan['checks']], key=lambda e:(e[0],e[1]))
            before=remote.millis(address)
            assert before<500, f'Unexpected startup clock: {before}'
            row['startup_firmware_ms']=before
            sim_started=time.monotonic()
            remote.command('sim start')
            for target,event_group in groupby(events,key=lambda e:e[0]):
                while True:
                    now=remote.millis(address)
                    if now>=target:
                        break
                    if time.monotonic()-sim_started > 4*plan['horizon_ms']/1000+30:
                        raise TimeoutError('Firmware clock did not progress fast enough')
                    time.sleep(.02)
                remote.command('sim stop')
                observed=remote.millis(address)
                for _,kind,item in event_group:
                    if kind==0:
                        button_index=input_pins.index(str(item['pin']))
                        remote.command(f"set part[{button_part:02d}].in[{button_index:02d}] {int(item['value'])}")
                        row['inputs'].append(dict(**item, actual_ms=observed))
                    else:
                        pins=remote.command('pinsl')
                        physical=pin_map[str(item['pin'])]
                        match=re.search(rf'pin\[{physical:02d}\] D ([IO]) ([01]) ',pins)
                        assert match, pins
                        value=int(match[2])
                        # The AVR backend can leave pinsl's direction stale after reset.
                        # Verify the real ATmega2560 DDRE register for these D2/D3 outputs.
                        assert str(item['pin']) in ('2','3')
                        ddr_reply=remote.command('dumpr 2d 1')
                        ddr_match=re.search(r'^002D: ([0-9A-Fa-f]{2})',ddr_reply,re.M)
                        assert ddr_match, ddr_reply
                        ddr=int(ddr_match[1],16)
                        output_enabled=bool(ddr & (1 << (4 if str(item['pin'])=='2' else 5)))
                        late=observed-target
                        row['checks'].append(dict(id=item['id'],pin=item['pin'],target_ms=target,
                                                  actual_ms=observed,late_ms=late,actual=value,
                                                  expected=item['expected'], pinsl_direction=match[1],
                                                  ddre=ddr, output_enabled=output_enabled,
                                                  timing_valid=late<=150,
                                                  passed=gpio_matches(value,output_enabled,item['expected'])))
                remote.command('sim start')
            while remote.millis(address)<plan['horizon_ms']:
                time.sleep(.05)
            remote.command('sim stop')
            row['final_firmware_ms']=remote.millis(address)
            row['execution_wall_seconds']=round(time.monotonic()-sim_started,3)
            # Preserve the emulator-generated waveform before its destructor removes it.
            for entry in Path(f'/proc/{app.pid}/fd').iterdir():
                try:
                    name=entry.resolve()
                    if name.suffix=='.vcd' and name.is_file():
                        header=name.read_text()[:1500]
                        target='ddre.vcd' if 'mem[45]' in header else (
                            'millis.vcd' if f'mem[{address}]' in header else 'gpio.vcd')
                        shutil.copyfile(name,output/target)
                except FileNotFoundError:
                    pass
            row['live_checks']=row['checks']
            row['checks']=waveform_checks(output,plan['checks'],output_pins)
            valid=all(c['timing_valid'] for c in row['checks'])
            row['status']='scenarios_passed' if valid and all(c['passed'] for c in row['checks']) else (
                'scenario_failed' if valid else 'timing_unverified')
            assert row['checks'], 'No assertions were executed'
            # A deliberately inverted expectation must be rejected by the same value comparison.
            first=row['checks'][0]
            row['inverted_expectation_rejected']=not gpio_matches(first['actual'],first['output_enabled'],1-first['actual'])
        except Exception as exc:
            row.update(status='infrastructure_unverified', error=f'{type(exc).__name__}: {exc}')
        finally:
            if remote:
                (output/'remote_commands.json').write_text(json.dumps(remote.log,indent=2))
                remote.sock.close()
            app.terminate()
            try:
                app.wait(timeout=5)
            except subprocess.TimeoutExpired:
                app.kill();app.wait()
            row['total_wall_seconds']=round(time.monotonic()-started,3)
            (output/'result.json').write_text(json.dumps(row,indent=2))
    return row


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case',action='append')
    args=parser.parse_args()
    os.nice(15)
    os.sched_setaffinity(0,{min(os.sched_getaffinity(0))})
    prior=load(OUT/'timing_remote.json')
    pins=next(r['reply'] for r in prior if r['command']=='pinsl')
    pin_map={}
    for physical,label in re.findall(r'pin\[(\d+)\].*?"([^"]+)"',pins):
        match=re.search(r'/~?(A?\d+)\s*$',label)
        if match:pin_map[match[1]]=int(physical)
    assert pin_map['2']==6 and pin_map['3']==7 and pin_map['A3']==94
    results=[]
    for case in (args.case or ('embedbench_19','embedbench_5','embedbench_1')):
        row=run_case(case,pin_map);results.append(row)
        print(json.dumps(row),flush=True)
        (OUT/'gpio_pilot_summary.json').write_text(json.dumps(dict(results=results,
            cpu_affinity=sorted(os.sched_getaffinity(0)),nice=os.getpriority(os.PRIO_PROCESS,0)),indent=2))


if __name__=='__main__':
    main()
