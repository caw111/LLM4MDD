#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

const uint8_t SERVO1_PIN = 2;   // mega:2 -> servo1:PWM
const uint8_t SERVO2_PIN = 3;   // mega:3 -> servo2:PWM
const uint8_t SERVO3_PIN = 4;   // mega:4 -> servo3:PWM

// Bargraph pins: mega:5..13 -> A1..A10
const uint8_t BAR_PINS[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};

static int angle1 = 0;
static int angle2 = 0;
static int angle3 = 0;

static bool increasing1 = true;
static bool increasing2 = true;
static bool increasing3 = true;

static unsigned long lastUpdateMs = 0;
static const unsigned long UPDATE_PERIOD_MS = 2000;

static void writeBargraphFromTotal(int totalAngle) {
  // totalAngle in [0..540]
  // 10 ranges of 54 units per LED: lit LEDs = total / 54, capped to 10.
  int leds = totalAngle / 54;
  if (totalAngle >= 540) leds = 10;
  if (leds < 0) leds = 0;
  if (leds > 10) leds = 10;

  for (int i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], (i < leds) ? HIGH : LOW);
  }
}

static void updateServo(int &angle, bool &increasing) {
  if (increasing) {
    angle += 18;
    if (angle >= 180) {
      angle = 180;
      increasing = false;
    }
  } else {
    angle -= 18;
    if (angle <= 0) {
      angle = 0;
      increasing = true;
    }
  }
}

void setup() {
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialization: all servos at 0°, LED bar graph off.
  angle1 = angle2 = angle3 = 0;
  increasing1 = increasing2 = increasing3 = true;

  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);

  for (int i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  writeBargraphFromTotal(0);

  lastUpdateMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastUpdateMs >= UPDATE_PERIOD_MS) {
    lastUpdateMs += UPDATE_PERIOD_MS; // keep strict 2s synchronization

    // Update all servos simultaneously each 2 seconds
    updateServo(angle1, increasing1);
    updateServo(angle2, increasing2);
    updateServo(angle3, increasing3);

    servo1.write(angle1);
    servo2.write(angle2);
    servo3.write(angle3);

    int total = angle1 + angle2 + angle3; // 0..540
    writeBargraphFromTotal(total);
  }
}