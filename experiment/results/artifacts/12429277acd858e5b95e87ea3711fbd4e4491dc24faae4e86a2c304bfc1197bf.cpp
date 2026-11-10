#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_101\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring.
constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 3;
constexpr uint8_t DATA_PIN = A0;   // 74HC595 DS
constexpr uint8_t LATCH_PIN = A1;  // 74HC595 STCP
constexpr uint8_t CLOCK_PIN = A2;  // 74HC595 SHCP

constexpr unsigned long LED_INTERVAL_MS = 2000UL;
constexpr unsigned long COUNTDOWN_INTERVAL_MS = 4000UL;

// Bits 0..6 correspond to segments A..G (Q0..Q6).
constexpr uint8_t DIGIT_SEGMENTS[10] = {
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

uint8_t countdownDigit = 9;
bool ledsStarted = false;
bool led1Active = false;
unsigned long lastLedUpdate = 0;
unsigned long lastCountdownUpdate = 0;

#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_101\\direct\\attempt_0\\sketch\\sketch.ino"
void displayDigit(uint8_t digit);
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_101\\direct\\attempt_0\\sketch\\sketch.ino"
void updateLeds();
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_101\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_101\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_101\\direct\\attempt_0\\sketch\\sketch.ino"
void displayDigit(uint8_t digit) {
  // COM is wired to +5 V: common-anode segments are active LOW.
  const uint8_t outputPattern =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[digit]);

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, outputPattern);
  digitalWrite(LATCH_PIN, HIGH);
}

void updateLeds() {
  // Switch the previously active LED off before enabling the other.
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(led1Active ? LED1_PIN : LED2_PIN, HIGH);
}

void setup() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  displayDigit(countdownDigit);

  const unsigned long now = millis();
  lastLedUpdate = now;
  lastCountdownUpdate = now;
}

void loop() {
  const unsigned long now = millis();

  while (now - lastLedUpdate >= LED_INTERVAL_MS) {
    lastLedUpdate += LED_INTERVAL_MS;

    if (!ledsStarted) {
      ledsStarted = true;
      led1Active = true;
    } else {
      led1Active = !led1Active;
    }

    updateLeds();
  }

  while (now - lastCountdownUpdate >= COUNTDOWN_INTERVAL_MS) {
    lastCountdownUpdate += COUNTDOWN_INTERVAL_MS;

    // Display each value, including 0, for one complete interval,
    // then restart at 9 without interrupting the LED sequence.
    countdownDigit = (countdownDigit == 0) ? 9 : countdownDigit - 1;
    displayDigit(countdownDigit);
  }
}
