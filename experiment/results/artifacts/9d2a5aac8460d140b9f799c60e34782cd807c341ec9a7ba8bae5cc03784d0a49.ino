#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9, 8); // CS, D/C, RST (matches wiring)

const uint8_t BUTTON_PIN = 2;
int lastButtonState = LOW;

void setup() {
  tft.begin();

  // Begin display and change it to landscape mode
  tft.setRotation(3);

  // Set text color to white, and size to 2
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  // Set button pin as INPUT (cathode-style: reads HIGH when pressed, LOW when released)
  pinMode(BUTTON_PIN, INPUT);

  // Initialize last state
  lastButtonState = digitalRead(BUTTON_PIN);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState != lastButtonState && buttonState == HIGH) {
    // Newly pressed: clear with black background and print "Pressed! at position (0, 0)"
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.print("Pressed! at position (0, 0)");
  } else if (buttonState != lastButtonState && buttonState == LOW) {
    // Released: clear with black background and print "Not pressed! at position (0, 0)"
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.print("Not pressed! at position (0, 0)");
  } else {
    // Continuously read the button state; only update display when state changes as required
    // No action
  }

  lastButtonState = buttonState;
}