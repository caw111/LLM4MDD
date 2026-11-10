"""Construct supported interface shapes and serial relationships before freezing."""
import copy
import json
import re


# Direction, scalar type and electrical unit are catalog facts, not LLM choices.
CATALOG = {
    'digital_input': ('input', 'bool', '1', {'active_high': True, 'pullup': False}, {'pin', 'active_high', 'pullup'}),
    'digital_output': ('output', 'bool', '1', {'active_high': True}, {'pin', 'active_high'}),
    'analog_input': ('input', 'f64', 'adc_count', {'scale': 1.0, 'offset': 0.0}, {'pin', 'scale', 'offset'}),
    'pwm_output': ('output', 'f64', 'pwm_count', {'scale': 1.0, 'offset': 0.0}, {'pin', 'scale', 'offset'}),
    'elapsed_ms': ('input', 'i32', 'ms', {'semantics': 'delta_since_previous_step'}, {'semantics'}),
    'dht_temperature': ('input', 'f64', 'degC', {'invalid_policy': 'hold_last_valid'}, {'pin', 'sensor', 'invalid_policy'}),
    'dht_humidity': ('input', 'f64', '%RH', {'invalid_policy': 'hold_last_valid'}, {'pin', 'sensor', 'invalid_policy'}),
    'dht_valid': ('input', 'bool', '1', {'invalid_policy': 'hold_last_valid'}, {'pin', 'sensor', 'invalid_policy'}),
    'ultrasonic_distance': ('input', 'f64', 'cm', {'sensor': 'HC-SR04', 'timeout_us': 25000, 'invalid_policy': 'hold_last_valid'},
                            {'trigger_pin', 'echo_pin', 'sensor', 'timeout_us', 'invalid_policy'}),
    'ultrasonic_valid': ('input', 'bool', '1', {'sensor': 'HC-SR04', 'timeout_us': 25000, 'invalid_policy': 'hold_last_valid'},
                         {'trigger_pin', 'echo_pin', 'sensor', 'timeout_us', 'invalid_policy'}),
    'shift595_number': ('output', 'i32', '1', {'leading_zero': True, 'invalid_policy': 'blank'},
                        {'data_pin', 'clock_pin', 'latch_pin', 'leading_zero', 'invalid_policy', 'registers'}),
    'servo_angle': ('output', 'f64', 'deg', {'invalid_policy': 'hold_last_valid'}, {'pin', 'invalid_policy'}),
    'tone_output': ('output', 'i32', 'Hz', {'invalid_policy': 'silence'}, {'pin', 'invalid_policy'}),
    'tm1637_number': ('output', 'i32', '1', {'brightness': 7, 'leading_zero': False, 'invalid_policy': 'blank'},
                      {'clk_pin', 'dio_pin', 'brightness', 'leading_zero', 'invalid_policy'}),
    'serial_command_input': ('input', 'i32', '1', {'framing': 'line_or_idle', 'idle_timeout_ms': 20},
                             {'baud', 'commands', 'framing', 'idle_timeout_ms'}),
    'serial_integer_input': ('input', 'i32', '1', {'framing': 'line_or_idle', 'idle_timeout_ms': 20},
                             {'baud', 'min', 'max', 'framing', 'idle_timeout_ms'}),
    'serial_text_output': ('output', 'bool', '1', {}, {'baud', 'text'}),
}

SERIAL_INPUTS = {'serial_command_input', 'serial_integer_input'}


class InterfaceCapabilityError(ValueError):
    code = 'HARDWARE_CAPABILITY'


def _fields(value, required, optional, path):
    if not isinstance(value, dict):
        raise ValueError(path + ' must be an object')
    missing = sorted(set(required) - value.keys())
    unknown = sorted(value.keys() - set(required) - set(optional))
    if missing or unknown:
        raise ValueError(path + ': missing fields ' + str(missing) + '; unknown fields ' + str(unknown) +
                         '; allowed fields ' + str(sorted(set(required) | set(optional))) +
                         '. Configuration keys belong directly on this object, not inside optional/binding.')


