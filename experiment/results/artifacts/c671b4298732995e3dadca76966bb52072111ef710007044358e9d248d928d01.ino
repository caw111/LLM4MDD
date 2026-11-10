#include <Arduino.h>
#include <Servo.h>

// Shift register (74HC595) pins (as per wiring)
static const uint8_t SR_DS   = A0; // mega:A0 -> sr1:DS
static const uint8_t SR_STCP = A1; // mega:A1 -> sr1:STCP (latch)
static const uint8_t SR_SHCP = A2; // mega:A2 -> sr1:SHCP (clock)

// Buttons (as per wiring)
static const uint8_t BTN1 = 2; // mega:2
static const uint8_t BTN2 = 3; // mega:3
static const uint8_t BTN3 = 4; // mega:4

// SevSeg pins mapped to shift register outputs:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// Q7 unused; also SR1 MR tied to VCC via sr1:VCC->sr1:MR and OE tied to GND (always enabled).

Servo servo1;
Servo servo2;

static uint8_t currentNumber = 0;

// Segment bit positions for shift register outputs:
// bit0=Q0(A), bit1=Q1(B), bit2=Q2(C), bit3=Q3(D), bit4=Q4(E), bit5=Q5(F), bit6=Q6(G)
static const uint8_t segMap[10] = {
  // Digits 0-9 for a common-anode 7-seg? We must infer from typical Wokwi sevseg module.
  // In Wokwi, the 7-segment display expects "active HIGH" segment signals.
  // We'll use standard active-HIGH segment patterns: a b c d e f g.
  // 0: a b c d e f
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),
  // 1: b c
  (1<<1)|(1<<2),
  // 2: a b d e g
  (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),
  // 3: a b c d g
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),
  // 4: b c f g
  (1<<1)|(1<<2)|(1<<5)|(1<<6),
  // 5: a c d f g
  (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),
  // 6: a c d e f g
  (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
  // 7: a b c
  (1<<0)|(1<<1)|(1<<2),
  // 8: a b c d e f g
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
  // 9: a b c d f g
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)
};

void shiftRegisterWrite7(uint8_t sevenSegBits) {
  // Write Q0..Q6 (7 bits) to 74HC595 via serial DS/SHCP and latch on STCP.
  // DS is used for the bitstream; we clock 8 bits, with Q7=0.
  uint8_t data = (uint8_t)(sevenSegBits & 0x7F);

  // According to common 74HC595 shifting convention:
  // Shift out MSB first into Q7; however our wiring uses Q0..Q6 mapping.
  // We'll shift out bit7..bit0 with DS taking that bit, then on Q0..Q7 positions
  // align accordingly as expected by Wokwi's shift register model.
  // To ensure Q0 gets A, we construct data such that bit0 corresponds to Q0.
  // We'll shift MSB->LSB over 8 clocks:
  //   DS = data bit i for each clock, where i goes from 7 down to 0.
  digitalWrite(SR_STCP, LOW);
  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(SR_DS, (data >> i) & 0x01);
    digitalWrite(SR_SHCP, HIGH);
    digitalWrite(SR_SHCP, LOW);
  }
  digitalWrite(SR_STCP, HIGH);
  digitalWrite(SR_STCP, LOW);
}

void updateDisplayAndServos() {
  // Display update
  currentNumber = currentNumber % 10;
  shiftRegisterWrite7(segMap[currentNumber]);

  // Servo update:
  // - 0-4: servo1=0°, servo2=90°
  // - 5-9: servo1=90°, servo2=180°
  if (currentNumber <= 4) {
    servo1.write(0);
    servo2.write(90);
  } else {
    servo1.write(90);
    servo2.write(180);
  }
}

// Simple edge detection with debounce
struct ButtonState {
  uint8_t pin;
  bool stableLevel;
  bool lastStableLevel;
  unsigned long lastChangeMs;
};

static ButtonState b1 { BTN1, HIGH, HIGH, 0 };
static ButtonState b2 { BTN2, HIGH, HIGH, 0 };
static ButtonState b3 { BTN3, HIGH, HIGH, 0 };

