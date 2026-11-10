#include <Arduino.h>

static constexpr uint8_t PIN_BTN1 = 2; // io_0
static constexpr uint8_t PIN_BTN2 = 3; // io_1

static constexpr uint8_t PIN_RGB_R = 4; // io_2
static constexpr uint8_t PIN_RGB_G = 5; // io_3
static constexpr uint8_t PIN_RGB_B = 6; // io_4

static Controller-like: // (marker to avoid name collisions; no extra behavior)

static inline void setRgbPWM(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(PIN_RGB_R, r);
  analogWrite(PIN_RGB_G, g);
  analogWrite(PIN_RGB_B, b);
}

struct Controller {
  // States: exact initial values per model
  int32_t colorIndex = 0;
  bool prevBtn1 = false;
  bool prevBtn2 = false;

  // Inputs (updated each tick)
  bool io_0 = false; // btn1 pressed (active-low => true when LOW)
  bool io_1 = false; // btn2 pressed (active-low => true when LOW)

  void tick() {
    // Perform action tick (in the same order as model updates)
    // if (io_0 && !prevBtn1) colorIndex := ...
    if (io_0 && (!prevBtn1)) {
      if (colorIndex == 8) colorIndex = 0;
      else colorIndex = colorIndex + 1;
    }

    // if (io_1 && !prevBtn2) colorIndex := ...
    if (io_1 && (!prevBtn2)) {
      if (colorIndex == 0) colorIndex = 8;
      else colorIndex = colorIndex - 1;
    }

    // prevBtn1 := io_0 ; prevBtn2 := io_1
    prevBtn1 = io_0;
    prevBtn2 = io_1;
  }

  // Outputs derived from colorIndex (exact boolean conditions)
  void writeOutputs() const {
    // io_2 (red): (colorIndex==1 || 4 || 5 || 7) ? 255 : 0
    uint8_t r = ((colorIndex == 1) || (colorIndex == 4) || (colorIndex == 5) || (colorIndex == 7)) ? 255 : 0;
    // io_3 (green): (colorIndex==2 || 4 || 6 || 7) ? 255 : 0
    uint8_t g = ((colorIndex == 2) || (colorIndex == 4) || (colorIndex == 6) || (colorIndex == 7)) ? 255 : 0;
    // io_4 (blue): (colorIndex==3 || 5 || 6 || 7) ? 255 : 0
    uint8_t b = ((colorIndex == 3) || (colorIndex == 5) || (colorIndex == 6) || (colorIndex == 7)) ? 255 : 0;

    setRgbPWM(r, g, b);
  }
};

static Controller controller;

// Control period is specified as 5ms; loop will tick at that period.
static constexpr uint32_t CONTROL_PERIOD_MS = 5;

void setup() {
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);

  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);

  // Initial outputs per model: colorIndex=0 => Off => (io_2,io_3,io_4) = (0,0,0)
  setRgbPWM(0, 0, 0);
}

void loop() {
  static uint32_t nextTickMs = millis();

  // Ensure timing respects 5ms control period (best-effort).
  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) < 0) return;
  nextTickMs += CONTROL_PERIOD_MS;

  // Read inputs (active-low => pressed => true when LOW)
  controller.io_0 = (digitalRead(PIN_BTN1) == LOW);
  controller.io_1 = (digitalRead(PIN_BTN2) == LOW);

  controller.tick();
  controller.writeOutputs();
}