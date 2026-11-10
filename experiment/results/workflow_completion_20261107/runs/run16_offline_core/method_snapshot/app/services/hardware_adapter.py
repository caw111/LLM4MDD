"""Deterministic Arduino glue from a validated interface inventory.

Bindings describe hardware operations, never arbitrary C++ or control expressions.
Business timing, thresholds and transitions remain in the compiled model.
"""
import json
import math
import re

from app.services.behavior_contract import validate_contract
from app.services.interface_semantics import digital_levels
from app.services.interface_registry import SERIAL_INPUTS, CATALOG
from app.services.serial_receiver import validate_receivers, receiver_code

from app.services.ultrasonic import KINDS as ULTRASONIC_KINDS, FIELDS as ULTRASONIC_FIELDS, groups as ultrasonic_groups, sensor_key
from app.services.dht import KINDS as DHT_KINDS
from app.services import extended_devices as extended

CPP_TYPES = {'bool': 'bool', 'i32': 'int32_t', 'f32': 'float', 'f64': 'double'}
INPUTS = {'digital_input', 'analog_input', 'elapsed_ms', 'serial_valid'} | SERIAL_INPUTS | DHT_KINDS | ULTRASONIC_KINDS
OUTPUTS = {'digital_output', 'pwm_output', 'serial_value', 'report_trigger', 'tone_output', 'tm1637_number', 'servo_angle', 'shift595_number', 'serial_text_output'}
INPUTS |= extended.INPUTS
OUTPUTS |= extended.OUTPUTS
BOARDS = {
    'arduino:avr:uno': {'digital_count':20, 'analog_count':6, 'analog_base':14,
                        'pwm':{3,5,6,9,10,11}, 'tone_pwm_conflicts':{3,11}, 'servo_timers':[{9,10}], 'macro':'ARDUINO_AVR_UNO'},
    'arduino:avr:mega': {'digital_count':70, 'analog_count':16, 'analog_base':54,
                         'pwm':set(range(2,14)) | {44,45,46}, 'tone_pwm_conflicts':{9,10}, 'servo_timers':[{44,45,46},{11,12},{2,3,5},{6,7,8}], 'macro':'ARDUINO_AVR_MEGA2560'},
}


def board_profile(board):
    if not isinstance(board, str) or board not in BOARDS:
        raise ValueError('Unsupported hardware board: ' + str(board))
    return BOARDS[board]


def pin(value, board='arduino:avr:uno'):
    profile = board_profile(board)
    if isinstance(value, bool):
        raise ValueError('Pin must be a digital number or analog pin name for ' + board)
    token = str(value)
    if re.fullmatch(r'A(?:0|[1-9][0-9]*)', token) and int(token[1:]) < profile['analog_count']:
        return token
    if re.fullmatch(r'D?(?:0|[1-9][0-9]*)', token) and int(token.removeprefix('D')) < profile['digital_count']:
        return token.removeprefix('D')
    raise ValueError('Unsupported pin for ' + board + ': ' + token)


def physical_pin(value, board='arduino:avr:uno'):
    value = pin(value, board)
    return board_profile(board)['analog_base'] + int(value[1:]) if value.startswith('A') else int(value)


def binding_pins(binding):
    """All physical signals, including devices with more than one pin."""
    return [binding[k] for k in ('pin', 'clk_pin', 'dio_pin', 'data_pin', 'clock_pin', 'latch_pin', 'trigger_pin', 'echo_pin') if k in binding] + extended.all_pins(binding)


def number(value):
    if isinstance(value, bool) or not isinstance(value, (int, float)) or not math.isfinite(value):
        raise ValueError('Hardware scaling values must be finite numbers')
    return repr(value)


def literal(value):
    return ('true' if value else 'false') if isinstance(value, bool) else number(value)


