#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_021\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <math.h>

constexpr uint8_t DHT_PIN = 7;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;
constexpr uint8_t LED_PIN = 13;

constexpr unsigned long SENSOR_INTERVAL_MS = 2000;
constexpr unsigned long DISPLAY_INTERVAL_MS = 2000;
constexpr unsigned long LED_BLINK_INTERVAL_MS = 500;

DHT dht(DHT_PIN, DHT11);
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

float temperature = NAN;
float humidity = NAN;

bool sensorError = false;
bool showTemperature = true;
bool ledState = false;

unsigned long lastSensorRead = 0;
unsigned long lastDisplayChange = 0;
unsigned long lastLedToggle = 0;

#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_021\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay();
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_021\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_021\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_021\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay() {
  if (sensorError) {
    display.showNumberDec(0, true, 4, 0);
    return;
  }

  uint8_t segments[4];

  if (showTemperature) {
    int value = (int)roundf(temperature);
    value = constrain(value, -99, 99);

    segments[0] = value < 0 ? SEG_G : 0;
    const int magnitude = abs(value);
    segments[1] = display.encodeDigit(magnitude / 10);
    segments[2] = display.encodeDigit(magnitude % 10);
    segments[3] = SEG_A | SEG_D | SEG_E | SEG_F; // C
  } else {
    int value = (int)roundf(humidity);
    value = constrain(value, 0, 100);

    segments[0] = value >= 100 ? display.encodeDigit(1) : 0;
    segments[1] = display.encodeDigit((value / 10) % 10);
    segments[2] = display.encodeDigit(value % 10);
    segments[3] = SEG_B | SEG_C | SEG_E | SEG_F | SEG_G; // H
  }

  display.setSegments(segments);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);
  Serial.setTimeout(20);

  dht.begin();
  display.setBrightness(7, true);
  display.clear();

  // Allow the DHT11 to stabilize before its first measurement.
  delay(1000);
  lastSensorRead = millis() - SENSOR_INTERVAL_MS;
  lastDisplayChange = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastSensorRead >= SENSOR_INTERVAL_MS) {
    lastSensorRead = now;

    const float newHumidity = dht.readHumidity();
    const float newTemperature = dht.readTemperature();

    if (isnan(newHumidity) || isnan(newTemperature)) {
      if (!sensorError) {
        ledState = true;
        digitalWrite(LED_PIN, HIGH);
        lastLedToggle = now;
      }

      sensorError = true;
      Serial.println(F("sensor error"));
      updateDisplay();
    } else {
      const bool recovering = sensorError;

      temperature = newTemperature;
      humidity = newHumidity;
      sensorError = false;

      ledState = false;
      digitalWrite(LED_PIN, LOW);

      if (recovering) {
        showTemperature = true;
        lastDisplayChange = now;
      }

      updateDisplay();
    }
  }

  if (sensorError) {
    if (now - lastLedToggle >= LED_BLINK_INTERVAL_MS) {
      lastLedToggle = now;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    }
  } else if (!isnan(temperature) && !isnan(humidity)) {
    if (now - lastDisplayChange >= DISPLAY_INTERVAL_MS) {
      lastDisplayChange = now;
      showTemperature = !showTemperature;
      updateDisplay();
    }
  }
}
