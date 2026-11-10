#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // Ensure D6 is ready for tone output
  pinMode(6, OUTPUT);

  // Optional: start serial only if needed by user; requirement did not mention serial.
}

void loop() {
  // Read potentiometer on A0
  int adc = analogRead(A0); // 0..1023

  // Map ADC to a practical audible frequency range
  //  - Low end above 0 to keep tone audible
  //  - High end within typical buzzer capability
  long freq = map(adc, 0, 1023, 100, 2000);

  // Generate tone on D6 with a fixed duty via Arduino tone()
  tone(6, freq);

  // Small delay to allow perceived pitch change responsiveness
  delay(20);

  // Keep behavior responsive while producing continuous sound
  // Stop briefly to avoid runaway tone duration overlap (tone() handles it, but be explicit).
  noTone(6);

  delay(5);
}
