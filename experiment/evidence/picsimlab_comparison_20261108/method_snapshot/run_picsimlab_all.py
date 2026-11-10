"""Run frozen AVR firmware in PICSimLab; derive observations from electrical traces."""
import argparse
import bisect
from concurrent.futures import ThreadPoolExecutor,as_completed
import gzip
import hashlib
import json
import math
import os
from pathlib import Path
import queue
import re
import shutil
import subprocess
import time

import picsimlab_assertions as assertions
import picsimlab_circuit
import picsimlab_native_scenarios
import picsimlab_core_checks

ROOT=Path(__file__).resolve().parents[2]
OUT=ROOT/'experiment/output/picsimlab_verification_20261107'
BASE=Path.home()/'.cache/llm4mdd-picsimlab-build'
VERSION='native-electrical-v11'
APP=BASE/'engines'/VERSION/'picsimlab_NOGUI'
RUN=BASE/'batch_workers'/VERSION
DIGITS={0:' ',0x3f:'0',0x06:'1',0x5b:'2',0x4f:'3',0x66:'4',0x6d:'5',0x7d:'6',0x07:'7',0x7f:'8',0x6f:'9',
        0x77:'A',0x7c:'b',0x39:'C',0x5e:'d',0x79:'E',0x71:'F',0x73:'P',0x76:'H',0x38:'L',0x54:'n',0x5c:'o',0x50:'r',0x78:'t',0x3e:'U',0x40:'-'}


def load(path):return json.loads(path.read_text(encoding='utf8'))


def save(path,value):
    path.parent.mkdir(parents=True,exist_ok=True)
    temp=path.with_suffix(path.suffix+'.tmp');temp.write_text(json.dumps(value,ensure_ascii=False,indent=2),encoding='utf8');temp.replace(path)


def board_map(board):
    if 'mega' not in board:
        return dict(enumerate([2,3,4,5,6,11,12,13,14,15,16,17,18,19,23,24,25,26,27,28]))
    text=next(r['reply'] for r in load(OUT/'timing_remote.json') if r['command']=='pinsl')
    result={}
    for physical,label in re.findall(r'pin\[(\d+)\].*?"([^"]+)"',text):
        match=re.search(r'/~?(A?\d+)\s*$',label)
        if match:result[assertions.pin(match[1],'mega')]=int(physical)
    assert len(result)==70 and result[2]==6
    return result


def circuit(case):
    config=case.get('configuration_resolution') or {}
    if config.get('resolved_circuits'):return config['resolved_circuits'][0]
    marker='Given circuit (must preserve wiring):'
    text=case.get('effective_requirement') or case['requirement']
    if marker in text:
        try:return json.JSONDecoder().raw_decode(text.split(marker,1)[1].lstrip())[0]
        except ValueError:pass
    return None


def observed_pins(plan,board):
    pins=set()
    def visit(obj):
        if isinstance(obj,list):
            for value in obj:visit(value)
        elif isinstance(obj,dict):
            for key,value in obj.items():
                if key=='pin' or key.endswith('_pin'):
                    try:pins.add(assertions.pin(value,board))
                    except (ValueError,TypeError):pass
                elif key in ('pins','digit_pins','select_pins','segment_pins') and isinstance(value,list):
                    pins.update(assertions.pin(p,board) for p in value)
                else:visit(value)
            name=obj.get('name','')
            if isinstance(name,str) and name.startswith('tm:'):
                pins.update(int(p) for p in name.split(':')[1:])
    visit(plan)
    pins.update(assertions.pin(p,board) for p in plan.get('observe_pins',[]))
    return pins


