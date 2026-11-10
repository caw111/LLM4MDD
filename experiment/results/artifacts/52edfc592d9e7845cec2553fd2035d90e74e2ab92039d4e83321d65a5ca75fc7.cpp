#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_58\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

// Arduino Mega 2560 wiring.
constexpr uint8_t RGB_RED_PIN   = 2;
constexpr uint8_t RGB_GREEN_PIN = 3;
constexpr uint8_t RGB_BLUE_PIN  = 4;
constexpr uint8_t SERVO_PIN     = 5;

constexpr uint8_t SR_DATA_PIN  = A0;
constexpr uint8_t SR_LATCH_PIN = A1;
constexpr uint8_t SR_CLOCK_PIN = A2;

constexpr unsigned long STEP_DURATION_MS = 2000UL;

// Bits 0..6 correspond to segments A..G. These patterns are
// inverted before transmission because the display is common-anode.
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

// Common-cathode RGB LED: HIGH turns a channel on.
constexpr uint8_t COLORS[7][3] = {
  { HIGH, LOW,  LOW  }, // Red
  { LOW,  HIGH, LOW  }, // Green
  { LOW,  LOW,  HIGH }, // Blue
  { HIGH, HIGH, LOW  }, // Yellow
  { LOW,  HIGH, HIGH }, // Cyan
  { HIGH, LOW,  HIGH }, // Magenta
  { HIGH, HIGH, HIGH }  // White
};

Servo servo;

uint8_t sequenceStep = 0;
unsigned long lastStepTime = 0;
bool sequenceStarted = false;

#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_58\\direct\\attempt_0\\sketch\\sketch.ino"
void updateHardware(uint8_t countdown, uint8_t red, uint8_t green, uint8_t blue);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_58\\direct\\attempt_0\\sketch\\sketch.ino"
void applySequenceStep();
#line 75 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_58\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 100 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_58\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_58\\direct\\attempt_0\\sketch\\sketch.ino"
void updateHardware(uint8_t countdown, uint8_t red,
                    uint8_t green, uint8_t blue) {
  const uint8_t displayData =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[countdown]);
  const uint8_t angle = static_cast<uint8_t>((9 - countdown) * 20);

  // Stage display data without changing the visible digit.
  digitalWrite(SR_LATCH_PIN, LOW);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, MSBFIRST, displayData);

  // Issue all visible state changes together, without blocking delays.
  digitalWrite(RGB_RED_PIN, red);
  digitalWrite(RGB_GREEN_PIN, green);
  digitalWrite(RGB_BLUE_PIN, blue);
  servo.write(angle);
  digitalWrite(SR_LATCH_PIN, HIGH);
}

void applySequenceStep() {
  const uint8_t countdown = 9 - sequenceStep;
  const uint8_t colorIndex = sequenceStep % 7;

  updateHardware(countdown,
                 COLORS[colorIndex][0],
                 COLORS[colorIndex][1],
                 COLORS[colorIndex][2]);
}

void setup() {
  digitalWrite(RGB_RED_PIN, LOW);
  digitalWrite(RGB_GREEN_PIN, LOW);
  digitalWrite(RGB_BLUE_PIN, LOW);

  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  digitalWrite(SR_DATA_PIN, LOW);
  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);

  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);

  // Preload the initial position before enabling servo pulses.
  servo.write(0);
  servo.attach(SERVO_PIN);

  // Initial state: RGB off, digit 9, servo at 0 degrees.
  updateHardware(9, LOW, LOW, LOW);
}

void loop() {
  const unsigned long now = millis();

  if (!sequenceStarted) {
    // Begin with red, digit 9, and 0 degrees.
    sequenceStarted = true;
    sequenceStep = 0;
    applySequenceStep();
    lastStepTime = millis();
    return;
  }

  if (now - lastStepTime >= STEP_DURATION_MS) {
    // Each complete countdown restarts the color sequence at red.
    sequenceStep = (sequenceStep + 1) % 10;
    applySequenceStep();
    lastStepTime = millis();
  }
}
