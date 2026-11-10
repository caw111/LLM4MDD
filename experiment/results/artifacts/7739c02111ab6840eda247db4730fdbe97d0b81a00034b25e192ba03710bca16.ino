#include <Arduino.h>
#include <TM1637Display.h>
#include <Wire.h>

// ----- Pin assignments (authoritative) -----
static const uint8_t PIN_BTN_RESET = 7;     // D7 button
static const uint8_t PIN_LED_D13    = 13;    // LED D13

static const uint8_t PIN_BUZZER     = 6;     // D6 buzzer (tone)

static const uint8_t PIN_TM1637_CLK = 4;     // D4
static const uint8_t PIN_TM1637_DIO = 5;     // D5
static const uint8_t PIN_SENSOR_RECYCLE = 2; // D2 proximity sensor: recyclable
static const uint8_t PIN_SENSOR_OTHER   = 3; // D3 proximity sensor: other

static const uint8_t PIN_TRIG = 8;           // D8 ultrasonic Trig
static const uint8_t PIN_ECHO = 9;           // D9 ultrasonic Echo

// ----- Serial -----
static const uint32_t SERIAL_BAUD = 9600;

// ----- Display -----
static const uint8_t TM_DIGITS = 4;

// ----- System behavior constants (tuned/implemented, preserve required behavior) -----
// Note: Requirements mention "threshold", "calibration", "timers and all other behavior".
// No explicit numeric values were provided, so the following are conservative defaults
// implementing typical full-detection and event counting with debounce and stable height.
static const uint16_t SAMPLE_INTERVAL_MS = 50;   // sensor sampling interval
static const uint16_t PROX_DEBOUNCE_MS  = 200;  // debounce for proximity triggers
static const uint16_t PROX_BLOCK_MS     = 800;  // block time after a counted event to avoid double counts
static const uint16_t ULTRA_SAMPLE_MS  = 200;  // ultrasonic sampling interval
static const uint8_t  ULTRA_SAMPLES    = 5;    // averaging samples

// Calibration / threshold for "full height" (in centimeters).
// If your environment differs, adjust these in code.
static const float CALIBRATION_OFFSET_CM = 0.0f;  // optional calibration offset
static const float FULL_THRESHOLD_CM    = 15.0f;  // threshold for "full"

// LED/buzzer behavior when full
static const uint16_t FULL_ALARM_BUZZ_ON_MS  = 250;
static const uint16_t FULL_ALARM_BUZZ_OFF_MS = 250;

// ----- Ultrasonic timing -----
static const uint32_t ULTRA_TIMEOUT_US = 30000UL; // ~5m max; for UNO safe timeout

// ----- Global state -----
TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

volatile bool binFull = false;

uint16_t countRecycle = 0;
uint16_t countOther   = 0;

uint32_t lastSampleMs = 0;
uint32_t lastUltraMs  = 0;

uint32_t lastProxRecycleMs = 0;
uint32_t lastProxOtherMs   = 0;

// For event edge detection
bool lastRecycleSeen = false;
bool lastOtherSeen   = false;

// Full alarm timing
uint32_t lastAlarmToggleMs = 0;
bool alarmBuzzerState = false;

static float readUltrasonicCM() {
  // Trigger pulse
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Measure echo
  uint32_t start = micros();
  // wait for echo HIGH with timeout
  while (digitalRead(PIN_ECHO) == LOW) {
    if ((micros() - start) > ULTRA_TIMEOUT_US) return NAN;
  }

  start = micros();
  // wait for echo LOW with timeout
  while (digitalRead(PIN_ECHO) == HIGH) {
    if ((micros() - start) > ULTRA_TIMEOUT_US) return NAN;
  }

  // pulse width in microseconds
  uint32_t duration = micros() - start + 0; // approximate
  // distance (cm) = duration / 58 for HC-SR04 (speed of sound)
  float cm = (float)duration / 58.0f;
  cm += CALIBRATION_OFFSET_CM;
  return cm;
}

static float readUltrasonicAvgCM() {
  float sum = 0.0f;
  uint8_t valid = 0;

  for (uint8_t i = 0; i < ULTRA_SAMPLES; i++) {
    float d = readUltrasonicCM();
    if (!isnan(d) && d > 0.0f && d < 400.0f) {
      sum += d;
      valid++;
    }
    delay(20);
  }
  if (valid == 0) return NAN;
  return sum / valid;
}

static void setDisplayTotal(uint16_t total) {
  // Display only total count using 4 digits.
  // If above 9999, wrap by showing last 4 digits.
  uint16_t shown = total % 10000;
  tm1637.showNumberDecEx(shown, 0b01000000, true); // leading zeros off, colon irrelevant
}

