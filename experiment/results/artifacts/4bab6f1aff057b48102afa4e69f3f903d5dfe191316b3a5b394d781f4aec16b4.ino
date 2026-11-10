#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Circuit wiring:
// CS=10, D/C=9, SCK=13, MOSI=11, MISO=12
Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9); // CS=10, D/C=9

void setup() {
  // 1. Declare Adafruit_ILI9341 object (done globally)
  // 2. Begin display and change it to landscape mode (rotation 1)
  tft.begin();
  tft.setRotation(1); // landscape mode (rotation 1)

  // 3. Initialize the serial monitor
  Serial.begin(9600);
}

void loop() {
  // framing=line_or_idle; idle_timeout_ms=20
  static String input;
  static unsigned long lastCharTime = 0;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    input += c;
    lastCharTime = millis();
  }

  if (input.length() > 0 && (millis() - lastCharTime > 20)) {
    // 5. If there is input from the Serial Monitor,
    // clear the display with a black background and print the input string
    // at position (0, 0) in white with text size 2
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print(input);

    // 6. Also print the input string to the serial monitor
    Serial.println(input);

    input = "";
  }
}