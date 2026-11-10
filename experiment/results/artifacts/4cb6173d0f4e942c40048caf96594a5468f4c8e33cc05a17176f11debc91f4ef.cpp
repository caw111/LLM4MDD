#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_82\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

constexpr uint8_t SERVO1_PIN = 8;
constexpr uint8_t SERVO2_PIN = 9;
constexpr uint8_t RED_PIN = 5;
constexpr uint8_t GREEN_PIN = 6;
constexpr uint8_t BLUE_PIN = 7;
constexpr uint8_t BUTTON_PINS[] = {2, 3, 4};

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long MIN_STATE_MS = 2000;

Servo servo1;
Servo servo2;

struct Button {
  bool lastReading;
  bool stableState;
  unsigned long lastChange;
};

Button buttons[3];

constexpr uint8_t QUEUE_SIZE = 64;
uint8_t commandQueue[QUEUE_SIZE];
uint8_t queueHead = 0;
uint8_t queueTail = 0;
uint8_t queueCount = 0;

unsigned long stateStartedAt = 0;

#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_82\\direct\\attempt_0\\sketch\\sketch.ino"
void setColor(bool red, bool green, bool blue);
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_82\\direct\\attempt_0\\sketch\\sketch.ino"
void enqueueCommand(uint8_t command);
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_82\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButtons(unsigned long now);
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_82\\direct\\attempt_0\\sketch\\sketch.ino"
void executeCommand(uint8_t command);
#line 90 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_82\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_82\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_82\\direct\\attempt_0\\sketch\\sketch.ino"
void setColor(bool red, bool green, bool blue) {
  // Common-cathode RGB LED: HIGH turns each channel on.
  digitalWrite(RED_PIN, red ? HIGH : LOW);
  digitalWrite(GREEN_PIN, green ? HIGH : LOW);
  digitalWrite(BLUE_PIN, blue ? HIGH : LOW);
}

void enqueueCommand(uint8_t command) {
  if (queueCount < QUEUE_SIZE) {
    commandQueue[queueTail] = command;
    queueTail = (queueTail + 1) % QUEUE_SIZE;
    ++queueCount;
  }
}

void updateButtons(unsigned long now) {
  for (uint8_t i = 0; i < 3; ++i) {
    bool reading = digitalRead(BUTTON_PINS[i]);

    if (reading != buttons[i].lastReading) {
      buttons[i].lastReading = reading;
      buttons[i].lastChange = now;
    }

    if (reading != buttons[i].stableState &&
        now - buttons[i].lastChange >= DEBOUNCE_MS) {
      buttons[i].stableState = reading;

      // Queue one command per debounced press, not repeatedly while held.
      if (reading == LOW) {
        enqueueCommand(i + 1);
      }
    }
  }
}

void executeCommand(uint8_t command) {
  switch (command) {
    case 1:
      servo1.write(90);
      // Servo2 retains its previous position.
      setColor(true, false, false);
      break;

    case 2:
      servo2.write(90);
      // Servo1 retains its previous position.
      setColor(false, true, false);
      break;

    case 3:
      servo1.write(0);
      servo2.write(0);
      setColor(false, false, true);
      break;
  }
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  setColor(false, false, false);

  for (uint8_t i = 0; i < 3; ++i) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
    buttons[i].lastReading = HIGH;
    buttons[i].stableState = HIGH;
    buttons[i].lastChange = millis();
  }

  // Preload zero-degree targets before enabling servo pulses.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(0);
  servo2.write(0);

  // Preserve the initial zero-degree, LED-off state for at least two seconds.
  stateStartedAt = millis();
}

void loop() {
  unsigned long now = millis();
  updateButtons(now);

  // Continue detecting presses during the minimum state-hold interval.
  if (queueCount > 0 && now - stateStartedAt >= MIN_STATE_MS) {
    uint8_t command = commandQueue[queueHead];
    queueHead = (queueHead + 1) % QUEUE_SIZE;
    --queueCount;

    executeCommand(command);
    stateStartedAt = millis();
  }
}
