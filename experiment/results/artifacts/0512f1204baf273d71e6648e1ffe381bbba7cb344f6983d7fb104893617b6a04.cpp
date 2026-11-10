#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_71\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Servo.h>

static constexpr uint8_t BTN1_PIN = 2;   // io_0, active-low
static constexpr uint8_t BTN2_PIN = 3;   // io_1, active-low
static constexpr uint8_t POT1_PIN = A3;  // io_2
static constexpr uint8_t POT2_PIN = A4;  // io_3

static constexpr uint8_t SERVO1_PWM_PIN = 4; // io_4
static constexpr uint8_t SERVO2_PWM_PIN = 5; // io_5

// Model configuration
static constexpr uint32_t CONTROL_PERIOD_MS = 5; // frozen contract
static constexpr int DEBOUNCE_COUNTER_INIT = 5;  // from model

struct ControllerModel {
  bool  previousBtn1Pressed = false;
  bool  previousBtn2Pressed = false;
  bool  btn1StablePressed   = false;
  bool  btn2StablePressed   = false;
  int32_t btn1DebounceCounter = 0;
  int32_t btn2DebounceCounter = 0;
  double servo1Angle = 0.0;
  double servo2Angle = 0.0;

  void tick(bool io_0, bool io_1, double io_2, double io_3) {
    // Replicate the exact update ordering/conditions from the frozen model.
    if (io_0 && !previousBtn1Pressed) {
      btn1DebounceCounter = DEBOUNCE_COUNTER_INIT;
    }

    if (io_0 && btn1DebounceCounter > 0) {
      btn1DebounceCounter = btn1DebounceCounter - 1;
    }

    if (!io_0) {
      btn1DebounceCounter = 0;
      btn1StablePressed = false;
    }

    if (io_0 && btn1DebounceCounter == 0) {
      btn1StablePressed = true;
    }

    if (btn1StablePressed && !previousBtn1Pressed) {
      // Linear clamped mapping [0..1023] -> [0..180], with exact boundary behavior
      if (io_2 <= 0.0) {
        servo1Angle = 0.0;
      } else if (io_2 >= 1023.0) {
        servo1Angle = 180.0;
      } else {
        servo1Angle = io_2 * 180.0 / 1023.0;
      }
    }

    previousBtn1Pressed = io_0;

    if (io_1 && !previousBtn2Pressed) {
      btn2DebounceCounter = DEBOUNCE_COUNTER_INIT;
    }

    if (io_1 && btn2DebounceCounter > 0) {
      btn2DebounceCounter = btn2DebounceCounter - 1;
    }

    if (!io_1) {
      btn2DebounceCounter = 0;
      btn2StablePressed = false;
    }

    if (io_1 && btn2DebounceCounter == 0) {
      btn2StablePressed = true;
    }

    if (btn2StablePressed && !previousBtn2Pressed) {
      if (io_3 <= 0.0) {
        servo2Angle = 0.0;
      } else if (io_3 >= 1023.0) {
        servo2Angle = 180.0;
      } else {
        servo2Angle = io_3 * 180.0 / 1023.0;
      }
    }

    previousBtn2Pressed = io_1;
  }
};

Servo servo1;
Servo servo2;
ControllerModel model;

#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_71\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readButtonActiveLow(uint8_t pin);
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_71\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static double readPotAdc(uint8_t pin);
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_71\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void applyOutputs();
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_71\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 123 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_71\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_71\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline bool readButtonActiveLow(uint8_t pin) {
  // Model semantics: true level is LOW; active_high=false; pullup=true.
  return (digitalRead(pin) == LOW);
}

static inline double readPotAdc(uint8_t pin) {
  // io_2/io_3 are in adc_count, no scaling offset
  return static_cast<double>(analogRead(pin)); // 0..1023
}

void applyOutputs() {
  // Servo outputs: io_4/io_5 with invalid_policy hold_last_valid.
  // Model always holds last valid; apply current model angles each tick.
  servo1.write(static_cast<int>(model.servo1Angle + 0.5));
  servo2.write(static_cast<int>(model.servo2Angle + 0.5));
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  servo1.attach(SERVO1_PWM_PIN);
  servo2.attach(SERVO2_PWM_PIN);

  // Initialization: set both servos to 0 degrees.
  model.servo1Angle = 0.0;
  model.servo2Angle = 0.0;
  servo1.write(0);
  servo2.write(0);
}

void loop() {
  static uint32_t lastTickMs = 0;
  uint32_t now = millis();
  if (now - lastTickMs < CONTROL_PERIOD_MS) return;
  lastTickMs = now;

  // Sample inputs at control period
  bool io_0 = readButtonActiveLow(BTN1_PIN); // btn1 pressed gate (debounced in model)
  bool io_1 = readButtonActiveLow(BTN2_PIN); // btn2 pressed gate (debounced in model)
  double io_2 = readPotAdc(POT1_PIN);       // pot1 adc_count
  double io_3 = readPotAdc(POT2_PIN);       // pot2 adc_count

  // Update controller
  model.tick(io_0, io_1, io_2, io_3);

  // Apply outputs
  applyOutputs();
}
