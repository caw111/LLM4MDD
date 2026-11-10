#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_21\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

// Arduino Mega 2560: preserve the supplied signal-pin wiring.
const uint8_t SERVO_PINS[3] = {2, 3, 4};

const uint8_t POSITIONS[6][3] = {
  {  0,   0,   0},
  {  0,  45,  90},
  { 45,  90, 135},
  { 90, 135, 180},
  {135, 180,   0},
  {180,   0,  45}
};

const unsigned long HOLD_MS = 2000;
const unsigned long MOVE_MS = 1200;
const unsigned long FRAME_MS = 20;

Servo servos[3];
uint8_t currentAngles[3] = {0, 0, 0};

#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_21\\direct\\attempt_0\\sketch\\sketch.ino"
void moveTogether(const uint8_t target[3]);
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_21\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_21\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_21\\direct\\attempt_0\\sketch\\sketch.ino"
void moveTogether(const uint8_t target[3]) {
  const unsigned long startedAt = millis();

  // Begin moving immediately, with synchronized, eased motion.
  while (true) {
    const unsigned long elapsed = millis() - startedAt;
    if (elapsed >= MOVE_MS) {
      break;
    }

    const float progress = (float)elapsed / (float)MOVE_MS;
    const float eased = progress * progress * (3.0f - 2.0f * progress);

    for (uint8_t i = 0; i < 3; ++i) {
      const float angle = currentAngles[i] +
                          ((int)target[i] - (int)currentAngles[i]) * eased;
      servos[i].write((int)(angle + 0.5f));
    }

    delay(FRAME_MS);
  }

  // Set exact endpoints before beginning the full two-second hold.
  for (uint8_t i = 0; i < 3; ++i) {
    servos[i].write(target[i]);
    currentAngles[i] = target[i];
  }
}

void setup() {
  // Preload zero before attaching to avoid an initial center-position command.
  for (uint8_t i = 0; i < 3; ++i) {
    servos[i].write(0);
    servos[i].attach(SERVO_PINS[i]);
    servos[i].write(0);
  }

  // Step 1 occurs only at startup/reset.
  delay(HOLD_MS);
}

void loop() {
  // Repeat steps 2 through 6 indefinitely, without repeating step 1.
  for (uint8_t step = 1; step < 6; ++step) {
    moveTogether(POSITIONS[step]);
    delay(HOLD_MS);
  }
}
