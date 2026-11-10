#include <Arduino.h>

// Frozen interface contract bindings (Arduino Mega 2560)
static const uint8_t PIN_LED1              = 2;   // io_0
static const uint8_t PIN_RGB1_R            = 3;   // io_1
static const uint8_t PIN_RGB1_G            = 4;   // io_2
static const uint8_t PIN_RGB1_B            = 5;   // io_3
static const uint8_t PIN_RGB2_R            = 6;   // io_4
static const uint8_t PIN_RGB2_G            = 7;   // io_5
static const uint8_t PIN_RGB2_B            = 8;   // io_6
static const uint8_t PIN_RGB3_R            = 9;   // io_7
static const uint8_t PIN_RGB3_G            = 10;  // io_8
static const uint8_t PIN_RGB3_B            = 11;  // io_9

static const uint8_t PIN_BAR_A1           = 12;  // io_10_segment_0
static const uint8_t PIN_BAR_A2           = 13;  // io_10_segment_1
static const uint8_t PIN_BAR_A3           = 44;  // io_10_segment_2
static const uint8_t PIN_BAR_A4           = 45;  // io_10_segment_3

// Accepted model: controller has stageIndex initial 1 and tick updates it.
// Each stage lasts exactly 2 seconds; update stage every 2 seconds.
static int stageIndex = 1;

static const unsigned long STAGE_DURATION_MS = 2000UL;

static inline void writeBool(uint8_t pin, bool value) {
  digitalWrite(pin, value ? HIGH : LOW); // active-high semantics per contract
}

static void applyOutputsForStage(int s) {
  // Model outputs exactly as specified by the accepted bundle.
  const bool io_0 = (s == 2 || s == 4); // led1 on in stages 2 & 4
  const bool io_1 = (s == 1);           // rgb1 red in stage 1 only
  const bool io_2 = (s == 2);           // rgb1 green in stage 2 only
  const bool io_3 = (s == 3);           // rgb1 blue in stage 3 only

  // rgb2 outputs:
  // io_4: rgb2 red true when stageIndex==3, else false (also false in stage2 & stage1)
  const bool io_4 = (s == 3);
  // io_5: rgb2 green true when stageIndex==4 (else false)
  const bool io_5 = (s == 4);
  // io_6: rgb2 blue true when stageIndex==2 or stageIndex==4, else false
  const bool io_6 = (s == 2 || s == 4);

  // rgb3 outputs:
  // io_7: rgb3 red true when stageIndex==4
  const bool io_7 = (s == 4);
  // io_8: rgb3 green true when stageIndex==3 or stageIndex==4
  const bool io_8 = (s == 3 || s == 4);
  // io_9: rgb3 blue true when stageIndex==4 only
  const bool io_9 = (s == 4);

  // bargraph outputs:
  const bool seg0 = true;                    // stage 1/2/3/4 always true
  const bool seg1 = (s == 2 || s == 3 || s == 4);
  const bool seg2 = (s == 3 || s == 4);
  const bool seg3 = (s == 4);

  // Apply to pins
  writeBool(PIN_LED1, io_0);

  writeBool(PIN_RGB1_R, io_1);
  writeBool(PIN_RGB1_G, io_2);
  writeBool(PIN_RGB1_B, io_3);

  writeBool(PIN_RGB2_R, io_4);
  writeBool(PIN_RGB2_G, io_5);
  writeBool(PIN_RGB2_B, io_6);

  writeBool(PIN_RGB3_R, io_7);
  writeBool(PIN_RGB3_G, io_8);
  writeBool(PIN_RGB3_B, io_9);

  writeBool(PIN_BAR_A1, seg0);
  writeBool(PIN_BAR_A2, seg1);
  writeBool(PIN_BAR_A3, seg2);
  writeBool(PIN_BAR_A4, seg3);
}

void setup() {
  // Initialize all LEDs OFF (contract initial false for all outputs, including bargraph segments).
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_RGB1_R, OUTPUT);
  pinMode(PIN_RGB1_G, OUTPUT);
  pinMode(PIN_RGB1_B, OUTPUT);
  pinMode(PIN_RGB2_R, OUTPUT);
  pinMode(PIN_RGB2_G, OUTPUT);
  pinMode(PIN_RGB2_B, OUTPUT);
  pinMode(PIN_RGB3_R, OUTPUT);
  pinMode(PIN_RGB3_G, OUTPUT);
  pinMode(PIN_RGB3_B, OUTPUT);

  pinMode(PIN_BAR_A1, OUTPUT);
  pinMode(PIN_BAR_A2, OUTPUT);
  pinMode(PIN_BAR_A3, OUTPUT);
  pinMode(PIN_BAR_A4, OUTPUT);

  // Off state for all outputs
  writeBool(PIN_LED1, false);
  writeBool(PIN_RGB1_R, false);
  writeBool(PIN_RGB1_G, false);
  writeBool(PIN_RGB1_B, false);
  writeBool(PIN_RGB2_R, false);
  writeBool(PIN_RGB2_G, false);
  writeBool(PIN_RGB2_B, false);
  writeBool(PIN_RGB3_R, false);
  writeBool(PIN_RGB3_G, false);
  writeBool(PIN_RGB3_B, false);
  writeBool(PIN_BAR_A1, false);
  writeBool(PIN_BAR_A2, false);
  writeBool(PIN_BAR_A3, false);
  writeBool(PIN_BAR_A4, false);

  // Accepted model initial state: stageIndex = 1
  stageIndex = 1;
  applyOutputsForStage(stageIndex);
}

void loop() {
  static unsigned long nextTickMs = millis() + STAGE_DURATION_MS;

  const unsigned long now = millis();
  if ((long)(now - nextTickMs) >= 0) {
    // perform action tick (stage cycling update)
    stageIndex = (stageIndex == 1) ? 2
                : (stageIndex == 2) ? 3
                : (stageIndex == 3) ? 4
                : 1;

    applyOutputsForStage(stageIndex);

    nextTickMs += STAGE_DURATION_MS; // preserve exact 2s periods without drift
  }
}