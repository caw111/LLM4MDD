#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t POT_PIN = A0;
constexpr uint8_t LED_PIN = 9;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

int previousBrightness = -1;

void setup() {
  pinMode(POT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  analogWrite(LED_PIN, 0);
  display.setBrightness(7, true);
  display.showNumberDec(0, false);
}

void loop() {
  const int potentiometerValue = analogRead(POT_PIN);
  const int brightness = map(potentiometerValue, 0, 1023, 0, 255);

  if (brightness != previousBrightness) {
    analogWrite(LED_PIN, brightness);
    // Display the LED PWM brightness: 0 (off) to 255 (maximum).
    display.showNumberDec(brightness, false);
    previousBrightness = brightness;
  }

  delay(20);
}