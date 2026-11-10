#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

const uint8_t BUTTON_PIN = 2;
Adafruit_ILI9341 lcd(10, 9, 8);
int displayedState = -1;

void updateDisplay() {
  const int state = digitalRead(BUTTON_PIN);
  if (state == displayedState) {
    return;
  }
  lcd.fillScreen(ILI9341_BLACK);
  lcd.setCursor(0, 0);
  lcd.setTextColor(ILI9341_WHITE);
  lcd.setTextSize(2);
  if (state == HIGH) {
    lcd.print("Pressed!");
  } else {
    lcd.print("Not pressed!");
  }
  displayedState = state;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  lcd.begin();
  lcd.setRotation(1);
  lcd.setTextColor(ILI9341_WHITE);
  lcd.setTextSize(2);
  updateDisplay();
}

void loop() {
  updateDisplay();
}
