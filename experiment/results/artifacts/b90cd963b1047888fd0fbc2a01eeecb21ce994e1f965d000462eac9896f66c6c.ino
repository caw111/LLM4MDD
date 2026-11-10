// Read potentiometer connected to A0 and control PWM LED brightness on D9.

const uint8_t PWM_PIN = 9;   // D9 (PWM output)
const uint8_t POT_PIN = A0;  // A0 (analog input)

void setup() {
  pinMode(PWM_PIN, OUTPUT);
}

void loop() {
  int potValue = analogRead(POT_PIN); // 0..1023
  // Map to 8-bit PWM duty: 0..255
  uint8_t duty = (uint8_t)((potValue * 255L) / 1023L);
  analogWrite(PWM_PIN, duty);
}