def prepare_fixture(case,plan,work):
    board=case['fqbn'];mapping=board_map(board)
    pins=observed_pins(plan,board)
    displays=set()
    config=case.get('configuration_resolution') or {}
    for device in config.get('devices',[]):
        if device['kind'].startswith('tm1637'):
            p=device['resolved'];displays.add((assertions.pin(p['clk_pin'],board),assertions.pin(p['dio_pin'],board)))
    if not displays:
        text=case.get('effective_requirement') or case['requirement']
        for match in re.finditer(r'CLK\s*[:=]\s*D?(\d+)[^\n]{0,40}?DIO\s*[:=]\s*D?(\d+)',text,re.I):displays.add(tuple(map(int,match.groups())))
    if not displays:
        for check in plan['checks']:
            name=check.get('name','')
            if name.startswith('tm:'):displays.add(tuple(map(int,name.split(':')[1:])))
    for clk,dio in displays:pins.update((clk,dio))
    loop_pc=(1<<64)-1
    symbols=Path(case.get('_symbols_path',OUT/'cases'/case['id']/'symbols.txt')).read_text()
    for line in symbols.splitlines():
        fields=line.split()
        if len(fields)==3 and fields[2] in ('loop','_Z4loopv'):loop_pc=int(fields[0],16)
    if loop_pc==(1<<64)-1:
        # LTO may inline loop(): observe its DWARF entry PC without modifying flash.
        from elftools.elf.elffile import ELFFile
        elf=Path(case.get('_firmware_path',OUT/'cases'/case['id']/'firmware/sketch.ino.hex')).with_suffix('.elf')
        with elf.open('rb') as file:
            dwarf=ELFFile(file).get_dwarf_info()
            for cu in dwarf.iter_CUs():
                for die in cu.iter_DIEs():
                    if die.tag!='DW_TAG_inlined_subroutine':continue
                    if 'DW_AT_abstract_origin' not in die.attributes:continue
                    origin=die.get_DIE_from_attribute('DW_AT_abstract_origin')
                    name=origin.attributes.get('DW_AT_name')
                    entry=die.attributes.get('DW_AT_entry_pc')
                    if name and name.value==b'loop' and entry:loop_pc=entry.value
    save(work/'setup_observer.json',dict(loop_entry_pc=loop_pc,available=loop_pc!=(1<<64)-1,method='ELF_symbol_or_DWARF_inline_entry'))
    horizon=max(plan.get('horizon_ms',3000),3000)
    lines=[f'H {horizon} {loop_pc}']
    lines.extend(f'B {logical} {physical}' for logical,physical in mapping.items())
    for pin in sorted(pins):
        if pin not in mapping:raise ValueError(f'Pin {pin} unavailable on {board}')
        lines.append(f'P {pin} {mapping[pin]}')
    for clk,dio in displays:lines.append(f'T {mapping[clk]} {mapping[dio]}')
    for player in plan.get('adaptive_players',[]):
        led=mapping[assertions.pin(player['led_pin'],board)]
        button=mapping[assertions.pin(player['button_pin'],board)]
        wrong=mapping[assertions.pin(player['wrong_button_pin'],board)] if player.get('wrong_button_pin') is not None else 0
        lines.append(f"C {led} {button} {wrong} {player.get('active_level',0)} {int(player.get('arm_ms',3000)*1000)} {int(player.get('delay_ms',500)*1000)} {int(player.get('hold_ms',200)*1000)}")
    for device,mux in enumerate(plan.get('analog_multiplexers',[])):
        signal=mapping[assertions.pin(mux['signal_pin'],board,analog=True)]
        selects=[mapping[assertions.pin(p,board)] for p in mux['select_pins']]
        lines.append('Q '+str(signal)+' '+' '.join(map(str,selects)))
        for sample in mux.get('samples',[]):
            volts=min(5,max(0,(float(sample['value'])+.5)*5/1023))
            lines.append(f"I {int(sample['at_ms']*1000)} mux {device*16+int(sample['channel'])} {volts}")
    for step in plan.get('input_steps',[]):
        physical=mapping[assertions.pin(step['pin'],board,analog=step['kind']=='analog')]
        value=step.get('value')
        if step['kind']=='analog':
            # simavr's ADC electrical input is in volts, with millivolt resolution.
            value=min(5,max(0,(float(value)+.5)*5/1023))
        if value is not None:lines.append(f"I {int(step['at_ms']*1000)} {step['kind']} {physical} {value}")
        else:lines.append(f"I {int(step['at_ms']*1000)} {step['kind']}_invalid {physical} 0")
    baud=next((c['baud'] for c in plan['checks'] if c.get('baud')),9600)
    for item in plan.get('serial_inputs',[]):
        for i,byte in enumerate(item['text'].encode('utf8')):
            lines.append(f"I {int(item['at_ms']*1000+i*10000000/baud)} uart 0 {byte}")
    for sample in plan.get('imu_samples',[]):
        values=sample.get('acceleration',[0,0,9.81])+sample.get('gyro',[0,0,0])
        lines.append('J '+str(int(sample['at_ms']*1000))+' '+' '.join(map(str,values))+f" {sample.get('raw_y',0)} {int('raw_y' in sample)}")
    (work/'fixture.tsv').write_text('\n'.join(lines)+'\n')
    parts=['version,0,0,0:0.9.3','scale,0,0,0:1.0','spare_on,0,0,0:1','osc_on,0,0,0:0']
    native,hardware=picsimlab_circuit.native_parts(case,plan,mapping)
    parts.extend(native)
    save(work/'hardware.json',hardware)
    outputs=sorted({assertions.pin(c['pin'],board) for c in plan['checks'] if c['kind'] in ('gpio','pwm') and 'pin'in c})
    for i in range(0,len(outputs),8):
        group=outputs[i:i+8];prefs=[mapping[p] for p in group]+[0]*(8-len(group))+[1]+[0]*8+[len(group)]
        parts.append('LEDs,30,30,0:'+','.join(map(str,prefs)))
    (work/'circuit.pcf').write_text('\n'.join(parts)+'\n')
    return horizon


