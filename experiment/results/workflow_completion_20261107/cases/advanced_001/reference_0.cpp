#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <math.h>

const uint8_t FAN = 5;
const uint8_t LIGHT = 9;
const uint8_t PUMP = 10;
const uint8_t BUZZER = 6;
DHT dht(7, DHT11);
TM1637Display display(4, 8);

struct Button {
  uint8_t pin;
  uint8_t sampled;
  uint8_t stable;
  unsigned long changed;
};
Button modeButton = {2, HIGH, HIGH, 0};
Button selectButton = {3, HIGH, HIGH, 0};

bool automaticMode = true;
uint8_t selected = 0;
int fanValue = 0;
bool lightValue = false;
bool pumpValue = false;
float temperatureValue = NAN;
bool dhtValid = false;
unsigned long lastDht = 0;
unsigned long displayEpoch = 0;
uint8_t previousFault = 0;

bool pressed(Button &b, unsigned long now) {
  uint8_t reading = digitalRead(b.pin);
  if (reading != b.sampled) {
    b.sampled = reading;
    b.changed = now;
  }
  if (b.stable != b.sampled && now - b.changed >= 30) {
    b.stable = b.sampled;
    return b.stable == LOW;
  }
  return false;
}

bool validAnalog(int value) {
  return value > 0 && value < 1023;
}

int automaticFan(float value) {
  if (value <= 25.0f) return 0;
  if (value >= 35.0f) return 255;
  return (int)((value - 25.0f) * 255.0f / 10.0f);
}

void writeActuators() {
  analogWrite(FAN, fanValue);
  digitalWrite(LIGHT, lightValue ? HIGH : LOW);
  digitalWrite(PUMP, pumpValue ? HIGH : LOW);
}

void setup() {
  pinMode(FAN, OUTPUT);
  pinMode(LIGHT, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  writeActuators();
  digitalWrite(BUZZER, LOW);
  Serial.begin(9600);
  dht.begin();
  display.setBrightness(7);
  display.clear();
  displayEpoch = millis();
  lastDht = millis() - 2000UL;
}

void loop() {
  unsigned long now = millis();
  bool modePress = pressed(modeButton, now);
  bool selectPress = pressed(selectButton, now);
  if (modePress) {
    automaticMode = !automaticMode;
    if (!automaticMode) selected = 0;
  }
  if (selectPress && !automaticMode) selected = (selected + 1) % 3;

  if (now - lastDht >= 2000UL) {
    lastDht = now;
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    dhtValid = !isnan(t) && !isnan(h);
    if (dhtValid) temperatureValue = t;
  }

  int lightReading = analogRead(A0);
  int soilReading = analogRead(A1);
  int knob = analogRead(A2);
  bool lightValid = validAnalog(lightReading);
  bool soilValid = validAnalog(soilReading);

  if (automaticMode) {
    if (dhtValid) fanValue = automaticFan(temperatureValue);
    if (lightValid) lightValue = lightReading < 400;
    if (soilValid) pumpValue = soilReading < 400;
  } else {
    if (selected == 0) fanValue = (long)knob * 255L / 1023L;
    else if (selected == 1) lightValue = knob >= 512;
    else pumpValue = knob >= 512;
  }
  writeActuators();

  uint8_t fault = 0;
  if (!dhtValid) fault |= 1;
  if (!lightValid) fault |= 2;
  if (!soilValid) fault |= 4;
  if (fault != previousFault) {
    if (fault != 0) {
      tone(BUZZER, 1000);
      Serial.print("Sensor fault: ");
      if (fault & 1) Serial.print("DHT ");
      if (fault & 2) Serial.print("light ");
      if (fault & 4) Serial.print("soil ");
      Serial.println();
    } else {
      noTone(BUZZER);
      digitalWrite(BUZZER, LOW);
    }
    previousFault = fault;
  }

  uint8_t phase = ((now - displayEpoch) / 2000UL) % 3;
  int shown;
  if (phase == 0) shown = dhtValid ? (int)round(temperatureValue) : 0;
  else if (phase == 1) shown = lightReading;
  else shown = soilReading;
  display.showNumberDec(shown, false);
  delay(5);
}
