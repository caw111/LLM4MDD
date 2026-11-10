#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_31\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

const uint8_t POT1_PIN = A3;
const uint8_t POT2_PIN = A4;

const uint8_t SERVO1_PIN = 2;
const uint8_t SERVO2_PIN = 3;
const uint8_t SERVO3_PIN = 4;

// Bar graph anodes A1 through A10, in physical segment order.
// All cathodes are connected to GND, so HIGH lights a segment.
const uint8_t LED_PINS[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};

Servo servo1;
Servo servo2;
Servo servo3;

// Scale a 10-bit ADC value, rounding to the nearest integer.
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_31\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t scaleInput(uint16_t value, uint16_t maximum);
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_31\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_31\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_31\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t scaleInput(uint16_t value, uint16_t maximum) {
  return (static_cast<uint32_t>(value) * maximum + 511UL) / 1023UL;
}

void setup() {
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  // Set initial positions before enabling servo pulses.
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
}

void loop() {
  const uint16_t pot1Value = analogRead(POT1_PIN);
  const uint16_t pot2Value = analogRead(POT2_PIN);

  // Round the average potentiometer reading to the nearest integer.
  const uint16_t averageValue = (pot1Value + pot2Value + 1U) / 2U;

  servo1.write(scaleInput(pot1Value, 180));
  servo2.write(scaleInput(pot2Value, 180));
  servo3.write(scaleInput(averageValue, 180));

  const uint8_t litSegments = scaleInput(averageValue, 10);

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(LED_PINS[i], i < litSegments ? HIGH : LOW);
  }

  delay(20);
}
