#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_020\\direct\\attempt_0\\sketch\\sketch.ino"
#include <TM1637Display.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t CLK_PIN = 4;
constexpr uint8_t DIO_PIN = 5;

TM1637Display display(CLK_PIN, DIO_PIN);

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_020\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_020\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_020\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_020\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay() {
  // Button connects D2 to GND when pressed.
  const bool pressed = (digitalRead(BUTTON_PIN) == LOW);
  static int lastValue = -1;
  const int value = pressed ? 1111 : 0;

  if (value != lastValue) {
    display.showNumberDec(value, true, 4, 0);
    lastValue = value;
  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  display.setBrightness(7, true);
  updateDisplay();
}

void loop() {
  updateDisplay();
}
