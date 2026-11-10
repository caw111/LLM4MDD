#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t PIN_LED_PWM = 9;     // PWM LED
const uint8_t PIN_BUTTON  = 2;     // Button (toggle auto/force-on)
const uint8_t PIN_SUNLIGHT = A0;    // LDR to A0 (analog)
const uint8_t PIN_ULTR_TRIG = 8;    // HC-SR04 Trig
const uint8_t PIN_ULTR_ECHO = 3;    // HC-SR04 Echo

// ---- Behavior requirements (energy-saving illumination) ----
static const uint16_t DARK_THRESHOLD = 550;  // "Only when environment is dark"
static const uint32_t MEASURE_INTERVAL_MS = 150; // sensor check interval
static const uint16_t DISTANCE_NEAR_CM = 120; // "detect people nearby" distance

// Force-on / auto mode
// - Button toggles: auto mode <-> forced constant on
static const uint32_t DEBOUNCE_MS = 50;

enum Mode : uint8_t { AUTO_MODE = 0, FORCE_ON_MODE = 1 };

Mode mode = AUTO_MODE;
bool ledState = false;

static uint32_t lastMeasureMs = 0;

// Button debounce
static bool lastButtonReading = HIGH;
static bool stableButtonState = HIGH;
static uint32_t lastDebounceChangeMs = 0;

// Ultrasonic helper: returns distance in cm (approx). If invalid, returns a large number.
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t readDistanceCm();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLed(bool on);
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t readDistanceCm() {
  // Ensure trigger low
  digitalWrite(PIN_ULTR_TRIG, LOW);
  delayMicroseconds(2);

  // Trigger pulse 10us
  digitalWrite(PIN_ULTR_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTR_TRIG, LOW);

  // Read echo with timeout to avoid blocking forever
  // Max distance ~ 400 cm, speed of sound ~ 58us/cm round-trip => 2*400*58 = 46400us
  // Use a bit above for safety.
  unsigned long duration = pulseIn(PIN_ULTR_ECHO, HIGH, 50000UL);
  if (duration == 0) return 10000; // timeout/no echo

  // duration is microseconds for round trip
  // distance_cm = duration / 58
  uint16_t dist = (uint16_t)(duration / 58UL);
  return dist;
}

static void setLed(bool on) {
  ledState = on;
  if (on) {
    // PWM LED at full brightness; keeps behavior deterministic.
    analogWrite(PIN_LED_PWM, 255);
  } else {
    analogWrite(PIN_LED_PWM, 0);
  }
}

void setup() {
  pinMode(PIN_LED_PWM, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  pinMode(PIN_ULTR_TRIG, OUTPUT);
  pinMode(PIN_ULTR_ECHO, INPUT);

  // Start in auto mode (energy-saving)
  mode = AUTO_MODE;
  setLed(false);

  // Initialize debounce states
  lastButtonReading = digitalRead(PIN_BUTTON);
  stableButtonState = lastButtonReading;
  lastDebounceChangeMs = millis();

  // Ensure proper trig initial state
  digitalWrite(PIN_ULTR_TRIG, LOW);
}

void loop() {
  // --- Button handling: toggle force-on and auto mode ---
  bool reading = digitalRead(PIN_BUTTON);

  if (reading != lastButtonReading) {
    lastDebounceChangeMs = millis();
    lastButtonReading = reading;
  }

  if (millis() - lastDebounceChangeMs > DEBOUNCE_MS) {
    if (reading != stableButtonState) {
      stableButtonState = reading;

      // Button press: INPUT_PULLUP so LOW means pressed
      if (stableButtonState == LOW) {
        if (mode == AUTO_MODE) {
          mode = FORCE_ON_MODE;
          setLed(true); // Force constant ON
        } else {
          mode = AUTO_MODE;
          // In auto mode, LED is decided by LDR + proximity
          setLed(false);
        }
      }
    }
  }

  // --- Auto mode: only light when dark and someone is near ---
  if (mode == AUTO_MODE) {
    uint32_t now = millis();
    if (now - lastMeasureMs >= MEASURE_INTERVAL_MS) {
      lastMeasureMs = now;

      int ldr = analogRead(PIN_SUNLIGHT);

      // Determine darkness
      bool isDark = (ldr < (int)DARK_THRESHOLD);

      // Determine proximity (ultrasonic)
      uint16_t distCm = readDistanceCm();
      bool someoneNear = (distCm <= DISTANCE_NEAR_CM);

      // Only when both conditions true
      setLed(isDark && someoneNear);
    }
  } else {
    // FORCE_ON_MODE: LED must remain ON
    if (!ledState) setLed(true);
  }
}
