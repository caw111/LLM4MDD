#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_030\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <math.h>

constexpr uint8_t LIGHT_PIN = A0;
constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t DHT_PIN = 7;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;

constexpr unsigned long SERIAL_BAUD = 9600;
constexpr unsigned long REPORT_INTERVAL_MS = 2000;
constexpr unsigned long LIGHT_SAMPLE_INTERVAL_MS = 100;
constexpr unsigned long DEBOUNCE_MS = 35;

// Adjustable alarm thresholds.
// Assumes the A0 reading increases with illumination.
// If the photoresistor divider has the opposite polarity, set this to false.
constexpr bool LIGHT_READING_INCREASES_WITH_BRIGHTNESS = true;
constexpr int LOW_LIGHT_THRESHOLD = 200;
constexpr float HIGH_TEMPERATURE_THRESHOLD_C = 30.0f;

DHT dht(DHT_PIN, DHT11);
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

enum DisplayMode : uint8_t {
  SHOW_LIGHT,
  SHOW_TEMPERATURE,
  SHOW_HUMIDITY,
  DISPLAY_MODE_COUNT
};

DisplayMode displayMode = SHOW_LIGHT;

int lightValue = 0;
float temperatureC = NAN;
float humidityPercent = NAN;

bool lastRawButtonState = HIGH;
bool stableButtonState = HIGH;
bool displayDirty = true;

unsigned long buttonChangedAt = 0;
unsigned long lastLightSampleAt = 0;
unsigned long lastReportAt = 0;

#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_030\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButton(unsigned long now);
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_030\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay();
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_030\\direct\\attempt_0\\sketch\\sketch.ino"
void readDhtAndReport();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_030\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 173 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_030\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_030\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButton(unsigned long now) {
  const bool rawState = digitalRead(BUTTON_PIN);

  if (rawState != lastRawButtonState) {
    lastRawButtonState = rawState;
    buttonChangedAt = now;
  }

  if (rawState != stableButtonState &&
      now - buttonChangedAt >= DEBOUNCE_MS) {
    stableButtonState = rawState;

    // INPUT_PULLUP: connect the button between D2 and GND.
    // Advance exactly once on each debounced press, not while held.
    if (stableButtonState == LOW) {
      displayMode = static_cast<DisplayMode>(
          (static_cast<uint8_t>(displayMode) + 1) %
          static_cast<uint8_t>(DISPLAY_MODE_COUNT));
      displayDirty = true;
    }
  }
}

void updateDisplay() {
  if (!displayDirty) {
    return;
  }
  displayDirty = false;

  const uint8_t sensorError[] = {
    SEG_G, SEG_G, SEG_G, SEG_G
  };

  switch (displayMode) {
    case SHOW_LIGHT:
      display.showNumberDec(lightValue, false, 4, 0);
      break;

    case SHOW_TEMPERATURE:
      if (isnan(temperatureC)) {
        display.setSegments(sensorError);
      } else {
        display.showNumberDec(
            static_cast<int>(roundf(temperatureC)), false, 4, 0);
      }
      break;

    case SHOW_HUMIDITY:
      if (isnan(humidityPercent)) {
        display.setSegments(sensorError);
      } else {
        display.showNumberDec(
            static_cast<int>(roundf(humidityPercent)), false, 4, 0);
      }
      break;

    default:
      break;
  }
}

void readDhtAndReport() {
  // DHT11 is sampled only once per two-second reporting interval.
  humidityPercent = dht.readHumidity();
  temperatureC = dht.readTemperature();
  lightValue = analogRead(LIGHT_PIN);
  displayDirty = true;

  Serial.print(F("Light="));
  Serial.print(lightValue);
  Serial.print(F("/1023, Temperature="));

  if (isnan(temperatureC)) {
    Serial.print(F("ERROR"));
  } else {
    Serial.print(temperatureC, 1);
    Serial.print(F(" C"));
  }

  Serial.print(F(", Humidity="));
  if (isnan(humidityPercent)) {
    Serial.print(F("ERROR"));
  } else {
    Serial.print(humidityPercent, 1);
    Serial.print(F(" %"));
  }
  Serial.println();

  const bool lowLight = LIGHT_READING_INCREASES_WITH_BRIGHTNESS
                            ? lightValue < LOW_LIGHT_THRESHOLD
                            : lightValue > LOW_LIGHT_THRESHOLD;

  if (lowLight && !isnan(temperatureC) &&
      temperatureC > HIGH_TEMPERATURE_THRESHOLD_C) {
    Serial.println(
        F("WARNING: Environment abnormal: low light AND high temperature."));
  }

  if (isnan(temperatureC) || isnan(humidityPercent)) {
    Serial.println(F("ERROR: DHT11 read failed; check sensor and wiring."));
  }
}

void setup() {
  pinMode(LIGHT_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(SERIAL_BAUD);
  dht.begin();

  display.setBrightness(4, true);
  display.clear();

  lightValue = analogRead(LIGHT_PIN);

  const unsigned long now = millis();
  lastRawButtonState = digitalRead(BUTTON_PIN);
  stableButtonState = lastRawButtonState;
  buttonChangedAt = now;
  lastLightSampleAt = now;
  lastReportAt = now;

  updateDisplay();
}

void loop() {
  const unsigned long now = millis();

  updateButton(now);

  if (now - lastLightSampleAt >= LIGHT_SAMPLE_INTERVAL_MS) {
    lastLightSampleAt = now;
    lightValue = analogRead(LIGHT_PIN);
    if (displayMode == SHOW_LIGHT) {
      displayDirty = true;
    }
  }

  if (now - lastReportAt >= REPORT_INTERVAL_MS) {
    lastReportAt = now;
    readDhtAndReport();
  }

  updateDisplay();
}
