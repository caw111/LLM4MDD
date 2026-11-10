"""Program-owned, versioned configuration decisions shared by every study arm."""
import copy
import json
import re

VERSION = 'configuration-defaults-v3'


def _resolve_circuits(requirement, pins, prose, circuits, board, profile):
    from app.services.hardware_adapter import physical_pin
    resolved, changes = copy.deepcopy(circuits), []
    endpoints = []
    for ci, circuit in enumerate(resolved):
        parts = {p.get('id'): p.get('type') for p in circuit['parts'] if isinstance(p, dict)}
        for ni, connection in enumerate(circuit['connections']):
            if not isinstance(connection, list) or len(connection) < 2:
                continue
            for side in (0, 1):
                endpoint, peer = connection[side], connection[1-side]
                if not isinstance(endpoint, str) or not isinstance(peer, str):
                    continue
                identifier, _, token = endpoint.partition(':')
                if parts.get(identifier) not in ('wokwi-arduino-uno', 'wokwi-arduino-mega') or not re.fullmatch(r'[DA]?\d+', token):
                    continue
                number = physical_pin(token, board)
                device, _, terminal = peer.partition(':')
                kind = parts.get(device, '')
                pwm = kind == 'wokwi-servo' or (kind == 'wokwi-led' and bool(re.search(r'PWM\s*(?:LED\s*)?(?:D)?' + str(number) + r'\b', prose, re.I)))
                priority = 0 if pwm or number >= profile['analog_base'] else (2 if kind == 'wokwi-led' else 5)
                allowed = set(profile['pwm']) if pwm and kind != 'wokwi-servo' else set(range(2, profile['digital_count']))
                endpoints.append(dict(circuit=ci, connection=ni, side=side, number=number,
                                      owner=(device, terminal), device_kind=kind, priority=priority, allowed=allowed))
    reserved = {e['number'] for e in endpoints}
    occupied, occupied_kinds, remapped = {}, {}, {}
    for e in sorted(endpoints, key=lambda x: x['priority']):
        # Repeated wires to the same device terminal are one signal, not a conflict.
        key = (e['circuit'], e['number'])
        if key in occupied and e['owner'][1].upper() in ('SDA', 'SCL', 'SCK', 'MOSI', 'MISO') and occupied[key][1].upper() == e['owner'][1].upper():
            continue
        loads = {'wokwi-led', 'wokwi-led-bar-graph'}
        if e['device_kind'] in loads and occupied_kinds.get(key) in loads:
            # Parallel passive loads can intentionally mirror one output signal.
            continue
        if key not in occupied or occupied[key] == e['owner']:
            occupied[key] = e['owner']
            occupied_kinds[key] = e['device_kind']
            continue
        signal = (e['circuit'], e['number'], e['owner'])
        chosen = remapped.get(signal)
        if chosen is None:
            chosen = next((p for p in sorted(e['allowed']) if p not in reserved), None)
        if chosen is None:
            raise ValueError('No compatible pin available to resolve circuit conflict')
        circuit = resolved[e['circuit']]
        before = circuit['connections'][e['connection']][e['side']]
        after = before.partition(':')[0] + ':' + str(chosen)
        circuit['connections'][e['connection']][e['side']] = after
        reserved.add(chosen)
        remapped[signal] = chosen
        occupied[e['circuit'], chosen] = e['owner']
        occupied_kinds[e['circuit'], chosen] = e['device_kind']
        changes.append(dict(device=e['owner'][0], terminal=e['owner'][1], original=before,
                            resolved=after, reason='explicit circuit conflict; rewiring required'))
    for literal in re.finditer(r'set the property name "value" to "([0-9]+(?:\.[0-9]+)? ohm)"',prose,re.I):
        magnitude=literal[1].split()[0]
        for circuit in resolved:
            for part in circuit['parts']:
                if part.get('type')=='wokwi-resistor' and part.get('attrs',{}).get('value')==magnitude:
                    part['attrs']['value']=literal[1]
                    changes.append(dict(device=part['id'],terminal='value',original=magnitude,
                        resolved=literal[1],reason='Explicit equivalent-resistance property representation; wiring and resistance unchanged'))
    decoder = json.JSONDecoder()
    replacements = []
    for match in re.finditer(r'\{', requirement):
        try:
            value, length = decoder.raw_decode(requirement[match.start():])
        except ValueError:
            continue
        for old, new in zip(circuits, resolved):
            if value == old and old != new:
                replacements.append((match.start(), match.start()+length, json.dumps(new, ensure_ascii=False)))
    effective = requirement
    for a, b, value in sorted(set(replacements), reverse=True):
        effective = effective[:a] + value + effective[b:]
    resolved_pins = copy.deepcopy(pins)
    for old, new in zip(circuits, resolved):
        if pins == old:
            resolved_pins = new
    # Rewrite matching prose labels too, so it does not contradict the new diagram.
    for change in changes:
        terminal = change['terminal']
        if terminal.upper() not in ('CLK', 'DIO', 'TRIG', 'ECHO'):
            continue
        old = 'D' + change['original'].partition(':')[2].removeprefix('D')
        new = 'D' + change['resolved'].partition(':')[2]
        effective = re.sub(r'(' + re.escape(terminal) + r'\s*[:=]\s*)' + re.escape(old) + r'\b',
                           lambda m: m[1]+new, effective, flags=re.I)
    return dict(version=VERSION, board=board, original_requirement=requirement,
                effective_requirement=effective, pins=resolved_pins, devices=[], changes=changes,
                serial=None, resolved_circuits=resolved, circuit_preserved=not changes)


