Return one JSON object with exactly controller and req_test_map.
The application assembles the full SysML package and ALL external declarations from the frozen interface contract.
Do NOT return system_sysml2, package text, external declarations, a second system instance, or package-level bindings.
The assembled SysML is checked by the reference validator, then compiled from its semantic model.

controller schema:
{
 "states": [{"name":"internalName", "type":"bool|i32|f64", "initial":"SysML constant expression"}],
 "updates": [ordered statements],
 "outputs": {"EXACT_CONTRACT_OUTPUT_NAME":"SysML expression"}
}
Only states, updates and outputs are required. Start from this minimal schema.
All input names in expressions must exactly match the frozen contract. The application
supplies the exact registered elapsed-time input name in the contract-specific instructions;
`elapsedMilliseconds`, `elapsed` and `deltaMs` in examples are placeholders unless one is
the exact registered port name.
For edge toggles and periodic pulses whose semantics match the requirement, use the typed components below
instead of reimplementing their internal state updates. They expand into ordinary SysML states/actions,
and the ENTIRE expanded model still passes reference parsing, IR compilation and interface checks.
Optional components: ordered array of either:
{"kind":"edge_toggle","name":"run","input":"pressed","edge":"rising","initial":true,"previous_input":false}
{"kind":"periodic_pulse","name":"beep","delta":"elapsedMilliseconds","enabled":"component_run_value","period_ms":3000,"pulse_ms":100,"first_pulse":"after_period","pause":"preserve"}
Each component exports one read-only Boolean: component_NAME_value. Never redeclare or assign any component_ name.
edge_toggle: input is a declared Boolean input or earlier component export; edge is rising or falling;
initial is the initial toggled value; previous_input is the initial edge memory. Both are JSON Booleans.
A normalized active-low button is true when PRESSED, so press-edge requires rising, not falling.
periodic_pulse: delta names the actual i32 millisecond delta input; enabled is "true", "false", a Boolean input,
or an earlier component export. period_ms is START-TO-START active time, including the sounding interval.
pulse_ms must be 1..period_ms. first_pulse is after_period (wait a FULL period), immediate (initial pulse),
or after_silence (wait period_ms - pulse_ms; requires pulse_ms < period_ms).
For an initially-off LED toggling every 500ms, use period_ms=1000, pulse_ms=500, first_pulse=after_silence.
For start/stop with restart from off, use an initially-false edge_toggle and pause=reset.
pause=preserve freezes phase/remaining pulse; pause=reset restarts timing when enabled again.
Output is always false while disabled. Delta belongs to the preceding interval: pause counts the interval
ending at the press, resume excludes the preceding paused interval. The first adapter delta is 0.
Components execute in array order BEFORE free-form updates. Outputs observe the resulting values.
Use the ORIGINAL requirement's parameters, edge, initial mode and pause policy. These are choices, not defaults.
If these two components do not express the required behavior, retain the ordinary states/updates schema;
never alter a requirement to fit a component. Components do not read pins, drive devices or format text.
Optional calculations: array of {name, type, expression} for named intermediate values, including simple arithmetic reused by several outputs.
Calculations are derived expressions, not persistent state or assignment targets. Use distinct names from external ports.
Optional decomposition fields, ONLY when required: parts: array of {name, definition};
bindings: array of {left, right}, each endpoint a declared scalar name or part.scalar reference, never a literal;
definitions: array of {name, inputs:[{name,type}], output_ports:[{name,type}], body:{states,updates,outputs}}.
Module bodies may also have calculations, parts, bindings and components. Never copy an unrelated module from a schema example.
Root inputs/output names and types are ALREADY supplied by the contract; never redeclare them in states/calculations.
Each output appears exactly once in outputs. Each state has an initial value and a type.
Use non-keyword identifiers such as pressEvent, timerDue and previousPressed. Bare names such as
event, part, action, state and attribute are SysML keywords and cannot name controller states or modules.
Do not introduce counters, enable/reset inputs or modules unless required by the actual behavior.

