"""Electrical facts from circuit endpoints; never infer application behavior."""
import re

from app.services.hardware_adapter import physical_pin, board_profile


def circuit_inventory(circuits, board):
    inventory = []
    for circuit in circuits:
        parts = {p['id']: p for p in circuit['parts'] if isinstance(p, dict) and isinstance(p.get('id'), str)}
        boards = {name for name, p in parts.items() if p.get('type') in ('wokwi-arduino-uno', 'wokwi-arduino-mega')}
        nets = {}
        def find(node):
            nets.setdefault(node, node)
            if nets[node] != node:
                nets[node] = find(nets[node])
            return nets[node]
        for wire in circuit['connections']:
            if isinstance(wire, list) and len(wire) >= 2 and all(isinstance(s, str) for s in wire[:2]):
                nets[find(wire[0])] = find(wire[1])
        for name,part in parts.items():
            if part.get('type')=='wokwi-pushbutton':
                for contact in ('1','2'):
                    nets[find(name+':'+contact+'.l')]=find(name+':'+contact+'.r')
        # Trace LED signal paths through resistors without merging their electrical
        # nets (a resistor is not a short, especially on pullup/divider circuits).
        resistor_paths = {}
        for rid, resistor in parts.items():
            if resistor.get('type') == 'wokwi-resistor' and rid+':1' in nets and rid+':2' in nets:
                a, b = find(rid+':1'), find(rid+':2')
                resistor_paths.setdefault(a, set()).add(b)
                resistor_paths.setdefault(b, set()).add(a)
        for name, part in parts.items():
            if name in boards or part.get('type') in ('wokwi-resistor',):
                continue
            signals, rails = {}, {}
            for endpoint in list(nets):
                device, _, terminal = endpoint.partition(':')
                if device != name:
                    continue
                pins, ties = set(), set()
                for other in list(nets):
                    bid, _, token = other.partition(':')
                    if bid not in boards or find(endpoint) != find(other):
                        continue
                    if re.fullmatch(r'[DA]?\d+', token):
                        physical_pin(token, board)
                        pins.add('D' + token if token.isdigit() else token)
                    elif token.startswith('GND') or token in ('5V', '3.3V'):
                        ties.add(token)
                if pins:
                    signals[terminal] = sorted(pins)
                if ties:
                    rails[terminal] = sorted(ties)
            kind = part.get('type', '')
            if kind in ('wokwi-led', 'wokwi-rgb-led'):
                for terminal in (('A','C') if kind == 'wokwi-led' else ('R','G','B')):
                    endpoint = name+':'+terminal
                    if endpoint not in nets:
                        continue
                    visited = {find(endpoint)}
                    frontier = set(visited)
                    for _ in range(4):
                        frontier = {v for root in frontier for v in resistor_paths.get(root, ())} - visited
                        visited.update(frontier)
                    pins = set(signals.get(terminal, []))
                    for other in list(nets):
                        bid, _, token = other.partition(':')
                        if bid in boards and re.fullmatch(r'[DA]?\d+', token) and find(other) in visited:
                            physical_pin(token, board)
                            pins.add('D'+token if token.isdigit() else token)
                    if pins:
                        signals[terminal] = sorted(pins)
            operations = []
            def operation(op, **fields):
                operations.append(dict(kind=op, **fields))
            def one(terminal):
                values = signals.get(terminal, [])
                return values[0] if len(values) == 1 else None
            if kind == 'wokwi-potentiometer' and one('SIG'):
                operation('analog_input', pin=one('SIG'))
            elif kind == 'wokwi-servo' and one('PWM'):
                operation('servo_angle', pin=one('PWM'))
            elif kind == 'wokwi-pushbutton':
                pins = sorted({pin for values in signals.values() for pin in values})
                if len(pins) == 1:
                    grounded = any(t.startswith('GND') for ties in rails.values() for t in ties)
                    roots={find(endpoint) for endpoint in nets if endpoint.partition(':')[0] in boards
                           and endpoint.partition(':')[2] in (pins[0],pins[0].removeprefix('D'))}
                    reached=set(roots);frontier=set(roots)
                    for _ in range(4):
                        frontier={v for root in frontier for v in resistor_paths.get(root,())}-reached
                        reached.update(frontier)
                    bias={token for endpoint in nets for bid,_,token in [endpoint.partition(':')]
                          if bid in boards and find(endpoint) in reached-roots and (token in ('5V','3.3V') or token.startswith('GND'))}
                    high=any(v in ('5V','3.3V') for v in bias)
                    low=any(v.startswith('GND') for v in bias)
                    if high != low:
                        operation('digital_input',pin=pins[0],active_high=not high,pullup=False)
                    else:
                        operation('digital_input', pin=pins[0], **({'active_high': False, 'pullup': True} if grounded else {}))
            elif kind in ('wokwi-dht11', 'wokwi-dht22'):
                data = one('SDA') or one('DATA')
                if data:
                    for op in ('dht_temperature', 'dht_humidity', 'dht_valid'):
                        operation(op, pin=data, sensor=kind.removeprefix('wokwi-').upper())
            elif kind == 'wokwi-hc-sr04' and one('TRIG') and one('ECHO'):
                for op in ('ultrasonic_distance', 'ultrasonic_valid'):
                    operation(op, trigger_pin=one('TRIG'), echo_pin=one('ECHO'))
            elif kind == 'wokwi-tm1637-7segment' and one('CLK') and one('DIO'):
                operation('tm1637_number', clk_pin=one('CLK'), dio_pin=one('DIO'))
                operation('tm1637_text', clk_pin=one('CLK'), dio_pin=one('DIO'))
            elif kind in ('wokwi-ds1307', 'wokwi-mpu6050') and one('SDA') and one('SCL'):
                fields = dict(sda_pin=one('SDA'), scl_pin=one('SCL'))
                if kind == 'wokwi-ds1307':
                    operation('rtc_epoch', **fields); operation('rtc_valid', **fields)
                else:
                    for op in ('mpu_acceleration', 'mpu_gyro', 'mpu_gyro_raw'):
                        for axis in (('y',) if op.endswith('raw') else ('x','y','z')):
                            operation(op, **fields, axis=axis)
                    operation('mpu_valid', **fields)
            elif kind == 'wokwi-ili9341' and one('CS') and one('D/C'):
                fields = dict(cs_pin=one('CS'), dc_pin=one('D/C'), rst_pin=one('RST') or -1,
                              mosi_pin=one('MOSI'), miso_pin=one('MISO'), sck_pin=one('SCK'))
                for op in ('lcd_text', 'lcd_number', 'lcd_datetime'):
                    operation(op, **fields)
            elif kind == 'wokwi-led-bar-graph':
                pins, levels, segments = [], [], []
                for i in range(1, 11):
                    if one('A' + str(i)) and any(t.startswith('GND') for t in rails.get('C' + str(i), [])):
                        pins.append(one('A' + str(i))); levels.append(True); segments.append(i)
                    elif one('C' + str(i)) and '5V' in rails.get('A' + str(i), []):
                        pins.append(one('C' + str(i))); levels.append(False); segments.append(i)
                if pins and segments == list(range(1,len(pins)+1)) and len(set(levels)) == 1:
                    operation('led_bar_graph', pins=pins, active_high=levels[0])
            elif kind in ('wokwi-led', 'wokwi-rgb-led'):
                terminals = ('A', 'C') if kind == 'wokwi-led' else ('R', 'G', 'B')
                for terminal in terminals:
                    pin = one(terminal)
                    if pin:
                        common = 'COM' if kind == 'wokwi-rgb-led' else ('C' if terminal == 'A' else 'A')
                        ties = rails.get(common, [])
                        active_low = '5V' in ties or '3.3V' in ties
                        operation('digital_output', pin=pin, **({'active_high':False} if active_low else {}))
                        if physical_pin(pin, board) in board_profile(board)['pwm']:
                            operation('pwm_output', pin=pin, **({'scale':-1.0,'offset':255.0} if active_low else {}))
            inventory.append(dict(device=name, component=kind, signals=signals, rails=rails, catalog_operations=operations))
    return inventory


