#include <Arduino.h>
#include <Servo.h>

// Arduino Mega 2560 wiring: common-cathode RGB LEDs.
const uint8_t RGB_PINS[3][3] = {
  {2, 3, 4},
  {5, 6, 7},
  {8, 9, 10}
};

// RGB channel states: Red, Green, Cyan, Magenta, Blue, Yellow, White.
const uint8_t COLORS[7][3] = {
  {HIGH, LOW,  LOW},
  {LOW,  HIGH, LOW},
  {LOW,  HIGH, HIGH},
  {HIGH, LOW,  HIGH},
  {LOW,  LOW,  HIGH},
  {HIGH, HIGH, LOW},
  {HIGH, HIGH, HIGH}
};

const uint8_t DATA_PIN  = A0;  // 74HC595 DS
const uint8_t LATCH_PIN = A1;  // 74HC595 STCP
const uint8_t CLOCK_PIN = A2;  // 74HC595 SHCP

// Bits 0..6 correspond to segments A..G; bit 7 is unused.
// The display COM is wired to +5V, so segments are active LOW.
const uint8_t DIGIT_PATTERNS[10] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F  // 9
};

const uint8_t SERVO_PINS[3] = {11, 12, 13};
Servo servos[3];

const unsigned long STATE_DURATION_MS = 2000UL;
unsigned long lastStateTime = 0;
uint8_t colorIndex = 0;
uint8_t countdown = 9;

void showDigit(uint8_t digit) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           static_cast<uint8_t>(~DIGIT_PATTERNS[digit]));
  digitalWrite(LATCH_PIN, HIGH);
}

void applyState() {
  for (uint8_t led = 0; led < 3; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(RGB_PINS[led][channel], COLORS[colorIndex][channel]);
    }
  }

  showDigit(countdown);

  uint8_t angle;
  if (countdown >= 7) {
    angle = 0;
  } else if (countdown >= 4) {
    angle = 60;
  } else if (countdown >= 1) {
    angle = 120;
  } else {
    angle = 180;
  }

  for (uint8_t i = 0; i < 3; ++i) {
    servos[i].write(angle);
  }
}

void setup() {
  for (uint8_t led = 0; led < 3; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      pinMode(RGB_PINS[led][channel], OUTPUT);
      digitalWrite(RGB_PINS[led][channel], LOW);
    }
  }

  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);

  for (uint8_t i = 0; i < 3; ++i) {
    servos[i].write(0);
    servos[i].attach(SERVO_PINS[i]);
  }

  applyState();
  lastStateTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastStateTime >= STATE_DURATION_MS) {
    lastStateTime += STATE_DURATION_MS;

    // Both cycles advance on the same clock, repeating independently.
    colorIndex = (colorIndex + 1) % 7;
    countdown = (countdown == 0) ? 9 : countdown - 1;

    applyState();
  }
}