def normalize_interface_proposal(proposal, *, allow_default_sampling=False,
                                 repair_catalog_fields=False, allow_default_initial=False):
    """Normalize known equivalent spellings; retain conflicts and unknown fields."""
    result, actions = copy.deepcopy(proposal), []
    if not isinstance(result, dict) or not isinstance(result.get('interface_requests'), list):
        return result, actions
    for index, request in enumerate(result['interface_requests']):
        if not isinstance(request, dict) or not isinstance(request.get('kind'), str) or request['kind'] not in CATALOG:
            continue
        direction, typ, unit, defaults, keys = CATALOG[request['kind']]
        path = 'interface_requests[' + str(index) + ']'
        allowed = keys | {'device', 'sample_period_ms', 'required_inputs', 'unit', 'raw_unit', 'type', 'direction'}
        if isinstance(request.get('optional'), dict):
            nested = request['optional']
            _fields(nested, set(), allowed, path + '.optional')
            for key, value in nested.items():
                if key in request and (type(request[key]) is not type(value) or request[key] != value):
                    raise ValueError(path + '.' + key + ' conflicts with optional.' + key)
                request[key] = value
            del request['optional']
            actions.append(path + ': expanded optional configuration')
        if 'raw_unit' in request:
            if request['raw_unit'] != unit or ('unit' in request and request['unit'] != unit):
                raise ValueError(path + '.raw_unit conflicts with catalog/unit; use unit with explicit calibration')
            request['unit'] = request.pop('raw_unit')
            actions.append(path + ': raw_unit -> unit')
        for key, expected in [('type', typ), ('direction', direction)]:
            if key in request:
                if request[key] != expected:
                    raise ValueError(path + '.' + key + ' conflicts with catalog ' + expected)
                del request[key]
                actions.append(path + ': removed matching catalog ' + key)
        for key in ('active_high', 'pullup', 'leading_zero'):
            if key in keys and type(request.get(key)) is int and request[key] in (0, 1):
                request[key] = bool(request[key])
                actions.append(path + '.' + key + ': integer Boolean -> Boolean')
        if repair_catalog_fields and request['kind'] in ('analog_input', 'pwm_output'):
            for key, neutral in (('active_high', True), ('pullup', False)):
                if key in request and type(request[key]) is bool and request[key] is neutral:
                    del request[key]
                    actions.append(path + '.' + key + ': removed neutral digital-only option from numeric interface')
        if repair_catalog_fields and request['kind'] in ('ultrasonic_distance', 'ultrasonic_valid') and 'pin' in request and 'trigger_pin' in request:
            # A generic pin may repeat the explicit trigger; never guess its role
            # when the trigger is absent or silently discard a contradictory pin.
            values = [request['pin'], request['trigger_pin']]
            matches = [re.fullmatch(r'D?(0|[1-9][0-9]*)', str(value))
                       if type(value) in (int, str) else None for value in values]
            same_digital = all(matches) and int(matches[0][1]) == int(matches[1][1])
            same_analog = all(type(value) is str for value in values) and values[0] == values[1] and re.fullmatch(r'A(0|[1-9][0-9]*)', values[0])
            if not (same_digital or same_analog):
                raise ValueError(path + '.pin conflicts with trigger_pin or is not a valid equivalent pin spelling')
            del request['pin']
            actions.append(path + '.pin: removed duplicate of explicit trigger_pin')
        if request['kind'] == 'shift595_number' and isinstance(request.get('registers'), list):
            for register in request['registers']:
                if not isinstance(register, dict) or not isinstance(register.get('segment_bits'), list):
                    continue
                for i, bit in enumerate(register['segment_bits']):
                    if isinstance(bit, str) and re.fullmatch(r'(?:' + re.escape(str(register.get('register', ''))) + r':)?Q[0-7]', bit):
                        register['segment_bits'][i] = int(bit[-1])
                        actions.append(path + '.registers.segment_bits: terminal spelling -> bit index')
        if repair_catalog_fields and typ == 'bool' and request.get('unit', unit) != unit:
            request['unit'] = unit
            actions.append(path + '.unit: restored Boolean catalog unit 1 (no numeric conversion)')
        if allow_default_initial:
            initial = request.get('initial')
            replacement = initial
            if initial is None:
                replacement = False if typ == 'bool' else 0
                if request['kind'] == 'serial_integer_input':
                    replacement = max(request.get('min', -2147483648), min(request.get('max', 2147483647), 0))
            elif typ == 'bool' and type(initial) is int and initial in (0, 1):
                replacement = bool(initial)
            elif typ != 'bool' and type(initial) is bool:
                replacement = int(initial)
            if 'initial' not in request or type(replacement) is not type(initial) or replacement != initial:
                request['initial'] = replacement
                actions.append(path + '.initial: typed startup default/Boolean-number normalization -> ' + repr(replacement))
        if allow_default_sampling and type(request.get('sample_period_ms')) is int and request['sample_period_ms'] == 0:
            del request['sample_period_ms']
            actions.append(path + ': zero sampling -> catalog default (source specifies no sampling period)')
    errors = []
    for index, request in enumerate(result['interface_requests']):
        if isinstance(request, dict) and isinstance(request.get('kind'), str) and request['kind'] in CATALOG:
            try:
                _fields(request, {'kind', 'initial', 'meaning'},
                        CATALOG[request['kind']][4] | {'device', 'sample_period_ms', 'required_inputs', 'unit'},
                        'interface_requests[' + str(index) + ']')
            except ValueError as exc:
                errors.append(str(exc))
    if errors:
        raise ValueError('\n'.join(errors))
    return result, actions


