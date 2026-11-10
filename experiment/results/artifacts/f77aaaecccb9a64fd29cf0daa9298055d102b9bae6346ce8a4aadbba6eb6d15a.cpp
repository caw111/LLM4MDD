#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_006\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t CLK_PIN = 4;
constexpr uint8_t DIO_PIN = 5;
constexpr uint8_t POT_PIN = A0;

TM1637Display display(CLK_PIN, DIO_PIN);

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_006\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_006\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_006\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(POT_PIN, INPUT);
  analogReference(DEFAULT);

  display.setBrightness(7, true);
  display.showNumberDec(0, false);
}

void loop() {
  const int rawValue = analogRead(POT_PIN);
  const int displayValue = static_cast<int>(
      map(rawValue, 0, 1023, 0, 9999));

  display.showNumberDec(displayValue, false);
  delay(20);
}
