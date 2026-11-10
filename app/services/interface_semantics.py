"""Model/electrical boundary definitions shared by validation, prompts and glue."""
import copy
import json


def digital_levels(binding):
    active_high = binding.get('active_high', True)
    if type(active_high) is not bool:
        raise ValueError('active_high must be Boolean')
    return ('HIGH', 'LOW') if active_high else ('LOW', 'HIGH')


def digital_semantics(port):
    binding = port.get('binding', {})
    if not isinstance(binding, dict) or binding.get('kind') not in ('digital_input', 'digital_output'):
        return None
    true_level, false_level = digital_levels(binding)
    return dict(kind='digital_boolean', true_level=true_level, false_level=false_level)


def validate_model_semantics(port):
    if 'model_semantics' in port:
        expected = digital_semantics(port)
        if expected is None or port['model_semantics'] != expected:
            raise ValueError('Interface '+port['name']+'.model_semantics contradicts its hardware binding; expected '+json.dumps(expected))


def freeze_model_semantics(contract):
    """Materialize derived facts before freezing; never overwrite a contradiction."""
    result = copy.deepcopy(contract)
    for port in result['interfaces']:
        validate_model_semantics(port)
        semantics = digital_semantics(port)
        if semantics is not None:
            port['model_semantics'] = semantics
    return result