Statement forms (use JSON objects, not raw action text):
{"assign":"internalStateName","value":"SysML expression"}
{"if":"Boolean SysML expression","then":[statements],"else":[statements]}
else may be omitted. An empty then/else list means no state change in that branch; its condition is still checked.
The renderer generates first start/then syntax and preserves list order, including explicit no-op branches.
Assignments target only initialized internal states (or qualified part states). Root outputs are derived expressions, not assignment targets.
Outputs read state AFTER all ordered updates. All performed behavior must form one control step; no concurrent action schedules.
Assignments take effect IMMEDIATELY: each later statement sees earlier writes, not a snapshot of old state.
For edge detection evaluate pressed and not previousPressed BEFORE updating previousPressed; update that memory LAST.
Outputs also observe the UPDATED edge memory: never use pressed and not previousPressed as an output
after assigning previousPressed=pressed. Capture the event in a separate per-step Boolean state first.
Likewise capture timer due BEFORE subtracting/resetting its accumulator; outputs read that captured event,
not accumulator>=period after the accumulator has been reduced. Reset event flags at each step.
Serial *_valid inputs last ONE step. Commands that SET an LED, PWM, mode or display need persistent
state: update only on valid events and hold between commands. Do not output zero/false on invalid steps.
Respect adc_count versus pwm_count versus degC: type Real alone does not convert units. For a requested
full-range ADC-to-PWM mapping, perform the conversion in the model unless explicitly calibrated in binding.
Do not pass raw ADC directly into PWM and rely on adapter clamping as a mapping.
Never replace required dynamics with constant outputs, self-assignments or unchanging timer state.
Sensor reports must use the corresponding native measurement and unit: temperature from dht_temperature,
humidity from dht_humidity. Update cached readings on valid frames; an initialized cache that is never updated
does not implement measurement reporting. Preserve the contract's invalid-frame policy and exact report labels.
For start/stop timing reset the accumulator and output at the transition; stopping must not leave a stale ON output.

Simple expressions are strings using not/and/or; + - * /; == != < <= > >=.
Literal operands, state initial values and conditional branches may also be JSON Booleans or finite numbers.
true/false remain Boolean, integer literals remain Integer and decimal literals such as 1.0 remain Real;
these do not bypass reference type checking. null, NaN and Infinity are not expressions.
Boolean negation may also use {"not":"Boolean expression"}; it is rendered as SysML not and fully type-checked.
PREFER structured Boolean operators and comparison guards: {"and":["enabled",{"not":"paused"}]} and {">=":["elapsedMilliseconds",3000]}.
For timed repeating pulses, prefer matching periodic_pulse components and the supplied frozen-obligation fragment;
do not expand an infinite repetition into a finite sequence of future time windows.
and/or/xor accept 2..256 expression operands; == != < <= > >= + - * / accept exactly two.
Operands may be expression strings or nested expression objects; their order and parentheses are preserved.
Do not use function-style and(a,b)/or(a,b)/xor(a,b) strings; use infix strings or these structured objects.
Comparisons and arithmetic also use INFIX syntax: "value >= limit", not ">=(value, limit)".
Never replace a full expression with an operator token such as ">=" or "and"; preserve all operands.
CONDITIONAL VALUE EXPRESSIONS MUST use an object: {"if":"condition","then":"yesValue","else":"noValue"}.
Use this object anywhere an expression is accepted (output, assignment value, calculation, or nested expression).
Each branch is an expression string or another conditional expression object. The application generates the SysML conditional syntax.
This differs from an if STATEMENT, whose then/else fields are arrays of actions. An expression never contains action arrays.
Generate canonical objects rather than C !, &&, ||, raw ternary or function-style Boolean strings.
The application can normalize equivalent scalar spellings, but it still checks every reference/type/branch.
Remainder %, statements, comments and declarations in expression strings remain unsupported.
For input gating use {"if":"pressed","then":"sampledValue * 255.0 / 1023.0","else":"0.0"} as the Real output value.
Toggled state is Boolean, not Integer. Division produces Real; do not put division results into Integer state.
For a required Integer output use RealFunctions::floor(realExpression), e.g.
{"if":"pressed","then":"RealFunctions::floor(adc * 255.0 / 1023.0)","else":"0"}.
floor rounds toward negative infinity; its result must fit signed 32 bits. Other function calls are unsupported.
Structured floor is also supported: {"floor": numericExpression}, or {"RealFunctions::floor": numericExpression}.
Its single operand may be a nested expression; never put the operand in an array. Both spellings render the same reference-library call.
For integer parity use "n == 2 * RealFunctions::floor(n / 2)" (even) or != (odd).
For nonnegative integer n and positive integer d, remainder is "n - d * RealFunctions::floor(n / d)".
This floor formula differs from C remainder for negative dividends; preserve the required sign/domain semantics.
Neither mod objects nor % strings are supported. Do not introduce timers or counters for ordinary arithmetic.
Timing inputs with binding.kind=elapsed_ms are actual delta SINCE THE PREVIOUS CONTROL STEP, first value 0.
Accumulate the delta in state for business timing; never invent a constant clock increment.
digital_input with active_high=false is ALREADY normalized: true means pressed/active. Do not invert it twice.
Hardware sampling and driving follow the contract; control, thresholds, timing and state transitions must remain in this model.
For servo_angle the output is a commanded angle in degrees, normally Real for a proportional mapping.
Compute every physical servo output from the specified sampled inputs; multiple servos may share an expression but each declared output is required.
The adapter only sends angles: any requested speed, motion sequence, pause or time interval must be implemented by model state and elapsed time.
Servo readback is not a measured physical position. Do not invent a sensor input or replace a potentiometer with a constant.
For tone_output the output is an Integer frequency in Hz: 0 means silent, a positive in-range value is the pitch.
Implement all beep start/stop times, pulse duration, repetition and pause/resume state in this controller.
The adapter generates only the carrier and never adds a beep timer or pulse duration.
If the frozen contract includes startup_checks, the model's FIRST step with declared initial inputs
must produce those expected outputs. Setting only the adapter initial output does not satisfy that check.
If it includes pause_checks, a normalized press rising edge toggles the specified initial running mode;
the output must equal expected whenever paused, including while held, after release and in a partly completed pulse.
These application-owned assertions are derived from explicit public requirements or supplied by the caller.
Preserve them exactly. Passing them covers only those properties; all other timing and behavior is still required.
Frozen periodic_checks test explicit start-to-start period, pulse width, first pulse and any press-edge pause policy using public synthetic traces.
Advance period time through BOTH sound and silence. Preserve remaining phase and pulse during preserve-mode pause; reset only if the contract explicitly requires reset.
Assignments are immediate: after remaining := remaining - delta, compare remaining <= 0; subtracting delta again in the guard tests a different value.
These checks do not replace independent final tests. Never remove an assertion or change its timing to make a model pass.
Frozen numeric_checks are application-owned public numeric obligations. counter_edges checks short press edges,
both range wraps, simultaneous increment/decrement and reset priority; it does not replace full repeat-time requirements.
At the lower bound, decrement must select the declared upper bound; at the upper bound, increment selects the lower.
For mapped_parity compute the specified mapped display integer first, then select actuator outputs by THAT integer's parity,
not by the raw ADC input. Preserve each actuator's own sampled brightness input, exact mapping and rounding.
All numeric_checks remain frozen and are re-executed after every repair. Do not edit or remove them, and retain the rest of the task.

