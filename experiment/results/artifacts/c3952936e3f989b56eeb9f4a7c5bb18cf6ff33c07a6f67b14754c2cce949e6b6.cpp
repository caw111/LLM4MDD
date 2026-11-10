#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;

const byte btn1Pin = 2;   // mega:2
const byte btn2Pin = 3;   // mega:3
const byte btn3Pin = 4;   // mega:4

// RGB LED common cathode
const byte rgbRPin = 5;   // mega:5 -> rgb1:R
const byte rgbGPin = 6;   // mega:6 -> rgb1:G
const byte rgbBPin = 7;   // mega:7 -> rgb1:B

const int servo1PinPWM = 8; // mega:8 -> servo1:PWM
const int servo2PinPWM = 9; // mega:9 -> servo2:PWM

const unsigned long holdMs = 2000;

#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGBOff();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGBRed();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGBGreen();
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGBBlue();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setServosTo0AndRGBBlueDelayIfNeeded();
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setServosTo0();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool anyButtonPressed();
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void waitForButtonsRelease();
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 91 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGBOff() {
  // common cathode: LOW turns LED ON
  digitalWrite(rgbRPin, HIGH);
  digitalWrite(rgbGPin, HIGH);
  digitalWrite(rgbBPin, HIGH);
}

void setRGBRed() {
  digitalWrite(rgbRPin, LOW);
  digitalWrite(rgbGPin, HIGH);
  digitalWrite(rgbBPin, HIGH);
}

void setRGBGreen() {
  digitalWrite(rgbRPin, HIGH);
  digitalWrite(rgbGPin, LOW);
  digitalWrite(rgbBPin, HIGH);
}

void setRGBBlue() {
  digitalWrite(rgbRPin, HIGH);
  digitalWrite(rgbGPin, HIGH);
  digitalWrite(rgbBPin, LOW);
}

void setServosTo0AndRGBBlueDelayIfNeeded() {
  servo1.write(0);
  servo2.write(0);
  setRGBBlue();
  delay(holdMs);
}

void setServosTo0() {
  servo1.write(0);
  servo2.write(0);
}

bool anyButtonPressed() {
  return (digitalRead(btn1Pin) == LOW) ||
         (digitalRead(btn2Pin) == LOW) ||
         (digitalRead(btn3Pin) == LOW);
}

void waitForButtonsRelease() {
  while (anyButtonPressed()) {
    delay(10);
  }
}

void setup() {
  // Buttons (wiring indicates input from pushbuttons to pins with pull-up required)
  pinMode(btn1Pin, INPUT_PULLUP);
  pinMode(btn2Pin, INPUT_PULLUP);
  pinMode(btn3Pin, INPUT_PULLUP);

  // RGB LED pins
  pinMode(rgbRPin, OUTPUT);
  pinMode(rgbGPin, OUTPUT);
  pinMode(rgbBPin, OUTPUT);
  setRGBOff();

  // Servos
  servo1.attach(servo1PinPWM);
  servo2.attach(servo2PinPWM);

  // Initialization: servos at 0 degrees, RGB off
  servo1.write(0);
  servo2.write(0);
  setRGBOff();
}

void loop() {
  // btn3 has highest priority (reset)
  if (digitalRead(btn3Pin) == LOW) {
    setServosTo0();
    setRGBBlue();
    delay(holdMs);
    waitForButtonsRelease();
    return;
  }

  if (digitalRead(btn1Pin) == LOW) {
    servo1.write(90);
    // servo2 unchanged per button-specific behavior; do not reset it here
    setRGBRed();
    delay(holdMs);
    waitForButtonsRelease();
    return;
  }

  if (digitalRead(btn2Pin) == LOW) {
    servo2.write(90);
    // servo1 unchanged per button-specific behavior; do not reset it here
    setRGBGreen();
    delay(holdMs);
    waitForButtonsRelease();
    return;
  }

  // No button pressed: maintain last state; do nothing
}
