#include <Servo.h>

const uint8_t LED1_PIN = 2;
const uint8_t LED2_PIN = 3;
const uint8_t SERVO1_PIN = 4;
const uint8_t SERVO2_PIN = 5;
const uint8_t SERVO3_PIN = 6;

const unsigned long STATE_DURATION_MS = 2000UL;

Servo servo1;
Servo servo2;
Servo servo3;

unsigned long lastStateChange;
bool firstSequenceState = false;

void setup() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Preload zero-degree targets before enabling servo pulses.
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Hold the initial all-zero, LEDs-off state for two seconds.
  lastStateChange = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastStateChange >= STATE_DURATION_MS) {
    lastStateChange = now;
    firstSequenceState = !firstSequenceState;

    if (firstSequenceState) {
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, LOW);

      servo1.write(0);
      servo2.write(90);
      servo3.write(180);
    } else {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, HIGH);

      servo1.write(180);
      servo2.write(0);
      servo3.write(90);
    }
  }
}