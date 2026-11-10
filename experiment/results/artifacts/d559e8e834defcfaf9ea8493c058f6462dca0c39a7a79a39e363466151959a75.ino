// Arduino Mega 2560 sketch
// Bar graph sequential pattern A1->A8, each LED on for 2 seconds.
// RGB behavior depends on active bar segment:
// - A1-A4: rgb1 = red, rgb2 = blue
// - A5-A8: rgb1 = green, rgb2 = yellow (red+green)

const uint8_t BAR_PINS[8] = {
  8,  // A1
  9,  // A2
  10, // A3
  11, // A4
  12, // A5
  13, // A6
  44, // A7
  45  // A8
};

const uint8_t RGB1_R = 2;  // rgb1 R
const uint8_t RGB1_G = 3;  // rgb1 G
const uint8_t RGB1_B = 4;  // rgb1 B

const uint8_t RGB2_R = 5;  // rgb2 R
const uint8_t RGB2_G = 6;  // rgb2 G
const uint8_t RGB2_B = 7;  // rgb2 B

// Common cathode LEDs in provided circuit -> HIGH turns ON, LOW turns OFF
static inline void setRgbCommonCathode(
  uint8_t rPin, uint8_t gPin, uint8_t bPin,
  bool rOn, bool gOn, bool bOn
) {
  digitalWrite(rPin, rOn ? HIGH : LOW);
  digitalWrite(gPin, gOn ? HIGH : LOW);
  digitalWrite(bPin, bOn ? HIGH : LOW);
}

void setup() {
  // Bar graph pins
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW); // off on reset/power
  }

  // RGB pins
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // Off state for all RGB channels
  setRgbCommonCathode(RGB1_R, RGB1_G, RGB1_B, false, false, false);
  setRgbCommonCathode(RGB2_R, RGB2_G, RGB2_B, false, false, false);
}

void loop() {
  for (uint8_t idx = 0; idx < 8; idx++) {
    // Turn off all bar LEDs first (ensures only one active at a time)
    for (uint8_t j = 0; j < 8; j++) {
      digitalWrite(BAR_PINS[j], LOW);
    }

    // Activate current bar segment
    digitalWrite(BAR_PINS[idx], HIGH);

    // Determine which quarter of the sequence we're in
    // idx: 0..3 => A1..A4, idx: 4..7 => A5..A8
    if (idx <= 3) {
      // rgb1 red, rgb2 blue
      setRgbCommonCathode(RGB1_R, RGB1_G, RGB1_B, true, false, false);
      setRgbCommonCathode(RGB2_R, RGB2_G, RGB2_B, false, false, true);
    } else {
      // rgb1 green, rgb2 yellow (red + green)
      setRgbCommonCathode(RGB1_R, RGB1_G, RGB1_B, false, true, false);
      setRgbCommonCathode(RGB2_R, RGB2_G, RGB2_B, true, true, false);
    }

    delay(2000);
  }
}