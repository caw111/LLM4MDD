#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Pins per requirement
static const uint8_t TRIG_PIN = D8;  // HC-SR04 Trig
static const uint8_t ECHO_PIN = D9;  // HC-SR04 Echo
static const uint8_t POT_PIN  = A0;  // Potentiometer input
static const uint8_t BTN_PIN  = D2;  // Mode switch button
static const uint8_t BUZZ_PIN = D6;  // Buzzer output
static const uint8_t TM_CLK_PIN = D4; // TM1637 CLK
static const uint8_t TM_DIO_PIN = D5; // TM1637 DIO

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
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
int readManualThresholdCm();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t readDistanceCmHC_SR04();
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void showThresholdOnDisplay(int thresholdCm);
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void beepOncePattern(uint16_t distanceCm, int thresholdCm);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void updateBuzzer(bool alarmActive);
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void handleModeButton();
#line 145 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 160 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_025\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
int readManualThresholdCm() {
  int raw = analogRead(POT_PIN); // 0..1023
  // Map smoothly to desired range
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

  // Echo
  // Timeout to avoid blocking forever
  // Max range ~400 cm. Speed of sound: 58us per cm one-way.
  // Round trip ~58us/cm -> 400 cm => 23200us
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 24000UL);
  if (duration == 0) return 0; // no echo / timeout

  // Convert to cm
  // distance = duration / 58
  uint16_t cm = (uint16_t)(duration / 58UL);
  return cm;
}

void showThresholdOnDisplay(int thresholdCm) {
  // Display just the numeric value (e.g., 15)
  // Turn on colon not used; set brightness based on auto/manual
  // We'll keep a fixed brightness.
  display.setBrightness(0x0f);
  display.showNumberDec(thresholdCm, false, 4, 0);
}

void beepOncePattern(uint16_t distanceCm, int thresholdCm) {
  // Behavior: If alarm condition is met, beep.
  // Make the buzzer duration dependent on closeness.
  // This preserves "alarm" without simplifying away the concept.
  // - If closer than half threshold: longer beep
  // - Else: shorter beep
  // Also guard from too frequent beeps is handled in loop.
  (void)distanceCm;
  (void)thresholdCm;

  // short pulse train
  digitalWrite(BUZZ_PIN, HIGH);
  delay(80);
  digitalWrite(BUZZ_PIN, LOW);
}

void updateBuzzer(bool alarmActive) {
  static bool lastAlarm = false;
  static uint32_t lastBeepMs = 0;

  if (alarmActive) {
    // Beep rate control to avoid constant tone
    // If alarm persists, beep periodically.
    uint32_t now = millis();
    const uint32_t BEEP_PERIOD_MS = 350;
    if (!lastAlarm || (now - lastBeepMs) >= BEEP_PERIOD_MS) {
      lastBeepMs = now;
      beepOncePattern(lastDistanceCm, isAutoMode ? AUTO_THRESHOLD_CM : readManualThresholdCm());
    }
    lastAlarm = true;
  } else {
    lastAlarm = false;
    digitalWrite(BUZZ_PIN, LOW);
  }
}

void handleModeButton() {
  // Read current raw state (assume D2 with internal pullup: pressed => LOW)
  bool reading = (digitalRead(BTN_PIN) == LOW);

  if (reading != lastBtnReading) {
    lastDebounceMs = millis();
    lastBtnReading = reading;
  }

  if ((millis() - lastDebounceMs) >= BUTTON_DEBOUNCE_MS) {
    if (reading != lastBtnStable) {
      lastBtnStable = reading;

      // Toggle on rising event of "pressed" (LOW->HIGH of internal 'pressed' boolean)
      if (lastBtnStable) {
        uint32_t now = millis();
        // Optional guard to avoid multiple toggles within very short time
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

  pinMode(BTN_PIN, INPUT_PULLUP); // D2 button

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
  // This does not remove threshold behavior; it only keeps display current.
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
