#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static const uint8_t PIN_LED1_PWM = 2;   // io_1
static const uint8_t PIN_RGB1_R   = 3;   // io_2
static const uint8_t PIN_RGB1_G   = 4;   // io_3
static const uint8_t PIN_RGB1_B   = 5;   // io_4
static const uint8_t PIN_RGB2_R   = 6;   // io_5
static const uint8_t PIN_RGB2_G   = 7;   // io_6
static const uint8_t PIN_RGB2_B   = 8;   // io_7
static const uint8_t PIN_SERVO1   = 9;   // io_8
static const uint8_t PIN_SERVO2   = 10;  // io_9
static const uint8_t PIN_SERVO3   = 11;  // io_10

// SysML controller state (frozen model)
static int32_t ledAccumMs = 0;
static int32_t rainbowAccumMs = 0;
static int32_t servoAccumMs = 0;
static int32_t servoPhase = 0;
static int32_t rainbowStep = 0;
static bool ledOn = false;

// Timing input io_0: delta since previous control step (ms)
static uint32_t lastTickMs = 0;

static Servo servo1;
static Servo servo2;
static Servo servo3;

#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setLed1PWM(float pwmCount);
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setRgbPWM(float r, float g, float b, uint8_t pinR, uint8_t pinG, uint8_t pinB);
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 140 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 172 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void setLed1PWM(float pwmCount) {
  // io_1 is pwm_count with scale 1.0, offset 0.0, expected 0 or 255.
  // Arduino analogWrite accepts 0..255.
  int v = (int)lrintf(pwmCount);
  if (v < 0) v = 0;
  if (v > 255) v = 255;
  analogWrite(PIN_LED1_PWM, v);
}

static inline void setRgbPWM(float r, float g, float b,
                              uint8_t pinR, uint8_t pinG, uint8_t pinB) {
  auto clamp255 = [](int x) -> int {
    if (x < 0) return 0;
    if (x > 255) return 255;
    return x;
  };
  int vr = clamp255((int)lrintf(r));
  int vg = clamp255((int)lrintf(g));
  int vb = clamp255((int)lrintf(b));
  analogWrite(pinR, vr);
  analogWrite(pinG, vg);
  analogWrite(pinB, vb);
}

static void applyOutputs() {
  // Outputs exactly as in the frozen model
  // io_1
  setLed1PWM(ledOn ? 255.0f : 0.0f);

  // RGB rainbow based on rainbowStep (0..6)
  // rgb1: io_2 (R), io_3 (G), io_4 (B)
  float rgb1R, rgb1G, rgb1B;
  float rgb2R, rgb2G, rgb2B;

  // Replicate nested conditional logic precisely via switch for safety:
  // rainbows: 0=red, 1=green, 2=blue, 3=yellow, 4=cyan, 5=magenta, 6=white
  switch (rainbowStep) {
    case 0: // red
      rgb1R = 255; rgb1G = 0;   rgb1B = 0;
      break;
    case 1: // green
      rgb1R = 0;   rgb1G = 255; rgb1B = 0;
      break;
    case 2: // blue
      rgb1R = 0;   rgb1G = 0;   rgb1B = 255;
      break;
    case 3: // yellow (R+B in model)
      rgb1R = 255; rgb1G = 255; rgb1B = 0; // Wait: model for rgb1 at step 3:
      // io_2=255, io_3=255, io_4=0 per expression.
      // But model expression for rgb1 blue at step 3 is 0.0 (correct).
      // However rgb1 green at step 3 is 255.0 (from io_3 expression).
      break;
    case 4: // cyan
      rgb1R = 0;   rgb1G = 255; rgb1B = 255; // model: R=0, G=255, B=255
      break;
    case 5: // magenta
      rgb1R = 255; rgb1G = 0;   rgb1B = 255; // model: R=255, G=0, B=255
      break;
    case 6: // white
    default: // model step 6 yields white: R=255,G=255,B=0? No, check model expressions:
      // For rgb1: io_2 step6=255, io_3 step6=255, io_4 step6=0 (per given model).
      // That would be yellow not white. But the frozen model's expressions explicitly define step6 as:
      // io_2=255, io_3=255, io_4=0.0 for rgb1 blue at step6.
      // For rgb2: io_5=255, io_6=255, io_7=0.0.
      // So we must follow the model, not the prose.
      rgb1R = 255; rgb1G = 255; rgb1B = 0;
      break;
  }

  // rgb2 exactly matches model expressions for steps 0..6
  switch (rainbowStep) {
    case 0: // red
      rgb2R = 255; rgb2G = 0;   rgb2B = 0;
      break;
    case 1: // green
      rgb2R = 0;   rgb2G = 255; rgb2B = 0;
      break;
    case 2: // blue
      rgb2R = 0;   rgb2G = 0;   rgb2B = 255;
      break;
    case 3: // yellow (per model: R=255,G=255,B=0)
      rgb2R = 255; rgb2G = 255; rgb2B = 0;
      break;
    case 4: // cyan (per model: R=0,G=255,B=255)
      rgb2R = 0;   rgb2G = 255; rgb2B = 255;
      break;
    case 5: // magenta (per model: R=255,G=0,B=255)
      rgb2R = 255; rgb2G = 0;   rgb2B = 255;
      break;
    case 6: // model step 6: R=255,G=255,B=0
    default:
      rgb2R = 255; rgb2G = 255; rgb2B = 0;
      break;
  }

  setRgbPWM(rgb1R, rgb1G, rgb1B, PIN_RGB1_R, PIN_RGB1_G, PIN_RGB1_B);
  setRgbPWM(rgb2R, rgb2G, rgb2B, PIN_RGB2_R, PIN_RGB2_G, PIN_RGB2_B);

  // Servo outputs io_8/io_9/io_10
  if (servoPhase == 0) {
    servo1.write(0);   // io_8
    servo2.write(180); // io_9
    servo3.write(90);  // io_10
  } else {
    servo1.write(180); // io_8
    servo2.write(0);   // io_9
    servo3.write(135); // io_10
  }
}

