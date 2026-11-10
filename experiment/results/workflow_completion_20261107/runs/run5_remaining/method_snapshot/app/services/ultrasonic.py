"""HC-SR04 acquisition contract; thresholds and invalid-data actions stay in SysML."""

KINDS={'ultrasonic_distance','ultrasonic_valid'}
FIELDS={'kind','trigger_pin','echo_pin','sensor','timeout_us','invalid_policy'}


def sensor_key(binding,board):
    from app.services.hardware_adapter import physical_pin
    return (physical_pin(binding.get('trigger_pin'),board),physical_pin(binding.get('echo_pin'),board))


def groups(contract):
    board=contract.get('board','arduino:avr:uno');found={}
    for port in contract['interfaces']:
        binding=port.get('binding',{})
        if not isinstance(binding,dict) or binding.get('kind') not in KINDS:continue
        if set(binding)!=FIELDS:
            raise ValueError('HC-SR04 binding requires exactly '+', '.join(sorted(FIELDS)))
        if port['direction']!='input' or binding['sensor']!='HC-SR04':
            raise ValueError('HC-SR04 bindings must be inputs with sensor="HC-SR04"')
        key=sensor_key(binding,board)
        if key[0]==key[1]:raise ValueError('HC-SR04 trigger_pin and echo_pin must be distinct')
        timeout=binding['timeout_us'];sample=port['sample_period_ms']
        if type(timeout) is not int or not 25000<=timeout<=60000:
            raise ValueError('HC-SR04 timeout_us must be an integer in 25000..60000 microseconds for the supported 2..400 cm range')
        minimum=max(60,(timeout+12+999)//1000)
        if type(sample) is not int or sample<minimum:
            raise ValueError(f'Interface {port["name"]}.sample_period_ms: got {sample!r}; HC-SR04 requires an integer >= {minimum} ms '
                             'to cover its bounded acquisition and minimum sensor interval. Set BOTH paired distance/valid inputs '
                             'to the same legal sampling period; for example 100 ms if allowed by the original response requirement. '
                             'Sensor sampling is separate from control_period_ms; preserve the required control period and deadlines.')
        if binding['invalid_policy']!='hold_last_valid':
            raise ValueError('HC-SR04 requires hold_last_valid distance plus a separate false validity input on timeout/out-of-range')
        kind=binding['kind']
        if kind=='ultrasonic_distance':
            if port['type'] not in ('f32','f64') or port['unit']!='cm' or port['initial']!=0:
                raise ValueError('ultrasonic_distance requires floating point cm, initial 0 (invalid until first valid acquisition)')
        elif port['type']!='bool' or port['unit']!='1' or port['initial'] is not False:
            raise ValueError('ultrasonic_valid requires bool, unit 1 and initial false')
        group=found.setdefault(key,{})
        if kind in group:raise ValueError('Duplicate HC-SR04 '+kind+' for one physical sensor')
        group[kind]=port
    result=[]
    for key,group in found.items():
        if set(group)!=KINDS:
            raise ValueError('Each HC-SR04 requires one ultrasonic_distance and one ultrasonic_valid on the SAME trigger/echo pins')
        distance,valid=group['ultrasonic_distance'],group['ultrasonic_valid']
        if (distance['sample_period_ms']!=valid['sample_period_ms'] or
                distance['binding']['timeout_us']!=valid['binding']['timeout_us']):
            raise ValueError('Paired HC-SR04 distance/valid inputs must share sample_period_ms and timeout_us')
        result.append((key,distance,valid))
    return result


def render_acquisition(contract):
    declarations=[];setup=[];sampling=[]
    for index,((trigger,echo),distance,valid) in enumerate(groups(contract)):
        sampled=f'hw_ultrasonic_sampled_{index}';attempted=f'hw_ultrasonic_attempted_{index}'
        now=f'hw_ultrasonic_now_{index}';pulse=f'hw_ultrasonic_pulse_{index}'
        value='hw_value_'+distance['name'];available='hw_value_'+valid['name']
        declarations.extend([f'static uint32_t {sampled} = 0;',f'static bool {attempted} = false;'])
        setup.extend([f'pinMode({trigger}, OUTPUT);',f'digitalWrite({trigger}, LOW);',f'pinMode({echo}, INPUT);'])
        sampling.extend([f'uint32_t {now} = (uint32_t)millis();',
            f'if (!{attempted} || (uint32_t)({now} - {sampled}) >= {distance["sample_period_ms"]}UL) {{',
            f'  {attempted} = true; {sampled} = {now};',
            f'  digitalWrite({trigger}, LOW); delayMicroseconds(2);',
            f'  digitalWrite({trigger}, HIGH); delayMicroseconds(10); digitalWrite({trigger}, LOW);',
            f'  unsigned long {pulse} = pulseIn({echo}, HIGH, {distance["binding"]["timeout_us"]}UL);',
            f'  {available} = ({pulse} >= 116UL && {pulse} <= 23200UL);',
            f'  if ({available}) {value} = {pulse} / 58.0;', '}'])
    return declarations,setup,sampling


def check_circuit(contract,circuit,physical):
    boards={p.get('id') for p in circuit['parts'] if isinstance(p,dict) and p.get('type') in ('wokwi-arduino-uno','wokwi-arduino-mega')}
    devices={p.get('id') for p in circuit['parts'] if isinstance(p,dict) and p.get('type')=='wokwi-hc-sr04'}
    sensors=groups(contract)
    if len(devices)>len(sensors):raise ValueError('Explicit HC-SR04 circuit devices missing paired distance/valid inputs')
    for device in devices:
        signals={}
        for connection in circuit['connections']:
            if not isinstance(connection,list) or len(connection)<2 or not all(isinstance(s,str) for s in connection[:2]):continue
            for left,right in (connection[:2],connection[:2][::-1]):
                dev,_,signal=left.partition(':');board,_,pin=right.partition(':')
                if dev==device and signal in ('TRIG','ECHO') and board in boards:
                    signals[signal]=physical(pin)
        if signals and not any(all(key[0 if signal=='TRIG' else 1]==pin for signal,pin in signals.items()) for key,_,_ in sensors):
            raise ValueError('Explicit HC-SR04 TRIG/ECHO wiring differs for '+str(device))
