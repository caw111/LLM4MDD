#include <Arduino.h>
#include <avr/interrupt.h>

#if !defined(__AVR_ATmega2560__)
#error "This sketch requires an Arduino Mega 2560."
#endif

static_assert(F_CPU == 16000000UL, "This timing configuration requires 16 MHz.");

// Preserved wiring:
// A0 -> 74HC595 DS, A1 -> STCP, A2 -> SHCP.
// Q0..Q6 -> display A..G; COM -> +5 V (common anode).
// D2/D3 -> led1/led2; D4..D13 -> bar graph A1..A10.
constexpr uint8_t DATA_PIN  = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;

constexpr uint8_t DATA_MASK  = _BV(PF0);
constexpr uint8_t LATCH_MASK = _BV(PF1);
constexpr uint8_t CLOCK_MASK = _BV(PF2);

constexpr uint8_t PORT_E_MASK = _BV(PE3) | _BV(PE4) | _BV(PE5);
constexpr uint8_t PORT_G_MASK = _BV(PG5);
constexpr uint8_t PORT_H_MASK =
    _BV(PH3) | _BV(PH4) | _BV(PH5) | _BV(PH6);
constexpr uint8_t PORT_B_MASK =
    _BV(PB4) | _BV(PB5) | _BV(PB6) | _BV(PB7);

// Active-low patterns for the common-anode display.
// Bit 7 is unused and remains HIGH.
const uint8_t DIGIT_PATTERNS[10] = {
  0xC0, 0xF9, 0xA4, 0xB0, 0x99,
  0x92, 0x82, 0xF8, 0x80, 0x90
};

static uint8_t pendingDigit = 1;
static uint8_t pendingE = 0;
static uint8_t pendingG = 0;
static uint8_t pendingH = 0;
static uint8_t pendingB = 0;

// Shift without latching: the currently displayed digit stays unchanged.
static void stageDisplay(uint8_t pattern) {
  PORTF &= static_cast<uint8_t>(~(LATCH_MASK | CLOCK_MASK));

  for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
    if (pattern & mask) {
      PORTF |= DATA_MASK;
    } else {
      PORTF &= static_cast<uint8_t>(~DATA_MASK);
    }

    PORTF |= CLOCK_MASK;
    PORTF &= static_cast<uint8_t>(~CLOCK_MASK);
  }
}

static inline void latchDisplay() {
  PORTF |= LATCH_MASK;
  PORTF &= static_cast<uint8_t>(~LATCH_MASK);
}

// Prepare the next complete state without changing visible outputs.
static void prepareNextState(uint8_t digit) {
  const uint16_t bars = (uint16_t(1) << digit) - 1U;

  // D2 = PE4 (even LED), D3 = PE5 (odd LED).
  pendingE = (digit & 1U) ? _BV(PE5) : _BV(PE4);

  // Bar segments 1..10 correspond to D4..D13.
  pendingG = (bars & 0x001U) ? _BV(PG5) : 0;  // D4
  if (bars & 0x002U) {
    pendingE |= _BV(PE3);                    // D5
  }

  pendingH = static_cast<uint8_t>((bars & 0x03CU) << 1); // D6..D9
  pendingB = static_cast<uint8_t>(bars & 0x3C0U) >> 2;   // D10..D13

  stageDisplay(DIGIT_PATTERNS[digit]);
}

ISR(TIMER1_COMPA_vect) {
  // Commit the precomputed state in one uninterrupted update.
  // Separate hardware ports cannot switch on precisely the same CPU edge;
  // these consecutive writes keep all visible changes within microseconds.
  PORTE = (PORTE & static_cast<uint8_t>(~PORT_E_MASK)) | pendingE;
  PORTG = (PORTG & static_cast<uint8_t>(~PORT_G_MASK)) | pendingG;
  PORTH = (PORTH & static_cast<uint8_t>(~PORT_H_MASK)) | pendingH;
  PORTB = (PORTB & static_cast<uint8_t>(~PORT_B_MASK)) | pendingB;
  latchDisplay();

  pendingDigit = (pendingDigit == 9) ? 0 : pendingDigit + 1;
  prepareNextState(pendingDigit);
}

void setup() {
  const uint8_t savedSREG = SREG;
  cli();

  // Stop Timer1 while initializing.
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  TCNT1 = 0;

  // Initially both parity LEDs and every bar segment are off.
  for (uint8_t pin = 2; pin <= 13; ++pin) {
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
  }

  digitalWrite(DATA_PIN, HIGH);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  // Initial state: display 0, bar off, both LEDs off.
  stageDisplay(DIGIT_PATTERNS[0]);
  latchDisplay();

  // Prepare 1, but leave the initial state visible for the first 2 seconds.
  pendingDigit = 1;
  prepareNextState(pendingDigit);

  // 16 MHz / 1024 = 15625 ticks/s; 31250 ticks = exactly 2 seconds.
  // Hardware CTC timing avoids accumulated software scheduling drift.
  OCR1A = 31249;
  TCNT1 = 0;
  TIFR1 = _BV(OCF1A) | _BV(OCF1B) | _BV(OCF1C) | _BV(TOV1);
  TIMSK1 = _BV(OCIE1A);
  TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);

  SREG = savedSREG;
}

void loop() {
  // Timer1 performs every synchronized update without blocking delays.
}