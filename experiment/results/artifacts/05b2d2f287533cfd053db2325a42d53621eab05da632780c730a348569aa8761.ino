#include <Arduino.h>
#include <TM1637Display.h>

// ---- Pin mapping (authoritative) ----
static const uint8_t PIN_BTN_ON   = 2;  // D2 本地开灯
static const uint8_t PIN_BTN_OFF  = 3;  // D3 本地关灯
static const uint8_t PIN_LED      = 13; // D13 LED
static const uint8_t PIN_BUZZER   = 6;  // D6 蜂鸣器 (tone)
static const uint8_t PIN_TM_CLK   = 4;  // D4 TM1637 CLK
static const uint8_t PIN_TM_DIO   = 5;  // D5 TM1637 DIO

// ---- Serial protocol ----
static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;

// Commands (as provided; preserved):
// LIGHT_ON      -> 6
// LIGHT_OFF     -> 5
// BEEP          -> 1
// COUNT_RESET   -> 3
// (Other tokens "CLK" "DIO" are identifiers; not used as executable commands here.)

// ---- Application state ----
static volatile bool g_lightOn = false;
static uint32_t g_count = 0;

// ---- Debounce ----
static const uint32_t BTN_DEBOUNCE_MS = 35;
static uint8_t lastBtnOnStable = HIGH;
static uint8_t lastBtnOffStable = HIGH;
static uint32_t lastBtnOnChangeMs = 0;
static uint32_t lastBtnOffChangeMs = 0;

static uint8_t lastBtnOnReading = HIGH;
static uint8_t lastBtnOffReading = HIGH;

// ---- Buzzer behavior ----
// Requirements didn't specify exact tone/pattern; preserve minimal behavior:
// BEEP command triggers one audible beep and does NOT alter count.
static const uint16_t BEEP_FREQ_HZ = 2000;
static const uint16_t BEEP_MS = 120;

// ---- TM1637 ----
TM1637Display display(PIN_TM_CLK, PIN_TM_DIO);
static const uint8_t TM_BRIGHTNESS = 4;

// ---- Utilities ----
static void setLight(bool on) {
  g_lightOn = on;
  digitalWrite(PIN_LED, on ? HIGH : LOW);
}

static void beepOnce() {
  tone(PIN_BUZZER, BEEP_FREQ_HZ);
  delay(BEEP_MS);
  noTone(PIN_BUZZER);
}

// Count increments only when local buttons trigger a valid ON/OFF operation
// (and when corresponding serial commands are received), i.e., each accepted
// operation increments the cumulative count.
static void doOperation(bool on) {
  // If you want counting only on state change, remove the next if.
  // Here we preserve "操作次数" semantics as each command/button press is an operation.
  g_count++;
  setLight(on);
}

// ---- Serial line-or-idle parser ----
// framing=line_or_idle: accept either line-based tokens separated by newline,
// or idle-delimited tokens if no newline arrives for SERIAL_IDLE_TIMEOUT_MS.
static String rxToken;
static uint32_t lastRxCharMs = 0;

static void handleToken(const String &t) {
  // Accept both textual commands and numeric IDs.
  // Examples:
  // - "LIGHT_ON", "LIGHT_OFF", "BEEP", "COUNT_RESET"
  // - "6", "5", "1", "3"
  String s = t;
  s.trim();
  if (s.length() == 0) return;

  if (s.equalsIgnoreCase("LIGHT_ON") || s == "6") {
    doOperation(true);
    return;
  }
  if (s.equalsIgnoreCase("LIGHT_OFF") || s == "5") {
    doOperation(false);
    return;
  }
  if (s.equalsIgnoreCase("BEEP") || s == "1") {
    beepOnce();
    return;
  }
  if (s.equalsIgnoreCase("COUNT_RESET") || s == "3") {
    g_count = 0;
    // After reset, show on display (below in loop).
    return;
  }

  // Unknown token: ignore (do not modify behavior).
}

static void pollSerial() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastRxCharMs = millis();

    if (c == '\n' || c == '\r') {
      if (rxToken.length() > 0) {
        handleToken(rxToken);
        rxToken = "";
      }
    } else if (c == '\t') {
      // Treat tabs as part of token; no special handling.
      rxToken += c;
    } else {
      // Accumulate character; tokens may be numeric or textual
      rxToken += c;
    }
  }

  // idle timeout framing: if idle and token present, process it
  if (rxToken.length() > 0 && (millis() - lastRxCharMs) > SERIAL_IDLE_TIMEOUT_MS) {
    handleToken(rxToken);
    rxToken = "";
  }
}

// ---- Display update ----
static void updateDisplay() {
  // Display cumulative operation count as decimal
  // TM1637Display supports showNumberDec
  display.showNumberDec(g_count, true, 4, 0); // right-aligned, leading zeros on? kept true
}

void setup() {
  pinMode(PIN_BTN_ON, INPUT_PULLUP);
  pinMode(PIN_BTN_OFF, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT); // tone() will use it; pin still needs output

  setLight(false);

  display.setBrightness(TM_BRIGHTNESS);
  display.clear();

  // Initialize stable readings
  lastBtnOnReading = digitalRead(PIN_BTN_ON);
  lastBtnOffReading = digitalRead(PIN_BTN_OFF);
  lastBtnOnStable = lastBtnOnReading;
  lastBtnOffStable = lastBtnOffReading;

  lastRxCharMs = millis();

  Serial.begin(SERIAL_BAUD);
}

void loop() {
  // ---- Local buttons (D2 open, D3 close) ----
  uint8_t readingOn = digitalRead(PIN_BTN_ON);
  uint8_t readingOff = digitalRead(PIN_BTN_OFF);

  uint32_t now = millis();

  // D2: active-low press
  if (readingOn != lastBtnOnReading) {
    lastBtnOnChangeMs = now;
    lastBtnOnReading = readingOn;
  }
  if ((now - lastBtnOnChangeMs) > BTN_DEBOUNCE_MS) {
    if (readingOn != lastBtnOnStable) {
      lastBtnOnStable = readingOn;
      // Detect falling edge (HIGH->LOW) means press
      if (lastBtnOnStable == LOW) {
        doOperation(true);
      }
    }
  }

  // D3: active-low press
  if (readingOff != lastBtnOffReading) {
    lastBtnOffChangeMs = now;
    lastBtnOffReading = readingOff;
  }
  if ((now - lastBtnOffChangeMs) > BTN_DEBOUNCE_MS) {
    if (readingOff != lastBtnOffStable) {
      lastBtnOffStable = readingOff;
      if (lastBtnOffStable == LOW) {
        doOperation(false);
      }
    }
  }

  // ---- Serial commands ----
  pollSerial();

  // ---- Display ----
  updateDisplay();
}