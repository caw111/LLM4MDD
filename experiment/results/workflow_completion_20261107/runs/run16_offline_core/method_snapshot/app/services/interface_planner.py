"""Plan hardware bindings before model generation; never claim behavioral proof."""
import hashlib
import copy
import json
import re

from app.services.hardware_adapter import validate_hardware_contract, physical_pin, board_profile, binding_pins
from app.services.behavior_contract import validate_sysml_contract
from app.services.interface_semantics import freeze_model_semantics
from app.services.llm_json import json_prompt, check_completion
from app.services.interface_registry import assemble_interface_contract, register_contract, InterfaceCapabilityError, normalize_interface_proposal, SERIAL_INPUTS


def contract_digest(contract):
    # JSON 数字没有 int/float 标签。浏览器会把 0.0、1.0 写回 0、1，
    # 不能把这种无损往返误判为契约篡改；不改动原契约或其类型校验。
    def numbers(value):
        if isinstance(value,float) and value.is_integer():return int(value)
        if isinstance(value,list):return [numbers(item) for item in value]
        if isinstance(value,dict):return {key:numbers(item) for key,item in value.items()}
        return value
    return hashlib.sha256(json.dumps(numbers(contract), ensure_ascii=False, sort_keys=True, separators=(',', ':'), allow_nan=False).encode('utf-8')).hexdigest()


