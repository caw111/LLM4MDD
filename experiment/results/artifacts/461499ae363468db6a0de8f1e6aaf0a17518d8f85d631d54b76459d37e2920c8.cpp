#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ---- Pin assignments (authoritative) ----
static const uint8_t TRIG_PIN = 8;   // HC-SR04 Trig
static const uint8_t ECHO_PIN = 9;   // HC-SR04 Echo
static const uint8_t CLK_PIN  = 4;   // TM1637 CLK
static const uint8_t DIO_PIN  = 5;   // TM1637 DIO

static const uint8_t LED_PIN  = 13;  // LED D13
static const uint8_t BUZZ_PIN = 6;   // Buzzer D6

// ---- Distance thresholds ----
static const uint8_t DIST_LIMIT_1 = 30; // <30 cm : LED on
static const uint8_t DIST_LIMIT_2 = 15; // <15 cm : slow beeping
static const uint8_t DIST_LIMIT_3 = 8;  // <8 cm : fast beeping

// Typical HC-SR04: distance(cm) = time(us)/58
static const uint8_t SPEED_DIV = 58;

// ---- Display ----
TM1637Display display(CLK_PIN, DIO_PIN);

// -------------------- Buzzer control (non-blocking) --------------------
enum BuzzMode : uint8_t { BUZZ_OFF = 0, BUZZ_SLOW, BUZZ_FAST };

static BuzzMode buzzMode = BUZZ_OFF;
static unsigned long lastToggleMs = 0;
static bool buzzLevelHigh = false;

// Slow: lower frequency, Fast: higher frequency (toggle period)
static const unsigned long SLOW_TOGGLE_PERIOD_MS = 250; // ~2 Hz
static const unsigned long FAST_TOGGLE_PERIOD_MS = 90;   // faster beeps

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateBuzzerMode(uint8_t distanceCm);
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void serviceBuzzer();
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint16_t readDistanceCm();
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showDistance(uint16_t distanceCm);
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_008\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateBuzzerMode(uint8_t distanceCm) {
  BuzzMode newMode = BUZZ_OFF;
  if (distanceCm < DIST_LIMIT_3) newMode = BUZZ_FAST;
  else if (distanceCm < DIST_LIMIT_2) newMode = BUZZ_SLOW;

  if (newMode != buzzMode) {
    buzzMode = newMode;
    // Reset buzzer state on mode change
    buzzLevelHigh = false;
    digitalWrite(BUZZ_PIN, LOW);
    lastToggleMs = millis();
  }
}

static void serviceBuzzer() {
  if (buzzMode == BUZZ_OFF) return;

  unsigned long now = millis();
  unsigned long period = (buzzMode == BUZZ_SLOW) ? SLOW_TOGGLE_PERIOD_MS : FAST_TOGGLE_PERIOD_MS;

  if (now - lastToggleMs >= period) {
    lastToggleMs = now;
    buzzLevelHigh = !buzzLevelHigh;
    digitalWrite(BUZZ_PIN, buzzLevelHigh ? HIGH : LOW);
  }
}

// -------------------- Ultrasonic measurement --------------------
static uint16_t readDistanceCm() {
  // Ensure clean trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // 10us trigger pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse width with timeout
  // Max measurable distance depends on timeout; here choose ~400cm max.
  const unsigned long timeoutUs = 40000UL * 2; // conservative
  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, timeoutUs);

  if (durationUs == 0) {
    return 0;
  }

  uint16_t distanceCm = (uint16_t)(durationUs / SPEED_DIV);
  return distanceCm;
}

// -------------------- Display helpers --------------------
static void showDistance(uint16_t distanceCm) {
  // Display distance in cm (0..99). Clamp to 99 for 2-digit display.
  if (distanceCm > 99) distanceCm = 99;
  display.showNumberDec(distanceCm, false, 2, 0);
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZ_PIN, LOW);

  display.setBrightness(0x0f); // bright
  display.clear();

  buzzMode = BUZZ_OFF;
  lastToggleMs = millis();
}

void loop() {
  uint16_t distanceCm = readDistanceCm();

  // LED: when distance < 30cm, LED on
  if (distanceCm < DIST_LIMIT_1) digitalWrite(LED_PIN, HIGH);
  else digitalWrite(LED_PIN, LOW);

  // Buzzer mode based on thresholds
  updateBuzzerMode((uint8_t)distanceCm);

  // Display distance
  showDistance(distanceCm);

  // Service buzzer timing (non-blocking)
  serviceBuzzer();

  // Measurement rate control
  delay(60);
}
