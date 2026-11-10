#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_traffic_light\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const int buttonPin = 2; // push button input (HIGH when pressed)
const int led1Pin = 8;   // red
const int led2Pin = 9;   // yellow
const int led3Pin = 10;  // green

static bool lastButtonPressed = false;

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_traffic_light\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setAllLedsOff();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_traffic_light\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_traffic_light\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_traffic_light\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setAllLedsOff() {
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  digitalWrite(led3Pin, LOW);
}

void setup() {
  pinMode(buttonPin, INPUT); // external 10k pulls down when not pressed

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

  // Expected behavior on power-up: green LED on if button not pressed
  if (digitalRead(buttonPin) == HIGH) {
    setAllLedsOff();
    // If pressed at startup, run the pressed sequence (then green back on)
    digitalWrite(led2Pin, HIGH);
    digitalWrite(led3Pin, LOW);
    digitalWrite(led1Pin, LOW);
    delay(1000);

    digitalWrite(led2Pin, LOW);
    digitalWrite(led1Pin, HIGH);
    delay(2000);

    digitalWrite(led1Pin, LOW);
    digitalWrite(led3Pin, HIGH);
  } else {
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, HIGH);
  }

  lastButtonPressed = (digitalRead(buttonPin) == HIGH);
}

void loop() {
  bool buttonPressed = (digitalRead(buttonPin) == HIGH);

  if (!buttonPressed) {
    // If not pressed, continuously keep green LED on
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, HIGH);
    lastButtonPressed = false;
    return;
  }

  // If pressed: green off, then yellow for 1000ms, red for 2000ms, then green on again.
  // Avoid retriggering the sequence continuously while button stays pressed.
  if (!lastButtonPressed) {
    digitalWrite(led3Pin, LOW);
    digitalWrite(led2Pin, HIGH);
    digitalWrite(led1Pin, LOW);
    delay(1000);

    digitalWrite(led2Pin, LOW);
    digitalWrite(led1Pin, HIGH);
    delay(2000);

    digitalWrite(led1Pin, LOW);
    digitalWrite(led3Pin, HIGH);
  }

  lastButtonPressed = true;
}
