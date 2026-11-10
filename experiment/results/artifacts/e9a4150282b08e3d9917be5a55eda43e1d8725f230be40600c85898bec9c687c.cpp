#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_buzzer_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t BUZZER_POS_PIN = 2;   // Connected to Arduino pin2
// Buzzer negative pin is connected to GND (GND.1 on Uno), so no code needed for that.

const unsigned int TONE_FREQ_HZ = 262; // 262Hz tone

#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_buzzer_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_buzzer_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_buzzer_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUZZER_POS_PIN, OUTPUT);
}

void loop() {
  // Generate a continuous 262Hz tone by toggling the buzzer pin.
  // For 262Hz: half-period = 1/(2*f) seconds.
  unsigned int halfPeriodMicros = (unsigned int)(500000UL / (unsigned long)TONE_FREQ_HZ);

  digitalWrite(BUZZER_POS_PIN, HIGH);
  delayMicroseconds(halfPeriodMicros);
  digitalWrite(BUZZER_POS_PIN, LOW);
  delayMicroseconds(halfPeriodMicros);
}
