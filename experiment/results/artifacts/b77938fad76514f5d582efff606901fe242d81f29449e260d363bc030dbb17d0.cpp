#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// ----- Pin assignments (as required; Uno numeric pins) -----
// Arduino Uno: D2=2, D4=4, D5=5, D7=7, A0=14 (A7=21 etc.)
static const uint8_t PIN_LDR    = A0;  // 光敏电阻 A0
static const uint8_t PIN_BTN    = 2;   // 按钮 D2
static const uint8_t PIN_DHT     = 7;   // DHT11 D7
static const uint8_t PIN_TM_CLK = 4;   // TM1637 CLK = D4
static const uint8_t PIN_TM_DIO = 5;   // TM1637 DIO = D5

// ----- Devices -----
static DHT dht(PIN_DHT, DHT11);
static TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);

// ----- System state -----
enum DisplayMode : uint8_t {
  MODE_LIGHT = 0,
  MODE_TEMP  = 1,
  MODE_HUMID  = 2
};

static DisplayMode mode = MODE_LIGHT;

// Button debounce
static bool lastBtnStable = HIGH;
static bool lastBtnRead = HIGH;
static unsigned long lastDebounceMs = 0;
static const unsigned long debounceDelayMs = 35;

// Thresholds (for abnormal prompt)
static const int LIGHT_TOO_LOW_THRESHOLD = 300;     // ADC threshold (0-1023)
static const float TEMP_TOO_HIGH_THRESHOLD = 30.0f;  // °C

// Serial timing
static unsigned long lastSerialMs = 0;
static const unsigned long serialPeriodMs = 2000;

// ----- Helper functions -----
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static int readLightADC();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static float readTemperatureC();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static float readHumidityRH();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showOnTM1637(float value);
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showLightADCOnTM1637(int adc);
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool readButtonEdge();
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateDisplayForCurrentMode(float tC, float hRH, int lightADC);
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 133 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static int readLightADC() {
  return analogRead(PIN_LDR);
}

static float readTemperatureC() {
  float t = dht.readTemperature(); // default: Celsius
  if (isnan(t)) return NAN;
  return t;
}

static float readHumidityRH() {
  float h = dht.readHumidity();
  if (isnan(h)) return NAN;
}

static void showOnTM1637(float value) {
  if (isnan(value)) {
    tm1637.showNumberDecEx(0, 0b01000000, true); // approximate dashes-style
    return;
  }

  // Clamp to reasonable display range (-99.9..199.9)
  if (value > 199.9f) value = 199.9f;
  if (value < -99.9f) value = -99.9f;

  bool negative = value < 0;
  float absV = negative ? -value : value;

  // Scale by 10 for one decimal place.
  int scaled = (int)round(absV * 10.0f); // 23.4 -> 234

  // For tm1637 one-decimal display, use showNumberDecEx with decimal point bitmask.
  uint8_t dots = 0b01000000; // middle dot (common for this library)
  int displayInt = scaled;

  if (negative) displayInt = -displayInt;

  tm1637.showNumberDecEx(displayInt, dots, true);
}

static void showLightADCOnTM1637(int adc) {
  if (adc < 0) adc = 0;
  if (adc > 9999) adc = 9999;
  tm1637.showNumberDec(adc, false);
}

static bool readButtonEdge() {
  // Debounced falling edge (button press active LOW with pullup)
  bool reading = digitalRead(PIN_BTN);

  if (reading != lastBtnRead) {
    lastDebounceMs = millis();
    lastBtnRead = reading;
  }

  if (millis() - lastDebounceMs > debounceDelayMs) {
    if (reading != lastBtnStable) {
      lastBtnStable = reading;
      if (lastBtnStable == LOW) return true; // active press
    }
  }
  return false;
}

static void updateDisplayForCurrentMode(float tC, float hRH, int lightADC) {
  if (mode == MODE_LIGHT) {
    showLightADCOnTM1637(lightADC);
  } else if (mode == MODE_TEMP) {
    showOnTM1637(tC);
  } else {
    showOnTM1637(hRH);
  }
}

// ----- Arduino setup/loop -----
void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP);

  analogReference(DEFAULT);
  dht.begin();

  tm1637.setBrightness(0x0f);

  lastBtnStable = digitalRead(PIN_BTN);
  lastBtnRead = lastBtnStable;

  int light = readLightADC();
  showLightADCOnTM1637(light);

  Serial.begin(9600);
}

void loop() {
  // 1) Button: each press cycles display mode: light -> temp -> humidity -> light ...
  if (readButtonEdge()) {
    mode = (DisplayMode)(((uint8_t)mode + 1) % 3);

    // Update TM1637 immediately upon mode change (as required)
    if (mode == MODE_LIGHT) {
      showLightADCOnTM1637(readLightADC());
    } else if (mode == MODE_TEMP) {
      showOnTM1637(readTemperatureC());
    } else { // MODE_HUMID
      showOnTM1637(readHumidityRH());
    }
  }

  // 2) Periodic serial output every 2 seconds:
  // Output three complete data items, and abnormal prompt when light too low and temp too high.
  unsigned long now = millis();
  if (now - lastSerialMs >= serialPeriodMs) {
    lastSerialMs = now;

    int lightADC = readLightADC();
    float tempC = readTemperatureC();
    float humRH = readHumidityRH();

    Serial.print("LightADC=");
    Serial.print(lightADC);

    Serial.print(", TempC=");
    if (isnan(tempC)) Serial.print("NaN");
    else Serial.print(tempC, 1);

    Serial.print(", HumRH=");
    if (isnan(humRH)) Serial.print("NaN");
    else Serial.print(humRH, 1);

    Serial.println();

    // Abnormal prompt condition: 光照过低且温度过高
    bool abnormal = (lightADC < LIGHT_TOO_LOW_THRESHOLD) && (!isnan(tempC)) && (tempC > TEMP_TOO_HIGH_THRESHOLD);
    if (abnormal) {
      Serial.println("环境异常提示：光照过低且温度过高");
    }

    // Keep TM1637 updated continuously for the selected mode
    updateDisplayForCurrentMode(tempC, humRH, lightADC);
  }
}