void setup() {
  // Frozen model initial outputs must be applied:
  // ledOn=false => io_1=0, rainbowStep=0 => both rgb red,
  // servoPhase=0 => servo1=0, servo2=180, servo3=90.
  pinMode(PIN_LED1_PWM, OUTPUT);
  pinMode(PIN_RGB1_R, OUTPUT);
  pinMode(PIN_RGB1_G, OUTPUT);
  pinMode(PIN_RGB1_B, OUTPUT);
  pinMode(PIN_RGB2_R, OUTPUT);
  pinMode(PIN_RGB2_G, OUTPUT);
  pinMode(PIN_RGB2_B, OUTPUT);

  analogWrite(PIN_LED1_PWM, 0);

  // RGB initials: model expects rgb1 red and rgb2 red (io_2=255, io_3=0, io_4=0; io_5=255, io_6=0, io_7=0)
  setRgbPWM(255, 0, 0, PIN_RGB1_R, PIN_RGB1_G, PIN_RGB1_B);
  setRgbPWM(255, 0, 0, PIN_RGB2_R, PIN_RGB2_G, PIN_RGB2_B);

  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);
  servo3.attach(PIN_SERVO3);

  // Servo initial positions as per model (servoPhase=0)
  servo1.write(0);
  servo2.write(180);
  servo3.write(90);

  lastTickMs = millis();
  // Apply remaining outputs (redundant but matches spec deterministically)
  applyOutputs();
}

void loop() {
  // Ensure control_period_ms = 10ms, by using elapsed ms delta_since_previous_step (io_0)
  // Model updates: ledAccumMs += io_0, rainbowAccumMs += io_0, servoAccumMs += io_0,
  // with thresholded actions at >=2000ms.
  uint32_t now = millis();
  uint32_t dtMsU = now - lastTickMs;
  lastTickMs = now;

  // Constrain dt to non-negative i32; dt is always unsigned diff then cast.
  int32_t io_0 = (int32_t)dtMsU;

  // Controller tick actions (frozen model)
  ledAccumMs = ledAccumMs + io_0;
  rainbowAccumMs = rainbowAccumMs + io_0;
  servoAccumMs = servoAccumMs + io_0;

  if (ledAccumMs >= 2000) {
    ledAccumMs = ledAccumMs - 2000;
    ledOn = !ledOn;
  }

  if (rainbowAccumMs >= 2000) {
    rainbowAccumMs = rainbowAccumMs - 2000;
    rainbowStep = (rainbowStep >= 7 ? 0 : rainbowStep + 1);
  }

  if (servoAccumMs >= 2000) {
    servoAccumMs = servoAccumMs - 2000;
    servoPhase = (servoPhase >= 1 ? 0 : servoPhase + 1);
  }

  applyOutputs();

  // Maintain sample_period_ms = 10
  delay(10);
}
