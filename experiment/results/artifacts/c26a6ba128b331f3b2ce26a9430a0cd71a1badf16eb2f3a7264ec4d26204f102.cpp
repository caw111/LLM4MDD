#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_010\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t POT_PIN = A1;  // Authoritative remapping of A0
constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_010\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_010\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_010\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(POT_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Button connects D2 to GND.
  display.setBrightness(7, true);
  display.clear();
}

void loop() {
  const bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);
  const int value = buttonPressed ? 8888 : analogRead(POT_PIN);

  display.showNumberDec(value, false);
  delay(10);
}
