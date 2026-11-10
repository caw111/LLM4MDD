#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

#define DHT_PIN 7        // D7
#define DHT_TYPE DHT11

#define FAN_PWM_PIN 5    // D5 (PWM)
#define BUTTON_PIN 2     // D2

#define TM1637_CLK 4     // D4
#define TM1637_DIO 3     // D3

DHT dht(DHT_PIN, DHT_TYPE);
TM1637Display display(TM1637_CLK, TM1637_DIO);

static const uint8_t SEG_BRIGHTNESS = 7; // 0..7, safe default

// Temperature control parameters (preserve requested behavior)
static const float FAN_ON_TEMP_C = 30.0f;

// Map temperature to fan duty:
// - Below or equal 30°C: fan off
// - Above 30°C: duty increases with temperature
// (Choose a sensible upper bound to avoid 100% beyond a typical range)
static const float TEMP_FOR_100_PERCENT_C = 45.0f; // 30C->0%, 45C->100%

// Timing
static const unsigned long SENSOR_INTERVAL_MS = 2000UL;   // DHT reads are slow; keep modest
static const unsigned long DEBOUNCE_MS = 50UL;              // button debounce

// Display mode
enum DisplayMode : uint8_t { SHOW_TEMP = 0, SHOW_HUM = 1 };
volatile DisplayMode mode = SHOW_TEMP;

// Button debounce state
static bool lastButtonLevel = HIGH;
static bool stableButtonLevel = HIGH;
static unsigned long lastDebounceTime = 0;

// Button press detection
static bool consumeEdge = false;

// Last sensor values
static float lastTempC = NAN;
static float lastHum = NAN;

// Fan control helper
static int fanDutyFromTemp(float tempC) {
  if (!isfinite(tempC)) return 0;
  if (tempC <= FAN_ON_TEMP_C) return 0;

  float t = tempC;
  if (t >= TEMP_FOR_100_PERCENT_C) return 255;

  float ratio = (t - FAN_ON_TEMP_C) / (TEMP_FOR_100_PERCENT_C - FAN_ON_TEMP_C); // 0..1
  if (ratio < 0.0f) ratio = 0.0f;
  if (ratio > 1.0f) ratio = 1.0f;

  int duty = (int)roundf(ratio * 255.0f);
  if (duty < 0) duty = 0;
  if (duty > 255) duty = 255;
  return duty;
}

static void updateFan(float tempC) {
  int duty = fanDutyFromTemp(tempC);
  analogWrite(FAN_PWM_PIN, duty);
}

// Display helper:
// DHT11 values might be fractional (library returns float). Display with one decimal if needed.
// TM1637 4 digits: show e.g. "23.4" or "55.0"
// If mode is humidity: show "HUM" isn't possible reliably; use numeric with percent.
static void showTemperature(float tempC) {
  if (!isfinite(tempC)) {
    display.showNumberDecEx(0, 0b01000000, true, 4); // "----" style isn't supported by all variants; fallback 0000.
    return;
  }

  // Scale to one decimal: 23.4 => 234
  int scaled = (int)roundf(tempC * 10.0f); // includes sign if negative

  bool neg = scaled < 0;
  if (neg) scaled = -scaled;

  int intPart = scaled / 10;
  int frac = scaled % 10;

  // Construct digits: [intPart][.][frac]
  // We'll use showNumberDecEx with decimal point.
  // showNumberDecEx expects value without sign via negative flag for some libs; easier: set negative sign by using 2's complement? Not.
  // We'll limit to 0..999.99 typical for DHT11, so assume non-negative.
  // If negative occurs, show 0.0 to avoid weirdness.
  if (neg) {
    intPart = 0;
    frac = 0;
  }

  int valueToShow = intPart * 10 + frac; // e.g. 23.4 -> 234
  // Place decimal point at the correct position using decimal bit mask.
  // decimal mask bits: from library: bit0..bit3 correspond to positions.
  // For "23.4": digits: [2][3][.][4] => decimal at third digit from left? In 4-digit display positions:
  // We'll set decimal point on the second digit from right (digit index 1) if that matches typical TM1637Display.
  // TM1637Display library uses:
  // showNumberDecEx(num, dots, leadingZeros, length)
  // where dots is 0b01000000 for colon? Actually depends; common mapping:
  // dot position bits: DP1=0x80, DP2=0x40, DP3=0x20, DP4=0x10 in some libs.
  // The provided constructor is for common "avrm" TM1637Display by "Avishay Orpaz".
  // In that library: dots parameter uses bit mask for decimal points:
  // 0x80=0b10000000 => DP1 (leftmost)
  // 0x40 => DP2
  // 0x20 => DP3
  // 0x10 => DP4 (rightmost)
  // For "23.4": decimal after second digit (between 3 and 4) => DP3? Let's do by example:
  // digits: D1 D2 D3 D4 (left to right)
  // "23.4": D1='2', D2='3', D3=' ', DP, D4='4' ; actually dot is on D2 or between D2 and D4 -> usually dot after D2, i.e., DP2.
  // We'll choose DP2 (0x40).
  uint8_t dots = 0x40;

  display.showNumberDecEx(valueToShow, dots, false, 4);
}

static void showHumidity(float hum) {
  if (!isfinite(hum)) {
    display.showNumberDecEx(0, 0, true, 4);
    return;
  }

  // Display with one decimal: 55.0 => 550
  int scaled = (int)roundf(hum * 10.0f);
  if (scaled < 0) scaled = 0;

  int intPart = scaled / 10;
  int frac = scaled % 10;

  int valueToShow = intPart * 10 + frac; // e.g. 55.0 -> 550

  // For "55.0": digits [5][5][.][0] -> decimal after second digit => DP2 (0x40)
  uint8_t dots = 0x40;

  display.showNumberDecEx(valueToShow, dots, false, 4);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(FAN_PWM_PIN, OUTPUT);
  analogWrite(FAN_PWM_PIN, 0);

  dht.begin();

  display.setBrightness(SEG_BRIGHTNESS);
  // Initial display: temperature mode as required
  mode = SHOW_TEMP;

  // Initialize button state
  stableButtonLevel = digitalRead(BUTTON_PIN);
  lastButtonLevel = stableButtonLevel;
  lastDebounceTime = millis();
}

void loop() {
  // ---- Button handling with debounce (switch display mode each press) ----
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonLevel) {
    lastDebounceTime = millis();
    lastButtonLevel = reading;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    if (reading != stableButtonLevel) {
      stableButtonLevel = reading;

      // Detect falling edge (pressed because INPUT_PULLUP: LOW when pressed)
      if (stableButtonLevel == LOW) {
        // Switch mode on each press
        mode = (mode == SHOW_TEMP) ? SHOW_HUM : SHOW_TEMP;
      }
    }
  }

  // ---- Sensor update and control ----
  static unsigned long lastSensorMs = 0;
  unsigned long now = millis();

  if (now - lastSensorMs >= SENSOR_INTERVAL_MS) {
    lastSensorMs = now;

    float h = dht.readHumidity();
    float t = dht.readTemperature(); // Celsius
    // DHT11 library returns NAN on error
    if (isfinite(h)) lastHum = h;
    if (isfinite(t)) lastTempC = t;

    // Fan control based on temperature regardless of display mode
    updateFan(lastTempC);

    // Display according to mode
    if (mode == SHOW_TEMP) showTemperature(lastTempC);
    else showHumidity(lastHum);
  }

  // No blocking delays; loop continues for button responsiveness
}