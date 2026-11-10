#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static constexpr uint8_t LED1_PIN = 2;   // io_0
static constexpr uint8_t LED2_PIN = 3;   // io_1
static constexpr uint8_t LED3_PIN = 4;   // io_2

static constexpr uint8_t RGB_R_PIN = 5;  // io_3 (PWM)
static constexpr uint8_t RGB_G_PIN = 6;  // io_4 (PWM)
static constexpr uint8_t RGB_B_PIN = 7;  // io_5 (PWM)

static constexpr uint8_t SERVO1_PIN = 8; // io_6
static constexpr uint8_t SERVO2_PIN = 9; // io_7

// Control period from frozen contract
static constexpr uint32_t CONTROL_PERIOD_MS = 5;

// Servo sweep model parameters to match the textual timing:
// - Sweep 0->180 takes 2s, pause 2s at 180
// - Sweep 180->0 takes 2s, pause 2s at 0
// Total cycle: 8s
static constexpr uint32_t SERVO_SWEEP_MS = 2000;
static constexpr uint32_t SERVO_PAUSE_MS = 2000;
static constexpr uint32_t SERVO_HALF_CYCLE_MS = SERVO_SWEEP_MS + SERVO_PAUSE_MS; // 4s
static constexpr uint32_t SERVO_CYCLE_MS = 2 * SERVO_HALF_CYCLE_MS;                // 8s

// LED / RGB gating occurs when servo reaches endpoints (0° or 180°).
// Endpoints every 2s: 0->180 arrival at 2s, pause-end at 4s (still 180), 180->0 arrival at 6s, pause-end at 8s (still 0).
// Model step should react immediately at these instants. We implement by event detection.
static constexpr int SERVO_MIN_DEG = 0;
static constexpr int SERVO_MAX_DEG = 180;

Servo servo1;
Servo servo2;

// Ensure no serial output
// Track last processed "endpoint event type" for edge-triggered synchronization.
enum Endpoint { EP_NONE = -1, EP_AT_0 = 0, EP_AT_180 = 1 };

#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint32_t wrapMod(uint32_t t, uint32_t mod);
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int servoAngleAt(uint32_t t_ms_in_cycle);
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static Endpoint endpointAt(uint32_t t_ms_in_cycle);
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool isEndpointEventTime(uint32_t t_ms_in_cycle, Endpoint &ep);
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t pwmFromUnit01(double unit);
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs(bool l1, bool l2, bool l3, int rgbIdx, int servoAngle0degOr180degOrMid);
#line 142 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 180 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline uint32_t wrapMod(uint32_t t, uint32_t mod) {
  return (mod == 0) ? 0 : (t % mod);
}

static int servoAngleAt(uint32_t t_ms_in_cycle) {
  // cycle timeline within SERVO_CYCLE_MS:
  // [0, 2s): 0 -> 180
  // [2s, 4s): 180 (pause)
  // [4s, 6s): 180 -> 0
  // [6s, 8s): 0 (pause)
  if (t_ms_in_cycle < SERVO_SWEEP_MS) {
    // linear from 0 to 180 over 0..2000ms
    // angle = 180 * t / 2000
    uint32_t dt = t_ms_in_cycle;
    // Use integer math mapping with rounding
    int angle = (int)(( (uint64_t)SERVO_MAX_DEG * dt + SERVO_SWEEP_MS/2 ) / SERVO_SWEEP_MS);
    if (angle < SERVO_MIN_DEG) angle = SERVO_MIN_DEG;
    if (angle > SERVO_MAX_DEG) angle = SERVO_MAX_DEG;
    return angle;
  } else if (t_ms_in_cycle < SERVO_HALF_CYCLE_MS) {
    return SERVO_MAX_DEG; // pause at 180 until 4s
  } else if (t_ms_in_cycle < SERVO_HALF_CYCLE_MS + SERVO_SWEEP_MS) {
    // 180 -> 0 over 2s (from 4s to 6s)
    uint32_t dt = t_ms_in_cycle - SERVO_HALF_CYCLE_MS;
    int angle = (int)(( (uint64_t)SERVO_MAX_DEG * (SERVO_SWEEP_MS - dt) + SERVO_SWEEP_MS/2 ) / SERVO_SWEEP_MS);
    if (angle < SERVO_MIN_DEG) angle = SERVO_MIN_DEG;
    if (angle > SERVO_MAX_DEG) angle = SERVO_MAX_DEG;
    return angle;
  } else {
    return SERVO_MIN_DEG; // pause at 0 until 8s
  }
}

