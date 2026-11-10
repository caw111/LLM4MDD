#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_12\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t BTN_PIN = 2;   // mega:2 -> btn1:1.l
static const uint8_t SHCP_PIN = A2;  // mega:A2 -> sr1:SHCP
static const uint8_t STCP_PIN = A1;  // mega:A1 -> sr1:STCP
static const uint8_t DS_PIN  = A0;   // mega:A0 -> sr1:DS

// 7-seg segments (common-cathode assumed based on wiring/Q->segment pins)
static const uint8_t segMap[10] = {
  // A, B, C, D, E, F, G correspond to Q0..Q6
  // bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G
  0b0111111, // 0
  0b0000110, // 1
  0b1011011, // 2
  0b1001111, // 3
  0b1100110, // 4
  0b1101101, // 5
  0b1111101, // 6
  0b0000111, // 7
  0b1111111, // 8
  0b1101111  // 9
};

#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_12\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOut74HC595(uint8_t value);
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_12\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showDigit(uint8_t d);
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_12\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_12\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_12\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOut74HC595(uint8_t value)
{
  // DS -> serial data, SHCP -> shift clock, STCP -> latch clock
  // Send 8 bits (Q0..Q7), Q7 unused but included as 0.
  digitalWrite(STCP_PIN, LOW);
  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(SHCP_PIN, LOW);
    digitalWrite(DS_PIN, (value & (1 << i)) ? HIGH : LOW);
    digitalWrite(SHCP_PIN, HIGH);
  }
  digitalWrite(STCP_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(STCP_PIN, LOW);
}

static void showDigit(uint8_t d)
{
  d %= 10;
  // Place bits into Q0..Q6, keep Q7 as 0.
  uint8_t segBits = segMap[d] & 0x7F; // bits for Q0..Q6
  uint8_t out = segBits;             // Q7 remains 0
  shiftOut74HC595(out);
}

void setup()
{
  pinMode(SHCP_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(DS_PIN, OUTPUT);

  pinMode(BTN_PIN, INPUT_PULLUP); // btn1 connected to GND (active LOW)

  digitalWrite(SHCP_PIN, LOW);
  digitalWrite(STCP_PIN, LOW);
  digitalWrite(DS_PIN, LOW);

  // Initialization: show 0 on the 7-seg display
  showDigit(0);
}

void loop()
{
  static bool lastStablePressed = false;
  static bool lastRawPressed = false;
  static unsigned long lastDebounceTime = 0;

  const unsigned long debounceMs = 40;

  // Active-low button
  bool rawPressed = (digitalRead(BTN_PIN) == LOW);

  if (rawPressed != lastRawPressed) {
    lastDebounceTime = millis();
    lastRawPressed = rawPressed;
  }

  if ((millis() - lastDebounceTime) >= debounceMs) {
    if (rawPressed != lastStablePressed) {
      lastStablePressed = rawPressed;

      // Increment on press event (transition to pressed)
      if (lastStablePressed) {
        static uint8_t count = 0;
        count = (count + 3) % 10;
        showDigit(count);
      }
    }
  }

  // small yield
  delay(1);
}
