#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_25\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// Arduino Mega 2560 wiring.
constexpr uint8_t POT1_PIN = A3;
constexpr uint8_t POT2_PIN = A4;
constexpr uint8_t SERVO1_PIN = 2;
constexpr uint8_t SERVO2_PIN = 3;
constexpr uint8_t SERVO3_PIN = 4;

constexpr unsigned long UPDATE_INTERVAL_MS = 10;

Servo servo1;
Servo servo2;
Servo servo3;

unsigned long lastUpdateMs = 0;

#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_25\\direct\\attempt_0\\sketch\\sketch.ino"
int readTargetAngle(uint8_t pin);
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_25\\direct\\attempt_0\\sketch\\sketch.ino"
void updateServos();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_25\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_25\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_25\\direct\\attempt_0\\sketch\\sketch.ino"
int readTargetAngle(uint8_t pin) {
  const int value = analogRead(pin);
  // Linear 0..1023 -> 0..180 mapping, rounded to the nearest degree.
  return (static_cast<long>(value) * 180L + 511L) / 1023L;
}

void updateServos() {
  const int angle1 = readTargetAngle(POT1_PIN);
  const int angle23 = readTargetAngle(POT2_PIN);

  // Frequent direct updates preserve the input mapping without introducing
  // a slew-rate limit or filtering delay. Servo generates pulses continuously.
  servo1.write(angle1);
  servo2.write(angle23);
  servo3.write(angle23);
}

void setup() {
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  // Set initial commands before enabling servo pulse generation.
  updateServos();

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  lastUpdateMs = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastUpdateMs >= UPDATE_INTERVAL_MS) {
    lastUpdateMs = now;
    updateServos();
  }
}
