#include <Arduino.h>

static const uint8_t POT_PIN = A3;      // pot1:SIG -> mega:A3
static const uint8_t LED_R_PIN = 2;     // rgb1:R -> mega:2
static const uint8_t LED_G_PIN = 3;     // rgb1:G -> mega:3
static const uint8_t LED_B_PIN = 4;     // rgb1:B -> mega:4

void setup() {
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);

  // rgb1 is common cathode: turning a color ON requires driving its pin LOW.
  digitalWrite(LED_R_PIN, HIGH);
  digitalWrite(LED_G_PIN, HIGH);
  digitalWrite(LED_B_PIN, HIGH);
}

void loop() {
  int potValue = analogRead(POT_PIN); // 0..1023 (default)

  if (potValue < 341) {
    // First third: Red
    digitalWrite(LED_R_PIN, LOW);
    digitalWrite(LED_G_PIN, HIGH);
    digitalWrite(LED_B_PIN, HIGH);
  } else if (potValue < 682) {
    // Second third: Green
    digitalWrite(LED_R_PIN, HIGH);
    digitalWrite(LED_G_PIN, LOW);
    digitalWrite(LED_B_PIN, HIGH);
  } else {
    // Final third: Blue
    digitalWrite(LED_R_PIN, HIGH);
    digitalWrite(LED_G_PIN, HIGH);
    digitalWrite(LED_B_PIN, LOW);
  }
}