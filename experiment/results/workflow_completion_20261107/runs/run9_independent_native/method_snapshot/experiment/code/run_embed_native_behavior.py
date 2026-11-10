"""Offline conversion of supplied EmbedBench test states, gated on reference consistency."""
import ast
import collections
import concurrent.futures
import hashlib
import json
from pathlib import Path
import re

import behavior_full_host as host

ROOT=host.ROOT
OUT=host.OUT/'embed_native'
COLORS={'off':(0,0,0),'black':(0,0,0),'red':(1,0,0),'green':(0,1,0),'blue':(0,0,1),
        'yellow':(1,1,0),'cyan':(0,1,1),'purple':(1,0,1),'magenta':(1,0,1),'white':(1,1,1)}


def diagram(row):
    d=ast.literal_eval(row['diagram']) if isinstance(row['diagram'],str) else row['diagram']
    return d['Arduino Mega']


class Circuit:
    def __init__(self,d):
        self.parts={p['id']:p for p in d['parts']};self.graph=collections.defaultdict(set)
        self.board=next(p['id'] for p in d['parts'] if p['type']=='wokwi-arduino-mega')
        for a,b,*_ in d['connections']:
            a,b=self.canonical(a),self.canonical(b);self.graph[a].add(b);self.graph[b].add(a)
    def canonical(self,node):
        if node.startswith(self.board+':'):
            s=node.split(':',1)[1]
            if s.startswith('GND'):return 'POWER:GND'
            if s in ('5V','3.3V'):return 'POWER:VCC'
        return node
    def network(self,node):
        start=self.canonical(node);visited={start};q=[start]
        for p in q:
            for n in self.graph[p]:
                if n not in visited:visited.add(n);q.append(n)
        return visited
    def pin(self,node):
        for n in self.network(node):
            if n.startswith(self.board+':'):
                p=n.split(':',1)[1]
                if re.fullmatch(r'A?\d+',p):return p
        return None
    def power(self,part):
        for suffix in ['COM','COM.1','COM.2','A','C']:
            net=self.network(part+':'+suffix)
            if 'POWER:VCC' in net and suffix.startswith('COM'):return True
            if 'POWER:GND' in net and suffix.startswith('COM'):return False
        return self.parts[part].get('attrs',{}).get('common')=='anode'
    def button(self,name):
        pins=[];ground=False;vcc=False
        for endpoint in list(self.graph):
            if not endpoint.startswith(name+':'):continue
            p=self.pin(endpoint)
            if p is not None:pins.append(p)
            net=self.network(endpoint);ground|='POWER:GND' in net;vcc|='POWER:VCC' in net
        return (pins[0],0 if ground or not vcc else 1) if pins else (None,None)
    def displays(self):
        direct=[];buses={};unresolved=[]
        registers={n for n,p in self.parts.items() if p['type']=='wokwi-74hc595'}
        def root(name,visited=None):
            visited=set() if visited is None else visited
            if name in visited:return None
            visited.add(name)
            data=self.pin(name+':DS')
            if data is not None:return name,data,0
            for reg in registers:
                if reg+':Q7S' in self.network(name+':DS'):
                    r=root(reg,visited)
                    if r:return r[0],r[1],r[2]+1
            return None
        for name,part in self.parts.items():
            if part['type']!='wokwi-7segment':continue
            segment_nodes=[name+':'+s for s in 'ABCDEFG'];pins=[self.pin(n) for n in segment_nodes]
            anode=self.power(name)
            if all(p is not None for p in pins):direct.append(dict(name=name,pins=pins,common_anode=anode));continue
            reg_bits=[]
            for n in segment_nodes:
                found=[]
                for endpoint in self.network(n):
                    if ':' not in endpoint:continue
                    reg,suffix=endpoint.split(':',1)
                    if reg in registers and re.fullmatch(r'Q[0-7]',suffix):found.append((reg,int(suffix[1:])))
                reg_bits.append(found[0] if found else None)
            if not all(reg_bits) or len({r[0] for r in reg_bits if r})!=1:unresolved.append(name);continue
            reg=reg_bits[0][0];r=root(reg)
            if not r:unresolved.append(name);continue
            root_name,data,index=r;clock=self.pin(root_name+':SHCP');latch=self.pin(root_name+':STCP')
            if clock is None or latch is None:unresolved.append(name);continue
            bus=buses.setdefault(root_name,dict(data_pin=data,clock_pin=clock,latch_pin=latch,registers=index+1,displays=[]))
            bus['registers']=max(bus['registers'],index+1)
            bus['displays'].append(dict(name=name,register_index=index,segment_bits=[x[1] for x in reg_bits],common_anode=anode))
        return direct,list(buses.values()),unresolved


