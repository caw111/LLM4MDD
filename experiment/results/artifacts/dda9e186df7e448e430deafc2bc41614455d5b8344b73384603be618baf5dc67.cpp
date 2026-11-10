#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// --- Pin assignments (UNO) ---
static const uint8_t PIN_LM35_A1       = A1; // analog input
static const uint8_t PIN_BUTTON_D2     = 2;  // D2 (active-low with pull-up)
static const uint8_t PIN_TM1637_CLK    = 4;  // D4
static const uint8_t PIN_TM1637_DIO    = 5;  // D5

// --- Serial ---
static const uint32_t SERIAL_BAUD = 9600;

// --- LM35 calibration (10mV/°C) ---
static const float VREF = 5.0f;
static const float ADC_FULL_SCALE = 1023.0f;
static const float LM35_MV_PER_C = 10.0f;

// --- History display hold time ---
static const uint16_t HISTORY_HOLD_MS = 3000; // show max temp for 3 seconds

TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

static float highestTempC = -1000.0f;

static bool showHistory = false;
static unsigned long showHistoryUntil = 0;

// --- Button debounce (active-low) ---
static bool lastStableState = HIGH;
static bool lastRawState = HIGH;
static unsigned long lastDebounceMs = 0;
static const unsigned long DEBOUNCE_MS = 30;

#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static float readTempC();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void formatAndShowTemp(float tempC);
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_014\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static float readTempC() {
  int raw = analogRead(PIN_LM35_A1);
  float voltage = (raw * VREF) / ADC_FULL_SCALE;                 // volts
  float tempC = (voltage * 1000.0f) / LM35_MV_PER_C;            // (mV)/(10mV/°C)
  return tempC;
}

static void formatAndShowTemp(float tempC) {
  // Display with 1 decimal place using TM1637Display showNumberDecEx(value, dots, leadingZeros)
  // Using tenths integer: tenths = round(tempC * 10)
  // showNumberDecEx expects integer; we place decimal after digit2 from left with dots=0x40.
  int16_t tenths = (int16_t)lroundf(tempC * 10.0f);

  int16_t intPart = tenths / 10;        // not used directly (kept for clarity)
  (void)intPart;

  long v = (long)tenths;
  display.showNumberDecEx(v, 0x40, true); // decimal after digit2
}

void setup() {
  pinMode(PIN_BUTTON_D2, INPUT_PULLUP);

  display.setBrightness(4);
  display.clear();

  Serial.begin(SERIAL_BAUD);

  highestTempC = readTempC();
  showHistory = false;
  showHistoryUntil = 0;

  formatAndShowTemp(highestTempC);
}

void loop() {
  float currentTempC = readTempC();

  if (currentTempC > highestTempC) {
    highestTempC = currentTempC;
  }

  // Button debounce and edge detect (press = LOW)
  bool rawState = digitalRead(PIN_BUTTON_D2);

  if (rawState != lastRawState) {
    lastDebounceMs = millis();
    lastRawState = rawState;
  }

  if ((millis() - lastDebounceMs) > DEBOUNCE_MS) {
    if (rawState != lastStableState) {
      lastStableState = rawState;

      // Detect falling edge: HIGH->LOW indicates a press
      if (lastStableState == LOW) {
        showHistory = true;
        showHistoryUntil = millis() + HISTORY_HOLD_MS;
      }
    }
  }

  float toDisplay = currentTempC;
  if (showHistory) {
    if ((long)(millis() - showHistoryUntil) >= 0) {
      showHistory = false;
    } else {
      toDisplay = highestTempC;
    }
  }

  formatAndShowTemp(toDisplay);

  // Serial output continuously: both current and historical highest
  Serial.print("CURRENT:");
  Serial.print(currentTempC, 1);
  Serial.print("C, HIGHEST:");
  Serial.print(highestTempC, 1);
  Serial.println("C");

  delay(200);
}
