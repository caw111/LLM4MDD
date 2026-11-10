#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_LCD_display\\direct\\attempt_0\\sketch\\sketch.ino"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

constexpr uint8_t TFT_DC = 9;
constexpr uint8_t TFT_CS = 10;
constexpr unsigned long SERIAL_BAUD = 9600;
constexpr unsigned long IDLE_TIMEOUT_MS = 20;

// Hardware SPI: MOSI = 11, MISO = 12, SCK = 13.
// No display reset pin is connected.
Adafruit_ILI9341 tft(TFT_CS, TFT_DC, -1);

String input;
unsigned long lastByteTime = 0;
bool previousWasCR = false;

#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_LCD_display\\direct\\attempt_0\\sketch\\sketch.ino"
void displayAndEcho();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_LCD_display\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_LCD_display\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_LCD_display\\direct\\attempt_0\\sketch\\sketch.ino"
void displayAndEcho() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print(input);

  Serial.println(input);
  input = "";
}

void setup() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setTextWrap(true);

  Serial.begin(SERIAL_BAUD);
  input.reserve(128);
}

void loop() {
  while (Serial.available() > 0) {
    char c = static_cast<char>(Serial.read());
    lastByteTime = millis();

    if (c == '\n' && previousWasCR) {
      previousWasCR = false;
      continue;
    }

    previousWasCR = (c == '\r');

    if (c == '\r' || c == '\n') {
      displayAndEcho();
    } else {
      input += c;
    }
  }

  if (input.length() > 0 &&
      static_cast<unsigned long>(millis() - lastByteTime) >= IDLE_TIMEOUT_MS) {
    displayAndEcho();
  }
}
