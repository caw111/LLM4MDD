// Arduino Mega 2560 - LED control sequence
// Wiring preserved:
// - led1:C -> mega GND
// - led1:A -> mega pin 2

const uint8_t LED_PIN = 2;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED off at power on/reset
}

void loop() {
  // 1) ON for 1 second
  digitalWrite(LED_PIN, HIGH);
  delay(1000);

  // 2) OFF for 2 seconds
  digitalWrite(LED_PIN, LOW);
  delay(2000);

  // 3) ON for 3 seconds
  digitalWrite(LED_PIN, HIGH);
  delay(3000);

  // 4) OFF for 3 seconds
  digitalWrite(LED_PIN, LOW);
  delay(3000);
}