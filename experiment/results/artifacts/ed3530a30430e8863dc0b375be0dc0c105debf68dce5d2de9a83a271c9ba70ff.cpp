#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_traffic_light\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Traffic Light - Arduino Uno
// Button on pin2 (HIGH when pressed)
// LEDs:
//   LED1 (red)   -> pin8 via 220 ohm, cathode to GND.2
//   LED2 (yellow)-> pin9 via 220 ohm, cathode to GND.2
//   LED3 (green) -> pin10 via 220 ohm, cathode to GND.2

const int buttonPin = 2;

const int led1Pin = 8;   // red
const int led2Pin = 9;   // yellow
const int led3Pin = 10;  // green

#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_traffic_light\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_traffic_light\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_traffic_light\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(buttonPin, INPUT); // per requirement: configure the button pin as INPUT

  pinMode(led1Pin, OUTPUT);  // configure LED pins as OUTPUT
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
}

void loop() {
  int buttonState = digitalRead(buttonPin); // HIGH when pressed

  if (buttonState == HIGH) {
    // If pressed: green off, yellow on for 1000ms, then red on for 2000ms, then green on again.
    digitalWrite(led3Pin, LOW);
    digitalWrite(led2Pin, HIGH);
    delay(1000);

    digitalWrite(led2Pin, LOW);
    digitalWrite(led1Pin, HIGH);
    delay(2000);

    digitalWrite(led1Pin, LOW);
    digitalWrite(led3Pin, HIGH);
  } else {
    // If not pressed: green on.
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, HIGH);
  }
}
