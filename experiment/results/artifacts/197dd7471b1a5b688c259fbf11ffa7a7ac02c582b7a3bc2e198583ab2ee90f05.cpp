#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(9, OUTPUT);          // D9 PWM output to LED
  pinMode(A0, INPUT);         // Potentiometer connected to A0
}

void loop() {
  // Read potentiometer value (0~1023)
  int pot = analogRead(A0);

  // Map to PWM duty cycle (0~255)
  int pwmValue = map(pot, 0, 1023, 0, 255);

  // Control LED brightness using PWM on D9
  analogWrite(9, pwmValue);

  // Small delay to reduce flicker and stabilize readings
  delay(10);
}
