#include <Arduino.h>

// Circuit mappings (preserve wiring)
static const uint8_t BTN_PIN = 2;     // btn1: button signal -> mega:GND.1 and mega:2 (typical pull-down)
static const uint8_t SR_SHCP = A1;     // mega:A1 -> sr1:STCP (latch)
static const uint8_t SR_SHCP_UNUSED = A2; // (not used, but keep DS/SHCP mapping explicit)
static const uint8_t SR_DS   = A0;     // mega:A0 -> sr1:DS
static const uint8_t SR_SHCP_ACT = A2; // mega:A2 -> sr1:SHCP

// 74HC595 outputs drive sevseg1 segments and (also via shared SR) bargraph1 segments.
// sevseg1 segments:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
static const uint8_t Q0 = 0;
static const uint8_t Q1 = 1;
static const uint8_t Q2 = 2;
static const uint8_t Q3 = 3;
static const uint8_t Q4 = 4;
static const uint8_t Q5 = 5;
static const uint8_t Q6 = 6;
static const uint8_t Q7 = 7;
static const uint8_t Q8 = 8;
static const uint8_t Q9 = 9;
static const uint8_t Q10 = 10;

// bargraph1 10 LEDs:
// mega:3 -> A1, mega:4 -> A2, ... mega:12 -> A10
// Note: Q outputs beyond Q6 do not exist for a single 74HC595 (0..7), but the provided
// circuit includes SR1 Q1..Q6 connected to sevseg segments and does not show further Qn for bargraph.
// However, the wiring list explicitly connects mega:3..12 to bargraph1:A1..A10 and
// mega:GND.1 to bargraph1:C1..C10, so we must drive those pins directly.
// Thus, bargraph is not driven by the shift register; it is driven by Mega pins 3..12.
// We'll set them high for the corresponding LED count.

// Mega pins for bargraph anodes:
static const uint8_t BAR_A_PINS[10] = {3,4,5,6,7,8,9,10,11,12};
static const uint8_t BAR_C_PINS_GND = 0; // tied to GND in circuit; no pin needed.

static uint8_t currentNumber = 0;

// Build the 74HC595 bit pattern for sevseg display.
// We'll use common 7-seg mapping based on typical active-high segment outputs.
// The circuit likely uses an active-high configuration (green wiring to segments).
// Digits 0..9:
static const uint8_t digitToSegments[10] = {
  // bits: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G
  // 0: A,B,C,D,E,F on; G off
  (1<<Q0)|(1<<Q1)|(1<<Q2)|(1<<Q3)|(1<<Q4)|(1<<Q5),
  // 1: B,C
  (1<<Q1)|(1<<Q2),
  // 2: A,B,D,E,G
  (1<<Q0)|(1<<Q1)|(1<<Q3)|(1<<Q4)|(1<<Q6),
  // 3: A,B,C,D,G
  (1<<Q0)|(1<<Q1)|(1<<Q2)|(1<<Q3)|(1<<Q6),
  // 4: B,C,F,G
  (1<<Q1)|(1<<Q2)|(1<<Q5)|(1<<Q6),
  // 5: A,C,D,F,G
  (1<<Q0)|(1<<Q2)|(1<<Q3)|(1<<Q5)|(1<<Q6),
  // 6: A,C,D,E,F,G
  (1<<Q0)|(1<<Q2)|(1<<Q3)|(1<<Q4)|(1<<Q5)|(1<<Q6),
  // 7: A,B,C
  (1<<Q0)|(1<<Q1)|(1<<Q2),
  // 8: A,B,C,D,E,F,G
  (1<<Q0)|(1<<Q1)|(1<<Q2)|(1<<Q3)|(1<<Q4)|(1<<Q5)|(1<<Q6),
  // 9: A,B,C,D,F,G
  (1<<Q0)|(1<<Q1)|(1<<Q2)|(1<<Q3)|(1<<Q5)|(1<<Q6)
};

static void srWrite7Segments(uint8_t segMask7)
{
  // Write 8 bits to 74HC595 (only Q0..Q6 used; Q7 don't care => set to 0).
  // DS is shifted MSB-first or LSB-first depends on how it's wired internally; we'll choose
  // a standard: shiftOut with MSBFIRST as bit order Q7..Q0.
  // Since Q0..Q6 must match mask bits, we map:
  // when shifting MSB first, bit position (7-i) is written to Q(i) after latch.
  // We'll just use shiftOut with explicit byte where bit n corresponds to Qn.
  uint8_t data = segMask7 & 0x7F; // keep Q0..Q6; clear Q7

  digitalWrite(SR_SHCP_ACT, LOW); // not strictly needed

  // shiftOut uses the given order to place bits into DS on each clock pulse.
  // We'll clock on rising edge (as per standard Arduino shiftOut).
  shiftOut(SR_DS, SR_SHCP_ACT, MSBFIRST, data);

  // Latch register via STCP
  digitalWrite(SR_SHCP, HIGH);
  delayMicroseconds(5);
  digitalWrite(SR_SHCP, LOW);
}

static void updateBargraph(uint8_t n)
{
  // Light up corresponding number of LEDs: LED index 1..10, for n in 0..9 light first n LEDs.
  // Requirement: when number is 0, no LEDs lit.
  for (uint8_t i = 0; i < 10; i++)
  {
    // i corresponds to A1..A10
    digitalWrite(BAR_A_PINS[i], (i < n) ? HIGH : LOW);
  }
}

static void updateDisplay(uint8_t n)
{
  currentNumber = n;

  // sevseg: show number 0..9 on 7-seg
  uint8_t segMask = digitToSegments[n];
  srWrite7Segments(segMask);

  // bargraph: show same number by lighting up corresponding number of LEDs
  updateBargraph(n);
}

void setup()
{
  // Button input (btn1): wiring indicates btn1 has one side to GND and the other to mega pin.
  // So use INPUT_PULLUP? But the circuit explicitly ties one side to GND and the other to mega:2.
  // We'll configure as INPUT_PULLUP and treat press as LOW to be robust.
  pinMode(BTN_PIN, INPUT_PULLUP);

  // Shift register control pins
  pinMode(SR_DS, OUTPUT);
  pinMode(SR_SHCP_ACT, OUTPUT); // SHCP (clock)
  pinMode(SR_SHCP, OUTPUT);     // STCP (latch)

  digitalWrite(SR_DS, LOW);
  digitalWrite(SR_SHCP_ACT, LOW);
  digitalWrite(SR_SHCP, LOW);

  // Bargraph anode pins
  for (uint8_t i = 0; i < 10; i++)
  {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW);
  }

  // Initial state: display 0, bargraph none lit
  updateDisplay(0);
}

void loop()
{
  // Button press increment with debounce and immediate update.
  static bool lastStablePressed = false;
  static unsigned long lastDebounceTime = 0;
  static bool debouncedPressed = false;

  bool reading = (digitalRead(BTN_PIN) == LOW); // pressed when pulled to GND

  if (reading != lastStablePressed)
  {
    lastDebounceTime = millis();
    lastStablePressed = reading;
  }

  if ((millis() - lastDebounceTime) >= 25)
  {
    // debounce settled
    if (debouncedPressed != reading)
    {
      debouncedPressed = reading;
      if (debouncedPressed)
      {
        // On rising edge of press (actuation)
        uint8_t next = currentNumber + 1;
        if (next > 9) next = 0;
        updateDisplay(next);
      }
    }
  }

  // Small yield
  delay(1);
}