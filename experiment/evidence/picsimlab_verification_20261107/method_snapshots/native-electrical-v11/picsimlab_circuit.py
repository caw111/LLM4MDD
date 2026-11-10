"""Translate supplied wiring into native PICSimLab sensor/actuator parts."""
import json
import re

from picsimlab_assertions import pin


def led_pins(case):
    """Output polarity from supplied LED wiring, independent of candidate code."""
    config=case.get('configuration_resolution') or {}
    diagram=(config.get('resolved_circuits') or [None])[0]
    text=case.get('effective_requirement') or case['requirement']
    if diagram is None and 'Given circuit (must preserve wiring):' in text:
        diagram=json.JSONDecoder().raw_decode(text.split('Given circuit (must preserve wiring):',1)[1].lstrip())[0]
    if not diagram:return {}
    graph={};boards={p['id'] for p in diagram['parts'] if 'arduino' in p['type']}
    for a,b,*_ in diagram['connections']:
        graph.setdefault(a,[]).append(b);graph.setdefault(b,[]).append(a)
    for part in diagram['parts']:
        if part['type']=='wokwi-resistor':
            a,b=part['id']+':1',part['id']+':2';graph.setdefault(a,[]).append(b);graph.setdefault(b,[]).append(a)
    def net(node):
        todo=[node];seen=set();found=[]
        while todo:
            item=todo.pop()
            if item in seen:continue
            seen.add(item)
            dev,_,port=item.partition(':')
            if dev in boards:found.append(port)
            todo.extend(graph.get(item,[]))
        return found
    result={}
    for part in diagram['parts']:
        kind=part['type'];pid=part['id']
        if kind=='wokwi-led':ports=['A'];active=1
        elif kind=='wokwi-rgb-led':ports=['R','G','B'];active=0 if any(v in ('5V','3.3V') for v in net(pid+':COM')) else 1
        elif kind=='wokwi-led-bar-graph':ports=['A'+str(i) for i in range(1,11)];active=1
        else:continue
        for port in ports:
            for value in net(pid+':'+port):
                try:result[pin(value,case['fqbn'])]=active
                except ValueError:pass
    return result


def native_parts(case,plan,mapping):
    board=case['fqbn'];config=case.get('configuration_resolution') or {}
    diagram=(config.get('resolved_circuits') or [None])[0]
    text=case.get('effective_requirement') or case['requirement']
    if diagram is None and 'Given circuit (must preserve wiring):' in text:
        try:diagram=json.JSONDecoder().raw_decode(text.split('Given circuit (must preserve wiring):',1)[1].lstrip())[0]
        except ValueError:pass
    lines=[];info={'parts':[],'dht_pins':[],'ultrasonic_echo_pins':[],'lcd_pins':[]}
    def add(name,prefs,metadata=None):
        lines.append(f'{name},60,60,0:'+','.join(map(str,prefs)));info['parts'].append(dict(name=name,preferences=prefs,**(metadata or {})))
    dht={}
    if diagram:
        graph={};boards={p['id'] for p in diagram['parts'] if 'arduino' in p['type']}
        for connection in diagram['connections']:
            a,b=connection[:2];graph.setdefault(a,[]).append(b);graph.setdefault(b,[]).append(a)
        def net(part,port):
            pending=[part+':'+port];seen=set()
            while pending:
                item=pending.pop()
                if item in seen:continue
                seen.add(item)
                dev,sep,value=item.partition(':')
                if dev in boards:
                    if value in ('5V','3.3V','3V3'):return 10 if 'mega' in board else 7
                    if value.startswith('GND'):return 11 if 'mega' in board else 8
                    aliases={'SDA':20 if 'mega' in board else 18,'SCL':21 if 'mega' in board else 19}
                    try:return mapping[aliases[value] if value in aliases else pin(value,board)]
                    except (KeyError,ValueError):pass
                pending.extend(graph.get(item,[]))
            return 0
        for part in diagram['parts']:
            kind=part['type'];pid=part['id']
            if kind in ('wokwi-dht22','wokwi-dht11'):
                physical=net(pid,'SDA') or net(pid,'DATA');dht[physical]='DHT22' if '22' in kind else 'DHT11'
            elif kind=='wokwi-ds1307':add('RTC ds1307',[net(pid,'SDA'),net(pid,'SCL'),net(pid,'SQW')])
            elif kind=='wokwi-mpu6050':add('MPU6050',[net(pid,p) for p in ('SCL','SDA','XDA','XCL','AD0','INT')])
            elif kind=='wokwi-ili9341':
                values=[net(pid,p) or (net(pid,'D/C') if p=='DC' else 0) for p in ('SCK','MOSI','RST','DC','CS')]
                add('LCD ili9341',values+[0]+[0]*13);info['lcd_pins'].append(values)
            elif kind=='wokwi-servo':add('Servo Motor',[net(pid,'PWM')])
    for step in plan.get('input_steps',[]):
        if step['kind'] in ('temperature','humidity'):
            physical=mapping[pin(step['pin'],board)]
            if not diagram:dht[physical]='DHT22' if 'DHT22' in text.upper() else 'DHT11'
    for device in config.get('devices',[]):
        if device['kind']=='dht':
            physical=mapping[pin(device['resolved']['pin'],board)]
            dht[physical]='DHT22' if 'DHT22' in text.upper() else 'DHT11'
    for physical,kind in dht.items():
        if physical:add(kind+' (Temp. Hum.)',[physical,92,100]);info['dht_pins'].append(physical)
    for ultra in plan.get('ultrasonic',[]):
        trigger=mapping[pin(ultra['trigger_pin'],board)];echo=mapping[pin(ultra['echo_pin'],board)]
        add('Ultrasonic HC-SR04',[trigger,echo,150]);info['ultrasonic_echo_pins'].append(echo)
    # Source tasks without a supplied diagram use the explicitly declared test bench.
    if not diagram:
        servo_pins=set()
        for check in plan['checks']:
            if check['kind']=='servo' or check.get('source')=='servo':servo_pins.add(mapping[pin(check['pin'],board)])
        for physical in servo_pins:add('Servo Motor',[physical])
    return lines,info
