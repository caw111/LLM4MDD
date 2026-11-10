#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// -------------------- Pin Assignments (per requirements) --------------------
static const uint8_t TRIG_PIN = D8;     // HC-SR04 Trig
static const uint8_t ECHO_PIN = D9;     // HC-SR04 Echo
static const uint8_t BUZZER_PIN = D6;   // Buzzer output
static const uint8_t BUTTON_PIN = D2;   // Mute/Unmute button
static const uint8_t LED_PIN = D13;     // LED indicator (mute mode: constant ON)

// -------------------- Serial Settings --------------------
static const uint32_t SERIAL_BAUD = 9600;

// -------------------- Distance/Buzzer Behavior --------------------
// Default behavior: adjust buzzer提示频率 based on distance.
static const uint16_t CALIBRATION_CM = 0; // keep explicit hook for potential future calibration

// Distance thresholds for mapping
// (All ranges are inclusive on lower bound; upper bound exclusive except last band.)
static const uint16_t T1_CM = 30;   // very close: fastest beeps
static const uint16_t T2_CM = 60;   // close: fast
static const uint16_t T3_CM = 100;  // medium: medium
static const uint16_t T4_CM = 150;  // far: slow
// beyond T4: no beeps

// Beep timing parameters (in ms) derived as "frequency" control.
// We treat each beep as a short ON pulse with OFF gap.
static const uint16_t BEEP_ON_MS_FAST   = 120;
static const uint16_t BEEP_OFF_MS_FAST  = 80;

static const uint16_t BEEP_ON_MS_MED    = 140;
static const uint16_t BEEP_OFF_MS_MED   = 180;

static const uint16_t BEEP_ON_MS_SLOW   = 160;
static const uint16_t BEEP_OFF_MS_SLOW  = 420;

static const uint16_t BEEP_ON_MS_VSLOW  = 180;
static const uint16_t BEEP_OFF_MS_VSLOW = 900;

// Buzzer tone settings (frequency) used when we "beep".
static const uint16_t BUZZ_TONE_HZ = 2000;

// -------------------- Button / Mute Behavior --------------------
// Button toggles mute on/off.
// When muted: buzzer canceled, LED constant ON, but serial still outputs distance info.
static const uint16_t DEBOUNCE_MS = 25;

// Debounce + toggle
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
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t readDistanceCm();
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBeepPattern(uint16_t cm);
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleBuzzerByPattern(bool shouldBeepNow);
#line 132 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 158 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t readDistanceCm() {
  // Trigger pulse: 10us HIGH
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width
  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, 30000UL); // ~max 5m, conservative timeout

  if (durationUs == 0) {
    // timeout / no echo
    return (uint16_t)MAX_DISTANCE_CM;
  }

  // Speed of sound: 343 m/s => 0.0343 cm/us
  // Distance (cm) = durationUs * 0.0343 / 2 = durationUs * 0.01715
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

  // If pattern indicates buzzing, generate short pulses with frequency
  // (Using tone() so it remains audible on the buzzer pin.)
  tone(BUZZER_PIN, BUZZ_TONE_HZ);
  isBeeping = true;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // assumes button wired to GND when pressed
  pinMode(LED_PIN, OUTPUT);

  // Default states: not muted, LED OFF, buzzer idle
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);

  // Serial
  Serial.begin(SERIAL_BAUD);

  // Initialize debounce readings
  lastButtonReading = digitalRead(BUTTON_PIN);
  stableButtonState = lastButtonReading;
  lastDebounceTime = millis();

  // Initialize beep pattern state
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

      // Detect press event: with INPUT_PULLUP, pressed == LOW
      if (stableButtonState == LOW) {
        muted = !muted;
        if (muted) {
          // Mute: buzzer canceled, LED constant ON
          noTone(BUZZER_PIN);
          digitalWrite(LED_PIN, HIGH);
          isBeeping = false;
        } else {
          // Unmute: LED OFF, buzzer resumes according to distance
          digitalWrite(LED_PIN, LOW);
          // Reset beep timing so changes happen immediately
          beepLastToggleMs = millis();
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
    // Simple line-oriented output: "distance_cm:XXX"
    Serial.print("distance_cm:");
    Serial.println(distanceCm);

    // Update beep pattern according to distance (default mode behavior)
    updateBeepPattern(distanceCm);

    // Reset beep cycle when pattern changes significantly
    // (Even if muted, we keep state consistent for when unmuted.)
    beepLastToggleMs = nowMs;
    isBeeping = false;

    if (!muted) {
      // Ensure buzzer follows the new pattern from this measurement boundary
      if (beepOnMs == 0) {
        noTone(BUZZER_PIN);
      } else {
        // Start immediate beep ON phase
        handleBuzzerByPattern(true);
      }
    }
  }

  // -------------------- Buzzer timing (only when not muted) --------------------
  if (!muted) {
    if (beepOnMs == 0) {
      // Far: no beep
      noTone(BUZZER_PIN);
      isBeeping = false;
    } else {
      unsigned long t = millis();
      if (!isBeeping) {
        // OFF phase: ensure tone is off; after off time, switch ON
        noTone(BUZZER_PIN);
        if (t - beepLastToggleMs >= beepOffMs) {
          beepLastToggleMs = t;
          handleBuzzerByPattern(true);
        }
      } else {
        // ON phase: tone on for beepOnMs, then go OFF
        if (t - beepLastToggleMs >= beepOnMs) {
          beepLastToggleMs = t;
          // Turn off
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
