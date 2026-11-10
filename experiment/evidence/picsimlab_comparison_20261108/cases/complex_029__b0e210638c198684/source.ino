#include <Arduino.h>
#include <TM1637Display.h>
#include <stdint.h>

constexpr uint8_t INCREMENT_PIN = 2;
constexpr uint8_t DECREMENT_PIN = 3;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t LED_PIN = 13;

constexpr uint32_t DEBOUNCE_MS = 30;
constexpr uint32_t RESET_HOLD_MS = 1500;
constexpr unsigned int ALARM_FREQUENCY_HZ = 2000;
constexpr int32_t ALARM_COUNT = 15;

// Wire each button between its input pin and GND.
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

struct Button {
  uint8_t pin;
  uint8_t rawState;
  uint8_t stableState;
  uint32_t lastChangeAt;
  uint32_t pressedAt;
  bool pressedEvent;

  void begin(uint8_t inputPin) {
    pin = inputPin;
    pinMode(pin, INPUT_PULLUP);
    rawState = digitalRead(pin);
    stableState = HIGH;
    lastChangeAt = millis();
    pressedAt = lastChangeAt;
    pressedEvent = false;
  }

  void update(uint32_t now) {
    pressedEvent = false;
    const uint8_t reading = digitalRead(pin);

    if (reading != rawState) {
      rawState = reading;
      lastChangeAt = now;
    }

    if (stableState != rawState &&
        static_cast<uint32_t>(now - lastChangeAt) >= DEBOUNCE_MS) {
      stableState = rawState;
      if (stableState == LOW) {
        pressedAt = now;
        pressedEvent = true;
      }
    }
  }

  bool isPressed() const {
    return stableState == LOW;
  }
};

Button incrementButton;
Button decrementButton;

int32_t count = 0;
bool alarmLocked = false;
bool resetHoldActive = false;
uint32_t resetHoldStartedAt = 0;

void updateOutputs() {
  digitalWrite(LED_PIN, (count % 2 == 0) ? HIGH : LOW);

  if (count >= -999 && count <= 9999) {
    display.showNumberDec(static_cast<int>(count), false);
  } else {
    // Retain the actual count even when it exceeds the four-digit display.
    const uint8_t overflowSegments[4] = {
      SEG_G, SEG_G, SEG_G, SEG_G
    };
    display.setSegments(overflowSegments);
  }
}

void startAlarm(uint32_t now) {
  alarmLocked = true;
  tone(BUZZER_PIN, ALARM_FREQUENCY_HZ);

  // A D3 press already in progress must still be held for a full
  // reset interval after the alarm starts.
  resetHoldActive = decrementButton.isPressed();
  resetHoldStartedAt = now;
}

void resetSystem() {
  count = 0;
  alarmLocked = false;
  resetHoldActive = false;
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
  updateOutputs();
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  incrementButton.begin(INCREMENT_PIN);
  decrementButton.begin(DECREMENT_PIN);

  display.setBrightness(7, true);
  display.clear();
  updateOutputs();
}

void loop() {
  const uint32_t now = millis();

  incrementButton.update(now);
  decrementButton.update(now);

  if (alarmLocked) {
    if (!decrementButton.isPressed()) {
      resetHoldActive = false;
    } else {
      if (!resetHoldActive) {
        resetHoldActive = true;
        resetHoldStartedAt = now;
      }

      if (static_cast<uint32_t>(now - resetHoldStartedAt) >= RESET_HOLD_MS) {
        resetSystem();
      }
    }
    return;
  }

  if (incrementButton.pressedEvent) {
    ++count;
    updateOutputs();

    if (count == ALARM_COUNT) {
      startAlarm(now);
      return;
    }
  }

  if (decrementButton.pressedEvent) {
    // Avoid signed integer overflow at the storage limit.
    if (count > INT32_MIN) {
      --count;
      updateOutputs();
    }
  }
}