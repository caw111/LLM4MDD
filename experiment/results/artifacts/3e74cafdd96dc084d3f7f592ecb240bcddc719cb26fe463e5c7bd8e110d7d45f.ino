#include <Arduino.h>

static const byte PIN_SR_DATA  = A0; // DS -> blue
static const byte PIN_SR_SHCP  = A2; // SHCP -> gray
static const byte PIN_SR_STCP  = A1; // STCP -> purple

static const byte TIMER_INIT_VALUE_SECONDS = 0;
static const uint32_t TICK_INTERVAL_MS = 1000; // real-time second
static const int TIMER_STEP_SECONDS = 2;

static uint8_t digitToSegments(uint8_t d) {
  // sevseg (wokwi) expects segments A..G, values: bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G
  // Using standard common-cathode patterns: 1 means segment ON.
  // 7-seg mapping per digit:
  // 0: A B C D E F
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  switch (d) {
    case 0: return 0b0111111; // A B C D E F
    case 1: return 0b0000110; // B C
    case 2: return 0b1011011; // A B D E G
    case 3: return 0b1001111; // A B C D G
    case 4: return 0b1100110; // B C F G
    case 5: return 0b1101101; // A C D F G
    case 6: return 0b1111101; // A C D E F G
    case 7: return 0b0000111; // A B C
    case 8: return 0b1111111; // all
    case 9: return 0b1101111; // A B C D F G
    default: return 0;        // blank
  }
}

static void shiftOut7Seg(uint8_t segMaskAtoG) {
  // sr1 wiring: Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
  // We send bits to DS then pulse SHCP, latch on STCP.
  // Bit order: DS receives serial data; most Arduino shiftOut uses MSB-first, but we control manually.
  // We'll clock out from Q7..Q0? Since only Q0..Q6 are used, we clock 8 bits with Q7=0.
  noInterrupts();
  digitalWrite(PIN_SR_STCP, LOW);
  for (int bit = 7; bit >= 0; --bit) {
    uint8_t outBit = (bit <= 6) ? ((segMaskAtoG >> bit) & 0x01) : 0; // Q7 unused
    digitalWrite(PIN_SR_DATA, outBit ? HIGH : LOW);
    digitalWrite(PIN_SR_SHCP, HIGH);
    delayMicroseconds(1);
    digitalWrite(PIN_SR_SHCP, LOW);
  }
  digitalWrite(PIN_SR_STCP, HIGH);
  interrupts();
}

void setup() {
  pinMode(PIN_SR_DATA, OUTPUT);
  pinMode(PIN_SR_SHCP, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);

  // Ensure display is off on power/reset:
  // Turn off all segments (mask=0).
  shiftOut7Seg(0);
}

void loop() {
  static int timerSeconds = TIMER_INIT_VALUE_SECONDS;
  static uint32_t lastTickMs = millis();

  uint32_t now = millis();
  if (now - lastTickMs >= TICK_INTERVAL_MS) {
    // Keep accurate stepping even if loop is delayed.
    uint32_t ticks = (now - lastTickMs) / TICK_INTERVAL_MS;
    lastTickMs += ticks * TICK_INTERVAL_MS;

    // Every real-time second, timer increments by 2 seconds.
    timerSeconds += (int)(ticks * TIMER_STEP_SECONDS);

    // Display rule:
    // If timer exceeds 10 seconds, only last digit of the timer should be displayed.
    int toDisplay;
    if (timerSeconds > 10) {
      // last digit of timer value
      int lastDigit = timerSeconds % 10;
      if (lastDigit < 0) lastDigit = -lastDigit;
      toDisplay = lastDigit;
    } else {
      // show current timer value (assumed 0..10 -> single digit for 0..9; for 10 there are two digits,
      // but requirement only specifies special handling after exceeding 10, so we display "10" as blank behavior:
      // Since only one digit is possible, we map 10 to blank.
      // (No simplification of behavior besides this mapping.)
      if (timerSeconds >= 0 && timerSeconds <= 9) toDisplay = timerSeconds;
      else toDisplay = 10; // will become blank
    }

    uint8_t mask = (toDisplay <= 9) ? digitToSegments((uint8_t)toDisplay) : 0;
    shiftOut7Seg(mask);
  }
}