def state_flat(test,circuit):
    result={}
    def walk(obj):
        if not isinstance(obj,dict):return
        for k,v in obj.items():
            if k in circuit.parts:result[k]=v
            elif isinstance(v,dict):walk(v)
    walk(test);return result


def adc(value):
    s=str(value).strip().lower()
    if 'minimum' in s or 'initial' in s:return 0
    if 'maximum' in s:return 1023
    nums=re.findall(r'[-+]?\d+(?:\.\d+)?',s)
    if not nums:return None
    x=float(nums[-1])
    if '%' in s:return round(x*1023/100)
    if 0<=x<=1:return round(x*1023)
    return round(x) if 0<=x<=1023 else None


def reference_cpp(sketch):
    # Arduino's auto-prototypes, restricted to ordinary procedural reference helpers.
    regex=r'(?m)^\s*((?:(?:static|inline|unsigned|long|short|const)\s+)*(?:void|bool|int|float|double|char|String|uint8_t|uint16_t|uint32_t|size_t|byte))\s+(\w+)\s*\(([^;{}]*)\)\s*\{'
    matches=list(re.finditer(regex,sketch));prototypes=[]
    for m in matches:
        args=re.sub(r'=[^,)]+','',m.group(3))
        prototypes.append(m.group(1)+' '+m.group(2)+'('+args+');')
    index=matches[0].start() if matches else 0
    return '#include <Arduino.h>\n'+sketch[:index]+'\n'+'\n'.join(prototypes)+'\n'+sketch[index:]