static Endpoint endpointAt(uint32_t t_ms_in_cycle) {
  // During pauses and at transitions endpoints are held at exact deg.
  // Endpoint events should occur when reaching endpoints: 2s, 4s, 6s, 8s (modulo cycle).
  // We detect endpoint at exact instants within discrete control tick:
  // if time within cycle is exactly 2000, 4000, 6000, 0 (or 8000).
  // However, using ms granularity, we allow detection within a tolerance of CONTROL_PERIOD_MS.
  // For correctness of behavior as close as possible to "when reaches", we detect near those times.
  // We'll detect in a separate event function using previous and current times.
  if (t_ms_in_cycle == 0) return EP_AT_0;
  if (t_ms_in_cycle == SERVO_SWEEP_MS) return EP_AT_180; // 2s
  if (t_ms_in_cycle == SERVO_HALF_CYCLE_MS) return EP_AT_180; // 4s (still 180)
  if (t_ms_in_cycle == SERVO_HALF_CYCLE_MS + SERVO_SWEEP_MS) return EP_AT_0; // 6s
  if (t_ms_in_cycle == SERVO_CYCLE_MS) return EP_AT_0; // won't happen due to mod
  return EP_NONE;
}

static bool isEndpointEventTime(uint32_t t_ms_in_cycle, Endpoint &ep) {
  // Detect endpoint times at 2s, 4s, 6s, and 0/8s.
  // Because loop timing is approximate, treat any time within +/- CONTROL_PERIOD_MS/2 as the event.
  // Use exact comparisons on the modulo timeline for deterministic behavior in simulation.
  // Here, to preserve determinism, we use exact equality with integer milliseconds.
  // Given control_period=5ms and servo durations are multiples of 1ms, equality will occur.
  if (t_ms_in_cycle == 0) { ep = EP_AT_0; return true; }
  if (t_ms_in_cycle == SERVO_SWEEP_MS) { ep = EP_AT_180; return true; }
  if (t_ms_in_cycle == SERVO_HALF_CYCLE_MS) { ep = EP_AT_180; return true; }
  if (t_ms_in_cycle == SERVO_HALF_CYCLE_MS + SERVO_SWEEP_MS) { ep = EP_AT_0; return true; }
  return false;
}

static int ledIndex = 0; // 0->led1, 1->led2, 2->led3
static int rgbColorIndex = 0; // 0=red,1=green,2=blue, advance on each endpoint event (0 or 180)

static Endpoint lastEndpoint = EP_NONE;

static uint8_t pwmFromUnit01(double unit) {
  // Contract mapping: pwm_count scale 255, but use integer for PWM.
  // Clamp to [0,255]
  if (unit < 0.0) unit = 0.0;
  if (unit > 1.0) unit = 1.0;
  return (uint8_t)lround(unit * 255.0);
}

static void applyOutputs(bool l1, bool l2, bool l3, int rgbIdx, int servoAngle0degOr180degOrMid) {
  // io_0..io_2
  digitalWrite(LED1_PIN, l1 ? HIGH : LOW);
  digitalWrite(LED2_PIN, l2 ? HIGH : LOW);
  digitalWrite(LED3_PIN, l3 ? HIGH : LOW);

  // io_3..io_5 as pwm_count with scale 255:
  // Model semantics: unit 1 = full on, 0 = off.
  // RGB LED is common cathode, so wiring uses R/G/B pins to sink current.
  // Using standard Arduino PWM HIGH drives the pin high; with common cathode,
  // HIGH turns off (depending on module). In Wokwi, common cathode RGB expects PWM values with
  // HIGH = on? Empirically in these tasks: set duty 255 to turn channel on.
  // We'll follow contract's "red on channel" mapping: unit 1 -> PWM 255.
  double r = 0.0, g = 0.0, b = 0.0;
  if (rgbIdx == 0) r = 1.0;
  else if (rgbIdx == 1) g = 1.0;
  else b = 1.0;

  analogWrite(RGB_R_PIN, pwmFromUnit01(r));
  analogWrite(RGB_G_PIN, pwmFromUnit01(g));
  analogWrite(RGB_B_PIN, pwmFromUnit01(b));

  // Servo angles io_6/io_7
  servo1.write(servoAngle0degOr180degOrMid);
  servo2.write(servoAngle0degOr180degOrMid);
}

void setup() {
  // IO init
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Attach servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Initialization per frozen model:
  // - all LEDs off
  // - RGB displays red (io_3=1.0, io_4=0, io_5=0)
  // - both servos at 0 degrees
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  analogWrite(RGB_R_PIN, 255); // red on
  analogWrite(RGB_G_PIN, 0);
  analogWrite(RGB_B_PIN, 0);

  servo1.write(SERVO_MIN_DEG);
  servo2.write(SERVO_MIN_DEG);

  // Initialize sequencing indices to match startup step:
  // On first control step after reset, outputs should reflect initial state only.
  // LED sequence: starts with all off, then first flicker occurs on first endpoint event.
  // We initialize indices so that the first endpoint event turns on LED1 and sets RGB to red (already red).
  ledIndex = 0;          // next LED after first endpoint -> led1
  rgbColorIndex = 0;    // first endpoint -> red (already red)
  lastEndpoint = EP_NONE;
}

