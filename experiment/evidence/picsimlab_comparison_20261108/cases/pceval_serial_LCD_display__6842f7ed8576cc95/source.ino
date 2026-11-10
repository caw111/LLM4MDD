#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS   10
#define TFT_DC   9
// SPI: SCK=13, MISO=12, MOSI=11 (wired per circuit)

Adafruit_ILI9341 tft(TFT_CS, TFT_DC);

static String serialLine;
static unsigned long lastCharTime = 0;

void setup() {
  // 1. Declare Adafruit_ILI9341 object (already done above)
  // 2. Begin display and change it to landscape mode (rotation 1)
  tft.begin();
  tft.setRotation(1);

  // 3. Initialize the serial monitor
  Serial.begin(9600);

  // Optionally clear to black at startup
  tft.fillScreen(ILI9341_BLACK);

  serialLine = "";
  lastCharTime = millis();
}

void loop() {
  // 4. Continuously read the serial monitor
  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    // Handle newline to end line (line_or_idle behavior)
    if (c == '\n' || c == '\r') {
      if (serialLine.length() > 0) {
        // 5. If there is input from the Serial Monitor, clear and print
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.print(serialLine);

        // 6. Also print to the serial monitor
        Serial.println(serialLine);
      }
      serialLine = "";
      lastCharTime = millis();
    } else {
      // framing=line_or_idle; keep accumulating characters
      serialLine += c;
      lastCharTime = millis();
    }
  }

  // Idle timeout (20ms): treat accumulated input as a line
  const unsigned long idle_timeout_ms = 20;
  if (serialLine.length() > 0 && (millis() - lastCharTime) > idle_timeout_ms) {
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print(serialLine);

    Serial.println(serialLine);

    serialLine = "";
    lastCharTime = millis();
  }
}