#include <Arduino.h>

static const uint8_t PIN_BTN1 = 2;   // btn1 -> D2 (active-low, pullup)
static const uint8_t PIN_BTN2 = 3;   // btn2 -> D3 (active-low, pullup)
static const uint8_t PIN_POT  = A3;  // pot1 SIG -> A3

static const uint8_t PIN_RGB_R = 4;  // rgb1 R -> D4 (PWM)
static const uint8_t PIN_RGB_G = 5;  // rgb1 G -> D5 (PWM)
static const uint8_t PIN_RGB_B = 6;  // rgb1 B -> D6 (PWM)

// ACCEPTED model state
static int32_t mode1Index = 0;                 // initial 0
static int32_t mode2Index = 0;                 // initial 0
static bool previousPressedBtn1 = false;       // initial false
static bool previousPressedBtn2 = false;       // initial false
static bool isSecondMode = false;             // initial false

// Required: initialize RGB LED to red at power-on/reset (R=255, G=0, B=0)
static void applyOutputsFromModel(int potRaw /*0..1023*/) {
  // io_2 mapped to brightness 0..255 using floor(io_2 * 255.0 / 1023.0)
  // Note: model uses RealFunctions::floor; implement deterministically with integer math.
  uint8_t brightness = (uint8_t)((int32_t)potRaw * 255L / 1023L);

  float io_3 = 0.0f; // red PWM
  float io_4 = 0.0f; // green PWM
  float io_5 = 0.0f; // blue PWM

  if (isSecondMode) {
    if (mode2Index == 0) {          // yellow: R=255, G=255, B=0
      io_3 = (float)brightness;
      io_4 = (float)brightness;
      io_5 = 0.0f;
    } else if (mode2Index == 1) {   // cyan: R=0, G=255, B=0 (per frozen model)
      io_3 = 0.0f;
      io_4 = (float)brightness;
      io_5 = 0.0f;
    } else if (mode2Index == 2) {   // magenta: R=0, G=0, B=255 (per frozen model)
      io_3 = 0.0f;
      io_4 = 0.0f;
      io_5 = (float)brightness;
    } else {
      io_3 = 0.0f; io_4 = 0.0f; io_5 = 0.0f;
    }
  } else {
    if (mode1Index == 0) {         // red: R=255, G=0, B=0
      io_3 = (float)brightness;
      io_4 = 0.0f;
      io_5 = 0.0f;
    } else if (mode1Index == 1) {  // green: R=0, G=255, B=0 (per frozen model)
      io_3 = 0.0f;
      io_4 = (float)brightness;
      io_5 = 0.0f;
    } else if (mode1Index == 2) {  // blue: R=0, G=0, B=0 (per frozen model)
      io_3 = 0.0f;
      io_4 = 0.0f;
      io_5 = (float)brightness;
    } else {
      io_3 = 0.0f; io_4 = 0.0f; io_5 = 0.0f;
    }
  }

  // Convert real PWM counts to uint8 for analogWrite.
  // Model outputs are always multiples of floor mapping (0..255), but keep robust.
  uint8_t rPwm = (uint8_t)constrain((int)lroundf(io_3), 0, 255);
  uint8_t gPwm = (uint8_t)constrain((int)lroundf(io_4), 0, 255);
  uint8_t bPwm = (uint8_t)constrain((int)lroundf(io_5), 0, 255);

  analogWrite(PIN_RGB_R, rPwm);
  analogWrite(PIN_RGB_G, gPwm);
  analogWrite(PIN_RGB_B, bPwm);
}

void setup() {
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);

  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);

  // PWM baseline (and per contract: initialize to red at first step)
  // Contract explicitly: R=255, G=0, B=0 before normal button/pot updates.
  analogWrite(PIN_RGB_R, 255);
  analogWrite(PIN_RGB_G, 0);
  analogWrite(PIN_RGB_B, 0);
}

void loop() {
  static uint32_t lastTickMs = 0;

  // control_period_ms = 10
  uint32_t now = millis();
  if ((uint32_t)(now - lastTickMs) < 10) return;
  lastTickMs = now;

  // Sample inputs (io_0..io_2)
  // Buttons are active-low with pullup; model uses io_0/io_1 as "pressed" with true=pressed.
  bool io_0 = (digitalRead(PIN_BTN1) == LOW); // btn1 pressed
  bool io_1 = (digitalRead(PIN_BTN2) == LOW); // btn2 pressed
  int potRaw = analogRead(PIN_POT);           // 0..1023, io_2 in adc_count

  // Perform action tick (ordered updates exactly as given)
  // Update for btn1: if io_0 and not previousPressedBtn1
  if (io_0 && !previousPressedBtn1) {
    if (isSecondMode) {
      isSecondMode = false;
    } else {
      mode1Index = (mode1Index == 2) ? 0 : (mode1Index + 1);
    }
  }

  // Update for btn2: if io_1 and not previousPressedBtn2
  if (io_1 && !previousPressedBtn2) {
    if (isSecondMode) {
      mode2Index = (mode2Index == 2) ? 0 : (mode2Index + 1);
    } else {
      isSecondMode = true;
    }
  }

  // Then assign previousPressedBtn1/2
  previousPressedBtn1 = io_0;
  previousPressedBtn2 = io_1;

  // Compute and drive outputs (continuous brightness update)
  applyOutputsFromModel(potRaw);
}