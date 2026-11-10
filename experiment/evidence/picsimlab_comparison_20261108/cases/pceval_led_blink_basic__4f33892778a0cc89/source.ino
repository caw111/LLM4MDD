#include <Arduino.h>

// Wiring: pin 13 -> 220 ohm resistor -> LED anode; LED cathode -> GND.1.
// Resistor property: "value" = "220 ohm".
constexpr uint8_t LED_PIN = 13;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);
  Serial.println("LED Blink test starts");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);

  digitalWrite(LED_PIN, LOW);
  delay(1000);
}