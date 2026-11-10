"""Numeric display formatting for statically wired, cascaded 74HC595 registers.

No counters, input handling, thresholds, or application timing belong here.
Registers are listed nearest the MCU first; digit 0 is the leftmost digit.
"""
import re
import json


def validate_display(port):
    binding = port['binding']
    registers = binding.get('registers')
    if not isinstance(registers, list) or not 1 <= len(registers) <= 4:
        raise ValueError('shift595_number requires 1..4 static single-digit registers; multiplexing is unsupported')
    if port['type'] != 'i32':
        raise ValueError('Interface ' + port['name'] + ': shift595_number requires i32, got ' +
                         str(port['type']) + '; rounding, counting and selection belong in SysML')
    # A display formats a quantity; it does not prescribe its physical dimension.
    # validate_contract requires an explicit unit, which is preserved without scaling.
    if binding.get('invalid_policy') != 'blank' or not isinstance(binding.get('leading_zero'), bool):
        raise ValueError('shift595_number requires invalid_policy=blank and Boolean leading_zero')
    digits, devices, displays = set(), set(), set()
    for register in registers:
        if not isinstance(register, dict) or set(register) != {'register', 'display', 'digit', 'segment_bits', 'active_low'}:
            raise ValueError('Each register needs register, display, digit, segment_bits and active_low')
        for field in ('register', 'display'):
            if not isinstance(register[field], str) or not re.fullmatch(r'[A-Za-z_]\w{0,63}', register[field]):
                raise ValueError('Register/display IDs must be explicit identifiers')
        digit, bits = register['digit'], register['segment_bits']
        if type(digit) is not int or not 0 <= digit < len(registers) or digit in digits:
            raise ValueError('Register digit positions must be a permutation of 0..digits-1, leftmost first')
        if (not isinstance(bits, list) or len(bits) != 7 or
                any(type(bit) is not int or not 0 <= bit <= 7 for bit in bits) or len(set(bits)) != 7):
            raise ValueError('segment_bits must map A..G to seven distinct Q0..Q7 bits')
        if not isinstance(register['active_low'], bool):
            raise ValueError('Each register requires explicit active_low polarity')
        if register['register'] in devices or register['display'] in displays:
            raise ValueError('A register or display cannot be repeated in one chain')
        devices.add(register['register']); displays.add(register['display']); digits.add(digit)
    if not 0 <= port['initial'] < 10**len(registers):
        raise ValueError('shift595_number initial must fit its unsigned decimal digits')


def render_display(port, board_pin):
    """Return helper declarations, setup statements, and output statements."""
    name, binding = port['name'], port['binding']
    registers = binding['registers']
    data, clock, latch = [board_pin(binding[k]) for k in ('data_pin', 'clock_pin', 'latch_pin')]
    helper = 'hw_shift595_' + name
    # Format a number into segment patterns; reverse chain order because the
    # earliest byte propagates into the register farthest from the MCU.
    lines = [f'static void {helper}(int32_t value) {{',
             '  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};',
             f'  bool valid = value >= 0 && value < {10**len(registers)}L;',
             f'  digitalWrite({latch}, LOW);']
    for index, register in reversed(list(enumerate(registers))):
        divisor = 10**(len(registers)-1-register['digit'])
        visible = 'true' if binding['leading_zero'] or divisor == 1 else f'value >= {divisor}L'
        lines += ['  {', f'    uint8_t segments = valid && ({visible}) ? digits[(value / {divisor}L) % 10] : 0;',
                  '    uint8_t bits = 0;']
        for segment, bit in enumerate(register['segment_bits']):
            lines.append(f'    if (segments & {1<<segment}) bits |= {1<<bit};')
        if register['active_low']:
            lines.append('    bits = (uint8_t)~bits;')
        lines += [f'    shiftOut({data}, {clock}, MSBFIRST, bits);', '  }']
    lines += [f'  digitalWrite({latch}, HIGH);', '}']
    previous = 'hw_shift595_previous_' + name
    lines.append(f'static int32_t {previous} = INT32_MIN;')
    setup = [f'pinMode({pin}, OUTPUT); digitalWrite({pin}, LOW);' for pin in (data, clock, latch)]
    write = [f'if (hw_value_{name} != {previous}) {{',
             f'  {helper}(hw_value_{name}); {previous} = hw_value_{name};', '}']
    return '\n'.join(lines), setup, write


