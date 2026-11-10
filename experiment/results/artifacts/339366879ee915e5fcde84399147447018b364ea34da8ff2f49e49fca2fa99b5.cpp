#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_001\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

//
// Frozen interface contract (period 10ms):
// io_0: input  f64  adc_count  A0
// io_1: output f64  pwm_count  D9 (PWM LED brightness; stronger light => dimmer => inverted mapping)
// io_2: output i32  Hz         D6 (buzzer short鸣 when io_0 > 800; frequency modeled as 1000, invalid_policy=silence when false)
// io_3: output bool 1         D13 (digital indicator; true when io_0 > 800)
//

static const uint8_t PIN_IO_1_PWM_LED = 9;   // D9
static const uint8_t PIN_IO_3_INDICATOR = 13; // D13
static const uint8_t PIN_IO_2_BUZZER = 6;     // D6
static const uint8_t PIN_IO_0_LIGHT = A0;    // A0

static const uint32_t CONTROL_PERIOD_MS = 10;
static uint32_t nextTickMs = 0;

#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_001\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setPwmCountOnD9(double pwmCount);
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_001\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setToneOnD6(int hz);
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_001\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_001\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_001\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setPwmCountOnD9(double pwmCount)
{
  // io_1 is a "pwm_count" (0..1023). Use analogWrite with 0..255 mapping.
  if (pwmCount < 0.0) pwmCount = 0.0;
  if (pwmCount > 1023.0) pwmCount = 1023.0;

  uint8_t pwm255 = (uint8_t)((pwmCount * 255.0 + 511.0 / 2.0) / 1023.0); // rounded
  analogWrite(PIN_IO_1_PWM_LED, pwm255);
}

static void setToneOnD6(int hz)
{
  // Contract says tone_output D6 with invalid_policy="silence".
  // If hz == 0 => silence. Otherwise play short鸣 at given frequency.
  // Use tone() directly each step when enabled; stop otherwise.
  if (hz > 0) {
    tone(PIN_IO_2_BUZZER, hz);
  } else {
    noTone(PIN_IO_2_BUZZER);
  }
}

void setup() {
  pinMode(PIN_IO_1_PWM_LED, OUTPUT);
  pinMode(PIN_IO_3_INDICATOR, OUTPUT);
  pinMode(PIN_IO_2_BUZZER, OUTPUT);

  // Initialize outputs to model initial values:
  // io_1 initial 0 => LED off
  // io_2 initial 0 => buzzer silent
  // io_3 initial false => D13 LOW (active_high=true)
  setPwmCountOnD9(0.0);
  setToneOnD6(0);
  digitalWrite(PIN_IO_3_INDICATOR, LOW);

  nextTickMs = millis();
}

void loop() {
  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) < 0) return;

  // Enforce exact periodic stepping based on the fixed period.
  nextTickMs += CONTROL_PERIOD_MS;

  // Read io_0: adc_count from A0 as Real
  int adc = analogRead(PIN_IO_0_LIGHT); // 0..1023
  double io_0 = (double)adc;

  // Accepted executable model outputs (exact logic from bundle):
  // io_1:
  // if io_0 > 800 then 0.0
  // else if io_0 > 0.0 then 1023.0 - floor(0.0 - 0.0)
  // else 1023.0
  // The floor expression is floor(0.0) = 0.0, so io_1 is:
  //   if io_0 > 800 => 0
  //   else if io_0 > 0 => 1023
  //   else => 1023
  double io_1;
  if (io_0 > 800.0) {
    io_1 = 0.0;
  } else {
    if (io_0 > 0.0) {
      // Preserve structure: 1023.0 - floor(0.0 - 0.0)
      double inner = 0.0 - 0.0;
      double flo = floor(inner);
      io_1 = 1023.0 - flo;
    } else {
      io_1 = 1023.0;
    }
  }

  // io_2:
  // if io_0 > 800 then 1000 else 0
  int io_2 = (io_0 > 800.0) ? 1000 : 0;

  // io_3:
  // if io_0 > 800 then true else false
  bool io_3 = (io_0 > 800.0);

  // Apply outputs in the same step
  setPwmCountOnD9(io_1);

  setToneOnD6(io_2);

  digitalWrite(PIN_IO_3_INDICATOR, io_3 ? HIGH : LOW);
}
