#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_117\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t led1Pin = 2;
const uint8_t led2Pin = 3;

const uint8_t btn1Pin = 4;
const uint8_t btn2Pin = 5;
const uint8_t btn3Pin = 6;

// Simple debounce parameters
const unsigned long debounceMs = 50;
// Stability window after a recognized press/toggle/reset
const unsigned long stableAfterToggleMs = 2000;

bool led1State = false;
bool led2State = false;

bool lastBtn1Reading = HIGH;
bool lastBtn2Reading = HIGH;
bool lastBtn3Reading = HIGH;

bool debouncedBtn1 = HIGH;
bool debouncedBtn2 = HIGH;
bool debouncedBtn3 = HIGH;

unsigned long btn1LastChangeMs = 0;
unsigned long btn2LastChangeMs = 0;
unsigned long btn3LastChangeMs = 0;

unsigned long led1LockUntilMs = 0;
unsigned long led2LockUntilMs = 0;

// For btn3 (reset both), lock both LEDs for 2s after reset
unsigned long bothLockUntilMs = 0;

#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_117\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool updateDebounce(uint8_t pin, bool &debouncedState, bool &lastReading, unsigned long &lastChangeMs);
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_117\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_117\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_117\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool updateDebounce(uint8_t pin, bool &debouncedState, bool &lastReading, unsigned long &lastChangeMs) {
  bool reading = digitalRead(pin);

  if (reading != lastReading) {
    lastReading = reading;
    lastChangeMs = millis();
  }

  if ((millis() - lastChangeMs) >= debounceMs && reading != debouncedState) {
    debouncedState = reading;
  }

  return debouncedState;
}

void setup() {
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  // Buttons wired with external pull-down/up? Circuit uses button left terminal to a Mega pin,
  // and right terminal to GND. That means the pin sees HIGH when pressed and LOW otherwise.
  pinMode(btn1Pin, INPUT);
  pinMode(btn2Pin, INPUT);
  pinMode(btn3Pin, INPUT);

  // Initialize: both LEDs off
  led1State = false;
  led2State = false;
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);

  // Initialize debounce tracking
  lastBtn1Reading = digitalRead(btn1Pin);
  lastBtn2Reading = digitalRead(btn2Pin);
  lastBtn3Reading = digitalRead(btn3Pin);

  debouncedBtn1 = lastBtn1Reading;
  debouncedBtn2 = lastBtn2Reading;
  debouncedBtn3 = lastBtn3Reading;

  btn1LastChangeMs = millis();
  btn2LastChangeMs = millis();
  btn3LastChangeMs = millis();
}

void loop() {
  const unsigned long now = millis();

  // Update debounced button states
  updateDebounce(btn1Pin, debouncedBtn1, lastBtn1Reading, btn1LastChangeMs);
  updateDebounce(btn2Pin, debouncedBtn2, lastBtn2Reading, btn2LastChangeMs);
  updateDebounce(btn3Pin, debouncedBtn3, lastBtn3Reading, btn3LastChangeMs);

  // Detect rising edge for "press" (released -> pressed, since pressed gives HIGH)
  // We'll use last debounced states by checking transitions.
  static bool prevBtn1 = debouncedBtn1;
  static bool prevBtn2 = debouncedBtn2;
  static bool prevBtn3 = debouncedBtn3;

  bool btn1PressedEvent = (!prevBtn1 && debouncedBtn1);
  bool btn2PressedEvent = (!prevBtn2 && debouncedBtn2);
  bool btn3PressedEvent = (!prevBtn3 && debouncedBtn3);

  // Update previous states
  prevBtn1 = debouncedBtn1;
  prevBtn2 = debouncedBtn2;
  prevBtn3 = debouncedBtn3;

  // If btn3 pressed: reset both LEDs off, stable for 2 seconds
  if (btn3PressedEvent) {
    // Apply reset regardless of current state
    led1State = false;
    led2State = false;
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);

    bothLockUntilMs = now + stableAfterToggleMs;
    led1LockUntilMs = bothLockUntilMs;
    led2LockUntilMs = bothLockUntilMs;
  }

  // Handle btn1 toggle with stability lock
  if (btn1PressedEvent) {
    if (now >= led1LockUntilMs) {
      // If a reset lock is active, this will also prevent toggling due to led1LockUntilMs.
      led1State = !led1State;
      digitalWrite(led1Pin, led1State ? HIGH : LOW);
      led1LockUntilMs = now + stableAfterToggleMs;
    }
  }

  // Handle btn2 toggle with stability lock
  if (btn2PressedEvent) {
    if (now >= led2LockUntilMs) {
      led2State = !led2State;
      digitalWrite(led2Pin, led2State ? HIGH : LOW);
      led2LockUntilMs = now + stableAfterToggleMs;
    }
  }
}
