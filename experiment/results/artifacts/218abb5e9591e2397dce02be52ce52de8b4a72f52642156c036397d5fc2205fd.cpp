#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>

// Pin assignments (Arduino UNO: AVR core may not define D8/D9/D6/D2/D13, so use numeric pins)
static const uint8_t TRIG_PIN   = 8;   // D8
static const uint8_t ECHO_PIN   = 9;   // D9
static const uint8_t BUZZER_PIN  = 6;   // D6 (tone output)
static const uint8_t BUTTON_PIN = 2;   // D2 (mute/unmute button)
static const uint8_t LED_PIN     = 13;  // D13

static const uint32_t SERIAL_BAUD = 9600;

// Ultrasonic
static const uint16_t MAX_DISTANCE_CM = 400; // reasonable upper bound for HC-SR04

// Button (mute/unmute)
static const uint32_t DEBOUNCE_MS = 30;

// Distance thresholds (distance-based cadence)
static const uint16_t DIST_NEAR_CM = 20; // near threshold
static const uint16_t DIST_MID_CM  = 80; // mid threshold

// "提示频率" (beep repetition rate) mapping via buzzer tone on/off cadence
static const uint32_t BEEP_PERIOD_NEAR_MS = 150; // faster beeps when near
static const uint32_t BEEP_PERIOD_MID_MS  = 350; // medium beeps when mid
static const uint32_t BEEP_PERIOD_FAR_MS  = 700; // slower beeps when far

// Beep on-time duration (part of the cadence)
static const uint16_t BEEP_ON_TIME_MS = 80;

// LED behavior
// - When muted: LED constant ON as an indicator
static const bool LED_ON_WHEN_MUTED = true;

// Sampling / measurement cadence
static const uint32_t MEASURE_INTERVAL_MS = 50;

#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint32_t clampU32(uint32_t x, uint32_t a, uint32_t b);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
uint16_t readDistanceCm();
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
uint32_t beepPeriodForDistance(uint16_t d);
#line 90 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void applyMuteLed();
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void updateButton();
#line 127 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void updateBuzzer(uint16_t d);
#line 155 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 174 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_017\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static inline uint32_t clampU32(uint32_t x, uint32_t a, uint32_t b) {
  if (x < a) return a;
  if (x > b) return b;
  return x;
}

volatile bool mute = false;

uint32_t lastMeasureMs = 0;
uint32_t lastToggleStableMs = 0;
int lastButtonReading = HIGH;
int stableButtonState = HIGH;

uint32_t beepStateLastMs = 0;
bool beepOn = false;

uint16_t currentDistanceCm = 0;

uint16_t readDistanceCm() {
  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Echo pulse width -> distance
  const unsigned long timeoutUs = 25000UL; // ~400cm round-trip upper bound
  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, timeoutUs);

  if (durationUs == 0) {
    // Out of range or timeout: treat as far (max)
    return MAX_DISTANCE_CM;
  }

  // Speed of sound: 343 m/s = 0.0343 cm/us
  // distance (cm) = durationUs * 0.0343 / 2
  float dist = (float)durationUs * 0.0343f / 2.0f;
  if (dist < 0) dist = 0;

  if (dist > (float)MAX_DISTANCE_CM) dist = (float)MAX_DISTANCE_CM;

  return (uint16_t)(dist + 0.5f);
}

uint32_t beepPeriodForDistance(uint16_t d) {
  // Distance far => slower beep period; near => faster.
  if (d <= DIST_NEAR_CM) return BEEP_PERIOD_NEAR_MS;
  if (d <= DIST_MID_CM)  return BEEP_PERIOD_MID_MS;
  return BEEP_PERIOD_FAR_MS;
}

void applyMuteLed() {
  if (LED_ON_WHEN_MUTED) {
    digitalWrite(LED_PIN, mute ? HIGH : LOW);
  } else {
    // If LED behavior were ever changed off, keep it off when not muted.
    digitalWrite(LED_PIN, LOW);
  }
}

void updateButton() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastToggleStableMs = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastToggleStableMs) > DEBOUNCE_MS) {
    if (stableButtonState != reading) {
      stableButtonState = reading;

      // Toggle on press (active LOW due to internal pull-up)
      if (stableButtonState == LOW) {
        mute = !mute;

        // Stop any active tone immediately when entering mute
        if (mute) {
          noTone(BUZZER_PIN);
          beepOn = false;
        }

        applyMuteLed();
      }
    }
  }
}

void updateBuzzer(uint16_t d) {
  if (mute) {
    noTone(BUZZER_PIN);
    beepOn = false;
    return;
  }

  uint32_t period = beepPeriodForDistance(d);

  uint32_t now = millis();
  if (!beepOn) {
    // Start beep when period elapses since last start
    if ((now - beepStateLastMs) >= period) {
      beepOn = true;
      beepStateLastMs = now;
      // Fixed tone frequency; repetition cadence controlled by period
      tone(BUZZER_PIN, 2000);
    }
  } else {
    // Turn off after on-time
    if ((now - beepStateLastMs) >= BEEP_ON_TIME_MS) {
      beepOn = false;
      beepStateLastMs = now;
      noTone(BUZZER_PIN);
    }
  }
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  noTone(BUZZER_PIN);

  mute = false;
  stableButtonState = digitalRead(BUTTON_PIN);
  lastButtonReading = stableButtonState;
  lastToggleStableMs = millis();

  applyMuteLed();
  Serial.begin(SERIAL_BAUD);
}

void loop() {
  updateButton();

  uint32_t now = millis();
  if ((now - lastMeasureMs) >= MEASURE_INTERVAL_MS) {
    lastMeasureMs = now;

    currentDistanceCm = readDistanceCm();

    // Serial output of distance info (always, even when muted)
    Serial.println(currentDistanceCm);
  }

  updateBuzzer(currentDistanceCm);
}