def validate_hardware_contract(contract):
    validate_contract(contract)
    period = contract['control_period_ms']
    if not isinstance(period, int) or not 1 <= period <= 2147483647:
        raise ValueError('control_period_ms must be an integer in 1..2147483647')
    board = contract.get('board', 'arduino:avr:uno')
    profile = board_profile(board)
    ports = {p['name']: p for p in contract['interfaces']}
    ultrasonic_groups(contract)
    chains = {}
    for p in ports.values():
        binding = p.get('binding')
        if isinstance(binding, dict) and binding.get('kind') == 'shift595_number':
            fields = ('data_pin', 'clock_pin', 'latch_pin')
            if all(field in binding for field in fields):
                chain = tuple(physical_pin(binding[field], board) for field in fields)
                if chain in chains:
                    raise ValueError('Interfaces ' + chains[chain] + ' and ' + p['name'] +
                                     ' share one 74HC595 cascade. Declare one i32 output for the whole number '
                                     'and list every register once in its binding; digit selects each decimal position. '
                                     'Do not split tens/units into separate interfaces. Compute the number in SysML; '
                                     'the adapter formats its digits. Preserve all devices and requirements.')
                chains[chain] = p['name']
    used_pins = {}
    report_triggers = set()
    consumed_triggers = set()
    bauds = set()
    shift_devices = set()
    shift_displays = set()
    for p in ports.values():
        binding = p.get('binding')
        if not isinstance(binding, dict):
            raise ValueError('Structured hardware binding required for ' + p['name'] +
                             '; interface ' + p['name'] + '.binding must be an object containing kind and the catalog binding fields')
        kind = binding.get('kind')
        if not isinstance(kind, str) or kind not in (INPUTS if p['direction'] == 'input' else OUTPUTS):
            raise ValueError('Unsupported hardware binding for ' + p['name'] + ': ' + str(kind) +
                             '; field ' + p['name'] + '.binding.kind; allowed ' + p['direction'] + ' kinds: ' +
                             json.dumps(sorted(INPUTS if p['direction'] == 'input' else OUTPUTS)) +
                             '. Preserve the actual device; if no catalog kind implements it, report unsupported rather than substitute a device.')
        allowed = {'kind'}
        if kind in extended.CATALOG:
            allowed |= extended.CATALOG[kind][4]
        if kind == 'elapsed_ms':
            allowed.add('semantics')
            if binding.get('semantics', 'delta_since_previous_step') != 'delta_since_previous_step':
                raise ValueError('elapsed_ms is delta_since_previous_step, never absolute uptime')
        if kind in ('digital_input', 'digital_output'):
            allowed |= {'pin', 'active_high'}
        if kind == 'digital_input':
            allowed.add('pullup')
        if kind in ('analog_input', 'pwm_output'):
            allowed |= {'pin', 'scale', 'offset'}
        if kind.startswith('dht_'):
            allowed |= {'pin', 'sensor', 'invalid_policy'}
        if kind in ULTRASONIC_KINDS:
            allowed |= ULTRASONIC_FIELDS
        if kind == 'serial_value':
            allowed |= {'trigger', 'label', 'baud'}
        if kind == 'serial_text_output':
            allowed |= {'text', 'baud'}
        if kind in SERIAL_INPUTS:
            allowed |= CATALOG[kind][4]
        if kind == 'serial_valid':
            allowed.add('value')
        if kind in ('tone_output', 'servo_angle'):
            allowed |= {'pin', 'invalid_policy'}
        if kind == 'tone_output':
            allowed.add('channel_mode')
        if kind == 'report_trigger':
            allowed |= {'format', 'values'}
        if kind == 'tm1637_number':
            allowed |= {'clk_pin', 'dio_pin', 'brightness', 'leading_zero', 'invalid_policy'}
        if kind == 'shift595_number':
            allowed |= {'data_pin', 'clock_pin', 'latch_pin', 'registers', 'leading_zero', 'invalid_policy'}
        if binding.keys() - allowed:
            raise ValueError('Interface ' + p['name'] + '.binding has unknown fields ' +
                             ', '.join(sorted(binding.keys() - allowed)) + '; allowed fields: ' +
                             ', '.join(sorted(allowed)) + '. Type, unit, initial and sample_period_ms belong on the interface object, outside binding.')
        sample = p['sample_period_ms']
        if not isinstance(sample, int) or sample < period or sample > 2147483647 or sample % period:
            raise ValueError('Sampling periods must be integer multiples of the control period: ' + p['name'])
        if p['direction'] == 'output' or kind == 'elapsed_ms':
            if sample != period:
                raise ValueError('Outputs and elapsed time must be updated every control step: ' + p['name'])
        pin_fields = ('clk_pin', 'dio_pin') if kind == 'tm1637_number' else (('pin',) if 'pin' in allowed else ())
        if kind == 'shift595_number':
            pin_fields = ('data_pin', 'clock_pin', 'latch_pin')
        if kind in ULTRASONIC_KINDS:
            pin_fields = ('trigger_pin', 'echo_pin')
        if kind in extended.CATALOG:
            extended.validate_port(dict(p, control_period_ms=period), ports, board)
            pin_fields = extended.pin_fields(binding)
        for field in pin_fields:
            try:
                physical = str(physical_pin(binding.get(field), board))
            except ValueError as exc:
                raise ValueError('Interface ' + p['name'] + '.binding.' + field + '=' + repr(binding.get(field)) + ': ' + str(exc)) from exc
            owner = used_pins.get(physical)
            dht = kind.startswith('dht_')
            ultrasonic = ('ultrasonic', sensor_key(binding,board)) if kind in ULTRASONIC_KINDS else None
            shared = None
            if kind.startswith(('rtc_', 'mpu_')):
                shared = ('i2c', board)
            elif kind in extended.LCD:
                shared = ('spi', board) if field in ('mosi_pin', 'miso_pin', 'sck_pin') else ('lcd',
                    physical_pin(binding['cs_pin'],board),physical_pin(binding['dc_pin'],board),
                    -1 if binding['rst_pin']==-1 else physical_pin(binding['rst_pin'],board))
            if owner and not ((dht and owner == ('dht', binding.get('sensor'))) or (ultrasonic and owner==ultrasonic) or (shared and owner == shared)):
                raise ValueError('Conflicting hardware bindings on pin ' + physical + ': ' + p['name'] + '.binding.' + field + ' conflicts with ' + repr(owner) + '. Preserve explicit wiring; contradictory wiring must be reported, not silently changed.')
            used_pins[physical] = shared or ultrasonic or (('dht', binding.get('sensor')) if dht else (kind, p['name']))
        if kind == 'seven_segment_number':
            for value in extended.all_pins(binding):
                physical = str(physical_pin(value, board))
                if physical in used_pins:
                    raise ValueError('Conflicting display pin ' + physical)
                used_pins[physical] = (kind, p['name'])
        if kind.startswith('digital_') or kind == 'report_trigger':
            if p['type'] != 'bool' or p['unit'] != '1':
                raise ValueError(kind + ' requires bool with unit 1')
            for flag in ('active_high', 'pullup'):
                if flag in binding and not isinstance(binding[flag], bool):
                    raise ValueError(flag + ' must be Boolean')
        if kind == 'analog_input':
            if not str(binding['pin']).startswith('A') or p['type'] == 'bool':
                raise ValueError('analog_input requires an analog pin name for ' + board + ' and numeric type')
            if p['unit'] != 'adc_count' and not {'scale', 'offset'} <= binding.keys():
                raise ValueError('Analog physical units require explicit scale and offset')
        if kind in ('analog_input', 'pwm_output'):
            for field in ('scale', 'offset'):
                if field in binding:
                    number(binding[field])
        if kind == 'pwm_output':
            if pin(binding['pin'], board) not in {str(p) for p in profile['pwm']} or p['type'] == 'bool':
                raise ValueError('pwm_output requires a PWM capable pin for ' + board + ' and numeric type')
            if p['unit'] != 'pwm_count' and not {'scale', 'offset'} <= binding.keys():
                raise ValueError('PWM physical units require explicit scale and offset')
        if kind == 'servo_angle':
            if p['type'] not in ('i32', 'f32', 'f64') or p['unit'] != 'deg':
                raise ValueError('servo_angle requires numeric degrees (unit deg); mapping and motion timing belong in SysML')
            if not 0 <= p['initial'] <= 180:
                raise ValueError('servo_angle initial must be in 0..180 degrees')
            if binding.get('invalid_policy') != 'hold_last_valid':
                raise ValueError('servo_angle invalid_policy must be hold_last_valid')
            # Servo 1.3.0 AVR stores the physical pin in a six-bit field.
            if physical_pin(binding['pin'], board) > 63:
                raise ValueError('Servo AVR library only represents physical pins 0..63')
        if kind == 'tone_output':
            if binding.get('channel_mode', 'single') not in ('single', 'exclusive'):
                raise ValueError('tone_output.channel_mode must be single/exclusive')
            if p['type'] != 'i32' or p['unit'] != 'Hz':
                raise ValueError('tone_output requires i32 Hz: 0 stops, 31..65535 commands pitch')
            if p['initial'] != 0 and not 31 <= p['initial'] <= 65535:
                raise ValueError('tone_output initial must be 0 or in 31..65535 Hz')
            if binding.get('invalid_policy') != 'silence':
                raise ValueError('tone_output invalid_policy must be silence for out-of-range model commands')
        if kind == 'tm1637_number':
            if p['type'] != 'i32':
                raise ValueError('tm1637_number requires i32; mapping, rounding and display selection belong in SysML')
            brightness = binding.get('brightness')
            if isinstance(brightness, bool) or not isinstance(brightness, int) or not 0 <= brightness <= 7:
                raise ValueError('TM1637 brightness must be an integer in 0..7')
            if not isinstance(binding.get('leading_zero'), bool) or binding.get('invalid_policy') != 'blank':
                raise ValueError('TM1637 requires Boolean leading_zero and invalid_policy=blank')
            # An out-of-display-range i32 initial value deliberately requests
            # blank through the same explicit invalid_policy used at runtime.
        if kind == 'shift595_number':
            from app.services.shift595 import validate_display
            validate_display(p)
            for register in binding['registers']:
                if register['register'] in shift_devices or register['display'] in shift_displays:
                    raise ValueError('A 74HC595 register or display cannot belong to multiple bindings')
                shift_devices.add(register['register']); shift_displays.add(register['display'])
        if kind == 'elapsed_ms' and (p['type'] != 'i32' or p['unit'] != 'ms' or p['initial'] != 0):
            raise ValueError('elapsed_ms requires i32, unit ms and initial 0')
        if kind == 'report_trigger':
            report_triggers.add(p['name'])
            if 'format' in binding:
                values = binding.get('values')
                if not isinstance(values, list) or not values or any(v not in ports or ports[v]['binding'].get('kind') != 'serial_value' or ports[v]['binding'].get('trigger') != p['name'] for v in values):
                    raise ValueError('Formatted report values must reference the registered numeric outputs sharing this trigger')
                extended.format_parts(binding['format'], len(values))
        if kind == 'serial_value':
            trigger = binding.get('trigger')
            if not isinstance(trigger, str) or trigger not in ports or ports[trigger]['direction'] != 'output' or ports[trigger]['type'] != 'bool':
                eligible = sorted(name for name, port in ports.items() if port['direction'] == 'output' and port['type'] == 'bool')
                raise ValueError('Serial formatting requires a Boolean model output trigger; field ' +
                                 p['name'] + '.binding.trigger=' + repr(trigger) + '; declared Boolean outputs: ' +
                                 json.dumps(eligible) + '. Reference the output that actually controls reporting. '
                                 'If a reporting control is required but absent, declare a complete report_trigger output; '
                                 'its initial value and timing must follow the original requirements, not an adapter timer.')
            consumed_triggers.add(trigger)
        if kind == 'serial_text_output':
            text = binding.get('text')
            if p['type'] != 'bool' or p['unit'] != '1':
                raise ValueError('serial_text_output requires a Boolean model gate, unit 1')
            if not isinstance(text, str) or not text or '\x00' in text or len(text.encode('utf-8')) > 256:
                raise ValueError('serial_text_output.text must be nonempty UTF-8 text of at most 256 bytes without NUL')
        if kind in {'serial_value', 'serial_text_output', 'serial_echo'} | SERIAL_INPUTS:
            baud = binding.get('baud')
            if isinstance(baud, bool) or not isinstance(baud, int) or not 300 <= baud <= 2000000:
                raise ValueError('Serial baud must be explicit and in 300..2000000')
            if kind == 'serial_value' and (not isinstance(binding.get('label'), str) or len(binding['label']) > 120):
                raise ValueError('Serial label must be a string of at most 120 characters')
            bauds.add(baud)
    if len(bauds) > 1:
        raise ValueError('All Serial bindings must use the same baud rate')
    validate_receivers({name: dict(p, control_period_ms=period) for name, p in ports.items()})
    if bauds and ({'0', '1'} & used_pins.keys()):
        raise ValueError('Physical Serial reserves D0/D1; GPIO bindings conflict with the UART')
    if report_triggers - consumed_triggers:
        raise ValueError('Report trigger has no consuming serial output: ' + ', '.join(sorted(report_triggers - consumed_triggers)) +
                         '. report_trigger gates serial_value only; tm1637_number consumes its number directly without a trigger. '
                         'If serial reporting is not in the original requirements, remove the invented reporting ports; '
                         'otherwise bind the required serial_value to this trigger. Preserve all requested devices and behavior.')
    tones = [p for p in ports.values() if p['binding']['kind'] == 'tone_output']
    if len(tones) > 1 and not all(p['binding'].get('channel_mode') == 'exclusive' for p in tones):
        raise ValueError('Arduino AVR core supports one simultaneous tone channel; multiple tone outputs are unsupported')
    if tones:
        for p in ports.values():
            if p['binding']['kind'] == 'pwm_output' and physical_pin(p['binding']['pin'], board) in profile['tone_pwm_conflicts']:
                raise ValueError('tone_output uses Timer2 and conflicts with PWM pin ' + str(p['binding']['pin']) + ' on ' + board)
    servos = [p for p in ports.values() if p['binding']['kind'] == 'servo_angle']
    if len(servos) > 12 * len(profile['servo_timers']):
        raise ValueError('Servo channel capacity exceeded on ' + board)
    occupied = set().union(*profile['servo_timers'][:(len(servos) + 11) // 12])
    for p in ports.values():
        if p['binding']['kind'] == 'pwm_output' and physical_pin(p['binding']['pin'], board) in occupied:
            raise ValueError('Servo timer conflicts with PWM pin ' + str(p['binding']['pin']) + ' on ' + board)
    extended.validate_groups(ports)
    return contract


def _parameters(header):
    match = re.search(r'void\s+ProgramPage1\s*\(([^)]*)\)', header)
    if not match:
        raise ValueError('Compiled model step signature missing')
    result = []
    for argument in match[1].split(','):
        arg = re.fullmatch(r'\s*(bool|int|int32_t|float|double)\s*(\*)?\s*([A-Za-z_]\w*)\s*', argument)
        if not arg:
            raise ValueError('Unsupported model argument: ' + argument)
        result.append((arg[3], 'output' if arg[2] else 'input', {'int': 'i32', 'int32_t': 'i32', 'bool': 'bool', 'float': 'f32', 'double': 'f64'}[arg[1]]))
    return result


def generate_hardware_adapter(header, contract):
    """Emit fixed initialization, scheduling, sampling and output application."""
    validate_hardware_contract(contract)
    board = contract.get('board', 'arduino:avr:uno')
    profile = board_profile(board)
    board_pin = lambda value: pin(value, board)
    parameters = _parameters(header)
    ports = contract['interfaces']
    expected = {(p['name'], p['direction'], p['type']) for p in ports}
    if set(parameters) != expected or len(parameters) != len(expected):
        raise ValueError('Compiled model signature differs from the interface contract')
    declarations = ['// LLM4MDD target-board: ' + board,
                    '#include <Arduino.h>', '#include <math.h>',
                    '#if defined(ARDUINO) && !defined(' + profile['macro'] + ')',
                    '#error "Selected Arduino board differs from the interface contract"', '#endif',
                    'static uint32_t hw_last_step = 0;', 'static bool hw_started = false;']
    setup, sampling, receiving, writing = [], [], [], []
    has_display = False
    has_servo = False
    bauds = set()
    exclusive_tones = [p for p in ports if p['binding']['kind'] == 'tone_output' and p['binding'].get('channel_mode') == 'exclusive']
    for p in ports:
        name, kind = p['name'], p['binding']['kind']
        b = p['binding']
        local = 'hw_value_' + name
        declarations.append(f'static {CPP_TYPES[p["type"]]} {local} = {literal(p["initial"])};')
        if kind in ('digital_input', 'analog_input', 'digital_output', 'pwm_output', 'tone_output'):
            mode = ('INPUT_PULLUP' if b.get('pullup', False) else 'INPUT') if p['direction'] == 'input' else 'OUTPUT'
            setup.append(f'pinMode({board_pin(b["pin"])}, {mode});')
        if p['direction'] == 'input' and kind not in ({'elapsed_ms', 'serial_valid'} | SERIAL_INPUTS | ULTRASONIC_KINDS | DHT_KINDS | extended.SENSORS):
            declarations += [f'static uint32_t hw_sampled_{name} = 0;', f'static bool hw_attempted_{name} = false;']
            sample = [f'if (!hw_attempted_{name} || (uint32_t)(hw_now - hw_sampled_{name}) >= {p["sample_period_ms"]}UL) {{',
                      f'  hw_attempted_{name} = true; hw_sampled_{name} = hw_now;']
            if kind == 'digital_input':
                read = f'(digitalRead({board_pin(b["pin"])}) == {digital_levels(b)[0]})'
            elif kind == 'analog_input':
                read = f'(analogRead({board_pin(b["pin"])}) * {number(b.get("scale", 1.0))} + {number(b.get("offset", 0.0))})'
            sample.append(f'  {local} = {read};')
            sampling += sample + ['}']
        if kind in SERIAL_INPUTS:
            bauds.add(b['baud'])
            receiver, poll = receiver_code(p)
            declarations.append(receiver)
            receiving.append(poll)
        if kind == 'digital_output':
            high, low = digital_levels(b)
            statement = f'digitalWrite({board_pin(b["pin"])}, {local} ? {high} : {low});'
            setup.append(statement)
            writing.append(statement)
        if kind == 'pwm_output':
            value = f'({local} * {number(b.get("scale", 1.0))} + {number(b.get("offset", 0.0))})'
            statement = f'analogWrite({board_pin(b["pin"])}, (int)constrain({value}, 0.0, 255.0));'
            setup.append(statement)
            writing.append(statement)
        if kind == 'tone_output' and not (len(exclusive_tones) > 1):
            # Carrier generation is hardware work; gating/duration/repetition
            # are supplied by the model's frequency command, never a tone timer.
            previous = 'hw_tone_' + name
            command = 'hw_tone_command_' + name
            declarations.append(f'static int32_t {previous} = -1;')
            statements = [f'int32_t {command} = ({local} >= 31 && {local} <= 65535) ? {local} : 0;',
                f'if ({command} != {previous}) {{',
                f'  if ({command} == 0) noTone({board_pin(b["pin"])});',
                f'  else tone({board_pin(b["pin"])}, (unsigned int){command});',
                f'  {previous} = {command};', '}']
            setup.extend(statements)
            writing.extend(statements)
        if kind == 'servo_angle':
            has_servo = True
            servo, previous = 'hw_servo_' + name, 'hw_servo_previous_' + name
            declarations += [f'Servo {servo};', f'static int {previous} = -1;']
            setup.append(f'{servo}.attach({board_pin(b["pin"])});')
            # Reject invalid angles before Servo.write: values >=544 are
            # interpreted as microseconds by the library, not degrees.
            statements = [f'if (isfinite((double){local}) && {local} >= 0 && {local} <= 180) {{',
                f'  int command = (int){local};', f'  if (command != {previous}) {{',
                f'    {servo}.write(command); {previous} = command;', '  }', '}']
            setup.extend(statements); writing.extend(statements)
        if kind == 'serial_value':
            bauds.add(b['baud'])
            trigger = next(q for q in ports if q['name'] == b['trigger'])
            if 'format' not in trigger['binding']:
                writing.append(f'if (hw_value_{b["trigger"]}) {{ Serial.print({json.dumps(b["label"], ensure_ascii=False)}); Serial.println({local}); }}')
        if kind == 'serial_text_output':
            bauds.add(b['baud'])
            writing.append(f'if ({local}) Serial.println({json.dumps(b["text"], ensure_ascii=False)});')
        if kind == 'tm1637_number':
            has_display = True
            display, previous = 'hw_display_' + name, 'hw_display_previous_' + name
            declarations += [f'TM1637Display {display}({board_pin(b["clk_pin"])}, {board_pin(b["dio_pin"])});',
                             f'static int32_t {previous} = INT32_MIN;']
            setup.append(f'{display}.setBrightness({b["brightness"]}, true);')
            low = 0 if b['leading_zero'] else -999
            statements = [f'if ({local} != {previous}) {{',
                f'  if ({local} < {low} || {local} > 9999) {display}.clear();',
                f'  else {display}.showNumberDec((int){local}, {literal(b["leading_zero"])}, 4, 0);',
                f'  {previous} = {local};', '}']
            setup.extend(statements);writing.extend(statements)
        if kind == 'shift595_number':
            from app.services.shift595 import render_display
            helper, initialization, statements = render_display(p, board_pin)
            declarations.append(helper)
            setup.extend(initialization + statements)
            writing.extend(statements)
    device_declarations, device_setup, device_sampling, device_writing, device_bauds = extended.render(contract)
    declarations = device_declarations + declarations
    setup += device_setup; sampling += device_sampling; writing += device_writing; bauds |= device_bauds
    if len(exclusive_tones) > 1:
        declarations.append('static int hw_active_tone_pin=-1; static int32_t hw_active_tone_frequency=0;')
        block = ['{ int selected=-1; int32_t frequency=0;']
        for p in exclusive_tones:
            value = 'hw_value_' + p['name']
            block.append(f'if(selected<0 && {value}>=31 && {value}<=65535) {{selected={board_pin(p["binding"]["pin"])};frequency={value};}}')
        block += ['if(selected!=hw_active_tone_pin || frequency!=hw_active_tone_frequency) {',
                  'if(hw_active_tone_pin>=0) noTone(hw_active_tone_pin);',
                  'if(selected>=0) tone(selected,(unsigned int)frequency);',
                  'hw_active_tone_pin=selected;hw_active_tone_frequency=frequency;', '}', '}']
        writing += block
    if has_display:
        declarations.insert(1, '#include <TM1637Display.h>')
    if has_servo:
        declarations.insert(1, '#include <Servo.h>')
    if bauds:
        setup.insert(0, f'Serial.begin({next(iter(bauds))});')
    from app.services.ultrasonic import render_acquisition
    ultrasonic_declarations,ultrasonic_setup,ultrasonic_sampling=render_acquisition(contract)
    declarations.extend(ultrasonic_declarations);setup.extend(ultrasonic_setup);sampling.extend(ultrasonic_sampling)
    from app.services.dht import render_acquisition as render_dht
    dht_declarations,dht_setup,dht_sampling,guards=render_dht(contract)
    declarations.extend(dht_declarations);setup.extend(dht_setup);sampling.extend(dht_sampling)
    args = ', '.join(('&' if direction == 'output' else '') + 'hw_value_' + name for name, direction, _ in parameters)
    loop = ['uint32_t hw_now = (uint32_t)millis();',
            'uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;',
            f'if (hw_started && hw_elapsed < {contract["control_period_ms"]}UL) return;',
            'if (hw_elapsed > 2147483647UL) return;', *sampling, *guards,
            # All blocking sensor acquisition belongs in actual elapsed time.
            'hw_now = (uint32_t)millis();',
            'hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;',
            'if (hw_elapsed > 2147483647UL) return;',
            *['hw_value_'+p['name']+' = (int32_t)hw_elapsed;' for p in ports if p['binding']['kind']=='elapsed_ms'],
            *receiving,
            'hw_last_step = hw_now; hw_started = true;', f'ProgramPage1({args});', *writing]
    return '\n'.join(declarations) + '\n\nvoid setup() {\n  ProgramPage1_Init(&ProgramPage1_instance);\n  ' + '\n  '.join(setup) + '\n}\n\nvoid loop() {\n  ' + '\n  '.join(loop) + '\n}\n'