CONTRACT_PROMPT = '''Return ONLY a JSON interface proposal. The application constructs and registers all ports.
Do NOT generate interfaces, interface_registry, port names, directions, binding objects, or serial trigger references.
This prohibition concerns program-owned port objects ONLY. All catalog interface_requests ARE allowed,
including serial_command_input, serial_integer_input, serial_text_output and multiple GPIO/RGB requests.
Use device for the source component ID when circuit facts below identify it. Device requests are not port objects.
Return {version:1, board:"arduino:avr:uno" or "arduino:avr:mega", control_period_ms:integer,
interface_requests:[device requests], serial_channels:[serial reporting groups], planning_notes:[assumptions]}.
Preserve the specified board and actual wiring. Empty supplied pins means no overrides, not absent source wiring.
Unknown/missing wiring, contradictory pins and unsupported operations must be reported as {"unsupported":[reasons]}.
Never invent a device, replace one with a different device, or silently omit a requirement.
Every explicitly wired functional board pin must be represented by a matching catalog request.
For example, a requirement that drives PWM on D9 requires a pwm_output request for D9;
serial input alone does not cover that physical output.

Each interface_requests[i] becomes a program-owned port io_i. Each request has kind, initial, meaning,
and only the catalog configuration keys for that operation. No name/type/direction/binding fields.
All configuration keys belong DIRECTLY on the request. There is NO field named optional or raw_unit.
Valid example: {"kind":"digital_output","pin":"D13","initial":false,"meaning":"LED command","active_high":true}.
Valid example: {"kind":"analog_input","pin":"A0","initial":0,"meaning":"Potentiometer ADC"}.
Valid example: {"kind":"tm1637_number","clk_pin":"D4","dio_pin":"D5","initial":0,"meaning":"Display number","leading_zero":false}.
Use JSON true/false for Boolean fields. Omit unspecified sampling to use a legal catalog default; NEVER use zero.
The application assigns the scalar type, electrical unit, and output update period.
Digital Boolean initial/meaning are normalized MODEL values. active_high=false means model true at LOW.
Do not invert an active-low pressed button again: its model value is true when pressed.
initial must follow the original startup requirement; never copy a generic example's value.
Control period is scheduling granularity, not business/report/blink period. Prefer 1..10 ms subject to explicit deadlines.
Outputs and elapsed_ms are updated every control step. Sensor sampling may be an integer multiple of that step.
Business thresholds, conversion, report timing, pause/resume, counters and transitions belong in the SysML model.
Do not put timers, conditions, speed/easing/duration logic into a device request.
Required real business time is automatically registered if absent; it is an i32 ms delta since the previous step,
first delta zero, NEVER absolute uptime or a fabricated constant control_period_ms increment.
A response deadline alone does not introduce model business time.


Additional supported native device requests (same direct configuration keys, no binding/name/type):
dht_valid: Boolean initial false, unit 1, pin, sensor DHT11/DHT22, invalid_policy hold_last_valid.
Declare validity for EACH DHT sensor. Temperature/humidity/validity share the same physical pin, sensor and sample_period_ms >=2000.
One acquisition publishes a coherent frame; invalid reads retain measurements and set validity false. Handle invalid data in SysML.
ultrasonic_distance AND ultrasonic_valid: declare BOTH requests with the same trigger_pin, echo_pin, sensor HC-SR04,
timeout_us 25000..60000, invalid_policy hold_last_valid and sample_period_ms >=60 and >=ceil((timeout_us+12)/1000),
rounded up to a control-period multiple. Distance: initial 0, f64 cm; validity: initial false, Boolean unit 1.
Timeout/out-of-range sets validity false, retaining previous distance. Supported distances 2..400cm.
Bounded pulseIn blocks; include acquisition time in response budgets. Missing-data decisions belong in SysML.
shift595_number: ONE i32 output for the WHOLE NUMBER on each 74HC595 cascade.
Direct keys: data_pin, clock_pin, latch_pin, leading_zero Boolean, invalid_policy blank, unit for displayed quantity,
registers:[{register:actual diagram ID,display:actual diagram ID,digit:decimal position,segment_bits:[seven physical Q0..Q7 bits for A..G],active_low:Boolean}].
List every register nearest MCU first; digit 0 is leftmost. Copy wiring, polarity and digit positions exactly.
Supports 1..4 statically wired digits, MR HIGH and OE LOW; no multiplexing/signs/text/decimal points.
No separate tens/units outputs sharing bus pins. Range 0..10^digits-1; invalid commands blank.
All counting, unit conversion, wrap/reset, parity and button logic belong in SysML.
tm1637_number also accepts an out-of-display-range i32 initial command (e.g. 10000) to request startup blank.
tm1637_text: i32 output code, clk_pin/dio_pin, messages mapping i32 codes to ASCII text,
brightness0..7(default7), scroll_ms100..5000(default300). The adapter renders fixed labels selected by the MODEL,
scrolling strings longer than4 glyphs; otherwise holds the visible text. Supports digits and letters
A,C,D,E,F,H,I,L,N,O,P,R,S,T,U,Y, space and dash (seven-segment approximations). Unknown codes in -999..9999
display as numbers. Example messages:{"-1":"OCCUPIED","-2":"FREE"}. Use this native operation for textual
TM1637 requirements; do not claim the hardware is numeric-only or require display-library calls inside SysML.

Catalog (create only required operations; all sample pin numbers are examples):
digital_input: initial Boolean, meaning, pin, optional active_high/pullup; program type bool, unit 1.
digital_output: initial Boolean, meaning, pin, optional active_high; bool, unit 1.
analog_input: initial numeric, meaning, pin A0 etc, optional scale/offset/sample_period_ms; f64, raw unit adc_count.
pwm_output: initial numeric, meaning, PWM-capable pin, optional scale/offset; f64, raw unit pwm_count.
Explicit physical-unit analog/PWM calibration requires unit AND explicit scale/offset; never fabricate calibration.
elapsed_ms: initial 0, meaning; i32 ms, optional semantics MUST be delta_since_previous_step.
dht_temperature/dht_humidity: initial numeric (use 0 when no startup measurement is specified), meaning, pin, sensor DHT11/DHT22, optional sample_period_ms >=2000;
f64 degC/%RH, program invalid_policy hold_last_valid. Both readings may share the same sensor pin.
servo_angle: initial 0..180, meaning, pin; f64 deg, program invalid_policy hold_last_valid. Declare EACH servo.
Only positional servos, default 544..2400us endpoints; no feedback. Uno Timer1 disables PWM D9/D10;
Mega groups of 12 disable PWM on D44/D45/D46, then D11/D12, then D2/D3/D5, then D6/D7/D8.
Servo AVR pins must be physical 0..63. Never substitute PWM/digital for an angle command.
tone_output: initial 0 or 31..65535, meaning, pin; i32 Hz, invalid_policy silence. One simultaneous tone channel;
for multiple fixed buzzer pins explicitly guaranteed mutually exclusive, set channel_mode="exclusive" on EVERY tone request.
The adapter switches the active physical pin with noTone before starting the next pin; simultaneous polyphony remains unsupported.
Timer2 conflicts with Uno PWM D3/D11 and Mega PWM D9/D10. Model duration/pauses using real elapsed time.
tm1637_number: initial integer, meaning, clk_pin, dio_pin, optional brightness/leading_zero; i32,
invalid_policy blank. Supported signed number -999..9999 (0..9999 with leading_zero), not arbitrary text.
Keep number selection, scaling/rounding in the model; NEVER use separate GPIO scalars for this bus protocol.
led_bar_graph: pins in SEGMENT order, initial Boolean or one Boolean per segment, meaning, optional active_high.
The application expands it into io_i_segment_0, io_i_segment_1, ... Boolean digital outputs.
A wired LED bar graph IS supported through those individual GPIO ports; do not invent an integer-mask driver.

Serial reporting MUST use serial_channels rather than independent report_trigger/serial_value requests:
serial_channels[i] = {baud:explicit integer, trigger:{initial:Boolean,meaning:business report condition},
values:[{type:"bool"/"i32"/"f64",unit:string,initial:typed value,meaning:reported quantity,label:string}]}.
The program creates serial_i_trigger and serial_i_value_0, serial_i_value_1, ... and binds them automatically.
Never author a serial port name, trigger reference, or channel ID. Put values sharing one report condition in one group;
Serial labels are VERBATIM prefixes: the adapter emits Serial.print(label) followed by Serial.println(value).
It does not insert a colon or space. Preserve a required prefix such as "Humidity: " exactly in label.
Do not use serial_text_output for a numeric prefix: that operation prints a separate complete line.
use separate groups for DIFFERENT business report conditions. All groups use the SAME physical Serial baud.
For FIXED text lines use an interface_requests entry serial_text_output, NOT a fake numeric value with a label:
{"kind":"serial_text_output","initial":false,"meaning":"Report gate for the required message","baud":9600,"text":"Example message"}.
This program-owned Boolean output gates Serial.println of that exact fixed text. No number is appended.
Generate report cadence in the SysML model using actual elapsed_ms. Text is fixed binding data, not a model String.
Different fixed messages may have separate Boolean gates; all Serial operations must use the SAME baud.
For a compound line, serial_channels may also contain format, for example
"format":"X: {0:.2f}, Y: {1:.2f}, Z: {2:.2f} m/s^2". Value indices refer to that group's values.
Supported format specs: d, .0f through .6f, iso (YYYY-MM-DDTHH:MM:SS), rtc (YYYY/M/D (weekday) H:M:S),
date (YYYY-MM-DD), weekday (full English weekday). The program validates formats and renders the MODEL output values.
For example a single i32 seconds value can use format="Current time: {0:rtc}" or format="D-{0:d}".
All comparisons, timezone offsets, day navigation, counters and reporting gates belong in SysML.
Serial receive uses interface_requests, NEVER serial_channels:
serial_command_input: initial integer code, meaning, explicit baud, commands mapping exact ASCII tokens to integer codes.
Example: {"kind":"serial_command_input","initial":0,"meaning":"Received command: ON=1, OFF=2","baud":9600,"commands":{"ON":1,"OFF":2}}.
serial_integer_input: initial integer, meaning, explicit baud, min/max inclusive integer bounds.
Example: {"kind":"serial_integer_input","initial":0,"meaning":"Received brightness command","baud":9600,"min":0,"max":255}.
The program creates io_i (i32 value) AND io_i_valid (Boolean input). Apply a new command only when io_i_valid.
Receive values hold the last valid value; io_i_valid is true for ONE model step. Interpret commands/control actuators in SysML.
One receiver on physical Serial is supported, with the SAME baud as any outgoing reporting; D0/D1 are reserved.
Framing defaults to CR/LF lines or 20ms idle completion (framing="line_or_idle", idle_timeout_ms=20).
If explicit line framing is required, use framing="line". Frames are at most 63 bytes; command/numeric/date
receivers trim surrounding spaces/tabs. serial_string_input preserves all non-delimiter frame bytes, including whitespace.
Invalid/overlong/out-of-range frames produce no valid event. Record unspecified framing as a planning assumption.
Do not invent serial reporting for a display-only requirement. Reporting periods are NOT adapter timers.

The source requirement inventory is retained by the application; do not duplicate it or invent assertion IDs.
Optional required_inputs on an OUTPUT request, trigger or serial value lists actual MODEL input names
(io_i or the automatically registered elapsedMilliseconds), never states, outputs or device labels.
Only specify a mandatory dependency when the original requirement actually requires it; report timing belongs
on the trigger, not on the reported sensor value. Displaying a mode ID need not depend on elapsed time.
Uno supports D0..D19, A0..A5, PWM D3/D5/D6/D9/D10/D11; Mega supports D0..D69, A0..A15,
PWM D2..D13/D44..D46. Do not rewire to hide resource conflicts or unsupported hardware.

Native I2C/SPI devices ARE supported; use these operations instead of substituting GPIO:
rtc_epoch: i32 seconds since Unix epoch; initial 0, sda_pin/scl_pin and address 104. DS1307 uses RTClib;
declare rtc_valid (Boolean initial false) with identical bus pins, address and sample_period_ms.
Both RTC requests accept acquisition="once" for an actual single setup-time read, or "periodic" (default).
Use once when the requirement says to read RTC only once/in setup; preserve the captured epoch in model decisions.
RTC is read, never adjusted from a test/example timestamp. Supported RTC epoch range is 2000-01-01 through 2038-01-19.
Derive hour/minute using INTEGER divisions and remainder a-(a/b)*b; executable SysML has no percent/modulo operator.
Use i32 constants for calendar comparisons.
mpu_acceleration: f64 m/s^2, axis x/y/z; mpu_gyro: f64 rad/s, axis x/y/z;
mpu_gyro_raw: i32 raw, axis y, MPU6050 native signed 16-bit output at 250 deg/s full scale.
All require sda_pin/scl_pin, address 104 or 105; declare mpu_valid (Boolean initial false).
All quantities from one sensor must share a sample_period_ms. Never fake sensor success or replace readings with initial values.
I2C pins: Uno SDA=A4/SCL=A5; Mega SDA=D20/SCL=D21. Different GND terminals on the SAME board share ground.

lcd_text/lcd_number/lcd_datetime: native ILI9341 outputs. All require cs_pin,dc_pin,mosi_pin,miso_pin,sck_pin;
rst_pin defaults -1 if not wired. SPI pins: Uno MOSI=D11/MISO=D12/SCK=D13; Mega D51/D50/D52.
Common optional fields: rotation (default1), color (default65535 white), background(default0 black),
text_size(default2, datetime3), x/y(default0). Same screen fields must share rotation/background.
lcd_text is i32 selecting messages:{"0":"Not pressed!","1":"Pressed!"}; these are fixed labels, model chooses the code.
lcd_number is f64, optional unit for the displayed physical quantity; format default "{0:.2f}", e.g. "Humidity: {0:.2f}".
lcd_datetime is i32 Unix seconds with format "{0:date}" or "{0:weekday}"; the MODEL applies selected day/timezone offsets.
One request per text field; all fields sharing a screen redraw together when any model value changes.
Business numeric conversion is in SysML; the adapter only formats the already computed output.

seven_segment_number: native direct 1-digit or multiplexed 4-digit display output i32.
segment_pins lists exactly seven MCU pins for A..G. digit_pins is [] for single-digit or four pins in left-to-right order.
common_anode, digit_active_high, leading_zero are Booleans. Choose control_period_ms <=5 for four-digit multiplex refresh.
The adapter handles electrical segment codes and scan; model supplies the whole number 0..9 or 0..9999.
Colon is an independent digital_output if separately wired. Do not model segment encoding as application state.

serial_string_input: i32 sequence counter (initial0) plus automatic io_i_valid. Bounded printable ASCII frame (63 bytes),
baud/framing/idle_timeout_ms as existing receivers. The immutable received text buffer stays in the adapter;
lcd_text may use source_input:"io_i" instead of messages, with the model output forwarding the accepted frame counter.
serial_echo is an i32 frame-counter output with baud and source_input:"io_i"; changed positive counter prints that received text once.
serial_datetime_input: i32 Unix seconds (initial0, unit s), same receive framing; parses arbitrary valid ISO YYYY-MM-DDTHH:MM:SS,
calendar-valid dates in 2000..2037. Validity is a one-step input; invalid dates/overflow/partial frames never become valid events.
Only ONE serial receiver per physical Serial. Never map example dates/strings to hardcoded tokens.
serial_packet_input: i32 initial0, explicit baud, commands:{"TIME":1,"SET1":2,...}, min/max payload bounds
within0..9999, framing and idle_timeout_ms as other receivers. Accepts COMMAND INTEGER frames, e.g. TIME 930.
Publishes packed value command_code*10000+payload and io_i_valid. In SysML use i32 division to get code=value/10000
and payload=value-(value/10000)*10000; no percent operator. App-specific scheduling, restocking, HHMM validation,
and command effects remain model logic. Do not reinterpret a frozen external command grammar; this transport is
for requirements/bench conditions that explicitly select COMMAND INTEGER framing.
'''

