// Arduino Mega 2560 sketch
// pot1 (A3) -> Red, pot2 (A4) -> Green, pot3 (A5) -> Blue
// rgb1 common cathode:
//   COM -> GND
//   R -> D2, G -> D3, B -> D4
//
// Behavior:
// - Start with LED off
// - Continuously update PWM based on potentiometer positions
// - If all pots at minimum (mapped to 0), LED remains off

const uint8_t POT1_PIN = A3;
const uint8_t POT2_PIN = A4;
const uint8_t POT3_PIN = A5;

const uint8_t RGB_R_PIN = 2;
const uint8_t RGB_G_PIN = 3;
const uint8_t RGB_B_PIN = 4;

static inline uint8_t mapToPWM(int val1023) {
  if (val1023 < 0) val1023 = 0;
  if (val1023 > 1023) val1023 = 1023;
  return (uint8_t)((val1023 * 255L) / 1023L);
}

void setup() {
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(POT3_PIN, INPUT);

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Start OFF state
  analogWrite(RGB_R_PIN, 0);
  analogWrite(RGB_G_PIN, 0);
  analogWrite(RGB_B_PIN, 0);
}

void loop() {
  int pot1 = analogRead(POT1_PIN); // 0..1023
  int pot2 = analogRead(POT2_PIN); // 0..1023
  int pot3 = analogRead(POT3_PIN); // 0..1023

  uint8_t redPWM   = mapToPWM(pot1);
  uint8_t greenPWM = mapToPWM(pot2);
  uint8_t bluePWM  = mapToPWM(pot3);

  // If all are at minimum, PWM values will be 0, keeping LED off.
  analogWrite(RGB_R_PIN, redPWM);
  analogWrite(RGB_G_PIN, greenPWM);
  analogWrite(RGB_B_PIN, bluePWM);
}