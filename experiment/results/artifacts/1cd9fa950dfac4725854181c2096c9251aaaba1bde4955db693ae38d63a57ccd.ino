#include <Arduino.h>

// Buttons (wiring: each button pulls signal pin to GND when pressed)
static const uint8_t PIN_BTN1 = 2;
static const uint8_t PIN_BTN2 = 3;
static const uint8_t PIN_BTN3 = 4;

// 74HC595 chaining control lines for both shift registers (sevseg1 + sevseg2)
static const uint8_t PIN_SR1_SHCP = A2; // gray
static const uint8_t PIN_SR1_STCP = A1; // purple (latch)
static const uint8_t PIN_SR1_DS   = A0; // blue (data)

// Bargraph pins (bargraph1 A1..A10 mapped to Mega 5..13 and 44)
static const uint8_t PIN_BAR_A1  = 5;
static const uint8_t PIN_BAR_A2  = 6;
static const uint8_t PIN_BAR_A3  = 7;
static const uint8_t PIN_BAR_A4  = 8;
static const uint8_t PIN_BAR_A5  = 9;
static const uint8_t PIN_BAR_A6  = 10;
static const uint8_t PIN_BAR_A7  = 11;
static const uint8_t PIN_BAR_A8  = 12;
static const uint8_t PIN_BAR_A9  = 13;
static const uint8_t PIN_BAR_A10 = 44;

// Debounce and display timing
static const uint32_t DEBOUNCE_MS = 35;
static const uint32_t MAX_UPDATE_AFTER_PRESS_MS = 150; // requirement target

// Helpers: split number into digits
static uint8_t numberTens(uint8_t n) { return (n / 10) % 10; }
static uint8_t numberOnes(uint8_t n) { return n % 10; }

// Segment encoding for 7-seg (common-cathode assumed: ON=1), bits A..G in bit0..6
static uint8_t digitToSegments(uint8_t d) {
  switch (d) {
    case 0: return (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);                // A B C D E F
    case 1: return (1 << 1) | (1 << 2);                                                                    // B C
    case 2: return (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6);                                // A B D E G
    case 3: return (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6);                                // A B C D G
    case 4: return (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6);                                            // B C F G
    case 5: return (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6);                                // A C D F G
    case 6: return (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);                    // A C D E F G
    case 7: return (1 << 0) | (1 << 1) | (1 << 2);                                                        // A B C
    case 8: return (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);       // A B C D E F G
    case 9: return (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6);                    // A B C D F G
    default: return 0;
  }
}

// Update both chained shift registers to show two digits simultaneously.
// Chaining wiring: sr1 Q7S -> sr2 DS, SHCP/STCP shared.
static void sr_update_two_digits(uint8_t tens, uint8_t ones) {
  uint8_t segT = digitToSegments(tens) & 0x7F; // sevseg1 bits A..G map to Q0..Q6
  uint8_t segO = digitToSegments(ones) & 0x7F; // sevseg2 bits A..G map to Q0..Q6

  // Shift MSB-first 16 bits: [byte_sr2][byte_sr1]
  uint16_t word = ((uint16_t)segO << 8) | segT;

  // Stable signals before shifting
  digitalWrite(PIN_SR1_STCP, LOW);
  digitalWrite(PIN_SR1_DS, LOW);

  noInterrupts();
  for (int i = 15; i >= 0; --i) {
    digitalWrite(PIN_SR1_SHCP, LOW);
    digitalWrite(PIN_SR1_DS, ((word >> i) & 0x01) ? HIGH : LOW);
    digitalWrite(PIN_SR1_SHCP, HIGH);
  }
  digitalWrite(PIN_SR1_STCP, HIGH);
  interrupts();
}

// Bargraph: light up LEDs equal to the tens digit (35 -> 3 LEDs lit).
static void bargraph_setTens(uint8_t tens) {
  if (tens > 9) tens = 9;

  // bargraph1 A1..A10 are driven by Arduino pins (C pins tied to GND in circuit)
  const uint8_t pins[10] = {
    PIN_BAR_A1, PIN_BAR_A2, PIN_BAR_A3, PIN_BAR_A4, PIN_BAR_A5,
    PIN_BAR_A6, PIN_BAR_A7, PIN_BAR_A8, PIN_BAR_A9, PIN_BAR_A10
  };

  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(pins[i], (i < tens) ? HIGH : LOW);
  }
}