def resolve_configuration(requirement, pins=None, *, board=None):
    from app.services.hardware_adapter import board_profile, physical_pin
    from app.services.interface_planner import _wiring_sources, requested_board
    board = requested_board(requirement, board) or 'arduino:avr:uno'
    profile = board_profile(board)
    marker = '\nResolved hardware configuration (authoritative pin assignments;'
    if marker in requirement:
        block = requirement.split(marker, 1)[1].split('\n', 1)[1]
        data, _ = json.JSONDecoder().raw_decode(block)
        if data.get('version') != VERSION or data.get('board') != board:
            raise ValueError('Resolved configuration version/board does not match the current policy')
        devices = []
        for item in data['devices']:
            fields = {k:v for k,v in item.items() if k.endswith('_pin') or k == 'pin'}
            for value in fields.values():
                physical_pin(value, board)
            devices.append(dict(kind=item['device_role'], original=fields.copy(), resolved=fields))
        return dict(version=VERSION, board=board, original_requirement=requirement,
                    effective_requirement=requirement, pins=copy.deepcopy(pins or {}), devices=devices,
                    changes=[], serial=data.get('serial'), circuit_preserved=False, already_resolved=True)
    prose, circuits = _wiring_sources(requirement, pins or {})
    # Resolve actual endpoint conflicts without inventing another copy of a device.
    if circuits:
        result = _resolve_circuits(requirement, pins or {}, prose, circuits, board, profile)
        if re.search(r'串口|\bSerial\b|\bUART\b', prose, re.I):
            baud = re.search(r'(?:baud(?:\s+rate)?\s*[:=]?\s*(\d+))|(?:(\d+)\s*(?:baud|波特率))|(?:波特率\s*[:=]?\s*(\d+))', prose, re.I)
            result['serial'] = dict(baud=int(next(v for v in baud.groups() if v)) if baud else 9600,
                                    framing='line_or_idle', idle_timeout_ms=20)
            if not baud:
                result['effective_requirement'] += '\nSerial baud=9600; framing=line_or_idle; idle_timeout_ms=20.'
                result['changes'].append(dict(field='serial.baud', original=None, resolved=9600, reason='unspecified transport configuration'))
        return result
    devices, changes = [], []
    # Resolve compact adjacent pairs left-to-right: D2 button D13 LED must
    # not also mean "button D13". Never infer a conflict from overlapping pairs.
    role_kinds={'button':'digital_input','pushbutton':'digital_input','按钮':'digital_input','按键':'digital_input',
                'led':'digital_output','指示灯':'digital_output','servo':'servo_angle','舵机':'servo_angle',
                'buzzer':'tone_output','蜂鸣器':'tone_output'}
    tokens=list(re.finditer(r'(?<![A-Za-z0-9_])D\d+(?!\d)|\b(?:button|pushbutton|LED|servo|buzzer)\b|按钮|按键|指示灯|舵机|蜂鸣器',prose,re.I))
    pin_owners={}
    index=0
    while index+1<len(tokens):
        left,right=tokens[index:index+2]
        gap=prose[left.end():right.start()]
        a,b=left[0].lower(),right[0].lower()
        if re.fullmatch(r'\s*(?:引脚)?(?:的)?\s*',gap) and ((a in role_kinds) != (b in role_kinds)):
            pin=right if a in role_kinds else left
            role=a if a in role_kinds else b
            pin_owners[pin.span()]=role_kinds[role]
            index+=2
        else:index+=1

    def device(kind, fields, preferred, patterns):
        if not re.search(patterns[0], prose, re.I):
            return
        values, spans, source_keys = {}, {}, {}
        for field, pattern in patterns[1].items():
            patterns_for_field = pattern if isinstance(pattern, list) else [pattern]
            matches = [m for expression in patterns_for_field for m in re.finditer(expression, prose, re.I)]
            if kind in ('digital_input','digital_output','servo_angle','tone_output'):
                matches=[m for m in matches if pin_owners.get(m.span('pin'),kind)==kind]
            if field == 'pin':
                # A role followed by a pin list declares several devices, not just its first pin.
                extra = []
                for match in matches:
                    tail = re.match(r'(?:\s*(?:、|,|/|或|和|及|and)\s*[DA]\d+)+', prose[match.end('pin'):], re.I)
                    if tail:
                        extra += list(re.compile(r'(?P<pin>[DA]\d+)', re.I).finditer(
                            prose, match.end('pin'), match.end('pin') + tail.end()))
                matches += extra
            if field == 'pin' and kind == 'pwm_output' and not matches and not re.search(r'风扇|\bfan\b|电机|\bmotor\b', prose, re.I):
                # A PWM explanation for one existing LED is not a new device.
                matches = [m for expression in (r'(?P<pin>D\d+)\s*(?:引脚)?(?:的)?\s*LED',
                                                r'LED\s*[（(]?\s*(?P<pin>D\d+)')
                           for m in re.finditer(expression, prose, re.I)]
            if field == 'pin' and kind == 'digital_output':
                for group in re.finditer(r'(?:LED|指示灯)[^。\n；;]{0,18}(?:分别)?连接(?:在|到)?[^。\n；;]+', prose, re.I):
                    matches += list(re.compile(r'(?P<pin>D\d+)').finditer(prose, group.start(), group.end()))
            found = {m.group('pin').upper() for m in matches}
            labels = {'clk_pin': 'CLK', 'dio_pin': 'DIO', 'trigger_pin': 'TRIG', 'echo_pin': 'ECHO'}
            keys = [key for key, value in (pins or {}).items()
                    if isinstance(key, str) and isinstance(value, str) and re.fullmatch(r'[DA]\d+', value, re.I)
                    and ((field == 'pin' and re.search(patterns[0], key, re.I))
                         or key.upper() == labels.get(field))]
            found.update(pins[key].upper() for key in keys)
            if len(found) > 1:
                if fields == ['pin']:
                    for value in sorted(found):
                        devices.append(dict(kind=kind, fields=fields, preferred=preferred,
                            original={field:value}, spans={field:[(m.start('pin'),m.end('pin')) for m in matches if m['pin'].upper()==value]},
                            source_keys={field:[k for k in keys if pins[k].upper()==value]}, resolved={}))
                # Several buses of one class need instance-level endpoint matching.
                return
            if found:
                values[field] = next(iter(found))
                spans[field] = [(m.start('pin'), m.end('pin')) for m in matches]
                source_keys[field] = keys
        devices.append(dict(kind=kind, fields=fields, preferred=preferred,
                            original=values, spans=spans, source_keys=source_keys, resolved={}))

    # More constrained pin roles have priority over movable digital buses.
    device('pwm_output', ['pin'], [5, 9, 6, 10, 3, 11],
           (r'PWM', {'pin': [r'PWM\s*(?:LED\s*)?(?:控制)?(?:引脚)?\s*[（(:=]?\s*(?P<pin>D\d+)',
                            r'(?P<pin>D\d+)\s*PWM']}))
    for kind, name, preferred in [('analog_input', r'电位器|potentiometer', 'A0'),
                                  ('analog_input', r'LM35', 'A1'),
                                  ('analog_input', r'光敏电阻|photoresistor', 'A0')]:
        device(kind, ['pin'], [preferred], (name, {'pin': [r'(?:' + name + r')\s*(?:温度传感器|传感器)?\s*[（(]?\s*(?P<pin>A\d+)',
                                                       r'(?P<pin>A\d+)\s*(?:引脚)?(?:的)?\s*(?:' + name + r')'] +
                                                      ([r'(?:环境|蒸发器|室内|室外)温度\s*(?P<pin>A\d+)'] if name == 'LM35' else [])}))
    for name, kind, preferred in [(r'按钮|按键|button|pushbutton', 'digital_input', 2),
                                   (r'(?<!PWM )LED|指示灯|继电器|加热器|风扇|水泵|增氧泵|气泵|fan|relay|pump', 'digital_output', 13),
                                   (r'蜂鸣器|buzzer', 'tone_output', 6),
                                   (r'舵机|servo', 'servo_angle', 10)]:
        patterns=[r'(?:' + name + r')\s*(?:PWM\s*)?(?:控制引脚|引脚|连接到?|接)?\s*[（(]?\s*(?P<pin>D\d+)',
                  r'(?P<pin>D\d+)\s*(?:引脚)?(?:的)?\s*(?:' + name + r')']
        if kind=='digital_output':
            colors=r'红色|绿色|黄色|蓝色|白色|red|green|yellow|blue|white'
            patterns += [r'(?:LED|指示灯)\s*(?:'+colors+r')\s*[（(:=]?\s*(?P<pin>D\d+)',
                         r'(?:'+colors+r')\s*(?:LED|指示灯)?\s*[（(:=]?\s*(?P<pin>D\d+)']
        device(kind, ['pin'], [preferred], (name, {'pin': patterns}))
    device('dht', ['pin'], [7], (r'DHT(?:11|22)',
           {'pin': [r'DHT(?:11|22)\s*(?:温湿度传感器)?\s*[（(]?\s*(?:数据引脚\s*)?(?P<pin>D\d+)',
                    r'(?P<pin>D\d+)\s*(?:引脚)?(?:的)?\s*(?:数据)?(?:温湿度)?传感器\s*DHT(?:11|22)',
                    r'(?:客厅|卧室|房间[一二三\d]*)传感器\s*(?P<pin>D\d+)']}))
    device('ultrasonic', ['trigger_pin', 'echo_pin'], [8, 9], (r'HC-SR04', {
        'trigger_pin': [r'(?:Trig|trigger)\s*(?:[:=]|连接到?|接|on|pin)\s*(?P<pin>D\d+)', r'HC-SR04\s*[（(]\s*(?P<pin>D\d+)\s*/\s*D\d+'],
        'echo_pin': [r'Echo\s*(?:[:=]|连接到?|接|on|pin)\s*(?P<pin>D\d+)', r'HC-SR04\s*[（(]\s*D\d+\s*/\s*(?P<pin>D\d+)']}))
    device('tm1637_number', ['clk_pin', 'dio_pin'], [4, 5], (r'TM1637', {
        'clk_pin': r'CLK\s*(?:[:=]|连接到?|接|on|pin)\s*(?P<pin>D\d+)',
        'dio_pin': r'DIO\s*(?:[:=]|连接到?|接|on|pin)\s*(?P<pin>D\d+)'}))
    pwm_pins = {d['original'].get('pin') for d in devices if d['kind'] == 'pwm_output'} - {None}
    devices = [d for d in devices if not (d['kind'] == 'digital_output' and d['original'].get('pin') in pwm_pins)]
    serial = None
    if re.search(r'串口|\bSerial\b|\bUART\b', prose, re.I):
        baud = re.search(r'(?:baud(?:\s+rate)?\s*[:=]?\s*(\d+))|(?:(\d+)\s*(?:baud|波特率))|(?:波特率\s*[:=]?\s*(\d+))', prose, re.I)
        serial = dict(baud=int(next(v for v in baud.groups() if v)) if baud else 9600,
                      framing='line_or_idle', idle_timeout_ms=20)
        receive_clauses = re.findall(r'(?:串口|Serial|UART)[^。\n]{0,240}', prose, re.I)
        tokens = sorted({t for clause in receive_clauses if re.search(r'输入|接收|input|receive|命令|command', clause, re.I)
                         for t in re.findall(r'\b[A-Z][A-Z0-9_]{1,24}\b', clause)
                         if not re.fullmatch(r'[DA]\d+|LED|PWM|DHT11|DHT22|HC|SR04|TM1637|UART|ONBOARD', t)})
        if tokens:
            serial['commands'] = {token: i + 1 for i, token in enumerate(tokens)}
        if not baud:
            changes.append(dict(field='serial.baud', original=None, resolved=9600, reason='unspecified transport configuration'))
    used = {0, 1}  # Keep programming/UART pins available even without reporting.
    resolved_pins = copy.deepcopy(pins or {})
    # Reserve every declared source pin, so new defaults cannot steal a later device's pin.
    declared = {physical_pin(p, board) for p in re.findall(r'\b[DA]\d{1,2}\b', prose)}
    replacements = []
    for dev in devices:
        for i, field in enumerate(dev['fields']):
            original = dev['original'].get(field)
            old_number = physical_pin(original, board) if original else None
            allowed = profile['pwm'] if dev['kind'] == 'pwm_output' else set(range(2, profile['digital_count']))
            if dev['kind'] == 'pwm_output':
                allowed = set(allowed)
                if any(d['kind'] == 'tone_output' for d in devices):
                    allowed -= profile['tone_pwm_conflicts']
                if any(d['kind'] == 'servo_angle' for d in devices):
                    allowed -= profile['servo_timers'][0]
            if dev['kind'] in ('analog_input', 'lm35_input', 'light_input'):
                allowed = set(range(profile['analog_base'], profile['analog_base'] + profile['analog_count']))
            if old_number is not None and old_number in allowed and old_number not in used:
                chosen = old_number
            else:
                preferences = [physical_pin(p, board) for p in dev['preferred'][i:i+1]]
                candidates = preferences + sorted(allowed)
                chosen = next((p for p in candidates if p in allowed and p not in used and p not in declared), None)
                if chosen is None:
                    raise ValueError('No free compatible pin for ' + dev['kind'] + '.' + field)
            used.add(chosen)
            resolved = ('A' + str(chosen - profile['analog_base']) if dev['kind'] in ('analog_input', 'lm35_input', 'light_input') else 'D' + str(chosen))
            dev['resolved'][field] = resolved
            if original != resolved:
                changes.append(dict(device=dev['kind'], field=field, original=original, resolved=resolved,
                                    reason='conflict or incompatible pin' if original else 'unspecified wiring'))
                replacements += [(a, b, resolved) for a, b in dev['spans'].get(field, [])]
                for key in dev['source_keys'].get(field, []):
                    resolved_pins[key] = resolved
    effective = prose.rsplit('\n', 1)[0] if prose.endswith('\n' + json.dumps(pins or {}, ensure_ascii=False)) else requirement
    # Spans refer to the original prose; replacements only touch a named device role.
    for a, b, value in sorted(replacements, reverse=True):
        effective = effective[:a] + value + effective[b:]
    public_devices = [{k: v for k, v in d.items() if k not in ('spans', 'source_keys', 'preferred', 'fields')} for d in devices]
    config = dict(version=VERSION, board=board, devices=public_devices, serial=serial)
    prompt_config = dict(config, devices=[dict(device_role=d['kind'], **d['resolved']) for d in public_devices])
    effective += ('\nResolved hardware configuration (authoritative pin assignments; original conflicts were explicitly remapped). '
                  'This is a PARTIAL inventory of recognized roles, not a whitelist: all other explicit source wiring remains required. '
                  'device_role identifies wiring only, not the final catalog operation: preserve requested PWM, '
                  'calibration, thresholds, timers and all other behavior.\n' + json.dumps(prompt_config, ensure_ascii=False))
    if serial:
        if re.search(r'framing\s*[:=]\s*["\x27]?line\b(?!_)|line.only|仅[^。\n]{0,10}换行', prose, re.I):
            serial['framing'] = 'line'
        effective += '\nSerial baud=' + str(serial['baud']) + '; framing=' + serial['framing'] + '; idle_timeout_ms=20.'
    return dict(**config, original_requirement=requirement, effective_requirement=effective,
                pins=resolved_pins, changes=changes, circuit_preserved=False)


