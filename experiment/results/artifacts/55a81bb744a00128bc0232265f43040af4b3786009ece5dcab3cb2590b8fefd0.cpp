#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_50\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

namespace GeneratedModel {
  struct Controller {
    int32_t activeSegmentIndex;      // i32
    int32_t elapsedInSegmentMs;      // i32

    // Outputs (bool)
    bool io_0;              // rgb1 red   (D2)
    bool io_1;              // rgb1 green (D3)
    bool io_2;              // rgb1 blue  (D4)
    bool io_3;              // rgb2 red   (D5)
    bool io_4;              // rgb2 green (D6)
    bool io_5;              // rgb2 blue  (D7)

    bool io_6_segment_0;   // D8
    bool io_6_segment_1;   // D9
    bool io_6_segment_2;   // D10
    bool io_6_segment_3;   // D11
    bool io_6_segment_4;   // D12
    bool io_6_segment_5;   // D13
    bool io_6_segment_6;   // D44
    bool io_6_segment_7;   // D45

    Controller() { // initial attributes
      activeSegmentIndex = 0;
      elapsedInSegmentMs = 0;

      // initial outputs per accepted model bundle
      io_0 = false;
      io_1 = false;
      io_2 = false;
      io_3 = false;
      io_4 = true;
      io_5 = false;

      io_6_segment_0 = true;
      io_6_segment_1 = false;
      io_6_segment_2 = false;
      io_6_segment_3 = false;
      io_6_segment_4 = false;
      io_6_segment_5 = false;
      io_6_segment_6 = false;
      io_6_segment_7 = false;
    }

    void tick() {
      // ACCEPTED executable model: both branches set:
      // activeSegmentIndex := 0; elapsedInSegmentMs := 0;
      activeSegmentIndex = 0;
      elapsedInSegmentMs = 0;

      // Outputs remain those defined in model bundle. (No update rule changes them.)
      // (Preserve exact behavior.)
      io_0 = false;
      io_1 = false;
      io_2 = false;
      io_3 = false;
      io_4 = true;
      io_5 = false;

      io_6_segment_0 = true;
      io_6_segment_1 = false;
      io_6_segment_2 = false;
      io_6_segment_3 = false;
      io_6_segment_4 = false;
      io_6_segment_5 = false;
      io_6_segment_6 = false;
      io_6_segment_7 = false;
    }
  };
}

using namespace GeneratedModel;

// Hardware pin bindings (active_high digital outputs)
static constexpr uint8_t PIN_io_0             = 2;   // D2  rgb1 red
static constexpr uint8_t PIN_io_1             = 3;   // D3  rgb1 green
static constexpr uint8_t PIN_io_2             = 4;   // D4  rgb1 blue
static constexpr uint8_t PIN_io_3             = 5;   // D5  rgb2 red
static constexpr uint8_t PIN_io_4             = 6;   // D6  rgb2 green
static constexpr uint8_t PIN_io_5             = 7;   // D7  rgb2 blue

static constexpr uint8_t PIN_io_6_segment_0  = 8;   // D8  bar A1
static constexpr uint8_t PIN_io_6_segment_1  = 9;   // D9  bar A2
static constexpr uint8_t PIN_io_6_segment_2  = 10;  // D10 bar A3
static constexpr uint8_t PIN_io_6_segment_3  = 11;  // D11 bar A4
static constexpr uint8_t PIN_io_6_segment_4  = 12;  // D12 bar A5
static constexpr uint8_t PIN_io_6_segment_5  = 13;  // D13 bar A6
static constexpr uint8_t PIN_io_6_segment_6  = 44;  // D44 bar A7
static constexpr uint8_t PIN_io_6_segment_7  = 45;  // D45 bar A8

static constexpr uint32_t CONTROL_PERIOD_MS = 10;

static Controller controller;

#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_50\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeBoolPin(uint8_t pin, bool v);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_50\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_50\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_50\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeBoolPin(uint8_t pin, bool v) {
  digitalWrite(pin, v ? HIGH : LOW);
}

void setup() {
  pinMode(PIN_io_0, OUTPUT);
  pinMode(PIN_io_1, OUTPUT);
  pinMode(PIN_io_2, OUTPUT);
  pinMode(PIN_io_3, OUTPUT);
  pinMode(PIN_io_4, OUTPUT);
  pinMode(PIN_io_5, OUTPUT);

  pinMode(PIN_io_6_segment_0, OUTPUT);
  pinMode(PIN_io_6_segment_1, OUTPUT);
  pinMode(PIN_io_6_segment_2, OUTPUT);
  pinMode(PIN_io_6_segment_3, OUTPUT);
  pinMode(PIN_io_6_segment_4, OUTPUT);
  pinMode(PIN_io_6_segment_5, OUTPUT);
  pinMode(PIN_io_6_segment_6, OUTPUT);
  pinMode(PIN_io_6_segment_7, OUTPUT);

  // Initialize outputs to match model initial state (all off except per bundle initial oracle outputs).
  writeBoolPin(PIN_io_0, controller.io_0);
  writeBoolPin(PIN_io_1, controller.io_1);
  writeBoolPin(PIN_io_2, controller.io_2);
  writeBoolPin(PIN_io_3, controller.io_3);
  writeBoolPin(PIN_io_4, controller.io_4);
  writeBoolPin(PIN_io_5, controller.io_5);

  writeBoolPin(PIN_io_6_segment_0, controller.io_6_segment_0);
  writeBoolPin(PIN_io_6_segment_1, controller.io_6_segment_1);
  writeBoolPin(PIN_io_6_segment_2, controller.io_6_segment_2);
  writeBoolPin(PIN_io_6_segment_3, controller.io_6_segment_3);
  writeBoolPin(PIN_io_6_segment_4, controller.io_6_segment_4);
  writeBoolPin(PIN_io_6_segment_5, controller.io_6_segment_5);
  writeBoolPin(PIN_io_6_segment_6, controller.io_6_segment_6);
  writeBoolPin(PIN_io_6_segment_7, controller.io_6_segment_7);
}

void loop() {
  static uint32_t last = millis();
  uint32_t now = millis();

  // Run controller at control period 10ms.
  if ((uint32_t)(now - last) >= CONTROL_PERIOD_MS) {
    last += CONTROL_PERIOD_MS; // preserve 10ms cadence

    controller.tick();

    // Apply outputs (active_high)
    writeBoolPin(PIN_io_0, controller.io_0);
    writeBoolPin(PIN_io_1, controller.io_1);
    writeBoolPin(PIN_io_2, controller.io_2);
    writeBoolPin(PIN_io_3, controller.io_3);
    writeBoolPin(PIN_io_4, controller.io_4);
    writeBoolPin(PIN_io_5, controller.io_5);

    writeBoolPin(PIN_io_6_segment_0, controller.io_6_segment_0);
    writeBoolPin(PIN_io_6_segment_1, controller.io_6_segment_1);
    writeBoolPin(PIN_io_6_segment_2, controller.io_6_segment_2);
    writeBoolPin(PIN_io_6_segment_3, controller.io_6_segment_3);
    writeBoolPin(PIN_io_6_segment_4, controller.io_6_segment_4);
    writeBoolPin(PIN_io_6_segment_5, controller.io_6_segment_5);
    writeBoolPin(PIN_io_6_segment_6, controller.io_6_segment_6);
    writeBoolPin(PIN_io_6_segment_7, controller.io_6_segment_7);
  }
}
