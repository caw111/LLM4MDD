#include <Arduino.h>

// -------------------- Pin Assignments (UNO-compatible) --------------------
static const uint8_t TRIG_PIN = 8;   // HC-SR04 Trig (D8)
static const uint8_t ECHO_PIN = 9;   // HC-SR04 Echo (D9)
static const uint8_t BUZZER_PIN = 6; // Buzzer output (D6)
static const uint8_t BUTTON_PIN = 2; // Mute/Unmute button (D2)
static const uint8_t LED_PIN = 13;  // LED indicator (D13)

// -------------------- Serial Settings --------------------
static const uint32_t SERIAL_BAUD = 9600;

// -------------------- Distance/Buzzer Behavior --------------------
static const uint16_t CALIBRATION_CM = 0; // keep explicit hook

// Distance thresholds for mapping
static const uint16_t T1_CM = 30;   // very close: fastest beeps
static const uint16_t T2_CM = 60;   // close: fast
static const uint16_t T3_CM = 100;  // medium: medium
static const uint16_t T4_CM = 150;  // far: slow

// Beep timing parameters (ms)
static const uint16_t BEEP_ON_MS_FAST   = 120;
static const uint16_t BEEP_OFF_MS_FAST  = 80;

static const uint16_t BEEP_ON_MS_MED    = 140;
static const uint16_t BEEP_OFF_MS_MED   = 180;

static const uint16_t BEEP_ON_MS_SLOW   = 160;
static const uint16_t BEEP_OFF_MS_SLOW  = 420;

static const uint16_t BEEP_ON_MS_VSLOW  = 180;
static const uint16_t BEEP_OFF_MS_VSLOW = 900;

// Buzzer tone frequency
static const uint16_t BUZZ_TONE_HZ = 2000;

// -------------------- Button / Mute Behavior --------------------
static const uint16_t DEBOUNCE_MS = 25;

static bool muted = false;
static bool lastButtonReading = HIGH;
static bool stableButtonState = HIGH;
static unsigned long lastDebounceTime = 0;

// -------------------- Ultrasonic Timing --------------------
static const uint16_t MAX_DISTANCE_CM = 400;
static unsigned long lastMeasureMs = 0;
static const uint16_t MEASURE_PERIOD_MS = 60; // periodic measurement rate

// -------------------- Beep State Machine --------------------
static bool isBeeping = false; // within beep pulse
static unsigned long beepLastToggleMs = 0;
static unsigned long beepOnMs = 0;
static unsigned long beepOffMs = 0;

// -------------------- Helper: Read HC-SR04 distance --------------------
static uint16_t readDistanceCm() {
  // Trigger pulse: 10us HIGH
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width
  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, 30000UL); // conservative timeout

  if (durationUs == 0) {
    // timeout / no echo
    return (uint16_t)MAX_DISTANCE_CM;
  }

  // Distance (cm) = durationUs * 0.01715
  uint32_t dist = (uint32_t)(durationUs * 0.01715f);

  if (dist > MAX_DISTANCE_CM) dist = MAX_DISTANCE_CM;

  // Calibration hook
  if (CALIBRATION_CM > 0) {
    if (dist > CALIBRATION_CM) dist -= CALIBRATION_CM;
    else dist = 0;
  }

  return (uint16_t)dist;
}

// -------------------- Helper: Determine beep pattern by distance --------------------
static void updateBeepPattern(uint16_t cm) {
  // Default: when far enough, stop beeping.
  if (cm < T1_CM) {
    beepOnMs = BEEP_ON_MS_FAST;
    beepOffMs = BEEP_OFF_MS_FAST;
  } else if (cm < T2_CM) {
    beepOnMs = BEEP_ON_MS_FAST;
    beepOffMs = BEEP_OFF_MS_MED;
  } else if (cm < T3_CM) {
    beepOnMs = BEEP_ON_MS_MED;
    beepOffMs = BEEP_OFF_MS_MED;
  } else if (cm < T4_CM) {
    beepOnMs = BEEP_ON_MS_SLOW;
    beepOffMs = BEEP_OFF_MS_SLOW;
  } else {
    // no beeps beyond T4
    beepOnMs = 0;
    beepOffMs = 0;
  }
}

// -------------------- Helper: Beep state machine --------------------
static void handleBuzzerByPattern(bool shouldBeepNow) {
  if (!shouldBeepNow) {
    noTone(BUZZER_PIN);
    isBeeping = false;
    return;
  }

  tone(BUZZER_PIN, BUZZ_TONE_HZ);
  isBeeping = true;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // button wired to GND when pressed
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);

  Serial.begin(SERIAL_BAUD);

  lastButtonReading = digitalRead(BUTTON_PIN);
  stableButtonState = lastButtonReading;
  lastDebounceTime = millis();

  updateBeepPattern(MAX_DISTANCE_CM);
  beepLastToggleMs = millis();
  isBeeping = false;
}

void loop() {
  // -------------------- Button handling (mute toggle) --------------------
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    if (reading != stableButtonState) {
      stableButtonState = reading;

      // pressed == LOW with INPUT_PULLUP
      if (stableButtonState == LOW) {
        muted = !muted;

        if (muted) {
          noTone(BUZZER_PIN);
          digitalWrite(LED_PIN, HIGH); // mute: LED constant ON
          isBeeping = false;
        } else {
          digitalWrite(LED_PIN, LOW);  // unmute: LED OFF
          beepLastToggleMs = millis(); // reset timing
          isBeeping = false;
        }
      }
    }
  }

  // -------------------- Ultrasonic measurement + serial output --------------------
  unsigned long nowMs = millis();
  if (nowMs - lastMeasureMs >= MEASURE_PERIOD_MS) {
    lastMeasureMs = nowMs;

    uint16_t distanceCm = readDistanceCm();

    // Always output distance info even in mute mode
    Serial.print("distance_cm:");
    Serial.println(distanceCm);

    // Update buzzer pattern according to distance
    updateBeepPattern(distanceCm);

    // Reset beep cycle when new measurement is taken
    beepLastToggleMs = nowMs;
    isBeeping = false;

    if (!muted) {
      if (beepOnMs == 0) {
        noTone(BUZZER_PIN);
      } else {
        handleBuzzerByPattern(true); // immediate ON phase
      }
    }
  }

  // -------------------- Buzzer timing (only when not muted) --------------------
  if (!muted) {
    if (beepOnMs == 0) {
      noTone(BUZZER_PIN);
      isBeeping = false;
    } else {
      unsigned long t = millis();
      if (!isBeeping) {
        // OFF phase: keep tone off, switch ON after beepOffMs
        noTone(BUZZER_PIN);
        if (t - beepLastToggleMs >= beepOffMs) {
          beepLastToggleMs = t;
          handleBuzzerByPattern(true);
        }
      } else {
        // ON phase: tone on for beepOnMs then go OFF
        if (t - beepLastToggleMs >= beepOnMs) {
          beepLastToggleMs = t;
          noTone(BUZZER_PIN);
          isBeeping = false;
        }
      }
    }
  } else {
    // Muted: buzzer canceled, LED constant ON
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, HIGH);
  }
}