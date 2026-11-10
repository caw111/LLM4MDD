#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_52\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// Frozen interface contract: io_0..io_9
// Outputs:
//  io_0: servo1 angle command (deg) -> D8
//  io_1: servo2 angle command (deg) -> D9
//  io_2: rgb1 red off (fixed false) -> D2
//  io_3: rgb1 green off (fixed false) -> D3
//  io_4: rgb1 blue on  (fixed true) -> D4
//  io_5: rgb2 red on   (fixed true) -> D5
//  io_6: rgb2 green off (fixed false) -> D6
//  io_7: rgb2 blue off  (fixed false) -> D7
// Bargraph segments (6 LEDs):
//  io_8_segment_0 -> D10
//  io_8_segment_1 -> D11
//  io_8_segment_2 -> D12
//  io_8_segment_3 -> D13
//  io_8_segment_4 -> D44
//  io_8_segment_5 -> D45

// Inputs:
//  io_9: Actual delta since previous control step (ms, i32)

static const uint8_t PIN_SERVO1 = 8;
static const uint8_t PIN_SERVO2 = 9;

static const uint8_t PIN_RGB1_R = 2;
static const uint8_t PIN_RGB1_G = 3;
static const uint8_t PIN_RGB1_B = 4;

static const uint8_t PIN_RGB2_R = 5;
static const uint8_t PIN_RGB2_G = 6;
static const uint8_t PIN_RGB2_B = 7;

static const uint8_t PIN_BAR_0 = 10;
static const uint8_t PIN_BAR_1 = 11;
static const uint8_t PIN_BAR_2 = 12;
static const uint8_t PIN_BAR_3 = 13;
static const uint8_t PIN_BAR_4 = 44;
static const uint8_t PIN_BAR_5 = 45;

Servo servo1;
Servo servo2;

// Controller state (exact model)
static float servo1Deg = 0.0f;
static float servo2Deg = 0.0f;
static int32_t accumMs = 0;

// Control period per contract
static const uint32_t CONTROL_PERIOD_MS = 10;

// Helper: digital output write with active-high semantics
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_52\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setBoolPin(uint8_t pin, bool value);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_52\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
int main(void);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_52\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void setBoolPin(uint8_t pin, bool value) {
  digitalWrite(pin, value ? HIGH : LOW);
}

