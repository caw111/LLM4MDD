// Arduino Mega 2560 sketch
// Controls led1/led2/led3 (single-color) and rgb1/rgb2 (RGB) using pot1/pot2.

const uint8_t LED1_PIN = 2;   // led1 -> mega pin 2
const uint8_t LED2_PIN = 3;   // led2 -> mega pin 3
const uint8_t LED3_PIN = 4;   // led3 -> mega pin 4

const uint8_t RGB1_R_PIN = 5; // rgb1:R -> mega pin 5
const uint8_t RGB1_G_PIN = 6; // rgb1:G -> mega pin 6
const uint8_t RGB1_B_PIN = 7; // rgb1:B -> mega pin 7

const uint8_t RGB2_R_PIN = 8;  // rgb2:R -> mega pin 8
const uint8_t RGB2_G_PIN = 9;  // rgb2:G -> mega pin 9
const uint8_t RGB2_B_PIN = 10; // rgb2:B -> mega pin 10

const uint8_t POT1_PIN = A3;   // pot1:SIG -> A3
const uint8_t POT2_PIN = A4;   // pot2:SIG -> A4

static inline uint8_t map1023To255(int v) {
  if (v <= 0) return 0;
  if (v >= 1023) return 255;
  return (uint8_t)((v * 255L) / 1023L);
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

  // Ensure LEDs start off
  analogWrite(LED1_PIN, 0);
  analogWrite(LED2_PIN, 0);
  analogWrite(LED3_PIN, 0);

  analogWrite(RGB1_R_PIN, 0);
  analogWrite(RGB1_G_PIN, 0);
  analogWrite(RGB1_B_PIN, 0);

  analogWrite(RGB2_R_PIN, 0);
  analogWrite(RGB2_G_PIN, 0);
  analogWrite(RGB2_B_PIN, 0);
}

void loop() {
  int pot1 = analogRead(POT1_PIN); // 0..1023
  int pot2 = analogRead(POT2_PIN); // 0..1023

  uint8_t led1Brightness = map1023To255(pot1);
  uint8_t led2Brightness = map1023To255(pot2);

  // led1 brightness controlled by pot1, led2 brightness controlled by pot2
  analogWrite(LED1_PIN, led1Brightness);
  analogWrite(LED2_PIN, led2Brightness);

  // If both pot1 and pot2 are at maximum value (1023), led3 should be on.
  // Otherwise, led3 should be off (no other brightness rule specified).
  if (pot1 == 1023 && pot2 == 1023) {
    analogWrite(LED3_PIN, 255);
  } else {
    analogWrite(LED3_PIN, 0);
  }

  // RGB color: red controlled by pot1, green controlled by pot2, blue fixed to 0.
  uint8_t redVal = map1023To255(pot1);
  uint8_t greenVal = map1023To255(pot2);
  uint8_t blueVal = 0;

  // Update both RGB LEDs with the same gradient
  analogWrite(RGB1_R_PIN, redVal);
  analogWrite(RGB1_G_PIN, greenVal);
  analogWrite(RGB1_B_PIN, blueVal);

  analogWrite(RGB2_R_PIN, redVal);
  analogWrite(RGB2_G_PIN, greenVal);
  analogWrite(RGB2_B_PIN, blueVal);
}