#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(2, INPUT_PULLUP); // D2: low means pressed
  pinMode(9, OUTPUT);      // D9: PWM output
  analogWrite(9, 0);       // LED off initially
}

void loop() {
  // Read button (INPUT_PULLUP): LOW when pressed
  if (digitalRead(2) == LOW) {
    // Read potentiometer (0..1023) and map to PWM (0..255)
    int a = analogRead(A0); // 0..1023

    // Linear mapping with rounding to reduce off-by-1 issues:
    // pwm = round(a * 255 / 1023)
    // Use integer math: pwm = (a*255 + 511) / 1023
    uint8_t pwm = (uint8_t)((a * 255L + 511L) / 1023L);

    analogWrite(9, pwm);
  } else {
    analogWrite(9, 0); // Released: output 0
  }
}
