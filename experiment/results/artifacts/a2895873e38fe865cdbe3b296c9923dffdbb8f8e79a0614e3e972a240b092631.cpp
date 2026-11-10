#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// ----- Pin assignments (per resolved configuration) -----
static const uint8_t DHT_PIN = 7;      // D7
static const uint8_t LED_PIN = 13;      // D13
static const uint8_t BUZZER_PIN = 6;    // D6
static const uint8_t POT_PIN = A0;      // A0
static const uint8_t TM1637_CLK = 4;    // D4
static const uint8_t TM1637_DIO = 5;    // D5

// ----- Sensors/Displays -----
static const uint8_t DHT_TYPE = DHT11;
DHT dht(DHT_PIN, DHT_TYPE);
TM1637Display display(TM1637_CLK, TM1637_DIO);

// ----- Requirements / Behavior -----
static const uint8_t TM1637_BRIGHTNESS = 7;

// Buzzer "cycle alarm" when temp > threshold:
// Implement a periodic alarm waveform: beep ON for BUZZ_ON_MS,
// OFF for BUZZ_OFF_MS, repeating.
static const uint16_t BUZZ_ON_MS  = 200;
static const uint16_t BUZZ_OFF_MS = 200;

// Map potentiometer A0 to temperature threshold (°C).
// Choose a practical range for DHT11 (0~50°C typical).
// Pot setting defines the alarm threshold.
static const float THRESH_MIN_C = 0.0f;
static const float THRESH_MAX_C = 50.0f;

// DHT11 read/update interval
static const uint32_t DHT_READ_INTERVAL_MS = 1000;

// Display update interval (keep responsive)
static const uint32_t DISPLAY_UPDATE_INTERVAL_MS = 250;

// ----- Utility -----
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readThresholdCFromPot();
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showTempAndThreshold(float tempC, float thrC);
#line 77 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC() {
  float t = dht.readTemperature(); // °C
  if (isnan(t)) return NAN;
  return t;
}

static float readThresholdCFromPot() {
  int raw = analogRead(POT_PIN); // 0..1023
  float ratio = raw / 1023.0f;
  float thr = THRESH_MIN_C + ratio * (THRESH_MAX_C - THRESH_MIN_C);
  return thr;
}

// Show temperature and threshold on TM1637.
// TM1637 has 4 digits; show:
// - First 2 digits: current temperature (with no decimals)
// - Last 2 digits: threshold (with no decimals)
// If you want a multiplexed approach with time-slicing, it can flicker.
// We'll instead pack both: [temp][thresh] using two digits each.
// Range: 0..99 fits. DHT11 temps typically 0..50; threshold mapped 0..50.
// We'll clamp to 0..99 and show both as integers.
static void showTempAndThreshold(float tempC, float thrC) {
  int tInt = (int)round(tempC);
  int thInt = (int)round(thrC);
  if (tInt < 0) tInt = 0;
  if (tInt > 99) tInt = 99;
  if (thInt < 0) thInt = 0;
  if (thInt > 99) thInt = 99;

  // Compose 4-digit number: tt hh (e.g., 23 15 => 2315)
  int packed = (tInt % 100) * 100 + (thInt % 100);

  display.showNumberDecEx(packed, 0b01000000, false, 4);
  // Bit0? The 0b01000000 sets leading zeros behavior depending on library.
  // Keep consistent display style.
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Pot pin is analog input; no pinMode required, but harmless.
  pinMode(POT_PIN, INPUT);

  dht.begin();

  display.setBrightness(TM1637_BRIGHTNESS);
  display.clear();
}

void loop() {
  static uint32_t lastDhtMs = 0;
  static uint32_t lastDisplayMs = 0;

  static float currentTempC = NAN;
  static float thresholdC = 0.0f;

  // Read threshold continuously so potentiometer changes take effect.
  thresholdC = readThresholdCFromPot();

  // Periodic DHT read
  uint32_t now = millis();
  if (now - lastDhtMs >= DHT_READ_INTERVAL_MS) {
    lastDhtMs = now;
    float t = readTemperatureC();
    if (!isnan(t)) currentTempC = t; // keep last valid value if a read fails
  }

  // Determine alarm condition
  bool alarm = false;
  if (!isnan(currentTempC)) {
    alarm = (currentTempC > thresholdC);
  }

  // LED behavior: when temperature exceeds threshold -> constant ON
  digitalWrite(LED_PIN, alarm ? HIGH : LOW);

  // Buzzer behavior: periodic alarm when temperature exceeds threshold
  static uint32_t buzzCycleStartMs = 0;
  static bool buzzOn = false;

  if (alarm) {
    if (buzzCycleStartMs == 0) buzzCycleStartMs = now; // initialize
    uint32_t elapsed = now - buzzCycleStartMs;

    uint32_t period = BUZZ_ON_MS + BUZZ_OFF_MS;
    uint32_t pos = elapsed % period;

    bool onNow = (pos < BUZZ_ON_MS);
    if (onNow != buzzOn) {
      buzzOn = onNow;
      digitalWrite(BUZZER_PIN, buzzOn ? HIGH : LOW);
    }
  } else {
    // When not alarming, ensure buzzer is off and reset cycle timing.
    buzzCycleStartMs = 0;
    buzzOn = false;
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Update TM1637 display: show current temperature and threshold
  if (now - lastDisplayMs >= DISPLAY_UPDATE_INTERVAL_MS) {
    lastDisplayMs = now;

    if (isnan(currentTempC)) {
      // If no valid temp yet, show "----" equivalent by showing 0s packed.
      showTempAndThreshold(0.0f, thresholdC);
    } else {
      showTempAndThreshold(currentTempC, thresholdC);
    }
  }
}
