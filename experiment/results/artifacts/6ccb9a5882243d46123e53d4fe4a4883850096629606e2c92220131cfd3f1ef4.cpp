#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Pins (Arduino Mega 2560 as per frozen contract)
static const uint8_t BTN1_PIN = 2;   // io_0
static const uint8_t BTN2_PIN = 3;   // io_1
static const uint8_t BTN3_PIN = 4;   // io_2

static const uint8_t RGB_RED_PIN   = 5;  // io_3  (PWM)
static const uint8_t RGB_GREEN_PIN = 6;  // io_4  (PWM)
static const uint8_t RGB_BLUE_PIN  = 7;  // io_5  (PWM)

static const uint8_t SEG_PINS[8] = { // io_6_segment_0..7
  8, 9, 10, 11, 12, 13, 44, 45
};

// Controller state (exact model variables)
static float redIntensity = 0.0f;
static float greenIntensity = 0.0f;
static float blueIntensity = 0.0f;

static bool previousBtn1Pressed = false;
static bool previousBtn2Pressed = false;
static bool previousBtn3Pressed = false;

static int32_t activeColor = 0; // 0=none, 1=red,2=green,3=blue

// I/O current samples (digital_boolean model: true_level=LOW, false_level=HIGH; pullup=true)
static bool io_0 = false;
static bool io_1 = false;
static bool io_2 = false;

// Bargraph outputs (computed from model outputs)
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyBargraphFromModel();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyRgbPwmFromModel();
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void tickModel();
#line 91 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyBargraphFromModel() {
  // Model:
  // segment_n is active iff activeColor==1 and redIntensity >= threshold_n
  // or activeColor==2 and greenIntensity >= threshold_n
  // or activeColor==3 and blueIntensity >= threshold_n
  // thresholds: 32,64,96,128,160,192,224,256 (segment_0..7)
  const float thresholds[8] = {32.0f, 64.0f, 96.0f, 128.0f, 160.0f, 192.0f, 224.0f, 256.0f};

  for (int i = 0; i < 8; ++i) {
    bool on = false;
    if (activeColor == 1) {
      on = (redIntensity >= thresholds[i]);
    } else if (activeColor == 2) {
      on = (greenIntensity >= thresholds[i]);
    } else if (activeColor == 3) {
      on = (blueIntensity >= thresholds[i]);
    } else {
      on = false;
    }
    digitalWrite(SEG_PINS[i], on ? HIGH : LOW); // active_high=true
  }
}

static void applyRgbPwmFromModel() {
  // Model outputs are real pwm_count values equal to intensities (0..255 by construction)
  // analogWrite expects 0..255 integers. Cast preserves behavior.
  analogWrite(RGB_RED_PIN,   (int)redIntensity);
  analogWrite(RGB_GREEN_PIN, (int)greenIntensity);
  analogWrite(RGB_BLUE_PIN,  (int)blueIntensity);
}

// One model tick: matches frozen SysML2 action tick logic (no extra debounce timing beyond edge gating)
static void tickModel() {
  // Inputs sampled already into io_0..io_2
  if (io_0 && (!previousBtn1Pressed)) {
    redIntensity = (redIntensity == 255.0f) ? 0.0f : (redIntensity + 32.0f);
    activeColor = 1;
  }

  if (io_1 && (!previousBtn2Pressed)) {
    greenIntensity = (greenIntensity == 255.0f) ? 0.0f : (greenIntensity + 32.0f);
    activeColor = 2;
  }

  if (io_2 && (!previousBtn3Pressed)) {
    blueIntensity = (blueIntensity == 255.0f) ? 0.0f : (blueIntensity + 32.0f);
    activeColor = 3;
  }

  previousBtn1Pressed = io_0;
  previousBtn2Pressed = io_1;
  previousBtn3Pressed = io_2;

  // Apply outputs from updated state
  applyRgbPwmFromModel();
  applyBargraphFromModel();
}

void setup() {
  // Inputs with internal pullups, active-high=false => pressed = LOW
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // Outputs: PWM and bargraph segments
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  for (int i = 0; i < 8; ++i) {
    pinMode(SEG_PINS[i], OUTPUT);
  }

  // Initialization: RGB LED and bar graph off
  redIntensity = 0.0f;
  greenIntensity = 0.0f;
  blueIntensity = 0.0f;
  previousBtn1Pressed = false;
  previousBtn2Pressed = false;
  previousBtn3Pressed = false;
  activeColor = 0;

  // Outputs: off (PWM=0, bar segments all false)
  applyRgbPwmFromModel();
  applyBargraphFromModel();
}

void loop() {
  // Control period granularity 5ms (frozen contract)
  static uint32_t lastTickMs = 0;
  uint32_t now = millis();
  if (now - lastTickMs < 5) return;
  lastTickMs = now;

  // Sample inputs: io_k = (digital read == true_level LOW)
  io_0 = (digitalRead(BTN1_PIN) == LOW);
  io_1 = (digitalRead(BTN2_PIN) == LOW);
  io_2 = (digitalRead(BTN3_PIN) == LOW);

  tickModel();
}