def requires_clock(requirement):
    requirement = requirement.split('\nOperational acceptance profile ', 1)[0]
    if re.search(r'\b(?:every|each)\s+(?:\d+(?:\.\d+)?[\s-]*)?(?:milliseconds?|ms|seconds?|secs?|s|minutes?|mins?)\b',requirement,re.I):
        return True
    # Timing descriptions often use elapsed-time language without saying
    # "every N seconds"; those behaviors still require the registered clock.
    if re.search(r'elapsed\s+(?:time|seconds?)|according\s+to\s+(?:the\s+)?elapsed\s+time|(?:after|between)\s+\d+(?:\s*[-–]\s*\d+)?\s*seconds?|(?:blink|flashing|toggle).{0,80}(?:interval|second|repeat)|(?:on|off)\s+for\s+\d+(?:\.\d+)?\s*seconds?', requirement, re.I):
        return True
    explicit_period = r'(?:每\s*(?:\d+(?:\.\d+)?\s*(?:ms|毫秒|秒|分钟|s)|秒|分钟)|every\s+(?:\d+(?:\.\d+)?\s+)?(?:milliseconds?|seconds?|minutes?))[^。\n.]{0,40}(?:递增|显示|打印|切换|increment|display|print|switch|toggle)'
    repeat = public_match(r'长按[^。\n；;，,不禁未]{0,12}重复|auto.?repeat', requirement)
    return bool(repeat or re.search(explicit_period + r'|每[^。\n]{0,30}(?:闪烁|翻转|报告|报警|响|输出)|every[^.\n]{0,40}(?:blink|toggle|report)|渐亮|渐暗|渐灭|渐变|呼吸|triangle.wave|fading|fade', requirement, re.I))


def has_only_response_deadline(requirement):
    """Recognize deadline-only timing conservatively; unknown timing stays unknown.

    This is an exclusion check for unnecessary clock ports, not a general natural
    language specification parser. Explicit clock/timing instructions override it.
    """
    amount = r'\d+(?:\.\d+)?\s*(?:milliseconds?|ms|毫秒|秒|s)'
    deadline = r'(?:响应(?:时间)?(?:不超过|不大于)|response\s+(?:time\s+)?(?:within|at\s+most|no\s+more\s+than))\s*'+amount+'|'+amount+r'\s*内(?:更新|响应|反映)'
    remainder, count = re.subn(deadline, '', requirement, flags=re.I)
    if not count:
        return False
    # Each button press is an event, not a periodic clock. Explicit duration,
    # delay, waveform or accumulated-time language below still blocks exclusion.
    remainder = re.sub(r'每(?:次|个)按下(?:沿)?', '', remainder)
    timing = r'\d+(?:\.\d+)?\s*(?:ms|milliseconds?|seconds?|minutes?|秒|分钟|毫秒)|每|周期|定时|计时|延时|持续|累[计积]|渐|呼吸|平滑|elapsed|timer|duration|period|interval|delay|wait|clock|integrat|derivative|smooth|ramp|slew|滤波|积分|微分'
    return not re.search(timing, remainder, re.I)


def public_match(pattern, requirement):
    """Do not turn a negated or conditional prose fragment into an obligation."""
    for match in re.finditer(pattern,requirement,re.I):
        start=max([requirement.rfind(c,0,match.start()) for c in '\n。；;.!?'])+1
        ends=[pos for c in '\n。；;.!?' if (pos:=requirement.find(c,match.end()))>=0]
        end=min(ends) if ends else len(requirement)
        prefix=requirement[max(start,match.start()-24):match.start()]
        clause=requirement[start:end]
        if re.search(r'不|无需|不要|未要求|并非|\b(?:not|never|no)\b',prefix,re.I):
            continue
        if re.search(r'除非|除.*外|\b(?:unless|except)\b',clause,re.I):
            continue
        return match
    return None


def add_public_behavior_checks(contract, requirement):
    """Derive a small, quoted public obligation; never read a hidden test trace.

    Unrecognized requirements add no assertion and are not considered verified.
    Hardware initial values retain their prior meaning; the extra check explicitly
    covers a model step. Supplied assertions are preserved, including conflicts.
    """
    from app.services.toggle_contract import add_toggle_checks
    contract = add_toggle_checks(contract, requirement)
    from app.services.source_behavior import add_source_checks
    contract = add_source_checks(contract, requirement)
    from app.services.numeric_contract import add_numeric_checks
    contract = add_numeric_checks(contract, requirement)
    match = public_match(r'初始(?:运行且)?静音|initially\s+(?:running\s+(?:and|but)\s+)?silent',requirement)
    tone = [p for p in contract['interfaces'] if p.get('binding',{}).get('kind')=='tone_output']
    if len(tone)!=1:
        return contract
    result = copy.deepcopy(contract)
    if match:
        checks = result.setdefault('startup_checks',[])
        if any(c['output']==tone[0]['name'] and c['expected']!=0 for c in checks):
            raise ValueError('Supplied startup assertion contradicts explicit initial silence: '+match[0])
        assertion = dict(output=tone[0]['name'],expected=0,evidence=match[0])
        if assertion not in checks:
            checks.append(assertion)
    buttons = [p for p in contract['interfaces'] if p.get('binding',{}).get('kind')=='digital_input']
    clocks = [p for p in contract['interfaces'] if p.get('binding',{}).get('kind')=='elapsed_ms']
    running = public_match(r'初始运行|initially\s+running',requirement)
    paused = public_match(r'初始暂停|initially\s+paused',requirement)
    press_toggle = public_match(r'按下沿切换暂停/恢复|press[ -]edge\s+toggles?\s+pause/resume',requirement)
    silence = public_match(r'暂停立即静音|pause\s+(?:immediately\s+)?silences',requirement)
    if len(buttons)==len(clocks)==1 and bool(running)!=bool(paused) and press_toggle and silence:
        assertion = dict(input=buttons[0]['name'],delta=clocks[0]['name'],output=tone[0]['name'],
                         initial_running=bool(running),expected=0,
                         evidence='; '.join(m[0] for m in (running or paused,press_toggle,silence)))
        tolerances=re.findall(r'(?:响应[^。；;\n]{0,16}容差|response\s+tolerance)\s*(\d+)\s*ms',requirement,re.I)
        assertion['response_tolerance_ms']=min(map(int,tolerances)) if tolerances else 0
        checks = result.setdefault('pause_checks',[])
        same_output = [c for c in checks if c['output']==assertion['output']]
        if any(any(c[k]!=v for k,v in assertion.items() if k not in ('evidence','response_tolerance_ms')) for c in same_output):
            raise ValueError('Supplied pause assertion conflicts with explicit press-edge pause/silence requirements')
        for c in same_output:
            c.setdefault('response_tolerance_ms',assertion['response_tolerance_ms'])
        if not same_output:
            checks.append(assertion)
    # Add only a fully specified, quoted public pulse train. No guessed pitch,
    # interval, tolerance, pause semantics, or hidden hardware oracle is used.
    def single(pattern):
        matches=list(re.finditer(pattern,requirement,re.I))
        match=public_match(pattern,requirement)
        return match if len(matches)==1 and match is not None else None
    repeat=single(r'此后每(?:累计运行)?\s*(\d+)\s*ms\s*开始一次|then every\s+(\d+)\s*ms\s+of active time')
    first=single(r'累计运行\s*(\d+)\s*ms\s*后首次发声|第一次在启动后\s*(\d+)\s*ms\s*开始|first pulse after\s+(\d+)\s*ms\s+of active time')
    width=single(r'每次持续\s*(\d+)\s*ms|each pulse lasts\s+(\d+)\s*ms')
    pitch=single(r'音调\s*(\d+)\s*Hz|pitch\s+(\d+)\s*Hz')
    tolerance=single(r'(?:响应及鸣叫|开始及结束)时间容差\s*(\d+)\s*ms|timing tolerance\s+(\d+)\s*ms')
    preserve=public_match(r'保留计时和剩余鸣叫时长|preserve phase and remaining pulse duration',requirement)
    ready=len(clocks)==1 and all((repeat,first,width,pitch,tolerance))
    if buttons and not (len(buttons)==1 and bool(running)!=bool(paused) and press_toggle and silence and preserve):
        ready=False
    if ready:
        amount=lambda m:int(next(g for g in m.groups() if g is not None))
        if amount(first)==amount(repeat):
            assertion=dict(delta=clocks[0]['name'],output=tone[0]['name'],period_ms=amount(repeat),pulse_ms=amount(width),
                           first_pulse='after_period',active_value=amount(pitch),inactive_value=0,tolerance_ms=amount(tolerance),
                           evidence='; '.join(m[0] for m in (repeat,first,width,pitch,tolerance)))
            if buttons:
                assertion.update(pause_input=buttons[0]['name'],initial_running=bool(running),pause_policy='preserve')
                assertion['evidence']+='; '+ '; '.join(m[0] for m in (running or paused,press_toggle,silence,preserve))
            checks=result.setdefault('periodic_checks',[])
            same=[c for c in checks if c['output']==assertion['output']]
            if any(any(c.get(k)!=v for k,v in assertion.items() if k!='evidence') for c in same):
                raise ValueError('Supplied periodic assertion conflicts with explicit public timing requirements')
            if not same:checks.append(assertion)
    return result


