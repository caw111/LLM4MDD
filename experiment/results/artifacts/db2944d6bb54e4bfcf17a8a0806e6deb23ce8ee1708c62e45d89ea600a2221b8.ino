#include <Arduino.h>

constexpr uint8_t R_PIN = 4;
constexpr uint8_t G_PIN = 3;
constexpr uint8_t B_PIN = 2;
constexpr unsigned long IDLE_TIMEOUT_MS = 20;

String input;
unsigned long lastByteTime = 0;

void applyColors(const String &command) {
  // Common-anode LED: LOW turns a color fully on; HIGH turns it off.
  digitalWrite(R_PIN, command.indexOf("Red") >= 0 ? LOW : HIGH);
  digitalWrite(G_PIN, command.indexOf("Green") >= 0 ? LOW : HIGH);
  digitalWrite(B_PIN, command.indexOf("Blue") >= 0 ? LOW : HIGH);
}

void setup() {
  // Preload the output states to keep all colors off at startup.
  digitalWrite(R_PIN, HIGH);
  digitalWrite(G_PIN, HIGH);
  digitalWrite(B_PIN, HIGH);

  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  Serial.begin(9600);
  input.reserve(64);
}

void loop() {
  while (Serial.available() > 0) {
    const char c = static_cast<char>(Serial.read());
    lastByteTime = millis();

    if (c == '\n') {
      applyColors(input);
      input = "";
    } else if (c != '\r') {
      input += c;
    }
  }

  // Also accept commands without a newline after 20 ms of serial inactivity.
  if (input.length() > 0 &&
      static_cast<unsigned long>(millis() - lastByteTime) >= IDLE_TIMEOUT_MS) {
    applyColors(input);
    input = "";
  }
}