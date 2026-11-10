/**
  Show serial input in ili9431 lcd display
  05 MAY 2025 by Blind Author
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
    Serial.begin(9600);

    tft.begin();
    tft.setRotation(1);
}

void loop() {
    if (Serial.available() > 0) {
        tft.fillScreen(ILI9341_BLACK);

        String input = Serial.readString();
        input.trim();
        Serial.println(input);
        
        tft.setCursor(0, 0);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.println(input);
    }

} 