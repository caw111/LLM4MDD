#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_88\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t BTN1_PIN = 2;
static const uint8_t BTN2_PIN = 3;

// Shift registers control pins (Mega 2560)
static const uint8_t SR1_SHCP = A2; // gray
static const uint8_t SR1_STCP = A1; // purple
static const uint8_t SR1_DS   = A0; // blue

// Common wiring ties: SR1 control lines to SR2 are same pins
static const uint8_t SR_OE = -1; // tied to GND in circuit (sr1:OE -> GND); no pin

// RGB LED pins
static const uint8_t RGB_R = 4; // red
static const uint8_t RGB_G = 5; // green
static const uint8_t RGB_B = 6; // blue

// Segment bit mapping to 7-segment digit (common cathode), using Q0..Q7S -> A..G, DP(not used)
static const uint8_t SEG_A = 0; // Q0 -> A
static const uint8_t SEG_B = 1; // Q1 -> B
static const uint8_t SEG_C = 2; // Q2 -> C
static const uint8_t SEG_D = 3; // Q3 -> D
static const uint8_t SEG_E = 4; // Q4 -> E
static const uint8_t SEG_F = 5; // Q5 -> F
static const uint8_t SEG_G = 6; // Q6 -> G
// Q7S -> sr2 DS? Actually in circuit: sr1:Q7S to sr2:DS. For sr1, Q7S is not used for segments.

static const uint8_t digits7seg[10][7] = {
  // A, B, C, D, E, F, G (1 = segment ON)
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_88\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t sr1ByteFromDigit(uint8_t digit);
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_88\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void pulseSTCP();
#line 66 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_88\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOut2x7seg(uint8_t tensDigit, uint8_t onesDigit);
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_88\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBForNumber(int n);
#line 123 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_88\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBOff();
#line 174 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_88\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplayAndRGB();
#line 182 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_88\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 208 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_88\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_88\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t sr1ByteFromDigit(uint8_t digit) {
  // Build byte where Q0..Q6 map to A..G; Q7 corresponds to DP (unused -> 0)
  uint8_t b = 0;
  if (digit <= 9) {
    for (uint8_t i = 0; i < 7; i++) {
      if (digits7seg[digit][i]) b |= (1u << i);
    }
  }
  // DP (bit 7) remains 0
  return b;
}

static uint8_t lastSR1Byte = 0;
static uint8_t lastSR2Byte = 0;

static void pulseSTCP() {
  digitalWrite(SR1_STCP, LOW);
  delayMicroseconds(2);
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(2);
  digitalWrite(SR1_STCP, LOW);
}

static void shiftOut2x7seg(uint8_t tensDigit, uint8_t onesDigit) {
  // Daisy chain: SR1 feeds SR2 through sr1:Q7S -> sr2:DS.
  // We shift 8 bits for SR1 and 8 bits for SR2, MSB first (bit 7 .. bit 0).
  // Total 16 clocks:
  //   - First 8 bits clocked end up in SR1
  //   - Next 8 bits end up in SR2
  uint8_t b1 = sr1ByteFromDigit(tensDigit); // SR1 -> sevseg1
  uint8_t b2 = sr1ByteFromDigit(onesDigit); // SR2 -> sevseg2

  // To ensure exact behavior with daisy chain ordering:
  // shift MSB first into DS.
  // After shifting 16 bits, latch both with STCP (shared).
  lastSR1Byte = b1;
  lastSR2Byte = b2;

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);

  // First: shift SR1 byte
  for (int8_t bit = 7; bit >= 0; --bit) {
    digitalWrite(SR1_DS, ((b1 >> bit) & 1u) ? HIGH : LOW);
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(1);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(1);
  }
  // Then: shift SR2 byte
  for (int8_t bit = 7; bit >= 0; --bit) {
    digitalWrite(SR1_DS, ((b2 >> bit) & 1u) ? HIGH : LOW);
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(1);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(1);
  }

  // Latch outputs
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(2);
  digitalWrite(SR1_STCP, LOW);
}

static void setRGBForNumber(int n) {
  // RGB LED should be OFF at boot, then per rules after updates.
  uint8_t r = 0, g = 0, b = 0;
  if (n >= 0 && n <= 33) {
    r = 255; g = 0;   b = 0;
  } else if (n >= 34 && n <= 66) {
    r = 0;   g = 255; b = 0;
  } else { // 67..99
    r = 0;   g = 0;   b = 255;
  }
  // These pins are connected directly to LED anodes (through R/G/B pins on RGB LED).
  analogWrite(RGB_R, r);
  analogWrite(RGB_G, g);
  analogWrite(RGB_B, b);
}

static void setRGBOff() {
  analogWrite(RGB_R, 0);
  analogWrite(RGB_G, 0);
  analogWrite(RGB_B, 0);
}

// Simple debounced buttons (active HIGH due to pullups? Circuit ties btn to Mega GND and signal pin)
// In circuit: btn1 has left to 5V through? Actually mega:2 connected to btn1:1.l (green), btn1:2.r to GND.
// So when not pressed: signal pin pulled HIGH (to 5V). When pressed: signal pin shorted to GND -> LOW.
// We implement active-low buttons.
struct DebouncedButton {
  uint8_t pin;
  bool stablePressed;      // true if currently pressed
  bool lastReading;
  unsigned long lastChangeMs;
  unsigned long debounceMs;

  void init(uint8_t p, unsigned long dbMs) {
    pin = p;
    debounceMs = dbMs;
    pinMode(pin, INPUT_PULLUP); // safe: circuit already uses 5V pull via wiring, but this prevents floating
    bool reading = (digitalRead(pin) == LOW); // pressed when LOW
    stablePressed = reading;
    lastReading = reading;
    lastChangeMs = millis();
  }

  // Returns true on press event (transition to pressed)
  bool update() {
    bool reading = (digitalRead(pin) == LOW);

    if (reading != lastReading) {
      lastReading = reading;
      lastChangeMs = millis();
    }

    if ((millis() - lastChangeMs) >= debounceMs) {
      if (reading != stablePressed) {
        stablePressed = reading;
        // event when becomes pressed
        if (stablePressed) return true;
      }
    }
    return false;
  }
};

static DebouncedButton btn1, btn2;

static int numberValue = 0;

static void updateDisplayAndRGB() {
  // Tens digit on sevseg1 (sr1), units digit on sevseg2 (sr2)
  int tens = (numberValue / 10) % 10;
  int ones = numberValue % 10;
  shiftOut2x7seg((uint8_t)tens, (uint8_t)ones);
  setRGBForNumber(numberValue);
}

void setup() {
  // Pins for shift registers
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS,   OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // RGB LED pins
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  setRGBOff();

  // Buttons
  btn1.init(BTN1_PIN, 30);
  btn2.init(BTN2_PIN, 30);

  // Initial display: "00"
  numberValue = 0;
  shiftOut2x7seg(0, 0);
  setRGBOff(); // requirement: RGB LED should be off at power on/reset
}

void loop() {
  bool incEvent = btn1.update();
  bool decEvent = btn2.update();

  if (incEvent || decEvent) {
    if (incEvent && !decEvent) {
      numberValue++;
      if (numberValue > 99) numberValue = 0;
    } else if (decEvent && !incEvent) {
      numberValue--;
      if (numberValue < 0) numberValue = 99;
    } else {
      // If both events somehow happen in same loop iteration, define net effect as:
      // increment then decrement => no change.
      // This preserves debouncing behavior without double-triggering.
    }

    updateDisplayAndRGB();
  }
}
