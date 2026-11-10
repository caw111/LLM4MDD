#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

constexpr uint8_t TFT_RST = 8;
constexpr uint8_t TFT_DC = 9;
constexpr uint8_t TFT_CS = 10;
constexpr uint8_t BUTTON_PIN = 2;

// Hardware SPI: MOSI = 11, MISO = 12, SCK = 13.
Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

int previousButtonState = -1;

void displayButtonState(int state) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.print(state == HIGH ? F("Pressed!") : F("Not pressed!"));
}

void setup() {
  // External 10k-ohm pull-down: HIGH when pressed, LOW when released.
  pinMode(BUTTON_PIN, INPUT);

  tft.begin();
  tft.setRotation(1);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setTextWrap(false);

  previousButtonState = digitalRead(BUTTON_PIN);
  displayButtonState(previousButtonState);
}

void loop() {
  const int buttonState = digitalRead(BUTTON_PIN);

  // Redraw on each state change, retaining the current state while held.
  if (buttonState != previousButtonState) {
    displayButtonState(buttonState);
    previousButtonState = buttonState;
  }
}