def apply_configuration(proposal, resolution):
    """Restore resolved physical fields before the normal strict validators run."""
    result = copy.deepcopy(proposal)
    actions = []
    if not isinstance(result, dict):
        return result, actions
    aliases = {'dht_temperature': 'dht', 'dht_humidity': 'dht', 'dht_valid': 'dht',
               'ultrasonic_distance': 'ultrasonic', 'ultrasonic_valid': 'ultrasonic'}
    from app.services.hardware_adapter import physical_pin
    ports = result.get('interface_requests', result.get('interfaces', []))
    if not isinstance(ports, list):
        raise ValueError('interface_requests/interfaces must be an array')
    for port in ports:
        if not isinstance(port, dict):
            raise ValueError('Each interface request must be an object')
        request = port.get('binding', port)
        if not isinstance(request, dict):
            raise ValueError('Interface binding must be an object')
        kind = aliases.get(request.get('kind'), request.get('kind'))
        records = [d for d in resolution['devices'] if d['kind'] == kind]
        if len(records) > 1:
            resolved_matches = [d for d in records if any(field in request and physical_pin(request[field], resolution['board']) == physical_pin(pin, resolution['board'])
                                for field, pin in d['resolved'].items())]
            records = resolved_matches or [d for d in records if any(field in request and physical_pin(request[field], resolution['board']) == physical_pin(original, resolution['board'])
                                          for field, original in d['original'].items())]
        if len(records) == 1:
            for field, pin in records[0]['resolved'].items():
                original = records[0]['original'].get(field)
                if field in request and physical_pin(request[field], resolution['board']) == physical_pin(pin, resolution['board']):
                    continue
                matches_original = original is not None and field in request and physical_pin(request[field], resolution['board']) == physical_pin(original, resolution['board'])
                if field not in request or matches_original:
                    request[field] = pin
                    actions.append(request['kind'] + '.' + field + ': resolved configuration ' + pin)
        if resolution['serial'] and request.get('kind', '').startswith('serial_'):
            request.setdefault('baud', resolution['serial']['baud'])
            if request['kind'] in ('serial_command_input', 'serial_integer_input'):
                request.setdefault('framing', resolution['serial']['framing'])
                request.setdefault('idle_timeout_ms', resolution['serial']['idle_timeout_ms'])
            if request['kind'] == 'serial_command_input' and not request.get('commands') and resolution['serial'].get('commands'):
                request['commands'] = resolution['serial']['commands']
    channels=result.get('serial_channels', [])
    if not isinstance(channels,list):
        raise ValueError('serial_channels must be an array of objects')
    for index,channel in enumerate(channels):
        if not isinstance(channel,dict):
            raise ValueError(f'serial_channels[{index}] must be an object with baud, trigger and values; strings/field names are not channels')
        if resolution['serial']:
            channel.setdefault('baud', resolution['serial']['baud'])
    return result, actions