// Debounced button handling
struct Button {
  uint8_t pin;
  bool stableState;     // last debounced state (true=pressed)
  bool lastReading;     // most recent raw read (true=pressed)
  uint32_t lastChangeMs;
  bool eventReady;     // one-shot on press (stableState rising)
};

static void initButton(Button &b) {
  pinMode(b.pin, INPUT_PULLUP); // pressed => LOW due to wiring
  bool pressed = (digitalRead(b.pin) == LOW); // true=pressed
  b.stableState = pressed;
  b.lastReading = pressed;
  b.lastChangeMs = millis();
  b.eventReady = false;
}

static void pollButton(Button &b) {
  bool pressed = (digitalRead(b.pin) == LOW);

  if (pressed != b.lastReading) {
    b.lastReading = pressed;
    b.lastChangeMs = millis();
  }

  if ((millis() - b.lastChangeMs) >= DEBOUNCE_MS) {
    if (pressed != b.stableState) {
      b.stableState = pressed;
      if (b.stableState) b.eventReady = true; // rising edge (press) only
    }
  }
}

static Button btn1 = { PIN_BTN1, false, false, 0, false };
static Button btn2 = { PIN_BTN2, false, false, 0, false };
static Button btn3 = { PIN_BTN3, false, false, 0, false };

static uint8_t currentNumber = 0;

void setup() {
  // Shift register control pins
  pinMode(PIN_SR1_DS, OUTPUT);
  pinMode(PIN_SR1_SHCP, OUTPUT);
  pinMode(PIN_SR1_STCP, OUTPUT);

  digitalWrite(PIN_SR1_DS, LOW);
  digitalWrite(PIN_SR1_SHCP, LOW);
  digitalWrite(PIN_SR1_STCP, LOW);

  // Bargraph pins (A1..A10)
  const uint8_t barPins[10] = {
    PIN_BAR_A1, PIN_BAR_A2, PIN_BAR_A3, PIN_BAR_A4, PIN_BAR_A5,
    PIN_BAR_A6, PIN_BAR_A7, PIN_BAR_A8, PIN_BAR_A9, PIN_BAR_A10
  };
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(barPins[i], OUTPUT);
    digitalWrite(barPins[i], LOW); // all off
  }

  // Buttons
  initButton(btn1);
  initButton(btn2);
  initButton(btn3);

  // Initialization: displays "00"; bargraph all off
  currentNumber = 0;
  sr_update_two_digits(0, 0);
  bargraph_setTens(0);
}

void loop() {
  // Keep loop fast to satisfy "update within 0.15 s after valid button press"
  pollButton(btn1);
  pollButton(btn2);
  pollButton(btn3);

  bool didChange = false;

  // btn3: Reset to 0 immediately (after debounced press is detected)
  if (btn3.eventReady) {
    btn3.eventReady = false;
    currentNumber = 0;
    didChange = true;
  }

  // btn1: Increment by 1; wrap at 99 -> 0
  if (btn1.eventReady) {
    btn1.eventReady = false;
    uint16_t tmp = (uint16_t)currentNumber + 1;
    currentNumber = (tmp > 99) ? 0 : (uint8_t)tmp;
    didChange = true;
  }

  // btn2: Increment by 5; wrap at 99 -> 0
  if (btn2.eventReady) {
    btn2.eventReady = false;
    uint16_t tmp = (uint16_t)currentNumber + 5;
    currentNumber = (tmp > 99) ? 0 : (uint8_t)tmp;
    didChange = true;
  }

  if (didChange) {
    uint8_t tens = numberTens(currentNumber);
    uint8_t ones = numberOnes(currentNumber);
    sr_update_two_digits(tens, ones);
    bargraph_setTens(tens);
  }

  delay(1);
}