def _port(name, direction, typ, unit, initial, meaning, period, binding, device=None):
    return dict(name=name, direction=direction, type=typ, unit=unit, initial=initial, meaning=meaning,
                sample_period_ms=period, device=device or binding['kind'],
                hardware=json.dumps(binding, ensure_ascii=False, sort_keys=True), binding=binding)


def derive_registry(contract):
    ports = {p['name']: dict(direction=p['direction'], type=p['type']) for p in contract['interfaces']}
    groups = {}
    for p in contract['interfaces']:
        if p.get('binding', {}).get('kind') == 'serial_value':
            trigger = p['binding'].get('trigger')
            if not isinstance(trigger, str):
                raise ValueError(p['name'] + '.binding.trigger must reference a registered Boolean output')
            groups.setdefault(trigger, []).append(p['name'])
    channels = [dict(id='serial_' + str(i), trigger_port=trigger, value_ports=values)
                for i, (trigger, values) in enumerate(groups.items())]
    registry = dict(version=1, ports=ports, serial_channels=channels)
    receivers = [dict(id=p['name'], value_port=p['name'], valid_port=p['name'] + '_valid',
                      **p['binding']) for p in contract['interfaces'] if p.get('binding', {}).get('kind') in SERIAL_INPUTS]
    if receivers:
        registry['serial_inputs'] = receivers
    texts = [dict(id=p['name'], trigger_port=p['name'], **p['binding']) for p in contract['interfaces']
             if p.get('binding', {}).get('kind') == 'serial_text_output']
    if texts:
        registry['serial_text_outputs'] = texts
    return registry


def validate_registry(contract):
    if 'interface_registry' in contract and contract['interface_registry'] != derive_registry(contract):
        raise ValueError('interface_registry differs from the declared ports or serial bindings; prepare a new contract before freezing')


