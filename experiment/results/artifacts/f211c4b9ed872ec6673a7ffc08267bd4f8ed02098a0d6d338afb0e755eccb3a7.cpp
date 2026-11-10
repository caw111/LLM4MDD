#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t PIN_LED = 13;     // D13 LED
static const uint8_t PIN_BTN = 2;      // D2 button
static const uint8_t PIN_CLK = 4;      // TM1637 CLK = D4
static const uint8_t PIN_DIO = 5;      // TM1637 DIO = D5
static const uint8_t PIN_POT = A0;     // A0 potentiometer

// -------------------- Timing / UI behavior (configurable system) --------------------
// The potentiometer controls the blinking period (in milliseconds).
// The 7-seg shows the current period value (in seconds) with 1 decimal place.
// If you want to change scaling, adjust POT period mapping and display formatting.
static const uint16_t PERIOD_MIN_MS = 100;     // min blinking period
static const uint16_t PERIOD_MAX_MS = 5000;    // max blinking period

// Convert ms period to a display value (tenths of seconds).
// Display range should fit TM1637 4 digits: up to 9.9 seconds fits comfortably.
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t readPeriodMsFromPot();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t periodTenthsSecondsFromMs(uint16_t periodMs);
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showPeriodTM1637(TM1637Display &disp, uint16_t periodMs);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool buttonPressedEdge();
#line 128 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyPeriodToBlink(uint16_t newPeriodMs);
#line 138 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 158 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_012\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t readPeriodMsFromPot()
{
  // Smooth-ish reading could be added, but requirement does not specify; use direct mapping.
  int pot = analogRead(PIN_POT); // 0..1023
  uint32_t span = (uint32_t)PERIOD_MAX_MS - PERIOD_MIN_MS;
  uint32_t ms = PERIOD_MIN_MS + (span * (uint32_t)pot) / 1023UL;
  return (uint16_t)ms;
}

static uint16_t periodTenthsSecondsFromMs(uint16_t periodMs)
{
  // Convert ms to tenths of seconds with rounding.
  // tenths = round(ms / 100)
  return (uint16_t)((periodMs + 50) / 100);
}

static void showPeriodTM1637(TM1637Display &disp, uint16_t periodMs)
{
  // Show X.X seconds using 4 digits: e.g., "1.2" or "12.3".
  // Use tenths-seconds: value in 0.1s units.
  // Example: 1200ms -> 12 tenths -> 1.2 seconds.
  uint16_t tenths = periodTenthsSecondsFromMs(periodMs); // 0..(5000+50)/100 = 50
  // We will display seconds with 1 decimal place:
  // seconds integer = tenths / 10, fractional = tenths % 10
  uint8_t secInt = tenths / 10; // 0..5
  uint8_t secFrac = tenths % 10;

  // Map to digits: D1 D2 D3 D4
  // We'll format as "s.f" (right-aligned) e.g., for 1.2 -> "  1.2"
  // TM1637Display::showNumberDecEx supports decimal point with bitmask.
  // Use showNumberDecEx with custom decimal position:
  // Provide value = secInt*10 + secFrac (tenths seconds) and set decimal point between them.
  // For tenths seconds "12" means 1.2 seconds => value is 12; decimal at position 1 (from right).
  // showNumberDecEx positions decimal by mask: (1..4) => depends on library:
  // TM1637Display in many implementations uses bit mask: 0x01=pos1, 0x02=pos2, 0x04=pos3, 0x08=pos4.
  // We'll call showNumberDecEx(value, mask, leadingZeros)
  // value is tenths seconds (e.g. 12), which would display "0.12" unless digits count differs.
  // So instead build raw segment bytes is complex.
  // Simpler: use showNumberDec for integer tenths and set decimal point so it becomes X.X.
  // For 0..5 seconds, tenths (0..50) display as up to two digits.
  // We'll display using showNumberDecEx(tenths, 0x40, true) style? Not portable.
  //
  // To ensure correct formatting, we will use disp.showNumberDecEx on tenths with decimal at second digit:
  // Many common TM1637Display versions define decimal points mask as:
  //   0x01 -> 1st digit (rightmost), 0x02 -> 2nd digit, 0x04 -> 3rd, 0x08 -> 4th.
  // We'll use 0x02 to place decimal between digit 2 and 1 (i.e., between tenths units and fractional).
  // Since tenths is 2 digits maximum (0..50), showNumberDecEx will put it right-aligned in 2 digits,
  // but the decimal point relative positions will be applied to the 4-digit buffer accordingly.
  // We'll set leading zeros to false and rely on right alignment.

  // Value to display: tenths
  uint16_t value = tenths;

  // If value < 10 (e.g. 0.7s => tenths=7), we want "0.7"
  // We'll ensure leading zero for integer seconds when tenths < 10 by formatting with 2 digits.
  bool addLeadingZero = (secInt == 0);

  // Use 2-digit display with decimal between them:
  // We'll show value as two digits "ab" where a=secInt, b=secFrac for tenths seconds when secInt=0..5.
  // Build packed two-digit number: secInt*10 + secFrac, which equals tenths.
  // Then show in 2 digits by right-aligning into 4 digits: just pass value.
  // Decimal point between digits: apply mask for 2nd digit from right -> 0x02 in typical libs.
  uint8_t decimalMask = 0x02;

  // Leading zeros: if addLeadingZero, we can set leading zeros true; otherwise false.
  disp.showNumberDecEx(value, decimalMask, addLeadingZero);
}