def adapter_boundary_facts(contract):
    """Facts of the registered AVR adapter, not guesses by the reviewer."""
    if not isinstance(contract,dict) or contract.get('board') not in ('arduino:avr:uno','arduino:avr:mega'):
        return []
    facts=[]
    from app.services import extended_devices
    import hashlib
    from pathlib import Path
    native_driver_sha256=hashlib.sha256(Path(extended_devices.__file__).read_bytes()).hexdigest()
    for port in contract.get('interfaces',[]):
        binding=port.get('binding',{})
        if binding.get('kind')=='report_trigger' and 'format' in binding:
            facts.append(dict(port=port['name'],kind='formatted_serial_report',format=binding['format'],
                value_ports=binding['values'],semantics='When this model Boolean is true, the deterministic adapter '
                    'formats the listed model output values by their numeric indices, then appends ONE newline. '
                    'iso/rtc/date/weekday are calendar formatting of an already model-computed Unix epoch, '
                    'not application time selection. SysML does not construct strings.'))
        if binding.get('kind') in ('serial_value','serial_text_output','serial_echo','serial_command_input',
                                   'serial_integer_input','serial_datetime_input','serial_string_input','serial_packet_input'):
            facts.append(dict(port=port['name'],kind='serial_adapter_transport',baud=binding['baud'],
                initialization='The adapter setup calls Serial.begin at this registered baud.',
                receive_value={'serial_packet_input':'COMMAND INTEGER frames produce command_code*10000+payload as one i32. '
                    'Recover code with Integer value/10000 and payload=value-(value/10000)*10000. '
                    'io_i_valid marks one accepted frame; validate application HHMM/minute rules in the model.',
                    'serial_datetime_input':'Calendar-valid ISO date input becomes i32 Unix seconds, with one-step validity.',
                    'serial_string_input':'Each accepted ASCII frame increments an i32 sequence counter; actual text stays '
                    'in the bounded immutable adapter buffer. Forward the counter to the linked lcd_text/serial_echo '
                    'output when the valid input is true. No model string operations are needed.'}.get(binding['kind']),
                semantics='Numeric reports print the registered label followed by the model value and newline, '
                    'unless their trigger has a compound format. Fixed text outputs print their registered text '
                    'when the model gate is true. Receive parsers publish framed values and one-step validity. '
                    'All application reporting cadence and command effects remain in the model.'))
        if binding.get('kind') in extended_devices.CATALOG:
            kind=binding['kind']
            facts.append(dict(port=port['name'],kind='native_device_adapter',operation=kind,
                binding=copy.deepcopy(binding),implementation='app/services/extended_devices.py',
                implementation_sha256=native_driver_sha256,
                responsibility='The deterministic adapter, not SysML, declares device/library objects, initializes the bus/device, '
                    'samples hardware input quantities and applies scalar output commands. Do not require C++ library calls '
                    'or GPIO setup in the scalar controller.',
                lcd_semantics=('Adafruit_ILI9341 constructor uses cs_pin/dc_pin/rst_pin; setup calls begin, setRotation(rotation) '
                    'and fillScreen(background). On a changed scalar field, the adapter clears the screen and redraws ALL '
                    'registered fields using each x/y, color and text_size. lcd_text selects binding.messages by integer code '
                    'or the immutable received frame; lcd_number/lcd_datetime format the model-computed scalar. '
                    'The model still owns message selection, values, timezone/day offsets and application timing.') if kind in extended_devices.LCD else None,
                tm1637_semantics=('The model supplies an i32 message code. The adapter renders the corresponding fixed '
                    'binding.messages label with seven-segment glyphs; text longer than four glyphs scrolls at scroll_ms. '
                    'Unmapped numeric commands show their number. Text scrolling is presentation, not application '
                    'occupancy/timer behavior; those decisions remain in SysML.') if kind=='tm1637_text' else None,
                acquisition_semantics=('RTC_DS1307.begin and now().unixtime, or the declared MPU native API, supply measured '
                    'quantities plus initialization validity before controller ticks. Initial scalars are not sensor measurements; '
                    'the model must use validity and implement thresholds/state/report decisions.') if kind in extended_devices.SENSORS else None,
                runtime_evidence='Registered implementation semantics only; actual target compilation and host/device tests are separate checks.'))
        if binding.get('kind')=='digital_input':
            facts.append(dict(port=port['name'],kind='digital_acquisition',
                initial_model_value=port['initial'],model_semantics=digital_semantics(port),
                setup_pinMode='INPUT_PULLUP' if binding.get('pullup',False) else 'INPUT',
                lifecycle='The contract initial value initializes an adapter variable only. The adapter samples digitalRead '
                          'and normalizes the electrical level BEFORE the first controller tick; subsequent sampling follows '
                          'sample_period_ms. The initial value is not injected as a synthetic button event. '
                          'Persistent edge-history initialization remains model behavior and must handle the first sampled press.'))
        if binding.get('kind')=='digital_output':
            facts.append(dict(port=port['name'],kind='digital_command',setup_initial_command=port['initial'],
                lifecycle='Setup writes the contract initial Boolean before any controller tick or model-gated serial text. '
                          'Recurring output writes follow contract interface order after ordered model updates. '
                          'If a message must precede activation, keep this initial command OFF and use model startup phases '
                          'or contract output order to emit the message before the first active command.'))
        if binding.get('kind')=='analog_input':
            scale,offset=binding.get('scale',1.0),binding.get('offset',0.0)
            endpoints=[offset,1023*scale+offset]
            facts.append(dict(port=port['name'],kind='analog_acquisition',raw_range=[0,1023],
                raw_unit='adc_count',scale=scale,offset=offset,model_range=sorted(endpoints),
                formula='model_value = analogRead(pin) * scale + offset',
                basis='Registered Uno/Mega AVR analogRead adapter; default 10-bit ADC'))
        if binding.get('kind') in ('serial_text_output','report_trigger'):
            facts.append(dict(port=port['name'],kind=binding['kind'],
                consumption='Level-sensitive: each model step with gate=true emits the fixed text or associated numeric report. Not rising-edge or one-shot.',
                emission_stage='After each controller step, in contract interface order; not during setup.',
                startup_once='To emit once, capture a startup event before setting persistent sent state; outputs observe ordered state updates.'))
        if binding.get('kind')=='pwm_output':
            scale,offset=binding.get('scale',1.0),binding.get('offset',0.0)
            facts.append(dict(port=port['name'],kind='pwm_command',model_unit=port.get('unit'),
                scale=scale,offset=offset,electrical_range=[0,255],
                formula='electrical_duty = truncate_toward_zero(clamp(model_output * scale + offset, 0, 255))',
                unclamped_model_range=sorted([(0-offset)/scale,(255-offset)/scale]) if scale else None,
                controllable=bool(scale),
                warning='Scaling is always applied, including pwm_count ports. Saturation does not implement a missing linear mapping. Finite commands required.'))
        if binding.get('kind')=='servo_angle':
            facts.append(dict(port=port['name'],kind='servo_command',unit='deg',valid_range=[0,180],
                invalid='Nonfinite or out-of-range commands retain the previous valid command.',
                conversion='Valid angles are truncated to integer degrees before Servo.write; writes are issued on changed commands.',
                setup_initial_command=port['initial'],
                lifecycle='Adapter attaches the servo and issues the contract initial command during setup, before recurring controller ticks. '
                          'Controller outputs are evaluated after ordered updates. Immediate first-tick movement need not preserve the initial '
                          'position until after the first tick; later movement/prepositioning must still follow the source lifecycle. '
                          'This describes commanded positions, not physical shaft dynamics.'))
    return facts


