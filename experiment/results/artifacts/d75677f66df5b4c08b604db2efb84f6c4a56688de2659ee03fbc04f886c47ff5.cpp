#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_duration\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const int buttonPin = 4; // specified in circuit/config
const int gndPin = 2;    // specified in circuit/config (physical net "GND.2")

static const unsigned long bounceDelayMs = 25; // short delay to handle switch bouncing

#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_duration\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_duration\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_duration\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
}

void loop() {
  int reading = digitalRead(buttonPin);

  // Detect press (pull-up: not pressed likely HIGH, pressed likely LOW)
  if (reading == LOW) {
    // Debounce: wait briefly and confirm
    delay(bounceDelayMs);
    if (digitalRead(buttonPin) != LOW) {
      return;
    }

    unsigned long startMs = millis();

    // Measure duration until button released
    while (digitalRead(buttonPin) == LOW) {
      delay(1); // small delay to reduce bouncing/noise effect
    }

    unsigned long endMs = millis();
    unsigned long durationMs = endMs - startMs;
    double durationSec = durationMs / 1000.0;

    Serial.print("Button pressed duration: ");
    Serial.print(durationSec);
    Serial.println("s");
  }

  delay(5); // small loop delay
}
