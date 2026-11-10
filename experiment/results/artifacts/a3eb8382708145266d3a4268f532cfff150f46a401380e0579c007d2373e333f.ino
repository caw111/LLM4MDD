#include <Arduino.h>
#include <Servo.h>

// Shift register (74HC595) wiring
static const uint8_t SR_DS   = A0; // mega:A0 -> sr1:DS
static const uint8_t SR_STCP = A1; // mega:A1 -> sr1:STCP (latch)
static const uint8_t SR_SHCP = A2; // mega:A2 -> sr1:SHCP (clock)

// Buttons wiring
static const uint8_t BTN1 = 2; // mega:2
static const uint8_t BTN2 = 3; // mega:3
static const uint8_t BTN3 = 4; // mega:4

// Servo wiring
static const uint8_t SERVO1_PIN = 5; // mega:5 -> servo1:PWM
static const uint8_t SERVO2_PIN = 6; // mega:6 -> servo2:PWM

// Digit segment bit positions for shift register outputs:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// (Segments are active HIGH as typical for direct wiring)
static const uint8_t segMap[10] = {
  // 0: a b c d e f
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),
  // 1: b c
  (1 << 1) | (1 << 2),
  // 2: a b d e g
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),
  // 3: a b c d g
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),
  // 4: b c f g
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),
  // 5: a c d f g
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),
  // 6: a c d e f g
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 7: a b c
  (1 << 0) | (1 << 1) | (1 << 2),
  // 8: a b c d e f g
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 9: a b c d f g
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)
};

static Servo servo1;
static Servo servo2;

static uint8_t currentNumber = 0;

// Debounced edge detection structure
struct ButtonEdgeDebounce {
  uint8_t pin;
  bool stableLevel;              // stable sampled level
  unsigned long lastChangeMs;  // last time the raw level differed from stableLevel
  bool prevStableLevel;         // previous stable level for edge detect
};

// Shift register write for 7-seg (Q0..Q6 mapped to segments A..G)
static void shiftRegisterWrite7(uint8_t sevenSegBits) {
  uint8_t data = sevenSegBits & 0x7F; // only Q0..Q6 used

  digitalWrite(SR_STCP, LOW);

  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(SR_DS, (data >> i) & 0x01);
    digitalWrite(SR_SHCP, HIGH);
    digitalWrite(SR_SHCP, LOW);
  }

  digitalWrite(SR_STCP, HIGH);
  digitalWrite(SR_STCP, LOW);
}

static void updateDisplayAndServos() {
  currentNumber %= 10;

  // Display update immediately
  shiftRegisterWrite7(segMap[currentNumber]);

  // Servo update rules
  if (currentNumber <= 4) {
    servo1.write(0);
    servo2.write(90);
  } else {
    servo1.write(90);
    servo2.write(180);
  }
}

// Debounce and detect press event (INPUT_PULLUP => pressed = LOW)
// Returns true exactly when a stable HIGH->LOW transition is detected.
static bool updateButtonPress(ButtonEdgeDebounce &btn, unsigned long now, unsigned long debounceMs) {
  bool level = digitalRead(btn.pin);

  // Debounce: update stableLevel if level stays different long enough
  if (level != btn.stableLevel) {
    if (now - btn.lastChangeMs >= debounceMs) {
      btn.stableLevel = level;
    }
  } else {
    // no change
  }

  // Track when the raw level first differs
  if (level != btn.stableLevel) {
    btn.lastChangeMs = now;
  }

  // Edge detect: stable HIGH -> stable LOW
  bool pressed = (btn.prevStableLevel == HIGH) && (btn.stableLevel == LOW);
  btn.prevStableLevel = btn.stableLevel;
  return pressed;
}

static ButtonEdgeDebounce b1{BTN1, HIGH, 0, HIGH};
static ButtonEdgeDebounce b2{BTN2, HIGH, 0, HIGH};
static ButtonEdgeDebounce b3{BTN3, HIGH, 0, HIGH};

static void handleButtons() {
  const unsigned long now = millis();
  const unsigned long debounceMs = 25;

  bool p1 = updateButtonPress(b1, now, debounceMs);
  bool p2 = updateButtonPress(b2, now, debounceMs);
  bool p3 = updateButtonPress(b3, now, debounceMs);

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

  // Buttons with internal pullups (pressed -> LOW)
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

  // Attach servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Initialization: show "0" and servos at their 0-degree position
  currentNumber = 0;
  servo1.write(0);
  servo2.write(0);

  // Show digit 0
  shiftRegisterWrite7(segMap[0]);

  // Apply servo rule mapping after number is set (for number 0 => servo1=0, servo2=90)
  updateDisplayAndServos();

  // Initialize button debouncing states
  b1.stableLevel = digitalRead(b1.pin);
  b2.stableLevel = digitalRead(b2.pin);
  b3.stableLevel = digitalRead(b3.pin);
  b1.prevStableLevel = b1.stableLevel;
  b2.prevStableLevel = b2.stableLevel;
  b3.prevStableLevel = b3.stableLevel;
  b1.lastChangeMs = millis();
  b2.lastChangeMs = millis();
  b3.lastChangeMs = millis();
}

void loop() {
  handleButtons();
}