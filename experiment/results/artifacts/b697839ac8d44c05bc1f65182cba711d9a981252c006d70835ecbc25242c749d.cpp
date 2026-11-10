#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;

// --------- Pins (Mega 2560 as in circuit) ---------
static const uint8_t LED1_PIN = 2;   // led1:A -> mega:2
static const uint8_t LED2_PIN = 3;   // led2:A -> mega:3

// 74HC595 (sr1)
static const uint8_t SR_DATA  = A0;  // mega:A0 -> sr1:DS (blue)
static const uint8_t SR_LATCH = A1;  // mega:A1 -> sr1:STCP (purple)
static const uint8_t SR_CLOCK = A2;  // mega:A2 -> sr1:SHCP (gray)

// 7-seg mapping: sr1 outputs Q0..Q6 -> sevseg1:A..G
// Use Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G
// Also: sr1:MR is tied to VCC (sr1:VCC -> sr1:MR), so MR high always.
// sr1:OE is tied low (sr1:MR -> sevseg1:COM.1, sr1:GND -> sr1:OE), so OE low always -> outputs enabled.

// Common wiring in this circuit implies these segments should be turned ON with '1'.
// We'll use standard 7-segment common-cathode segment codes for digits 0..9:
// segments: A B C D E F G
static const uint8_t digitToSegments[10] = {
  // 0: A B C D E F
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),                          // 0
  (1 << 1) | (1 << 2),                                                                      // 1
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),                                     // 2
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),                                     // 3
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),                                              // 4
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),                                     // 5
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),                            // 6
  (1 << 0) | (1 << 1) | (1 << 2),                                                             // 7
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),              // 8
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)                             // 9
};

// Send one 8-bit value to 74HC595 (LSB -> Q0)
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWrite(uint8_t value);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showDigit(uint8_t d);
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateServoSweep( Servo &sv, unsigned long &stepStartMs, bool &atHigh, bool &inStop, unsigned long &stopUntilMs, bool &beforeFirstSweepDone, int startAngle, int endAngle, unsigned long sweepDurationMs, unsigned long stopDurationMs, unsigned long currentMs );
#line 143 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 192 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_103\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWrite(uint8_t value) {
  // Shift out MSB first for typical Arduino shiftOut; but we will manually write LSB-first to match Q0..Q7.
  // We'll do LSB-first: for i=0..7, set DS to bit i, pulse SHCP.
  digitalWrite(SR_LATCH, LOW);
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR_CLOCK, LOW);
    digitalWrite(SR_DATA, (value >> i) & 0x01);
    digitalWrite(SR_CLOCK, HIGH);
  }
  digitalWrite(SR_LATCH, HIGH);
}

static void showDigit(uint8_t d) {
  // Only Q0..Q6 used; Q7 left as 0.
  uint8_t seg = (d < 10) ? digitToSegments[d] : 0;
  srWrite(seg & 0x7F);
}

// --------- Scheduling ---------
unsigned long nowMs;

unsigned long ledPrevMs = 0;
bool ledStateLed2On = true; // "First led1 off, led2 on"

unsigned long segPrevMs = 0;
uint8_t currentDigit = 0;

// Servo state machines
// servo1: 0->180->0 (each sweep takes 4s) with 2s stop at each end (except before first sweep)
unsigned long s1StepStartMs = 0;
bool s1At0 = true;                // current position conceptually at 0 at start
bool s1InStop = false;          // whether currently in end-stop dwell
unsigned long s1StopUntilMs = 0;
bool s1BeforeFirstSweepDone = false;

// servo2: 180->0->180 (each sweep takes 4s) with 2s stop at each end (except before first sweep)
unsigned long s2StepStartMs = 0;
bool s2At180 = true;            // current position conceptually at 180 at start
bool s2InStop = false;
unsigned long s2StopUntilMs = 0;
bool s2BeforeFirstSweepDone = false;

static void updateServoSweep(
  Servo &sv,
  unsigned long &stepStartMs,
  bool &atHigh,           // true means at 180, false means at 0 (for direction logic)
  bool &inStop,
  unsigned long &stopUntilMs,
  bool &beforeFirstSweepDone,
  int startAngle,         // start angle for current sweep (0 or 180 depending on state)
  int endAngle,           // end angle for current sweep
  unsigned long sweepDurationMs, // 4000
  unsigned long stopDurationMs,  // 2000
  unsigned long currentMs
) {
  if (inStop) {
    if (currentMs >= stopUntilMs) {
      inStop = false;
      stepStartMs = currentMs;
      beforeFirstSweepDone = true;
      // startAngle may already match current; but we move via interpolation in loop
    } else {
      return; // keep holding
    }
  } else {
    unsigned long elapsed = currentMs - stepStartMs;
    if (!beforeFirstSweepDone) {
      // prior to first sweep: no 2s stop; sweep starts immediately
      beforeFirstSweepDone = true;
      stepStartMs = currentMs;
      elapsed = 0;
    }

    if (elapsed >= sweepDurationMs) {
      // snap to end, then dwell if not first-sweep dwell exception
      sv.write(endAngle);

      // Determine if we should dwell at this end
      // "When move to 180° or 0°, the servo should stop for 2 second and then continue sweep."
      // Exception: "Before the servo start the first sweep, although the servo is in 0° or 180°, the servo don't need to stop for 2 second."
      // So: dwell after completing a sweep always (including after first sweep completion).
      inStop = true;
      stopUntilMs = currentMs + stopDurationMs;

      // Update atHigh state to match endAngle
      atHigh = (endAngle == 180);
      return;
    } else {
      // Linear interpolation during sweep
      float t = (float)elapsed / (float)sweepDurationMs; // 0..1
      int angle = (int)(startAngle + (endAngle - startAngle) * t + 0.5f);
      sv.write(angle);
    }
  }
}

