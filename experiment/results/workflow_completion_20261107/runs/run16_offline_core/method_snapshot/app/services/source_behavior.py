"""Small executable obligations derived from narrow, unconditional source statements.

Not a general natural-language oracle. Unsupported/ambiguous requirements add no
obligation, rather than guessing polarity, thresholds, timing or calibration.
"""
import copy
import math
import re


def source_requirement(contract, requirement):
    """Behavior literals come from user text, not appended configuration metadata."""
    resolution = contract.get('configuration_resolution', {})
    original = resolution.get('original_requirement') if isinstance(resolution, dict) else None
    if isinstance(original, str) and original.strip():
        return original
    for marker in ('\nResolved hardware configuration (authoritative pin assignments;',
                   '\nOperational acceptance profile ', '\nFrozen public validation assumptions '):
        requirement = requirement.split(marker, 1)[0]
    return requirement


def sensor_statement(requirement):
    from app.services.interface_planner import public_match
    measurement = public_match(r"Continuously read the DHT sensor's (humidity|temperature) value", requirement)
    formatting = public_match(r'Print (humidity|temperature) to the serial monitor in the following format: "([^"{}]*)\{(?:humidity|temperature) value\}"', requirement)
    if measurement and formatting and measurement[1].lower() == formatting[1].lower():
        return measurement[1].lower(), formatting[2]
    return None


def startup_message(requirement):
    match=re.search(r'Send a message "([^"\n{}]+)" to the serial monitor',requirement)
    return match[1] if match else None


def check_source_messages(contract, requirement):
    """Check message availability, including conditional messages; not trigger correctness."""
    requirement = source_requirement(contract, requirement)
    ports = [p for p in contract['interfaces'] if p['binding']['kind'] in ('serial_text_output','serial_value')]
    available = [p['binding'].get('text',p['binding'].get('label','')) for p in ports]
    message=startup_message(requirement)
    if message and not any(p['binding']['kind']=='serial_text_output' and p['binding']['text']==message for p in ports):
        raise ValueError('SOURCE_FIXED_TEXT: send the exact startup message via serial_text_output, without an appended numeric payload: '+repr(message))
    lamps=[p for p in contract['interfaces'] if p['binding']['kind']=='digital_output']
    if message and len(lamps)==1 and re.search(r'Send a message[^\n]+\n3\. Begin blinking',requirement) and lamps[0]['initial']:
        raise ValueError('SOURCE_STARTUP: LED contract initial must be false: setup applies it before the model can emit the required startup message. Activate blinking only after sending that message.')
    conditional=re.search(r'print "([^"\n]+)" if the value is HIGH, or print "([^"\n]+)" if the value is LOW',requirement)
    if conditional:
        for text in conditional.groups():
            if not any(text in value for value in available):
                raise ValueError('SOURCE_CONDITIONAL_TEXT: preserve exact message '+repr(text)+
                    ' in a serial_text_output with a model-controlled gate for its state; Boolean/numeric output is not that message.')
    for match in re.finditer(r'串口输出[ \t]+([A-Za-z][A-Za-z0-9_]*(?:[ \t]+[A-Za-z][A-Za-z0-9_]*)*)', requirement):
        start = max(requirement.rfind(c,0,match.start()) for c in '\n。；;')+1
        prefix = requirement[start:match.start()]
        if re.search(r'不需要|无需|禁止|不要|不得|不必',prefix):
            continue
        token = match[1].strip()
        # A bare quantity name or a format name is not an unambiguous text literal.
        if not (' ' in token or '_' in token) or re.match(r'\s*(?:格式|编码|协议)',requirement[match.end():]):
            continue
        if not any(token in text for text in available):
            raise ValueError('SOURCE_MESSAGE: missing required serial text ' + repr(token) +
                             '; preserve it in a fixed-text output or report label. Conditions belong in the model.')


