#include <Arduino.h>
#include <Servo.h>

constexpr uint8_t POT_PIN = A3;
constexpr uint8_t SERVO1_PIN = 2;
constexpr uint8_t SERVO2_PIN = 3;

// Bargraph anodes A1 through A10, in left-to-right order.
// These are digital pins, not the Mega's analog input pins.
constexpr uint8_t LED_PINS[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

constexpr unsigned long UPDATE_INTERVAL_MS = 20;
constexpr uint8_t ADC_SAMPLES = 16;
constexpr uint8_t LED_HYSTERESIS_ADC = 3;

Servo servo1;
Servo servo2;

uint8_t activeLEDs = 0;
int lastServoAngle = -1;
bool firstControlUpdate = true;
unsigned long lastUpdateMs = 0;

uint16_t readPotentiometer() {
  uint32_t total = 0;
  for (uint8_t i = 0; i < ADC_SAMPLES; ++i) {
    total += analogRead(POT_PIN);
  }
  return (total + ADC_SAMPLES / 2) / ADC_SAMPLES;
}

void setBargraph(uint8_t count) {
  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(LED_PINS[i], i < count ? HIGH : LOW);
  }
}

uint8_t calculateLEDCount(uint16_t reading) {
  // Rounded linear mapping gives exactly five LEDs at either
  // central ADC code (511 or 512).
  if (firstControlUpdate) {
    return (static_cast<uint32_t>(reading) * 10UL + 511UL) / 1023UL;
  }

  uint8_t count = activeLEDs;

  // Small hysteresis around each rounded mapping boundary prevents
  // ADC noise from repeatedly switching a segment on and off.
  const int32_t scaledReading = static_cast<int32_t>(reading) * 20L;
  const int32_t margin = static_cast<int32_t>(LED_HYSTERESIS_ADC) * 20L;

  while (count < 10 &&
         scaledReading >= (2L * count + 1L) * 1023L + margin) {
    ++count;
  }

  while (count > 0 &&
         scaledReading < (2L * count - 1L) * 1023L - margin) {
    --count;
  }

  return count;
}

void setup() {
  pinMode(POT_PIN, INPUT);

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  // Preload zero-degree commands before enabling servo pulses.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(0);
  servo2.write(0);

  // Establish the required startup state before potentiometer control.
  delay(500);
  lastUpdateMs = millis() - UPDATE_INTERVAL_MS;
}

void loop() {
  const unsigned long now = millis();
  if (now - lastUpdateMs < UPDATE_INTERVAL_MS) {
    return;
  }
  lastUpdateMs = now;

  const uint16_t reading = readPotentiometer();
  const int angle =
      (static_cast<uint32_t>(reading) * 180UL + 511UL) / 1023UL;
  const uint8_t newLEDCount = calculateLEDCount(reading);

  if (angle != lastServoAngle) {
    servo1.write(angle);
    servo2.write(180 - angle);
    lastServoAngle = angle;
  }

  if (firstControlUpdate || newLEDCount != activeLEDs) {
    setBargraph(newLEDCount);
    activeLEDs = newLEDCount;
  }

  firstControlUpdate = false;
}