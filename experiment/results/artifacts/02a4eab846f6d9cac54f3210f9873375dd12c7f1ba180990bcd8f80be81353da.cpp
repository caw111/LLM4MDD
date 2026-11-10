#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t SR_DATA_PIN  = A0; // DS
static const uint8_t SR_SHCP_PIN  = A1; // STCP
static const uint8_t SR_OE_PIN    = A0; // OE is tied to SR itself in circuit; SR uses MR? (not used here)

static const uint8_t SR_LATCH_PIN = A1;
static const uint8_t SR_CLOCK_PIN = A2;
static const uint8_t SR_DATA_OUT_PIN = A0;

static const uint8_t POT1_PIN = A3;
static const uint8_t POT2_PIN = A4;
static const uint8_t POT3_PIN = A5;

// 74HC595 bit mapping to 7-seg pins (from wiring):
// sr1:Q0 -> A
// sr1:Q1 -> B
// sr1:Q2 -> C
// sr1:Q3 -> D
// sr1:Q4 -> E
// sr1:Q5 -> F
// sr1:Q6 -> G
// sr1:Q7 -> (unused)
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitTo7SegMask(uint8_t digit);
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void pulsePin(uint8_t pin);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOut595(uint8_t value);
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t readPot0to1023(uint8_t pin);
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 112 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitTo7SegMask(uint8_t digit) {
  // Common-cathode assumptions (typical Wokwi sevseg): segment ON = 1
  // Segments: A B C D E F G
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
  switch (digit) {
    case 0: return (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);                 // A B C D E F
    case 1: return (1<<1) | (1<<2);                                                       // B C
    case 2: return (1<<0) | (1<<1) | (1<<3) | (1<<4) | (1<<6);                            // A B D E G
    case 3: return (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<6);                            // A B C D G
    case 4: return (1<<1) | (1<<2) | (1<<5) | (1<<6);                                     // B C F G
    case 5: return (1<<0) | (1<<2) | (1<<3) | (1<<5) | (1<<6);                            // A C D F G
    case 6: return (1<<0) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6);                 // A C D E F G
    case 7: return (1<<0) | (1<<1) | (1<<2);                                              // A B C
    case 8: return (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6);      // A B C D E F G
    case 9: return (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<5) | (1<<6);                 // A B C D F G
    default: return 0;
  }
}

static void pulsePin(uint8_t pin) {
  digitalWrite(pin, LOW);
  delayMicroseconds(1);
  digitalWrite(pin, HIGH);
  delayMicroseconds(1);
}

static void shiftOut595(uint8_t value) {
  // Uses DS (data), SHCP (shift clock), STCP (latch)
  // ds/clock/latch mapping from circuit:
  // mega:A0 -> sr1:DS
  // mega:A2 -> sr1:SHCP
  // mega:A1 -> sr1:STCP
  digitalWrite(SR_LATCH_PIN, LOW);
  // shift out MSB first to Q0..Q7 correspondence? Wokwi wires:
  // Q0 corresponds to least significant output after MSB-first shiftOut.
  // To be safe, we will use shift-right approach with MSB-first by placing desired bit positions.
  // We'll transmit MSB first; hardware shifts into internal register such that Q0 becomes bit0 after 8 clocks.
  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(SR_DATA_OUT_PIN, (value & (1u << i)) ? HIGH : LOW);
    pulsePin(SR_CLOCK_PIN);
  }
  digitalWrite(SR_LATCH_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_LATCH_PIN, LOW);
}

static uint8_t readPot0to1023(uint8_t pin) {
  return analogRead(pin) & 0xFF; // not used directly
}

void setup() {
  // Use real Arduino Mega pin mapping from circuit:
  // 74HC595:
  // mega:A2 -> SHCP
  // mega:A1 -> STCP
  // mega:A0 -> DS
  pinMode(SR_CLOCK_PIN, OUTPUT); // A2
  pinMode(SR_LATCH_PIN, OUTPUT); // A1
  pinMode(SR_DATA_OUT_PIN, OUTPUT); // A0

  // LED bar graph controlled by direct pins:
  // mega:2..11 -> bargraph A1..A10
  // and all C1..C10 tied to mega:GND.1
  for (uint8_t p = 2; p <= 11; ++p) pinMode(p, OUTPUT);

  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(POT3_PIN, INPUT);

  // Start with digit 0 displayed off initially, bargraph off.
  for (uint8_t p = 2; p <= 11; ++p) digitalWrite(p, LOW);
  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);
  digitalWrite(SR_DATA_OUT_PIN, LOW);

  // Display 0 on 7-seg
  shiftOut595(digitTo7SegMask(0));
}

void loop() {
  // Read potentiometers continuously
  int pot1Raw = analogRead(POT1_PIN); // 0..1023 -> brightness 0..255
  int pot2Raw = analogRead(POT2_PIN); // speed component
  int pot3Raw = analogRead(POT3_PIN); // speed component

  uint8_t brightness = (uint8_t)map(constrain(pot1Raw, 0, 1023), 0, 1023, 0, 255);

  // Digit and update timing:
  // Rule: digit increments every 2 seconds by default, but speed is controlled by pot2 and pot3:
  // "speed range 1 second to 2 seconds" for each, final speed = sum of pot2 and pot3.
  // We'll interpret "final speed" as milliseconds between digit changes.
  // So: t1 = 1..2s from pot2, t2 = 1..2s from pot3, total = t1+t2 (2..4s).
  float t1 = map(constrain(pot2Raw, 0, 1023), 0, 1023, 1000, 2000) / 1000.0f; // seconds
  float t2 = map(constrain(pot3Raw, 0, 1023), 0, 1023, 1000, 2000) / 1000.0f; // seconds
  uint32_t intervalMs = (uint32_t)((t1 + t2) * 1000.0f);

  static uint8_t digit = 0;
  static uint32_t lastChange = millis();

  // Update digit based on interval that can change dynamically with pots
  uint32_t now = millis();
  if ((uint32_t)(now - lastChange) >= intervalMs) {
    digit = (digit + 1) % 10;
    lastChange = now;
  }

  // Update 7-segment via 74HC595
  shiftOut595(digitTo7SegMask(digit));

  // Bargraph: number of lit segments = digit
  // Brightness controlled by pot1: since bargraph uses individual A pins (2..11) without PWM hardware,
  // emulate brightness with software PWM on those pins.
  //
  // We'll use a simple time-sliced PWM over a small period.
  // The bargraph "lit segments" are the first 'digit' outputs (A1..Adigit).
  // Digit=0 -> all off.
  const uint8_t segmentsToLight = digit;

  // Software PWM:
  // period 256 ticks -> use millis granularity (good enough for Wokwi).
  // Use micros for better resolution.
  static uint32_t pwmPeriodMicros = 2000; // 2ms period -> ~500Hz
  static uint32_t pwmStart = micros();
  uint32_t t = micros() - pwmStart;
  if (t >= pwmPeriodMicros) {
    pwmStart = micros();
    t = 0;
  }
  uint8_t pwmPhase = (uint8_t)((t * 255UL) / pwmPeriodMicros);

  bool barOn = pwmPhase < brightness;

  for (uint8_t i = 0; i < 10; ++i) {
    uint8_t pin = 2 + i; // A1..A10
    if (i < segmentsToLight && barOn) digitalWrite(pin, HIGH);
    else digitalWrite(pin, LOW);
  }
}