def convert(row):
    case='embedbench_'+str(row['id']);c=Circuit(diagram(row));checks=[];inputs=[];skipped=[];last_t=0
    direct,buses,unresolved=c.displays()
    pots=[n for n,p in c.parts.items() if 'potentiometer' in p['type']]
    buttons=[n for n,p in c.parts.items() if p['type']=='wokwi-pushbutton']
    for name in buttons:
        p,pressed=c.button(name)
        if p is not None:inputs.append(dict(at_ms=0,kind='digital',pin=p,value=1-pressed))
    def add(test_id,kind,**kw):checks.append(dict(id=test_id+'_'+str(len(checks)),kind=kind,requirement_quote=row['problem'].strip()[:180],**kw))
    for i,text in enumerate(row['test']):
        try:t=ast.literal_eval(text.split(':',1)[1].strip()) if isinstance(text,str) else text
        except (ValueError,SyntaxError):skipped.append(dict(test=i,reason='unparseable_native_test'));continue
        desc=str(t.get('description',''));tid='native_'+str(i+1)
        if re.search(r'contradict|specific interpretation|test code logic|but .*should|should .*but',desc,re.I):
            skipped.append(dict(test=i,reason='description_signals_expected_value_conflict',description=desc));continue
        timevalue=t.get('time');now=int(float(timevalue)*1000) if isinstance(timevalue,(int,float)) else last_t+2500
        if now<=last_t:now=last_t+2500
        now=max(now,500)
        states=state_flat(t,c)
        for name in pots:
            value=states.get(name)
            if value is None and len(pots)==1:
                for key in ['pot_pos','pot_position','potentiometer_position','position']:
                    if key in t:value=t[key]
            if value is None:continue
            v=adc(value);p=c.pin(name+':SIG')
            if p is None:p=c.pin(name+':SIG.1')
            if v is not None and p is not None:inputs.append(dict(at_ms=max(last_t+1,now-400) if i else 0,kind='analog',pin=p,value=v))
        explicit_buttons=[]
        for name in buttons:
            state=str(states.get(name,'')).lower()
            if 'pressed' in state and not any(w in state for w in ['not pressed','unpressed','released']):explicit_buttons.append((name,state))
        if not explicit_buttons and re.search(r'press|pressed',desc,re.I) and not re.search(r'without .*press|no .*press|not .*press|initialization',desc,re.I):
            mentioned=[name for name in buttons if re.search(r'\b'+re.escape(name)+r'\b',desc,re.I)]
            if not mentioned and len(buttons)==1:mentioned=buttons
            if not mentioned:
                aliases=re.findall(r'(?:button|btn|k)\s*(\d+)',desc,re.I)
                mentioned=[name for name in buttons if re.search(r'('+ '|'.join(aliases)+r')$',name)] if aliases else []
            explicit_buttons=[(name,'pressed') for name in mentioned]
        for name,state in explicit_buttons:
            p,pressed=c.button(name)
            if p is None:continue
            duration=re.search(r'for\s*(\d+(?:\.\d+)?)\s*s',state)
            if duration:
                duration_ms=round(float(duration.group(1))*1000);now=max(now,last_t+duration_ms+600)
                down=now-duration_ms-200;up=now-200
            else:
                down=max(last_t+100,now-400);up=now-200
            inputs.extend([dict(at_ms=down,kind='digital',pin=p,value=pressed),dict(at_ms=up,kind='digital',pin=p,value=1-pressed)])
        for name,value in states.items():
            part=c.parts[name];kind=part['type'];s=str(value).strip().lower()
            if kind=='wokwi-led':
                p=c.pin(name+':A');active=1
                if p is None:p=c.pin(name+':C');active=0
                if p is None:continue
                on=s.startswith('on') or 'full brightness' in s
                off=s.startswith('off')
                if on or off:add(tid,'gpio',pin=p,at_ms=now,expected=active if on else 1-active)
            elif kind=='wokwi-rgb-led':
                color=next((v for k,v in COLORS.items() if re.search(r'\b'+k+r'\b',s)),None)
                if color is None:skipped.append(dict(test=i,part=name,reason='unrecognized_color',value=value));continue
                anode=c.power(name)
                for channel,on in zip('RGB',color):
                    p=c.pin(name+':'+channel)
                    if p is None:continue
                    if not on:add(tid,'pwm',pin=p,at_ms=now,expected=255 if anode else 0)
                    else:add(tid,'pwm',pin=p,at_ms=now,min=0 if anode else 1,max=254 if anode else 255)
            elif kind=='wokwi-servo':
                p=c.pin(name+':PWM');n=re.search(r'[-+]?\d+(?:\.\d+)?',s)
                if p is not None and n:add(tid,'servo',pin=p,at_ms=now,expected=float(n.group()),tolerance=2)
            elif kind=='wokwi-7segment':
                if name in unresolved:skipped.append(dict(test=i,part=name,reason='unresolved_segment_wiring'));continue
                if re.fullmatch(r'[-+]?\d+',s):add(tid,'display',name=name,at_ms=now,expected=s,numeric=True)
                elif s in ['off','blank',' ']:add(tid,'display',name=name,at_ms=now,expected='',numeric=False)
            elif kind=='wokwi-led-bar-graph':
                if isinstance(value,list) and len(value)==10:values=value
                elif 'all off' in s:values=[0]*10
                elif 'all on' in s:values=[1]*10
                elif 'others off' in s:
                    on={int(n) for n in re.findall(r'A(\d+)',s,re.I)};values=[int(n in on) for n in range(1,11)]
                else:skipped.append(dict(test=i,part=name,reason='unrecognized_bargraph_state',value=value));continue
                for n,on in enumerate(values,1):
                    p=c.pin(name+':A'+str(n));active=1
                    if p is None:p=c.pin(name+':C'+str(n));active=0
                    if p is not None:add(tid,'gpio',pin=p,at_ms=now,expected=active if on else 1-active)
        # Some original tests put bargraph lists outside a state dict; state_flat handles IDs.
        last_t=now
    if not checks:return None,dict(skipped=skipped,reason='No reliable machine-readable output assertions')
    plan=dict(horizon_ms=min(180000,max(3000,last_t+1000)),input_steps=inputs,serial_inputs=[],ultrasonic=[],seven_segments=direct,shift_registers=buses,
              checks=checks,reference_cpp=reference_cpp(row['sketch']),coverage='partial',origin='supplied_EmbedBench_descriptive_tests_and_original_reference',
              native_test_entries=row['test'],skipped_conditions=skipped,assumptions=['Convert numeric test time to milliseconds; apply changed controls 400ms before observation unless explicit hold duration exists.','Normalized potentiometer positions0..1 map to ADC0..1023.','Every inferred button press is released after200ms unless an explicit hold duration is supplied.','Reference preflight required; no claim that reference consistency proves all prose requirements.'])
    return plan,dict(skipped=skipped)


