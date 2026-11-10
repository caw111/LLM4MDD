// LED control sequence for Arduino Mega 2560
// Wiring preserved:
//   mega:GND -> led1:C
//   led1:A   -> mega pin 2 (red)

const uint8_t LED_PIN = 2;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED off on power/reset
}

void loop() {
  digitalWrite(LED_PIN, HIGH); delay(1000); // ON for 1s
  digitalWrite(LED_PIN, LOW);  delay(2000); // OFF for 2s
  digitalWrite(LED_PIN, HIGH); delay(3000); // ON for 3s
  digitalWrite(LED_PIN, LOW);  delay(3000); // OFF for 3s
}