def requested_board(requirement, target_board=None):
    if target_board is not None:
        board_profile(target_board)
        return target_board
    if re.search(r'\b(?:arduino[- ]+)?mega\b', requirement, re.I):
        return 'arduino:avr:mega'
    if re.search(r'\buno\b', requirement, re.I):
        return 'arduino:avr:uno'
    return None


def _wiring_sources(requirement, pins):
    text = requirement + '\n' + json.dumps(pins, ensure_ascii=False)
    decoder = json.JSONDecoder()
    circuits, prose = [], []
    end = 0
    for start in re.finditer(r'\{', text):
        if start.start() < end:
            continue
        try:
            circuit, length = decoder.raw_decode(text[start.start():])
        except ValueError:
            continue
        if not isinstance(circuit, dict) or not isinstance(circuit.get('parts'), list) or not isinstance(circuit.get('connections'), list):
            continue
        circuits.append(circuit)
        prose.append(text[end:start.start()])
        end = start.start() + length
    prose.append(text[end:])
    text = '\n'.join(prose)
    # Qualified component terminals are not Arduino pins, including in prose.
    for circuit in circuits:
        for part in circuit['parts']:
            if (isinstance(part, dict) and isinstance(part.get('id'), str) and part['id']
                    and part.get('type') not in ('wokwi-arduino-mega', 'wokwi-arduino-uno')):
                text = re.sub(r'(?<![A-Za-z0-9_])' + re.escape(part['id']) + r':[A-Za-z0-9_.]+', '', text)
    return text, circuits


def _declared_pins(text, circuits):
    # Unqualified A1..A10 in bar-graph prose name component segments when a diagram supplies MCU wiring.
    if any(p.get('type') == 'wokwi-led-bar-graph' for c in circuits for p in c['parts'] if isinstance(p, dict)):
        def segment_labels(fragment):
            def label(pin):
                prefix = fragment[max(0, pin.start()-60):pin.start()]
                if re.search(r'(?:mega|uno|arduino)\s*(?:pin|引脚|:)?\s*$|'
                             r'(?:potentiometer|ADC|analog|电位器|模拟|sensor|传感器)\s*'
                             r'(?:pin|引脚|input|输入|on|at|连接到?|[:=（(])?\s*$', prefix, re.I):
                    return pin[0]
                return 'segment'
            return re.sub(r'(?<![A-Za-z0-9_])A(?:10|[0-9])(?![A-Za-z0-9_])', label, fragment)
        paragraphs = re.split(r'(\n\s*\n)', text)
        for i, paragraph in enumerate(paragraphs):
            scope = re.search(r'(?:bar[ -]?graph\w*|segments?|LEDs?)\b', paragraph, re.I)
            if scope:
                paragraphs[i] = paragraph[:scope.start()] + segment_labels(paragraph[scope.start():])
        text = ''.join(paragraphs)
    declared = set(re.findall(r'(?<![A-Za-z0-9_])(?:D[0-9]{1,2}|A[0-9]{1,2})(?![A-Za-z0-9_])', text))
    for circuit in circuits:
        board_ids = {p['id'] for p in circuit['parts'] if isinstance(p, dict) and isinstance(p.get('id'), str)
                     and p.get('type') in ('wokwi-arduino-mega', 'wokwi-arduino-uno')}
        endpoints = [endpoint for connection in circuit['connections'] if isinstance(connection, list)
                     for endpoint in connection[:2] if isinstance(endpoint, str)]
        for board_id in board_ids:
            endpoints.extend(board_id + ':' + m[1] for m in re.finditer(
                r'(?<![A-Za-z0-9_])' + re.escape(board_id) + r':([DA]?[0-9]+)(?![A-Za-z0-9_])', text))
        for endpoint in endpoints:
            board_id, _, name = endpoint.partition(':')
            if board_id in board_ids and re.fullmatch(r'[DA]?[0-9]+', name):
                declared.add('D' + name if name.isdigit() else name)
    return declared


def confirmed_source_wiring(requirement, pins):
    """Only unique source-supported roles; ambiguous multi-device wiring stays explicit."""
    text, circuits = _wiring_sources(requirement, pins)
    facts = {}
    def add(kind, field, value):
        facts.setdefault(kind, {}).setdefault(field, set()).add(value.upper() if isinstance(value, str) else value)
    # A pin inventory does not establish ownership by a particular device.
    # Confirm roles only from explicit assignments or actual circuit endpoints.
    declared = _declared_pins(text, circuits)
    role_names = {
        'analog_input': r'potentiometer|ADC|电位器|模拟输入',
        'digital_input': r'button|按钮',
        'digital_output': r'LED|指示灯',
        'pwm_output': r'PWM',
        'tone_output': r'buzzer|蜂鸣器',
        'servo_angle': r'servo|舵机',
    }
    for kind, names in role_names.items():
        pins_for_role = [p for p in declared if p.startswith('A' if kind == 'analog_input' else 'D')]
        if len(pins_for_role) != 1:
            continue
        pin = re.escape(pins_for_role[0])
        role = r'(?<![A-Za-z])(?:' + names + r')(?![A-Za-z])'
        endpoint = r'(?<![A-Za-z0-9_])' + pin + r'(?![A-Za-z0-9_])'
        if re.search(role + r'\s*(?:[（(]\s*)?(?:(?:on|at|pin|引脚|接|连接到|为|:|=)\s*)?' + endpoint,
                     text, re.I) or re.search(endpoint + r'\s*(?:[）)]\s*)?(?:引脚\s*)?' + role, text, re.I):
            add(kind, 'pin', pins_for_role[0])
    if re.search(r'TM1637', requirement, re.I):
        for label, field in [('CLK', 'clk_pin'), ('DIO', 'dio_pin')]:
            for value in re.findall(r'\b' + label + r'\s*[:=]\s*([DA][0-9]{1,2})\b', requirement, re.I):
                add('tm1637_number', field, value)
    roles = {'wokwi-potentiometer': ('analog_input', {'SIG': 'pin'}),
             'wokwi-servo': ('servo_angle', {'PWM': 'pin'}),
             'wokwi-tm1637-7segment': ('tm1637_number', {'CLK': 'clk_pin', 'DIO': 'dio_pin'})}
    for circuit in circuits:
        parts = {p.get('id'): p.get('type') for p in circuit['parts'] if isinstance(p, dict)}
        for connection in circuit['connections']:
            if not isinstance(connection, list) or len(connection) < 2 or not all(isinstance(x, str) for x in connection[:2]):
                continue
            for device, board in [connection[:2], connection[:2][::-1]]:
                device_id, _, terminal = device.partition(':')
                board_id, _, pin_name = board.partition(':')
                if parts.get(board_id) not in ('wokwi-arduino-uno', 'wokwi-arduino-mega') or not re.fullmatch(r'[DA]?[0-9]+', pin_name):
                    continue
                role = roles.get(parts.get(device_id))
                if role and terminal in role[1]:
                    add(role[0], role[1][terminal], 'D' + pin_name if pin_name.isdigit() else pin_name)
    return {kind: {field: next(iter(values)) for field, values in fields.items() if len(values) == 1}
            for kind, fields in facts.items()}