def one(row,manifest):
    case='embedbench_'+str(row['id']);folder=OUT/case;folder.mkdir(parents=True,exist_ok=True)
    result=dict(case=case,origin='supplied_dataset_tests',coverage='partial',readiness='unverified_native_conversion',rows={})
    try:
        plan,conversion=convert(row);result['conversion']=conversion
        if plan is None:host.save(folder/'result.json',result);return result
        host.save(folder/'native_plan.json',plan)
        config_case='native_'+case;host.case_files(config_case,plan,'arduino:avr:mega')
        ref=folder/'original_reference.cpp';ref.write_text(plan['reference_cpp'],encoding='utf-8')
        preflight=host.execute(ref,'arduino:avr:mega',config_case,plan,folder/'reference',True)
        result['reference_evidence']=preflight['evidence'];result['reference_status']=preflight['status']
        if preflight['status']!='passed':result['readiness']='unverified_native_reference';host.save(folder/'result.json',result);return result
        mutant=folder/'no_output.cpp';mutant.write_text('#include <Arduino.h>\nvoid setup(){}void loop(){}')
        sensitivity=host.execute(mutant,'arduino:avr:mega',config_case,plan,folder/'mutant',True)
        if sensitivity['status']!='failed':result['readiness']='unverified_native_sensitivity';host.save(folder/'result.json',result);return result
        result['readiness']='reference_consistent_native_tests'
        jobs={}
        for r in manifest:
            if r['case']!=case or not r['compile_ok']:continue
            cpp=Path(r['source']).parent.parent/'build/sketch/sketch.ino.cpp'
            key=hashlib.sha256(json.dumps([r['board'],host.sha(cpp)]).encode()).hexdigest();jobs.setdefault(key,r)
        executions={}
        for key,r in jobs.items():
            assert host.sha(r['source'])==r['source_sha256']
            cpp=Path(r['source']).parent.parent/'build/sketch/sketch.ino.cpp'
            executions[key]=host.execute(cpp,r['board'],config_case,plan,folder/'executions'/key)
        for r in manifest:
            if r['case']!=case:continue
            if not r['compile_ok']:result['rows'][r['group']]=dict(status='compile_failed_not_run');continue
            cpp=Path(r['source']).parent.parent/'build/sketch/sketch.ino.cpp';key=hashlib.sha256(json.dumps([r['board'],host.sha(cpp)]).encode()).hexdigest()
            e=executions[key];result['rows'][r['group']]=dict(status=e['status'],evidence=e['evidence'])
    except Exception as exc:result.update(readiness='unverified_native_conversion',error_type=type(exc).__name__,error=str(exc)[:500])
    host.save(folder/'result.json',result);return result


def main():
    rows=json.loads((ROOT/'tests/EmbedAgent/dataset/EmbedBench.json').read_text())
    manifest=json.loads((ROOT/'experiment/output/all_groups_host_smoke_20261023/frozen_manifest.json').read_text())
    assert len(rows)==126
    results=[]
    with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
        futures={pool.submit(one,row,manifest):row['id'] for row in rows}
        for f in concurrent.futures.as_completed(futures):
            results.append(f.result());host.save(OUT/'results.json',results)
            if len(results)%10==0:print(json.dumps(dict(native_cases=len(results),statuses=dict(collections.Counter(r['readiness'] for r in results)))),flush=True)
    host.save(OUT/'summary.json',dict(final=True,cases=len(results),statuses=dict(collections.Counter(r['readiness'] for r in results))))
    print('Native EmbedBench conversion/preflight/execution complete.',flush=True)


if __name__=='__main__':main()
