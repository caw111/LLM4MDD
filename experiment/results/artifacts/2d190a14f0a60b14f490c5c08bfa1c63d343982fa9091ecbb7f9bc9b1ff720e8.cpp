#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ---------- Pin assignments (Arduino Uno: use numeric pin IDs) ----------
static const uint8_t TRIG_PIN = 8;   // D8  -> HC-SR04 Trig
static const uint8_t ECHO_PIN = 9;   // D9  -> HC-SR04 Echo

static const uint8_t POT_PIN  = A0;  // A0  -> Potentiometer wiper

static const uint8_t BTN_PIN  = 2;   // D2  -> Button (to GND, INPUT_PULLUP)
static const uint8_t BUZZER_PIN = 6; // D6  -> Buzzer

static const uint8_t TM1637_CLK = 4;  // D4 -> TM1637 CLK
static const uint8_t TM1637_DIO = 5;  // D5 -> TM1637 DIO

// ---------- System behavior requirements ----------
static const int AUTO_DISTANCE_CM = 15;   // Auto mode fixed alarm threshold

// Button switching (debounce)
static const unsigned long DEBOUNCE_MS = 50;

// Convert potentiometer reading to a distance threshold in cm.
// Manual mode threshold由电位器设置阈值.
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static int potToThresholdCm(int analogValue);
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static int measureDistanceCm();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateDisplayForThreshold(int thresholdCm);
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void toggleMode();
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static int potToThresholdCm(int analogValue) {
  // Map 0..1023 to 5..80 cm (avoid 0 and keep visible range)
  long mapped = map(analogValue, 0, 1023, 5, 80);
  if (mapped < 0) mapped = 0;
  if (mapped > 999) mapped = 999;
  return (int)mapped;
}

// Measure distance using HC-SR04 (cm)
static int measureDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(4);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL); // timeout ~30ms
  if (duration == 0) {
    return 999; // out of range / no echo
  }

  return (int)(duration / 58UL);
}

// ---------- TM1637 display ----------
TM1637Display display(TM1637_CLK, TM1637_DIO);
static const uint8_t BRIGHTNESS = 7; // 0..7

static void updateDisplayForThreshold(int thresholdCm) {
  display.setBrightness(BRIGHTNESS);
  display.showNumberDec(thresholdCm, false); // integer display
}

// ---------- Mode state and debounce ----------
static bool isAutoMode = true;
static unsigned long lastDebounceTime = 0;
static int lastButtonReading = HIGH;
static int stableButtonState = HIGH;

static void toggleMode() {
  isAutoMode = !isAutoMode;

  int threshold = isAutoMode ? AUTO_DISTANCE_CM : potToThresholdCm(analogRead(POT_PIN));
  updateDisplayForThreshold(threshold);
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  display.setBrightness(BRIGHTNESS);
  updateDisplayForThreshold(AUTO_DISTANCE_CM);

  lastButtonReading = digitalRead(BTN_PIN);
  stableButtonState = lastButtonReading;
  lastDebounceTime = millis();
}

void loop() {
  // -------- Button toggle with debounce --------
  int reading = digitalRead(BTN_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    if (reading != stableButtonState) {
      stableButtonState = reading;

      // INPUT_PULLUP: pressed == LOW
      if (stableButtonState == LOW) {
        toggleMode();
      }
    }
  }

  // -------- Determine threshold --------
  int thresholdCm;
  if (isAutoMode) {
    thresholdCm = AUTO_DISTANCE_CM;
  } else {
    thresholdCm = potToThresholdCm(analogRead(POT_PIN));
  }

  // -------- Display current threshold --------
  // Requirement: “数码管显示当前阈值。”
  if (!isAutoMode) {
    updateDisplayForThreshold(thresholdCm);
  } // auto: already shown on toggle; keep behavior stable

  // -------- Measure distance and control buzzer --------
  int distanceCm = measureDistanceCm();

  // Alarm when distance <= threshold
  bool alarm = (distanceCm <= thresholdCm);

  if (alarm) {
    // Simple continuous tone while alarm is active
    tone(BUZZER_PIN, 2000); // 2kHz
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(50);
}
