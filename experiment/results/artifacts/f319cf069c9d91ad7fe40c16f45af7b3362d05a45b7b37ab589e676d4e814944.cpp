#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t TRIG_PIN = D8;
const uint8_t ECHO_PIN = D9;

const uint8_t LED_PIN = D13;
const uint8_t BUZZER_PIN = D6;

const uint32_t SERIAL_BAUD = 9600;
const uint16_t IDLE_TIMEOUT_MS = 20;

const uint16_t NEAR_DISTANCE_CM = 10;   // within 10cm
const uint32_t ALERT_RECOVER_MS = 5000; // recover after 5s without near

// Timing/behavior
const uint16_t BEEP_COUNT = 3;
const uint16_t BEEP_ON_MS = 150;       // short鸣一次时长
const uint16_t BEEP_OFF_MS = 120;      // 间隔
const uint16_t LED_FAST_ON_MS = 80;
const uint16_t LED_FAST_OFF_MS = 80;

// Ultrasonic measurement
const uint16_t MAX_ECHO_WAIT_US = 30000; // ~5m max (approx)
const uint8_t SOUND_SPEED_CM_PER_US = 0; // unused placeholder to preserve clarity

#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static long readUltrasonicCM();
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void buzzShortBeepNTimes(uint8_t n);
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 77 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline long readUltrasonicCM() {
  // Trigger a 10us pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, MAX_ECHO_WAIT_US);
  if (duration == 0) {
    return -1; // no reading
  }

  // Convert time to distance (cm):
  // distance = (duration * speed_of_sound)/2
  // For cm: duration(us) / 58 ≈ cm
  long cm = (long)(duration / 58UL);
  return cm;
}

static inline void buzzShortBeepNTimes(uint8_t n) {
  // Using buzzer as tone output on UNO pin D6
  // Choose a typical frequency; exact is not specified, so keep consistent.
  const uint16_t freq = 2000;

  for (uint8_t i = 0; i < n; i++) {
    tone(BUZZER_PIN, freq);
    delay(BEEP_ON_MS);
    noTone(BUZZER_PIN);
    if (i + 1 < n) delay(BEEP_OFF_MS);
  }
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  Serial.begin(SERIAL_BAUD);

  // Ensure initial line/idle behavior won't block; reading is not required per spec.
  // idle_timeout_ms is handled inherently by non-blocking logic here (no serial reads).
}

void loop() {
  static bool alerted = false;
  static uint32_t lastNearMillis = 0;

  // Measure distance to determine proximity
  long cm = readUltrasonicCM();
  bool near = (cm >= 0 && cm <= (long)NEAR_DISTANCE_CM);

  uint32_t now = millis();

  // When detected within 10cm, enter alert/renew timer
  if (near) {
    lastNearMillis = now;

    // On first detection of a near event (or if previously recovered), perform actions:
    if (!alerted) {
      alerted = true;

      // welcome on serial
      Serial.println("welcome");

      // LED fast blink and buzzer short鸣 three times.
      // The buzzer should happen immediately on detection; LED continues flashing.
      buzzShortBeepNTimes(BEEP_COUNT);
    }
  }

  // If currently alerted, run LED fast blinking until 5s passes without near
  if (alerted) {
    if ((now - lastNearMillis) >= ALERT_RECOVER_MS) {
      // Restore standby
      alerted = false;
      digitalWrite(LED_PIN, LOW);
      noTone(BUZZER_PIN);
    } else {
      // Fast blinking LED while near within timeout window.
      static uint32_t ledToggleAt = 0;
      static bool ledState = false;

      uint32_t interval = ledState ? LED_FAST_OFF_MS : LED_FAST_ON_MS;
      if ((now - ledToggleAt) >= interval) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState ? HIGH : LOW);
        ledToggleAt = now;
      }
    }
  } else {
    // Standby: ensure outputs are off
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }

  // No other behavior required. Avoid blocking for serial due to timeout requirement.
  // (Serial idle timeout is effectively irrelevant since we are only printing "welcome".)
}
