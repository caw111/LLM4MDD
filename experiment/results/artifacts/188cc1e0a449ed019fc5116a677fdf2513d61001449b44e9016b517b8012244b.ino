#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

enum ColorState {
  RED,
  GREEN,
  BLUE,
  YELLOW,
  CYAN,
  MAGENTA,
  WHITE
};

static const ColorState colorSequence[] = {RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE};
static const uint8_t colorCount = sizeof(colorSequence) / sizeof(colorSequence[0]);

// RGB LED (common cathode):
// Mega pins: R=2, G=3, B=4
static const uint8_t PIN_R = 2;
static const uint8_t PIN_G = 3;
static const uint8_t PIN_B = 4;

// Servo wiring (PWM):
// servo1 PWM on pin 5, servo2 PWM on pin 6, servo3 PWM on pin 7
static const uint8_t PIN_SERVO1 = 5;
static const uint8_t PIN_SERVO2 = 6;
static const uint8_t PIN_SERVO3 = 7;

static void setRGB(ColorState c) {
  // Common cathode: HIGH turns channel on (LED emits).
  // Keep only the required channels on per color.
  switch (c) {
    case RED:
      digitalWrite(PIN_R, HIGH); digitalWrite(PIN_G, LOW);  digitalWrite(PIN_B, LOW);
      break;
    case GREEN:
      digitalWrite(PIN_R, LOW);  digitalWrite(PIN_G, HIGH); digitalWrite(PIN_B, LOW);
      break;
    case BLUE:
      digitalWrite(PIN_R, LOW);  digitalWrite(PIN_G, LOW);  digitalWrite(PIN_B, HIGH);
      break;
    case YELLOW:
      digitalWrite(PIN_R, HIGH); digitalWrite(PIN_G, HIGH); digitalWrite(PIN_B, LOW);
      break;
    case CYAN:
      digitalWrite(PIN_R, LOW);  digitalWrite(PIN_G, HIGH); digitalWrite(PIN_B, HIGH);
      break;
    case MAGENTA:
      digitalWrite(PIN_R, HIGH); digitalWrite(PIN_G, LOW);  digitalWrite(PIN_B, HIGH);
      break;
    case WHITE:
      digitalWrite(PIN_R, HIGH); digitalWrite(PIN_G, HIGH); digitalWrite(PIN_B, HIGH);
      break;
  }
}

static void setServosForColor(ColorState c) {
  // Servo angles per requirement.
  // If "225° not supported" is encountered: Arduino Servo library typically
  // accepts up to 180°, so we clamp to 180 to ensure deterministic behavior.
  auto clamp180 = [](int deg) -> int {
    if (deg < 0) return 0;
    if (deg > 180) return 180;
    return deg;
  };

  int a1 = 0, a2 = 90, a3 = 180; // initial positions for WHITE per requirement

  switch (c) {
    case RED:
      a1 = 0;   a2 = 90;  a3 = 180;
      break;
    case GREEN:
      a1 = 90;  a2 = 180; a3 = 0;
      break;
    case BLUE:
      a1 = 180; a2 = 0;   a3 = 90;
      break;
    case YELLOW:
      a1 = 45;   a2 = 135;  a3 = 225;  // clamp if needed
      break;
    case CYAN:
      a1 = 135;  a2 = 225;  a3 = 45;   // clamp if needed
      break;
    case MAGENTA:
      a1 = 225;  a2 = 45;   a3 = 135;  // clamp if needed
      break;
    case WHITE:
      a1 = 0;   a2 = 90;  a3 = 180;
      break;
  }

  servo1.write(clamp180(a1));
  servo2.write(clamp180(a2));
  servo3.write(clamp180(a3));
}

void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);

  // Ensure RGB starts off until first state is set
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_B, LOW);

  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);
  servo3.attach(PIN_SERVO3);

  // Set initial positions as specified for WHITE
  servo1.write(0);
  servo2.write(90);
  servo3.write(180);
}

void loop() {
  static uint8_t idx = 0;
  static unsigned long stateStartMs = 0;

  ColorState current = colorSequence[idx];

  // On state entry
  if (stateStartMs == 0) {
    setRGB(current);
    setServosForColor(current);
    stateStartMs = millis();
  }

  // Maintain each state for 2 seconds
  const unsigned long stateDurationMs = 2000UL;
  if (millis() - stateStartMs >= stateDurationMs) {
    idx = (idx + 1) % colorCount;
    stateStartMs = 0; // triggers re-application for new state in next loop iteration
  }
}