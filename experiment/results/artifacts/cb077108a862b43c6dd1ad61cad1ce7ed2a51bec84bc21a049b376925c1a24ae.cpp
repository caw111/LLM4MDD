#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_LCD_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8

#define BUTTON_PIN 2

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_LCD_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_LCD_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_LCD_display\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT);

  tft.begin();

  // Change to landscape mode
  tft.setRotation(1);

  // Text settings
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  // Initial screen (optional but keeps behavior deterministic)
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.print("Not pressed! at position (0, 0)");
}

void loop() {
  static int lastButtonState = LOW;

  int buttonState = digitalRead(BUTTON_PIN);

  // Newly pressed = transition LOW -> HIGH
  if (buttonState == HIGH && lastButtonState == LOW) {
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.print("Pressed! at position (0, 0)");
  } else {
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.print("Not pressed! at position (0, 0)");
  }

  lastButtonState = buttonState;
}
