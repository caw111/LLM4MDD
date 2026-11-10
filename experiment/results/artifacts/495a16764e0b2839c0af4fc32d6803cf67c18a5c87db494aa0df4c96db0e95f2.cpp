#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_004\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t BUTTON_UP_PIN = 2;
constexpr uint8_t BUTTON_DOWN_PIN = 3;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;
constexpr uint8_t BUZZER_PIN = 6;

constexpr int MIN_COUNT = 0;
constexpr int MAX_COUNT = 20;
constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned int ALARM_FREQUENCY_HZ = 2000;
constexpr unsigned long ALARM_DURATION_MS = 200;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

struct Button {
  uint8_t pin;
  uint8_t lastReading;
  uint8_t stableState;
  unsigned long lastChangeMs;
};

Button upButton = {BUTTON_UP_PIN, HIGH, HIGH, 0};
Button downButton = {BUTTON_DOWN_PIN, HIGH, HIGH, 0};

int count = 0;

// Buttons connect their input pins to GND when pressed.
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_004\\direct\\attempt_0\\sketch\\sketch.ino"
bool pressed(Button &button, unsigned long now);
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_004\\direct\\attempt_0\\sketch\\sketch.ino"
void changeCount(int delta);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_004\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 75 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_004\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_004\\direct\\attempt_0\\sketch\\sketch.ino"
bool pressed(Button &button, unsigned long now) {
  const uint8_t reading = digitalRead(button.pin);

  if (reading != button.lastReading) {
    button.lastReading = reading;
    button.lastChangeMs = now;
  }

  if (reading != button.stableState &&
      now - button.lastChangeMs >= DEBOUNCE_MS) {
    button.stableState = reading;
    return button.stableState == LOW;
  }

  return false;
}

void changeCount(int delta) {
  const int requestedCount = count + delta;

  if (requestedCount < MIN_COUNT || requestedCount > MAX_COUNT) {
    // Reject out-of-range changes and signal the reached limit.
    tone(BUZZER_PIN, ALARM_FREQUENCY_HZ, ALARM_DURATION_MS);
    return;
  }

  count = requestedCount;
  display.showNumberDec(count, false);

  if (count == MIN_COUNT || count == MAX_COUNT) {
    tone(BUZZER_PIN, ALARM_FREQUENCY_HZ, ALARM_DURATION_MS);
  }
}

void setup() {
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  display.setBrightness(7, true);
  display.showNumberDec(count, false);
}

void loop() {
  const unsigned long now = millis();
  const bool upPressed = pressed(upButton, now);
  const bool downPressed = pressed(downButton, now);

  if (upPressed) {
    changeCount(1);
  }

  if (downPressed) {
    changeCount(-1);
  }
}
