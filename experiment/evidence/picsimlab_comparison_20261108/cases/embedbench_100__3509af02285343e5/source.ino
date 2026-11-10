#include <Arduino.h>

const uint8_t LED1_PIN = 2;   // led1:A
const uint8_t LED2_PIN = 3;   // led2:A

// bargraph1: A1..A10
const uint8_t BAR_A_PINS[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

// Potentiometers: pot1->A3, pot2->A4, pot3->A5
const uint8_t POT1_PIN = A3;
const uint8_t POT2_PIN = A4;
const uint8_t POT3_PIN = A5;

// Initialize PWM for LED pins that support it on Mega 2560:
// - D2: OC3B
// - D3: OC3C
static void setupPWMDefaults() {
  // Use analogWrite defaults (frequency varies by core),
  // but we ensure pins are configured as outputs.
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Start with all off (requirement).
  analogWrite(LED1_PIN, 0);
  analogWrite(LED2_PIN, 0);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW);
  }
}

void setup() {
  setupPWMDefaults();

  // Pots are analog inputs; default INPUT is fine.
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(POT3_PIN, INPUT);
}

void loop() {
  int pot1 = analogRead(POT1_PIN); // 0..1023
  int pot2 = analogRead(POT2_PIN); // 0..1023
  int pot3 = analogRead(POT3_PIN); // 0..1023

  // 1) Map pot1 uniformly from 0..255
  uint8_t led1Brightness = (uint8_t)map(pot1, 0, 1023, 0, 255);

  // 2) Map pot2 uniformly from 0..255
  uint8_t led2Brightness = (uint8_t)map(pot2, 0, 1023, 0, 255);

  // 3) Map pot3 uniformly from 0..10 (segments count)
  int segCount = map(pot3, 0, 1023, 0, 10);
  if (segCount < 0) segCount = 0;
  if (segCount > 10) segCount = 10;

  // Update continuously
  analogWrite(LED1_PIN, led1Brightness);
  analogWrite(LED2_PIN, led2Brightness);

  for (uint8_t i = 0; i < 10; i++) {
    if ((int)i < segCount) digitalWrite(BAR_A_PINS[i], HIGH);
    else digitalWrite(BAR_A_PINS[i], LOW);
  }
}