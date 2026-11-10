#include <Arduino.h>

// Segment order: A, B, C, D, E, F, G, DP.
// Each segment connects through a 330-ohm resistor.
// The display's common anode connects to 5V.
const uint8_t segmentPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

// A set bit means that the corresponding segment should be illuminated.
const uint8_t digitPatterns[10] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F  // 9
};

const unsigned long IDLE_TIMEOUT_MS = 20;

uint8_t displayedPattern = 0;
uint8_t pendingPattern = 0;
bool frameActive = false;
bool pendingValid = false;
unsigned long lastByteTime = 0;

void writeDisplay(uint8_t pattern) {
  for (uint8_t i = 0; i < 8; ++i) {
    // Common anode: LOW turns a segment on; HIGH turns it off.
    digitalWrite(segmentPins[i], (pattern & (1U << i)) ? LOW : HIGH);
  }
  displayedPattern = pattern;
}

void finishFrame() {
  if (pendingValid) {
    writeDisplay(pendingPattern);
  }

  frameActive = false;
  pendingValid = false;
}

void receiveCharacter(char character) {
  if (character == '\r' || character == '\n') {
    finishFrame();
    return;
  }

  if (!frameActive) {
    pendingPattern = displayedPattern;
    pendingValid = false;
    frameActive = true;
  }

  lastByteTime = millis();

  if (character >= '0' && character <= '9') {
    // A one-digit display retains the last digit in a multi-digit frame.
    pendingPattern = digitPatterns[character - '0'];
    pendingValid = true;
  } else if (character == '.') {
    pendingPattern |= 0x80;
    pendingValid = true;
  }
}

void setup() {
  for (uint8_t i = 0; i < 8; ++i) {
    // Preload HIGH before enabling the output to avoid lighting segments.
    digitalWrite(segmentPins[i], HIGH);
    pinMode(segmentPins[i], OUTPUT);
  }

  writeDisplay(0);
  Serial.begin(9600);
}

void loop() {
  if (frameActive &&
      static_cast<unsigned long>(millis() - lastByteTime) >= IDLE_TIMEOUT_MS) {
    finishFrame();
  }

  while (Serial.available() > 0) {
    receiveCharacter(static_cast<char>(Serial.read()));
  }

  if (frameActive &&
      static_cast<unsigned long>(millis() - lastByteTime) >= IDLE_TIMEOUT_MS) {
    finishFrame();
  }
}