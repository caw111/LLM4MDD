#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t CLK_PIN = 4;
constexpr uint8_t DIO_PIN = 5;
constexpr uint8_t ADD_BUTTON_PIN = 2;
constexpr uint8_t START_BUTTON_PIN = 3;
constexpr uint8_t BUZZER_PIN = 6;

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long ALARM_DURATION_MS = 2000;
constexpr unsigned int ALARM_FREQUENCY_HZ = 2000;

TM1637Display display(CLK_PIN, DIO_PIN);

// Connect each button between its input pin and GND.
class DebouncedButton {
public:
  explicit DebouncedButton(uint8_t pin) : pin_(pin) {}

  void begin() {
    pinMode(pin_, INPUT_PULLUP);
    rawState_ = digitalRead(pin_);
    stableState_ = rawState_;
    lastChangeMs_ = millis();
  }

  bool pressed(unsigned long now) {
    const uint8_t reading = digitalRead(pin_);

    if (reading != rawState_) {
      rawState_ = reading;
      lastChangeMs_ = now;
    }

    if (stableState_ != rawState_ &&
        now - lastChangeMs_ >= DEBOUNCE_MS) {
      stableState_ = rawState_;
      return stableState_ == LOW;
    }

    return false;
  }

private:
  uint8_t pin_;
  uint8_t rawState_ = HIGH;
  uint8_t stableState_ = HIGH;
  unsigned long lastChangeMs_ = 0;
};

DebouncedButton addButton(ADD_BUTTON_PIN);
DebouncedButton startButton(START_BUTTON_PIN);

enum class State : uint8_t {
  IDLE,
  COUNTING,
  ALARM
};

State state = State::IDLE;
uint64_t remainingSeconds = 0;
unsigned long lastTickMs = 0;
unsigned long alarmStartedMs = 0;

void updateDisplay() {
  static uint64_t lastDisplayedSeconds = UINT64_MAX;

  if (remainingSeconds == lastDisplayedSeconds) {
    return;
  }

  lastDisplayedSeconds = remainingSeconds;

  if (remainingSeconds <= 9999ULL) {
    display.showNumberDec(static_cast<int>(remainingSeconds), false);
  } else {
    // Four digits cannot represent larger values. Keep the complete
    // countdown internally and show an overflow indication.
    const uint8_t overflow[] = {
      SEG_G, SEG_G, SEG_G, SEG_G
    };
    display.setSegments(overflow);
  }
}

void updateCountdown(unsigned long now) {
  if (state != State::COUNTING) {
    return;
  }

  const unsigned long elapsedSeconds = (now - lastTickMs) / 1000UL;
  if (elapsedSeconds == 0) {
    return;
  }

  lastTickMs += elapsedSeconds * 1000UL;

  if (static_cast<uint64_t>(elapsedSeconds) >= remainingSeconds) {
    remainingSeconds = 0;
    state = State::ALARM;
    alarmStartedMs = now;
    tone(BUZZER_PIN, ALARM_FREQUENCY_HZ);
  } else {
    remainingSeconds -= elapsedSeconds;
  }
}

void setup() {
  addButton.begin();
  startButton.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  display.setBrightness(7, true);
  updateDisplay();
}

void loop() {
  const unsigned long now = millis();

  updateCountdown(now);

  if (state == State::ALARM &&
      now - alarmStartedMs >= ALARM_DURATION_MS) {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
    state = State::IDLE;
  }

  const bool addPressed = addButton.pressed(now);
  const bool startPressed = startButton.pressed(now);

  // Each debounced press adds exactly 10 seconds, including while running.
  if (addPressed) {
    remainingSeconds += 10ULL;
  }

  // Repeated START presses do not reset an active countdown or interrupt
  // the required two-second continuous alarm.
  if (startPressed &&
      state == State::IDLE &&
      remainingSeconds > 0) {
    lastTickMs = now;
    state = State::COUNTING;
  }

  updateDisplay();
}