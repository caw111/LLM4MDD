#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_76\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

constexpr uint8_t DATA_PIN = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;
constexpr uint8_t SERVO1_PIN = 5;
constexpr uint8_t SERVO2_PIN = 6;
constexpr unsigned long DEBOUNCE_MS = 25;

// Q0..Q6 connect to A..G. COM is wired to +5V, so segments
// are active LOW. Q7 is unused and remains HIGH.
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

struct Button {
  uint8_t pin;
  uint8_t increment;
  uint8_t lastReading;
  uint8_t stableState;
  unsigned long lastChangeTime;
};

Button buttons[] = {
  {2, 1, HIGH, HIGH, 0},
  {3, 2, HIGH, HIGH, 0},
  {4, 3, HIGH, HIGH, 0}
};

Servo servo1;
Servo servo2;
uint8_t displayedNumber = 0;

#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_76\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_76\\direct\\attempt_0\\sketch\\sketch.ino"
void updateServos();
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_76\\direct\\attempt_0\\sketch\\sketch.ino"
void handlePress(uint8_t increment);
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_76\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_76\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_76\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay() {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           DIGIT_PATTERNS[displayedNumber]);
  digitalWrite(LATCH_PIN, HIGH);
}

void updateServos() {
  if (displayedNumber <= 4) {
    servo1.write(0);
    servo2.write(90);
  } else {
    servo1.write(90);
    servo2.write(180);
  }
}

void handlePress(uint8_t increment) {
  displayedNumber += increment;
  if (displayedNumber > 9) {
    displayedNumber = 0;
  }

  updateDisplay();
  updateServos();
}

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  for (Button &button : buttons) {
    pinMode(button.pin, INPUT_PULLUP);
  }

  displayedNumber = 0;
  updateDisplay();

  // Explicit startup exception: both servos start at 0 degrees.
  // After a button press, apply the number-dependent positions.
  servo1.write(0);
  servo2.write(0);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(0);
  servo2.write(0);
}

void loop() {
  const unsigned long now = millis();

  for (Button &button : buttons) {
    const uint8_t reading = digitalRead(button.pin);

    if (reading != button.lastReading) {
      button.lastReading = reading;
      button.lastChangeTime = now;
    }

    if (reading != button.stableState &&
        now - button.lastChangeTime >= DEBOUNCE_MS) {
      button.stableState = reading;

      // One increment per debounced press; holding does not repeat.
      if (button.stableState == LOW) {
        handlePress(button.increment);
      }
    }
  }
}
