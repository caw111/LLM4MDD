#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------- Pins (as specified) --------
static const uint8_t PIN_BTN   = 2;  // D2
static const uint8_t PIN_CLK   = 4;  // D4 (TM1637 CLK)
static const uint8_t PIN_DIO   = 5;  // D5 (TM1637 DIO)
static const uint8_t PIN_A0    = A0; // Analog input (pot)

// -------- TM1637 --------
TM1637Display display(PIN_CLK, PIN_DIO);

// -------- Button handling (cycle mode) --------
static const uint16_t DEBOUNCE_MS = 30;
static const uint16_t LONGPRESS_MS = 1000; // keep for robustness (no forced reset unless long press)

volatile bool g_btnStablePressed = false;

static uint8_t modeIdx = 0; // 0=count, 1=pot value, 2=1234
static unsigned long lastDebounceTime = 0;
static bool lastBtnRead = HIGH; // due to INPUT_PULLUP
static bool btnLatched = false;
static unsigned long btnPressStart = 0;

// -------- Count behavior --------
static unsigned long countValue = 0;
static unsigned long lastCountMs = 0;
static const uint16_t COUNT_INTERVAL_MS = 250; // deterministic periodic increment

// -------- Display helpers --------
static const uint8_t BRIGHTNESS = 5; // 0..7 typical for TM1637 library

#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showNumber4digits(uint16_t value);
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showFixed1234();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t readPotMappedTo0_9999();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void advanceMode();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleButtonEvent();
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showNumber4digits(uint16_t value) {
  // Show 0..9999 right-aligned, will clip above 9999 (library handles mod if needed, but we clamp)
  if (value > 9999) value = 9999;
  display.showNumberDecEx(value, 0b00000000, true);
}

static void showFixed1234() {
  display.showNumberDecEx(1234, 0b00000000, true);
}

static uint16_t readPotMappedTo0_9999() {
  // Map analog A0 (0..1023) to 0..9999
  uint16_t raw = analogRead(PIN_A0);
  // Use long to prevent overflow
  uint32_t mapped = (uint32_t)raw * 9999UL / 1023UL;
  return (uint16_t)mapped;
}

static void advanceMode() {
  modeIdx++;
  if (modeIdx > 2) modeIdx = 0;
}

// Optional: long press to reset count (kept conservative; only affects count mode)
static void handleButtonEvent() {
  // Called when a stable rising edge indicates a press.
  if (!btnLatched) return;
  btnLatched = false;

  unsigned long now = millis();
  unsigned long pressDur = now - btnPressStart;

  if (pressDur >= LONGPRESS_MS) {
    // Long press: reset count and set mode to count
    countValue = 0;
    modeIdx = 0;
  } else {
    // Short press: cycle mode
    advanceMode();
  }
}

// Interrupt not required; use polling for deterministic behavior on AVR.
void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP);

  display.setBrightness(BRIGHTNESS);
  display.clear();

  // Initial mode: count
  modeIdx = 0;
  lastCountMs = millis();
  lastDebounceTime = millis();

  lastBtnRead = digitalRead(PIN_BTN);
  btnLatched = false;
  btnPressStart = 0;

  // Prime display
  showNumber4digits((uint16_t)countValue);
}

void loop() {
  // ---- Button debounce & press detection (polling) ----
  bool btnRead = digitalRead(PIN_BTN); // HIGH when not pressed (INPUT_PULLUP), LOW when pressed
  unsigned long now = millis();

  if (btnRead != lastBtnRead) {
    lastDebounceTime = now;
    lastBtnRead = btnRead;
  }

  if ((now - lastDebounceTime) >= DEBOUNCE_MS) {
    // Stable state reached
    if (btnRead == LOW) {
      // Button pressed
      if (!btnLatched) {
        btnLatched = true;
        btnPressStart = now;
      }
    } else {
      // Button released
      if (btnLatched) {
        // Treat release after stable press as event trigger (prevents repeated triggers)
        // Use the recorded press duration.
        handleButtonEvent();
      }
    }
  }

  // ---- Mode actions / updates ----
  switch (modeIdx) {
    case 0: {
      // Count mode: increment periodically
      if (now - lastCountMs >= COUNT_INTERVAL_MS) {
        lastCountMs = now;
        countValue++;
      }
      showNumber4digits((uint16_t)countValue);
      break;
    }
    case 1: {
      // Pot value mode: show mapped A0 -> 0..9999
      uint16_t potVal = readPotMappedTo0_9999();
      showNumber4digits(potVal);
      break;
    }
    case 2: {
      // Fixed 1234 mode
      showFixed1234();
      break;
    }
    default: {
      modeIdx = 0;
      showNumber4digits((uint16_t)countValue);
      break;
    }
  }

  // Small delay to reduce display flicker and CPU load; does not affect count interval materially.
  delay(10);
}
