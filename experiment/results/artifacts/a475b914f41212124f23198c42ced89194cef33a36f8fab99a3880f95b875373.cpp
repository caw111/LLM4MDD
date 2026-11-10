#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------- Pin assignments (per authoritative wiring) --------
static const uint8_t TRIG_PIN = D8;     // HC-SR04 Trig = D8
static const uint8_t ECHO_PIN = D9;     // HC-SR04 Echo = D9

static const uint8_t POT_PIN  = A0;     // Potentiometer wiper = A0

static const uint8_t BTN_PIN  = D2;     // Mode toggle button = D2
static const uint8_t BUZZER_PIN = D6;   // Buzzer = D6

static const uint8_t TM1637_CLK = D4;    // TM1637 CLK = D4
static const uint8_t TM1637_DIO = D5;    // TM1637 DIO = D5

// -------- System behavior requirements --------
static const int AUTO_DISTANCE_CM = 15;   // Automatic mode threshold fixed at 15cm

// Button switching:
// - Button toggles between Auto/Manual modes
// - Debounce handled to avoid multiple toggles per press
static const unsigned long DEBOUNCE_MS = 50;
static const unsigned long LONG_PRESS_MS = 0; // not required; keep normal toggle only

// -------- Utilities --------
TM1637Display display(TM1637_CLK, TM1637_DIO);

// Display settings: show threshold in cm (0..99 is enough for common range)
static const uint8_t BRIGHTNESS = 7; // 0..7; adjust if needed

static bool isAutoMode = true;

static unsigned long lastDebounceTime = 0;
static int lastButtonReading = HIGH;
static int stableButtonState = HIGH;

// Convert potentiometer reading to a distance threshold in cm.
// Since the pot-to-range calibration is not specified, map to a reasonable range.
// Preserve requirement: manual mode threshold由电位器设置阈值.
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int potToThresholdCm(int analogValue);
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int measureDistanceCm();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplayForThreshold(int thresholdCm);
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void toggleMode();
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int potToThresholdCm(int analogValue) {
  // Map 0..1023 to 5..80 cm (avoid 0 and keep visible range)
  long mapped = map(analogValue, 0, 1023, 5, 80);
  if (mapped < 0) mapped = 0;
  if (mapped > 999) mapped = 999;
  return (int)mapped;
}

// Measure distance using HC-SR04
static int measureDistanceCm() {
  // Ensure clean trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(4);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse width
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL); // timeout ~30ms
  if (duration == 0) {
    // No echo received: treat as out of range (return large value)
    return 999;
  }

  // distance_cm = duration_us / 58
  int cm = (int)(duration / 58UL);
  return cm;
}

static void updateDisplayForThreshold(int thresholdCm) {
  // Show threshold in cm on TM1637.
  // We show up to 3 digits; TM1637Display shows decimal? We'll show integer.
  // If threshold exceeds 99, it will still display as three digits.
  display.setBrightness(BRIGHTNESS);

  // TM1637Display.showNumberDecEx allows formatting; keep simplest:
  display.showNumberDec(thresholdCm, false);
}

static void toggleMode() {
  isAutoMode = !isAutoMode;
  int threshold = isAutoMode ? AUTO_DISTANCE_CM : potToThresholdCm(analogRead(POT_PIN));
  updateDisplayForThreshold(threshold);
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BTN_PIN, INPUT_PULLUP); // button to GND when pressed
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  display.setBrightness(BRIGHTNESS);
  updateDisplayForThreshold(AUTO_DISTANCE_CM);

  lastButtonReading = digitalRead(BTN_PIN);
  stableButtonState = lastButtonReading;
  lastDebounceTime = millis();
}

void loop() {
  // -------- Handle button toggle with debounce --------
  int reading = digitalRead(BTN_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    if (reading != stableButtonState) {
      stableButtonState = reading;

      // On transition to pressed (LOW because INPUT_PULLUP)
      if (stableButtonState == LOW) {
        toggleMode();
      }
    }
  }

  // -------- Determine current threshold --------
  int thresholdCm;
  if (isAutoMode) {
    thresholdCm = AUTO_DISTANCE_CM;
  } else {
    // Manual mode: threshold from potentiometer
    thresholdCm = potToThresholdCm(analogRead(POT_PIN));
  }

  // -------- Update display in manual mode (show current threshold) --------
  // Requirement: “数码管显示当前阈值。”
  // To reflect knob changes, update display continuously in manual mode.
  if (!isAutoMode) {
    updateDisplayForThreshold(thresholdCm);
  } else {
    // Keep displaying auto fixed threshold
    // (UpdateDisplay called on toggle, but keep it harmless to update rarely)
    // We won't update every cycle to reduce flicker.
  }

  // -------- Measure distance and control buzzer --------
  int distanceCm = measureDistanceCm();

  // Requirement: Automatic mode fixed at 15cm; Manual threshold set by pot.
  // Alarm condition: distance <= threshold => alarm ON
  bool alarm = (distanceCm <= thresholdCm);

  if (alarm) {
    // Requirement only says "蜂鸣器 D6"—not timing pattern.
    // Use a simple audible tone: ON then brief off while looping.
    // Preserve behavior determinism: beep continuously while alarm condition holds.
    // If you want a fixed frequency, use tone().
    tone(BUZZER_PIN, 2000); // 2kHz
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // -------- Small loop delay to stabilize sensor --------
  delay(50);
}
