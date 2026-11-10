#include <Arduino.h>

static constexpr uint8_t PIN_A  = 2;  // io_1
static constexpr uint8_t PIN_B  = 3;  // io_2
static constexpr uint8_t PIN_C  = 4;  // io_3
static constexpr uint8_t PIN_D  = 5;  // io_4
static constexpr uint8_t PIN_E  = 6;  // io_5
static constexpr uint8_t PIN_F  = 7;  // io_6
static constexpr uint8_t PIN_G  = 8;  // io_7
static constexpr uint8_t PIN_DP = 9;  // io_8

// Controller state per frozen model: outputs depend on io_0_valid and io_0.
// No explicit internal states provided; however the oracle says display remains
// same as last valid frame during invalid steps. The frozen model expressions
// yield false when io_0_valid=false, so we implement the contract behavior by
// retaining last computed outputs when io_0_valid is false.
struct DisplayLatch {
  bool io_1 = false;
  bool io_2 = false;
  bool io_3 = false;
  bool io_4 = false;
  bool io_5 = false;
  bool io_6 = false;
  bool io_7 = false;
  bool io_8 = false;
} latch;

// Segment outputs are "true_level = HIGH, false_level = LOW" in the model.
// But hardware is common anode, and requirement states "LOW lights segment".
// The frozen model already treats "io_* true" as HIGH. We must map the model's
// boolean to the actual pin logic to satisfy common-anode active-low.
// Therefore: model_true => pin LOW, model_false => pin HIGH.
static inline void writeSegmentPin(uint8_t pin, bool modelValue) {
  digitalWrite(pin, modelValue ? LOW : HIGH);
}

static uint8_t parseSingleDigitFromLine(const String& s, bool &valid) {
  valid = false;
  // Accept optional whitespace; then one character digit 0..9
  int i = 0;
  while (i < (int)s.length() && isspace((unsigned char)s[i])) i++;
  if (i >= (int)s.length()) return 0;

  char c = s[i];
  if (c >= '0' && c <= '9') {
    // Ensure rest is whitespace
    int j = i + 1;
    while (j < (int)s.length() && isspace((unsigned char)s[j])) j++;
    if (j == (int)s.length()) {
      valid = true;
      return (uint8_t)(c - '0');
    }
  }
  return 0;
}

static void updateOutputsFromModel(bool io_0_valid, int32_t io_0) {
  if (!io_0_valid) {
    // Retain previous latch (matches oracle requirement).
    return;
  }

  const bool m_io_1 = ((io_0_valid && io_0 == 0) || (io_0_valid && io_0 == 2) || (io_0_valid && io_0 == 3) ||
                        (io_0_valid && io_0 == 5) || (io_0_valid && io_0 == 6) || (io_0_valid && io_0 == 7) ||
                        (io_0_valid && io_0 == 8) || (io_0_valid && io_0 == 9));

  const bool m_io_2 = (io_0_valid && (io_0 == 0 || io_0 == 1 || io_0 == 2 || io_0 == 3 || io_0 == 4 ||
                                     io_0 == 7 || io_0 == 8 || io_0 == 9));

  const bool m_io_3 = (io_0_valid && (io_0 == 0 || io_0 == 1 || io_0 == 3 || io_0 == 4 || io_0 == 5 ||
                                     io_0 == 6 || io_0 == 7 || io_0 == 8 || io_0 == 9));

  const bool m_io_4 = (io_0_valid && (io_0 == 0 || io_0 == 2 || io_0 == 3 || io_0 == 5 || io_0 == 6 ||
                                     io_0 == 8 || io_0 == 9));

  const bool m_io_5 = (io_0_valid && (io_0 == 0 || io_0 == 2 || io_0 == 6 || io_0 == 8));

  const bool m_io_6 = (io_0_valid && (io_0 == 0 || io_0 == 4 || io_0 == 5 || io_0 == 6 || io_0 == 8 || io_0 == 9));

  const bool m_io_7 = (io_0_valid && (io_0 == 0 || io_0 == 2 || io_0 == 3 || io_0 == 5 || io_0 == 6 || io_0 == 8 || io_0 == 9));

  const bool m_io_8 = (io_0_valid && (io_0 == 0 || io_0 == 1 || io_0 == 2 || io_0 == 3 || io_0 == 4 ||
                                     io_0 == 5 || io_0 == 6 || io_0 == 7 || io_0 == 8 || io_0 == 9));

  latch.io_1 = m_io_1;
  latch.io_2 = m_io_2;
  latch.io_3 = m_io_3;
  latch.io_4 = m_io_4;
  latch.io_5 = m_io_5;
  latch.io_6 = m_io_6;
  latch.io_7 = m_io_7;
  latch.io_8 = m_io_8;
}

void setup() {
  // Set 2~9 pin as an output
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_D, OUTPUT);
  pinMode(PIN_E, OUTPUT);
  pinMode(PIN_F, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_DP, OUTPUT);

  // Initial model outputs are false; but latch begins with false and we retain on invalid.
  // Drive pins to match common-anode behavior for false => pin HIGH (segment off).
  writeSegmentPin(PIN_A, latch.io_1);
  writeSegmentPin(PIN_B, latch.io_2);
  writeSegmentPin(PIN_C, latch.io_3);
  writeSegmentPin(PIN_D, latch.io_4);
  writeSegmentPin(PIN_E, latch.io_5);
  writeSegmentPin(PIN_F, latch.io_6);
  writeSegmentPin(PIN_G, latch.io_7);
  writeSegmentPin(PIN_DP, latch.io_8);

  Serial.begin(9600);
}

void loop() {
  // Controller period is 5ms. Implement a simple 5ms paced loop.
  static unsigned long nextTick = 0;
  unsigned long now = millis();
  if ((long)(now - nextTick) < 0) return;
  nextTick = now + 5;

  int32_t io_0 = 0;
  bool io_0_valid = false;

  // Serial framing: line_or_idle. We'll parse complete line frames (newline terminated)
  // and also allow "idle_timeout_ms" behavior by attempting parse on inactivity.
  static String lineBuf;
  static unsigned long lastCharTime = 0;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastCharTime = millis();

    if (c == '\n' || c == '\r') {
      if (lineBuf.length() > 0) {
        bool ok = false;
        uint8_t digit = parseSingleDigitFromLine(lineBuf, ok);
        if (ok) {
          io_0 = digit;
          io_0_valid = true;
        }
      }
      lineBuf = "";
    } else {
      // Accumulate; limit buffer to reasonable size
      if (lineBuf.length() < 16) lineBuf += c;
    }
  }

  // idle_timeout_ms = 20: if no new chars and buffer has something, try parse as a frame
  if (!io_0_valid && lineBuf.length() > 0) {
    if ((unsigned long)(millis() - lastCharTime) >= 20) {
      bool ok = false;
      uint8_t digit = parseSingleDigitFromLine(lineBuf, ok);
      if (ok) {
        io_0 = digit;
        io_0_valid = true;
      }
      lineBuf = "";
    }
  }

  // Update model and latch
  updateOutputsFromModel(io_0_valid, io_0);

  // Drive outputs from latch
  writeSegmentPin(PIN_A, latch.io_1);
  writeSegmentPin(PIN_B, latch.io_2);
  writeSegmentPin(PIN_C, latch.io_3);
  writeSegmentPin(PIN_D, latch.io_4);
  writeSegmentPin(PIN_E, latch.io_5);
  writeSegmentPin(PIN_F, latch.io_6);
  writeSegmentPin(PIN_G, latch.io_7);
  writeSegmentPin(PIN_DP, latch.io_8);
}