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


def model_boundary_prompt(contract):
    from app.services.ultrasonic import groups
    ultrasonic=groups(contract)
    sensor_prompt=''
    from app.services.dht import groups as dht_groups
    dht=dht_groups(contract)
    if dht:
        sensor_prompt=('\nDHT model boundary: '+json.dumps([
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
