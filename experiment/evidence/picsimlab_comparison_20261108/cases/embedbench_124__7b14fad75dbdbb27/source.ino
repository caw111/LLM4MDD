#include <Arduino.h>

static const uint8_t PIN_BTN1 = 3;    // mega:3 -> btn1:1.l
static const uint8_t PIN_BTN2 = 4;    // mega:4 -> btn2:1.l
static const uint8_t PIN_LED  = 2;    // mega:2 -> led1:A (note: LED cathode via GND.1)

static const uint8_t PIN_SR_DS   = A0; // mega:A0 -> sr1:DS
static const uint8_t PIN_SR_STCP = A1; // mega:A1 -> sr1:STCP
static const uint8_t PIN_SR_SHCP = A2; // mega:A2 -> sr1:SHCP

// 7-seg common anode/cathode not explicitly stated; this mapping is for typical
// 74HC595 + 7seg wiring as used by Wokwi sevseg. We'll set bits for digits.
static const uint8_t DIGIT_TO_SEG[10] = {
  // Bits order: Q0..Q6 -> A..G
  // 0 => A,B,C,D,E,F on; G off  (common cathode style in many sims)
  // A B C D E F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),                 // 0
  (0 << 0) | (1 << 1) | (1 << 2),                                                    // 1
  (1 << 0) | (1 << 1) | (0 << 2) | (1 << 3) | (1 << 4) | (1 << 5),              // 2
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (0 << 4) | (1 << 5),              // 3
  (0 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (1 << 5) | (1 << 6),  // 4
  (1 << 0) | (0 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),  // 5
  (1 << 0) | (0 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),  // 6
  (1 << 0) | (1 << 1) | (1 << 2),                                                    // 7
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6), // 8
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (0 << 4) | (1 << 5) | (1 << 6)  // 9
};

// Debounce parameters
static const unsigned long DEBOUNCE_MS = 30;

static uint8_t currentNumber = 0;

static bool btn1Stable = false;
static bool btn2Stable = false;

static bool btn1LastRead = false;
static bool btn2LastRead = false;

static unsigned long btn1LastChangeMs = 0;
static unsigned long btn2LastChangeMs = 0;

static void srWrite7Seg(uint8_t digit)
{
  uint8_t segBits = DIGIT_TO_SEG[digit];

  // Shift out 8 bits to 74HC595, MSB first, mapping to Q0..Q6.
  // We have no use for Q7; set it to 0.
  // For MSB-first shifting: bit7->first. We'll place our Q0 at the last shifted bit.
  // Construct byte with:
  // Q0..Q6 correspond to segBits bit0..bit6.
  // Q7 is 0.
  uint8_t outByte = (uint8_t)(segBits & 0x7F);

  digitalWrite(PIN_SR_STCP, LOW);
  shiftOut(PIN_SR_DS, PIN_SR_SHCP, MSBFIRST, outByte);
  digitalWrite(PIN_SR_STCP, HIGH);
}

static void updateDisplayAndLed()
{
  // Update 7-seg
  srWrite7Seg(currentNumber);

  // LED even => on, odd => off
  // Wiring indicates led1:A is connected to mega:2, and led1:C to GND.
  // So driving pin HIGH turns LED on.
  if ((currentNumber % 2) == 0) digitalWrite(PIN_LED, HIGH);
  else digitalWrite(PIN_LED, LOW);
}

static bool readButton(uint8_t pin)
{
  // Buttons are wired with one side to GND and the other to the MCU pin.
  // Use INPUT_PULLUP so released is HIGH and pressed is LOW.
  return digitalRead(pin) == LOW; // pressed?
}

void setup()
{
  // Shift register pins
  pinMode(PIN_SR_DS, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);
  pinMode(PIN_SR_SHCP, OUTPUT);

  digitalWrite(PIN_SR_DS, LOW);
  digitalWrite(PIN_SR_STCP, LOW);
  digitalWrite(PIN_SR_SHCP, LOW);

  // Buttons
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);

  // LED
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  // Initialize debouncer states
  bool b1 = readButton(PIN_BTN1);
  bool b2 = readButton(PIN_BTN2);

  btn1Stable = b1;
  btn2Stable = b2;
  btn1LastRead = b1;
  btn2LastRead = b2;
  btn1LastChangeMs = millis();
  btn2LastChangeMs = millis();

  currentNumber = 0;
  updateDisplayAndLed();
}

void loop()
{
  unsigned long now = millis();

  // Read raw
  bool b1Read = readButton(PIN_BTN1);
  bool b2Read = readButton(PIN_BTN2);

  // Debounce btn1
  if (b1Read != btn1LastRead) {
    btn1LastRead = b1Read;
    btn1LastChangeMs = now;
  }
  if ((now - btn1LastChangeMs) >= DEBOUNCE_MS) {
    btn1Stable = b1Read;
  }

  // Debounce btn2
  if (b2Read != btn2LastRead) {
    btn2LastRead = b2Read;
    btn2LastChangeMs = now;
  }
  if ((now - btn2LastChangeMs) >= DEBOUNCE_MS) {
    btn2Stable = b2Read;
  }

  // Detect a new press (stable went from not pressed to pressed)
  static bool btn1PrevStable = false;
  static bool btn2PrevStable = false;

  bool btn1PressedEvent = (btn1Stable && !btn1PrevStable);
  bool btn2PressedEvent = (btn2Stable && !btn2PrevStable);

  btn1PrevStable = btn1Stable;
  btn2PrevStable = btn2Stable;

  if (btn1PressedEvent) {
    currentNumber++;
    if (currentNumber > 9) currentNumber = 0;
    updateDisplayAndLed();
  } else if (btn2PressedEvent) {
    if (currentNumber == 0) currentNumber = 9;
    else currentNumber--;
    updateDisplayAndLed();
  }

  // No delay; debounce is time-based
}