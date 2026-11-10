#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// TM1637 four-digit display
// Requirement wiring (authoritative pin assignments):
// CLK -> D2, DIO -> D3
// NOTE: Provided configuration JSON remapped pins to D4/D5; however the user requirement
// explicitly states D2/D3. We preserve the user-requested wiring.
static const uint8_t TM1637_CLK = 2;  // D2
static const uint8_t TM1637_DIO = 3;  // D3

TM1637Display display(TM1637_CLK, TM1637_DIO);

#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_017\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  display.setBrightness(0x0f); // maximum brightness (typical safe default)
  display.clear();
  display.showNumberDec(1234, false);
}

void loop() {
  // Continuously keep displaying 1234 as required.
  display.showNumberDec(1234, false);
}