def register_contract(contract):
    result = copy.deepcopy(contract)
    validate_registry(result)
    registry = derive_registry(result)
    for channel in registry['serial_channels']:
        port = registry['ports'].get(channel['trigger_port'])
        if port != {'direction': 'output', 'type': 'bool'}:
            raise ValueError('Serial channel ' + channel['id'] + ' requires a registered Boolean output trigger')
    for channel in registry.get('serial_inputs', []):
        valid = next((p for p in result['interfaces'] if p['name'] == channel['valid_port']), None)
        if (registry['ports'].get(channel['value_port']) != {'direction': 'input', 'type': 'i32'} or
                valid is None or valid['direction'] != 'input' or valid['type'] != 'bool' or
                valid.get('binding') != {'kind': 'serial_valid', 'value': channel['value_port']}):
            raise ValueError('Serial receiver requires a registered i32 value and matching Boolean input validity')
    for channel in registry.get('serial_text_outputs', []):
        if registry['ports'].get(channel['trigger_port']) != {'direction': 'output', 'type': 'bool'}:
            raise ValueError('Fixed serial text requires a registered Boolean output gate')
    result['interface_registry'] = registry
    return result


def assemble_interface_contract(proposal, *, board=None, require_clock=False, exclude_clock=False):
    """New proposals describe devices/groups; legacy names stay stable for compatibility.

    Initial values, physical wiring and business dependencies are never guessed to
    suppress an error. Registration does not make an unsupported device executable.
    """
    if not isinstance(proposal, dict):
        raise ValueError('Interface proposal must be a JSON object')
    result, actions = normalize_interface_proposal(proposal)
    constructed = 'interface_requests' in result or 'serial_channels' in result
    if constructed:
        if 'interfaces' in result or 'interface_registry' in result:
            raise ValueError('Use device requests and serial channels OR a legacy interfaces list, not both')
        requests = result.pop('interface_requests', [])
        channels = result.pop('serial_channels', [])
        if not isinstance(requests, list) or not isinstance(channels, list) or len(requests) + len(channels) > 128:
            raise ValueError('interface_requests and serial_channels must be bounded arrays')
        period = result.get('control_period_ms', 10)
        if type(period) is not int or not 1 <= period <= 2147483647:
            raise ValueError('control_period_ms must be an integer in 1..2147483647')
        result.setdefault('version', 1)
        result.setdefault('board', board or 'arduino:avr:uno')
        result['control_period_ms'] = period
        result['interfaces'] = []
        dependencies = result.setdefault('required_dependencies', {})
        if not isinstance(dependencies, dict):
            raise ValueError('required_dependencies must be an object')
        for index, request in enumerate(requests):
            path = 'interface_requests[' + str(index) + ']'
            if not isinstance(request, dict) or not isinstance(request.get('kind'), str):
                raise ValueError(path + '.kind must select a supported operation')
            kind = request['kind']
            name = 'io_' + str(index)
            if exclude_clock and kind == 'elapsed_ms':
                references = list(dependencies.values()) + [r.get('required_inputs', []) for r in requests if isinstance(r, dict)]
                references += [item.get('required_inputs', []) for c in channels if isinstance(c, dict)
                               for item in [c.get('trigger', {}), *c.get('values', [])] if isinstance(item, dict)]
                if any(isinstance(refs, list) and name in refs for refs in references):
                    raise ValueError(name + ': deadline-only clock has explicit dependencies; repair those obligations before freezing')
                actions.append(name + ': omitted unused clock for response-deadline-only requirement; other port IDs retained')
                continue
            if kind == 'led_bar_graph':
                _fields(request, {'kind', 'pins', 'initial', 'meaning'}, {'active_high', 'device'}, path)
                pins = request['pins']
                if not isinstance(pins, list) or not 1 <= len(pins) <= 128:
                    raise ValueError(path + '.pins must list each wired segment in physical order')
                initials = request['initial']
                if type(initials) is bool:
                    initials = [initials] * len(pins)
                if not isinstance(initials, list) or len(initials) != len(pins) or any(type(v) is not bool for v in initials):
                    raise ValueError(path + '.initial must be Boolean or one Boolean per segment')
                for segment, (pin, initial) in enumerate(zip(pins, initials)):
                    binding = dict(kind='digital_output', pin=pin, active_high=request.get('active_high', True))
                    result['interfaces'].append(_port(name + '_segment_' + str(segment), 'output', 'bool', '1',
                        initial, request['meaning'], period, binding, 'LED bar graph segment'))
                continue
            if kind not in CATALOG:
                raise InterfaceCapabilityError('Unsupported hardware operation ' + kind + ' at ' + path + '; no driver substitution is allowed')
            direction, typ, unit, defaults, binding_keys = CATALOG[kind]
            _fields(request, {'kind', 'initial', 'meaning'}, binding_keys | {'device', 'sample_period_ms', 'required_inputs', 'unit'}, path)
            binding = dict(kind=kind, **defaults)
            binding.update({key: request[key] for key in binding_keys if key in request})
            sample = period
            if direction == 'input' and kind != 'elapsed_ms':
                minimum = 2000 if kind.startswith('dht_') else period
                if kind.startswith('ultrasonic_'):
                    timeout = binding['timeout_us']
                    if type(timeout) is not int or not 25000 <= timeout <= 60000:
                        raise ValueError(path + '.timeout_us must be an integer in 25000..60000')
                    minimum = max(60, (timeout + 12 + 999) // 1000)
                sample = request.get('sample_period_ms', ((minimum + period - 1) // period) * period)
            elif 'sample_period_ms' in request and request['sample_period_ms'] != period:
                raise ValueError(path + '.sample_period_ms must equal control_period_ms for outputs and elapsed time')
            if 'unit' in request:
                if kind not in ('analog_input', 'pwm_output', 'tm1637_number', 'shift595_number') and request['unit'] != unit:
                    raise ValueError(path + '.unit conflicts with the catalog unit ' + unit)
                if kind in ('analog_input', 'pwm_output') and request['unit'] != unit and not {'scale', 'offset'} <= request.keys():
                    raise ValueError(path + '.unit needs explicit scale and offset calibration')
                unit = request['unit']
            result['interfaces'].append(_port(name, direction, typ, unit, request['initial'], request['meaning'],
                sample, binding, request.get('device')))
            if kind in SERIAL_INPUTS:
                result['interfaces'].append(_port(name + '_valid', 'input', 'bool', '1', False,
                    'True for this model step only when a complete valid serial frame was received; '
                    'value holds its previous value otherwise', period, dict(kind='serial_valid', value=name), 'Serial receive validity'))
                if 'framing' not in request or 'idle_timeout_ms' not in request:
                    note = name + ': receive framing=' + binding['framing'] + ', idle_timeout_ms=' + str(binding['idle_timeout_ms']) + '; catalog defaults used where unspecified'
                    actions.append(note)
                    notes = result.setdefault('planning_notes', [])
                    if not isinstance(notes, list):
                        raise ValueError('planning_notes must be an array')
                    notes.append(note)
            if 'required_inputs' in request:
                if direction != 'output' or name in dependencies:
                    raise ValueError(path + '.required_inputs must define one output dependency without duplicates')
                dependencies[name] = request['required_inputs']
        for index, channel in enumerate(channels):
            path = 'serial_channels[' + str(index) + ']'
            _fields(channel, {'baud', 'trigger', 'values'}, set(), path)
            trigger = channel['trigger']
            _fields(trigger, {'initial', 'meaning'}, {'required_inputs'}, path + '.trigger')
            trigger_name = 'serial_' + str(index) + '_trigger'
            result['interfaces'].append(_port(trigger_name, 'output', 'bool', '1', trigger['initial'],
                trigger['meaning'], period, {'kind': 'report_trigger'}, 'Serial report control'))
            if 'required_inputs' in trigger:
                if trigger_name in dependencies:
                    raise ValueError(path + '.trigger dependency is declared twice')
                dependencies[trigger_name] = trigger['required_inputs']
            if not isinstance(channel['values'], list) or not 1 <= len(channel['values']) <= 128:
                raise ValueError(path + '.values must contain 1..128 reported values')
            for value_index, value in enumerate(channel['values']):
                _fields(value, {'type', 'unit', 'initial', 'meaning', 'label'}, {'required_inputs'}, path + '.values')
                if value['type'] not in ('bool', 'i32', 'f64'):
                    raise ValueError(path + '.values.type must select bool/i32/f64; arbitrary text needs a separate driver')
                name = 'serial_' + str(index) + '_value_' + str(value_index)
                binding = dict(kind='serial_value', trigger=trigger_name, baud=channel['baud'], label=value['label'])
                result['interfaces'].append(_port(name, 'output', value['type'], value['unit'], value['initial'],
                    value['meaning'], period, binding, 'Serial value'))
                if 'required_inputs' in value:
                    if name in dependencies:
                        raise ValueError(path + '.values dependency is declared twice')
                    dependencies[name] = value['required_inputs']
        actions.append('catalog_interfaces_and_serial_channels_assembled')
        if len(result['interfaces']) > 128:
            raise ValueError('Constructed interfaces exceed the executable 128-port budget')
    else:
        ports = result.get('interfaces')
        if isinstance(ports, list) and all(isinstance(p, dict) for p in ports):
            triggers = [p for p in ports if isinstance(p.get('binding'), dict) and p['binding'].get('kind') == 'report_trigger'
                        and p.get('direction') == 'output' and p.get('type') == 'bool' and isinstance(p.get('name'), str)]
            names = {p.get('name') for p in ports if isinstance(p.get('name'), str)}
            # Only migrate the known template alias, never arbitrary/multi-channel references.
            if 'interface_registry' not in result and len(triggers) == 1 and 'reportDue' not in names:
                for p in ports:
                    binding = p.get('binding')
                    if isinstance(binding, dict) and binding.get('kind') == 'serial_value' and binding.get('trigger') == 'reportDue':
                        binding['trigger'] = triggers[0]['name']
                        actions.append('legacy_reportDue_alias_bound_to_unique_registered_trigger')
    if require_clock and isinstance(result.get('interfaces'), list) and not any(
            isinstance(p, dict) and isinstance(p.get('binding'), dict) and p['binding'].get('kind') == 'elapsed_ms'
            for p in result['interfaces']):
        period = result.get('control_period_ms')
        if type(period) is not int or not 1 <= period <= 2147483647:
            raise ValueError('control_period_ms must be an integer in 1..2147483647 before registering elapsed time')
        names = {p.get('name') for p in result['interfaces'] if isinstance(p, dict) and isinstance(p.get('name'), str)}
        name = 'elapsedMilliseconds'
        while name in names:
            name += '_delta'
        result['interfaces'].append(_port(name, 'input', 'i32', 'ms', 0,
            'Actual elapsed time since previous model step; first delta is zero', period,
            dict(kind='elapsed_ms', semantics='delta_since_previous_step'), 'clock'))
        actions.append('required_elapsed_input_registered')
    return result, actions


def registry_prompt(contract):
    registry = contract.get('interface_registry')
    if registry is None:
        return ''
    validate_registry(contract)
    return ('\nApplication-owned interface registry (authoritative names and serial relationships):\n' +
            json.dumps(registry, ensure_ascii=False) +
            '\nUse only these registered root inputs and outputs; never invent or rename serial ports. '
            'For each serial channel compute its trigger Boolean and its value expressions in controller.outputs. '
            'For each serial input inspect its valid_port before applying a new command/value. Command codes are '
            'transport tokens, not actuator states; interpret them in the model. Inputs retain their last valid value, '
            'while validity is a one-step event. Business report timing stays in the model. '
            'Each serial_text_outputs trigger_port is a Boolean gate: true prints its fixed text exactly once '
            'for that model step, with a line ending. Generate report timing in SysML; never append a dummy numeric value. '
            'Bindings and trigger references are assembled by the application.\n')
