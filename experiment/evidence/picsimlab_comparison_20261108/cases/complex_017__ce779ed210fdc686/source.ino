#include <Arduino.h>

static const uint8_t TRIG_PIN = D8;
static const uint8_t ECHO_PIN = D9;
static const uint8_t BUZZER_PIN = D6;
static const uint8_t BUTTON_PIN = D2;
static const uint8_t LED_PIN = D13;

static const uint32_t SERIAL_BAUD = 9600;

// Ultrasonic
static const uint16_t MAX_DISTANCE_CM = 400; // reasonable upper bound for HC-SR04

// Button (mute/unmute)
static const uint32_t DEBOUNCE_MS = 30;

// Tone behavior (distance-based)
static const uint16_t DIST_NEAR_CM = 20;    // near threshold
static const uint16_t DIST_MID_CM  = 80;    // mid threshold

// "提示频率" (beep repetition rate) mapping via buzzer tone on/off cadence.
// These are intentionally expressed as cadence rather than continuous tone.
static const uint32_t BEEP_PERIOD_NEAR_MS = 150; // faster beeps when near
static const uint32_t BEEP_PERIOD_MID_MS  = 350; // medium beeps when mid
static const uint32_t BEEP_PERIOD_FAR_MS  = 700; // slower beeps when far

// Beep on-time duration (part of the cadence)
static const uint16_t BEEP_ON_TIME_MS = 80;

// LED behavior
// - When not muted: LED off (optional; requirement only specifies muted behavior)
// - When muted: LED constant ON as an indicator
// But we also keep LED off in normal mode to preserve "常亮作为提示" during mute.
static const bool LED_ON_WHEN_MUTED = true;

// Sampling / measurement cadence
static const uint32_t MEASURE_INTERVAL_MS = 50;

static inline float constrainFloat(float x, float a, float b) {
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
  // Timeout to avoid blocking forever:
  // Max distance ~400cm -> round trip time ~ 400*2/34300 s = ~0.0233 s = 23300 us
  const unsigned long timeoutUs = 25000UL;

  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, timeoutUs);
  if (durationUs == 0) {
    // Out of range or timeout: treat as far (max)
    return MAX_DISTANCE_CM;
  }

  // Speed of sound: 343 m/s = 0.0343 cm/us
  // distance (cm) = durationUs * 0.0343 / 2
  float dist = (float)durationUs * 0.0343f / 2.0f;
  if (dist < 0) dist = 0;

  // Clamp
  if (dist > MAX_DISTANCE_CM) dist = MAX_DISTANCE_CM;

  return (uint16_t)(dist + 0.5f);
}

uint32_t beepPeriodForDistance(uint16_t d) {
  // Distance far => slower beep period; near => faster.
  if (d <= DIST_NEAR_CM) return BEEP_PERIOD_NEAR_MS;
  if (d <= DIST_MID_CM)  return BEEP_PERIOD_MID_MS;

  return BEEP_PERIOD_FAR_MS;
}

void applyMuteLed() {
  if (LED_ON_WHEN_MUTED) digitalWrite(LED_PIN, mute ? HIGH : LOW);
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

      // Toggle on press (assuming active LOW due to internal pull-up)
      if (stableButtonState == LOW) {
        mute = !mute;

        // Stop any active tone immediately when entering mute
        if (mute) {
          noTone(BUZZER_PIN);
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
      // Tone frequency (fixed); repetition frequency controlled by cadence
      // Choose a typical buzzer frequency:
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
    // "line_or_idle": output a line per measurement
    Serial.println(currentDistanceCm);
  }

  updateBuzzer(currentDistanceCm);
}