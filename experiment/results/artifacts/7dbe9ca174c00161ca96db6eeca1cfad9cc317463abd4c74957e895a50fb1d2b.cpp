#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_010\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <math.h>

constexpr uint8_t DHT_PIN = 7;
constexpr uint8_t FAN_PIN = 5;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 3;
constexpr uint8_t BUTTON_PIN = 2;

constexpr unsigned long SENSOR_INTERVAL_MS = 2000;
constexpr unsigned long BUTTON_DEBOUNCE_MS = 35;

constexpr float FAN_ON_TEMPERATURE_C = 30.0f;
constexpr float FAN_FULL_TEMPERATURE_C = 40.0f;
constexpr uint8_t FAN_MIN_PWM = 85;
constexpr uint8_t FAN_MAX_PWM = 255;

DHT dht(DHT_PIN, DHT11);
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

bool showHumidity = false;
float temperatureC = NAN;
float humidityPercent = NAN;

uint8_t lastRawButtonState = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long buttonChangedAt = 0;
unsigned long lastSensorReadAt = 0;

constexpr uint8_t SEGMENT_MINUS = 0x40;
constexpr uint8_t SEGMENT_DEGREE = 0x63;
constexpr uint8_t SEGMENT_C = 0x39;
constexpr uint8_t SEGMENT_H = 0x76;

#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_010\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay();
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_010\\direct\\attempt_0\\sketch\\sketch.ino"
void updateFan(float temperature);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_010\\direct\\attempt_0\\sketch\\sketch.ino"
void handleButton(unsigned long now);
#line 121 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_010\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 138 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_010\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_010\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay() {
  uint8_t segments[4] = {0, 0, 0, 0};

  if (showHumidity) {
    segments[3] = SEGMENT_H;

    if (isnan(humidityPercent)) {
      segments[0] = SEGMENT_MINUS;
      segments[1] = SEGMENT_MINUS;
      segments[2] = SEGMENT_MINUS;
    } else {
      int value = constrain((int)roundf(humidityPercent), 0, 100);

      if (value >= 100) {
        segments[0] = display.encodeDigit(value / 100);
      }
      if (value >= 10) {
        segments[1] = display.encodeDigit((value / 10) % 10);
      }
      segments[2] = display.encodeDigit(value % 10);
    }
  } else {
    segments[2] = SEGMENT_DEGREE;
    segments[3] = SEGMENT_C;

    if (isnan(temperatureC)) {
      segments[0] = SEGMENT_MINUS;
      segments[1] = SEGMENT_MINUS;
    } else {
      // DHT11's temperature range fits in the two numeric positions.
      int value = constrain((int)roundf(temperatureC), 0, 99);

      if (value >= 10) {
        segments[0] = display.encodeDigit(value / 10);
      }
      segments[1] = display.encodeDigit(value % 10);
    }
  }

  display.setSegments(segments);
}

void updateFan(float temperature) {
  if (isnan(temperature)) {
    // A failed reading must not stop a fan that was already running.
    return;
  }

  if (temperature <= FAN_ON_TEMPERATURE_C) {
    analogWrite(FAN_PIN, 0);
    return;
  }

  // Above 30 C, start at usable PWM and increase to full speed at 40 C.
  float fraction =
      (temperature - FAN_ON_TEMPERATURE_C) /
      (FAN_FULL_TEMPERATURE_C - FAN_ON_TEMPERATURE_C);
  fraction = constrain(fraction, 0.0f, 1.0f);

  uint8_t pwm = (uint8_t)roundf(
      FAN_MIN_PWM + fraction * (FAN_MAX_PWM - FAN_MIN_PWM));
  analogWrite(FAN_PIN, pwm);
}

void handleButton(unsigned long now) {
  // Connect the button between D2 and GND; the internal pull-up is enabled.
  uint8_t rawState = digitalRead(BUTTON_PIN);

  if (rawState != lastRawButtonState) {
    lastRawButtonState = rawState;
    buttonChangedAt = now;
  }

  if (now - buttonChangedAt >= BUTTON_DEBOUNCE_MS &&
      rawState != stableButtonState) {
    stableButtonState = rawState;

    if (stableButtonState == LOW) {
      showHumidity = !showHumidity;
      updateDisplay();
    }
  }
}

void setup() {
  // Drive the fan through a suitable transistor/MOSFET driver with flyback
  // protection and a common ground; do not power a motor directly from D5.
  pinMode(FAN_PIN, OUTPUT);
  analogWrite(FAN_PIN, 0);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  dht.begin();
  display.setBrightness(5, true);
  updateDisplay();

  // Allow the DHT11 to stabilize before its first read.
  lastSensorReadAt = millis();
  buttonChangedAt = millis();
}

void loop() {
  unsigned long now = millis();
  handleButton(now);

  if (now - lastSensorReadAt >= SENSOR_INTERVAL_MS) {
    lastSensorReadAt = now;

    humidityPercent = dht.readHumidity();
    temperatureC = dht.readTemperature();

    updateFan(temperatureC);
    updateDisplay();
  }
}
