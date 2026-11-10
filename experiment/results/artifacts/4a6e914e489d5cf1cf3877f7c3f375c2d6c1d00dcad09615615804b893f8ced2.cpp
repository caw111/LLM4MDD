#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Shift register (74HC595) pins as per wiring
static const uint8_t PIN_SHCP = A2;  // sr1:SHCP (gray) -> mega:A2
static const uint8_t PIN_STCP = A1;  // sr1:STCP (purple) -> mega:A1
static const uint8_t PIN_DS   = A0;    // sr1:DS (blue) -> mega:A0

// 74HC595 control wiring per circuit
// sr1:MR is tied to VCC (no manual reset needed)
// sr1:OE is tied to GND (always enabled)

static const uint8_t LED1_PIN = 2;  // led1:A -> mega:2
static const uint8_t LED2_PIN = 3;  // led2:A -> mega:3

// Countdown and blinking timing
static const unsigned long LED_INTERVAL_MS = 2000UL;     // 2 seconds
static const unsigned long COUNT_INTERVAL_MS = 4000UL;   // 4 seconds

// Segment bit mapping for Q0..Q6 -> A..G
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// We'll shift out bits [Q7..Q0] using DS, then latch with STCP.
// Wiring uses Q0..Q6 only; Q7 is unused (set to 0).
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitToSegments(unsigned int d);
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWriteByte(uint8_t value);
#line 90 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showDigit(unsigned int d);
#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitToSegments(unsigned int d) {
  // Common wiring assumption: each segment output from shift register turns the segment on
  // (i.e., segments are active-high). Numbers:
  // 0: A B C D E F (G off)
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  bool A=false, B=false, C=false, D=false, E=false, F=false, G=false;

  switch (d) {
    case 0: A=true; B=true; C=true; D=true; E=true; F=true; G=false; break;
    case 1: A=false; B=true; C=true; D=false; E=false; F=false; G=false; break;
    case 2: A=true; B=true; C=false; D=true; E=true; F=false; G=true; break;
    case 3: A=true; B=true; C=true; D=true; E=false; F=false; G=true; break;
    case 4: A=false; B=true; C=true; D=false; E=false; F=true; G=true; break;
    case 5: A=true; B=false; C=true; D=true; E=false; F=true; G=true; break;
    case 6: A=true; B=false; C=true; D=true; E=true; F=true; G=true; break;
    case 7: A=true; B=true; C=true; D=false; E=false; F=false; G=false; break;
    case 8: A=true; B=true; C=true; D=true; E=true; F=true; G=true; break;
    case 9: A=true; B=true; C=true; D=true; E=false; F=true; G=true; break;
    default: A=false; B=false; C=false; D=false; E=false; F=false; G=false; break;
  }

  // Build byte with Q0..Q6 mapped to A..G respectively:
  // bit0 = Q0 = A
  // bit1 = Q1 = B
  // bit2 = Q2 = C
  // bit3 = Q3 = D
  // bit4 = Q4 = E
  // bit5 = Q5 = F
  // bit6 = Q6 = G
  uint8_t b = 0;
  if (A) b |= (1u << 0);
  if (B) b |= (1u << 1);
  if (C) b |= (1u << 2);
  if (D) b |= (1u << 3);
  if (E) b |= (1u << 4);
  if (F) b |= (1u << 5);
  if (G) b |= (1u << 6);
  // bit7 left as 0 (Q7 unused)
  return b;
}

static void srWriteByte(uint8_t value) {
  // Shift out MSB first (bit7 -> bit0) across DS -> shift register,
  // then latch to outputs using STCP.
  digitalWrite(PIN_SHCP, LOW);
  digitalWrite(PIN_STCP, LOW);

  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(PIN_DS, (value & (1u << i)) ? HIGH : LOW);
    // Rising edge on SHCP clocks the bit into the register.
    digitalWrite(PIN_SHCP, HIGH);
    digitalWrite(PIN_SHCP, LOW);
  }

  // Rising edge on STCP latches the shifted data to Q outputs.
  digitalWrite(PIN_STCP, HIGH);
  digitalWrite(PIN_STCP, LOW);
}

static void showDigit(unsigned int d) {
  uint8_t segments = digitToSegments(d);
  srWriteByte(segments);
}

void setup() {
  // LED pins
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW); // both off on reset
  digitalWrite(LED2_PIN, LOW);

  // Shift register pins
  pinMode(PIN_DS, OUTPUT);
  pinMode(PIN_SHCP, OUTPUT);
  pinMode(PIN_STCP, OUTPUT);

  digitalWrite(PIN_DS, LOW);
  digitalWrite(PIN_SHCP, LOW);
  digitalWrite(PIN_STCP, LOW);

  // Initial display: show "9" on power-up/reset
  showDigit(9);
}

void loop() {
  static unsigned long lastLedToggleMs = 0;
  static unsigned long lastCountMs = 0;

  static bool led1On = false;     // start with both off, so first toggle turns led1 on
  static uint8_t countValue = 9; // countdown starts from 9

  const unsigned long now = millis();

  // LED alternating blink every 2 seconds: led1 -> led2 -> led1 -> led2...
  if (now - lastLedToggleMs >= LED_INTERVAL_MS) {
    lastLedToggleMs = now;

    // Toggle between led1 and led2
    led1On = !led1On;

    if (led1On) {
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, LOW);
    } else {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, HIGH);
    }
  }

  // Countdown decrement every 4 seconds
  if (now - lastCountMs >= COUNT_INTERVAL_MS) {
    lastCountMs = now;

    if (countValue == 0) {
      countValue = 9; // reset when reaching 0
    } else {
      countValue--;     // decrement
    }

    // Update display immediately after decrement/reset decision
    showDigit(countValue);
  }
}
