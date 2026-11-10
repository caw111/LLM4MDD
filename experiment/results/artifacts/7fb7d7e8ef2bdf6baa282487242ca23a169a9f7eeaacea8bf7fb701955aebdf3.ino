// One Digit 7-segment Display (Common Anode) showing number 5
// Pins A..G,DP connected via 330Ω resistors to Arduino D2..D9 as given by the circuit:
// Arduino D2 -> resistor7:2 -> 7seg F
// Arduino D3 -> resistor8:2 -> 7seg A
// Arduino D4 -> resistor6:2 -> 7seg G
// Arduino D5 -> resistor2:2 -> 7seg B
// Arduino D6 -> resistor5:2 -> 7seg D
// Arduino D7 -> resistor3:2 -> 7seg C
// Arduino D8 -> resistor4:2 -> 7seg E
// Arduino D9 -> resistor1:2 -> 7seg DP
//
// Common anode COM.1 -> 5V
// For common anode displays: segment ON = LOW, segment OFF = HIGH.

const uint8_t PIN_F  = 2; // segment F
const uint8_t PIN_A  = 3; // segment A
const uint8_t PIN_G  = 4; // segment G
const uint8_t PIN_B  = 5; // segment B
const uint8_t PIN_D  = 6; // segment D
const uint8_t PIN_C  = 7; // segment C
const uint8_t PIN_E  = 8; // segment E
const uint8_t PIN_DP = 9; // decimal point

void setup() {
  // 1. Set 2~9 pin as output.
  pinMode(PIN_F, OUTPUT);
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_D, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_E, OUTPUT);
  pinMode(PIN_DP, OUTPUT);

  // Start with all segments off (common anode: OFF = HIGH)
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_D, HIGH);
  digitalWrite(PIN_E, HIGH);
  digitalWrite(PIN_F, HIGH);
  digitalWrite(PIN_G, HIGH);
  digitalWrite(PIN_DP, HIGH);

  // 2. Set pins so that number 5 appears.
  // For digit '5' on a common anode 7-seg:
  // ON segments: A, F, G, C, D
  // OFF segments: B, E, DP
  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_F, LOW);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_D, LOW);

  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_E, HIGH);
  digitalWrite(PIN_DP, HIGH);
}

void loop() {
  // Keep displaying '5' (no additional behavior required).
  // (Re-apply to ensure stable output after any potential disturbances.)
  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_F, LOW);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_D, LOW);

  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_E, HIGH);
  digitalWrite(PIN_DP, HIGH);
}