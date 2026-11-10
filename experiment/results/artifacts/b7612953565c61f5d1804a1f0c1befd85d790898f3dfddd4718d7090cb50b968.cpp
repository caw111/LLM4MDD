#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_73\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

constexpr uint8_t SERVO_PIN = 45;
constexpr uint8_t BUTTON_COUNT = 3;
constexpr uint8_t LED_COUNT = 10;

const uint8_t buttonPins[BUTTON_COUNT] = {2, 3, 4};
const uint8_t ledPins[LED_COUNT] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};
const uint8_t servoAngles[BUTTON_COUNT] = {30, 60, 90};
const uint8_t litLedCounts[BUTTON_COUNT] = {3, 6, 10};

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long MIN_STATE_MS = 2000;

// Queue button presses so a press during a state's minimum hold time
// can be serviced afterward without interrupting the current state.
constexpr uint8_t QUEUE_CAPACITY = 64;
uint8_t commandQueue[QUEUE_CAPACITY];
uint8_t queueHead = 0;
uint8_t queueTail = 0;
uint8_t queueCount = 0;

struct ButtonState {
  uint8_t lastReading;
  uint8_t stableReading;
  unsigned long lastChangeMs;
  bool waitingToQueue;
};

ButtonState buttons[BUTTON_COUNT];
Servo servo1;
unsigned long stateStartedMs = 0;

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_73\\direct\\attempt_0\\sketch\\sketch.ino"
bool enqueueCommand(uint8_t buttonIndex);
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_73\\direct\\attempt_0\\sketch\\sketch.ino"
void displayState(uint8_t angle, uint8_t illuminatedLeds);
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_73\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_73\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_73\\direct\\attempt_0\\sketch\\sketch.ino"
bool enqueueCommand(uint8_t buttonIndex) {
  if (queueCount >= QUEUE_CAPACITY) {
    return false;
  }

  commandQueue[queueTail] = buttonIndex;
  queueTail = (queueTail + 1) % QUEUE_CAPACITY;
  ++queueCount;
  return true;
}

void displayState(uint8_t angle, uint8_t illuminatedLeds) {
  servo1.write(angle);

  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(ledPins[i], i < illuminatedLeds ? HIGH : LOW);
  }

  stateStartedMs = millis();
}

void setup() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(ledPins[i], LOW);
    pinMode(ledPins[i], OUTPUT);
  }

  const unsigned long now = millis();

  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    buttons[i].lastReading = HIGH;
    buttons[i].stableReading = HIGH;
    buttons[i].lastChangeMs = now;
    buttons[i].waitingToQueue = false;
  }

  // Preload zero degrees before attaching to avoid an initial 90-degree pulse.
  servo1.write(0);
  servo1.attach(SERVO_PIN);
  displayState(0, 0);
}

void loop() {
  const unsigned long now = millis();

  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    ButtonState &button = buttons[i];
    const uint8_t reading = digitalRead(buttonPins[i]);

    if (reading != button.lastReading) {
      button.lastReading = reading;
      button.lastChangeMs = now;
    }

    if (now - button.lastChangeMs >= DEBOUNCE_MS &&
        reading != button.stableReading) {
      button.stableReading = reading;

      // Active-low press edge: holding a button does not repeat commands.
      if (reading == LOW) {
        button.waitingToQueue = true;
      }
    }

    if (button.waitingToQueue && enqueueCommand(i)) {
      button.waitingToQueue = false;
    }
  }

  // Keep every displayed state, including startup, for at least two seconds.
  // Continue polling buttons throughout the hold interval.
  if (queueCount > 0 && now - stateStartedMs >= MIN_STATE_MS) {
    const uint8_t buttonIndex = commandQueue[queueHead];
    queueHead = (queueHead + 1) % QUEUE_CAPACITY;
    --queueCount;

    displayState(servoAngles[buttonIndex], litLedCounts[buttonIndex]);
  }
}