def decode_trace(path,plan,board):
    trace=[dict(type='reset_state',gpio_latch=0,at_ms=0)]
    frequency=16000000
    serial={};gpio={};setup_at=None;ended=False;end_ms=0
    with path.open() as file:
        for sequence,line in enumerate(file):
            fields=line.split()
            if not fields:continue
            kind=fields[0]
            if kind=='V':frequency=int(fields[2]);continue
            if kind not in ('G','S','M','A','E','I','F'):continue
            at_us=int(fields[1])*1000000/frequency;at_ms=at_us/1000
            if kind=='G':
                p,v,d=map(int,fields[2:5]);v=int(bool(v))
                e=dict(type='gpio',sequence=sequence,at_ms=at_ms,at_us=at_us,pin=p,value=v,pwm=False,direction=d)
                trace.append(e);gpio.setdefault(p,[]).append(e)
            elif kind=='S':
                uart,baud,byte=map(int,fields[2:5]);items=serial.setdefault(uart,[]);items.append((at_ms,baud,byte,sequence))
            elif kind=='M':trace.append(dict(type='pin_mode',pin=int(fields[2]),value=int(fields[3])))
            elif kind=='F':
                clk,dio,on=map(int,fields[2:5]);segments=list(map(int,fields[5:9]))
                trace.append(dict(type='display',name=f'tm:{clk}:{dio}',at_ms=at_ms,sequence=sequence,
                                  text=''.join(DIGITS.get(s&127,'?') if on else ' ' for s in segments),segments=segments,enabled=bool(on)))
            elif kind=='A':setup_at=at_ms
            elif kind=='E':ended=True;end_ms=at_ms
    for uart,items in serial.items():
        buffer=bytearray();last_at=0;start_at=0;last_baud=9600;seq=0
        def flush():
            if buffer:trace.append(dict(type='serial',uart=uart,at_ms=last_at,sequence=seq,baud=last_baud,text=buffer.decode('utf8',errors='replace').rstrip('\r')))
        for at,baud,byte,sequence in items:
            if buffer and at-last_at>20:flush();buffer.clear()
            if not buffer:start_at=at
            last_at=at;last_baud=baud;seq=sequence
            if byte==10:flush();buffer.clear()
            else:buffer.append(byte)
        flush()
    # Pulse-width observations are physical PWM measurements, not Servo API calls.
    servo_pins=set()
    def visit(obj):
        if isinstance(obj,list):
            for x in obj:visit(x)
        elif isinstance(obj,dict):
            if obj.get('kind')=='servo' or obj.get('source')=='servo':servo_pins.add(assertions.pin(obj['pin'],board))
            for value in obj.values():
                if isinstance(value,(dict,list)):visit(value)
    visit(plan['checks'])
    for p in servo_pins:
        rise=None;last_angle=None
        for event in gpio.get(p,[]):
            if event['value']:rise=event['at_us']
            elif rise is not None:
                width=event['at_us']-rise;rise=None
                if 400<=width<=2600:
                    angle=round((width-544)*180/(2400-544))
                    if angle!=last_angle:
                        trace.append(dict(type='servo',pin=p,at_ms=event['at_ms'],sequence=event['sequence'],
                                          angle=angle,pulse_us=width,active=True,operation='pulse'))
                        last_angle=angle
    pwm_queries=set()
    for check in plan['checks']:
        if check['kind'] in ('gpio_count','display_gpio_count','distinct_gpio_patterns'):
            times=check.get('at_ms',[])
            if not isinstance(times,list):times=[times]
            for pin in check['pins']:
                p=assertions.pin(pin,board)
                if str(p) in plan.get('electrical_led_pins',{}):
                    pwm_queries.update((p,t) for t in times)
        if check['kind']=='pwm' or check.get('source')=='pwm' or (check['kind']=='gpio' and (plan.get('timing_interpretation') or str(assertions.pin(check['pin'],board)) in plan.get('electrical_led_pins',{}))):
            times=check.get('at_ms',[])
            if not isinstance(times,list):times=[times]
            if 'native_eventual_from_ms' in check:
                times+=list(range(math.ceil(check['native_eventual_from_ms']),math.floor(check['native_eventual_to_ms'])+1,20))
            for at in times:pwm_queries.add((assertions.pin(check['pin'],board),at))
    pwm_index={}
    for p,events in gpio.items():
        times=[];area=[];rises=[];level=0;last=0;integral=0
        for event in events:
            t=event['at_us'];integral+=(t-last)*level;times.append(t);area.append(integral)
            if event['value'] and not level:rises.append(t)
            level=event['value'];last=t
        pwm_index[p]=(events,times,area,rises)
    for p,at in sorted(pwm_queries):
        events,times,area,rises=pwm_index.get(p,([],[],[],[]));i=bisect.bisect_right(times,at*1000)-1
        value=255 if i>=0 and events[i]['value'] else 0;begin=at;end=at
        r=bisect.bisect_right(rises,at*1000)
        if r>=3 and at*1000-rises[r-1]<5000:
            begin=rises[r-3];end=rises[r-1]
            def cumulative(t):
                index=bisect.bisect_right(times,t)-1
                return area[index]+(t-times[index])*events[index]['value'] if index>=0 else 0
            if end>begin:value=round(255*(cumulative(end)-cumulative(begin))/(end-begin))
            begin/=1000;end/=1000
        trace.append(dict(type='pwm',pin=p,at_ms=at,value=value,window_from_ms=begin,window_to_ms=end,method='complete_hardware_pwm_periods'))
    # Decode the supplied electrical seven-segment wiring from real GPIO edges.
    states={};shift_states=[0]*len(plan.get('shift_registers',[]));previous={}
    def frame(name,text,at,sequence):
        if previous.get(name)!=text:trace.append(dict(type='display',name=name,text=text,at_ms=at,sequence=sequence));previous[name]=text
    for event in sorted((e for events in gpio.values() for e in events),key=lambda e:(e['at_us'],e['sequence'])):
        p=event['pin'];old=states.get(p,0);states[p]=event['value']
        for display in plan.get('seven_segments',[]):
            dp=[assertions.pin(p,board) for p in display['pins']]
            if p in dp:
                mask=sum((states.get(pin,0)^(bool(display.get('common_anode',False))))<<i for i,pin in enumerate(dp[:7]))
                frame(display['name'],DIGITS.get(mask,'?'),event['at_ms'],event['sequence'])
        for i,register in enumerate(plan.get('shift_registers',[])):
            clock=assertions.pin(register['clock_pin'],board);data=assertions.pin(register['data_pin'],board);latch=assertions.pin(register['latch_pin'],board)
            if p==clock and event['value'] and not old:
                shift_states[i]=((shift_states[i]<<1)|states.get(data,0))&((1<<(8*register.get('registers',1)))-1)
            if p==latch and event['value'] and not old:
                for display in register['displays']:
                    byte=(shift_states[i]>>(8*display.get('register_index',0)))&255
                    mask=sum((((byte>>bit)&1)^bool(display.get('common_anode',False)))<<j for j,bit in enumerate(display['segment_bits'][:7]))
                    frame(display['name'],DIGITS.get(mask,'?'),event['at_ms'],event['sequence'])
    execution=dict(type='execution',status='completed' if ended else 'incomplete_capture',virtual_ms=end_ms,setup_returned=setup_at is not None)
    if setup_at is not None:execution['setup_at_ms']=setup_at
    trace.append(execution)
    return trace


