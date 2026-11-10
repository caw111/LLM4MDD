#include <Arduino.h>

static const uint8_t BTN_PINS[3] = {2, 3, 4}; // btn1, btn2, btn3
static const uint8_t RGB1_PINS[3] = {5, 6, 7}; // R,G,B (common cathode)
static const uint8_t RGB2_PINS[3] = {8, 9, 10}; // R,G,B (common cathode)

// 74HC595 sr1 wiring
static const uint8_t SR_DATA_PIN  = A0; // DS
static const uint8_t SR_LATCH_PIN = A1; // STCP
static const uint8_t SR_CLOCK_PIN = A2; // SHCP
// MR tied to VCC (sr1:VCC to sr1:MR). OE tied to GND (sr1:OE -> black), so always enabled.

static const uint8_t SEG_PINS_TO_SR_Q[7] = {
  0, // Q0 -> sevseg1:A
  1, // Q1 -> sevseg1:B
  2, // Q2 -> sevseg1:C
  3, // Q3 -> sevseg1:D
  4, // Q4 -> sevseg1:E
  5, // Q5 -> sevseg1:F
  6  // Q6 -> sevseg1:G
};

// Common cathode 7-seg segment order: A,B,C,D,E,F,G
// Bit i corresponds to Q(i) -> segment mapping above.
// We will set HIGH to light segments (because common cathode).
static const uint8_t SEG_MAP[10] = {
  // 0: A B C D E F
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),
  // 1: B C
  (1 << 1) | (1 << 2),
  // 2: A B D E G
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),
  // 3: A B C D G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),
  // 4: B C F G
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),
  // 5: A C D F G
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),
  // 6: A C D E F G
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 7: A B C
  (1 << 0) | (1 << 1) | (1 << 2),
  // 8: A B C D E F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 9: A B C D F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)
};

static void srWrite7Segments(uint8_t segBitsAtoG) {
  // srBits 0..6 correspond to Q0..Q6 -> A..G respectively.
  // We will shift out Q0..Q6 and set Q7 (unused) to 0.
  uint8_t byteToShift = segBitsAtoG & 0x7F;

  digitalWrite(SR_LATCH_PIN, LOW);

  // Shift out MSB first? We'll choose a deterministic approach:
  // Use Q7..Q0 ordering by clocking 8 bits MSB->LSB into DS.
  // If the physical mapping expects a specific order, the following matches
  // common usage when Q0 receives the first/LSB depending on wiring.
  //
  // In Wokwi/standard 74HC595 behavior with "shift register bits":
  // When shifting with MSB first, Q7..Q0 follow the shifted bits.
  // Here we want Q0..Q6 to correspond to bit0..bit6 in byteToShift,
  // so we shift MSB first from bit7 down to bit0.
  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(SR_CLOCK_PIN, LOW);
    digitalWrite(SR_DATA_PIN, (byteToShift >> i) & 0x01);
    digitalWrite(SR_CLOCK_PIN, HIGH);
  }

  digitalWrite(SR_LATCH_PIN, HIGH);
}

static void updateDisplay(uint8_t count) {
  uint8_t shown = count;
  if (count > 15) shown = 0;         // should not happen, but keep safe
  if (count > 9) shown = count % 10; // ones digits for 10..15

  uint8_t segBits = SEG_MAP[shown]; // bits A..G mapped to Q0..Q6
  srWrite7Segments(segBits);
}

static void setRGBOff() {
  // Common cathode: HIGH turns a segment/LED color channel ON.
  for (uint8_t i = 0; i < 3; i++) {
    digitalWrite(RGB1_PINS[i], LOW);
    digitalWrite(RGB2_PINS[i], LOW);
  }
}

static void setRGB(uint8_t r1, uint8_t g1, uint8_t b1,
                   uint8_t r2, uint8_t g2, uint8_t b2) {
  digitalWrite(RGB1_PINS[0], r1 ? HIGH : LOW);
  digitalWrite(RGB1_PINS[1], g1 ? HIGH : LOW);
  digitalWrite(RGB1_PINS[2], b1 ? HIGH : LOW);

  digitalWrite(RGB2_PINS[0], r2 ? HIGH : LOW);
  digitalWrite(RGB2_PINS[1], g2 ? HIGH : LOW);
  digitalWrite(RGB2_PINS[2], b2 ? HIGH : LOW);
}

static void updateColors(uint8_t count) {
  if (count < 5) {
    // rgb1 red, rgb2 blue
    setRGB(1, 0, 0,  0, 0, 1);
  } else if (count <= 9) {
    // rgb1 green, rgb2 yellow (red+green)
    setRGB(0, 1, 0,  1, 1, 0);
  } else {
    // rgb1 purple (red+blue), rgb2 cyan (green+blue)
    setRGB(1, 0, 1,  0, 1, 1);
  }
}

static uint8_t count = 0;

// Debounce state
static const unsigned long DEBOUNCE_MS = 40;
static uint8_t lastStableLevel[3] = {HIGH, HIGH, HIGH};
static uint8_t lastReadLevel[3] = {HIGH, HIGH, HIGH};
static unsigned long lastChangeTime[3] = {0, 0, 0};

static void handleButtonPress(uint8_t idx) {
  // Buttons are wired to GND when pressed and to +5V through input pin when not pressed?
  // Circuit shows: button left to mega digital pin via green wire, right to GND.
  // That means the pin reads HIGH when not pressed (if we use INPUT_PULLUP),
  // and reads LOW when pressed. We'll use INPUT_PULLUP accordingly.
  uint8_t reading = digitalRead(BTN_PINS[idx]);

  if (reading != lastReadLevel[idx]) {
    lastReadLevel[idx] = reading;
    lastChangeTime[idx] = millis();
  }

  if ((millis() - lastChangeTime[idx]) >= DEBOUNCE_MS) {
    if (reading != lastStableLevel[idx]) {
      lastStableLevel[idx] = reading;

      // Count on press (transition to LOW)
      if (reading == LOW) {
        if (count > 15) count = 0; // keep safe
        count++;
        if (count > 15) count = 0;

        updateDisplay(count);
        if (count == 0) {
          setRGBOff();
        } else {
          updateColors(count);
        }
      }
    }
  }
}

void setup() {
  // Button inputs (btn1/2/3)
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(BTN_PINS[i], INPUT_PULLUP);
    lastReadLevel[i] = digitalRead(BTN_PINS[i]);
    lastStableLevel[i] = lastReadLevel[i];
    lastChangeTime[i] = millis();
  }

  // RGB outputs
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(RGB1_PINS[i], OUTPUT);
    pinMode(RGB2_PINS[i], OUTPUT);
    digitalWrite(RGB1_PINS[i], LOW);
    digitalWrite(RGB2_PINS[i], LOW);
  }

  // 74HC595 control pins
  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  digitalWrite(SR_DATA_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);
  digitalWrite(SR_CLOCK_PIN, LOW);

  // Initialization: show 0 on 7-seg; both RGB LEDs off
  count = 0;
  updateDisplay(count);
  setRGBOff();
}

void loop() {
  for (uint8_t i = 0; i < 3; i++) {
    handleButtonPress(i);
  }
}