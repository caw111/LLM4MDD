#include <Arduino.h>

static const uint8_t BTN_PIN = 2;      // btn1:1.l connected to mega:2
static const uint8_t POT_PIN = A3;     // pot1:SIG connected to mega:A3

// rgb1: common cathode, so use active HIGH for each channel
static const uint8_t RGB_R_PIN = 3;    // mega:3 -> rgb1:R
static const uint8_t RGB_G_PIN = 4;    // mega:4 -> rgb1:G
static const uint8_t RGB_B_PIN = 5;    // mega:5 -> rgb1:B

enum Channel : uint8_t { RED = 0, GREEN = 1, BLUE = 2 };
static Channel currentChannel = RED;

static const uint16_t DEBOUNCE_MS = 50;
static bool lastStableBtnState = LOW;
static bool lastReading = LOW;
static unsigned long lastDebounceTime = 0;

static void setRgb(Channel ch, uint8_t brightness) {
  // Unselected channels must remain off
  digitalWrite(RGB_R_PIN, (ch == RED) ? (brightness ? HIGH : LOW) : LOW);
  digitalWrite(RGB_G_PIN, (ch == GREEN) ? (brightness ? HIGH : LOW) : LOW);
  digitalWrite(RGB_B_PIN, (ch == BLUE) ? (brightness ? HIGH : LOW) : LOW);
}

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP); // Wiring: one side to GND, other to mega:2; use pull-up

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Initialization: RGB LED must remain off
  digitalWrite(RGB_R_PIN, LOW);
  digitalWrite(RGB_G_PIN, LOW);
  digitalWrite(RGB_B_PIN, LOW);

  // Start with button state stable
  lastReading = digitalRead(BTN_PIN);
  lastStableBtnState = lastReading;
  lastDebounceTime = millis();
}

void loop() {
  // Debounced button press detection
  bool reading = digitalRead(BTN_PIN);

  if (reading != lastReading) {
    lastDebounceTime = millis();
    lastReading = reading;
  }

  if ((millis() - lastDebounceTime) >= DEBOUNCE_MS) {
    if (reading != lastStableBtnState) {
      lastStableBtnState = reading;

      // Button press: with INPUT_PULLUP, pressed typically reads LOW.
      if (lastStableBtnState == LOW) {
        // Cycle channels: Red -> Green -> Blue -> Red...
        currentChannel = static_cast<Channel>((static_cast<uint8_t>(currentChannel) + 1) % 3);
      }
    }
  }

  // Brightness control: pot adjusts currently selected channel (0-255)
  int potValue = analogRead(POT_PIN); // 0..1023
  uint8_t brightness = (uint8_t)((potValue * 255L) / 1023L);

  // Unselected channels remain off; selected updates continuously
  setRgb(currentChannel, brightness);
}