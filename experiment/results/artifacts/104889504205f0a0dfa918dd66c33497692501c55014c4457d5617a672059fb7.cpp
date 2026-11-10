#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

#define BTN_INC_PIN 2   // D2
#define BTN_DEC_PIN 3   // D3

#define BUZZER_PIN   6   // D6

#define TM1637_CLK   4   // D4
#define TM1637_DIO   5   // D5

static const int COUNT_MIN = 0;
static const int COUNT_MAX = 20;

TM1637Display display(TM1637_CLK, TM1637_DIO);

static int counterValue = 0;

// Simple debouncing + repeat control
static unsigned long lastIncTime = 0;
static unsigned long lastDecTime = 0;
static bool lastIncState = HIGH;
static bool lastDecState = HIGH;

// Debounce and autorepeat timing (ms)
static const unsigned long DEBOUNCE_MS = 40;
static const unsigned long REPEAT_DELAY_MS = 300;
static const unsigned long REPEAT_INTERVAL_MS = 80;

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool handleButton( uint8_t pin, bool &lastState, unsigned long &lastActionTime, unsigned long nowMs );
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 90 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void buzzAlarm(unsigned int durationMs = 150) {
  // Beep for a short duration; avoid blocking too long is not required by spec.
  tone(BUZZER_PIN, 2000, durationMs);
}

static bool handleButton(
  uint8_t pin,
  bool &lastState,
  unsigned long &lastActionTime,
  unsigned long nowMs
) {
  bool reading = digitalRead(pin);

  // Debounce edge detection (pressed when reading goes LOW)
  if (reading != lastState) {
    // state change detected; wait debounce window
    delay(DEBOUNCE_MS);
    reading = digitalRead(pin);
    if (reading != lastState) {
      lastState = reading;
      // Only act on press edge
      if (lastState == LOW) {
        lastActionTime = nowMs;
        return true;
      }
    }
  }

  // Autorepeat while held down
  if (lastState == LOW) {
    if (nowMs - lastActionTime >= REPEAT_DELAY_MS) {
      // Once delay passed, trigger at interval
      unsigned long elapsed = nowMs - lastActionTime;
      if (elapsed >= REPEAT_DELAY_MS) {
        // Trigger repeatedly with interval; update anchor after each trigger
        unsigned long countIntervals = (elapsed - REPEAT_DELAY_MS) / REPEAT_INTERVAL_MS;
        if (countIntervals > 0) {
          // Move anchor forward by intervals to prevent drift
          lastActionTime += countIntervals * REPEAT_INTERVAL_MS;
          return true;
        }
      }
    }
  }

  return false;
}

void setup() {
  pinMode(BTN_INC_PIN, INPUT_PULLUP); // D2
  pinMode(BTN_DEC_PIN, INPUT_PULLUP); // D3
  pinMode(BUZZER_PIN, OUTPUT);        // D6 (tone uses output)

  display.setBrightness(0x0f); // reasonable brightness
  display.clear();

  // Real-time display of initial counter
  display.showNumberDec(counterValue, false, 2, 0);
}

void loop() {
  unsigned long nowMs = millis();

  // Handle increment
  bool incPressed = handleButton(BTN_INC_PIN, lastIncState, lastIncTime, nowMs);
  if (incPressed) {
    if (counterValue >= COUNT_MAX) {
      // At upper limit: alarm and forbid further increment
      buzzAlarm(150);
    } else {
      counterValue++;
      display.showNumberDec(counterValue, false, 2, 0);
    }
  }

  // Handle decrement
  bool decPressed = handleButton(BTN_DEC_PIN, lastDecState, lastDecTime, nowMs);
  if (decPressed) {
    if (counterValue <= COUNT_MIN) {
      // At lower limit: alarm and forbid further decrement
      buzzAlarm(150);
    } else {
      counterValue--;
      display.showNumberDec(counterValue, false, 2, 0);
    }
  }

  // Keep display always showing current value (real-time).
  // (Not strictly required since we update on change, but satisfies "实时显示计数值".)
  static int lastShown = -32768;
  if (lastShown != counterValue) {
    display.showNumberDec(counterValue, false, 2, 0);
    lastShown = counterValue;
  }

  // No other behavior specified.
}
