#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_84\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract bindings
// Inputs (active-low pushbuttons with pullups)
static constexpr uint8_t PIN_BTN1 = 2;   // io_0
static constexpr uint8_t PIN_BTN2 = 3;   // io_1
static constexpr uint8_t PIN_POT1 = A3;   // io_2
static constexpr uint8_t PIN_POT2 = A4;   // io_3
static constexpr uint8_t PIN_POT3 = A5;   // io_4

// Outputs (PWM to RGB LED cathode)
static constexpr uint8_t PIN_RGB_R = 4;  // io_5
static constexpr uint8_t PIN_RGB_G = 5;  // io_6
static constexpr uint8_t PIN_RGB_B = 6;  // io_7

// Controller state (must preserve exact initial values)
static int32_t mode = 1;                    // initial: 1
static bool previousBtn1Pressed = false;  // initial: false

// Control period: 5ms
static constexpr uint32_t CONTROL_PERIOD_MS = 5;

// Convert analog ADC (0..1023) to PWM (0..255) linearly.
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_84\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t adcToPwm(uint16_t adc);
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_84\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setPwm(uint8_t pin, uint8_t pwm);
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_84\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_84\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_84\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t adcToPwm(uint16_t adc) {
  // Exact proportional mapping: pwm = adc * 255 / 1023
  return (uint32_t(adc) * 255u) / 1023u;
}

static void setPwm(uint8_t pin, uint8_t pwm) {
  analogWrite(pin, pwm); // 0..255
}

void setup() {
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);

  pinMode(PIN_POT1, INPUT);
  pinMode(PIN_POT2, INPUT);
  pinMode(PIN_POT3, INPUT);

  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);

  // Initialization: rgb1 off -> initial PWM outputs are 0
  analogWrite(PIN_RGB_R, 0);
  analogWrite(PIN_RGB_G, 0);
  analogWrite(PIN_RGB_B, 0);
}

void loop() {
  static uint32_t nextTick = 0;
  const uint32_t now = millis();
  if (now < nextTick) return;
  nextTick = now + CONTROL_PERIOD_MS;

  // Read inputs and map to model semantics
  // active_high=false means model is true when button is pressed (LOW level)
  const bool io_0 = (digitalRead(PIN_BTN1) == LOW); // btn1 pressed => true
  const bool io_1 = (digitalRead(PIN_BTN2) == LOW); // btn2 pressed => true

  const uint16_t adc_2 = analogRead(PIN_POT1); // pot1
  const uint16_t adc_3 = analogRead(PIN_POT2); // pot2
  const uint16_t adc_4 = analogRead(PIN_POT3); // pot3

  // Model tick update (ACCEPTED executable semantics)
  // if io_1 then mode := 1; previousBtn1Pressed := io_0
  // else if io_0 and not previousBtn1Pressed then mode := (mode==4?1:mode+1)
  // then previousBtn1Pressed := io_0
  if (io_1) {
    mode = 1;
    previousBtn1Pressed = io_0;
  } else {
    if (io_0 && (!previousBtn1Pressed)) {
      mode = (mode == 4) ? 1 : (mode + 1);
    }
    previousBtn1Pressed = io_0;
  }

  // Model outputs:
  // io_5 = (mode==1 or mode==4) ? io_2 : 0
  // io_6 = (mode==2 or mode==4) ? io_3 : 0
  // io_7 = (mode==3 or mode==4) ? io_4 : 0
  // Note: io_2/io_3/io_4 are Real adc_count; map directly to PWM via linear mapping.
  uint8_t pwm_r = 0, pwm_g = 0, pwm_b = 0;

  if ((mode == 1) || (mode == 4)) {
    pwm_r = adcToPwm(adc_2);
  }
  if ((mode == 2) || (mode == 4)) {
    pwm_g = adcToPwm(adc_3);
  }
  if ((mode == 3) || (mode == 4)) {
    pwm_b = adcToPwm(adc_4);
  }

  // Outputs update every tick (immediate after button press / potentiometer adjustment)
  setPwm(PIN_RGB_R, pwm_r);
  setPwm(PIN_RGB_G, pwm_g);
  setPwm(PIN_RGB_B, pwm_b);
}
