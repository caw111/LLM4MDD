#include <Arduino.h>

// Arduino Mega 2560: preserve all supplied wiring.
constexpr uint8_t LED_PIN = 2;
constexpr uint8_t RGB_RED_PIN = 3;
constexpr uint8_t RGB_GREEN_PIN = 4;
constexpr uint8_t RGB_BLUE_PIN = 5;

constexpr uint8_t DATA_PIN = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;

constexpr unsigned long INTERVAL_MS = 2000UL;

// Segment bits: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G.
// Display commons are wired to +5V, so segments are active LOW.
// Q7 is unused and is kept HIGH.
constexpr uint8_t DIGIT_PATTERNS[10] = {
  0xC0, // 0
  0xF9, // 1
  0xA4, // 2
  0xB0, // 3
  0x99, // 4
  0x92, // 5
  0x82, // 6
  0xF8, // 7
  0x80, // 8
  0x90  // 9
};

// Common-cathode RGB LED: HIGH turns a channel on.
constexpr uint8_t COLORS[7][3] = {
  {HIGH, LOW,  LOW }, // Red
  {LOW,  HIGH, LOW }, // Green
  {LOW,  LOW,  HIGH}, // Blue
  {HIGH, HIGH, LOW }, // Yellow
  {LOW,  HIGH, HIGH}, // Cyan
  {HIGH, LOW,  HIGH}, // Magenta
  {HIGH, HIGH, HIGH}  // White
};

bool ledOn = false;
uint8_t colorIndex = 0;
uint8_t count = 0;
unsigned long lastUpdate = 0;

void showColor(uint8_t index) {
  digitalWrite(RGB_RED_PIN, COLORS[index][0]);
  digitalWrite(RGB_GREEN_PIN, COLORS[index][1]);
  digitalWrite(RGB_BLUE_PIN, COLORS[index][2]);
}

void showNumber(uint8_t number) {
  const uint8_t tens = number / 10;
  const uint8_t units = number % 10;

  digitalWrite(LATCH_PIN, LOW);

  // First byte propagates to sr2 (units); second stays in sr1 (tens).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[units]);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[tens]);

  // Update both displays simultaneously.
  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(RGB_RED_PIN, LOW);
  digitalWrite(RGB_GREEN_PIN, LOW);
  digitalWrite(RGB_BLUE_PIN, LOW);
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  showNumber(count);       // Start at 00.
  showColor(colorIndex);   // Start with red.
  lastUpdate = millis();
}

void loop() {
  const unsigned long now = millis();

  // Unsigned subtraction handles millis() rollover. Advancing the
  // reference by the interval prevents cumulative scheduling drift.
  if (now - lastUpdate >= INTERVAL_MS) {
    lastUpdate += INTERVAL_MS;

    ledOn = !ledOn;
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);

    colorIndex = (colorIndex + 1) % 7;
    showColor(colorIndex);

    count = (count + 1) % 100;
    showNumber(count);
  }
}