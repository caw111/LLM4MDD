#include <Arduino.h>

// --- Pin mapping from provided wiring (Arduino Mega 2560) ---
// RGB LED rgb1 (common cathode)
const uint8_t RGB_R_PIN = 2;
const uint8_t RGB_G_PIN = 3;
const uint8_t RGB_B_PIN = 4;

// Slide potentiometer pot1
const uint8_t POT_SIG_PIN = A3;

// 10-segment bar graph bargraph1
// bargraph1 has 10 anodes on A1..A10 and common cathode on C1..C10 to GND.1
const uint8_t BAR_PINS[10] = {A1, A2, A3, A4, A5, A6, A7, A8, A9, A10};

// Mode durations
static const uint32_t MODE_MS = 5000;

// Mode indices: 0=Red, 1=Green, 2=Blue
uint8_t modeIndex = 0;

uint16_t redVal = 0;   // 0..1023
uint16_t greenVal = 0; // 0..1023
uint16_t blueVal = 0;  // 0..1023

uint16_t readPot10bit() {
  return analogRead(POT_SIG_PIN); // 0..1023
}

uint8_t scale10bitTo8bit(uint16_t v10) {
  // Map 0..1023 to 0..255
  return (uint8_t)((uint32_t)v10 * 255UL / 1023UL);
}

void applyRgb() {
  // Common cathode: higher PWM value turns component brighter.
  analogWrite(RGB_R_PIN, scale10bitTo8bit(redVal));
  analogWrite(RGB_G_PIN, scale10bitTo8bit(greenVal));
  analogWrite(RGB_B_PIN, scale10bitTo8bit(blueVal));
}

void applyBarGraph(uint16_t pot10) {
  // 0..1023 -> 0..10 segments
  uint8_t segments = (uint8_t)((uint32_t)pot10 * 10UL / 1023UL); // 0..10
  if (segments > 10) segments = 10;

  // Bar graph is common cathode (wired to GND.1). Drive each segment anode.
  // Light segments from 1..segments inclusive.
  for (uint8_t i = 0; i < 10; i++) {
    if (i < segments) {
      digitalWrite(BAR_PINS[i], HIGH);
    } else {
      digitalWrite(BAR_PINS[i], LOW);
    }
  }
}

void setModePins(uint8_t idx) {
  // Ensure "off state at power-on/reset":
  // done by initial values set to 0; pins are configured and PWM/outputs applied.
  // This function intentionally does nothing else because persistence is required.
  (void)idx;
}

void setup() {
  // Initialize analog input
  pinMode(POT_SIG_PIN, INPUT);

  // Initialize RGB pins (PWM-capable pins on Mega for proper brightness control)
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Initialize bar graph segment pins
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW); // off at start
  }

  // All components off at power-on/reset
  redVal = 0;
  greenVal = 0;
  blueVal = 0;

  modeIndex = 0;
  setModePins(modeIndex);

  // Apply initial states
  applyRgb();
  applyBarGraph(0);
}

void loop() {
  static uint32_t modeStartMs = 0;

  if (modeStartMs == 0) {
    modeStartMs = millis();
  }

  // Read potentiometer continuously for bar graph and current active component
  uint16_t pot10 = readPot10bit();

  // Update bar graph continuously according to potentiometer value
  applyBarGraph(pot10);

  // Update only the currently active RGB component intensity, preserving others
  switch (modeIndex) {
    case 0: redVal = pot10; break;    // Red mode
    case 1: greenVal = pot10; break;  // Green mode
    case 2: blueVal = pot10; break;   // Blue mode
  }

  // Apply combined RGB color (current mode component + persisted other components)
  applyRgb();

  // Cycle modes every exactly 5 seconds
  if (millis() - modeStartMs >= MODE_MS) {
    modeIndex = (modeIndex + 1) % 3;
    modeStartMs += MODE_MS; // keep exact 5s boundaries stable
  }
}