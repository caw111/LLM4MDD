#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_46\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

enum ColorIndex : uint8_t {
  RED = 0,
  GREEN,
  BLUE,
  YELLOW,
  CYAN,
  MAGENTA,
  WHITE
};

Servo servo1;
Servo servo2;
Servo servo3;

const uint8_t RGB_R_PIN = 2; // mega:2 -> rgb1:R
const uint8_t RGB_G_PIN = 3; // mega:3 -> rgb1:G
const uint8_t RGB_B_PIN = 4; // mega:4 -> rgb1:B

// 2 seconds per color
const unsigned long COLOR_DURATION_MS = 2000;

ColorIndex currentColor = RED;
unsigned long lastChangeMs = 0;

// Helper: set RGB LED color for a common-cathode RGB LED.
// With common cathode, setting a channel LOW turns it ON (through current to COM at GND).
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_46\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(ColorIndex c);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_46\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setServosForColor(ColorIndex c);
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_46\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void applyState(ColorIndex c);
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_46\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void advanceColor();
#line 132 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_46\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 150 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_46\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_46\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(ColorIndex c) {
  // Default all off
  digitalWrite(RGB_R_PIN, HIGH);
  digitalWrite(RGB_G_PIN, HIGH);
  digitalWrite(RGB_B_PIN, HIGH);

  switch (c) {
    case RED:
      digitalWrite(RGB_R_PIN, LOW);
      break;
    case GREEN:
      digitalWrite(RGB_G_PIN, LOW);
      break;
    case BLUE:
      digitalWrite(RGB_B_PIN, LOW);
      break;
    case YELLOW:
      digitalWrite(RGB_R_PIN, LOW);
      digitalWrite(RGB_G_PIN, LOW);
      break;
    case CYAN:
      digitalWrite(RGB_G_PIN, LOW);
      digitalWrite(RGB_B_PIN, LOW);
      break;
    case MAGENTA:
      digitalWrite(RGB_R_PIN, LOW);
      digitalWrite(RGB_B_PIN, LOW);
      break;
    case WHITE:
      digitalWrite(RGB_R_PIN, LOW);
      digitalWrite(RGB_G_PIN, LOW);
      digitalWrite(RGB_B_PIN, LOW);
      break;
  }
}

void setServosForColor(ColorIndex c) {
  // When the RGB LED is Red, servo1 should move to 0°, servo2 to 90°, and servo3 to 180°.
  // When the RGB LED is Green, servo1 should move to 90°, servo2 to 180°, and servo3 to 0°.
  // When the RGB LED is Blue, servo1 should move to 180°, servo2 to 0°, and servo3 to 90°.
  // When the RGB LED is Yellow, servo1 should move to 45°, servo2 to 135°, and servo3 to 225°
  // (if supported, otherwise 180°).
  // When the RGB LED is Cyan, servo1 should move to 135°, servo2 to 225°
  // (if supported, otherwise 180°), and servo3 to 45°.
  // When the RGB LED is Magenta, servo1 should move to 225° (if supported, otherwise 180°),
  // servo2 to 45°, and servo3 to 135°.
  // When the RGB LED is White, all servos should return to their initial positions:
  // servo1 to 0°, servo2 to 90°, and servo3 to 180°.

  auto clampTo180 = [](int deg) -> int {
    if (deg < 0) return 0;
    if (deg > 180) return 180;
    return deg;
  };

  int s1 = 0, s2 = 90, s3 = 180;

  switch (c) {
    case RED:
      s1 = 0;   s2 = 90;  s3 = 180;
      break;
    case GREEN:
      s1 = 90;  s2 = 180; s3 = 0;
      break;
    case BLUE:
      s1 = 180; s2 = 0;   s3 = 90;
      break;
    case YELLOW:
      s1 = 45;  s2 = 135; s3 = 225; // if not supported, clamp to 180
      break;
    case CYAN:
      s1 = 135; s2 = 225; s3 = 45;  // if not supported, clamp to 180
      break;
    case MAGENTA:
      s1 = 225; s2 = 45;  s3 = 135; // if not supported, clamp to 180
      break;
    case WHITE:
      s1 = 0;   s2 = 90;  s3 = 180;
      break;
  }

  servo1.write(clampTo180(s1));
  servo2.write(clampTo180(s2));
  servo3.write(clampTo180(s3));
}

void applyState(ColorIndex c) {
  setRGB(c);
  setServosForColor(c);
}

void advanceColor() {
  switch (currentColor) {
    case RED:     currentColor = GREEN;    break;
    case GREEN:   currentColor = BLUE;     break;
    case BLUE:    currentColor = YELLOW;   break;
    case YELLOW:  currentColor = CYAN;     break;
    case CYAN:     currentColor = MAGENTA; break;
    case MAGENTA:  currentColor = WHITE;   break;
    case WHITE:    currentColor = RED;     break;
  }
}

void setup() {
  // RGB LED pins
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Servo pins (circuit: mega:5 -> servo1:PWM, mega:6 -> servo2:PWM, mega:7 -> servo3:PWM)
  servo1.attach(5);
  servo2.attach(6);
  servo3.attach(7);

  // Start from initial state per requirement (white returns to initial positions).
  // We begin at Red since the sequence starts with Red.
  currentColor = RED;
  applyState(currentColor);
  lastChangeMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastChangeMs >= COLOR_DURATION_MS) {
    lastChangeMs = now;
    advanceColor();
    applyState(currentColor);
  }
}