def evaluate(plan,trace,board):
    adapted=json.loads(json.dumps(plan))
    adapted['checks']=[c for c in adapted['checks'] if c['kind'] not in picsimlab_core_checks.KINDS]
    result=assertions.evaluate(adapted,trace,board)
    result['checks'].extend(picsimlab_core_checks.evaluate(c,trace,board) for c in plan['checks'] if c['kind'] in picsimlab_core_checks.KINDS)
    missing=[]
    for check in result['checks']:
        c=check['test'];kind=c['kind'];reason=None
        if kind=='setup' and not any(e['type']=='execution' and e.get('setup_returned') for e in trace):reason='setup_anchor_unavailable_or_not_reached'
        if kind=='lcd' and not any(e['type']=='lcd' for e in trace):reason='no_supported_font_observed_in_native_pixels'
        elif kind=='display' and c.get('name','').startswith('tm:') and not any(e['type']=='display' and e.get('name')==c['name'] for e in trace):reason='tm1637_model_not_observed'
        elif kind=='servo' and not any(e['type']=='servo' and e['pin']==assertions.pin(c['pin'],board) for e in trace):reason='servo_pulses_not_observed'
        elif kind in ('state_witness','state_invariant','event_window','event_order') and 'servo' in json.dumps(c):reason='host_api_order_not_equivalent_to_physical_servo_pulses'
        elif kind=='command_sequence' and c.get('source')=='servo':reason='api_command_sequence_not_directly_observable_from_physical_pulses'
        if reason:check.update(passed=False,unverified=reason);missing.append(reason)
    picsimlab_native_scenarios.replace_results(plan,trace,board,result)
    failed=[c for c in result['checks'] if not c['passed'] and not c.get('unverified')]
    result['status']='unverified_no_deterministic_checks' if not result['checks'] else 'failed' if failed else 'partial_unverified' if missing or any(c.get('unverified') for c in result['checks']) else 'scenarios_passed'
    result['failed_assertions']=len(failed);result['unverified_checks']=sum(bool(c.get('unverified')) for c in result['checks'])
    return result


