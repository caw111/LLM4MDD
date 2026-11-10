#include <Arduino.h>
#include <TM1637Display.h>

// ==== Frozen hardware contract bindings ====
// io_0 (input bool): D2, active_high, pullup=true, model true=HIGH
static const uint8_t PIN_BTN = 2;

// io_1 (output i32): tm1637_number on D4=CLK, D5=DIO, brightness=7, invalid_policy=blank, leading_zero=false
static const uint8_t PIN_TM1637_CLK = 4; // CLK
static const uint8_t PIN_TM1637_DIO = 5; // DIO

// ==== Model: ACCEPTED executable model ====
struct ControllerModel {
  int32_t countSeconds = 0;        // initial 0
  int32_t accumulatedMs = 0;       // initial 0
  bool previousPressed = false;    // initial false

  // Perform one tick, using sampled io_0 and elapsedMilliseconds(delta since previous step)
  void tick(bool io_0, int32_t elapsedMilliseconds) {
    // if ((io_0) and ((not previousPressed))) then reset
    if (io_0 && (!previousPressed)) {
      countSeconds = 0;
      accumulatedMs = 0;
    } else {
      // accumulate ms
      accumulatedMs = accumulatedMs + elapsedMilliseconds;
      // if (accumulatedMs >= 1000) then subtract 1000 and increment countSeconds
      if (accumulatedMs >= 1000) {
        accumulatedMs = accumulatedMs - 1000;
        countSeconds = countSeconds + 1;
      }
    }

    // previousPressed := io_0
    previousPressed = io_0;
  }
};

static ControllerModel model;

// ==== TM1637 setup ====
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

static void tm1637ShowCountSeconds(int32_t v) {
  // leading_zero=false, invalid_policy=blank
  // We assume countSeconds is non-negative per model usage.
  if (v < 0) return;
  // TM1637Display::showNumberDec prints without leading zeros by default.
  display.showNumberDec(v, false, 4, 0); // digits=4, leadingZeros=false, pos=0
}

// ==== Control period: 5ms ====
static const uint32_t CONTROL_PERIOD_MS = 5;

static uint32_t lastStepMs = 0;

void setup() {
  // Hardware init
  pinMode(PIN_BTN, INPUT_PULLUP); // pullup=true

  // TM1637 init: brightness=7
  display.setBrightness(7);

  // Initialize time base
  lastStepMs = millis();

  // Initial output corresponds to countSeconds=0
  tm1637ShowCountSeconds(model.countSeconds);
}

void loop() {
  const uint32_t now = millis();
  if (now - lastStepMs < CONTROL_PERIOD_MS) return;

  // Ensure delta is computed in a stable way for the model:
  // "Actual elapsed time since previous model step; first delta is zero"
  // With periodic loop, delta may deviate slightly; we preserve the delta semantics.
  int32_t elapsedMs = (now - lastStepMs);
  lastStepMs = now;

  // Sample io_0 (button): active_high true=HIGH pressed
  // With INPUT_PULLUP, HIGH means released; if wiring is opposite, user must adjust externally.
  bool io_0 = (digitalRead(PIN_BTN) == HIGH);

  // Model tick
  model.tick(io_0, elapsedMs);

  // Output io_1 := countSeconds
  tm1637ShowCountSeconds(model.countSeconds);
}