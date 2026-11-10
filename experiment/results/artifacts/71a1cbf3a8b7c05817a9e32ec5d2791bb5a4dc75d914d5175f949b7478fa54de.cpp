#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Pins per requirement (Arduino UNO does NOT define D8/D9-style macros in all cores)
// Use explicit numeric pins to avoid compilation errors.
static const uint8_t TRIG_PIN   = 8;  // HC-SR04 Trig
static const uint8_t ECHO_PIN   = 9;  // HC-SR04 Echo
static const uint8_t POT_PIN    = A0; // Potentiometer input
static const uint8_t BTN_PIN    = 2;  // Mode switch button
static const uint8_t BUZZ_PIN   = 6;  // Buzzer output
static const uint8_t TM_CLK_PIN = 4; // TM1637 CLK
static const uint8_t TM_DIO_PIN = 5; // TM1637 DIO

// Mode thresholds
static const int AUTO_THRESHOLD_CM = 15; // fixed in auto mode

// Manual threshold parameters (map pot -> 1..200 cm)
static const int MANUAL_MIN_CM = 1;
static const int MANUAL_MAX_CM = 200;

// Timing / filtering
static const uint16_t SENSOR_MEASURE_INTERVAL_MS = 120;
static const uint32_t BUTTON_DEBOUNCE_MS = 40;
static const uint32_t BUTTON_LONGPRESS_REPEAT_MS = 0; // no repeating needed

// Display settings
TM1637Display display(TM_CLK_PIN, TM_DIO_PIN);

// State
bool isAutoMode = true;

uint32_t lastMeasureMs = 0;
uint16_t lastDistanceCm = 0;

// Button handling
bool lastBtnStable = false;
bool lastBtnReading = false;
uint32_t lastDebounceMs = 0;

uint32_t lastToggleMs = 0;

// Helpers
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
int readManualThresholdCm();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
uint16_t readDistanceCmHC_SR04();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void showThresholdOnDisplay(int thresholdCm);
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void beepOncePattern(uint16_t distanceCm, int thresholdCm);
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void updateBuzzer(bool alarmActive);
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void handleModeButton();
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
int readManualThresholdCm() {
  int raw = analogRead(POT_PIN); // 0..1023
  long mapped = map(raw, 0, 1023, MANUAL_MIN_CM, MANUAL_MAX_CM);
  if (mapped < MANUAL_MIN_CM) mapped = MANUAL_MIN_CM;
  if (mapped > MANUAL_MAX_CM) mapped = MANUAL_MAX_CM;
  return (int)mapped;
}

uint16_t readDistanceCmHC_SR04() {
  // Trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Echo with timeout
  // Max range ~400 cm. Speed of sound: 58us per cm one-way.
  // Round trip ~58us/cm -> 400 cm => 23200us
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 24000UL);
  if (duration == 0) return 0; // no echo / timeout

  // Convert to cm
  uint16_t cm = (uint16_t)(duration / 58UL);
  return cm;
}

void showThresholdOnDisplay(int thresholdCm) {
  // Display just the numeric value (e.g., 15)
  display.setBrightness(0x0f);
  display.showNumberDec(thresholdCm, false, 4, 0);
}

void beepOncePattern(uint16_t distanceCm, int thresholdCm) {
  // If alarm condition is met, beep.
  // Make the buzzer duration dependent on closeness.
  // Preserve concept; current implementation uses a fixed short pulse.
  (void)distanceCm;
  (void)thresholdCm;

  digitalWrite(BUZZ_PIN, HIGH);
  delay(80);
  digitalWrite(BUZZ_PIN, LOW);
}

void updateBuzzer(bool alarmActive) {
  static bool lastAlarm = false;
  static uint32_t lastBeepMs = 0;

  if (alarmActive) {
    uint32_t now = millis();
    const uint32_t BEEP_PERIOD_MS = 350;
    if (!lastAlarm || (now - lastBeepMs) >= BEEP_PERIOD_MS) {
      lastBeepMs = now;
      // Beep uses most recent distance and current threshold context
      int thr = isAutoMode ? AUTO_THRESHOLD_CM : readManualThresholdCm();
      beepOncePattern(lastDistanceCm, thr);
    }
    lastAlarm = true;
  } else {
    lastAlarm = false;
    digitalWrite(BUZZ_PIN, LOW);
  }
}

void handleModeButton() {
  // D2 with internal pullup: pressed => LOW
  bool reading = (digitalRead(BTN_PIN) == LOW);

  if (reading != lastBtnReading) {
    lastDebounceMs = millis();
    lastBtnReading = reading;
  }

  if ((millis() - lastDebounceMs) >= BUTTON_DEBOUNCE_MS) {
    if (reading != lastBtnStable) {
      lastBtnStable = reading;

      // Toggle on "pressed" (when pressed boolean becomes true)
      if (lastBtnStable) {
        uint32_t now = millis();
        if (now - lastToggleMs >= 200) {
          lastToggleMs = now;
          isAutoMode = !isAutoMode;

          int thresholdToShow = isAutoMode ? AUTO_THRESHOLD_CM : readManualThresholdCm();
          showThresholdOnDisplay(thresholdToShow);
        }
      }
    }
  }
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BTN_PIN, INPUT_PULLUP);

  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(BUZZ_PIN, LOW);

  display.setBrightness(0x0f);

  // Initial threshold display
  showThresholdOnDisplay(AUTO_THRESHOLD_CM);
}

void loop() {
  handleModeButton();

  // Periodic distance measurement
  uint32_t now = millis();
  if (now - lastMeasureMs >= SENSOR_MEASURE_INTERVAL_MS) {
    lastMeasureMs = now;
    lastDistanceCm = readDistanceCmHC_SR04();
  }

  int thresholdCm = isAutoMode ? AUTO_THRESHOLD_CM : readManualThresholdCm();

  // In manual mode, keep display updated as pot changes
  static uint32_t lastDisplayUpdateMs = 0;
  static int lastDisplayed = -1;
  if (!isAutoMode) {
    if (now - lastDisplayUpdateMs >= 120) {
      lastDisplayUpdateMs = now;
      if (thresholdCm != lastDisplayed) {
        lastDisplayed = thresholdCm;
        showThresholdOnDisplay(thresholdCm);
      }
    }
  }

  // Alarm condition: distance <= threshold (and distance > 0 to avoid false alarms)
  bool alarmActive = (lastDistanceCm > 0 && lastDistanceCm <= thresholdCm);

  updateBuzzer(alarmActive);
}
