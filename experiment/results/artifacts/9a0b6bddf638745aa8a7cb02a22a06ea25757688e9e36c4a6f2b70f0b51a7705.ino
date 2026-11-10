#include <Arduino.h>

const uint8_t BTN_PIN = 2; // btn1

// 74HC595 (sr1) connections from the circuit
const uint8_t SR_DS   = A0; // mega:A0 -> sr1:DS (data)
const uint8_t SR_STCP = A1; // mega:A1 -> sr1:STCP (latch)
const uint8_t SR_SHCP = A2; // mega:A2 -> sr1:SHCP (clock)

// Mapping from number 0..9 to 74HC595 output states for sevseg1 + bargraph1.
// 74HC595 outputs Q0..Q6 go to 7-segment A..G respectively.
// 74HC595 outputs Q7..Q15 go to bargraph segments A1..A10 and C1..C10 respectively,
// but in this circuit only A1..A10 are driven (Q0..Q15 are available on 595).
// However, Wokwi's "74hc595" part is 8-bit; outputs are Q0..Q7.
// The circuit wiring shows Q0..Q6 used for segments A..G, and also Q1..Q6 for some
// bargraph lines which indicates the same 8-bit device is being reused for both.
// Because the provided wiring uses sr1:Q0..Q6 only, we must drive only those pins.
// Therefore, we implement bargraph based on Q0..Q6 as the circuit implies.
// LEDs bargraph1:A1..A10 connect to mega:3..12 but those are independent of SR; in
// Wokwi netlist those would normally be driven separately. Since the circuit only
// provides SR connections for bargraph, the correct behavior in Wokwi is to use SR outputs.
//
// Practically: follow the given wiring intent: use the 74HC595 to output a "digit"
 // value that simultaneously lights sevseg and the bargraph corresponding LED number.
// We'll use a standard 7-seg decoder for A..G on Q0..Q6, and a standard bargraph
// of 10 LEDs by lighting first N segments on the same outputs if available.
// With only Q0..Q6 available, we can only represent 0..6 directly. To preserve
// the required 0..9 behavior, we additionally drive the bargraph "A1..A10" pins directly
// from Arduino Mega pins 3..12 as specified in the circuit wiring.
//
// From the provided connections:
// mega:3 -> bargraph1:A1, mega:4 -> A2, ..., mega:12 -> A10
// and all mega:GND.1 -> bargraph1:C1..C10, meaning bargraph is a set of anodes to pins,
// with cathodes grounded. Thus: set mega pins 3..12 HIGH for anodes 1..N, LOW otherwise.

const uint8_t BAR_A_PINS[10] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; // A1..A10

// 7-segment segment patterns for common cathode assumed by Wokwi "74hc595" wiring.
// We'll drive Q0..Q6 as A..G, where HIGH means segment ON.
// Standard digits (A..G):
// 0: A,B,C,D,E,F
// 1: B,C
// 2: A,B,D,E,G
// 3: A,B,C,D,G
// 4: B,C,F,G
// 5: A,C,D,F,G
// 6: A,C,D,E,F,G
// 7: A,B,C
// 8: A,B,C,D,E,F,G
// 9: A,B,C,D,F,G
static const uint8_t segOn[10] = {
  // Bits: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G (LSB=bit0=Q0)
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),                 // 0
  (1<<1)|(1<<2),                                             // 1
  (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),                       // 2
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),                       // 3
  (1<<1)|(1<<2)|(1<<5)|(1<<6),                               // 4
  (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),                       // 5
  (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),                // 6
  (1<<0)|(1<<1)|(1<<2),                                       // 7
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),        // 8
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)                 // 9
};

static uint8_t currentNumber = 0;

static void srWrite8(uint8_t value) {
  // SR_DS -> bit0=Q0, bit1=Q1, ... bit7=Q7
  // Pulse SHCP 8 times, latch with STCP.
  digitalWrite(SR_STCP, LOW);
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_SHCP, LOW);
    digitalWrite(SR_DS, (value >> i) & 0x01);
    digitalWrite(SR_SHCP, HIGH);
  }
  digitalWrite(SR_STCP, HIGH);
  delayMicroseconds(5);
  digitalWrite(SR_STCP, LOW);
}

static void updateDisplay(uint8_t n) {
  // Update 7-seg on Q0..Q6; Q7 unused.
  uint8_t sev = 0;
  if (n <= 9) sev = segOn[n];
  uint8_t out = sev & 0x7F; // only Q0..Q6 meaningful
  srWrite8(out);

  // Update bargraph: light up LEDs 1..n using anode pins A1..A10
  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_A_PINS[i], (i < n) ? HIGH : LOW);
  }
}

void setup() {
  // Button
  pinMode(BTN_PIN, INPUT_PULLUP); // btn1: push button between mega:2 and mega:2? Circuit shows 2->r and 2 -> l; use pull-up.

  // 74HC595 pins
  pinMode(SR_DS, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);

  // Ensure SR control lines low initially
  digitalWrite(SR_DS, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_SHCP, LOW);

  // Bargraph anodes
  for (uint8_t i = 0; i < 10; ++i) {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW); // no LEDs lit initially
  }

  currentNumber = 0;
  updateDisplay(currentNumber);
}

void loop() {
  static uint8_t lastBtnState = HIGH;
  static uint32_t lastDebounceTime = 0;
  const uint32_t debounceMs = 30;

  uint8_t reading = digitalRead(BTN_PIN);

  if (reading != lastBtnState) {
    lastDebounceTime = millis();
    lastBtnState = reading;
  }

  static uint8_t stableState = HIGH;
  if ((millis() - lastDebounceTime) >= debounceMs && reading != stableState) {
    stableState = reading;

    // Detect press: with INPUT_PULLUP, press brings pin LOW
    if (stableState == LOW) {
      uint8_t nextVal = currentNumber + 1;
      if (nextVal > 9) nextVal = 0;
      currentNumber = nextVal;
      updateDisplay(currentNumber);
    }
  }
}