def prepare_interface_contract(proposal, requirement, pins, *, target_board=None, confirmed=None, resolution=None):
    requirement = requirement.split('\nOperational acceptance profile ', 1)[0]
    # Source facts are recomputed at the shared API/planner boundary, never supplied by the model.
    sampling_specified = re.search(r'采样|sampling|sample[_ -]?period', requirement, re.I)
    configuration_actions = []
    source = resolution.get('original_requirement', requirement) if resolution else requirement
    numeric_serial=re.search(r'串口输入\s*(-?\d+)\s*[~～至到]\s*(-?\d+)\s*[：:]',source)
    if numeric_serial and isinstance(proposal,dict) and isinstance(proposal.get('interface_requests'),list):
        proposal=copy.deepcopy(proposal)
        for request in proposal['interface_requests']:
            if not isinstance(request,dict) or request.get('kind')!='serial_command_input':continue
            commands=request.get('commands',{})
            if isinstance(commands,dict) and all(isinstance(k,str) and re.fullmatch(r'-?\d+',k)
                    and type(v) is int and int(k)==v for k,v in commands.items()):
                request['kind']='serial_integer_input'
                request.pop('commands',None)
                request.update(min=int(numeric_serial[1]),max=int(numeric_serial[2]))
                configuration_actions.append('source_numeric_serial: integer range decoder replaces identity token enumeration')
    binary_messages=re.search(r'Continuously check the button value, and print "([^"\n]+)" if the value is HIGH, or print "([^"\n]+)" if the value is LOW',source)
    if binary_messages and isinstance(proposal,dict) and isinstance(proposal.get('interface_requests'),list):
        requests=proposal['interface_requests']
        allowed={'digital_input','elapsed_ms','serial_text_output'}
        if (all(isinstance(r,dict) and r.get('kind') in allowed for r in requests)
                and sum(r['kind']=='digital_input' for r in requests)==1
                and set(re.findall(r'print "([^"\n]+)"',source))<=set(binary_messages.groups())):
            proposal=copy.deepcopy(proposal)
            proposal['interface_requests']=[r for r in proposal['interface_requests'] if r['kind']!='serial_text_output']
            baud=resolution['serial']['baud'] if resolution and resolution.get('serial') else 9600
            for level,text in zip(('HIGH','LOW'),binary_messages.groups()):
                proposal['interface_requests'].append(dict(kind='serial_text_output',initial=False,baud=baud,text=text,
                    meaning='Exact state report gate for physical '+level+'; controller must report stable states, not only edges'))
            proposal['serial_channels']=[]
            configuration_actions.append('source_binary_messages: assembled two exact state-message gates; no appended Boolean payload')
    startup_servos = re.search(r'Initialization:[^\n]*both servos[^\n]*their 0-degree position',source,re.I)
    if startup_servos and isinstance(proposal,dict) and isinstance(proposal.get('interface_requests'),list):
        proposal=copy.deepcopy(proposal)
        for request in proposal['interface_requests']:
            if isinstance(request,dict) and request.get('kind')=='servo_angle':
                request['initial']=0
                configuration_actions.append('source_servo_startup: zero-degree power-on command; cycle reset remains model behavior')
    from app.services.source_behavior import startup_message
    message=startup_message(source)
    if message and isinstance(proposal,dict) and isinstance(proposal.get('interface_requests'),list):
        proposal=copy.deepcopy(proposal)
        channels=proposal.get('serial_channels',[])
        if isinstance(channels,list):
            proposal['serial_channels']=[c for c in channels if not (isinstance(c,dict) and
                isinstance(c.get('values'),list) and len(c['values'])==1 and isinstance(c['values'][0],dict) and c['values'][0].get('label')==message)]
        if not any(isinstance(r,dict) and r.get('kind')=='serial_text_output' and r.get('text')==message for r in proposal['interface_requests']):
            baud=resolution['serial']['baud'] if resolution and resolution.get('serial') else 9600
            proposal['interface_requests'].append(dict(kind='serial_text_output',initial=False,baud=baud,text=message,
                meaning='Exact source-required startup text gate; emit before subsequent startup operations, no numeric payload'))
        configuration_actions.append('source_startup_text: exact fixed message replaces a fake numeric report; model retains startup ordering')
    fixed_message = re.fullmatch(r'\s*每\s*(\d+)\s*秒向串口监视器输出一次\s+([^。\n]+)[。]?\s*', source)
    if fixed_message and isinstance(proposal, dict) and isinstance(proposal.get('interface_requests'), list):
        requests = proposal['interface_requests']
        if all(isinstance(r, dict) and r.get('kind') in ('elapsed_ms','serial_text_output') for r in requests):
            # This exact source form specifies all output data. Assemble its
            # fixed-text boundary instead of asking the LLM to invent a payload.
            proposal = copy.deepcopy(proposal)
            proposal['interface_requests'] = [r for r in proposal['interface_requests'] if r['kind']=='elapsed_ms']
            proposal['interface_requests'].append(dict(kind='serial_text_output',initial=False,
                baud=9600,text=fixed_message[2].strip(),meaning='Source-required fixed message gate'))
            proposal['serial_channels'] = []
            configuration_actions.append('source_fixed_message: assembled exact literal, no numeric payload')
    if resolution is not None:
        from app.services.configuration_policy import apply_configuration
        proposal, resolved_actions = apply_configuration(proposal, resolution)
        configuration_actions += resolved_actions
    from app.services.wiring_inventory import circuit_inventory, restore_circuit_fields
    _, circuits = _wiring_sources(requirement, pins)
    if isinstance(proposal, dict) and circuits:
        proposal = copy.deepcopy(proposal)
        circuit_board = requested_board(requirement, target_board) or proposal.get('board', 'arduino:avr:uno')
        proposal, restored = restore_circuit_fields(proposal, circuit_inventory(circuits, circuit_board), circuit_board)
        configuration_actions += restored
    candidate, actions = normalize_interface_proposal(proposal, allow_default_sampling=not sampling_specified,
        repair_catalog_fields=True,
        allow_default_initial=not re.search(r'初始|上电|启动时|\binitial(?:ly)?\b|\bstartup\b', requirement, re.I))
    actions = configuration_actions + actions
    facts = confirmed_source_wiring(requirement, pins)
    for kind, fields in (confirmed or {}).items():
        for field, value in fields.items():
            facts.setdefault(kind, {}).setdefault(field, value)
    requests = candidate.get('interface_requests', []) if isinstance(candidate, dict) else []
    sensor_period = re.fullmatch(r'\s*每\s*(\d+)\s*秒读取\s*(DHT11|DHT22)（D\d+）温度并串口输出[。]?\s*', source, re.I)
    if sensor_period:
        for request in requests:
            if isinstance(request,dict) and request.get('kind') in ('dht_temperature','dht_valid'):
                expected = int(sensor_period[1])*1000
                if request.get('sample_period_ms') != expected:
                    request['sample_period_ms'] = expected
                    actions.append('source_sensor_period: acquisition uses explicit read-and-report interval')
    expected_board = requested_board(requirement, target_board)
    board = expected_board or (candidate.get('board', 'arduino:avr:uno') if isinstance(candidate, dict) else 'arduino:avr:uno')
    for request in requests if isinstance(requests, list) else []:
        if not isinstance(request, dict):
            continue
        kind = request.get('kind')
        if not isinstance(kind, str):
            continue
        if sum(isinstance(r, dict) and r.get('kind') == kind for r in requests) != 1:
            continue
        for field, value in facts.get(kind, {}).items():
            if field in request and physical_pin(request[field], board) != physical_pin(value, board):
                raise ValueError(kind + '.' + field + ' contradicts confirmed wiring ' + str(value) + '; preserve the source connection')
            if field not in request:
                request[field] = value
                actions.append(kind + '.' + field + ': restored confirmed source pin ' + str(value))
    contract, assembled = assemble_interface_contract(candidate, board=expected_board,
        require_clock=requires_clock(requirement), exclude_clock=has_only_response_deadline(requirement))
    if facts and isinstance(candidate, dict) and 'interface_requests' in candidate:
        kinds = {r['kind'] for r in requests if isinstance(r, dict) and isinstance(r.get('kind'), str)}
        contract['wiring_snapshot'] = {k: v for k, v in facts.items() if k in kinds and v}
    if resolution is not None:
        contract['configuration_resolution'] = resolution
    return contract, actions + assembled


