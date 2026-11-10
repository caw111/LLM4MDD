#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_77\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t SERVO1_PIN = 3;
constexpr uint8_t SERVO2_PIN = 4;
constexpr uint8_t DATA_PIN = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;

constexpr unsigned long DEBOUNCE_MS = 25;

// Q0-Q6 connect to segments A-G. COM is wired to +5V,
// so segments are active LOW. Q7 stays HIGH (unused).
const uint8_t DIGIT_PATTERNS[10] = {
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

Servo servo1;
Servo servo2;

uint8_t displayedNumber = 0;
uint8_t lastButtonReading = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastButtonChange = 0;

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_77\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_77\\direct\\attempt_0\\sketch\\sketch.ino"
void updateServos();
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_77\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_77\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_77\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay() {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           DIGIT_PATTERNS[displayedNumber]);
  digitalWrite(LATCH_PIN, HIGH);
}

void updateServos() {
  const uint8_t angle = (displayedNumber % 2 == 0) ? 0 : 90;
  servo1.write(angle);
  servo2.write(angle);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  displayedNumber = 0;
  updateDisplay();

  // Preload the initial angle before enabling servo pulses.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  updateServos();

  lastButtonChange = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastButtonReading = reading;
    lastButtonChange = now;
  }

  if (reading != stableButtonState &&
      now - lastButtonChange >= DEBOUNCE_MS) {
    stableButtonState = reading;

    // Count only debounced press edges, not releases or holds.
    if (stableButtonState == LOW) {
      displayedNumber = (displayedNumber + 1) % 10;
      updateDisplay();
      updateServos();
    }
  }
}
