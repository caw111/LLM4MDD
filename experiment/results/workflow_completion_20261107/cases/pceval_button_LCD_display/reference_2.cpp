#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

const uint8_t BUTTON_PIN = 2;
const uint8_t LCD_CS = 10;
const uint8_t LCD_DC = 9;
const uint8_t LCD_RST = 8;

Adafruit_ILI9341 display(LCD_CS, LCD_DC, LCD_RST);
int lastButtonState = LOW;

void showButtonState(int state) {
  display.fillScreen(ILI9341_BLACK);
  display.setCursor(0, 0);
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(2);
  if (state == HIGH) {
    display.print("Pressed!");
  } else {
    display.print("Not pressed!");
  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  display.begin();
  display.setRotation(1);
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(2);
  lastButtonState = digitalRead(BUTTON_PIN);
  showButtonState(lastButtonState);
}

void loop() {
  const int state = digitalRead(BUTTON_PIN);
  if (state != lastButtonState) {
    showButtonState(state);
    lastButtonState = state;
  }
  delay(1);
}