def check_explicit_requirements(contract, requirement, pins, *, target_board=None):
    """Conservative electrical/deadline obligations from explicit source text.

    This does not claim to understand all natural-language behavior. Independent
    trace tests remain authoritative for state, timing and requirement coverage.
    """
    requirement = requirement.split('\nOperational acceptance profile ', 1)[0]
    text, circuits = _wiring_sources(requirement, pins)
    board = contract.get('board', 'arduino:avr:uno')
    expected_board = requested_board(requirement, target_board)
    if expected_board is not None and board != expected_board:
        raise ValueError('Requested board ' + expected_board + ' differs from contract board ' + str(board))
    board_profile(board)
    declared = _declared_pins(text, circuits)
    def physical(value):
        return physical_pin(value, board)
    actual = {physical(value) for p in contract['interfaces'] for value in binding_pins(p['binding'])}
    if any(p['binding']['kind'] in ({'serial_value', 'serial_text_output', 'serial_echo'} | SERIAL_INPUTS) for p in contract['interfaces']):
        actual |= {0, 1}
    bounds = re.findall(r'(?:容差|响应时间不超过)\s*(\d+)\s*ms', requirement, re.I)
    bounds += re.findall(r'变化\s*(\d+)\s*ms\s*内反映',requirement,re.I)
    if bounds and contract['control_period_ms'] > min(map(int, bounds)):
        raise ValueError('Control period exceeds the explicit response/timing tolerance; business periods belong in SysML state, not the adapter scheduler.')
    from app.services.ultrasonic import groups as ultrasonic_groups
    ultrasonic=ultrasonic_groups(contract)
    # Narrow, explicit device-specific ranges; never apply the DHT interval to
    # sonar or use a buzzer tolerance as a sensor sampling period.
    distance_interval=public_match(r'距离采样开始间隔\s*(\d+)\.\.(\d+)\s*ms',requirement)
    if distance_interval:
        low,high=map(int,distance_interval.group(1,2))
        if any(not low<=p['sample_period_ms']<=high for _,p,_ in ultrasonic):
            raise ValueError('Distance sampling period must preserve the explicit '+str(low)+'..'+str(high)+'ms interval')
    dht_interval=public_match(r'DHT采样开始间隔\s*(\d+)\.\.(\d+)\s*ms',requirement)
    if dht_interval:
        low,high=map(int,dht_interval.group(1,2))
        if any(not low<=p['sample_period_ms']<=high for p in contract['interfaces'] if p['binding']['kind'].startswith('dht_')):
            raise ValueError('DHT sampling period must preserve the explicit '+str(low)+'..'+str(high)+'ms interval')
    blocking_ms=sum((d['binding']['timeout_us']+12+999)//1000 for _,d,_ in ultrasonic)
    if bounds and blocking_ms>min(map(int,bounds)):
        raise ValueError('HC-SR04 bounded blocking acquisition may take '+str(blocking_ms)+
                         'ms, exceeding the explicit timing tolerance. This adapter cannot promise that deadline; '
                         'a separately validated nonblocking acquisition is required. Do not relax the requirement or fabricate elapsed time.')
    # A buzzer edge tolerance or display page tolerance constrains scheduling,
    # not the age of a newly sampled sensor value. Only explicit input-response
    # deadlines constrain acquisition period + transport. Keep the original
    # control/blocking checks above; do not rewrite one device's sample period
    # to satisfy an unrelated output's timing clause.
    sample_response_bounds = re.findall(r'响应时间不超过\s*(\d+)\s*ms',requirement,re.I)
    sample_response_bounds += re.findall(r'变化\s*(\d+)\s*ms\s*内反映',requirement,re.I)
    if sample_response_bounds:
        for _,distance,_ in ultrasonic:
            if distance['sample_period_ms']+blocking_ms>min(map(int,sample_response_bounds)):
                raise ValueError('HC-SR04 '+distance['name']+'.sample_period_ms plus bounded acquisition exceeds the '
                                 'explicit response deadline. Budget sampling, acquisition and output transfer; do not remove the deadline.')
    timeout=public_match(r'回波必须有(\d+)\s*(us|µs|μs|ms)超时',requirement)
    if timeout and len(ultrasonic)==1:
        expected=int(timeout[1])*(1000 if timeout[2]=='ms' else 1)
        if ultrasonic[0][1]['binding']['timeout_us']!=expected:
            raise ValueError('HC-SR04 timeout_us must match the explicit requirement: '+str(expected)+' microseconds')
    if requires_clock(requirement) and not any(p['binding']['kind']=='elapsed_ms' for p in contract['interfaces']):
        raise ValueError('Periodic control requires one elapsed_ms input (i32 ms, initial 0, delta_since_previous_step). Do not substitute a constant time increment for actual elapsed time.')
    if has_only_response_deadline(requirement) and any(p['binding']['kind']=='elapsed_ms' for p in contract['interfaces']):
        raise ValueError('Only a response deadline is specified, not model business timing. Use sampling/control periods for that deadline; remove the invented elapsed_ms interface and its required_dependencies BEFORE freezing. Preserve the original sensor/display behavior and deadline; never add a fake timer dependence to the model.')
    receives_serial = public_match(r'串口[^。\n]{0,35}(?:接收|输入)|串口(?:也)?支持[^。\n]{0,100}命令|(?:支持)?串口命令|(?:serial|UART)[^.\n]{0,35}(?:input|receive)', requirement)
    if receives_serial and not re.search(r'不|无需|禁止|\b(?:not|no|never)\b', receives_serial[0], re.I):
        receivers = [p for p in contract['interfaces'] if p['binding']['kind'] in SERIAL_INPUTS]
        if len(receivers) != 1:
            raise ValueError('Explicit serial receive requirement needs exactly one registered serial receiver; outgoing serial_channels cannot replace receiving')
        binding = receivers[0]['binding']
        baud_values = re.findall(r'(\d+)\s*(?:波特率|baud)|(?:baud(?:\s+rate)?\s*[:=]?\s*)(\d+)', requirement, re.I)
        if any(binding['baud'] != int(a or b) for a, b in baud_values):
            raise ValueError('Serial receive baud contradicts the explicit source baud')
        if re.search(r'\bON\b', requirement) and re.search(r'\bOFF\b', requirement):
            commands = binding.get('commands', {})
            if (binding['kind'] != 'serial_command_input' or 'ON' not in commands or 'OFF' not in commands or
                    commands['ON'] == commands['OFF']):
                raise ValueError('Explicit ON/OFF receive requirement needs distinct command codes for both exact tokens')
    if re.search(r'无源蜂鸣器|passive\s+buzzer', requirement, re.I) and not any(p['binding']['kind']=='tone_output' for p in contract['interfaces']):
        raise ValueError('An explicitly passive buzzer requires a tone_output (i32 Hz); a static digital output or PWM duty cannot replace the carrier.')
    if re.search(r'TM1637', requirement, re.I) and not any(p['binding']['kind']=='tm1637_number' for p in contract['interfaces']):
        raise ValueError('An explicit TM1637 display requires tm1637_number with CLK/DIO bindings; separate digital outputs do not implement its display protocol.')
    if public_match(r'HC[- ]?SR04',requirement) and not any(p['binding']['kind']=='ultrasonic_distance' for p in contract['interfaces']):
        raise ValueError('An explicit HC-SR04 requires paired ultrasonic_distance (f64 cm) and ultrasonic_valid (bool) inputs, with trigger_pin and echo_pin; digital inputs alone cannot replace distance acquisition.')
    for sensor in ('DHT11','DHT22'):
        if public_match(r'\b'+sensor+r'\b',requirement) and not any(p['binding']['kind'] in ('dht_temperature','dht_humidity') and p['binding'].get('sensor')==sensor for p in contract['interfaces']):
            raise ValueError('Explicit '+sensor+' requires matching DHT measurement inputs; do not replace the sensor type or use a digital/constant input.')
    serial_ports=[p for p in contract['interfaces'] if p['binding']['kind']=='serial_value']
    from app.services.source_behavior import check_source_messages
    check_source_messages(contract, requirement)
    for literal in re.finditer(r'原因包含\s*([A-Z][A-Z0-9_]+)',requirement):
        if not public_match(re.escape(literal[0]),requirement):
            continue
        token=literal[1]
        if not any(token in p['binding'].get('label','') for p in serial_ports):
            raise ValueError('Explicit serial reason token '+token+' must be preserved in a serial_value label with a model-owned Boolean trigger')
    baud=public_match(r'串口\s*(\d{3,7})\b',requirement)
    if baud and serial_ports and any(p['binding']['baud']!=int(baud[1]) for p in serial_ports):
        raise ValueError('Serial baud must match the explicit requirement: '+baud[1])
    displays = [p for p in contract['interfaces'] if p['binding']['kind'] in ('tm1637_number','shift595_number')]
    if public_match(r'初始显示空白',requirement) and len(displays)==1 and displays[0]['binding']['kind']=='tm1637_number':
        p=displays[0];low=0 if p['binding']['leading_zero'] else -999
        if low<=p['initial']<=9999:
            raise ValueError('Interface '+p['name']+'.initial must request blank (e.g. i32 10000 with invalid_policy=blank), '
                             'not a displayed zero, because the original requirement specifies initial blank. Preserve all sensor logic.')
    leading = public_match(r'保留前导零|显示\s*[“"\x27]?0000(?!\d)|preserv(?:e|ing)\s+leading\s+zeros?|leading\s+zeros?\s+(?:(?:must|should)\s+be\s+|are\s+)?(?:preserved|kept)', requirement)
    # A global phrase identifies the single numeric display. With several
    # displays, device-specific prose needs a separate binding of the obligation.
    if leading and len(displays)==1 and not displays[0]['binding']['leading_zero']:
        raise ValueError('Interface '+displays[0]['name']+'.binding.leading_zero must be true: '
                         'the original requirement explicitly says '+repr(leading[0])+'. '
                         'Preserve the full displayed number and every control requirement; do not change '
                         'the model value or remove the leading-zero requirement to pass this check.')
    for port in displays:
        binding = port['binding']
        if binding['kind'] != 'shift595_number' or len(binding['registers']) != 2:
            continue
        for register in binding['registers']:
            display = re.escape(register['display'])
            for position, term in enumerate((r'tens digit', r'(?:units|ones) digit')):
                assignment = rf'\b{term}\b\s+(?:should be displayed\s+)?on\s+(?:(?:one|the other)\s+(?:display\s*)?)?\(?{display}\b'
                if public_match(assignment, requirement) and register['digit'] != position:
                    raise ValueError(f'Explicit digit assignment for {register["display"]} requires digit={position}, '
                                     'where digit 0 is the tens position and digit 1 is the units position. '
                                     'Preserve the electrical cascade order.')
    if public_match(r'\bservos?\b|舵机', requirement) and not any(p['binding']['kind']=='servo_angle' for p in contract['interfaces']):
        raise ValueError('An explicit positional servo requires servo_angle outputs; digital/PWM duty outputs do not implement angle commands. Unsupported servo semantics must remain a planning failure.')
    # Explicit Wokwi servo wiring supplies physical obligations even when board
    # pins are written as "mega:2" rather than D2. No inferred control logic.
    for circuit in circuits:
        from app.services.wiring_inventory import check_circuit_inventory, circuit_inventory
        check_circuit_inventory(contract, circuit_inventory([circuit], board))
        from app.services.shift595 import check_circuit
        check_circuit(contract, circuit, physical)
        from app.services.ultrasonic import check_circuit as check_ultrasonic_circuit
        check_ultrasonic_circuit(contract,circuit,physical)
        from app.services.dht import check_circuit as check_dht_circuit
        check_dht_circuit(contract,circuit,physical)
        board_ids = {p.get('id') for p in circuit['parts'] if isinstance(p,dict) and p.get('type') in ('wokwi-arduino-mega','wokwi-arduino-uno')}
        servo_ids = {p.get('id') for p in circuit['parts'] if isinstance(p,dict) and p.get('type')=='wokwi-servo'}
        servo_pins = {physical(p['binding']['pin']) for p in contract['interfaces'] if p['binding']['kind']=='servo_angle'}
        for connection in circuit['connections']:
            if not isinstance(connection,list) or len(connection)<2 or not all(isinstance(x,str) for x in connection[:2]):
                continue
            for device, endpoint in [(connection[0],connection[1]),(connection[1],connection[0])]:
                device_id,_,signal=device.partition(':');board_id,_,pin_name=endpoint.partition(':')
                if device_id in servo_ids and signal=='PWM' and board_id in board_ids and physical(pin_name) not in servo_pins:
                    raise ValueError('Explicit servo wiring missing servo_angle on ' + endpoint + ' for ' + device_id)
    missing = [p for p in sorted(declared) if physical(p) not in actual]
    if missing:
        raise ValueError('Explicit requirement pins missing from bindings: ' + ', '.join(missing) + '. Example pins must not replace requested wiring.')
    for label,field in [('CLK','clk_pin'),('DIO','dio_pin')]:
        if not re.search(r'TM1637',requirement,re.I):
            continue
        declared_roles=re.findall(r'\b'+label+r'\s*[:=]\s*([DA][0-9]{1,2})\b',requirement,re.I)
        actual_roles={physical(p['binding'][field]) for p in contract['interfaces'] if p['binding']['kind'] in ('tm1637_number','tm1637_text')}
        if any(physical(value.upper()) not in actual_roles for value in declared_roles):
            raise ValueError('Explicit '+label+' wiring does not match TM1637 '+field+'; bus roles must not be swapped')
    for label,field in [('TRIG','trigger_pin'),('ECHO','echo_pin')]:
        if not re.search(r'HC[- ]?SR04',requirement,re.I):continue
        roles=re.findall(r'\b'+label+r'\s*[:=]\s*([DA][0-9]{1,2})\b',requirement,re.I)
        actual_roles={physical(p['binding'][field]) for p in contract['interfaces'] if p['binding']['kind']=='ultrasonic_distance'}
        if any(physical(value.upper()) not in actual_roles for value in roles):
            raise ValueError('Explicit '+label+' wiring does not match HC-SR04 '+field+'; sensor roles must not be swapped')


def plan_interface_contract(call, requirement, pins, *, previous=None, max_repairs=2, target_board=None,
                            resolve_defaults=True, configuration_resolution=None, diagnostic_feedback=None, repair_call=None):
    resolution = configuration_resolution
    if resolve_defaults and resolution is None:
        from app.services.configuration_policy import resolve_configuration
        resolution = resolve_configuration(requirement, pins, board=target_board)
    if resolution is not None:
        resolved = resolution['effective_requirement']
        # Preserve public conditions appended after configuration was resolved.
        if requirement.startswith(resolved):
            resolution = copy.deepcopy(resolution)
            resolution['effective_requirement'] = requirement
        else:
            requirement = resolved
        pins = resolution['pins']
    prompt = CONTRACT_PROMPT + '\nOriginal requirements:\n' + requirement + '\nSupplied pins:\n' + json.dumps(pins, ensure_ascii=False)
    expected_board = requested_board(requirement, target_board)
    if expected_board is not None:
        prompt += '\nRequired target board: ' + expected_board
    from app.services.wiring_inventory import circuit_inventory
    from app.services.interface_registry import CATALOG
    profile = board_profile(expected_board or 'arduino:avr:uno')
    prompt += '\nActual target PWM pins: ' + json.dumps(sorted(profile['pwm']))
    inventory = circuit_inventory(_wiring_sources(requirement, pins)[1], expected_board or 'arduino:avr:uno')
    if inventory:
        prompt += ('\nProgram-extracted per-component electrical facts (complete circuit endpoints, not a pin whitelist). '
                   'catalog_operations are supported request shapes, not model ports. Choose required quantities; '
                   'RGB channels use one scalar request per color; a bar graph uses its SEGMENT-ORDERED MCU pins, '
                   'not component terminal labels A1..A10. Ground/common pins do not need model ports. '
                   'An empty operation list means extraction has no known template, NOT that wiring is absent.\n' + json.dumps(inventory, ensure_ascii=False))
        loads = {}
        for device in inventory:
            if device['component'] not in ('wokwi-led', 'wokwi-led-bar-graph', 'wokwi-rgb-led'):
                continue
            for values in device['signals'].values():
                for pin in values:
                    loads.setdefault(physical_pin(pin, expected_board or 'arduino:avr:uno'), set()).add(device['device'])
        shared = {str(pin):sorted(names) for pin,names in loads.items() if len(names)>1}
        if shared:
            prompt += ('\nPhysically shared passive LED loads: '+json.dumps(shared)+
                       '\nDeclare ONE digital_output per shared MCU GPIO, not overlapping led_bar_graph requests. '
                       'All loads on that net observe the same signal; retain the original shifted/mirrored display relationship in SysML. '
                       'If the requirement demands incompatible independent values on one net, report that actual contradiction.')
    prompt += ('\nExact allowed request configuration keys by kind (do not copy fields across kinds):\n' +
               json.dumps({k: sorted(v[4]) for k, v in CATALOG.items()}) +
               '\nAdd initial and meaning to each request; device identifies the component, not a port. '
               'For led_bar_graph use pins, initial, meaning, active_high. '
               'Never use active_high on analog_input or pwm_output; their polarity is numeric scale/offset. '
               'Thresholds and control logic missing from prose are model questions, not missing electrical capability. '
               'Record unresolved business parameters in planning_notes; do not put them in unsupported. '
               'If serial state-report fields/labels are unspecified, choose typed reports of the required state and record the format assumption. '
               'Preserve any explicitly required exact text/protocol. Unspecified digital polarity uses the catalog default unless circuit facts establish it. '
               'A 10-segment component may have only 4/6/8 wired segments: use the actual wired segment list when the behavior uses that subset.')
    from app.services.shift595 import wiring_evidence
    wiring = wiring_evidence(requirement, lambda pin: physical_pin(pin, expected_board or 'arduino:avr:uno'))
    if wiring:
        prompt += ('\nVerified electrical facts extracted from the supplied circuit (these mappings and polarities ARE provided; '
                   'do not report them as missing or unsupported):\n'+json.dumps(wiring,ensure_ascii=False)+
                   '\nUse shift595_number for this static numeric display. Copy each segment_bits and active_low exactly. '
                   'Assign each register its digit position from the ORIGINAL display meaning (digit 0 is leftmost); '
                   'register chain order does not itself determine tens/units. Preserve requested leading zeros and behavior. '
                   'These facts do not implement counting, reset or button logic.')
    if has_only_response_deadline(requirement):
        prompt += ('\nOnly response-deadline timing was identified: implement the deadline with sampling/control periods. '
                   'Do not add elapsed_ms or a fake time dependency to event-driven arithmetic. '
                   'Preserve every button event and state update in SysML.')
    declared = sorted(_declared_pins(*_wiring_sources(requirement, pins)))
    prompt += ('\nAuthoritative pin names from explicit prose and supplied circuit BOARD endpoints (use these, even if extra pins is empty):\n' + json.dumps(declared) +
               '\nDevice terminals such as bargraph:A10 are component labels, not board analog pins. '
               'Preserve each original connection and its device role; this pin list does not authorize swapping devices or bus roles.')
    if requires_clock(requirement):
        prompt += ('\nThe requirement includes time-dependent behavior: an elapsed_ms input is REQUIRED. '
                   'The application registers elapsedMilliseconds when absent. You may instead request '
                   '{"kind":"elapsed_ms","initial":0,"meaning":"Actual delta since previous control step"} '
                   'to obtain an io_i time input. initial belongs outside binding.')
    if previous:
        prompt += '\nPrevious contract (update only for the changed requirements):\n' + json.dumps(previous, ensure_ascii=False)
    if diagnostic_feedback is not None:
        prompt += ('\nAudited diagnostics from the previous generation attempt (DATA, not new requirements). '
                   'Repair only the identified contract defects; preserve original requirements and authoritative wiring. '
                   'Never remove a required behavior or relax an acceptance check to make an error disappear.\n' +
                   json.dumps(diagnostic_feedback, ensure_ascii=False))
    current = prompt
    attempts = []
    confirmed = {}
    for _ in range(max_repairs + 1):
        active_call=repair_call if attempts and repair_call is not None else call
        model, raw = active_call(json_prompt(current))
        contract = None  # A malformed retry must not reuse the previous candidate's diagnostics.
        try:
            check_completion(raw)
            payload = re.sub(r'^\s*```(?:json)?\s*\n|\n```\s*$', '', raw.strip())
            contract = json.loads(payload)
            if isinstance(contract, dict) and contract.get('unsupported'):
                reasons = contract['unsupported']
                # A literal empty-status sentinel is not a claim that a driver is missing.
                empty = {'none', 'none.', 'none detected', 'none detected.', '无'}
                if isinstance(reasons, list) and all(isinstance(r, str) and r.strip().lower() in empty for r in reasons):
                    contract.pop('unsupported')
                else:
                    raise InterfaceCapabilityError('Unsupported hardware: ' + json.dumps(reasons, ensure_ascii=False))
            contract, construction = prepare_interface_contract(contract, requirement, pins,
                target_board=target_board, confirmed=confirmed, resolution=resolution)
            if 'requirements' not in contract:
                source_requirement = resolution['original_requirement'] if resolution is not None else requirement
                contract['requirements'] = [dict(id='R1', text=source_requirement, assertion_ids=['source_requirement_R1'])]
            validate_hardware_contract(contract)
            validate_sysml_contract(contract)
            check_explicit_requirements(contract, requirement, pins, target_board=expected_board)
            from app.services.feedback_regression import check_public_button_contract
            check_public_button_contract(contract, requirement)
            for port in contract['interfaces']:
                kind = port['binding']['kind']
                if sum(p['binding']['kind'] == kind for p in contract['interfaces']) == 1:
                    confirmed[kind] = {k: v for k, v in port['binding'].items() if k in ('pin', 'clk_pin', 'dio_pin')}
            if not any(p['direction']=='output' for p in contract['interfaces']):
                raise InterfaceCapabilityError('No executable model output registered; preserve the required device/serial output or report the hardware capability gap. Do not invent a dummy output.')
            contract = add_public_behavior_checks(contract, requirement)
            contract = freeze_model_semantics(contract)
            contract = register_contract(contract)
            validate_sysml_contract(contract)
            attempts.append({'ok': True, 'model': model, 'response': raw, 'construction': construction,
                             'completion': getattr(raw, 'completion', {})})
            return contract, {'contract_sha256': contract_digest(contract), 'provenance': 'program_assembled',
                              'interface_registry': contract['interface_registry'], 'registered_relationships': 'verified',
                              'functional_status': 'unverified', 'attempts': attempts,
                              'configuration_resolution': resolution}
        except (ValueError, TypeError, KeyError) as exc:
            from app.services.planning_feedback import independent_field_diagnostics
            field_diagnostics=independent_field_diagnostics(contract,requirement)
            # Renaming an unsupported port does not fix the same binding error.
            def error_key(message):
                return re.sub(r'Unsupported hardware binding for [^:]+: ([^;\n]+).*', r'Unsupported hardware binding: \1', message)
            repeated = bool(attempts and (error_key(attempts[-1].get('error','')) == error_key(str(exc)) or
                            (field_diagnostics and field_diagnostics==attempts[-1].get('field_diagnostics'))))
            attempts.append({'ok': False, 'model': model, 'response': raw, 'error': str(exc), 'no_progress': repeated,
                             'code': getattr(exc, 'code', 'CONTRACT_FORMAT'), 'completion': getattr(raw, 'completion', {}),
                             'field_diagnostics':field_diagnostics})
            if getattr(exc, 'code', None) == 'LLM_REFUSAL':
                break
            current = (prompt + '\nPrevious contract:\n' + raw + '\nRepair this contract without dropping requirements:\n' + str(exc) +
                       '\nConfirmed wiring (restore omitted fields; never change these connections):\n' + json.dumps(confirmed, ensure_ascii=False) +
                       '\nCorrect ONLY the fields and references identified above; return the complete JSON proposal. '
                       'Keep unaffected interfaces, devices, wiring, timing and requirements. '
                       'required_dependencies keys must be declared output names, and each value must be an array of declared input names, '
                       'never state names, output names, expressions or device labels. '
                       'Do not delete a required dependency or invent an input just to pass validation. '
                       'If the required hardware or wiring is unsupported or contradictory, report unsupported instead of substituting or rewiring it.')
            if field_diagnostics:
                current += ('\nIndependent field diagnostics: fix ALL listed fields together before retrying. '
                            'This list supplements the primary diagnostic; the complete contract will be validated again.\n'+
                            '\n'.join('- '+message for message in field_diagnostics))
            if 'serial receive requirement' in str(exc) or 'Explicit ON/OFF receive' in str(exc):
                current += ('\nSERIAL DIRECTION REPAIR: serial_channels are OUTPUT ONLY. Add an input in interface_requests. '
                            'For literal ON/OFF use {"kind":"serial_command_input","initial":0,"meaning":"Received ON/OFF command",'
                            '"baud":9600,"commands":{"ON":1,"OFF":2}}. Use the SOURCE baud when it differs from this example. '
                            'Keep the LED output request. Do not add acknowledgements or output channels unless required. '
                            'The application creates value and one-step valid inputs; persistent ON/OFF state belongs in the model.')
            if "missing fields ['label']" in str(exc):
                current += ('\nSERIAL VALUE SCHEMA REPAIR: each serial_channels[i].values[j] object must contain label '
                            'at the SAME level as type/unit/initial/meaning. Example: '
                            '{"type":"f64","unit":"degC","initial":0,"meaning":"Measured temperature","label":"Temperature: "}. '
                            'Preserve any exact SOURCE prefix. When the source does not specify a prefix, declare the chosen label explicitly; '
                            'do not omit it or move it into binding/optional. Do not confuse reporting with receiving.')
            if 'SOURCE_SENSOR' in str(exc):
                current += ('\nSENSOR QUANTITY REPAIR: native humidity and its serial numeric value BOTH use f64 %RH, '
                            'not %. Native temperature and its report use f64 degC. Keep the sensor type, pin, validity, '
                            'exact source label and unscaled measurement. A correct sensor input alone does not fix a wrong report unit.')
            if 'PWM capable pin' in str(exc):
                current += ('\nOUTPUT CAPABILITY REPAIR: preserve the wired pins. For a source requiring only fully ON/OFF '
                            '(including full brightness), use digital_output, not PWM on a non-PWM pin. '
                            'Preserve source/common-anode polarity. For genuinely variable brightness on unsupported pins, '
                            'report the capability gap instead of rewiring or silently replacing brightness with ON/OFF.')
            if 'SOURCE_FIXED_TEXT' in str(exc) or 'values must contain 1..128' in str(exc):
                current += ('\nFIXED TEXT REPAIR: if the source requires only a fixed message, use interface_requests '
                            '{"kind":"serial_text_output","baud":9600,"text":"EXACT SOURCE MESSAGE",'
                            '"initial":false,"meaning":"Model-controlled message gate"}; use the source baud. '
                            'Do not create an empty values list or append a numeric value. '
                            'The model controls its Boolean gate and real elapsed-time cadence.')
            if 'Explicit requirement pins missing from bindings:' in str(exc):
                current += ('\nThe validator found a physical board pin that your proposal omitted. '
                            'Add the catalog request that owns that exact pin; do not describe it only in meaning '
                            'or planning_notes. For PWM brightness, use pwm_output with the exact D pin. '
                            'For a digital lamp, use digital_output. Preserve all existing requests as well.\n')
            if 'Initial value has wrong type/range:' in str(exc):
                current += ('\nThe named port is numeric but received a Boolean or otherwise invalid initial value. '
                            'Use a finite numeric initial matching the catalog type (normally 0 when startup '
                            'measurement is unspecified); validity/report trigger ports remain Boolean separately.\n')
            if repeated:
                current += ('\nNO PROGRESS: the same contract error occurred twice. Correct the named field at its exact nesting level. '
                            'Compare the allowed binding keys with the complete interface fields. Keep devices, timing and requirements intact.')
    raise ContractPlanningError(attempts)


class ContractPlanningError(ValueError):
    def __init__(self, attempts):
        self.attempts = attempts
        self.blocking_category = ('hardware_capability' if attempts and all(a.get('code')=='HARDWARE_CAPABILITY' for a in attempts)
                                  else 'proposal_validation')
        super().__init__('Interface contract planning exhausted the repair budget')