def run_plan(case,entry,slot,index):
    if not APP.exists():
        APP.parent.mkdir(parents=True,exist_ok=True)
        temporary=APP.with_suffix(f'.{os.getpid()}.tmp')
        shutil.copy2(BASE/'picsimlab-0.9.3/src/picsimlab_NOGUI',temporary);temporary.replace(APP)
    prefix=case.get('_variant','plan')
    output=OUT/'cases'/case['id']/'simulation'/VERSION/f'{prefix}_{index:02d}'
    different_plan=(output/'original_test_plan.json').exists() and load(output/'original_test_plan.json')!=entry['plan']
    different_source=(output/'result.json').exists() and load(output/'result.json').get('source_sha256',case['source_sha256'])!=case['source_sha256']
    if different_plan or different_source:
        digest=hashlib.sha256(json.dumps(dict(plan=entry['plan'],source=case['source_sha256']),sort_keys=True).encode()).hexdigest()[:10]
        output=output.with_name(output.name+'_'+digest)
    output.mkdir(parents=True,exist_ok=True)
    existing=output/'result.json'
    if existing.exists():
        previous=load(existing)
        if previous.get('status')!='infrastructure_unverified':return previous
        existing.replace(output/f'result.infrastructure.{time.time_ns()}.json')
    plan=picsimlab_native_scenarios.prepare(entry)
    plan['electrical_led_pins']={str(p):level for p,level in picsimlab_circuit.led_pins(case).items()}
    work=RUN/str(slot);work.mkdir(parents=True,exist_ok=True)
    profile=work/'profile';profile.mkdir(exist_ok=True)
    board_name='Arduino_Mega' if 'mega' in case['fqbn'] else 'Arduino_Uno'
    processor_name='atmega2560' if 'mega' in case['fqbn'] else 'atmega328p'
    (profile/'picsimlab.ini').write_text(f'picsimlab_version = "0.9.3"\npicsimlab_lab = "{board_name}"\n{board_name}_proc = "{processor_name}"\n{board_name}_clock = "16.0"\npicsimlab_remotecp = "{5400+slot}"\npicsimlab_debug = "0"\nspare_on = "1"\n')
    started=time.monotonic();row=dict(case=case['id'],dataset=case['dataset'],version=VERSION,plan_name=entry.get('name'),plan_index=index,plan_coverage=plan.get('coverage'),result_directory=str(output.relative_to(OUT)))
    raw=work/'trace.tsv'
    if raw.exists():raw.unlink()
    try:
        horizon=prepare_fixture(case,plan,work)
        save(output/'original_test_plan.json',entry['plan']);save(output/'test_plan.json',plan);shutil.copyfile(work/'fixture.tsv',output/'fixture.tsv');shutil.copyfile(work/'circuit.pcf',output/'circuit.pcf');shutil.copyfile(work/'hardware.json',output/'hardware.json');shutil.copyfile(work/'setup_observer.json',output/'setup_observer.json')
        source=OUT/'cases'/case['id'];compiled=case.get('_compiled_override') or load(source/'compilation.json')
        firmware=Path(case.get('_firmware_path',source/'firmware/sketch.ino.hex'))
        assert compiled['compile_ok'] and compiled['source_sha256']==case['source_sha256']
        assert hashlib.sha256(firmware.read_bytes()).hexdigest()==compiled['hex']['sha256']
        env=dict(os.environ,DISPLAY='',WAYLAND_DISPLAY='',PULSE_SERVER='',ALSOFT_DRIVERS='null',
                 PICSIMLAB_USER_DATA=str(profile),PICSIMLAB_FIXTURE=str(work/'fixture.tsv'),PICSIMLAB_TRACE=str(raw),
                 PICSIMLAB_LCD_FRAMES=str(work/'lcd_frames.bin.gz'),TZ='UTC')
        if 'lcd_capture_interval_ms' in plan:env['PICSIMLAB_LCD_FRAME_MS']=str(plan['lcd_capture_interval_ms'])
        if 'rtc_epoch_s' in plan:env['PICSIMLAB_RTC_EPOCH']=str(plan['rtc_epoch_s'])
        board='Arduino_Mega' if 'mega' in case['fqbn'] else 'Arduino_Uno';processor='atmega2560' if 'mega' in case['fqbn'] else 'atmega328p'
        with (output/'simulator.log').open('wb') as log:
            process=subprocess.run([str(APP),board,processor,str(firmware),str(work/'circuit.pcf')],cwd=work,env=env,stdout=log,stderr=log,timeout=max(60,horizon/1000*10+30))
        row.update(exit_code=process.returncode,source_sha256=case['source_sha256'],firmware_sha256=compiled['hex']['sha256'])
        if process.returncode!=0:raise RuntimeError(f'PICSimLab exit {process.returncode}')
        trace=decode_trace(raw,plan,case['fqbn'])
        frames=work/'lcd_frames.bin.gz'
        if frames.exists():
            import picsimlab_raster
            trace.extend(picsimlab_raster.observe(frames,output))
        with gzip.open(output/'observations.jsonl.gz','wt',encoding='utf8') as file:
            for event in trace:file.write(json.dumps(event,ensure_ascii=False)+'\n')
        row.update(evaluate(plan,trace,case['fqbn']))
        hardware=load(work/'hardware.json');mapping=board_map(case['fqbn'])
        row['observed_inputs_unhandled']=[s for s in plan.get('input_steps',[]) if s['kind'] not in ('digital','analog') and not (
            s['kind'] in ('temperature','humidity') and mapping[assertions.pin(s['pin'],case['fqbn'])] in hardware['dht_pins'] or
            s['kind']=='distance' and mapping[assertions.pin(s['pin'],case['fqbn'])] in hardware['ultrasonic_echo_pins'])]
        if row['observed_inputs_unhandled']:
            row['status']='input_model_unverified'
    except Exception as exc:row.update(status='infrastructure_unverified',error_type=type(exc).__name__,error=str(exc)[:2000])
    finally:
        if raw.exists():
            with raw.open('rb') as source,gzip.open(output/'electrical_trace.tsv.gz','wb') as target:shutil.copyfileobj(source,target)
        if (work/'lcd_frames.bin.gz').exists():
            shutil.copyfile(work/'lcd_frames.bin.gz',output/'lcd_frames.bin.gz');(work/'lcd_frames.bin.gz').unlink()
        row['seconds']=round(time.monotonic()-started,3);save(existing,row)
    return row


