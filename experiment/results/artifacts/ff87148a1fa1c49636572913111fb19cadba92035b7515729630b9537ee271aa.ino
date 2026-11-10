#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9); // CS=10, D/C=9

void setup() {
  tft.begin();
  tft.setRotation(1); // landscape mode (rotation 1)

  Serial.begin(9600);
}

void loop() {
  static String input;
  static unsigned long lastCharTime = 0;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    input += c;
    lastCharTime = millis();
  }

  // framing=line_or_idle; idle_timeout_ms=20
  if (!input.isEmpty() && (millis() - lastCharTime > 20)) {
    // If there is input, clear display and print
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print(input);

    // Also print to serial monitor
    Serial.println(input);

    input = "";
  }
}