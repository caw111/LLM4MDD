#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_105\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560: preserve all supplied wiring.
constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 3;
constexpr uint8_t LED3_PIN = 4;

constexpr uint8_t DATA_PIN  = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;
constexpr uint8_t POT_PIN   = A3;

constexpr unsigned long MIN_STATE_TIME_MS = 2000UL;

// Bits 0..6 correspond to segments A..G.
// The displays are common-anode, so these patterns are inverted when sent.
const uint8_t DIGIT_SEGMENTS[10] = {
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

unsigned long stateStartedAt = 0;
uint8_t currentNumber = 0;
uint8_t currentBrightness = 0;
bool initialState = true;

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_105\\direct\\attempt_0\\sketch\\sketch.ino"
void showNumber(uint8_t number);
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_105\\direct\\attempt_0\\sketch\\sketch.ino"
void applyState(uint8_t number, uint8_t brightness);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_105\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_105\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_105\\direct\\attempt_0\\sketch\\sketch.ino"
void showNumber(uint8_t number) {
  const uint8_t tens = number / 10;
  const uint8_t units = number % 10;

  digitalWrite(LATCH_PIN, LOW);

  // The first byte travels through sr1 into sr2 (units).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           static_cast<uint8_t>(~DIGIT_SEGMENTS[units]));
  // The second byte remains in sr1 (tens).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           static_cast<uint8_t>(~DIGIT_SEGMENTS[tens]));

  digitalWrite(LATCH_PIN, HIGH);
}

void applyState(uint8_t number, uint8_t brightness) {
  showNumber(number);

  analogWrite(LED1_PIN, brightness);
  analogWrite(LED2_PIN, number >= 34 ? brightness : 0);
  analogWrite(LED3_PIN, number >= 67 ? brightness : 0);
}

void setup() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  analogWrite(LED1_PIN, 0);
  analogWrite(LED2_PIN, 0);
  analogWrite(LED3_PIN, 0);
  showNumber(0);

  // Keep the required startup state (all LEDs off, "00") for two seconds.
  stateStartedAt = millis();
}

void loop() {
  // Continuously read the potentiometer, even while holding an output state.
  const int rawValue = analogRead(POT_PIN);
  const uint8_t requestedNumber =
      static_cast<uint8_t>(map(rawValue, 0, 1023, 0, 99));

  // Vary PWM brightness with the potentiometer. Keep a nonzero minimum
  // so LED1 remains enabled throughout the required 0..33 range.
  const uint8_t requestedBrightness =
      static_cast<uint8_t>(map(rawValue, 0, 1023, 1, 255));

  const unsigned long now = millis();
  if (now - stateStartedAt < MIN_STATE_TIME_MS) {
    return;
  }

  // Only change outputs after the previous complete state has lasted
  // at least two seconds; apply the latest potentiometer reading.
  if (initialState ||
      requestedNumber != currentNumber ||
      requestedBrightness != currentBrightness) {
    applyState(requestedNumber, requestedBrightness);

    currentNumber = requestedNumber;
    currentBrightness = requestedBrightness;
    initialState = false;
    stateStartedAt = millis();
  }
}