def derive_checks(contract, requirement):
    requirement = source_requirement(contract, requirement)
    ports = contract['interfaces']
    def of(kind):
        return [p for p in ports if p['binding']['kind'] == kind]
    checks = []
    buttons=of('digital_input')
    text_outputs=of('serial_text_output')
    message=startup_message(requirement)
    lamps=of('digital_output')
    if message and len(lamps)==1 and re.search(r'Send a message[^\n]+\n3\. Begin blinking',requirement):
        gates=[p for p in text_outputs if p['binding'].get('text')==message]
        if len(gates)!=1:raise ValueError('SOURCE_STARTUP: preserve exactly one exact-message gate before blinking')
        checks.append(dict(kind='startup_before_activation',output=lamps[0]['name'],message=gates[0]['name'],
            message_first=ports.index(gates[0])<ports.index(lamps[0]),evidence=requirement))
    conditional=re.search(r'Continuously check the button value, and print "([^"\n]+)" if the value is HIGH, or print "([^"\n]+)" if the value is LOW',requirement)
    if conditional and len(buttons)==1:
        names=[]
        for text in conditional.groups():
            matches=[p['name'] for p in text_outputs if p['binding'].get('text')==text]
            if len(matches)!=1:raise ValueError('SOURCE_CONDITIONAL_TEXT: declare one fixed-text gate for each exact HIGH/LOW message.')
            names.append(matches[0])
        checks.append(dict(kind='button_text_state',input=buttons[0]['name'],
            high_output=names[0],low_output=names[1],active_high=buttons[0]['binding'].get('active_high',True),evidence=requirement))
    segments=[p for p in of('digital_output') if re.search(r'_segment_\d+$',p['name'])]
    if len(buttons)==1 and len(segments)==10 and 'Second Press: The next two LEDs light up' in requirement and 'Sixth Press: All LEDs turn off' in requirement:
        checks.append(dict(kind='bargraph_pair_count',input=buttons[0]['name'],outputs=[p['name'] for p in segments],evidence=requirement))
    sensor = sensor_statement(requirement)
    if sensor:
        quantity, prefix = sensor
        unit = '%RH' if quantity == 'humidity' else 'degC'
        measurements = of('dht_' + quantity)
        values = [p for p in of('serial_value') if p['unit'] == unit]
        if len(measurements) != 1 or len(values) != 1:
            raise ValueError('SOURCE_SENSOR: preserve one native ' + quantity + ' input and matching-unit numeric report.')
        if values[0]['binding']['label'] != prefix:
            raise ValueError('SOURCE_SENSOR_FORMAT: required serial prefix is ' + repr(prefix) +
                             '; preserve punctuation/spaces in the report label, not a separate println.')
        from app.services.hardware_adapter import physical_pin
        validity = [p['name'] for p in of('dht_valid') if physical_pin(p['binding']['pin'],contract['board']) ==
                    physical_pin(measurements[0]['binding']['pin'],contract['board'])]
        checks.append(dict(kind='sensor_report', input=measurements[0]['name'], output=values[0]['name'],
                           trigger=values[0]['binding']['trigger'], valid_inputs=validity,
                           # Stay inside the shared DHT11/DHT22 measurement domain.
                           samples=[35.0,60.0,80.0] if quantity=='humidity' else [10.0,23.0,37.0],
                           evidence=requirement))
    # Whole-statement matching avoids turning conditional reports into unconditional ones.
    period = re.fullmatch(r'\s*每\s*(\d+)\s*秒向串口监视器输出一次\s+([^。\n]+)[。]?\s*', requirement)
    if period and 1 <= int(period[1]) <= 60:
        expected = period[2].strip()
        text = of('serial_text_output')
        if len(text) != 1 or text[0]['binding'].get('text') != expected or of('serial_value'):
            raise ValueError('SOURCE_FIXED_TEXT: one fixed-text output must preserve ' + repr(expected) +
                             '; remove duplicate numeric/report channels for this single-message requirement.')
        clocks = of('elapsed_ms')
        if len(clocks) != 1:
            raise ValueError('SOURCE_PERIODIC: periodic reporting needs exactly one elapsed_ms delta input.')
        checks.append(dict(kind='periodic_report', output=text[0]['name'], delta=clocks[0]['name'],
                           period_ms=int(period[1])*1000, evidence=requirement))
    sensor_report = re.fullmatch(r'\s*每\s*(\d+)\s*秒读取\s*DHT(?:11|22)（D\d+）温度并串口输出[。]?\s*', requirement, re.I)
    if sensor_report and 1 <= int(sensor_report[1]) <= 60:
        triggers = of('report_trigger')
        clocks = of('elapsed_ms')
        if len(triggers) != 1 or len(clocks) != 1 or not of('dht_temperature'):
            raise ValueError('SOURCE_PERIODIC: preserve one temperature report trigger, measurement and real elapsed_ms input.')
        checks.append(dict(kind='periodic_report', output=triggers[0]['name'], delta=clocks[0]['name'],
                           period_ms=int(sensor_report[1])*1000, evidence=requirement))
    command = re.fullmatch(r'\s*通过串口(?:（\d+\s*波特率）)?接收字符串\s*ON\s*或\s*OFF，控制\s*(D\d+)\s*LED\s*开关[。]?\s*', requirement, re.I)
    if command:
        from app.services.hardware_adapter import physical_pin
        outputs = [p for p in of('digital_output') if physical_pin(p['binding']['pin'], contract['board']) == physical_pin(command[1], contract['board'])]
        receivers = of('serial_command_input')
        if len(outputs) != 1 or len(receivers) != 1:
            raise ValueError('SOURCE_COMMAND_HOLD: ON/OFF needs one receiver and the specified LED output.')
        receiver = receivers[0]
        valid = [p for p in of('serial_valid') if p['binding'].get('value') == receiver['name']]
        codes = receiver['binding']['commands']
        if len(valid) != 1 or not {'ON', 'OFF'} <= codes.keys():
            raise ValueError('SOURCE_COMMAND_HOLD: preserve ON/OFF codes and the receiver validity input.')
        checks.append(dict(kind='command_hold', output=outputs[0]['name'], input=receiver['name'],
                           valid=valid[0]['name'], on=codes['ON'], off=codes['OFF'], evidence=requirement))
    pwm = re.fullmatch(r'\s*读取连接在\s*(A\d+)\s*引脚的电位器，根据数值控制\s*(D\d+)\s*PWM\s*LED\s*的亮度[。]?\s*', requirement, re.I)
    if pwm:
        from app.services.hardware_adapter import physical_pin
        def bound(kind, pin):
            matches = [p for p in of(kind) if physical_pin(p['binding']['pin'], contract['board']) == physical_pin(pin, contract['board'])]
            if len(matches) != 1:
                raise ValueError('SOURCE_PWM_RANGE: preserve the specified ' + kind + ' on ' + pin)
            return matches[0]
        adc, output = bound('analog_input', pwm[1]), bound('pwm_output', pwm[2])
        if adc['unit'] == 'adc_count' and output['unit'] == 'pwm_count':
            checks.append(dict(kind='pwm_range', input=adc['name'], output=output['name'], evidence=requirement))
    def on_pin(kind, pin):
        from app.services.hardware_adapter import physical_pin
        matches = [p for p in of(kind) if physical_pin(p['binding']['pin'], contract['board']) == physical_pin(pin, contract['board'])]
        if len(matches) != 1:
            raise ValueError('SOURCE_BUTTON: preserve one ' + kind + ' on ' + pin)
        return matches[0]['name']
    counter = re.fullmatch(r'\s*使用\s*(D\d+)\s*按钮进行计数，每按一次计数加\s*1，并通过串口输出当前计数值[。]?\s*', requirement, re.I)
    display_counter = re.fullmatch(r'\s*使用\s*TM1637\s*\+\s*按钮[（(](D\d+)[）)]：每按一次计数\s*\+\s*1\s*并显示[。]?\s*', requirement, re.I)
    if counter or display_counter:
        button = on_pin('digital_input', (counter or display_counter)[1])
        values = of('serial_value') if counter else of('tm1637_number')
        if len(values) != 1 or values[0]['type'] != 'i32':
            raise ValueError('SOURCE_BUTTON: counter needs one integer count output, not a Boolean or an omitted report.')
        check = dict(kind='button_counter', input=button, output=values[0]['name'], evidence=requirement)
        if counter:
            check['trigger'] = values[0]['binding']['trigger']
        checks.append(check)
    switches = re.fullmatch(r'\s*使用两个按钮控制\s*(D\d+)\s*LED，其中\s*(D\d+)\s*按钮用于开灯，\s*(D\d+)\s*按钮用于关灯[。]?\s*', requirement, re.I)
    if switches:
        checks.append(dict(kind='button_set_hold', output=on_pin('digital_output', switches[1]),
                           on_input=on_pin('digital_input', switches[2]),
                           off_input=on_pin('digital_input', switches[3]), evidence=requirement))
    toggle = re.fullmatch(r'\s*使用\s*(D\d+)\s*按钮控制\s*(D\d+)\s*LED，每按一次切换开关状态[。]?\s*', requirement, re.I)
    from app.services.interface_planner import public_match
    english_toggle = (public_match(re.escape("The goal is to toggle the LED's state (on/off) each time the button is pressed."), requirement) and
                      public_match(re.escape('The LED should maintain its current state until the button is pressed again.'), requirement))
    if toggle or english_toggle:
        buttons, lamps = of('digital_input'), of('digital_output')
        if toggle:
            button, lamp = on_pin('digital_input', toggle[1]), on_pin('digital_output', toggle[2])
        elif len(buttons) == len(lamps) == 1:
            button, lamp = buttons[0]['name'], lamps[0]['name']
        else:
            raise ValueError('SOURCE_BUTTON: toggle statement needs unambiguous button and LED bindings.')
        assertion = dict(kind='button_toggle', input=button, output=lamp, evidence=requirement)
        if english_toggle and 'Initialization: Upon powering on or resetting, the LED should be in the "on" state.' in requirement:
            assertion['initial'] = True
        checks.append(assertion)
    tone = re.fullmatch(r'\s*让\s*(D\d+)\s*引脚的蜂鸣器每\s*(\d+)\s*秒响\s*(\d+)\s*ms[。]?\s*', requirement, re.I)
    if tone:
        from app.services.hardware_adapter import physical_pin
        outputs = [p for p in ports if p['binding']['kind'] in ('tone_output','digital_output') and
                   physical_pin(p['binding']['pin'],contract['board']) == physical_pin(tone[1],contract['board'])]
        clocks = of('elapsed_ms')
        period, width = int(tone[2])*1000, int(tone[3])
        if not 0 < width < period <= 60000:
            raise ValueError('SOURCE_PULSE: sounding duration must be positive and shorter than its period (up to 60s).')
        if len(outputs) != 1 or len(clocks) != 1:
            raise ValueError('SOURCE_PULSE: preserve the buzzer output and one real elapsed_ms input.')
        checks.append(dict(kind='periodic_activity', output=outputs[0]['name'], delta=clocks[0]['name'],
                           period_ms=period, pulse_ms=width, evidence=requirement))
    return checks


