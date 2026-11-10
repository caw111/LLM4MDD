#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_009\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t START_PAUSE_PIN = 2;
constexpr uint8_t RESET_PIN = 3;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;
constexpr uint8_t BUZZER_PIN = 6;

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long ALARM_DURATION_MS = 1000;
constexpr uint16_t ALARM_FREQUENCY_HZ = 2000;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

class DebouncedButton {
public:
  explicit DebouncedButton(uint8_t pin) : pin_(pin) {}

  void begin() {
    pinMode(pin_, INPUT_PULLUP);
    rawState_ = stableState_ = digitalRead(pin_);
    lastChangeMs_ = millis();
  }

  bool pressed(unsigned long now) {
    const uint8_t reading = digitalRead(pin_);

    if (reading != rawState_) {
      rawState_ = reading;
      lastChangeMs_ = now;
    }

    if (rawState_ != stableState_ &&
        (unsigned long)(now - lastChangeMs_) >= DEBOUNCE_MS) {
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

DebouncedButton startPauseButton(START_PAUSE_PIN);
DebouncedButton resetButton(RESET_PIN);

bool running = false;
bool alarmTriggered = false;
bool alarmActive = false;

uint64_t elapsedMs = 0;
unsigned long lastUpdateMs = 0;
unsigned long alarmStartedMs = 0;
uint16_t lastDisplayedSeconds = 0xFFFF;

#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_009\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_009\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_009\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  startPauseButton.begin();
  resetButton.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  display.setBrightness(7, true);
  display.showNumberDec(0, false);
  lastDisplayedSeconds = 0;

  lastUpdateMs = millis();
}

void loop() {
  const unsigned long now = millis();
  const unsigned long deltaMs = now - lastUpdateMs;
  lastUpdateMs = now;

  if (running) {
    elapsedMs += deltaMs;
  }

  const bool startPausePressed = startPauseButton.pressed(now);
  const bool resetPressed = resetButton.pressed(now);

  // Reset takes priority if both buttons are pressed together.
  if (resetPressed) {
    running = false;
    elapsedMs = 0;
    alarmTriggered = false;
    alarmActive = false;
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  } else if (startPausePressed) {
    running = !running;
  }

  // Sound once upon reaching 60 seconds; counting continues.
  // Resetting allows the alarm to trigger again.
  if (!alarmTriggered && elapsedMs >= 60000ULL) {
    alarmTriggered = true;
    alarmActive = true;
    alarmStartedMs = now;
    tone(BUZZER_PIN, ALARM_FREQUENCY_HZ, ALARM_DURATION_MS);
  }

  if (alarmActive &&
      (unsigned long)(now - alarmStartedMs) >= ALARM_DURATION_MS) {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
    alarmActive = false;
  }

  // The four-digit display shows seconds, wrapping after 9999.
  const uint16_t displayedSeconds =
      static_cast<uint16_t>((elapsedMs / 1000ULL) % 10000ULL);

  if (displayedSeconds != lastDisplayedSeconds) {
    display.showNumberDec(displayedSeconds, false);
    lastDisplayedSeconds = displayedSeconds;
  }
}
