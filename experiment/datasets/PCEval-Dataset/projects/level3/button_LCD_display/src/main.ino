/**
  Show button state in ili9431 lcd display
  05 MAY 2025 by Blind Author
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define BUTTON_PIN 2

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

bool pressed = true;

void setup() {
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);

    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);

    pinMode(BUTTON_PIN, INPUT);
}

void loop() {
    if (digitalRead(BUTTON_PIN) == HIGH && !pressed) {
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(0, 0);
        tft.println("Pressed!");
        pressed = true;
    } else if (digitalRead(BUTTON_PIN) == LOW && pressed) {
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(0, 0);
        tft.println("Not Pressed!");
        pressed = false;
    }

    delay(100);
} 