int main(void) {
  init();
  // setup() content
  // Pin bindings for RGB channels
  pinMode(PIN_RGB1_R, OUTPUT);
  pinMode(PIN_RGB1_G, OUTPUT);
  pinMode(PIN_RGB1_B, OUTPUT);

  pinMode(PIN_RGB2_R, OUTPUT);
  pinMode(PIN_RGB2_G, OUTPUT);
  pinMode(PIN_RGB2_B, OUTPUT);

  // Pin bindings for bar graph
  pinMode(PIN_BAR_0, OUTPUT);
  pinMode(PIN_BAR_1, OUTPUT);
  pinMode(PIN_BAR_2, OUTPUT);
  pinMode(PIN_BAR_3, OUTPUT);
  pinMode(PIN_BAR_4, OUTPUT);
  pinMode(PIN_BAR_5, OUTPUT);

  // Initial outputs per contract initial values:
  // io_2=false io_3=false io_4=true io_5=true io_6=false io_7=false
  setBoolPin(PIN_RGB1_R, false); // io_2
  setBoolPin(PIN_RGB1_G, false); // io_3
  setBoolPin(PIN_RGB1_B, true);  // io_4
  setBoolPin(PIN_RGB2_R, true);  // io_5
  setBoolPin(PIN_RGB2_G, false); // io_6
  setBoolPin(PIN_RGB2_B, false); // io_7

  // Bargraph initial: all false
  setBoolPin(PIN_BAR_0, false);
  setBoolPin(PIN_BAR_1, false);
  setBoolPin(PIN_BAR_2, false);
  setBoolPin(PIN_BAR_3, false);
  setBoolPin(PIN_BAR_4, false);
  setBoolPin(PIN_BAR_5, false);

  // Servo attach and initial angle 0
  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);
  servo1.write((int)servo1Deg);
  servo2.write((int)servo2Deg);

  // emulate Arduino loop scheduling with exact elapsed ms deltas
  uint32_t lastTick = millis();

  // loop()
  while (true) {
    uint32_t now = millis();
    uint32_t elapsed = now - lastTick;

    if (elapsed >= CONTROL_PERIOD_MS) {
      lastTick += CONTROL_PERIOD_MS;
      // input io_9: Actual delta since previous control step (i32)
      int32_t io_9 = (int32_t)elapsed;

      // Tick action per model:
      // perform action tick {
      //   if accumMs + io_9 >= 2000:
      //     accumMs := accumMs + io_9 - 2000
      //     servo1Deg := (if servo1Deg >= 180 then 0 else if servo1Deg + 30 > 180 then 0 else servo1Deg + 30)
      //     servo2Deg similarly
      //   else accumMs := accumMs + io_9
      // }
      if ((int64_t)accumMs + (int64_t)io_9 >= 2000) {
        accumMs = accumMs + io_9 - 2000;

        // Update servo1Deg
        if (servo1Deg >= 180.0f) {
          servo1Deg = 0.0f;
        } else {
          float cand = servo1Deg + 30.0f;
          if (cand > 180.0f) {
            servo1Deg = 0.0f;
          } else {
            servo1Deg = cand;
          }
        }

        // Update servo2Deg
        if (servo2Deg >= 180.0f) {
          servo2Deg = 0.0f;
        } else {
          float cand = servo2Deg + 30.0f;
          if (cand > 180.0f) {
            servo2Deg = 0.0f;
          } else {
            servo2Deg = cand;
          }
        }
      } else {
        accumMs = accumMs + io_9;
      }

      // Outputs per model outputs mapping
      // io_0 := servo1Deg, io_1 := servo2Deg
      servo1.write((int)servo1Deg);
      servo2.write((int)servo2Deg);

      // io_2=false, io_3=false, io_4=true, io_5=true, io_6=false, io_7=false (constants)
      setBoolPin(PIN_RGB1_R, false);
      setBoolPin(PIN_RGB1_G, false);
      setBoolPin(PIN_RGB1_B, true);
      setBoolPin(PIN_RGB2_R, true);
      setBoolPin(PIN_RGB2_G, false);
      setBoolPin(PIN_RGB2_B, false);

      // Bargraph segments from expressions
      float avg = (servo1Deg + servo2Deg) / 2.0f;

      bool seg0 = false;
      if (avg <= 90.0f) {
        seg0 = (avg >= 0.0f) ? true : false;
      } else {
        seg0 = false;
      }

      bool seg1 = false;
      if (avg > 90.0f) {
        seg1 = (avg <= 120.0f) ? true : false;
      } else {
        seg1 = false;
      }

      bool seg2 = false;
      if (avg > 120.0f) {
        seg2 = (avg <= 135.0f) ? true : false;
      } else {
        seg2 = false;
      }

      bool seg3 = false;
      if (avg > 135.0f) {
        seg3 = (avg <= 150.0f) ? true : false;
      } else {
        seg3 = false;
      }

      bool seg4 = false;
      if (avg > 150.0f) {
        seg4 = (avg <= 165.0f) ? true : false;
      } else {
        seg4 = false;
      }

      bool seg5 = false;
      if (avg > 165.0f) {
        seg5 = (avg <= 180.0f) ? true : false;
      } else {
        seg5 = false;
      }

      setBoolPin(PIN_BAR_0, seg0);
      setBoolPin(PIN_BAR_1, seg1);
      setBoolPin(PIN_BAR_2, seg2);
      setBoolPin(PIN_BAR_3, seg3);
      setBoolPin(PIN_BAR_4, seg4);
      setBoolPin(PIN_BAR_5, seg5);
    }

    // Avoid busy spin
    delay(1);
  }

  return 0;
}
