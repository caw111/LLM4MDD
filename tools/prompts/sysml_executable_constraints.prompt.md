Complete constraints for THIS executable SysML v2 generation profile
Reference language: https://www.omg.org/spec/SysML/2.0/Language/PDF
The operational rules below follow the pinned validator and this repository's assembler/compiler.
SysML permits more constructs; only the following profile can be executed by this pipeline.

1. Declarations and ownership
One package imports ScalarValues, contains part definitions, and instantiates exactly one root system.
Concrete scalar declarations use Boolean, Integer or Real; JSON model types map bool->Boolean, i32->Integer, f64->Real.
An external input is `in attribute x : Real;`; a derived output is `out attribute y : Real = expression;`.
An internal persistent scalar is `attribute saved : Integer := 0;` (initialized ONCE).
An internal pure calculation uses `attribute scaled : Real = expression;` (recomputed as needed).
Use distinct names for inputs, outputs, states, calculations, components and part instances.
Use C-compatible identifiers starting with a letter; avoid language/C keywords and reserved ProgramPage1/sysml_ prefixes.
Inputs/outputs from the frozen contract are already declared by the application. Use their exact types and names.
Never redeclare a port as state or a calculation. Assigning an output does not create persistent memory.

2. Expressions: syntax, arity and types
Scalar literals: true/false are Boolean, -2/0/7 are Integer, 0.0/2.5 are Real; all numbers must be finite.
Unary + and - accept a number; unary not accepts a Boolean. Parenthesize mixed operations explicitly.
Binary +,-,* accept numbers; / accepts numbers and always produces Real, including Integer/Integer.
Binary <,<=,>,>= compare numbers. == and != compare numbers with numbers, or Booleans with Booleans.
Binary and/or/xor accept Booleans only. Numeric truthiness and Boolean-to-number coercion are unsupported.
Integer may widen to Real. Real cannot silently narrow to Integer; Boolean never coerces to/from numbers.
The only executable function call is RealFunctions::floor(numericExpression), yielding Integer rounded downward.
Integer values, arithmetic and floor results must fit signed i32 [-2147483648,2147483647]. Guard overflow and division by zero.
No abs/min/max/sin/cos/round/map/constrain/millis/digitalRead/analogRead or user-defined function calls.
Use conditional expressions to implement clamping/selection rather than calling Arduino/C functions.
SysML conditional value syntax is `(if condition ? trueValue else falseValue)`; the condition is Boolean.
Both value branches must be compatible: both Boolean, or both numeric (Real if either branch is Real).
In controller JSON, express this as {"if":condition,"then":value,"else":value}; branches are EXPRESSIONS.
Use the registered elapsedMilliseconds input for time deltas and keep any accumulator as a
separately declared state, e.g. {">=":["elapsed",500]}, and complete arithmetic strings such as
"elapsed - 500". Never use elapsed or deltaMs as an input name.
{"if":">=","then":...,"else":...} is INVALID: >= alone has no operands and is not a Boolean condition.
A valid statement is {"if":"elapsed >= 500","then":[{"assign":"elapsed","value":"elapsed - 500"}]}.
In a conditional STATEMENT, then/else are ACTION ARRAYS. Never interchange these two shapes.
JSON structured operators: {"not":expr}, {"and":[expr,expr,...]}, {"or":[...]}, {"xor":[...]},
or a binary object with exactly one key from +,-,*,/,==,!=,<,<=,>,>= and value [left,right], e.g. {">=":["sample",512.0]}.
not takes one expression; and/or/xor take 2..256 expressions; each arithmetic/comparison operator takes exactly two.
Generate canonical JSON objects for Booleans and conditional values, not function-style and(a,b) or C !/&&/||/ternary strings.
Equivalent scalar spellings may be normalized by the assembler, with all references/types/branches checked afterwards.
Remainder %, `if ... then ... else` strings, declarations and comments in expressions remain unsupported.
No arrays, strings, indexing, pointers, casts, dynamic allocation or uninitialized values.

3. State, action and reference semantics
Only initialized internal states (including qualified child states) may be assigned; inputs and derived outputs are read-only.
Use ONE performed control-step action for the whole system, with no parallel/concurrent schedules.
Every block with multiple statements has its OWN `first start; then ...` succession chain, including nested branches.
Controller JSON update arrays supply that order; the assembler emits the actual succession syntax.
Writes take effect immediately. A later expression observes earlier assignments. Outputs observe state AFTER updates.
If two updates need old values, explicitly save the needed old values BEFORE writing either state.
For press edges test the normalized pressed input against previousPressed, then overwrite previousPressed LAST.
Pure sampled mappings use outputs/calculations, not invented counters or mutable output ports.
All scalar references must resolve in the root or an explicitly instantiated child. A dot is a part.feature path, not hardware access.
Child inputs require explicit bindings. Bind two declared, compatible scalar endpoints; a binding is equality with ONE actual driver.
Never bind a literal/expression as an endpoint, leave an input undriven, multiply-drive a scalar, or create an algebraic cycle.
Part inheritance, recursion, inout ports, state-machine syntax, events/accept/send, loops and multiple performed actions are outside this profile.
Existing edge_toggle/periodic_pulse components expand into this same profile and remain subject to full validation.
If you reference component_beep_value, KEEP the corresponding beep entry in controller.components in every repair.
Never copy a component export while removing its declaration, or replace first-pulse timing with phase<width at startup.

4. Time, units and preservation of behavior
Use the REGISTERED i32 elapsed_ms input for real delta since the previous control step; its first value is zero.
An internal state named deltaMs initialized to zero is NOT a clock. Never replace missing elapsed input with a constant control period.
Business period and pulse width differ. A repeating pulse advances active time during BOTH pulse and silence.
Check first pulse/initial silence, residual time at period crossings, missed intervals, press edges and preserve/reset pause policy.
Use periodic_pulse for a matching bounded pulse policy, with original parameters; retain full ordinary behavior when it does not match.
Raw ADC/PWM model units are adc_count/pwm_count; preserve calibration at the declared boundary and avoid double scaling.
Map physical units using explicit declared calibration. A servo output is a commanded degree value, not measured position feedback.
A tone output is Integer Hz, zero for silence; thresholds, durations and mode transitions stay in the model.
Every required sensor must influence the proper output through genuine data/control dependence. Constants cannot replace missing behavior.
Preserve all frozen startup/pause/periodic assertions, requirements, dependencies, initial states and exact output keys.
If the frozen inputs cannot represent a required behavior, do not invent input names or a fake clock to make a valid-looking model.

5. Serialization, limits and final review
Contract mode returns EXACTLY {"controller":{...},"req_test_map":{"requirements_to_testpoints":[...]}}.
No system_sysml2 field in contract mode. No fences, explanations, IR, C or hardware bindings in the actual response.
Uncontracted text mode returns system_sysml2 as a complete escaped JSON string plus the same req_test_map object.
Each mapping row has requirement_id, requirement_text, test_point_id, test_method, stimulus and oracle; retain actual requirement IDs/text.
Mapping rows propose traceability; they do not prove functional correctness.
Model limits: states <=128, calculations <=256, parts <=128, bindings <=256, definitions <=64, components <=16.
Each expression/action nesting depth <=32; each action branch <=256 statements; each expression <=8000 characters.
Before returning, check exact schema/names, types, all branch values, initialized targets, update order, units, clock source,
startup outputs, thresholds (including equality), timing/pause behavior, and every source requirement. Return the complete object.
