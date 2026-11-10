#include <Arduino.h>

static const uint8_t LED_PIN = 9;

static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;
static const uint32_t SERIAL_BAUD = 9600;

enum Mode : uint8_t { MODE_UNKNOWN = 0, MODE1 = 1, MODE2 = 2 };
static volatile Mode currentMode = MODE_UNKNOWN;

// For MODE1 "渐变" (fade)
static uint8_t fadeValue = 0;
static int8_t fadeStep = 1;
static uint32_t lastFadeMs = 0;
static const uint16_t FADE_INTERVAL_MS = 8; // smooth enough on UNO

// Simple line/idle command parser with idle timeout.
static String cmdBuf;
static uint32_t lastRxMs = 0;

static void applyModeOutput(Mode m) {
  switch (m) {
    case MODE1:
      // Ensure pin stays driven; actual fading handled in loop()
      analogWrite(LED_PIN, fadeValue);
      break;
    case MODE2:
      analogWrite(LED_PIN, 255);
      break;
    default:
      // If unknown, turn off to avoid spurious behavior
      analogWrite(LED_PIN, 0);
      break;
  }
}

void setup() {
  analogWrite(LED_PIN, 0);

  pinMode(LED_PIN, OUTPUT);

  Serial.begin(SERIAL_BAUD);
  cmdBuf.reserve(16);
  lastRxMs = millis();
  currentMode = MODE_UNKNOWN;
}

void loop() {
  // Serial command handling: accept "MODE1" or "MODE2"
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    uint32_t now = millis();
    lastRxMs = now;

    // Treat common line endings as separators
    if (c == '\n' || c == '\r') {
      if (cmdBuf.length() > 0) {
        String t = cmdBuf;
        cmdBuf = "";
        t.trim();

        if (t == "MODE1") {
          currentMode = MODE1;
          // Initialize fade state when entering MODE1
          fadeValue = 0;
          fadeStep = 1;
          lastFadeMs = now;
        } else if (t == "MODE2") {
          currentMode = MODE2;
        }
      }
    } else {
      // Accumulate non-line characters
      if (cmdBuf.length() < 31) {
        // Ignore spaces/tabs
        if (c != ' ' && c != '\t') cmdBuf += c;
      }
    }
  }

  // If no new bytes for idle timeout, attempt to parse whatever is in buffer
  uint32_t now = millis();
  if (cmdBuf.length() > 0 && (now - lastRxMs) >= SERIAL_IDLE_TIMEOUT_MS) {
    String t = cmdBuf;
    cmdBuf = "";
    t.trim();

    if (t == "MODE1") {
      currentMode = MODE1;
      fadeValue = 0;
      fadeStep = 1;
      lastFadeMs = now;
    } else if (t == "MODE2") {
      currentMode = MODE2;
    }
  }

  // Mode outputs
  if (currentMode == MODE1) {
    if ((now - lastFadeMs) >= FADE_INTERVAL_MS) {
      lastFadeMs = now;

      // Fade up/down between 0..255
      fadeValue = (uint8_t)(fadeValue + fadeStep);
      if (fadeValue == 0 && fadeStep < 0) fadeStep = 1;
      else if (fadeValue == 255 && fadeStep > 0) fadeStep = -1;

      analogWrite(LED_PIN, fadeValue);
    }
  } else if (currentMode == MODE2) {
    // Constant on
    analogWrite(LED_PIN, 255);
  } else {
    // Unknown: keep off
    analogWrite(LED_PIN, 0);
  }
}