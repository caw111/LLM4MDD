#include <Arduino.h>
#include <Servo.h>

static inline uint8_t mapInt(uint16_t x, uint16_t in_min, uint16_t in_max, uint8_t out_min, uint8_t out_max) {
  if (in_max == in_min) return out_min;
  if (x <= in_min) return out_min;
  if (x >= in_max) return out_max;
  uint32_t num = (uint32_t)(x - in_min) * (uint32_t)(out_max - out_min);
  uint32_t den = (uint32_t)(in_max - in_min);
  return (uint8_t)(out_min + (num / den));
}

static inline int clampInt(int v, int lo, int hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

// 7-seg segment bit order for the shift register outputs:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// Q7S is connected to the other DS line; we keep it low for both displays.
static const uint8_t segDigit[10] = {
  // 0
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),
  // 1
  (1 << 1) | (1 << 2),
  // 2
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),
  // 3
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),
  // 4
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),
  // 5
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),
  // 6
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 7
  (1 << 0) | (1 << 1) | (1 << 2),
  // 8
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 9
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)
};

// Pins (as per wiring in the provided circuit)
static const uint8_t SR1_SHCP = A2;  // gray
static const uint8_t SR1_STCP = A1;  // purple
static const uint8_t SR1_DS   = A0;  // blue

static const uint8_t SR1_OE   = 0;   // not used directly (OE tied between sr1 and sr2 to GND in circuit)

// Servo PWM pins
static const uint8_t SERVO1_PWM = 2; // orange
static const uint8_t SERVO2_PWM = 5; // orange

// Potentiometers
static const uint8_t POT1_SIG = A3;  // pot1 -> A3
static const uint8_t POT2_SIG = A4;  // pot2 -> A4

Servo servo1;
Servo servo2;

static int targetDeg1 = 0;
static int targetDeg2 = 0;

static int currentDeg1 = 0;
static int currentDeg2 = 0;

static unsigned long lastUpdateMs = 0;

static void shiftRegisterWrite(uint8_t valueWithQ0toQ6, bool enableQ7S = false) {
  // We need to preserve wiring behavior; OE is tied low, so output always enabled.
  // We shift Q0..Q7S bits in sequence; the circuit uses outputs Q0..Q6 to A..G.
  // DS is SR1_DS. SHCP and STCP are shared with both shift registers through sr1's pins.
  // To update both digits: strobe (STCP) after shifting data.
  uint8_t v = (valueWithQ0toQ6 & 0x7F); // bits 0..6
  if (enableQ7S) v |= (1 << 7);
  else v &= ~(1 << 7);

  // Write MSB first for typical 74HC595 expectations.
  // This will set Q7S..Q0 depending on bit order; the circuit labeling expects Q0->A etc.
  // Using MSB-first with our bit positions for Q0..Q6 works if DS->Q0 alignment follows typical behavior.
  // To be robust, we shift out bits in reverse so that DS->first shifted corresponds to Q0.
  // Since the diagram labels Q0..Q7S explicitly, we match by shifting LSB first:
  for (int i = 0; i < 8; i++) {
    digitalWrite(SR1_DS, (v & (1 << i)) ? HIGH : LOW); // LSB first -> Q0 first
    // SHCP rising edge shifts in one bit
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(2);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(2);
  }

  // STCP rising edge latches outputs
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(5);
  digitalWrite(SR1_STCP, LOW);
}

static void displayTwoDigits(uint8_t tens, uint8_t units) {
  tens = clampInt(tens, 0, 9);
  units = clampInt(units, 0, 9);

  // Both shift registers share SHCP and STCP from sr1:
  // - sr1 outputs go to sevseg1 (tens)
  // - sr2 outputs go to sevseg2 (units)
  // The circuit connects sr1:Q7S to sr2:DS, so Q7S becomes the serial input to sr2.
  // In the combined chain, updating requires shifting 16 bits (sr1 then sr2).
  //
  // We therefore shift out sr1's 8 bits first, then sr2's 8 bits.
  // We'll keep Q7S low for both so it doesn't light a segment unexpectedly;
  // segments are only on Q0..Q6.

  uint8_t v1 = segDigit[tens] & 0x7F;   // sr1 Q0..Q6
  uint8_t v2 = segDigit[units] & 0x7F;  // sr2 Q0..Q6

  // Clear Q7S (bit7) for both.
  v1 &= 0x7F;
  v2 &= 0x7F;

  // Shift 16 bits LSB-first for each chip to align Q0..Q6 labeling.
  // Bits to shift:
  // - First 8 bits go into sr1 (DS -> sr1 shift register), correspond to v1 Q0..Q7
  // - Next 8 bits go into sr2 (through sr1 Q7S), correspond to v2 Q0..Q7
  for (int i = 0; i < 8; i++) {
    digitalWrite(SR1_DS, (v1 & (1 << i)) ? HIGH : LOW); // sr1 bit i -> Q{i}
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(2);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(2);
  }
  for (int i = 0; i < 8; i++) {
    digitalWrite(SR1_DS, (v2 & (1 << i)) ? HIGH : LOW); // sr2 bit i -> Q{i}
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(2);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(2);
  }

  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(5);
  digitalWrite(SR1_STCP, LOW);
}

static int smoothStep(int current, int target, int maxDeltaPerUpdate) {
  if (current == target) return current;
  if (target > current) {
    int next = current + maxDeltaPerUpdate;
    return (next > target) ? target : next;
  } else {
    int next = current - maxDeltaPerUpdate;
    return (next < target) ? target : next;
  }
}

void setup() {
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // Attach servos
  servo1.attach(SERVO1_PWM);
  servo2.attach(SERVO2_PWM);

  // Initialization requirement: displays "00", servos at 0 degrees.
  currentDeg1 = 0;
  currentDeg2 = 0;
  targetDeg1 = 0;
  targetDeg2 = 0;

  servo1.write(0);
  servo2.write(0);

  displayTwoDigits(0, 0);

  lastUpdateMs = millis();
}

void loop() {
  // Read potentiometers mapped to 0..99 degrees.
  int raw1 = analogRead(POT1_SIG); // 0..1023
  int raw2 = analogRead(POT2_SIG);

  // Map 0..1023 -> 0..99 (inclusive)
  int newTarget1 = mapInt((uint16_t)raw1, 0, 1023, 0, 99);
  int newTarget2 = mapInt((uint16_t)raw2, 0, 1023, 0, 99);

  targetDeg1 = newTarget1;
  targetDeg2 = newTarget2;

  unsigned long now = millis();
  unsigned long dt = now - lastUpdateMs;

  // Smoothly update servos; choose speed based on dt for smooth motion.
  // Max delta per 20ms update ~ 1 degree gives smooth movement.
  const float maxDegPerSecond = 60.0f; // adjust for smoothness without being too fast
  int maxDelta = (int)clampInt((int)(maxDegPerSecond * (dt / 1000.0f)), 1, 5);
  if (dt == 0) maxDelta = 1;

  currentDeg1 = smoothStep(currentDeg1, targetDeg1, maxDelta);
  currentDeg2 = smoothStep(currentDeg2, targetDeg2, maxDelta);

  servo1.write(currentDeg1);
  servo2.write(currentDeg2);

  // Display requirement: show current angles of servo1 as two digits.
  uint8_t tens = (uint8_t)(currentDeg1 / 10);
  uint8_t units = (uint8_t)(currentDeg1 % 10);
  displayTwoDigits(tens, units);

  lastUpdateMs = now;
}