def run_case(case,slots):
    slot=slots.get()
    try:
        entries=load(OUT/'cases'/case['id']/'test_plans.json')
        supplementary=OUT/'cases'/case['id']/'offline_supplementary_plans.json'
        if not any(e['plan'].get('checks') for e in entries) and supplementary.exists():entries=load(supplementary)
        rows=[run_plan(case,entry,slot,index) for index,entry in enumerate(entries)]
        status='no_saved_test_plan' if not rows else 'failed' if any(r['status']=='failed' for r in rows) else 'scenarios_passed' if all(r['status']=='scenarios_passed' for r in rows) else 'partial_unverified'
        row=dict(case=case['id'],dataset=case['dataset'],status=status,plans=rows,version=VERSION)
        save(OUT/'cases'/case['id']/'simulation'/VERSION/'case_result.json',row)
        return row
    finally:slots.put(slot)


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--workers',type=int,default=6);parser.add_argument('--case',action='append');args=parser.parse_args()
    assert os.name=='posix' and 1<=args.workers<=12
    if not APP.exists():
        APP.parent.mkdir(parents=True,exist_ok=True)
        shutil.copy2(BASE/'picsimlab-0.9.3/src/picsimlab_NOGUI',APP)
    os.nice(5);os.sched_setaffinity(0,set(sorted(os.sched_getaffinity(0))[:16]))
    cases=load(OUT/'manifest.json')['cases']
    if args.case:cases=[c for c in cases if c['id'] in args.case]
    cases.sort(key=lambda c:(c['dataset']!='EmbedBench',len(c['requirement'])))
    slots=queue.Queue()
    for n in range(args.workers):slots.put(n)
    results=[];started=time.time()
    with ThreadPoolExecutor(max_workers=args.workers) as pool:
        futures=[pool.submit(run_case,c,slots) for c in cases]
        for future in as_completed(futures):
            row=future.result();results.append(row)
            print(row['case'],row['status'],flush=True)
            counts={s:sum(r['status']==s for r in results) for s in {r['status'] for r in results}}
            save(OUT/f'{VERSION}_summary.json',dict(planned=len(cases),completed=len(results),counts=counts,started_at=started,elapsed_seconds=time.time()-started,version=VERSION,workers=args.workers))


if __name__=='__main__':main()
