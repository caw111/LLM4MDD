#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_119\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

constexpr uint8_t LED_PIN = 2;
constexpr uint8_t BTN1_PIN = 3;
constexpr uint8_t BTN2_PIN = 4;
constexpr uint8_t SERVO1_PIN = 5;
constexpr uint8_t SERVO2_PIN = 6;

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long MIN_STATE_MS = 2000;
constexpr unsigned long FLASH_HALF_PERIOD_MS = 2000;

Servo servo1;
Servo servo2;

enum SystemState : uint8_t {
  INITIAL,
  BUTTON1_ACTIVE,
  BUTTON2_ACTIVE,
  FLASHING
};

struct Button {
  uint8_t pin;
  uint8_t rawLevel;
  uint8_t stableLevel;
  unsigned long lastChange;
};

Button button1 = {BTN1_PIN, HIGH, HIGH, 0};
Button button2 = {BTN2_PIN, HIGH, HIGH, 0};

SystemState state = INITIAL;
unsigned long stateStarted = 0;
unsigned long flashLastToggle = 0;
bool ledOn = false;

// Preserve the order of presses received during the minimum state hold.
// Each debounced press produces one event; holding a button does not repeat.
constexpr uint8_t QUEUE_SIZE = 128;
uint8_t pressQueue[QUEUE_SIZE];
uint8_t queueHead = 0;
uint8_t queueTail = 0;
uint8_t queueCount = 0;

#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_119\\direct\\attempt_0\\sketch\\sketch.ino"
void enqueuePress(uint8_t buttonNumber);
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_119\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t dequeuePress();
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_119\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButton(Button &button, uint8_t buttonNumber, unsigned long now);
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_119\\direct\\attempt_0\\sketch\\sketch.ino"
void enterState(SystemState nextState, unsigned long now);
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_119\\direct\\attempt_0\\sketch\\sketch.ino"
void handlePress(uint8_t buttonNumber, unsigned long now);
#line 142 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_119\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 161 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_119\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_119\\direct\\attempt_0\\sketch\\sketch.ino"
void enqueuePress(uint8_t buttonNumber) {
  if (queueCount < QUEUE_SIZE) {
    pressQueue[queueTail] = buttonNumber;
    queueTail = (queueTail + 1) % QUEUE_SIZE;
    ++queueCount;
  }
}

uint8_t dequeuePress() {
  uint8_t buttonNumber = pressQueue[queueHead];
  queueHead = (queueHead + 1) % QUEUE_SIZE;
  --queueCount;
  return buttonNumber;
}

void updateButton(Button &button, uint8_t buttonNumber,
                  unsigned long now) {
  uint8_t level = digitalRead(button.pin);

  if (level != button.rawLevel) {
    button.rawLevel = level;
    button.lastChange = now;
  }

  if (button.stableLevel != button.rawLevel &&
      now - button.lastChange >= DEBOUNCE_MS) {
    button.stableLevel = button.rawLevel;

    // Buttons are wired to ground and use internal pull-ups.
    if (button.stableLevel == LOW) {
      enqueuePress(buttonNumber);
    }
  }
}

void enterState(SystemState nextState, unsigned long now) {
  if (nextState == state) {
    return;
  }

  state = nextState;
  stateStarted = now;

  switch (state) {
    case INITIAL:
      servo1.write(0);
      servo2.write(0);
      ledOn = false;
      break;

    case BUTTON1_ACTIVE:
      servo1.write(90);
      servo2.write(0);
      ledOn = true;
      break;

    case BUTTON2_ACTIVE:
      servo1.write(0);
      servo2.write(180);
      ledOn = false;
      break;

    case FLASHING:
      servo1.write(0);
      servo2.write(0);
      ledOn = true;
      flashLastToggle = now;
      break;
  }

  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
}

void handlePress(uint8_t buttonNumber, unsigned long now) {
  switch (state) {
    case INITIAL:
    case FLASHING:
      // A new press exits flashing and starts a fresh button state.
      enterState(buttonNumber == 1 ? BUTTON1_ACTIVE : BUTTON2_ACTIVE,
                 now);
      break;

    case BUTTON1_ACTIVE:
      if (buttonNumber == 2) {
        enterState(FLASHING, now);
      }
      break;

    case BUTTON2_ACTIVE:
      if (buttonNumber == 1) {
        enterState(FLASHING, now);
      }
      break;
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  // Set the initial pulse widths before attaching the servos.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  unsigned long now = millis();
  stateStarted = now;
  button1.lastChange = now;
  button2.lastChange = now;
}

void loop() {
  unsigned long now = millis();

  updateButton(button1, 1, now);
  updateButton(button2, 2, now);

  // Keep every state, including initialization, for at least two seconds.
  // Presses during this interval are queued rather than blocking or lost.
  if (queueCount > 0 && now - stateStarted >= MIN_STATE_MS) {
    handlePress(dequeuePress(), now);
  }

  if (state == FLASHING) {
    unsigned long elapsed = now - flashLastToggle;

    if (elapsed >= FLASH_HALF_PERIOD_MS) {
      unsigned long intervals = elapsed / FLASH_HALF_PERIOD_MS;
      flashLastToggle += intervals * FLASH_HALF_PERIOD_MS;

      if (intervals & 1UL) {
        ledOn = !ledOn;
        digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
      }
    }
  }
}