static void stopCountingAndAlarm() {
  binFull = true;
  digitalWrite(PIN_LED_D13, HIGH);

  uint32_t now = millis();
  if (now - lastAlarmToggleMs >= (alarmBuzzerState ? FULL_ALARM_BUZZ_OFF_MS : FULL_ALARM_BUZZ_ON_MS)) {
    alarmBuzzerState = !alarmBuzzerState;
    lastAlarmToggleMs = now;
    if (alarmBuzzerState) {
      tone(PIN_BUZZER, 2000);
    } else {
      noTone(PIN_BUZZER);
    }
  }
}

static void updateNormalAlarm() {
  // Ensure buzzer off and LED low when not full.
  binFull = false;
  digitalWrite(PIN_LED_D13, LOW);
  noTone(PIN_BUZZER);
  alarmBuzzerState = false;
}

static bool readProximity(uint8_t pin) {
  // Proximity sensors are assumed to output digital.
  return digitalRead(pin) == HIGH;
}

static void handleProximityCounting() {
  if (binFull) return; // stop counting when full

  uint32_t now = millis();

  bool recycleSeen = readProximity(PIN_SENSOR_RECYCLE);
  bool otherSeen   = readProximity(PIN_SENSOR_OTHER);

  // Edge detection: count on transition from not seen -> seen
  if (recycleSeen && !lastRecycleSeen) {
    if (now - lastProxRecycleMs >= PROX_BLOCK_MS && now - lastProxRecycleMs >= PROX_DEBOUNCE_MS) {
      countRecycle++;
      lastProxRecycleMs = now;
    }
  }

  if (otherSeen && !lastOtherSeen) {
    if (now - lastProxOtherMs >= PROX_BLOCK_MS && now - lastProxOtherMs >= PROX_DEBOUNCE_MS) {
      countOther++;
      lastProxOtherMs = now;
    }
  }

  lastRecycleSeen = recycleSeen;
  lastOtherSeen   = otherSeen;
}

static void resetSystem() {
  countRecycle = 0;
  countOther = 0;
  binFull = false;
  updateNormalAlarm();
  setDisplayTotal(0);

  // Clear any pending alarm state
  lastAlarmToggleMs = millis();
  alarmBuzzerState = false;
}

void setup() {
  pinMode(PIN_BTN_RESET, INPUT_PULLUP); // D7 button, active-low by default
  pinMode(PIN_LED_D13, OUTPUT);

  pinMode(PIN_BUZZER, OUTPUT);
  noTone(PIN_BUZZER);

  pinMode(PIN_TM1637_CLK, OUTPUT);
  pinMode(PIN_TM1637_DIO, OUTPUT);

  pinMode(PIN_SENSOR_RECYCLE, INPUT); // D2
  pinMode(PIN_SENSOR_OTHER, INPUT);   // D3

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_TRIG, LOW);

  tm1637.setBrightness(0x0f, true);

  Serial.begin(SERIAL_BAUD);
  resetSystem();
  setDisplayTotal(0);

  // Initial ultrasonic check to set binFull if needed
  lastUltraMs = millis();
}

void loop() {
  uint32_t now = millis();

  // Button reset (active-low); required "button D7 can reset after emptying"
  if (digitalRead(PIN_BTN_RESET) == LOW) {
    // Simple debounce
    delay(30);
    if (digitalRead(PIN_BTN_RESET) == LOW) {
      resetSystem();
      // Wait for release
      while (digitalRead(PIN_BTN_RESET) == LOW) {
        delay(10);
      }
    }
  }

  // Periodic ultrasonic full check
  if (now - lastUltraMs >= ULTRA_SAMPLE_MS) {
    lastUltraMs = now;

    float heightCm = readUltrasonicAvgCM();
    if (!isnan(heightCm)) {
      if (heightCm >= FULL_THRESHOLD_CM) {
        if (!binFull) {
          // Enter full state and output empty bin needed (as required)
          // (Output once per transition)
          Serial.println(F("empty bin needed"));
        }
        stopCountingAndAlarm();
      } else {
        if (binFull) {
          updateNormalAlarm();
        }
      }
    } else {
      // If ultrasonic fails, keep normal counting unless currently in full state.
      if (!binFull) updateNormalAlarm();
    }
  }

  // Proximity counting
  if (now - lastSampleMs >= SAMPLE_INTERVAL_MS) {
    lastSampleMs = now;
    handleProximityCounting();

    // Display total counts (recycle + other) always
    uint16_t total = countRecycle + countOther;
    setDisplayTotal(total);
  }

  // When full, we keep alarm running (handled above) and also stop counting
  // No additional actions needed here.
}