def restore_circuit_fields(proposal, inventory, board):
    """Only an identified, unambiguous device can restore physical fields."""
    actions = []
    for request in proposal.get('interface_requests', []) if isinstance(proposal, dict) else []:
        if not isinstance(request, dict):
            continue
        devices = [d for d in inventory if d['device'] == request.get('device')]
        if not devices and request.get('kind') == 'led_bar_graph' and not request.get('device'):
            devices = [d for d in inventory if d['component'] == 'wokwi-led-bar-graph']
        choices = [op for d in devices for op in d['catalog_operations'] if op['kind'] == request.get('kind')]
        if 'axis' in request:
            choices = [op for op in choices if op.get('axis') == request['axis']]
        if len(choices) > 1 and 'pin' in request:
            choices = [op for op in choices if 'pin' in op and physical_pin(op['pin'], board) == physical_pin(request['pin'], board)]
        if not devices and not request.get('device') and 'pin' in request:
            choices = [op for d in inventory for op in d['catalog_operations']
                       if op['kind'] == request.get('kind') and 'pin' in op and
                       physical_pin(op['pin'], board) == physical_pin(request['pin'], board)]
        if len(choices) == 1:
            if ('scale' in choices[0] and request.get('unit', 'pwm_count') != 'pwm_count'):
                raise ValueError('Common-anode PWM electrical restoration expects raw pwm_count commands; '
                                 'do not overwrite an explicit physical-unit calibration. Convert the quantity in the model '
                                 'or supply a separately validated calibrated electrical mapping.')
            for field, value in choices[0].items():
                if field != 'kind' and request.get(field) != value:
                    request[field] = value
                    actions.append(str(request.get('device', 'bar graph')) + '.' + field + ': restored verified circuit fact')
    return proposal, actions


