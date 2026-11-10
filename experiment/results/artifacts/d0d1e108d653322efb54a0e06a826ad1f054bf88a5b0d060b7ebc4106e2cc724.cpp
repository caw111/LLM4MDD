#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_96\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract (Arduino Mega 2560 bindings)
static constexpr uint8_t LED1_PIN = 2;   // io_0
static constexpr uint8_t LED2_PIN = 3;   // io_1
static constexpr uint8_t LED3_PIN = 4;   // io_2

// bargraph1 segments (physical A1..A10) => pins D5..D44 in order as given
static constexpr uint8_t BAR_PINS[10] = {
  5,   // io_3_segment_0 (A1)
  6,   // io_3_segment_1 (A2)
  7,   // io_3_segment_2 (A3)
  8,   // io_3_segment_3 (A4)
  9,   // io_3_segment_4 (A5)
  10,  // io_3_segment_5 (A6)
  11,  // io_3_segment_6 (A7)
  12,  // io_3_segment_7 (A8)
  13,  // io_3_segment_8 (A9)
  44   // io_3_segment_9 (A10)
};

// Model states
static int32_t phaseAccumulatedMs = 0; // initial 0
static int32_t phaseIndex = 0;         // initial 0
static bool led1State = false;
static bool led2State = false;
static bool led3State = false;

static unsigned long lastControlMs = 0;

#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_96\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_96\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_96\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_96\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs()
{
  // Outputs io_0..io_2
  digitalWrite(LED1_PIN, led1State ? HIGH : LOW);
  digitalWrite(LED2_PIN, led2State ? HIGH : LOW);
  digitalWrite(LED3_PIN, led3State ? HIGH : LOW);

  // Outputs io_3_segment_0..io_3_segment_9
  // As per accepted model:
  // segment_0: phaseIndex==3 || phaseIndex in {0,1,2}
  // segment_1: phaseIndex==3 || phaseIndex==1
  // segments_2..9: true only when phaseIndex==3
  for (int i = 0; i < 10; i++) {
    bool segOn = false;
    switch (i) {
      case 0:
        segOn = (phaseIndex == 3) || (phaseIndex == 0) || (phaseIndex == 1) || (phaseIndex == 2);
        break;
      case 1:
        segOn = (phaseIndex == 3) || (phaseIndex == 1);
        break;
      default:
        segOn = (phaseIndex == 3);
        break;
    }
    digitalWrite(BAR_PINS[i], segOn ? HIGH : LOW);
  }
}

void setup()
{
  // Per requirement: all LEDs and bar graph segments off at start/reset.
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  for (int i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
  }

  led1State = false;
  led2State = false;
  led3State = false;
  phaseAccumulatedMs = 0;
  phaseIndex = 0;

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  for (int i = 0; i < 10; i++) digitalWrite(BAR_PINS[i], LOW);

  lastControlMs = millis();
}

void loop()
{
  // Implement control_period_ms = 10 (elapsed_ms input is delta since previous control step)
  static uint32_t nextTickMs = 0;
  if (nextTickMs == 0) nextTickMs = millis();

  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) < 0) {
    return;
  }
  nextTickMs = nextTickMs + 10;

  // io_4: actual delta since previous control step; used for 2s phase durations
  unsigned long currentMs = now;
  int32_t io_4 = (int32_t)(currentMs - lastControlMs);
  lastControlMs = currentMs;
  if (io_4 < 0) io_4 = 0; // defensive

  // ACCEPTED executable model: Controller.tick
  // start
  // phaseAccumulatedMs := phaseAccumulatedMs + io_4
  phaseAccumulatedMs = phaseAccumulatedMs + io_4;

  // if phaseAccumulatedMs >= 2000 then
  //   phaseAccumulatedMs := phaseAccumulatedMs - 2000
  //   phaseIndex := (if phaseIndex == 3 then 0 else phaseIndex + 1)
  if (phaseAccumulatedMs >= 2000) {
    phaseAccumulatedMs = phaseAccumulatedMs - 2000;
    phaseIndex = (phaseIndex == 3) ? 0 : (phaseIndex + 1);
  }

  // if phaseIndex == 0 then led1State := true; led2State := false; led3State := false
  // else if phaseIndex == 1 then led1State := false; led2State := true; led3State := false
  // else if phaseIndex == 2 then led1State := false; led2State := false; led3State := true
  // else if phaseIndex == 3 then led1State := false; led2State := false; led3State := false
  if (phaseIndex == 0) {
    led1State = true;
    led2State = false;
    led3State = false;
  } else if (phaseIndex == 1) {
    led1State = false;
    led2State = true;
    led3State = false;
  } else if (phaseIndex == 2) {
    led1State = false;
    led2State = false;
    led3State = true;
  } else { // phaseIndex == 3 (and any other unexpected value)
    led1State = false;
    led2State = false;
    led3State = false;
  }

  // out attributes mapping
  applyOutputs();
}