TM1637Display display(PIN_CLK, PIN_DIO);

// -------------------- Button toggle with debouncing --------------------
static const uint16_t DEBOUNCE_MS = 30;

static bool running = false;
static unsigned long lastBtnChangeMs = 0;
static bool lastBtnRaw = HIGH;

static bool buttonPressedEdge()
{
  // Active-low button assumed: requires wiring to pull-up / use INPUT_PULLUP.
  bool raw = digitalRead(PIN_BTN);
  bool pressed = false;

  if (raw != lastBtnRaw) {
    lastBtnChangeMs = millis();
    lastBtnRaw = raw;
  }

  // Consider stable after debounce
  if ((millis() - lastBtnChangeMs) >= DEBOUNCE_MS) {
    // Pressed edge: transitions to LOW after being stable HIGH
    static bool stablePrev = HIGH;
    if (raw != stablePrev) {
      bool newStable = raw;
      bool oldStable = stablePrev;
      stablePrev = newStable;
      if (oldStable == HIGH && newStable == LOW) {
        pressed = true;
      }
    }
  }
  return pressed;
}

// -------------------- Blinking --------------------
static bool ledState = false;
static unsigned long lastToggleMs = 0;
static uint16_t currentPeriodMs = PERIOD_MIN_MS;

static void applyPeriodToBlink(uint16_t newPeriodMs)
{
  currentPeriodMs = newPeriodMs;

  // Keep current phase: do not reset lastToggleMs unless period changes while running.
  // Requirement doesn't specify; but to "preserve all behavior", we maintain phase:
  // If the period changes while running, the next toggle timing uses the updated period.
  // We keep ledState and lastToggleMs to let it adjust naturally.
}

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_BTN, INPUT_PULLUP); // button on D2, active-low

  analogReference(DEFAULT);
  analogRead(PIN_POT); // warm-up

  display.setBrightness(0x0f);

  currentPeriodMs = readPeriodMsFromPot();
  showPeriodTM1637(display, currentPeriodMs);

  running = false; // start stopped
  ledState = false;
  lastToggleMs = millis();
}

void loop()
{
  // Update period from potentiometer continuously.
  uint16_t potPeriodMs = readPeriodMsFromPot();
  if (potPeriodMs != currentPeriodMs) {
    applyPeriodToBlink(potPeriodMs);
  }

  // Always display current configured period value.
  showPeriodTM1637(display, currentPeriodMs);

  // Button toggles start/stop
  if (buttonPressedEdge()) {
    running = !running;
    if (!running) {
      // Stop: LED off, but keep current period setting.
      ledState = false;
      digitalWrite(PIN_LED, LOW);
    } else {
      // Start: begin blinking using currentPeriodMs.
      lastToggleMs = millis();
      ledState = false;
      digitalWrite(PIN_LED, LOW);
    }
  }

  // Blinking while running
  if (running) {
    unsigned long now = millis();
    if ((now - lastToggleMs) >= currentPeriodMs) {
      lastToggleMs = now;
      ledState = !ledState;
      digitalWrite(PIN_LED, ledState ? HIGH : LOW);
    }
  }

  // If not running, LED remains OFF (stop behavior).
}
