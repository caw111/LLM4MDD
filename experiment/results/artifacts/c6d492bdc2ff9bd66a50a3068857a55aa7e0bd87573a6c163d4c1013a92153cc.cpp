#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_88\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
#include <Arduino.h>

// === Arduino Mega 2560 (as specified in the circuit) ===

// 74HC595 control pins (wiring preserved)
static const uint8_t SR1_SHCP = A2; // gray  -> SHCP
static const uint8_t SR1_STCP = A1; // purple-> STCP
static const uint8_t SR1_DS   = A0; // blue  -> DS

// RGB LED pins (common cathode; COM tied to GND in circuit)
static const uint8_t RGB_R = 4;  // red
static const uint8_t RGB_G = 5;  // green
static const uint8_t RGB_B = 6;  // blue

// Buttons (wired to GND on one side; other side to MCU pins -> INPUT_PULLUP, active LOW)
static const uint8_t BTN1 = 2; // increments
static const uint8_t BTN2 = 3; // decrements

// Segment mapping: bits 0..6 = A..G (active HIGH)
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_88\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static uint8_t segAtoGForDigit(uint8_t d);
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_88\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void srSendDigits(uint8_t tensDigit, uint8_t unitsDigit);
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_88\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void setRGBForNumber(int n);
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_88\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void turnRGBOff();
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_88\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void initButton(DebouncedButton &btn, uint8_t p);
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_88\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static bool pollButtonPressed(DebouncedButton &btn);
#line 125 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_88\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
void setup();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_88\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_88\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static uint8_t segAtoGForDigit(uint8_t d) {
  static const uint8_t map[10] = {
    // A B C D E F G
    (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),                 // 0
    (1 << 1) | (1 << 2),                                                                 // 1
    (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),                               // 2
    (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),                               // 3
    (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),                                         // 4
    (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),                                // 5
    (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),                   // 6
    (1 << 0) | (1 << 1) | (1 << 2),                                                      // 7
    (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),    // 8
    (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)                    // 9
  };
  return map[d % 10];
}

// Shift register chain: sr1 -> sr2 using sr1:Q7S to sr2:DS
static void srSendDigits(uint8_t tensDigit, uint8_t unitsDigit) {
  uint8_t sr1Byte = segAtoGForDigit(tensDigit);  // sevseg1
  uint8_t sr2Byte = segAtoGForDigit(unitsDigit); // sevseg2

  // Two bytes shifted through the daisy chain in order:
  // First SR1 byte, then SR2 byte (with SR2 wired to SR1 Q7S).
  uint8_t bytes[2] = { sr1Byte, sr2Byte };

  digitalWrite(SR1_STCP, LOW);
  for (uint8_t b = 0; b < 2; b++) {
    for (int bit = 7; bit >= 0; bit--) {
      digitalWrite(SR1_SHCP, LOW);
      digitalWrite(SR1_DS, ((bytes[b] >> bit) & 0x01) ? HIGH : LOW);
      digitalWrite(SR1_SHCP, HIGH);
    }
  }
  digitalWrite(SR1_STCP, HIGH);
}

static void setRGBForNumber(int n) {
  // COM is tied to GND (common cathode). Turning a channel HIGH turns it ON.
  if (n >= 0 && n <= 33) {          // red
    digitalWrite(RGB_R, HIGH);
    digitalWrite(RGB_G, LOW);
    digitalWrite(RGB_B, LOW);
  } else if (n >= 34 && n <= 66) { // green
    digitalWrite(RGB_R, LOW);
    digitalWrite(RGB_G, HIGH);
    digitalWrite(RGB_B, LOW);
  } else {                          // 67..99 blue
    digitalWrite(RGB_R, LOW);
    digitalWrite(RGB_G, LOW);
    digitalWrite(RGB_B, HIGH);
  }
}

static void turnRGBOff() {
  digitalWrite(RGB_R, LOW);
  digitalWrite(RGB_G, LOW);
  digitalWrite(RGB_B, LOW);
}

// Debounced button
struct DebouncedButton {
  uint8_t pin = 0;
  bool lastReading = false;   // last raw read state
  bool stableState = false;   // stable raw state
  unsigned long lastChangeMs = 0;
};

static void initButton(DebouncedButton &btn, uint8_t p) {
  btn.pin = p;
  pinMode(btn.pin, INPUT_PULLUP); // active LOW

  bool reading = (digitalRead(btn.pin) == HIGH);
  btn.lastReading = reading;
  btn.stableState = reading;
  btn.lastChangeMs = millis();
}

// Returns true once per press when stable transitions to pressed (active LOW => LOW)
static bool pollButtonPressed(DebouncedButton &btn) {
  const unsigned long debounceMs = 30;

  bool reading = (digitalRead(btn.pin) == HIGH);

  if (reading != btn.lastReading) {
    btn.lastReading = reading;
    btn.lastChangeMs = millis();
  }

  unsigned long now = millis();
  if ((now - btn.lastChangeMs) >= debounceMs) {
    if (btn.stableState != btn.lastReading) {
      btn.stableState = btn.lastReading;
      if (!btn.stableState) { // became LOW => pressed
        return true;
      }
    }
  }
  return false;
}

static int displayedNumber = 0;
static DebouncedButton buttonInc;
static DebouncedButton buttonDec;

void setup() {
  // Shift register pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // RGB pins (off at start)
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  turnRGBOff();

  // Buttons
  initButton(buttonInc, BTN1);
  initButton(buttonDec, BTN2);

  // Initial display "00" and RGB off
  displayedNumber = 0;
  srSendDigits(0, 0);
  turnRGBOff();
}

void loop() {
  bool incPressed = pollButtonPressed(buttonInc);
  bool decPressed = pollButtonPressed(buttonDec);

  if (incPressed) {
    displayedNumber++;
    if (displayedNumber > 99) displayedNumber = 0;

    uint8_t tens = (uint8_t)(displayedNumber / 10);
    uint8_t units = (uint8_t)(displayedNumber % 10);
    srSendDigits(tens, units);
    setRGBForNumber(displayedNumber);
  }

  if (decPressed) {
    displayedNumber--;
    if (displayedNumber < 0) displayedNumber = 99;

    uint8_t tens = (uint8_t)(displayedNumber / 10);
    uint8_t units = (uint8_t)(displayedNumber % 10);
    srSendDigits(tens, units);
    setRGBForNumber(displayedNumber);
  }

  delay(5);
}