def add_source_checks(contract, requirement):
    result = copy.deepcopy(contract)
    # Configuration appends board/pin JSON to the prompt; it must not hide the
    # original behavioral sentence from exact source extraction.
    requirement = result.get('configuration_resolution', {}).get('original_requirement', requirement)
    check_source_messages(result, requirement)
    derived = derive_checks(result, requirement)
    existing = result.get('source_behavior_checks', [])
    if existing and existing != derived:
        raise ValueError('source_behavior_checks contradict program-derived source obligations.')
    if derived:
        result['source_behavior_checks'] = derived
    return result


def validate_source_checks(contract):
    checks = contract.get('source_behavior_checks', [])
    if not isinstance(checks, list) or len(checks) > 2:
        raise ValueError('source_behavior_checks must be a bounded list')
    if not checks:
        return checks
    evidence = checks[0].get('evidence') if isinstance(checks[0], dict) else None
    if not isinstance(evidence, str) or not evidence.strip() or checks != derive_checks(contract, evidence):
        raise ValueError('source_behavior_checks must match their exact source statement and interfaces')
    step = contract['control_period_ms']
    if type(step) is not int or not 1 <= step <= 1000:
        raise ValueError('source_behavior_checks require control period 1..1000ms')
    return checks


def check_source_behavior(ir, contract):
    checks = validate_source_checks(contract)
    report = dict(status='not_requested' if not checks else 'passed', checked=0, traces=[],
                  scope='Only the exact source-derived scenarios; not full functionality proof')
    issues = []
    from app.services.ir_execution import IRExecution
    for check in checks:
        engine = IRExecution(ir)
        inputs = {p['name']: p['initial'] for p in contract['interfaces'] if p['direction'] == 'input'}
        trace = []
        try:
            if check['kind'] == 'bargraph_pair_count':
                button=check['input']
                step=contract['control_period_ms']
                clocks=[p['name'] for p in contract['interfaces'] if p['binding']['kind']=='elapsed_ms']
                inputs.update({name:step for name in clocks})
                settling=max(1,math.ceil(250/step))
                for _ in range(settling+1):engine.step(dict(inputs,**{button:False}))
                for press,expected in enumerate([2,4,6,8,10,0,2],1):
                    for pressed in [True,False]:
                        for n in range(settling+3):
                            actual=engine.step(dict(inputs,**{button:pressed}))
                            count=sum(bool(actual[name]) for name in check['outputs'])
                            if not pressed or n>=settling-1:
                                trace.append(dict(press=press,pressed=pressed,settled_ms=(n+1)*step,expected_count=expected,actual_count=count,outputs=actual))
                                if count!=expected:raise AssertionError('Each press adds exactly two retained segments within250ms; sixth clears all; holding/releasing must not advance.')
            elif check['kind']=='startup_before_activation':
                if next(p['initial'] for p in contract['interfaces'] if p['name']==check['output']):
                    raise AssertionError('Startup LED must not activate in setup before the model-controlled startup message.')
                emitted=False;activated=False
                step=contract['control_period_ms']
                clocks=[p['name'] for p in contract['interfaces'] if p['binding']['kind']=='elapsed_ms']
                for n in range(max(2,math.ceil(2000/step))):
                    actual=engine.step(dict(inputs,**{name:0 if n==0 else step for name in clocks}))
                    gate=bool(actual[check['message']]);active=bool(actual[check['output']])
                    trace.append(dict(at_ms=n*step,message_gate=gate,active=active))
                    if active and not (emitted or gate and check['message_first']):
                        raise AssertionError('Emit the startup message before activating the LED; adapter writes follow contract interface order. Use a separate startup model phase if needed.')
                    emitted=emitted or gate;activated=activated or active
                    if emitted and activated:break
                if not emitted or not activated:
                    report.update(status='incomplete',skipped=[dict(check=check,reason='Startup not fully observed in bounded horizon')])
                    continue
            elif check['kind'] == 'button_text_state':
                step=contract['control_period_ms']
                clocks=[p['name'] for p in contract['interfaces'] if p['binding']['kind']=='elapsed_ms']
                inputs.update({name:step for name in clocks})
                for high in [False,True,False]:
                    expected=check['high_output'] if high else check['low_output']
                    other=check['low_output'] if high else check['high_output']
                    seen=False
                    for n in range(max(2,math.ceil(250/step))):
                        actual=engine.step(dict(inputs,**{check['input']:high==check['active_high']}))
                        seen=seen or bool(actual[expected])
                        trace.append(dict(high=high,outputs=actual,expected_gate=expected))
                        if n>=math.ceil(250/step)-1 and actual[other]:raise AssertionError('Settled state-report message contradicts the sampled HIGH/LOW state.')
                    if not seen:raise AssertionError('Continuous HIGH/LOW reporting omits a stable state, including startup LOW; edge-only reporting is insufficient.')
            elif check['kind'] == 'sensor_report':
                step = contract['control_period_ms']
                clocks = [p['name'] for p in contract['interfaces'] if p['binding']['kind']=='elapsed_ms']
                inputs.update({name:True for name in check['valid_inputs']})
                elapsed_steps = 0
                for value in check['samples']:
                    observed = []
                    for n in range(math.ceil(6000/step)+1):
                        stimulus = dict(inputs, **{check['input']:value}, **{name:0 if elapsed_steps==0 else step for name in clocks})
                        actual = engine.step(stimulus)
                        elapsed_steps += 1
                        if actual[check['trigger']]:
                            observed.append(actual[check['output']])
                    trace.append(dict(input=value, valid_inputs=check['valid_inputs'], horizon_ms=6000,
                                      report_count=len(observed), last_reports=observed[-5:]))
                    if not observed:
                        report.update(status='incomplete', skipped=[dict(check=check, reason='No report in bounded sensor scenario; no source response deadline specified')])
                        break
                    # Permit ordinary integer rounding when source text specifies no precision.
                    if not any(abs(actual-value) <= 0.6 for actual in observed):
                        raise AssertionError('Reported sensor values never match the stable valid measurement; inspect stale/constant/wrong-input model state.')
                if report['status'] == 'incomplete':
                    report['traces'].append(dict(check=check,trace=trace))
                    continue
            elif check['kind'] in ('button_counter', 'button_set_hold', 'button_toggle'):
                buttons = [p['name'] for p in contract['interfaces'] if p['binding']['kind'] == 'digital_input']
                # Model inputs are normalized pressed values; no physical HIGH/LOW assumption is added here.
                inputs.update({name:False for name in buttons})
                baseline = engine.step(inputs)[check['output']]
                if 'initial' in check and baseline != check['initial']:
                    trace.append(dict(inputs=inputs, actual=baseline, expected=check['initial']))
                    raise AssertionError('Startup LED state differs from the explicit source requirement.')
                if check['kind'] == 'button_counter':
                    scenarios = [('first_press', {check['input']:True}, baseline+1),
                                 ('first_release', {}, baseline+1),
                                 ('second_press', {check['input']:True}, baseline+2),
                                 ('second_release', {}, baseline+2)]
                elif check['kind'] == 'button_toggle':
                    scenarios = [('first_press', {check['input']:True}, not baseline),
                                 ('first_release', {}, not baseline),
                                 ('second_press', {check['input']:True}, baseline),
                                 ('second_release', {}, baseline)]
                else:
                    scenarios = [('off', {check['off_input']:True}, False), ('off_hold', {}, False),
                                 ('on', {check['on_input']:True}, True), ('on_hold', {}, True),
                                 ('off_again', {check['off_input']:True}, False)]
                for label, pressed, expected in scenarios:
                    reported = False
                    for n in range(4):
                        stimulus = dict(inputs, **pressed)
                        actual = engine.step(stimulus)
                        observable = not check.get('trigger') or actual[check['trigger']]
                        trace.append(dict(scenario=label, step=n, inputs=stimulus, outputs=actual, expected=expected))
                        if observable:
                            reported = True
                            if actual[check['output']] != expected:
                                raise AssertionError('Button press must change state once; holding/releasing must preserve it.')
                    if check.get('trigger') and label.endswith('press') and not reported:
                        raise AssertionError('Button press did not emit the required count report; check edge memory/output ordering.')
            elif check['kind'] == 'pwm_range':
                adc = next(p for p in contract['interfaces'] if p['name'] == check['input'])['binding']
                pwm = next(p for p in contract['interfaces'] if p['name'] == check['output'])['binding']
                for raw in [0, 256, 512, 768, 1023]:
                    stimulus = dict(inputs, **{check['input']: raw*adc.get('scale', 1)+adc.get('offset', 0)})
                    actual = engine.step(stimulus)[check['output']]
                    electrical = actual*pwm.get('scale', 1)+pwm.get('offset', 0)
                    trace.append(dict(inputs=stimulus, output=actual, pwm_before_clamping=electrical))
                    if not 0 <= electrical <= 255:
                        raise AssertionError('PWM command outside 0..255; adapter saturation is not ADC-to-PWM conversion.')
            elif check['kind'] == 'command_hold':
                for name, code, valid, expected in [('on', check['on'], True, True),
                        ('on_hold', check['on'], False, True), ('off', check['off'], True, False),
                        ('off_hold', check['off'], False, False)]:
                    for _ in range(3):
                        stimulus = dict(inputs, **{check['input']: code, check['valid']: valid})
                        actual = engine.step(stimulus)[check['output']]
                        trace.append(dict(scenario=name, inputs=stimulus, expected=expected, actual=actual))
                        if actual != expected:
                            raise AssertionError('Command state must persist after its one-step valid event.')
                        valid = False
            elif check['kind'] in ('periodic_report','periodic_activity'):
                if check['kind'] == 'periodic_report':
                    # Cadence is tested with valid measurements available. Keeping
                    # validity at its startup false value would test sensor failure.
                    for port in contract['interfaces']:
                        if port['direction'] == 'input' and port['binding']['kind'] in ('dht_valid','ultrasonic_valid'):
                            inputs[port['name']] = True
                step, period = contract['control_period_ms'], check['period_ms']
                horizon = 3*period + check.get('pulse_ms', 0)
                if math.ceil(horizon/step) > 20000 or check.get('pulse_ms', period) < step:
                    report.update(status='incomplete', skipped=[dict(check=check, reason='Execution budget or sampling resolution insufficient')])
                    continue
                schedules = [[step]]
                if 3*step < min(period, check.get('pulse_ms', period)):
                    schedules.append([step, 2*step, step, 3*step])
                for deltas in schedules:
                    engine = IRExecution(ir)
                    events, widths = [], []
                    previous, started, now, n = False, None, 0, 0
                    while now <= horizon:
                        delta = 0 if n == 0 else deltas[(n-1) % len(deltas)]
                        stimulus = dict(inputs, **{check['delta']: delta})
                        actual = engine.step(stimulus)[check['output']]
                        active = bool(actual)
                        if active and (check['kind'] == 'periodic_report' or not previous):
                            events.append(now)
                        if active and not previous:
                            started = now
                        if not active and previous:
                            widths.append(now-started)
                        previous = active
                        now += deltas[n % len(deltas)]
                        n += 1
                    trace.append(dict(delta_schedule_ms=deltas, event_times_ms=events[:20], count=len(events),
                                      period_ms=period, pulse_widths_ms=widths[:20]))
                    # Permit an optional startup message, but require recurrence and no flooding.
                    tolerance = max(deltas)
                    if len(events) < 2 or any(abs((b-a)-period) > tolerance for a,b in zip(events, events[1:])):
                        raise AssertionError('Expected recurrence in elapsed milliseconds, not a fixed number of control steps; check timer/output ordering.')
                    if check['kind'] == 'periodic_activity' and (len(widths) < 2 or any(abs(w-check['pulse_ms']) > tolerance for w in widths)):
                        raise AssertionError('Buzzer activity must recur with the source sounding duration; constant ON/OFF is insufficient.')
            report['checked'] += 1
            report['traces'].append(dict(check=check, trace=trace))
        except (AssertionError, ValueError, ArithmeticError) as exc:
            report.update(status='failed', counterexample=dict(check=check, trace=trace, reason=str(exc)))
            issues.append(dict(severity='error', code='SOURCE_BEHAVIOR', stage='requirements',
                message=str(exc), counterexample=report['counterexample']))
            break
    return report, issues