def check_circuit_inventory(contract, inventory):
    board = contract.get('board', 'arduino:avr:uno')
    for device in inventory:
        native = {'wokwi-mpu6050': ('mpu_acceleration', 'mpu_gyro', 'mpu_gyro_raw'),
                  'wokwi-ds1307': ('rtc_epoch',), 'wokwi-ili9341': ('lcd_text', 'lcd_number', 'lcd_datetime')}
        if device['component'] in native:
            operations = device['catalog_operations']
            fields = ('cs_pin', 'dc_pin', 'rst_pin', 'mosi_pin', 'miso_pin', 'sck_pin') if device['component']=='wokwi-ili9341' else ('sda_pin','scl_pin')
            if not any(p['binding']['kind'] in native[device['component']] and any(
                    all(p['binding'].get(f) == op.get(f) or
                        (p['binding'].get(f) is not None and op.get(f) not in (None,-1) and
                         physical_pin(p['binding'][f],board)==physical_pin(op[f],board)) for f in fields)
                    for op in operations) for p in contract['interfaces']):
                raise ValueError('Circuit device '+device['device']+' requires its registered native device operation; GPIO cannot replace the protocol')
            continue
        ops = device['catalog_operations']
        if not ops:
            continue
        # DHT quantities are selected by the requirement; validity is checked by the sensor validator.
        if device['component'] in ('wokwi-dht11', 'wokwi-dht22', 'wokwi-hc-sr04'):
            continue
        expected = {}
        for op in ops:
            if op['kind'] == 'led_bar_graph':
                for pin in op['pins']:
                    expected.setdefault(pin, []).append(dict(kind='digital_output', pin=pin, active_high=op['active_high']))
            elif 'pin' in op:
                expected.setdefault(op['pin'], []).append(op)
            else:
                expected.setdefault(device['device'], []).append(op)
        for choices in expected.values():
            def matches(op, binding):
                return binding.get('kind') == op['kind'] and all(
                    (field in binding and physical_pin(binding[field], board) == physical_pin(value, board))
                    if field == 'pin' or field.endswith('_pin') else binding.get(field, True if field == 'active_high' else False) == value
                    for field, value in op.items() if field != 'kind')
            if not any(matches(op, p['binding']) for op in choices for p in contract['interfaces']):
                raise ValueError('Circuit device ' + device['device'] + ' missing matching operation/configuration: ' + str(choices) +
                                 '. Use source component device ID and these MCU pins; component terminals are not MCU pins.')