void loop() {
  static uint32_t t0 = millis();
  static uint32_t lastTick = t0;

  uint32_t now = millis();
  if (now - lastTick < CONTROL_PERIOD_MS) return;
  lastTick += CONTROL_PERIOD_MS;

  uint32_t elapsedMs = now - t0;
  uint32_t tInCycle = wrapMod(elapsedMs, SERVO_CYCLE_MS);

  // Servo angle is continuous model (but endpoints gate LEDs/RGB)
  int servoAngle = servoAngleAt(tInCycle);

  // Endpoint event detection at discrete instants.
  // We detect if endpointAt within current tick differs from lastEndpoint.
  // Since we use exact equality on ms, ensure deterministic event times.
  Endpoint epNow = EP_NONE;
  Endpoint dummy;
  if (isEndpointEventTime(tInCycle, dummy)) {
    epNow = dummy;
  } else {
    epNow = EP_NONE;
  }

  if (epNow != EP_NONE && epNow != lastEndpoint) {
    // On each endpoint arrival (0 or 180): advance LED and RGB flicker
    // Frozen contract indicates LEDs blink in sequence, each flicker occurs when servo reaches 0 or 180.
    // So: increment LED on each endpoint event; also increment RGB color on each endpoint event,
    // and keep synchronized with LED.
    // Apply updates by setting "active" to ledIndex and rgbColorIndex before incrementing?
    // Requirement: "then the next LED turns on" at each event.
    // We'll treat ledIndex as the LED that becomes ON at the event, starting from led1 on first event.
    // So do outputs using current ledIndex/rgbColorIndex, then advance them for subsequent events.
    // To achieve that, we set active LEDs based on ledIndex/rgbColorIndex immediately in output stage below,
    // and then advance indices here for next event.
    lastEndpoint = epNow;

    // RGB: advance to next color each endpoint event, but requirement says cycle red,green,blue,
    // each flicker occurs when servo reaches endpoints. Typically flicker shows that color at that time.
    // With initialization already red and first event at 0 deg (t=0) would conflict, but we avoid turning on LEDs at t=0.
    // Since we will not generate an event at t=0 (lastEndpoint=NONE -> epNow=0 -> event would fire immediately),
    // but this would violate "all LEDs off at startup". Thus we suppress event at t=0 by checking elapsedMs==0.
    // However, t0 is set in setup, so first loop tick occurs later; tInCycle will not necessarily be 0.
    // To be safe: only react to endpoint events after at least 1ms elapsed.
    // If tInCycle==0 at later times (8s boundary), we must react.
    if (elapsedMs > 0) {
      // LEDs: turn ON the current ledIndex at this endpoint; RGB: current rgbColorIndex at this endpoint.
      // After flicker, prepare next values.
      ledIndex = (ledIndex % 3); // keep in bounds; active uses ledIndex already
      rgbColorIndex = (rgbColorIndex % 3);

      // Advance for next flicker:
      ledIndex = (ledIndex + 1) % 3;
      rgbColorIndex = (rgbColorIndex + 1) % 3;

      // Note: We advanced indices, so we must reflect the color/led that "turns on" at this event.
      // We'll compensate by subtracting 1 in output stage using flags below.
    }
  }

  // Determine active outputs at this control step:
  // LED should blink in sequence: each flicker occurs at endpoint event and then next LED turns on.
  // Interpretation for continuous control: between events, exactly one LED is on (the most recently activated one),
  // starting from "all off" at initialization until first endpoint event.
  // Implement using separate latched active indices updated at endpoint events.
  static bool haveFirstFlicker = false;
  static int activeLed = 0;   // which LED currently on
  static int activeRgb = 0;   // which RGB currently displayed

  // Update latches when an endpoint event occurs (at current tick), using elapsedMs>0 to avoid startup event.
  if (epNow != EP_NONE) {
    if (elapsedMs > 0) {
      haveFirstFlicker = true;

      // Determine which values should be active at this event:
      // We set active to the *current* ledIndex/rgbColorIndex before increment at event time.
      // But indices were advanced in the event handler above. Instead of undoing, we recompute:
      // If we advanced ledIndex and rgbColorIndex after event, then the values that were active at the event
      // are (ledIndex-1) and (rgbColorIndex-1) mod N.
      int prevLed = (ledIndex + 3 - 1) % 3;
      int prevRgb = (rgbColorIndex + 3 - 1) % 3;

      activeLed = prevLed;
      activeRgb = prevRgb;
    } else {
      // elapsedMs==0 (startup), keep all LEDs off and RGB red per init contract.
    }
  }

  bool l1 = false, l2 = false, l3 = false;
  int rgbIdx = 0;

  if (!haveFirstFlicker) {
    // Keep all LEDs off until first synchronized flicker event.
    l1 = false; l2 = false; l3 = false;
    rgbIdx = 0; // red
  } else {
    l1 = (activeLed == 0);
    l2 = (activeLed == 1);
    l3 = (activeLed == 2);
    rgbIdx = activeRgb;
  }

  applyOutputs(l1, l2, l3, rgbIdx, servoAngle);
}
