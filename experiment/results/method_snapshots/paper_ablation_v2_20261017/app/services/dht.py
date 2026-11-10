"""Coherent DHT frames and explicit validity for independent mixed-device control."""

KINDS = {'dht_temperature', 'dht_humidity', 'dht_valid'}
FIELDS = {'kind', 'pin', 'sensor', 'invalid_policy'}


def groups(contract):
    from app.services.hardware_adapter import physical_pin
    result = {}
    mixed = any(p['direction'] == 'input' and p.get('binding', {}).get('kind') not in KINDS | {'elapsed_ms'}
                for p in contract['interfaces'] if isinstance(p.get('binding', {}), dict))
    for p in contract['interfaces']:
        b = p.get('binding', {})
        if not isinstance(b, dict) or b.get('kind') not in KINDS:
            continue
        if set(b) != FIELDS or p['direction'] != 'input' or b['sensor'] not in ('DHT11', 'DHT22'):
            raise ValueError('DHT inputs require exactly kind, pin, sensor (DHT11/DHT22), invalid_policy in binding')
        if b['invalid_policy'] != 'hold_last_valid':
            raise ValueError('DHT invalid_policy must be hold_last_valid; validity separately identifies failed acquisitions')
        kind = b['kind']
        if kind == 'dht_valid':
            if p['type'] != 'bool' or p['unit'] != '1' or p['initial'] is not False:
                raise ValueError('dht_valid requires bool, unit 1 and initial false')
        else:
            unit = 'degC' if kind == 'dht_temperature' else '%RH'
            if p['type'] not in ('f32', 'f64') or p['unit'] != unit:
                raise ValueError(kind + ' requires floating point ' + unit)
        sample = p['sample_period_ms']
        if type(sample) is not int or not 2000 <= sample <= 2147483647:
            raise ValueError(f'Interface {p["name"]}.sample_period_ms must be an integer >=2000 ms for the DHT library cache')
        key = (physical_pin(b['pin'], contract.get('board', 'arduino:avr:uno')), b['sensor'])
        group = result.setdefault(key, {})
        if kind in group:
            raise ValueError('Duplicate DHT channel on one physical sensor: ' + kind + ' at ' + p['name'] +
                             '; ' + group[kind]['name'] + ' already supplies this quantity. Declare ONE request per '
                             'quantity on this sensor; temperature and humidity share ONE dht_valid request. '
                             'Remove the duplicate request and update dependent io_i references after reindexing.')
        group[kind] = p
    for key, group in result.items():
        channels = [p for kind, p in group.items() if kind != 'dht_valid']
        if not channels:
            raise ValueError('dht_valid must accompany temperature and/or humidity on the same physical sensor')
        if len({p['sample_period_ms'] for p in group.values()}) != 1:
            raise ValueError('DHT channels and validity on one sensor must share sample_period_ms to represent one frame')
        if mixed and 'dht_valid' not in group:
            raise ValueError('Mixed-device control requires a dht_valid input for each DHT sensor. '
                             'Share its pin, sensor, sample period and hold_last_valid policy; use bool, unit 1, initial false. '
                             'The model must handle invalid data without stopping unrelated devices.')
    # Even without an Arduino export, a physical pin cannot mean two sensor types.
    if len({key[0] for key in result}) != len(result):
        raise ValueError('Conflicting DHT sensor types on one physical pin')
    return list(result.items())


def render_acquisition(contract):
    declarations, setup, sampling, guards = [], [], [], []
    sensors = groups(contract)
    if sensors:
        declarations.append('#include <DHT.h>')
    for i, ((pin, sensor_type), ports) in enumerate(sensors):
        sensor = f'hw_dht_{i}'
        channels = [p for kind, p in ports.items() if kind != 'dht_valid']
        validity = ports.get('dht_valid')
        period = channels[0]['sample_period_ms']
        declarations += [f'DHT {sensor}({pin}, {sensor_type});',
                         f'static bool {sensor}_attempted = false;', f'static uint32_t {sensor}_sampled = 0;']
        setup.append(sensor + '.begin();')
        if validity is None:
            declarations.append(f'static bool {sensor}_ready = false;')
            guards.append(f'if (!{sensor}_ready) return;')
        sampling += [f'uint32_t {sensor}_now = (uint32_t)millis();',
                     f'if (!{sensor}_attempted || (uint32_t)({sensor}_now - {sensor}_sampled) >= {period}UL) {{',
                     f'  {sensor}_attempted = true; {sensor}_sampled = {sensor}_now;',
                     f'  bool {sensor}_valid = {sensor}.read();']
        for p in channels:
            method = 'readTemperature' if p['binding']['kind'] == 'dht_temperature' else 'readHumidity'
            sampling.append(f'  double {sensor}_{p["name"]} = {sensor}_valid ? {sensor}.{method}() : NAN;')
        finite = ' && '.join(f'isfinite({sensor}_{p["name"]})' for p in channels)
        sampling += [f'  {sensor}_valid = {sensor}_valid && {finite};', f'  if ({sensor}_valid) {{']
        sampling += [f'    hw_value_{p["name"]} = {sensor}_{p["name"]};' for p in channels]
        if validity is None:
            sampling.append(f'    {sensor}_ready = true;')
        sampling.append('  }')
        if validity:
            sampling.append(f'  hw_value_{validity["name"]} = {sensor}_valid;')
        sampling.append('}')
    return declarations, setup, sampling, guards


def check_circuit(contract, circuit, physical):
    boards={p.get('id') for p in circuit['parts'] if isinstance(p,dict) and p.get('type') in ('wokwi-arduino-uno','wokwi-arduino-mega')}
    devices={p.get('id'):p['type'].removeprefix('wokwi-').upper() for p in circuit['parts']
             if isinstance(p,dict) and p.get('type') in ('wokwi-dht11','wokwi-dht22')}
    available={key for key,_ in groups(contract)}
    if len(devices)>len(available):
        raise ValueError('Explicit DHT circuit devices missing sensor input bindings')
    for connection in circuit['connections']:
        if not isinstance(connection,list) or len(connection)<2 or not all(isinstance(s,str) for s in connection[:2]):
            continue
        for left,right in (connection[:2],connection[:2][::-1]):
            device,_,signal=left.partition(':');board,_,pin=right.partition(':')
            if device in devices and signal in ('SDA','DATA') and board in boards:
                if (physical(pin),devices[device]) not in available:
                    raise ValueError('Explicit DHT data wiring or sensor type differs for '+device+' on '+right)