def model_boundary_prompt(contract):
    from app.services.ultrasonic import groups
    ultrasonic=groups(contract)
    facts=adapter_boundary_facts(contract)
    sensor_prompt=('\nNormative registered adapter facts: '+json.dumps(facts,ensure_ascii=False)+'\n') if facts else ''
    from app.services.dht import groups as dht_groups
    dht=dht_groups(contract)
    if dht:
        sensor_prompt+=('\nDHT model boundary: '+json.dumps([
            dict(channels={kind:p['name'] for kind,p in ports.items()},
                 startup='model runs with validity false' if 'dht_valid' in ports else 'legacy: wait for first valid frame',
                 invalid='retain last valid measurements; dht_valid is false for a failed acquisition' if 'dht_valid' in ports else 'retain last valid measurements')
            for _,ports in dht],ensure_ascii=False)+
            '\nInputs already use degC and %RH. Validity describes the most recent acquisition, not a one-step freshness pulse. '
            'Do not treat initial or retained measurements as valid when dht_valid is false. '
            'Invalid-data handling, display selection, thresholds, alarms and timers belong in SysML. '
            'A failed DHT read must not freeze unrelated sensors or model state.\n')
    if ultrasonic:
        sensor_prompt+=('\nHC-SR04 model boundary: '+json.dumps([
            dict(distance=d['name'],validity=v['name'],unit='cm',valid_range=[2,400],initial_valid=False,
                 invalid='distance retains last valid sample; validity becomes false') for _,d,v in ultrasonic],ensure_ascii=False)+
            '\nDo not treat the initial or retained distance as a current valid sample. '
            'The distance input is ALREADY centimeters; the adapter converts echo duration, '
            'so do not divide that input by 58 again or read hardware pins in the model. Use the validity input for the '
            'requested timeout behavior. Threshold comparisons, rounding, display selection and alarm control belong in SysML.\n')
    rows = []
    for port in contract['interfaces']:
        validate_model_semantics(port)
        semantics = digital_semantics(port)
        if semantics is not None:
            rows.append(dict(name=port['name'], direction=port['direction'], pin=port['binding']['pin'],
                             initial_model_value=port['initial'], **semantics))
    if not rows:
        return sensor_prompt
    return ('\nNormative model/electrical boundary (derived from the SAME binding used by the adapter):\n'+
            json.dumps(rows, ensure_ascii=False)+'\nFor each input, the model Boolean is true exactly when the pin is at true_level; '
            'it is false at false_level. For each output, model true drives true_level and false drives false_level. '
            'initial_model_value is a MODEL value, not a raw pin voltage. Meaning/hardware prose may describe physical levels; '
            'do not reinterpret those levels as model Boolean values. For an active-low pressed button, LOW already becomes true '
            'before the model runs: test that input directly for pressed, and detect a rising MODEL edge for press. '
            'Do not invert again or change the binding to fit a mistaken interpretation. Control and state stay in SysML.\n'+sensor_prompt)
