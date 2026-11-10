#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_83\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

const uint8_t BTN1_PIN = 2;
const uint8_t BTN2_PIN = 3;
const uint8_t RED_PIN = 4;
const uint8_t GREEN_PIN = 5;
const uint8_t BLUE_PIN = 6;
const uint8_t SERVO1_PIN = 7;
const uint8_t SERVO2_PIN = 8;

const unsigned long DEBOUNCE_MS = 30;
const unsigned long MIN_STATE_MS = 2000;

Servo servo1;
Servo servo2;

struct DebouncedButton {
  uint8_t pin;
  bool rawPressed;
  bool pressed;
  bool justPressed;
  unsigned long lastChange;

  void begin(uint8_t buttonPin) {
    pin = buttonPin;
    pinMode(pin, INPUT_PULLUP);
    rawPressed = false;
    pressed = false;
    justPressed = false;
    lastChange = millis();
  }

  void update(unsigned long now) {
    justPressed = false;
    const bool reading = (digitalRead(pin) == LOW);

    if (reading != rawPressed) {
      rawPressed = reading;
      lastChange = now;
    }

    if (rawPressed != pressed &&
        (unsigned long)(now - lastChange) >= DEBOUNCE_MS) {
      pressed = rawPressed;
      justPressed = pressed;
    }
  }
};

DebouncedButton btn1;
DebouncedButton btn2;

enum State {
  READY,
  BUTTON1_LATCHED,
  BUTTON2_LATCHED,
  RESET_GREEN
};

State state = READY;
unsigned long stateStarted = 0;
bool resetPending = false;

#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_83\\direct\\attempt_0\\sketch\\sketch.ino"
void setColor(bool red, bool green, bool blue);
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_83\\direct\\attempt_0\\sketch\\sketch.ino"
void startSingleButton(uint8_t button, unsigned long now);
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_83\\direct\\attempt_0\\sketch\\sketch.ino"
void startReset(unsigned long now);
#line 94 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_83\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 112 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_83\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_83\\direct\\attempt_0\\sketch\\sketch.ino"
void setColor(bool red, bool green, bool blue) {
  // The RGB LED has a common cathode: HIGH turns a channel on.
  digitalWrite(RED_PIN, red ? HIGH : LOW);
  digitalWrite(GREEN_PIN, green ? HIGH : LOW);
  digitalWrite(BLUE_PIN, blue ? HIGH : LOW);
}

void startSingleButton(uint8_t button, unsigned long now) {
  stateStarted = now;

  if (button == 1) {
    servo1.write(90);
    setColor(true, false, false);
    state = BUTTON1_LATCHED;
  } else {
    servo2.write(180);
    setColor(false, false, true);
    state = BUTTON2_LATCHED;
  }
}

void startReset(unsigned long now) {
  servo1.write(0);
  servo2.write(0);
  setColor(false, true, false);
  stateStarted = now;
  state = RESET_GREEN;
  resetPending = false;
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  setColor(false, false, false);

  btn1.begin(BTN1_PIN);
  btn2.begin(BTN2_PIN);

  // Preload the initial positions before enabling the servo outputs.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(0);
  servo2.write(0);
}

void loop() {
  const unsigned long now = millis();

  btn1.update(now);
  btn2.update(now);

  const bool bothPressed = btn1.pressed && btn2.pressed;
  const bool neitherPressed = !btn1.pressed && !btn2.pressed;
  const bool minimumElapsed =
      (unsigned long)(now - stateStarted) >= MIN_STATE_MS;

  switch (state) {
    case READY:
      if (bothPressed) {
        startReset(now);
      } else if (btn1.justPressed) {
        startSingleButton(1, now);
      } else if (btn2.justPressed) {
        startSingleButton(2, now);
      }
      break;

    case BUTTON1_LATCHED:
    case BUTTON2_LATCHED: {
      // Remember the first button even after release. Pressing the other
      // button requests a reset, without shortening the first 2-second state.
      const bool firstWasButton1 = (state == BUTTON1_LATCHED);
      const bool otherPressed =
          firstWasButton1 ? btn2.justPressed : btn1.justPressed;

      if (bothPressed || otherPressed) {
        resetPending = true;
      }

      if (resetPending) {
        if (minimumElapsed) {
          startReset(now);
        }
        break;
      }

      const bool firstPressedAgain =
          firstWasButton1 ? btn1.justPressed : btn2.justPressed;

      if (firstPressedAgain) {
        startSingleButton(firstWasButton1 ? 1 : 2, now);
      } else if (minimumElapsed && neitherPressed) {
        setColor(false, false, false);
      }
      break;
    }

    case RESET_GREEN:
      // Physical buttons cannot be released by software. Consume their
      // presses and require both to be released before accepting a new cycle.
      if (minimumElapsed && neitherPressed) {
        setColor(false, false, false);
        state = READY;
      }
      break;
  }
}
