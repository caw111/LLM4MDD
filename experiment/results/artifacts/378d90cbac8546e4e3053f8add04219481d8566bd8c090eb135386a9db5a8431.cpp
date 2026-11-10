#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_62\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <util/atomic.h>

#if !defined(__AVR_ATmega2560__)
#error "This sketch requires an Arduino Mega 2560."
#endif

// Cascaded 74HC595 wiring: Mega -> sr1 (tens) -> sr2 (ones).
constexpr uint8_t DATA_PIN  = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;

constexpr uint8_t RGB_PINS[] = {2, 3, 4, 5, 6, 7};
constexpr uint8_t BAR_PINS[] = {8, 9, 10, 11, 12, 13, 44, 45};

// A9 and A10 of the bar graph are not wired and remain unlit.
// At most the first eight wired segments are illuminated.

// Bits 0..6 correspond to segments A..G.
// Displays have their common terminals connected to +5V:
// segment outputs are active LOW.
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

constexpr unsigned long STEP_INTERVAL_MS = 2000UL;

uint8_t currentNumber = 0;
unsigned long lastStepTime = 0;

struct OutputFrame {
  uint8_t portB;
  uint8_t portE;
  uint8_t portG;
  uint8_t portH;
  uint8_t portL;
};

#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_62\\direct\\attempt_0\\sketch\\sketch.ino"
OutputFrame makeFrame(uint8_t number, bool initialState);
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_62\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 137 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_62\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_62\\direct\\attempt_0\\sketch\\sketch.ino"
OutputFrame makeFrame(uint8_t number, bool initialState) {
  OutputFrame frame = {0, 0, 0, 0, 0};

  uint8_t litSegments = number / 10;
  if (litSegments > 8) {
    litSegments = 8;
  }

  // Bar graph A1..A8: D8,D9,D10,D11,D12,D13,D44,D45.
  if (litSegments >= 1) frame.portH |= _BV(PH5);
  if (litSegments >= 2) frame.portH |= _BV(PH6);
  if (litSegments >= 3) frame.portB |= _BV(PB4);
  if (litSegments >= 4) frame.portB |= _BV(PB5);
  if (litSegments >= 5) frame.portB |= _BV(PB6);
  if (litSegments >= 6) frame.portB |= _BV(PB7);
  if (litSegments >= 7) frame.portL |= _BV(PL5);
  if (litSegments >= 8) frame.portL |= _BV(PL4);

  // Initialization explicitly requires both RGB LEDs to be off.
  // Subsequent updates, including the rollover to 00, follow the ranges.
  if (!initialState) {
    if (number <= 5) {
      frame.portE |= _BV(PE4) | _BV(PE3); // D2, D5: red
    } else if (number <= 9) {
      frame.portE |= _BV(PE5);            // D3: green
      frame.portH |= _BV(PH3);            // D6: green
    } else {
      frame.portG |= _BV(PG5);            // D4: blue
      frame.portH |= _BV(PH4);            // D7: blue
    }
  }

  return frame;
}

void updateOutputs(uint8_t number, bool initialState = false) {
  const OutputFrame frame = makeFrame(number, initialState);

  const uint8_t tensPattern =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[number / 10]);
  const uint8_t onesPattern =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[number % 10]);

  // Stage both digits without changing the visible register outputs.
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, onesPattern);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, tensPattern);

  // Commit all visible changes together in a short, uninterrupted block.
  // Different GPIO ports cannot physically switch on the same CPU cycle;
  // direct port writes minimize this skew to a few microseconds.
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    PORTB = (PORTB & static_cast<uint8_t>(~0xF0)) | frame.portB;
    PORTE = (PORTE & static_cast<uint8_t>(
        ~(_BV(PE3) | _BV(PE4) | _BV(PE5)))) | frame.portE;
    PORTG = (PORTG & static_cast<uint8_t>(~_BV(PG5))) | frame.portG;
    PORTH = (PORTH & static_cast<uint8_t>(
        ~(_BV(PH3) | _BV(PH4) | _BV(PH5) | _BV(PH6)))) | frame.portH;
    PORTL = (PORTL & static_cast<uint8_t>(
        ~(_BV(PL4) | _BV(PL5)))) | frame.portL;

    PORTF |= _BV(PF1); // A1: latch both display digits simultaneously.
  }
}

void setup() {
  // Set output latches LOW before enabling output drivers.
  for (uint8_t pin : RGB_PINS) {
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
  }

  for (uint8_t pin : BAR_PINS) {
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
  }

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  currentNumber = 0;
  updateOutputs(currentNumber, true);
  lastStepTime = millis();
}

void loop() {
  const unsigned long now = millis();

  // Unsigned subtraction remains correct across millis() rollover.
  if (now - lastStepTime >= STEP_INTERVAL_MS) {
    lastStepTime += STEP_INTERVAL_MS;
    currentNumber = (currentNumber + 1) % 100;
    updateOutputs(currentNumber);
  }
}
