#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision39\\cases\\medium_019\\arduino\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <math.h>

// Wiring:
// D2: button to GND (internal pull-up enabled)
// D7: DHT11 data
// D8: HC-SR04 TRIG
// D9: HC-SR04 ECHO
// Display: Serial Monitor, 9600 baud.

const uint8_t BUTTON_PIN = 2;
const uint8_t DHT_PIN = 7;
const uint8_t TRIGGER_PIN = 8;
const uint8_t ECHO_PIN = 9;

const unsigned long DEBOUNCE_MS = 35;
const unsigned long DHT_INTERVAL_MS = 2000;
const unsigned long DISTANCE_INTERVAL_MS = 250;
const unsigned long ECHO_TIMEOUT_US = 30000UL;

DHT dht(DHT_PIN, DHT11);

enum DisplayMode : uint8_t {
  TEMPERATURE_MODE,
  DISTANCE_MODE
};

DisplayMode displayMode = TEMPERATURE_MODE;

uint8_t lastButtonReading = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long buttonChangedAt = 0;
unsigned long lastTemperatureReadAt = 0;
unsigned long lastDistanceReadAt = 0;

float temperatureC = NAN;
float distanceCm = NAN;
bool temperatureAttempted = false;
bool refreshDisplay = true;
bool distanceReadRequested = false;

#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision39\\cases\\medium_019\\arduino\\sketch\\sketch.ino"
float measureDistanceCm();
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision39\\cases\\medium_019\\arduino\\sketch\\sketch.ino"
void updateButton(unsigned long now);
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision39\\cases\\medium_019\\arduino\\sketch\\sketch.ino"
void printDisplay();
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision39\\cases\\medium_019\\arduino\\sketch\\sketch.ino"
void setup();
#line 121 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision39\\cases\\medium_019\\arduino\\sketch\\sketch.ino"
void loop();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision39\\cases\\medium_019\\arduino\\sketch\\sketch.ino"
float measureDistanceCm() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  const unsigned long duration =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (duration == 0) {
    return NAN;
  }

  return duration * 0.0343f / 2.0f;
}

void updateButton(unsigned long now) {
  const uint8_t reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastButtonReading = reading;
    buttonChangedAt = now;
  }

  if ((unsigned long)(now - buttonChangedAt) >= DEBOUNCE_MS &&
      reading != stableButtonState) {
    stableButtonState = reading;

    // Toggle once per debounced press, not while held.
    if (stableButtonState == LOW) {
      displayMode = (displayMode == TEMPERATURE_MODE)
                        ? DISTANCE_MODE
                        : TEMPERATURE_MODE;

      if (displayMode == DISTANCE_MODE) {
        distanceReadRequested = true;
      }

      refreshDisplay = true;
    }
  }
}

void printDisplay() {
  if (displayMode == TEMPERATURE_MODE) {
    Serial.print(F("Temperature: "));
    if (!temperatureAttempted) {
      Serial.println(F("waiting for DHT11"));
    } else if (isnan(temperatureC)) {
      Serial.println(F("DHT11 read error"));
    } else {
      Serial.print(temperatureC, 1);
      Serial.println(F(" C"));
    }
  } else {
    Serial.print(F("Distance: "));
    if (isnan(distanceCm)) {
      Serial.println(F("no echo / out of range"));
    } else {
      Serial.print(distanceCm, 1);
      Serial.println(F(" cm"));
    }
  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIGGER_PIN, LOW);

  Serial.begin(9600);
  dht.begin();

  lastTemperatureReadAt = millis();
}

void loop() {
  const unsigned long now = millis();

  updateButton(now);

  // Keep temperature cached, respecting the DHT11 sampling interval.
  if ((unsigned long)(now - lastTemperatureReadAt) >= DHT_INTERVAL_MS) {
    lastTemperatureReadAt = now;
    temperatureC = dht.readTemperature();
    temperatureAttempted = true;

    if (displayMode == TEMPERATURE_MODE) {
      refreshDisplay = true;
    }
  }

  if (displayMode == DISTANCE_MODE &&
      (distanceReadRequested ||
       (unsigned long)(now - lastDistanceReadAt) >= DISTANCE_INTERVAL_MS)) {
    lastDistanceReadAt = now;
    distanceReadRequested = false;
    distanceCm = measureDistanceCm();
    refreshDisplay = true;
  }

  if (refreshDisplay) {
    refreshDisplay = false;
    printDisplay();
  }
}
