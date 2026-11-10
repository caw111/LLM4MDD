"""Independent field diagnostics for repair; never an alternative acceptance gate."""
import re


def independent_field_diagnostics(contract, requirement):
    if not isinstance(contract, dict) or not isinstance(contract.get('interfaces'), list):
        return []
    errors=[];ports=[p for p in contract['interfaces'] if isinstance(p,dict)]
    by_name={p['name']:p for p in ports if isinstance(p.get('name'),str)}
    units={'dht_temperature':(('f32','f64'),'degC'), 'dht_humidity':(('f32','f64'),'%RH'),
           'dht_valid':(('bool',),'1'),'ultrasonic_distance':(('f32','f64'),'cm'),
           'ultrasonic_valid':(('bool',),'1')}
    for index,p in enumerate(ports[:128]):
        name=p.get('name',f'interfaces[{index}]')
        for field in ('unit','meaning','hardware','device'):
            if not isinstance(p.get(field),str) or not p[field].strip():
                errors.append(f'{name}.{field}: expected a non-empty string; typed device mappings belong in binding.')
        b=p.get('binding')
        if not isinstance(b,dict):
            errors.append(f'{name}.binding: expected a structured hardware mapping object.');continue
        kind=b.get('kind')
        if not isinstance(kind,str):
            errors.append(f'{name}.binding.kind: expected a supported binding name string.');continue
        if kind in units:
            types,unit=units[kind]
            if p.get('type') not in types or p.get('unit')!=unit:
                errors.append(f'{name}: binding {kind} requires type {"/".join(types)} and unit {unit}; '
                              f'got type={p.get("type")!r}, unit={p.get("unit")!r}. Match the actual quantity: '
                              'dht_temperature is degC; dht_humidity is %RH. Do not relabel one measurement as another.')
        if kind in ('dht_valid','ultrasonic_valid') and p.get('initial') is not False:
            errors.append(f'{name}.initial: validity starts as Boolean false, not a valid measurement or a numeric zero.')
        minimum=2000 if kind.startswith('dht_') else 60 if kind.startswith('ultrasonic_') else None
        if kind.startswith('ultrasonic_') and type(b.get('timeout_us')) is int:
            minimum=max(minimum,(b['timeout_us']+12+999)//1000)
        if minimum is not None and (type(p.get('sample_period_ms')) is not int or p['sample_period_ms']<minimum):
            errors.append(f'{name}.sample_period_ms: got {p.get("sample_period_ms")!r}; {kind} requires an integer >= {minimum} ms. '
                          'Give all channels and validity of this sensor the same legal period, preserving the original sampling bounds. '
                          'The sensor sampling period is separate from control_period_ms.')
        if kind=='serial_value':
            trigger=b.get('trigger');target=by_name.get(trigger) if isinstance(trigger,str) else None
            target_binding=target.get('binding') if target else None
            if not target or target.get('direction')!='output' or target.get('type')!='bool' or not isinstance(target_binding,dict) or target_binding.get('kind')!='report_trigger':
                errors.append(f'{name}.binding.trigger={trigger!r}: declare the named bool output with binding kind report_trigger; '
                              'its timing/condition belongs in the SysML model, not the adapter.')
    # These are literal protocol requirements supplied to every method, not
    # hidden oracle values. Report every missing token in the same repair.
    from app.services.interface_planner import public_match
    labels=[p['binding'].get('label','') for p in ports if isinstance(p.get('binding'),dict) and p['binding'].get('kind')=='serial_value']
    for match in re.finditer(r'原因包含\s*([A-Z][A-Z0-9_]+)',requirement):
        if public_match(re.escape(match[0]),requirement) and not any(isinstance(label,str) and match[1] in label for label in labels):
            errors.append('Missing serial reason token '+match[1]+': preserve it in a serial_value label, with its own model-controlled '
                          'report_trigger where the reason has a distinct condition. Keep all requested devices and behaviors.')
    return list(dict.fromkeys(errors))[:64]
