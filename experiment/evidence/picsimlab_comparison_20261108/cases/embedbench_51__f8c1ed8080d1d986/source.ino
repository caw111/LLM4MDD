// Arduino Mega 2560 sketch for synchronized RGB + 5-segment bargraph phases

// RGB common cathode LEDs (COM to GND)
const uint8_t RGB1_R = 2;
const uint8_t RGB1_G = 3;
const uint8_t RGB1_B = 4;

const uint8_t RGB2_R = 5;
const uint8_t RGB2_G = 6;
const uint8_t RGB2_B = 7;

const uint8_t RGB3_R = 8;
const uint8_t RGB3_G = 9;
const uint8_t RGB3_B = 10;

// Bar graph segment anodes (A1..A5)
const uint8_t BAR_A1 = 11;
const uint8_t BAR_A2 = 12;
const uint8_t BAR_A3 = 13;
const uint8_t BAR_A4 = 44;
const uint8_t BAR_A5 = 45;

static inline void setRGB(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool rOn, bool gOn, bool bOn) {
  // Common cathode: HIGH turns ON, LOW turns OFF (assuming standard module wiring).
  digitalWrite(rPin, rOn ? HIGH : LOW);
  digitalWrite(gPin, gOn ? HIGH : LOW);
  digitalWrite(bPin, bOn ? HIGH : LOW);
}

static inline void setBarSegments(uint8_t segments) {
  // lights up from A1 to A5
  digitalWrite(BAR_A1, (segments >= 1) ? HIGH : LOW);
  digitalWrite(BAR_A2, (segments >= 2) ? HIGH : LOW);
  digitalWrite(BAR_A3, (segments >= 3) ? HIGH : LOW);
  digitalWrite(BAR_A4, (segments >= 4) ? HIGH : LOW);
  digitalWrite(BAR_A5, (segments >= 5) ? HIGH : LOW);
}

void setup() {
  // RGB pins
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  pinMode(RGB3_R, OUTPUT);
  pinMode(RGB3_G, OUTPUT);
  pinMode(RGB3_B, OUTPUT);

  // Bar graph pins
  pinMode(BAR_A1, OUTPUT);
  pinMode(BAR_A2, OUTPUT);
  pinMode(BAR_A3, OUTPUT);
  pinMode(BAR_A4, OUTPUT);
  pinMode(BAR_A5, OUTPUT);

  // Ensure everything starts OFF
  setRGB(RGB1_R, RGB1_G, RGB1_B, false, false, false);
  setRGB(RGB2_R, RGB2_G, RGB2_B, false, false, false);
  setRGB(RGB3_R, RGB3_G, RGB3_B, false, false, false);
  setBarSegments(0);
}

void loop() {
  // Phase durations: each 2 seconds
  const unsigned long phaseMs = 2000;

  // Phase 1: RGB1 red, bargraph 1 segment
  setRGB(RGB1_R, RGB1_G, RGB1_B, true, false, false);
  setRGB(RGB2_R, RGB2_G, RGB2_B, false, false, false);
  setRGB(RGB3_R, RGB3_G, RGB3_B, false, false, false);
  setBarSegments(1);
  delay(phaseMs);

  // Phase 2: RGB2 green, bargraph 2 segments
  setRGB(RGB1_R, RGB1_G, RGB1_B, false, false, false);
  setRGB(RGB2_R, RGB2_G, RGB2_B, false, true, false);
  setRGB(RGB3_R, RGB3_G, RGB3_B, false, false, false);
  setBarSegments(2);
  delay(phaseMs);

  // Phase 3: RGB3 blue, bargraph 3 segments
  setRGB(RGB1_R, RGB1_G, RGB1_B, false, false, false);
  setRGB(RGB2_R, RGB2_G, RGB2_B, false, false, false);
  setRGB(RGB3_R, RGB3_G, RGB3_B, false, false, true);
  setBarSegments(3);
  delay(phaseMs);

  // Phase 4: RGB1 red + RGB2 green, bargraph 4 segments
  setRGB(RGB1_R, RGB1_G, RGB1_B, true, false, false);
  setRGB(RGB2_R, RGB2_G, RGB2_B, false, true, false);
  setRGB(RGB3_R, RGB3_G, RGB3_B, false, false, false);
  setBarSegments(4);
  delay(phaseMs);

  // Phase 5: RGB1+RGB2+RGB3 white, bargraph all 5 segments
  setRGB(RGB1_R, RGB1_G, RGB1_B, true, true, true);
  setRGB(RGB2_R, RGB2_G, RGB2_B, true, true, true);
  setRGB(RGB3_R, RGB3_G, RGB3_B, true, true, true);
  setBarSegments(5);
  delay(phaseMs);

  // Loop restarts automatically to Phase 1
}