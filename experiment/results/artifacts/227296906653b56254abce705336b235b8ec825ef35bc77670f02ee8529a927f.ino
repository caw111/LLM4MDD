#include <Arduino.h>

static const uint8_t PIN_BTN1 = 2;
static const uint8_t PIN_BTN2 = 3;
static const uint8_t PIN_BTN3 = 4;

// 74HC595 #1 (sevseg1)
static const uint8_t PIN_SR1_SHCP = A2; // gray
static const uint8_t PIN_SR1_STCP = A1; // purple
static const uint8_t PIN_SR1_DS   = A0; // blue

// 74HC595 #2 (sevseg2) shares same clock/data/latch as per wiring
// SR1/ SR2 OE tied to GND (always enabled). MR tied to VCC (no async clear)

// Bargraph pins (assumed: A1..A10 -> Mega 5..44)
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

static const uint8_t BAR_OFF_CATHODE_PIN = 100; // not used; C1..C10 tied to GND.1

// Debounce
static const uint32_t DEBOUNCE_MS = 35;

// Display update timing requirement
static const uint32_t MAX_UPDATE_AFTER_PRESS_MS = 150;

static uint8_t numberTens(uint8_t n) { return (n / 10) % 10; }
static uint8_t numberOnes(uint8_t n) { return n % 10; }

// Segment encoding for 7-seg common-cathode style (segments ON=1).
// Wiring suggests SR outputs drive segments directly; with default parts in Wokwi this is typically common-cathode.
// We'll use standard mapping: bits [A..G] => Q0..Q6, no DP.
static uint8_t digitToSegments(uint8_t d) {
  // A B C D E F G
  // 0: 1 1 1 1 1 1 0
  // 1: 0 1 1 0 0 0 0
  // 2: 1 1 0 1 1 0 1
  // 3: 1 1 1 1 0 0 1
  // 4: 0 1 1 0 0 1 1
  // 5: 1 0 1 1 0 1 1
  // 6: 1 0 1 1 1 1 1
  // 7: 1 1 1 0 0 0 0
  // 8: 1 1 1 1 1 1 1
  // 9: 1 1 1 1 0 1 1
  switch (d) {
    case 0: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5);                 // A B C D E F, G=0
    case 1: return (1<<1)|(1<<2);                                             // B C
    case 2: return (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6);                        // A B D E G
    case 3: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6);                        // A B C D G
    case 4: return (1<<1)|(1<<2)|(1<<5)|(1<<6);                               // B C F G
    case 5: return (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6);                        // A C D F G
    case 6: return (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6);                 // A C D E F G
    case 7: return (1<<0)|(1<<1)|(1<<2);                                      // A B C
    case 8: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6);          // A B C D E F G
    case 9: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6);                  // A B C D F G
    default: return 0;
  }
}

static void sr1_send7seg(uint8_t segBitsAtoG) {
  // 74HC595 #1 outputs:
  // Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G, Q7 -> SR2 DS (shared chain)
  // We'll clock out 8 bits where Q7 is don't-care; set to 0.
  // Bit order in shift register: DS (SER) is sampled on each SHCP rising edge with the first shifted bit ending up at Q7
  // But in typical Arduino shiftOut( MSBFIRST ) we'd want consistent mapping.
  //
  // We'll avoid assumptions by using shiftOut which handles bit order.
  // Data to SR1 is simultaneously shifted into SR2 at Q7S -> sr2 DS.
  // Therefore for both digits update, we must shift a single 16-bit stream.
}

