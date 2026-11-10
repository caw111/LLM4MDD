#include <Arduino.h>

#define LED1_PIN 2     // from circuit: led1:A -> mega:2

#define BTN1_PIN 3     // mega:3 -> btn1:1.l
#define BTN2_PIN 4     // mega:4 -> btn2:1.l

// Shift register pins (from circuit)
#define SR_DS   A0     // mega:A0 -> sr1:DS
#define SR_STCP A1     // mega:A1 -> sr1:STCP
#define SR_SHCP A2     // mega:A2 -> sr1:SHCP
// sr1:OE is tied to GND, sr1:MR tied to VCC, so output always enabled and no reset required.

static uint8_t currentDigit = 0;

static bool lastBtn1Stable = false;
static bool lastBtn2Stable = false;

static bool lastBtn1Raw = false;
static bool lastBtn2Raw = false;

static unsigned long btn1LastChangeMs = 0;
static unsigned long btn2LastChangeMs = 0;

static const unsigned long DEBOUNCE_MS = 25;

static void updateDisplayAndLed(uint8_t digit) {
  // Sevseg1 wiring expects segment outputs:
  // Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
  //
  // Common mapping for a typical 7-seg (active HIGH for segments).
  // If your hardware uses inverted logic, you'd need to invert bits,
  // but we preserve behavior based on given wiring.
  static const uint8_t digitToSegments[10] = {
    // A  B  C  D  E  F  G
    0b0111111, // 0
    0b0000110, // 1
    0b1011011, // 2
    0b1001111, // 3
    0b1100110, // 4
    0b1101101, // 5
    0b1111101, // 6
    0b0000111, // 7
    0b1111111, // 8
    0b1101111  // 9
  };

  uint8_t seg = digitToSegments[digit];

  // Build shift register byte where:
  // Q0->A (bit0), Q1->B (bit1), ... Q6->G (bit6)
  // Q7 unused (0)
  uint8_t srValue = seg & 0x7F;

  // Shift out MSB->LSB in the required order for 74HC595:
  // With Q0 mapped to bit0, we send bit7..bit0 so that the last shifted bit
  // ends up on Q0. We'll explicitly shift 8 bits.
  digitalWrite(SR_STCP, LOW);
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_SHCP, LOW);
    digitalWrite(SR_DS, (srValue >> i) & 0x01);
    digitalWrite(SR_SHCP, HIGH);
  }
  digitalWrite(SR_STCP, HIGH);

  // LED indicates parity: even -> on, odd -> off
  digitalWrite(LED1_PIN, (digit % 2 == 0) ? HIGH : LOW);
}

static bool readButtonRaw(uint8_t pin) {
  // Buttons wired to GND and to input pins, so press connects pin to GND.
  // Use INPUT_PULLUP so unpressed reads HIGH, pressed reads LOW.
  return digitalRead(pin) == HIGH; // return "not pressed" as true; will treat changes robustly below
}

static bool updateDebouncedEdge(uint8_t pin, bool &lastStableState, bool &lastRawState,
                                 unsigned long &lastChangeMs) {
  bool rawPressed = (digitalRead(pin) == LOW); // pressed when LOW

  if (rawPressed != lastRawState) {
    lastRawState = rawPressed;
    lastChangeMs = millis();
  }

  if ((millis() - lastChangeMs) >= DEBOUNCE_MS) {
    if (lastStableState != rawPressed) {
      lastStableState = rawPressed;
      // Return true only on transition to pressed
      return (rawPressed == true);
    }
  }
  return false;
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW); // LED off initially

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  pinMode(SR_DS, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);

  digitalWrite(SR_DS, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_SHCP, LOW);

  // Initialize to "0"
  currentDigit = 0;
  updateDisplayAndLed(currentDigit);

  // Initialize debouncing states
  lastBtn1Raw = (digitalRead(BTN1_PIN) == LOW);
  lastBtn2Raw = (digitalRead(BTN2_PIN) == LOW);
  lastBtn1Stable = lastBtn1Raw;
  lastBtn2Stable = lastBtn2Raw;

  btn1LastChangeMs = millis();
  btn2LastChangeMs = millis();
}

void loop() {
  // Button presses:
  // btn1 increments, wraps 9->0
  // btn2 decrements, wraps 0->9

  bool btn1PressedEdge = updateDebouncedEdge(BTN1_PIN, lastBtn1Stable, lastBtn1Raw, btn1LastChangeMs);
  if (btn1PressedEdge) {
    currentDigit = (currentDigit + 1) % 10;
    updateDisplayAndLed(currentDigit);
  }

  bool btn2PressedEdge = updateDebouncedEdge(BTN2_PIN, lastBtn2Stable, lastBtn2Raw, btn2LastChangeMs);
  if (btn2PressedEdge) {
    if (currentDigit == 0) currentDigit = 9;
    else currentDigit--;
    updateDisplayAndLed(currentDigit);
  }
}