<!-- example:constant -->
For constant ON with output lamp, the ENTIRE controller is:
{"states":[],"updates":[],"outputs":{"lamp":"true"}}
<!-- /example -->
<!-- example:sample -->
For a button input pressed directly driving output lamp:
{"states":[],"updates":[],"outputs":{"lamp":"pressed"}}
For a threshold use an output expression such as "temperature > 30.0". Preserve the actual requirement's boundary.
<!-- /example -->
<!-- example:numeric_parity -->
Example of STATELESS sampled arithmetic: an ADC sample chooses a number 0..19; even numbers enable a separately sampled Real drive value 0..200.
These names and bounds are illustrative: preserve the actual contract names, types, domains, mappings and all output selection conditions.
{"states":[],"updates":[],
 "calculations":[{"name":"mappedCount","type":"i32","expression":{"floor":"numberSample * 19.0 / 1023.0"}},
                 {"name":"evenCount","type":"bool","expression":"mappedCount == 2 * RealFunctions::floor(mappedCount / 2)"}],
 "outputs":{"number":"mappedCount","drive":{"if":"evenCount","then":"levelSample * 200.0 / 1023.0","else":0.0}}}
For an i32 drive output, keep the contract and use an explicitly rounded branch:
{"if":"evenCount","then":{"floor":"levelSample * 200.0 / 1023.0"},"else":0}.
Do not assign to number or drive in updates. Multiple outputs can reuse calculations without persistent state.
Opposite parity selection uses not evenCount. Preserve all additional devices and conditions in the actual task.
<!-- /example -->
<!-- example:timer -->
Example of an initially-off output lamp toggling every 500ms using input elapsedMilliseconds (adapt all names and intervals):
{"states":[{"name":"accumulated","type":"i32","initial":"0"},{"name":"lit","type":"bool","initial":"false"}],
 "updates":[{"assign":"accumulated","value":"accumulated + elapsedMilliseconds"},
            {"if":"accumulated >= 500","then":[{"assign":"accumulated","value":"accumulated - 500"},{"assign":"lit","value":"not lit"}]}],
 "outputs":{"lamp":"lit"}}
