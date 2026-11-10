#include <Arduino.h>

const uint8_t LED1_PIN = 2;   // led1 -> mega:2
const uint8_t LED2_PIN = 3;   // led2 -> mega:3
const uint8_t LED3_PIN = 4;   // led3 -> mega:4

// RGB1 pins: mega:5 (R), mega:6 (G), mega:7 (B)
const uint8_t RGB1_R_PIN = 5;
const uint8_t RGB1_G_PIN = 6;
const uint8_t RGB1_B_PIN = 7;

// RGB2 pins: mega:8 (R), mega:9 (G), mega:10 (B)
const uint8_t RGB2_R_PIN = 8;
const uint8_t RGB2_G_PIN = 9;
const uint8_t RGB2_B_PIN = 10;

// Potentiometers
const uint8_t POT1_PIN = A3;  // pot1:SIG -> mega:A3
const uint8_t POT2_PIN = A4;  // pot2:SIG -> mega:A4

static uint8_t clampTo255(long x) {
  if (x < 0) return 0;
  if (x > 255) return 255;
  return (uint8_t)x;
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);

  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  // Pots use external wiring to 5V/GND; read analog signals.
}

void loop() {
  int pot1 = analogRead(POT1_PIN); // 0..1023
  int pot2 = analogRead(POT2_PIN); // 0..1023

  // Single-color LEDs brightness
  // Map pot values to 0..255
  uint8_t led1Brightness = (uint8_t)clampTo255((long)pot1 * 255L / 1023L);
  uint8_t led2Brightness = (uint8_t)clampTo255((long)pot2 * 255L / 1023L);

  analogWrite(LED1_PIN, led1Brightness);
  analogWrite(LED2_PIN, led2Brightness);

  // led3 on only when both at maximum
  bool pot1Max = (pot1 >= 1023);
  bool pot2Max = (pot2 >= 1023);
  analogWrite(LED3_PIN, (pot1Max && pot2Max) ? 255 : 0);

  // RGB color gradient from combined pot values:
  // red controlled by pot1, green controlled by pot2
  // blue follows the "combined values gradient": set to 0, but ensure
  // white at max/max by making blue depend on both reaching max.
  //
  // Requirements specify red=pot1, green=pot2, and white when both max.
  // A simple mapping that satisfies that:
  //   R = map(pot1)
  //   G = map(pot2)
  //   B = map(min(pot1,pot2)) so at max/max it becomes 255
  uint8_t r = (uint8_t)clampTo255((long)pot1 * 255L / 1023L);
  uint8_t g = (uint8_t)clampTo255((long)pot2 * 255L / 1023L);
  int combined = (pot1 < pot2) ? pot1 : pot2; // min(pot1, pot2)
  uint8_t b = (uint8_t)clampTo255((long)combined * 255L / 1023L);

  // Apply same color gradient to both RGB LEDs
  analogWrite(RGB1_R_PIN, r);
  analogWrite(RGB1_G_PIN, g);
  analogWrite(RGB1_B_PIN, b);

  analogWrite(RGB2_R_PIN, r);
  analogWrite(RGB2_G_PIN, g);
  analogWrite(RGB2_B_PIN, b);
}