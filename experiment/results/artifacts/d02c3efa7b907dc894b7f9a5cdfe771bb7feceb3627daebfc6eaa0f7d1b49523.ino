#include <Arduino.h>
#include <TM1637Display.h>

// ==== Frozen interface contract bindings ====
// io_0: analog input A0 (LDR ADC count), sample period 10ms
static constexpr uint8_t PIN_LDR = A0;

// io_1: PWM output D9 (brightness command 0..255)
static constexpr uint8_t PIN_PWM_LED = 9;

// io_2: button D2, active_high=false => pressed when pin reads LOW (assume pullup)
static constexpr uint8_t PIN_BUTTON = 2;

// io_3: digital output D13 (status indicator)
static constexpr uint8_t PIN_STATUS = 13;

// io_4: TM1637 display CLK=D4, DIO=D5
static constexpr uint8_t PIN_TM1637_CLK = 4;
static constexpr uint8_t PIN_TM1637_DIO = 5;

// ==== Controller ACCEPTED executable model (generated) ====
struct GeneratedModelController {
  // States
  bool previousPressed = false;
  bool manualMode = false;
  int32_t frozenDisplayValue = 0;
  double frozenBrightnessValue = 0.0;

  // Inputs (mapped each tick)
  double io_0 = 0.0; // adc_count
  bool io_2 = false; // button pressed semantics: true when pressed

  // Outputs
  double io_1 = 0.0; // pwm_count
  bool io_3 = false;  // status
  int32_t io_4 = 0;   // display value

  static inline double floorReal(double x) { return floor(x); }

  void tick() {
    // Update block exactly as model:
    // updates:
    // if (io_2 && !previousPressed) then toggle+freeze and set previousPressed=true else previousPressed=io_2
    if (io_2 && (!previousPressed)) {
      manualMode = (!manualMode);

      // frozenDisplayValue := (if manualMode ? floor(io_0) : floor(io_0))
      // (both branches identical per model; keep exact semantics)
      if (manualMode) {
        frozenDisplayValue = (int32_t)floorReal(io_0);
      } else {
        frozenDisplayValue = (int32_t)floorReal(io_0);
      }

      // frozenBrightnessValue := mapping from io_0 to 0..255 via piecewise, using same formula in both branches.
      // if manualMode ? <map> : <map>
      auto mapBrightness = [&]() -> double {
        if (io_0 >= 1023.0) return 255.0;
        else {
          if (io_0 <= 0.0) return 0.0;
          else return io_0 * 255.0 / 1023.0;
        }
      };

      if (manualMode) frozenBrightnessValue = mapBrightness();
      else frozenBrightnessValue = mapBrightness();

      previousPressed = true;
    } else {
      previousPressed = io_2;
    }

    // Outputs
    auto mapBrightness = [&]() -> double {
      if (io_0 >= 1023.0) return 255.0;
      else {
        if (io_0 <= 0.0) return 0.0;
        else return io_0 * 255.0 / 1023.0;
      }
    };

    io_1 = manualMode ? frozenBrightnessValue : mapBrightness();
    io_3 = manualMode;
    io_4 = manualMode ? frozenDisplayValue : (int32_t)floorReal(io_0);
  }
};

GeneratedModelController controller;

// ==== Timing ====
static constexpr uint32_t CONTROL_PERIOD_MS = 10;
static uint32_t lastTickMs = 0;

// ==== TM1637 configuration ====
TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// Display formatting: model says integer, leading_zero=false, invalid_policy=blank.
// For i32 range produced here (0..1023), just show integer value without leading zeros.
static void renderDisplay(int32_t value) {
  // Keep behavior straightforward: show value as decimal, no leading zeros.
  // tm1637 library will handle truncation for supported range.
  tm1637.showNumberDec(value, false, 4, 0); // digits=4, leading zeros=false, no leading blanking needed
}

void setup() {
  pinMode(PIN_PWM_LED, OUTPUT);
  pinMode(PIN_STATUS, OUTPUT);

  pinMode(PIN_BUTTON, INPUT_PULLUP); // active_high=false => pressed when LOW
  // Initialize TM1637
  tm1637.setBrightness(0x0f); // library brightness; business mapping is via io_1 to PWM LED, not TM brightness

  lastTickMs = millis();
}

void loop() {
  uint32_t now = millis();
  if ((uint32_t)(now - lastTickMs) < CONTROL_PERIOD_MS) return;
  lastTickMs += CONTROL_PERIOD_MS;

  // === Input mapping ===
  controller.io_0 = (double)analogRead(PIN_LDR); // adc_count

  // io_2 pressed semantics: active_high=false => pressed when pin reads LOW
  bool pinState = (digitalRead(PIN_BUTTON) == HIGH);
  controller.io_2 = !pinState;

  // === Control tick ===
  controller.tick();

  // === Output mapping ===
  // io_1: pwm_count, scale 255 offset 0 => use direct 0..255 domain.
  // Clamp to [0,255] for safety against numeric artifacts while preserving mapping.
  double pwmCmd = controller.io_1;
  if (pwmCmd < 0.0) pwmCmd = 0.0;
  if (pwmCmd > 255.0) pwmCmd = 255.0;
  analogWrite(PIN_PWM_LED, (int)lrint(pwmCmd));

  // io_3: manualMode status boolean true_level HIGH
  digitalWrite(PIN_STATUS, controller.io_3 ? HIGH : LOW);

  // io_4: TM1637 numeric display
  renderDisplay(controller.io_4);
}