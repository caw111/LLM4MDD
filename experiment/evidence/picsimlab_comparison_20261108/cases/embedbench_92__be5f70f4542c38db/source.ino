const int LED_PIN = 2;     // led1:A -> mega:2
const int POT1_PIN = A3;   // pot1:SIG -> mega:A3
const int POT2_PIN = A4;   // pot2:SIG -> mega:A4

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0); // LED should be off on power-up/reset
}

void loop() {
  int pot1Val = analogRead(POT1_PIN); // 0..1023
  int pot2Val = analogRead(POT2_PIN); // 0..1023

  int brightness = map(pot1Val, 0, 1023, 0, 255);

  if (pot1Val > pot2Val) {
    analogWrite(LED_PIN, brightness); // on at brightness from pot1
  } else {
    analogWrite(LED_PIN, 0); // off when pot1 <= pot2
  }
}