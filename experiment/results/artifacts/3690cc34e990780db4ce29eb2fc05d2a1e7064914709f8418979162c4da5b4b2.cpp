#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

const uint8_t LED1_PIN = 2;
const uint8_t LED2_PIN = 3;
const uint8_t LED3_PIN = 4;

const uint8_t RGB_R_PIN = 5;
const uint8_t RGB_G_PIN = 6;
const uint8_t RGB_B_PIN = 7;

const uint8_t SERVO1_PWM_PIN = 8;
const uint8_t SERVO2_PWM_PIN = 9;

Servo servo1;
Servo servo2;

// Timing (per requirements)
static const unsigned long SWEEP_TIME_MS = 2000; // 0->180 or 180->0
static const unsigned long PAUSE_TIME_MS = 2000;  // pause at 0 and 180

// Additional: LED/RGB synchronization occurs when servos reach 0 or 180 in scanning seconds.
// That means only at those boundary instants; however we keep behavior aligned with servo
// boundary timing in the state machine below.
static const unsigned long HALF_SWEEP_MS = SWEEP_TIME_MS; // from boundary to boundary

enum Phase : uint8_t { PAUSE_AT_0, SWEEP_0_TO_180, PAUSE_AT_180, SWEEP_180_TO_0 };

Phase phase = PAUSE_AT_0;
unsigned long phaseStartMs = 0;

// Sequence indices
static const uint8_t LED_COUNT = 3;
static const uint8_t RGB_COUNT = 3;

uint8_t ledIndex = 0; // 0->led1,1->led2,2->led3
uint8_t rgbIndex = 0; // 0->red,1->green,2->blue

// Helper: set RGB based on current index
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGBByIndex(uint8_t idx);
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setLEDByIndex(uint8_t idx);
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void stopAllLEDs();
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void onServoBoundaryReached();
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 106 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGBByIndex(uint8_t idx) {
  // Common cathode: higher pin -> ON
  digitalWrite(RGB_R_PIN, idx == 0 ? HIGH : LOW);
  digitalWrite(RGB_G_PIN, idx == 1 ? HIGH : LOW);
  digitalWrite(RGB_B_PIN, idx == 2 ? HIGH : LOW);
}

// Helper: set LEDs based on current index; others off.
void setLEDByIndex(uint8_t idx) {
  digitalWrite(LED1_PIN, idx == 0 ? HIGH : LOW);
  digitalWrite(LED2_PIN, idx == 1 ? HIGH : LOW);
  digitalWrite(LED3_PIN, idx == 2 ? HIGH : LOW);
}

void stopAllLEDs() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
}

// Boundary instant handler: occurs when servos reach 0 or 180.
// At each boundary, blink next LED and flicker RGB synchronized.
void onServoBoundaryReached() {
  // Turn next LED on in sequence (turn previous off via setLEDByIndex)
  // and set RGB to corresponding color.
  ledIndex = (ledIndex + 1) % LED_COUNT;
  rgbIndex = (rgbIndex + 1) % RGB_COUNT;

  setLEDByIndex(ledIndex);
  setRGBByIndex(rgbIndex);
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  stopAllLEDs();

  // RGB starts red on power/reset
  rgbIndex = 0;
  setRGBByIndex(rgbIndex);

  // Attach servos
  servo1.attach(SERVO1_PWM_PIN);
  servo2.attach(SERVO2_PWM_PIN);

  // Initialize servos at 0 degrees
  servo1.write(0);
  servo2.write(0);

  // Initialize phase machine at PAUSE_AT_0
  phase = PAUSE_AT_0;
  phaseStartMs = millis();

  // LEDs must be OFF at initialization.
  // Do not turn on a first LED until the first boundary synchronization event.
  // Therefore, ledIndex is set such that first boundary will turn on led2 (after increment).
  // Alternatively, we can keep ledIndex=2 so first increment wraps to 0 (led1).
  // Choose ledIndex = LED_COUNT - 1 so first boundary makes ledIndex=0 (led1).
  ledIndex = LED_COUNT - 1;
}

void loop() {
  unsigned long now = millis();

  // Update servo positions according to current phase.
  unsigned long elapsed = now - phaseStartMs;

  // For sweep phases, map linearly 0->180 or 180->0 over SWEEP_TIME_MS.
  if (phase == SWEEP_0_TO_180) {
    if (elapsed >= SWEEP_TIME_MS) {
      // Clamp to boundary
      servo1.write(180);
      servo2.write(180);
    } else {
      float t = (float)elapsed / (float)SWEEP_TIME_MS; // 0..1
      int angle = (int)(t * 180.0f + 0.5f);
      servo1.write(angle);
      servo2.write(angle);
    }
  } else if (phase == SWEEP_180_TO_0) {
    if (elapsed >= SWEEP_TIME_MS) {
      servo1.write(0);
      servo2.write(0);
    } else {
      float t = (float)elapsed / (float)SWEEP_TIME_MS; // 0..1
      int angle = (int)((1.0f - t) * 180.0f + 0.5f);
      servo1.write(angle);
      servo2.write(angle);
    }
  } else {
    // Pauses: hold at boundary angles
    if (phase == PAUSE_AT_0) {
      servo1.write(0);
      servo2.write(0);
    } else { // PAUSE_AT_180
      servo1.write(180);
      servo2.write(180);
    }
  }

  // Phase transitions at exact boundaries/pause endpoints.
  // We must synchronize LED/RGB flickers with instants when servos reach 0 or 180.
  // Those are:
  // - End of PAUSE_AT_0 (servos at 0, then start sweep) -> boundary instant (0)
  // - End of SWEEP_0_TO_180 (servos arrive at 180) -> boundary instant (180)
  // - End of PAUSE_AT_180 (servos at 180, then start sweep) -> boundary instant (180)
  // - End of SWEEP_180_TO_0 (servos arrive at 0) -> boundary instant (0)
  bool boundaryEvent = false;

  if (phase == PAUSE_AT_0) {
    if (elapsed >= PAUSE_TIME_MS) {
      // At this moment servos are at 0 degrees.
      boundaryEvent = true;
      phase = SWEEP_0_TO_180;
      phaseStartMs = now;
    }
  } else if (phase == SWEEP_0_TO_180) {
    if (elapsed >= SWEEP_TIME_MS) {
      // At this moment servos reached 180 degrees.
      boundaryEvent = true;
      phase = PAUSE_AT_180;
      phaseStartMs = now;
    }
  } else if (phase == PAUSE_AT_180) {
    if (elapsed >= PAUSE_TIME_MS) {
      // At this moment servos are at 180 degrees.
      boundaryEvent = true;
      phase = SWEEP_180_TO_0;
      phaseStartMs = now;
    }
  } else if (phase == SWEEP_180_TO_0) {
    if (elapsed >= SWEEP_TIME_MS) {
      // At this moment servos reached 0 degrees.
      boundaryEvent = true;
      phase = PAUSE_AT_0;
      phaseStartMs = now;
    }
  }

  if (boundaryEvent) {
    onServoBoundaryReached();
    // Note: Servos are already at the boundary angles by construction.
    // LED/RGB flickers are synchronized to this boundary event.
  }

  // No further delay; loop keeps updating sweeps smoothly.
}
