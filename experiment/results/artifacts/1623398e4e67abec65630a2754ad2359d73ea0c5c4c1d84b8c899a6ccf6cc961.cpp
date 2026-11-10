#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_101\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const int LED1_PIN = 2;
static const int LED2_PIN = 3;

// Shift register pins (74HC595) wired per provided circuit
static const int SR_SHCP = A2; // SHCP -> sr1:SHCP (gray)
static const int SR_STCP = A1; // STCP -> sr1:STCP (purple)
static const int SR_DS   = A0; // DS   -> sr1:DS   (blue)

static const unsigned long LED_INTERVAL_MS = 2000UL; // 2 seconds
static const unsigned long COUNT_INTERVAL_MS = 4000UL; // 4 seconds

// Segment map for sevseg1 driven by 74HC595 outputs (A..G via Q0..Q6)
// Wiring indicates: Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// We'll use common "active-high" assumption: a segment turns on with HIGH.
static const uint8_t digitToSegments[10] = {
  // a, b, c, d, e, f, g
  // 0: a b c d e f = on, g = off
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),
  // 1: b c
  (1 << 1) | (1 << 2),
  // 2: a b d e g
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),
  // 3: a b c d g
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),
  // 4: b c f g
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),
  // 5: a c d f g
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),
  // 6: a c d e f g
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 7: a b c
  (1 << 0) | (1 << 1) | (1 << 2),
  // 8: a b c d e f g
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 9: a b c d f g
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)
};

#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_101\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWrite(uint8_t value);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_101\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayDigit(uint8_t d);
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_101\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_101\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_101\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWrite(uint8_t value)
{
  // Shift out to DS on SR_DS; pulse SHCP; then latch with STCP.
  // Q0..Q6 correspond to bits 0..6. We'll clock MSB..LSB with bit order MSB first.
  // Since we only care about Q0..Q6, we map bits accordingly by sending 8 bits.
  // We'll shift out (MSB->LSB) using value byte.
  shiftOut(SR_DS, SR_SHCP, MSBFIRST, value);

  // Latch
  digitalWrite(SR_STCP, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_STCP, LOW);
}

static void displayDigit(uint8_t d)
{
  if (d > 9) d = 0;
  // Put segment bit pattern into Q0..Q6 (bits 0..6). Q7 is unused; keep 0.
  uint8_t pattern = digitToSegments[d] & 0x7F;
  srWrite(pattern);
}

void setup()
{
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  pinMode(SR_SHCP, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_DS, OUTPUT);

  digitalWrite(SR_SHCP, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_DS, LOW);

  // Per requirements:
  // Upon powering on or resetting, show "9" and turn both LEDs off.
  displayDigit(9);

  // Start timers so behavior begins immediately per requirements.
}

void loop()
{
  static bool led1On = false; // begin with both off; first blink will be led1
  static unsigned long lastLedToggleMs = 0;
  static unsigned long lastCountMs = 0;

  static uint8_t countdown = 9;

  const unsigned long now = millis();

  // Countdown: decrement every 4 seconds, update immediately after decrement
  if (now - lastCountMs >= COUNT_INTERVAL_MS) {
    lastCountMs += COUNT_INTERVAL_MS;

    if (countdown == 0) {
      countdown = 9; // reset condition
    } else {
      countdown--; // decrement
    }

    displayDigit(countdown);
  }

  // LED blinking: alternate with 2-second interval, sequence led1 -> led2 -> ...
  if (now - lastLedToggleMs >= LED_INTERVAL_MS) {
    lastLedToggleMs += LED_INTERVAL_MS;

    // If both were off at start, first toggle turns on led1.
    // Then alternate each interval.
    led1On = !led1On;

    if (led1On) {
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, LOW);
    } else {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, HIGH);
    }
  }
}
