#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// -------------------- Pin / Device Setup --------------------
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// TM1637: CLK=D4, DIO=D2 (as specified)
#define TM1637_CLK 4
#define TM1637_DIO 2
TM1637Display display(TM1637_CLK, TM1637_DIO);

// Fan PWM output: D5
const uint8_t FAN_PWM_PIN = 5;

// -------------------- Behavior Requirements --------------------
// Threshold: if temperature is above threshold, start fan
// (Pick a sensible default; adjust if your original requirement specified a value.)
const float TEMP_THRESHOLD_C = 28.0;

// PWM characteristics
const uint8_t FAN_PWM_ON = 255;   // full speed when above threshold
const uint8_t FAN_PWM_OFF = 0;    // stop when at/below threshold

// Display formatting
// Show temperature as xx.x if possible, otherwise integer.
const unsigned long DISPLAY_UPDATE_MS = 1000;
unsigned long lastDisplayUpdateMs = 0;

// Fan update control (avoid jitter by aligning with display update, but still responsive)
const unsigned long FAN_UPDATE_MS = 250;
unsigned long lastFanUpdateMs = 0;

// -------------------- Helpers --------------------
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC();
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateFan(float tempC);
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplay(float tempC);
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC() {
  // DHT11 temperature range: use Celsius
  float t = dht.readTemperature();
  if (isnan(t)) return NAN;
  return t;
}

static void updateFan(float tempC) {
  if (!isnan(tempC) && tempC > TEMP_THRESHOLD_C) {
    analogWrite(FAN_PWM_PIN, FAN_PWM_ON);
  } else {
    analogWrite(FAN_PWM_PIN, FAN_PWM_OFF);
  }
}

static void updateDisplay(float tempC) {
  if (isnan(tempC)) {
    // Show blank-ish / error indicator on TM1637.
    // TM1637Display supports showNumberDecEx, but for error we show "----".
    // We'll use -1 to trigger segment formatting is not guaranteed; instead show dashes by setting brightness low.
    display.clear();
    // Write "----" using raw segments is implementation-specific; do a simple safe indicator:
    // show 0 with decimal point and low brightness isn't ideal, but avoids undefined behavior.
    // Since requirement says preserve behavior but no explicit error behavior given, keep safe:
    display.showNumberDec(0, false, 2, 0); // minimal fallback
    return;
  }

  // Display temperature with one decimal if possible.
  // TM1637Display's showNumberDecEx supports decimal positions, but availability depends on library.
  int16_t t_int = (int16_t)(tempC * 10.0f); // tenths
  bool negative = (t_int < 0);
  if (negative) t_int = -t_int;

  uint8_t digits = 4; // typical 4-digit display

  // Convert tenths to integer with decimal:
  // Example: 25.3C -> tenths=253 -> showNumberDecEx(253, ..., 1 decimal at position 0?).
  // Library uses dots bitmask: 0b0001 = rightmost decimal, etc.
  // We'll show "25.3" as 253 with decimal at one position from right.
  // Approach: showNumberDecEx with decimal position at 1 (tenths place).
  // showNumberDecEx(value, dots, leading_zero, length)
  // We'll use length=digits to keep consistent.
  uint16_t valueToShow = (uint16_t)(t_int);

  // Determine decimal point: dots=0b0100 for rightmost? depends on library.
  // For TM1637Display (avishorp), decimal points mask:
  // bit0: 1st digit rightmost, bit1: 2nd, bit2: 3rd, bit3: 4th.
  // For "25.3" using 253, decimal after 2nd digit means dots at digit 2 (index 1 from right?) => bit2? Let's map:
  // Digits: [d1 d2 d3 d4], decimal after d2 => dot on d3? Actually decimal point displayed on digit after decimal.
  // We'll set dot on the third digit (thousands-based display): dots mask = 0b0100.
  const uint8_t DOT_MASK_TENTHS = 0x04;

  // Set brightness moderately
  display.setBrightness(0x0f);

  // If negative, showNumberDecEx handles minus sign automatically when within digit count.
  // Use showNumberDecEx with a signed value.
  int16_t signedTenths = negative ? -(int16_t)valueToShow : (int16_t)valueToShow;

  // Display tenths with one decimal place: 253 -> "25.3"
  display.showNumberDecEx(signedTenths, negative ? 0 : 0, true, digits);
  // Apply decimal point mask; library supports setting dots via parameter.
  // Re-display with decimal dots:
  display.showNumberDecEx(signedTenths, DOT_MASK_TENTHS, true, digits);
}

void setup() {
  // Serial not required by prompt
  // dht init
  dht.begin();

  // Fan PWM pin
  pinMode(FAN_PWM_PIN, OUTPUT);
  analogWrite(FAN_PWM_PIN, FAN_PWM_OFF);

  // TM1637 init
  display.setBrightness(0x0f);
  display.clear();
}

void loop() {
  unsigned long now = millis();

  static float lastTemp = NAN;

  // Read sensor (do at display update interval)
  if (now - lastDisplayUpdateMs >= DISPLAY_UPDATE_MS) {
    lastDisplayUpdateMs = now;
    float t = readTemperatureC();
    if (!isnan(t)) lastTemp = t;
    updateDisplay(lastTemp);
  }

  // Update fan more frequently but using last known temperature
  if (now - lastFanUpdateMs >= FAN_UPDATE_MS) {
    lastFanUpdateMs = now;
    updateFan(lastTemp);
  }
}
