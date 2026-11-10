#include <Arduino.h>
#include <Servo.h>
#include <LedControl.h>

//
// Wiring from the circuit (Mega 2560):
// 74HC595 #1 (sr1) - controls sevseg1 (COM.1 tied to both displays' COM.1)
//   DS  = mega A0
//   SHCP= mega A2
//   STCP= mega A1
//   MR  = sr1:MR tied to sr1:VCC (so no reset pin control needed)
//   OE  = sr1:OE tied to GND (always enabled)
//   GND = shared with sr1 GND (and also with sr2 GND via sr1:GND <-> sr2:GND)
// 74HC595 #2 (sr2) - controls sevseg2
//   DS  = sr2:DS connected to sr1:Q7S (i.e., Q7S from sr1 shifts into sr2)
//   SHCP and STCP shared with sr1
//
// 10-segment bargraph (bargraph1)
//   A1..A10 => mega pins 2..11
//   C1..C10 => all tied to mega GND.1 (same net)
//
// Servos:
//   servo1 PWM = mega pin 12
//   servo2 PWM = mega pin 13
//   servo1/2 V+ to 5V, GND to GND.1
//

// ----------------- 7-seg via 2 chained 74HC595 -----------------
// We use LedControl just to generate segment patterns and handle cascading.
static const int DIN_PIN  = A0; // sr1:DS
static const int CS_PIN   = A1; // sr1:STCP
static const int CLK_PIN  = A2; // sr1:SHCP

// We'll treat each 7-seg display as one "device" in the chain.
// Device order in LedControl: device 0 is the last shifted-in (closest to MCU) depending on wiring.
// Since sr2:DS is connected to sr1:Q7S, sr1 is closer to MCU and sr2 is farther.
// LedControl shifts MSB-first across devices; with typical wiring, device 0 corresponds to the first device after shifting.
// In practice for cascaded 74HC595, the earlier device in chain ends up with higher-order bits.
// We'll set device0=sevseg1 (sr1 output) and device1=sevseg2 (sr2 output) and verify mapping by setting patterns.
// For this problem, assume standard ordering: device 0 controls the first display (sr1), device 1 controls second (sr2).
static LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 2);

// 7-seg segment map for digits 0-9 in terms of LedControl's bit positions.
// LedControl expects bits: 0=A,1=B,2=C,3=D,4=E,5=F,6=G,7=DP.
// We assume the circuit connects sr1 Q0..Q6 to sevseg A..G and (Q7S is unused here for DP).
// Therefore we map A..G to bits 0..6.
static const byte DIGIT_TO_SEG[10] = {
  // A  B  C  D  E  F  G  DP (DP off)
  // bit 0=A,1=B,2=C,3=D,4=E,5=F,6=G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),          // 0: A B C D E F
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),                                   // 1: B C F G
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),                      // 2: A B D E G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),                      // 3: A B C D G
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6) | (1 << 4),                      // 4: B C F G E  (i.e., B C E F G)
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 6),                      // 5: A C D E G
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),         // 6: A C D E F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 5),                                   // 7: A B C F
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),// 8: all A..G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)          // 9: A B C D F G
};

// LedControl uses setRow(device,row,value) for 8x8 matrix. For 7-seg emulation,
// we use row=0 and encode segments in value bits 0..6 (and optionally 7 for DP).
// We disable scan of other rows by just setting row 0.
static void showDigitOnDevice(int device, int digit) {
  if (digit < 0 || digit > 9) digit = 0;
  lc.setRow(device, 0, DIGIT_TO_SEG[digit]);
}

// Render two-digit number on sevseg1 (tens) and sevseg2 (units).
static void showNumberTwoDigits(int n) {
  int tens  = (n / 10) % 10;
  int units = n % 10;
  // device 0 -> sevseg1 (tens), device 1 -> sevseg2 (units)
  showDigitOnDevice(0, tens);
  showDigitOnDevice(1, units);
}

// ----------------- Bargraph (10 LEDs) -----------------
// Wiring: each LED segment A1..A10 to pins 2..11. All cathodes C1..C10 tied to GND.1.
// We'll treat LED as active HIGH: drive A pins HIGH for lit LEDs.
// Use count = number/10? Requirement: "if number is 50, 5 LEDs lit" => number%/10.
static const uint8_t BAR_PINS[10] = {2,3,4,5,6,7,8,9,10,11};

// ----------------- Servos -----------------
static const uint8_t SERVO1_PIN = 12;
static const uint8_t SERVO2_PIN = 13;
Servo servo1, servo2;

static float angleForNumber(int n) {
  return (float)n * 1.8f; // 0..178.2 for 0..99
}

static void updateAll(int n) {
  // 7-seg
  showNumberTwoDigits(n);

  // Bargraph: proportional to number as percentage 0..100, but for 0..99 it maps to LEDs=number/10
  // Example given: 50 -> 5 LEDs.
  int ledsOn = n / 10; // 0..9 (for 99 -> 9)
  if (ledsOn < 0) ledsOn = 0;
  if (ledsOn > 10) ledsOn = 10;

  for (int i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], (i < ledsOn) ? HIGH : LOW);
  }

  // Servos
  float ang = angleForNumber(n);
  servo1.write(ang);
  servo2.write(ang);
}

// Timing: increment every 2 seconds; reset after reaching 99.
static unsigned long lastTickMs = 0;
static int numberValue = 0;

void setup() {
  // Bargraph pins
  for (int i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  // 7-seg / 74HC595 chain via LedControl
  lc.shutdown(0, false);
  lc.shutdown(1, false);
  // LedControl uses intensity (0..15). We'll set a mid value.
  lc.setIntensity(0, 8);
  lc.setIntensity(1, 8);
  lc.clearDisplay(0);
  lc.clearDisplay(1);

  // Servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(0);
  servo2.write(0);

  // Initial state: "00", bargraph off, servos at 0 degrees
  numberValue = 0;
  updateAll(numberValue);

  lastTickMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastTickMs >= 2000UL) {
    lastTickMs = now;

    numberValue++;
    if (numberValue > 99) numberValue = 0;

    updateAll(numberValue);
  }
}