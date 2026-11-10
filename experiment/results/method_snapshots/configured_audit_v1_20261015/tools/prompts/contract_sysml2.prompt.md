Generate executable SysML v2 and a requirement-to-test mapping as one JSON object:
{"system_sysml2":"...", "req_test_map":{"requirements_to_testpoints":[...]}}.
Each mapping row requires requirement_id, requirement_text, test_point_id, test_method, stimulus, oracle.
Use the frozen contract's requirement IDs. Generated test mappings do not establish correctness.

Use private import ScalarValues::*; and one part system : Controller;.
Declare EXACTLY the contract's external inputs/outputs, with EXACT names and types:
i32 = Integer; f32/f64 = Real; bool = Boolean. Do not add enable/reset/time inputs or extra output aliases.
Bindings describe hardware. SysML contains only control behavior, no pins or device libraries.
An elapsed_ms binding is the delta SINCE THE PREVIOUS STEP, first value 0. Accumulate it in persistent state when needed.
A digital_input with active_high=false is already normalized: true means active/pressed. Do not invert it again.
Output initial values describe required initial hardware state; preserve initial behavior in the model too.

Prefer direct output expressions for stateless behavior. Constant ON needs no state and no actions.
For a sampled input controlling an output directly, use out attribute OUTPUT : Boolean = INPUT;.
Do NOT generate perform action tick { assign OUTPUT := INPUT; }; derived outputs are not assignment targets.
Never introduce a counter solely to make an example look executable.
Persistent state: attribute lit : Boolean := false; or attribute accumulated : Integer := 0;.
Update only initialized state with assign. Boolean toggling uses not lit, never an Integer state.
Use a SINGLE perform action tick. In every block containing multiple actions, start with first start;
then prefix EVERY subsequent action with then. Single assignment branches need no succession syntax.
After actions execute, output expressions read the updated state.
Boolean operators are not, and, or; NOT !, && or || in SysML.
Conditional value expression is if CONDITION ? YES else NO, never if ... then ... or C's colon ternary.
Use +, -, *, /, comparisons. No %, mod, general loops, arrays or unsupported library calls.
All root output attributes must have exactly one value expression. Do not declare an output twice.
Bind connects distinct part interfaces; do not bind an output to itself or create extra alias outputs.

<!-- example:constant -->
Minimal legal constant-output pattern (adapt names/values to the actual contract):
package ConstantExample {
 private import ScalarValues::*;
 part def Controller { out attribute lamp : Boolean = true; }
 part system : Controller;
}
<!-- /example -->

<!-- example:sample -->
Minimal legal sampled combinational pattern:
package SampleExample {
 private import ScalarValues::*;
 part def Controller {
  in attribute pressed : Boolean;
  out attribute lamp : Boolean = pressed;
 }
 part system : Controller;
}
<!-- /example -->

<!-- example:timer -->
Legal persistent timer pattern. The time input is a DELTA, not absolute uptime.
500 below is an example threshold; implement the actual requirement's interval.
The scheduler must resolve this interval; business timing remains here in the model.
package TimerExample {
 private import ScalarValues::*;
 part def Controller {
  in attribute elapsedMilliseconds : Integer;
  attribute accumulated : Integer := 0;
  attribute lit : Boolean := false;
  perform action tick {
   first start;
   then assign accumulated := accumulated + elapsedMilliseconds;
   then if accumulated >= 500 {
    first start;
    then assign accumulated := accumulated - 500;
    then assign lit := not lit;
   }
  }
  out attribute lamp : Boolean = lit;
 }
 part system : Controller;
}
<!-- /example -->

Implement ALL original requirements; do not copy unrelated inputs, outputs, states or devices from examples.
Do not replace sensor data with constants. Constant outputs are appropriate only when explicitly required.
Original requirements:
<<USER_REQUIREMENTS>>