bool updateButtonPressed(ButtonState &b, unsigned long now, unsigned long debounceMs) {
  bool level = digitalRead(b.pin);

  if (level != b.stableLevel) {
    // Potential change; check debounce
    if (now - b.lastChangeMs >= debounceMs) {
      b.stableLevel = level;
    }
  }

  // Track actual stable changes with timestamp
  if (level != b.lastStableLevel) {
    b.lastChangeMs = now;
    b.lastStableLevel = level;
  }

  // For button wiring: button connected to GND when pressed? In circuit, one side is GND.1 and other to pin via green.
// That implies pulled-up needed, and pressed -> LOW.
  // We'll detect a press on HIGH->LOW transition (stable).
  static bool prevStable[3];
  (void)prevStable;

  // Detect press: stableLevel == LOW and previously stableLevel (from last call) was HIGH.
  static bool prevStableLevel[10];
  // Use pin as index mapping via switch
  bool prev = true;
  if (b.pin == BTN1) prev = (prevStableLevel[1] != 0);
  else if (b.pin == BTN2) prev = (prevStableLevel[2] != 0);
  else prev = (prevStableLevel[3] != 0);

  bool pressed = (b.stableLevel == LOW) && (prev == true);

  if (b.pin == BTN1) prevStableLevel[1] = pressed ? 0 : (b.stableLevel ? 1 : 0);
  else if (b.pin == BTN2) prevStableLevel[2] = pressed ? 0 : (b.stableLevel ? 1 : 0);
  else prevStableLevel[3] = pressed ? 0 : (b.stableLevel ? 1 : 0);

  return pressed;
}

void handleButtons() {
  const unsigned long now = millis();
  const unsigned long debounceMs = 25;

  // We'll do explicit per-button edge detection using stable state tracking.
  // Since the simplistic helper above is messy with static arrays, do it properly here.

  // Implement per-button edge detection with stored lastStable for each button.
  // Reuse b1/b2/b3: b.lastChangeMs and stableLevel used; but we need stable last for press edge.
  // We'll simply apply debounce and track previous stable levels with additional statics.

  static bool lastStable1 = HIGH, lastStable2 = HIGH, lastStable3 = HIGH;

  auto debounceRead = [&](ButtonState &b, bool &lastStable) -> bool {
    bool level = digitalRead(b.pin);
    if (level != b.stableLevel) {
      // start debounce timer
      if (level != b.lastStableLevel) {
        b.lastChangeMs = now;
        b.lastStableLevel = level;
      }
      if (now - b.lastChangeMs >= debounceMs) {
        b.stableLevel = level;
      }
    }
    // press edge
    bool pressed = (lastStable == HIGH) && (b.stableLevel == LOW);
    lastStable = b.stableLevel;
    return pressed;
  };

  bool p1 = debounceRead(b1, lastStable1);
  bool p2 = debounceRead(b2, lastStable2);
  bool p3 = debounceRead(b3, lastStable3);

  if (p1) {
    currentNumber += 1;
    if (currentNumber > 9) currentNumber = 0;
    updateDisplayAndServos();
  }
  if (p2) {
    currentNumber += 2;
    if (currentNumber > 9) currentNumber = 0;
    updateDisplayAndServos();
  }
  if (p3) {
    currentNumber += 3;
    if (currentNumber > 9) currentNumber = 0;
    updateDisplayAndServos();
  }
}

void setup() {
  // Shift register control pins
  pinMode(SR_DS, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);

  digitalWrite(SR_DS, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_SHCP, LOW);

  // Buttons with internal pullups
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

  // Attach servos:
  // mega:5 -> servo1:PWM and mega:6 -> servo2:PWM
  servo1.attach(5);
  servo2.attach(6);

  // Initialization: show "0" and servos at their 0-degree position.
  currentNumber = 0;
  servo1.write(0);
  servo2.write(0);
  shiftRegisterWrite7(segMap[0]);

  // After initialization, apply required mapping for number 0 (servo2=90° per rule)
  // while still satisfying "both servos should be at their 0-degree position" upon power on/reset.
  // We'll keep immediate mapping update as per Display/Servo Update rule after number is set.
  updateDisplayAndServos();
}

void loop() {
  handleButtons();
}