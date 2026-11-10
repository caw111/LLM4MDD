#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_48\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// Wiring for Arduino Mega 2560; RGB LEDs are common-cathode.
const uint8_t RED_PINS[]   = {2, 5, 8};
const uint8_t GREEN_PINS[] = {3, 6, 9};
const uint8_t BLUE_PINS[]  = {4, 7, 10};

const uint8_t POT1_PIN = A3;
const uint8_t POT2_PIN = A4;

const uint8_t SERVO1_PIN = 11;
const uint8_t SERVO2_PIN = 12;
const uint8_t SERVO3_PIN = 13;

const unsigned long UPDATE_INTERVAL_MS = 100UL;
const unsigned long SERVO3_INTERVAL_MS = 2000UL;

// On the Mega, the first three Servo instances use Timer5,
// leaving hardware PWM on LED pins 2 through 10 available.
Servo servo1;
Servo servo2;
Servo servo3;

unsigned long lastUpdateMs = 0;
unsigned long lastServo3ToggleMs = 0;
bool servo3At180 = false;

#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_48\\direct\\attempt_0\\sketch\\sketch.ino"
void updateOutputs();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_48\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_48\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_48\\direct\\attempt_0\\sketch\\sketch.ino"
void updateOutputs() {
  const int pot1Value = analogRead(POT1_PIN);
  const int pot2Value = analogRead(POT2_PIN);

  const uint8_t red = (uint8_t)map(pot1Value, 0, 1023, 0, 255);
  const uint8_t green = (uint8_t)map(pot2Value, 0, 1023, 0, 255);

  for (uint8_t i = 0; i < 3; ++i) {
    analogWrite(RED_PINS[i], red);
    analogWrite(GREEN_PINS[i], green);
    digitalWrite(BLUE_PINS[i], LOW);
  }

  servo1.write((int)map(pot1Value, 0, 1023, 0, 180));
  servo2.write((int)map(pot2Value, 0, 1023, 0, 180));
  servo3.write(servo3At180 ? 180 : 0);
}

void setup() {
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  for (uint8_t i = 0; i < 3; ++i) {
    pinMode(RED_PINS[i], OUTPUT);
    pinMode(GREEN_PINS[i], OUTPUT);
    pinMode(BLUE_PINS[i], OUTPUT);

    digitalWrite(RED_PINS[i], LOW);
    digitalWrite(GREEN_PINS[i], LOW);
    digitalWrite(BLUE_PINS[i], LOW);
  }

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  updateOutputs();

  lastUpdateMs = millis();
  lastServo3ToggleMs = lastUpdateMs;
}

void loop() {
  const unsigned long now = millis();

  // Independent, nonblocking two-second servo3 schedule.
  const unsigned long elapsed = now - lastServo3ToggleMs;
  if (elapsed >= SERVO3_INTERVAL_MS) {
    const unsigned long intervals = elapsed / SERVO3_INTERVAL_MS;
    lastServo3ToggleMs += intervals * SERVO3_INTERVAL_MS;

    if (intervals & 1UL) {
      servo3At180 = !servo3At180;
    }
    servo3.write(servo3At180 ? 180 : 0);
  }

  if (now - lastUpdateMs >= UPDATE_INTERVAL_MS) {
    lastUpdateMs +=
        ((now - lastUpdateMs) / UPDATE_INTERVAL_MS) * UPDATE_INTERVAL_MS;
    updateOutputs();
  }
}