def check_circuit(contract, circuit, physical_pin):
    """Check actual signal nets, cascade order, polarity and static enable ties.

    This validates declared wiring, not the task's arithmetic or digit meanings.
    Unsupported wiring remains an explicit planning failure.
    """
    parts = {p['id']: p for p in circuit['parts'] if isinstance(p, dict) and 'id' in p}
    devices = {name for name, p in parts.items() if p.get('type') == 'wokwi-74hc595'}
    if not devices:
        return
    bindings = [p['binding'] for p in contract['interfaces'] if p['binding']['kind'] == 'shift595_number']
    declared = [r['register'] for b in bindings for r in b['registers']]
    if set(declared) != devices or len(declared) != len(set(declared)):
        raise ValueError('Explicit 74HC595 wiring requires every register exactly once in shift595_number bindings')
    boards = {name for name, p in parts.items() if p.get('type') in ('wokwi-arduino-mega', 'wokwi-arduino-uno')}
    parent = {}
    def find(item):
        parent.setdefault(item, item)
        if parent[item] != item: parent[item] = find(parent[item])
        return parent[item]
    for connection in circuit['connections']:
        if isinstance(connection, list) and len(connection) >= 2 and all(isinstance(v, str) for v in connection[:2]):
            parent[find(connection[0])] = find(connection[1])
    def connected(a, b):
        return find(a) == find(b)
    def rail(endpoint, high):
        return any(connected(endpoint, candidate) for candidate in list(parent)
                   if candidate.partition(':')[0] in boards and
                   (candidate.partition(':')[2] == '5V' if high else candidate.partition(':')[2].startswith('GND')))
    def signal(endpoint, pin):
        targets = []
        for candidate in list(parent):
            board, _, name = candidate.partition(':')
            if board not in boards: continue
            try:
                if physical_pin(name) == physical_pin(pin): targets.append(candidate)
            except ValueError: pass
        return any(connected(endpoint, target) for target in targets)
    def require(condition, reason):
        if not condition: raise ValueError('74HC595 wiring mismatch: ' + reason)
    for binding in bindings:
        previous = None
        for register in binding['registers']:
            chip, display = register['register'], register['display']
            require(parts.get(display, {}).get('type') == 'wokwi-7segment', 'single-digit display ' + display)
            attrs = parts[display].get('attrs', {})
            require(str(attrs.get('digits', '1')) == '1', 'multiplexed display is unsupported')
            common = attrs.get('common', 'anode')
            require(common in ('anode', 'cathode') and register['active_low'] == (common == 'anode'), 'display polarity ' + display)
            require(signal(chip+':SHCP', binding['clock_pin']), 'SHCP ' + chip)
            require(signal(chip+':STCP', binding['latch_pin']), 'STCP ' + chip)
            require(connected(chip+':DS', previous+':Q7S') if previous else signal(chip+':DS', binding['data_pin']), 'cascade DS ' + chip)
            for pin, high in (('VCC', True), ('MR', True), ('GND', False), ('OE', False)):
                require(rail(chip+':'+pin, high), 'static power/enable ' + chip+':'+pin)
            require(any(rail(display+':'+pin, common == 'anode') for pin in ('COM', 'COM.1', 'COM.2')), 'display common rail ' + display)
            for segment, bit in zip('ABCDEFG', register['segment_bits']):
                require(connected(chip+':Q'+str(bit), display+':'+segment), 'segment ' + display+':'+segment)
            previous = chip


def wiring_evidence(requirement, physical_pin):
    """Extract only verified electrical facts, leaving digit meaning to the model.

    A conservative direct-wire subset: unsupported/ambiguous diagrams yield no
    fact proposal. No contract, state, output name or digit order is invented.
    """
    evidence = []
    decoder = json.JSONDecoder()
    for start in re.finditer(r'\{', requirement):
        try:
            circuit, _ = decoder.raw_decode(requirement[start.start():])
            if not isinstance(circuit, dict) or not isinstance(circuit.get('parts'), list) or not isinstance(circuit.get('connections'), list):
                continue
            parts = {p['id']:p for p in circuit['parts']}
            chips = {name for name,p in parts.items() if p['type']=='wokwi-74hc595'}
            if not chips: continue
            boards = {name for name,p in parts.items() if p['type'] in ('wokwi-arduino-mega','wokwi-arduino-uno')}
            edges = {}
            for a,b,*_ in circuit['connections']:
                edges.setdefault(a,[]).append(b);edges.setdefault(b,[]).append(a)
            def direct_pin(endpoint):
                values=[v.partition(':')[2] for v in edges.get(endpoint,[]) if v.partition(':')[0] in boards]
                if len(values)!=1: raise ValueError('ambiguous MCU signal')
                return values[0]
            roots=[name for name in chips if any(v.partition(':')[0] in boards for v in edges.get(name+':DS',[]))]
            if len(roots)!=1: continue
            first=roots[0];chain=[];current=first
            while current not in chain:
                chain.append(current)
                nexts=[v.partition(':')[0] for v in edges.get(current+':Q7S',[]) if v.partition(':')[0] in chips and v.endswith(':DS')]
                if not nexts: break
                if len(nexts)!=1: raise ValueError('branched cascade')
                current=nexts[0]
            registers=[]
            for index,chip in enumerate(chain):
                displays={v.partition(':')[0] for bit in range(8) for v in edges.get(chip+':Q'+str(bit),[])
                          if parts.get(v.partition(':')[0],{}).get('type')=='wokwi-7segment'}
                if len(displays)!=1: raise ValueError('ambiguous display')
                display=next(iter(displays));bits=[]
                for segment in 'ABCDEFG':
                    found=[bit for bit in range(8) if display+':'+segment in edges.get(chip+':Q'+str(bit),[])]
                    if len(found)!=1: raise ValueError('ambiguous segment')
                    bits.append(found[0])
                registers.append(dict(register=chip,display=display,digit=index,segment_bits=bits,
                    active_low=parts[display].get('attrs',{}).get('common','anode')=='anode'))
            binding=dict(kind='shift595_number',data_pin=direct_pin(first+':DS'),clock_pin=direct_pin(first+':SHCP'),
                latch_pin=direct_pin(first+':STCP'),registers=registers,leading_zero=True,invalid_policy='blank')
            port=dict(type='i32',unit='1',initial=0,binding=binding)
            validate_display(port);check_circuit({'interfaces':[port]},circuit,physical_pin)
            # Probe-only digit indices and numeric formatting are not electrical
            # facts. Do not promote them into the planned contract or prompt.
            fact={k:binding[k] for k in ('kind','data_pin','clock_pin','latch_pin')}
            fact['registers_nearest_mcu_first']=[{k:v for k,v in r.items() if k!='digit'} for r in registers]
            evidence.append(fact)
        except (ValueError,TypeError,KeyError,IndexError):
            continue
    return evidence
