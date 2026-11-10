#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

#define TM1637_CLK D4
#define TM1637_DIO D5
#define POT_PIN A0

// Display: show 0..9999 mapped from A0 0..1023
static TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // TM1637Display defaults: 8 digits, brightness can be adjusted if needed.
  tm1637.setBrightness(0x0f);

  // Optional: ensure analog reference/default
  analogReference(DEFAULT);
}

void loop() {
  int raw = analogRead(POT_PIN); // 0..1023

  // Map to 0..9999 (inclusive), with bounds safety
  long mapped = (long)raw * 9999L / 1023L;
  if (mapped < 0) mapped = 0;
  if (mapped > 9999) mapped = 9999;

  tm1637.showNumberDec((int)mapped, false, 4, 0); // no leading zeros? false keeps leading zeros off
  // Note: keep 4 digits (0..9999); wiring/pins preserved per requirements.
}
