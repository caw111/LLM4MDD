#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_LCD_display_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8

// Wiring: SCK=13, MISO=12, MOSI=11 are handled by SPI.
// CS, D/C, RST are defined above.
Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_LCD_display_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_LCD_display_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_LCD_display_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  tft.begin();

  // Change to landscape mode (rotation = 1)
  tft.setRotation(1);

  // Fill display with black color
  tft.fillScreen(ILI9341_BLACK);

  // Display text at (0,0), white color, size 2
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Hello World!");
}

void loop() {
  // No additional behavior required
}