void setup() {
  // LEDs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // 7-seg / 74HC595 pins
  pinMode(SR_DATA, OUTPUT);
  pinMode(SR_LATCH, OUTPUT);
  pinMode(SR_CLOCK, OUTPUT);

  digitalWrite(SR_LATCH, LOW);
  digitalWrite(SR_CLOCK, LOW);
  digitalWrite(SR_DATA, LOW);

  // Initial LED state: led1 off, led2 on
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, HIGH);

  // Initial 7-seg digit 0
  showDigit(0);

  // Servo attach
  // servo1:PWM -> mega:4, servo2:PWM -> mega:5
  servo1.attach(4);
  servo2.attach(5);

  // Set initial positions as implied by opposite direction starts
  // servo1 at 0°, servo2 at 180°
  servo1.write(0);
  servo2.write(180);

  // Servo state initialization
  s1StepStartMs = millis();
  s1At0 = true; // at 0 at start
  s1InStop = false;
  s1StopUntilMs = 0;
  s1BeforeFirstSweepDone = false;

  s2StepStartMs = millis();
  s2At180 = true; // at 180 at start
  s2InStop = false;
  s2StopUntilMs = 0;
  s2BeforeFirstSweepDone = false;

  // Start times for other components
  ledPrevMs = millis();
  segPrevMs = millis();
}

void loop() {
  nowMs = millis();

  // -------- LEDs blink alternately: 2s ON, 2s OFF (led1 off then led2 on) --------
  if (nowMs - ledPrevMs >= 2000UL) {
    ledPrevMs = nowMs;
    ledStateLed2On = !ledStateLed2On;

    if (ledStateLed2On) {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, HIGH);
    } else {
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, LOW);
    }
  }

  // -------- 7-seg count 0..9 every 2s --------
  if (nowMs - segPrevMs >= 2000UL) {
    segPrevMs = nowMs;
    currentDigit++;
    if (currentDigit > 9) currentDigit = 0;
    showDigit(currentDigit);
  }

  // -------- Servos sweep concurrently --------
  // servo1: 0->180->0, each sweep takes 4s; stop 2s at ends after reaching them
  // servo2: 180->0->180, each sweep takes 4s; stop 2s at ends after reaching them
  //
  // We keep two independent state machines:
  // - if inStop: wait until stopUntilMs
  // - else interpolate until sweepDurationMs reached, then snap and set inStop true.
  //
  // For each update, choose current sweep direction based on state.

  // servo1 current sweep direction:
  // s1At0 true means next sweep should go 0->180, else 180->0
  {
    unsigned long stepStart = s1StepStartMs;
    bool atHigh = !s1At0; // atHigh meaning at 180
    (void)stepStart;

    if (s1At0) {
      updateServoSweep(
        servo1,
        s1StepStartMs,
        atHigh,
        s1InStop,
        s1StopUntilMs,
        s1BeforeFirstSweepDone,
        0, 180,
        4000UL, 2000UL,
        nowMs
      );
      s1At0 = (atHigh ? false : true); // atHigh == true means at180, so at0=false
    } else {
      updateServoSweep(
        servo1,
        s1StepStartMs,
        atHigh,
        s1InStop,
        s1StopUntilMs,
        s1BeforeFirstSweepDone,
        180, 0,
        4000UL, 2000UL,
        nowMs
      );
      s1At0 = (atHigh ? false : true); // endAngle 0 => atHigh false => s1At0 true
    }
  }

  // servo2 current sweep direction:
  // s2At180 true means next sweep 180->0, else 0->180
  {
    unsigned long stepStart = s2StepStartMs;
    bool atHigh = s2At180; // atHigh meaning at 180
    (void)stepStart;

    if (s2At180) {
      updateServoSweep(
        servo2,
        s2StepStartMs,
        atHigh,
        s2InStop,
        s2StopUntilMs,
        s2BeforeFirstSweepDone,
        180, 0,
        4000UL, 2000UL,
        nowMs
      );
      s2At180 = atHigh; // endAngle 0 => atHigh false => s2At180 false
    } else {
      updateServoSweep(
        servo2,
        s2StepStartMs,
        atHigh,
        s2InStop,
        s2StopUntilMs,
        s2BeforeFirstSweepDone,
        0, 180,
        4000UL, 2000UL,
        nowMs
      );
      s2At180 = atHigh; // endAngle 180 => atHigh true
    }
  }
}
