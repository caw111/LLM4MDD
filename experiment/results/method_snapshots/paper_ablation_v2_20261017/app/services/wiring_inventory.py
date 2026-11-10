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
        if device['component'] in ('wokwi-mpu6050', 'wokwi-ds1307', 'wokwi-ili9341'):
            from app.services.interface_registry import InterfaceCapabilityError
            raise InterfaceCapabilityError('No registered native acquisition/display driver for ' + device['component'] +
                                           ' (' + device['device'] + '). GPIO/I2C pin coverage cannot substitute for its device protocol.')
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
