// Requirements:
// - Use A1 photoresistor to detect light intensity.
// - If below a threshold, turn ON D8 LED.

const uint8_t PHOTO_PIN = A1;
const uint8_t LED_PIN = 8;

// Threshold for "dark" condition (raw ADC value).
// Adjust as needed to match your specific photoresistor/LDR and wiring.
const int THRESHOLD = 500;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  int light = analogRead(PHOTO_PIN); // 0..1023
  if (light < THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}