static void sr_update_two_digits(uint8_t tens, uint8_t ones) {
  // Need to drive both SRs in a single chained shift:
  // - SR1 (sevseg1) uses Q0..Q6 => A..G.
  // - SR2 (sevseg2) uses Q0..Q6 => A..G.
  //
  // Wiring: sr1 Q7S -> sr2 DS. sr1 SHCP and STCP shared.
  // That means when shifting N bits:
  // - The last 8 bits shifted will end up in SR1, the previous 8 bits in SR2 (for a daisy chain with Q7S feeding DS).
  //
  // However, which is "first" depends on bit order used.
  // We'll implement explicitly with shiftOut using MSB-first over 16 bits, placing SR2 then SR1 in the chain
  // assuming: first shifted bits go to the farthest register (SR2) when using MSBFIRST on cascaded 595s.
  //
  // This is the standard behavior used by many chained 74HC595 examples.

  uint8_t segT = digitToSegments(tens);
  uint8_t segO = digitToSegments(ones);

  // Construct byte payloads with bit0=A ... bit6=G, bit7=dp(ignored=0).
  uint8_t byte_sr1 = segT & 0x7F; // sevseg1 for tens
  uint8_t byte_sr2 = segO & 0x7F; // sevseg2 for ones

  // We will clock out 16 bits: [byte_sr2][byte_sr1] with MSB-first.
  // Provide DS bit as MSB of each byte first.
  uint16_t word = ((uint16_t)byte_sr2 << 8) | byte_sr1;

  // Manual shift-out to control bit order (MSB first)
  // DS pin is sr1 DS (A0)
  pinMode(PIN_SR1_DS, OUTPUT);
  pinMode(PIN_SR1_SHCP, OUTPUT);
  pinMode(PIN_SR1_STCP, OUTPUT);

  noInterrupts();
  digitalWrite(PIN_SR1_STCP, LOW);

  for (int i = 15; i >= 0; --i) {
    digitalWrite(PIN_SR1_SHCP, LOW);
    uint8_t bitVal = (word >> i) & 0x01;
    digitalWrite(PIN_SR1_DS, bitVal ? HIGH : LOW);
    digitalWrite(PIN_SR1_SHCP, HIGH);
  }

  digitalWrite(PIN_SR1_STCP, HIGH);
  digitalWrite(PIN_SR1_STCP, LOW);
  interrupts();
}

static void bargraph_setTens(uint8_t tens) {
  uint8_t pins[10] = {
    PIN_BAR_A1, PIN_BAR_A2, PIN_BAR_A3, PIN_BAR_A4, PIN_BAR_A5,
    PIN_BAR_A6, PIN_BAR_A7, PIN_BAR_A8, PIN_BAR_A9, PIN_BAR_A10
  };

  // Light up LEDs equal to tens digit.
  // Tens range 0..9. Turn on first tens LEDs.
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(pins[i], (i < tens) ? HIGH : LOW);
  }
}

struct Button {
  uint8_t pin;
  bool stableState;     // last debounced state
  bool lastReading;     // immediate last read
  uint32_t lastChangeMs;
  bool eventReady;     // one-shot edge event
};

static Button btn1 { PIN_BTN1, false, false, 0, false };
static Button btn2 { PIN_BTN2, false, false, 0, false };
static Button btn3 { PIN_BTN3, false, false, 0, false };

static void initButton(Button &b) {
  pinMode(b.pin, INPUT_PULLUP); // buttons wired to GND when pressed (as per typical)
  bool r = (digitalRead(b.pin) == LOW); // pressed -> LOW
  b.stableState = r;
  b.lastReading = r;
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
      // Create a one-shot event on press
      if (b.stableState) b.eventReady = true;
    }
  }
}

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
  uint8_t barPins[10] = {
    PIN_BAR_A1, PIN_BAR_A2, PIN_BAR_A3, PIN_BAR_A4, PIN_BAR_A5,
    PIN_BAR_A6, PIN_BAR_A7, PIN_BAR_A8, PIN_BAR_A9, PIN_BAR_A10
  };
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(barPins[i], OUTPUT);
    digitalWrite(barPins[i], LOW);
  }

  initButton(btn1);
  initButton(btn2);
  initButton(btn3);

  // Initialization: display "00" and bargraph all off
  currentNumber = 0;
  sr_update_two_digits(0, 0);
  bargraph_setTens(0);
}

void loop() {
  // Poll buttons frequently to ensure updates within 0.15s after a valid press.
  pollButton(btn1);
  pollButton(btn2);
  pollButton(btn3);

  bool didChange = false;

  // Priority: btn3 resets immediately to 0 (even if other presses happen).
  if (btn3.eventReady) {
    btn3.eventReady = false;
    currentNumber = 0;
    didChange = true;
  }

  if (btn1.eventReady) {
    btn1.eventReady = false;
    currentNumber = (currentNumber + 1);
    if (currentNumber > 99) currentNumber = 0;
    didChange = true;
  }

  if (btn2.eventReady) {
    btn2.eventReady = false;
    currentNumber = (currentNumber + 5);
    if (currentNumber > 99) currentNumber = 0;
    didChange = true;
  }

  if (didChange) {
    uint8_t tens = numberTens(currentNumber);
    uint8_t ones = numberOnes(currentNumber);

    // Update display and bargraph immediately on valid button press.
    // Ensures requirement: within 0.15 seconds after valid button press.
    sr_update_two_digits(tens, ones);
    bargraph_setTens(tens);
  }

  // Small yield; keep responsive.
  delay(1);
}