Keep report-trigger pulses and multi-step state transitions in model updates too. Implement the actual requirement, not unrelated example behavior.
<!-- /example -->

<!-- example:edge_timer -->
Example of press-edge start/stop with reset-on-start timing (adapt names and periods to the actual contract):
{"states":[{"name":"running","type":"bool","initial":"false"},
 {"name":"previous","type":"bool","initial":"false"},
 {"name":"elapsed","type":"i32","initial":"0"},
 {"name":"lit","type":"bool","initial":"false"}],
 "updates":[
  {"if":"pressed and not previous","then":[
    {"assign":"running","value":"not running"},
    {"assign":"elapsed","value":"0"},{"assign":"lit","value":"false"}],
   "else":[{"if":"running","then":[
    {"assign":"elapsed","value":"elapsed + elapsedMilliseconds"},
    {"if":"elapsed >= 500","then":[{"assign":"elapsed","value":"elapsed - 500"},{"assign":"lit","value":"not lit"}]}]}]},
  {"assign":"previous","value":"pressed"}],
 "outputs":{"lamp":"lit"}}
Here the edge is evaluated before previous is overwritten. Startup skips accumulation of the interval before the press.
For a different control requirement preserve that requirement; this example only explains ordering and scope.
<!-- /example -->

<!-- example:periodic_pulse -->
For periodic short beeps with an initially silent output, track whether the FIRST scheduled pulse has begun.
Testing phase < duration alone is WRONG at startup: phase starts at 0 and would sound immediately.
Example: first beep after 1000ms, then every 1000ms, duration 100ms, pitch 1000Hz.
Adapt the names, period, duration and pitch to the actual frozen requirements; do not copy these example values blindly.
{"states":[],"updates":[],
 "components":[{"kind":"periodic_pulse","name":"beep","delta":"elapsedMilliseconds","enabled":"true",
                 "period_ms":1000,"pulse_ms":100,"first_pulse":"after_period","pause":"preserve"}],
 "outputs":{"frequency":{"if":"component_beep_value","then":"1000","else":"0"}}}
The start-to-start period and the pulse duration are different conditions, both executed in this model.
If the requirement explicitly calls for an immediate initial beep, preserve that requirement instead.
<!-- /example -->

<!-- example:paused_pulse -->
Pause/resume and pulse timing are distinct state concepts. For a period measured start-to-start in ACTIVE running time,
advance the period counter throughout BOTH sounding and silent portions whenever running. Advancing it only while silent
silently changes the required period to period + pulseDuration. Freeze the whole active-time phase only when paused.
Example: initially running but silent; first pulse after 3000ms active time, repeated every 3000ms active time,
100ms pulse at 1000Hz; press edge pauses/resumes preserving phase, including a partly completed pulse.
Adapt names, all numeric parameters, initial mode and pause/reset policy to the actual requirement.
{"states":[],"updates":[],
 "components":[{"kind":"edge_toggle","name":"run","input":"pressed","edge":"rising","initial":true,"previous_input":false},
               {"kind":"periodic_pulse","name":"beep","delta":"elapsedMilliseconds","enabled":"component_run_value",
                "period_ms":3000,"pulse_ms":100,"first_pulse":"after_period","pause":"preserve"}],
 "outputs":{"frequency":{"if":"component_beep_value","then":"1000","else":"0"}}}
The component's first_pulse policy prevents a spurious pulse at startup. The adapter still samples real elapsed time even during pause;
only the model decides whether that delta advances business time. No hidden test timeline belongs in this controller.
<!-- /example -->

req_test_map MUST be an OBJECT, never an array. Its complete shape is:
{"requirements_to_testpoints":[{"requirement_id":"R1","requirement_text":"original requirement","test_point_id":"T1","test_method":"trace","stimulus":"input","oracle":"expected output"}]}
req_test_map.requirements_to_testpoints must be non-empty. Each row requires requirement_id, requirement_text,
test_point_id, test_method, stimulus, oracle. Preserve ALL frozen requirement IDs. These proposed tests are not proof of correctness.

Original requirements:
<<USER_REQUIREMENTS>>
