#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

constexpr uint8_t PIN_LIGHT = A0;
constexpr uint8_t PIN_BUTTON = 2;   // D2
constexpr uint8_t PIN_DHT = 7;       // D7
constexpr uint8_t PIN_TM1637_CLK = 4; // D4
constexpr uint8_t PIN_TM1637_DIO = 5; // D5

// NOTE: Requirements did not specify thresholds; to preserve "behavior" and enable
// anomaly output, we define reasonable defaults. If your original thresholds differ,
// adjust these constants accordingly.
static const int LIGHT_LOW_THRESHOLD = 300;  // analogRead (0-1023), "light too low"
static const float TEMP_HIGH_THRESHOLD = 30.0f; // Celsius, "temperature too high"

// Display modes
enum DisplayMode : uint8_t { MODE_LIGHT = 0, MODE_TEMP = 1, MODE_HUM = 2 };
volatile DisplayMode mode = MODE_LIGHT;

// DHT
DHT dht(PIN_DHT, DHT11);

// TM1637
TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// Button debounce
static const unsigned long DEBOUNCE_MS = 50;
unsigned long lastButtonEventMs = 0;

#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readHumidity();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readLightRaw();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void printDataLine(int lightRaw, float tempC, float humPct, bool anomaly);
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showOnTM1637(DisplayMode m, int lightRaw, float tempC, float humPct);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 112 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline float readHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) return NAN;
  return h;
}

static inline float readTemperatureC() {
  float t = dht.readTemperature(); // Celsius
  if (isnan(t)) return NAN;
  return t;
}

static inline int readLightRaw() {
  return analogRead(PIN_LIGHT);
}

static inline void printDataLine(int lightRaw, float tempC, float humPct, bool anomaly) {
  Serial.print("Light=");
  Serial.print(lightRaw);
  Serial.print(", Temp=");
  if (isnan(tempC)) Serial.print("NaN");
  else Serial.print(tempC, 1);
  Serial.print("C, Hum=");
  if (isnan(humPct)) Serial.print("NaN");
  else Serial.print(humPct, 1);
  if (anomaly) {
    Serial.print(" | ENV_ANOMALY: light too low & temp too high");
  }
  Serial.println();
}

static inline void showOnTM1637(DisplayMode m, int lightRaw, float tempC, float humPct) {
  // TM1637Display supports integer display; scale/round values for required mode display.
  // Light: show raw 0-1023 truncated (fits 4 digits)
  // Temp: show tenths or integer? We'll show integer Celsius for clarity.
  // Hum: show integer % for clarity.
  tm1637.clearDisplay();

  switch (m) {
    case MODE_LIGHT: {
      // Show 0-1023; use full number within 4 digits
      tm1637.showNumberDec(lightRaw, false, 4, 0);
      break;
    }
    case MODE_TEMP: {
      if (!isnan(tempC)) {
        int t = (int)round(tempC);
        tm1637.showNumberDec(t, false, 4, 0);
      } else {
        tm1637.showNumberDec(0, false, 4, 0);
      }
      break;
    }
    case MODE_HUM: {
      if (!isnan(humPct)) {
        int h = (int)round(humPct);
        tm1637.showNumberDec(h, false, 4, 0);
      } else {
        tm1637.showNumberDec(0, false, 4, 0);
      }
      break;
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_BUTTON, INPUT_PULLUP); // Button to GND when pressed

  dht.begin();

  tm1637.setBrightness(0x0f, true); // max brightness, auto

  // Ensure initial display
  int lightRaw = readLightRaw();
  float tempC = readTemperatureC();
  float humPct = readHumidity();
  showOnTM1637(MODE_LIGHT, lightRaw, tempC, humPct);
}

void loop() {
  // Read current sensor values every loop for responsive display and anomaly checks.
  int lightRaw = readLightRaw();
  float tempC = readTemperatureC();
  float humPct = readHumidity();

  // Button press handling: each press switches display mode (light->temp->hum->light)
  // Detect falling edge (pressed: LOW with INPUT_PULLUP).
  int buttonState = digitalRead(PIN_BUTTON);
  static int lastButtonState = HIGH;

  unsigned long nowMs = millis();
  if (lastButtonState == HIGH && buttonState == LOW) {
    if (nowMs - lastButtonEventMs >= DEBOUNCE_MS) {
      lastButtonEventMs = nowMs;
      // Switch mode
      noInterrupts();
      mode = static_cast<DisplayMode>((static_cast<uint8_t>(mode) + 1) % 3);
      interrupts();

      // Update display immediately after mode change
      showOnTM1637(mode, lightRaw, tempC, humPct);
    }
  }
  lastButtonState = buttonState;

  // Update TM1637 continuously (keeps values fresh if mode not changed)
  showOnTM1637(mode, lightRaw, tempC, humPct);

  // Serial every 2 seconds: output three complete items, and anomaly warning when conditions met.
  static unsigned long lastSerialMs = 0;
  if (nowMs - lastSerialMs >= 2000UL) {
    lastSerialMs = nowMs;

    bool anomaly = (lightRaw < LIGHT_LOW_THRESHOLD) && (!isnan(tempC)) && (tempC > TEMP_HIGH_THRESHOLD);

    printDataLine(lightRaw, tempC, humPct, anomaly);
  }
}
