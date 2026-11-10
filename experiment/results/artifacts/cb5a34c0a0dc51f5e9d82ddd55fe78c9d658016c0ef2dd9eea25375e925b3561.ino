#include <Arduino.h>

const uint8_t LED1_PIN = 2;
const uint8_t LED2_PIN = 3;
const uint8_t LED3_PIN = 4;

const uint8_t RGB_R_PIN = 5;
const uint8_t RGB_G_PIN = 6;
const uint8_t RGB_B_PIN = 7;

const uint8_t POT_PIN = A3;

static void setAllOff() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  analogWrite(RGB_R_PIN, 0);
  analogWrite(RGB_G_PIN, 0);
  analogWrite(RGB_B_PIN, 0);
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  // RGB LED is common cathode (per circuit), so driving HIGH/greater PWM increases brightness.
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  pinMode(POT_PIN, INPUT);

  setAllOff();
}

void loop() {
  int potVal = analogRead(POT_PIN); // 0..1023

  // Indicators based on thirds:
  // 0-341 -> led1, 342-682 -> led2, 683-1023 -> led3
  if (potVal <= 341) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
  } else if (potVal <= 682) {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, LOW);
  } else {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, HIGH);
  }

  // RGB brightness rules:
  // - red max when in lower third
  // - green max when in middle third
  // - blue max when in upper third
  // "max brightness" is 255; each color channel mapped per the potentiometer value rules.
  // Given the explicit third-based channel selection, we apply 255 to the selected channel.
  if (potVal <= 341) {
    analogWrite(RGB_R_PIN, 255);
    analogWrite(RGB_G_PIN, 0);
    analogWrite(RGB_B_PIN, 0);
  } else if (potVal <= 682) {
    analogWrite(RGB_R_PIN, 0);
    analogWrite(RGB_G_PIN, 255);
    analogWrite(RGB_B_PIN, 0);
  } else {
    analogWrite(RGB_R_PIN, 0);
    analogWrite(RGB_G_PIN, 0);
    analogWrite(RGB_B_PIN, 255);
  }

  delay(10); // small stabilizing delay; behavior remains continuous
}