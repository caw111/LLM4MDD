#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_113\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Arduino Mega 2560 sketch
// Stages cycle every 2 seconds:
// Stage 1 (0-2s):  led1 off;  rgb1 red; rgb2 off; rgb3 off; bargraph A1
// Stage 2 (2-4s):  led1 on;   rgb1 green; rgb2 blue; rgb3 off; bargraph A1-A2
// Stage 3 (4-6s):  led1 off;  rgb1 blue; rgb2 red; rgb3 green; bargraph A1-A3
// Stage 4 (6-8s):  led1 on;   rgb1 white; rgb2 white; rgb3 white; bargraph A1-A4

// Wiring from circuit:
// led1:A -> mega D2, led1:C -> GND
const uint8_t PIN_LED1 = 2;

// rgb1 common cathode: COM->GND, R->D3, G->D4, B->D5
const uint8_t RGB1_R = 3;
const uint8_t RGB1_G = 4;
const uint8_t RGB1_B = 5;

// rgb2 common cathode: COM->GND, R->D6, G->D7, B->D8
const uint8_t RGB2_R = 6;
const uint8_t RGB2_G = 7;
const uint8_t RGB2_B = 8;

// rgb3 common cathode: COM->GND, R->D9, G->D10, B->D11
const uint8_t RGB3_R = 9;
const uint8_t RGB3_G = 10;
const uint8_t RGB3_B = 11;

// Bar graph (common cathode): Cx -> GND, Ax -> these pins
// A1 -> D12, A2 -> D13, A3 -> D44, A4 -> D45
const uint8_t BAR_A1 = 12;
const uint8_t BAR_A2 = 13;
const uint8_t BAR_A3 = 44;
const uint8_t BAR_A4 = 45;

// Common cathode means: HIGH turns LED channel OFF? Actually for common cathode,
// cathode is tied to GND, and anodes are driven. The anodes are active HIGH.
// So HIGH = ON for each segment/channel.
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_113\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBChannel(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool rOn, bool gOn, bool bOn);
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_113\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void barSetSegments(uint8_t n);
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_113\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_113\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_113\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setRGBChannel(uint8_t rPin, uint8_t gPin, uint8_t bPin,
                                  bool rOn, bool gOn, bool bOn) {
  digitalWrite(rPin, rOn ? HIGH : LOW);
  digitalWrite(gPin, gOn ? HIGH : LOW);
  digitalWrite(bPin, bOn ? HIGH : LOW);
}

static inline void barSetSegments(uint8_t n) {
  // n in 0..4
  digitalWrite(BAR_A1, (n >= 1) ? HIGH : LOW);
  digitalWrite(BAR_A2, (n >= 2) ? HIGH : LOW);
  digitalWrite(BAR_A3, (n >= 3) ? HIGH : LOW);
  digitalWrite(BAR_A4, (n >= 4) ? HIGH : LOW);
}

void setup() {
  // Initialize all pins as outputs
  pinMode(PIN_LED1, OUTPUT);

  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  pinMode(RGB3_R, OUTPUT);
  pinMode(RGB3_G, OUTPUT);
  pinMode(RGB3_B, OUTPUT);

  pinMode(BAR_A1, OUTPUT);
  pinMode(BAR_A2, OUTPUT);
  pinMode(BAR_A3, OUTPUT);
  pinMode(BAR_A4, OUTPUT);

  // Initialization: all LEDs off
  digitalWrite(PIN_LED1, LOW);

  setRGBChannel(RGB1_R, RGB1_G, RGB1_B, false, false, false);
  setRGBChannel(RGB2_R, RGB2_G, RGB2_B, false, false, false);
  setRGBChannel(RGB3_R, RGB3_G, RGB3_B, false, false, false);

  barSetSegments(0);
}

void loop() {
  static const uint32_t STAGE_MS = 2000;
  static const uint32_t CYCLE_MS = STAGE_MS * 4;

  uint32_t t = millis() % CYCLE_MS;
  uint8_t stage = t / STAGE_MS; // 0..3

  switch (stage) {
    case 0: // Stage 1 (0-2s)
      digitalWrite(PIN_LED1, LOW);
      setRGBChannel(RGB1_R, RGB1_G, RGB1_B, true, false, false);   // red
      setRGBChannel(RGB2_R, RGB2_G, RGB2_B, false, false, false);  // off
      setRGBChannel(RGB3_R, RGB3_G, RGB3_B, false, false, false);  // off
      barSetSegments(1); // A1
      break;

    case 1: // Stage 2 (2-4s)
      digitalWrite(PIN_LED1, HIGH);
      setRGBChannel(RGB1_R, RGB1_G, RGB1_B, false, true, false);  // green
      setRGBChannel(RGB2_R, RGB2_G, RGB2_B, false, false, true);  // blue
      setRGBChannel(RGB3_R, RGB3_G, RGB3_B, false, false, false);  // off
      barSetSegments(2); // A1-A2
      break;

    case 2: // Stage 3 (4-6s)
      digitalWrite(PIN_LED1, LOW);
      setRGBChannel(RGB1_R, RGB1_G, RGB1_B, false, false, true);   // blue
      setRGBChannel(RGB2_R, RGB2_G, RGB2_B, true, false, false);   // red
      setRGBChannel(RGB3_R, RGB3_G, RGB3_B, false, true, false);   // green
      barSetSegments(3); // A1-A3
      break;

    case 3: // Stage 4 (6-8s)
    default:
      digitalWrite(PIN_LED1, HIGH);
      // All RGBs display white (all colors activated)
      setRGBChannel(RGB1_R, RGB1_G, RGB1_B, true, true, true);
      setRGBChannel(RGB2_R, RGB2_G, RGB2_B, true, true, true);
      setRGBChannel(RGB3_R, RGB3_G, RGB3_B, true, true, true);
      barSetSegments(4); // A1-A4
      break;
  }

  // No blocking delays; stage timing comes from millis().
}
