#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_119\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static constexpr uint8_t PIN_LED  = 2;  // led1 -> D2
static constexpr uint8_t PIN_BTN1 = 3;  // btn1 -> D3 (pull-up, active_high=false)
static constexpr uint8_t PIN_BTN2 = 4;  // btn2 -> D4 (pull-up, active_high=false)
static constexpr uint8_t PIN_S1   = 5;  // servo1 -> D5
static constexpr uint8_t PIN_S2   = 6;  // servo2 -> D6

static constexpr uint32_t CONTROL_PERIOD_MS = 5;
static constexpr uint32_t FLASH_PERIOD_MS_ON  = 2000; // 2s on
static constexpr uint32_t FLASH_PERIOD_MS_OFF = 2000; // 2s off
static constexpr uint32_t FLASH_PERIOD_MS     = 4000; // on+off

// Accepted model state
static bool modeConflict   = false;
static bool ledGate         = false;
static bool previousBtn1   = false;
static bool previousBtn2   = false;
static int32_t ledPulseAccumMs = 0; // corresponds to io_5 accumulation

// Outputs
static float io_0_deg = 0.0f; // servo1 angle command
static float io_1_deg = 0.0f; // servo2 angle command
static bool  io_2_led = false;

// Hardware
Servo servo1;
Servo servo2;

#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_119\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readButtonActiveHighModelTrue(uint8_t pin);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_119\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void applyOutputs();
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_119\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_119\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_119\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline bool readButtonActiveHighModelTrue(uint8_t pin) {
  // Buttons are wired to use INPUT_PULLUP with active_high=false:
  // - model true when pressed => electrical LOW
  return (digitalRead(pin) == LOW);
}

void applyOutputs() {
  // Servo angle commands: hold_last_valid policy implied by Servo attach updates.
  servo1.write((int)io_0_deg);
  servo2.write((int)io_1_deg);
  digitalWrite(PIN_LED, io_2_led ? HIGH : LOW); // active_high=true
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);

  servo1.attach(PIN_S1);
  servo2.attach(PIN_S2);

  // Initialization: both servos at 0 deg and LED off
  io_0_deg = 0.0f;
  io_1_deg = 0.0f;
  io_2_led = false;
  applyOutputs();
}

void loop() {
  static uint32_t lastTickMs = millis();

  // Enforce 5 ms control period; compute delta since previous step for io_5
  uint32_t now = millis();
  uint32_t elapsed = now - lastTickMs;
  if (elapsed < CONTROL_PERIOD_MS) {
    // Small wait to reduce jitter
    delay(CONTROL_PERIOD_MS - elapsed);
    now = millis();
    elapsed = now - lastTickMs;
  }
  lastTickMs = now;

  // io inputs
  bool io_3_btn1 = readButtonActiveHighModelTrue(PIN_BTN1); // model true when pressed
  bool io_4_btn2 = readButtonActiveHighModelTrue(PIN_BTN2); // model true when pressed
  int32_t io_5_deltaMs = (int32_t)elapsed;                 // elapsed_ms delta_since_previous_step

  // === Controller.tick() exact logic ===

  // updates
  if (io_3_btn1 && io_4_btn2) {
    modeConflict = true;
  } else if (!modeConflict) {
    if ((io_3_btn1 && (!previousBtn1)) && io_4_btn2) {
      modeConflict = true;
    }
    if ((io_4_btn2 && (!previousBtn2)) && io_3_btn1) {
      modeConflict = true;
    }
  } else {
    // modeConflict == true
    if ( ( (io_3_btn1 && (!io_4_btn2)) || (io_4_btn2 && (!io_3_btn1)) ) ) {
      modeConflict = false;
      ledPulseAccumMs = 0;
      ledGate = false;
    }
  }

  if (modeConflict) {
    ledPulseAccumMs = ledPulseAccumMs + io_5_deltaMs;
    if (ledPulseAccumMs >= (int32_t)FLASH_PERIOD_MS) {
      ledPulseAccumMs = ledPulseAccumMs - (int32_t)FLASH_PERIOD_MS; // may be >4000 only if large delta; model subtracts once
    }
    ledGate = (ledPulseAccumMs < (int32_t)FLASH_PERIOD_MS_ON);
  } else {
    ledGate = false;
    ledPulseAccumMs = 0;
  }

  previousBtn1 = io_3_btn1;
  previousBtn2 = io_4_btn2;

  // outputs io_0, io_1, io_2
  io_0_deg = modeConflict ? 0.0f : (io_3_btn1 ? 90.0f : 0.0f);
  io_1_deg = modeConflict ? 0.0f : (io_4_btn2 ? 180.0f : 0.0f);
  io_2_led = ledGate;

  applyOutputs();
}
