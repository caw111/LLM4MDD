#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_013\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint32_t SERIAL_BAUD = 9600;
static const uint16_t PRINT_INTERVAL_MS = 1000;

const uint8_t ANALOG_PIN = A0; // 电位器
const uint8_t BUTTON_PIN = 2; // D2 按钮

uint32_t lastPrintMs = 0;

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_013\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_013\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_013\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(BUTTON_PIN, INPUT_PULLUP); // 默认上拉，按钮按下为低电平
  lastPrintMs = millis();
}

void loop() {
  uint32_t now = millis();
  if ((uint32_t)(now - lastPrintMs) >= PRINT_INTERVAL_MS) {
    lastPrintMs = now;

    int a0Value = analogRead(ANALOG_PIN);
    int d2State = digitalRead(BUTTON_PIN); // 按下(低) / 未按(高)

    Serial.print("A0=");
    Serial.print(a0Value);
    Serial.print(",D2=");
    Serial.println(d2State);
  }
}
