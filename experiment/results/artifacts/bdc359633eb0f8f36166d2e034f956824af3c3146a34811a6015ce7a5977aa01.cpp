#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Pin assignments (UNO, per resolved configuration)
static const uint8_t TM1637_CLK = 4; // D4
static const uint8_t TM1637_DIO = 5; // D5

static const uint8_t BTN_ADD     = 2; // D2
static const uint8_t BTN_START   = 3; // D3

static const uint8_t BUZZER_PIN  = 6; // D6

TM1637Display display(TM1637_CLK, TM1637_DIO);

// Countdown behavior
static const uint32_t ADD_STEP_MS = 10UL * 1000UL; // +10 seconds per press
static const uint32_t BEEP_MS      = 2000UL;        // continuous beep for 2 seconds

// Timing
static const uint32_t DEBOUNCE_MS = 30; // basic debounce

uint32_t remainingMs = 0;  // displayed value = remaining seconds
bool running = false;

uint32_t lastTickMs = 0;

// Simple debounced button state tracking
struct Button {
  uint8_t pin;
  bool lastReading;
  bool stableState;
  uint32_t lastDebounceMs;

  void begin(uint8_t p) {
    pin = p;
    pinMode(pin, INPUT_PULLUP); // assumes buttons to GND
    lastReading = digitalRead(pin);
    stableState = lastReading;
    lastDebounceMs = millis();
  }

  // Returns true on rising edge of "pressed" state (active-low -> pressed when LOW)
  bool pressedEdge() {
    bool reading = digitalRead(pin);
    if (reading != lastReading) {
      lastDebounceMs = millis();
      lastReading = reading;
    }
    if ((millis() - lastDebounceMs) >= DEBOUNCE_MS) {
      if (reading != stableState) {
        stableState = reading;
        // pressedEdge: transition to LOW (pressed)
        if (stableState == LOW) return true;
      }
    }
    return false;
  }
};

Button btnAdd;
Button btnStart;

#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setDisplaySeconds(uint32_t seconds);
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_019\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setDisplaySeconds(uint32_t seconds) {
  // TM1637Display shows an integer; limit to 0..9999 (typical behavior)
  if (seconds > 9999) seconds = 9999;
  display.showNumberDec((int)seconds, false); // no leading zeros
}

void setup() {
  btnAdd.begin(BTN_ADD);
  btnStart.begin(BTN_START);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  display.setBrightness(0x0f); // max brightness (common default)
  remainingMs = 0;
  running = false;
  lastTickMs = millis();
  setDisplaySeconds(0);
}

void loop() {
  uint32_t now = millis();

  // Button D2: add 10 seconds
  if (!running) {
    if (btnAdd.pressedEdge()) {
      remainingMs += ADD_STEP_MS;
      setDisplaySeconds(remainingMs / 1000UL);
    }
  } else {
    // While running, ignore add presses to preserve behavior as countdown in progress
  }

  // Button D3: start countdown
  if (!running) {
    if (btnStart.pressedEdge()) {
      running = true;
      lastTickMs = now;
      // If remaining is 0, keep displaying 0 and run logic (will beep immediately)
    }
  }

  if (running) {
    uint32_t elapsed = now - lastTickMs;
    if (elapsed >= 1000UL) {
      uint32_t secondsElapsed = elapsed / 1000UL;
      lastTickMs += secondsElapsed * 1000UL;

      if (remainingMs > secondsElapsed * 1000UL) {
        remainingMs -= secondsElapsed * 1000UL;
      } else {
        remainingMs = 0;
      }

      setDisplaySeconds(remainingMs / 1000UL);
    }

    // End condition
    if (remainingMs == 0) {
      // Continuous beep for 2 seconds
      uint32_t beepStart = millis();
      while (millis() - beepStart < BEEP_MS) {
        digitalWrite(BUZZER_PIN, HIGH);
      }
      digitalWrite(BUZZER_PIN, LOW);

      // Stop after beeping; keep display at 0
      running = false;
      setDisplaySeconds(0);

      // Ensure tick base updated
      lastTickMs = millis();
    }
  }
}
