// Arduino Mega 2560 sketch generated from Accepted executable model + frozen IO contract.
// Pin bindings (per provided circuit + contract):
// pot1: SIG -> A3, VCC -> 5V, GND -> GND
// rgb1 (common cathode): COM -> GND, R->D5, G->D6, B->D7
// led1 -> D2, led2 -> D3, led3 -> D4 (active high)

// Control period: 10 ms (sample_period_ms for all ports)

#include <Arduino.h>

static constexpr uint8_t PIN_POT_SIG = A3;
static constexpr uint8_t PIN_LED1 = 2;
static constexpr uint8_t PIN_LED2 = 3;
static constexpr uint8_t PIN_LED3 = 4;

static constexpr uint8_t PIN_RGB_R = 5; // PWM
static constexpr uint8_t PIN_RGB_G = 6; // PWM
static constexpr uint8_t PIN_RGB_B = 7; // PWM

static constexpr uint32_t CONTROL_PERIOD_MS = 10;

static inline void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(PIN_RGB_R, r);
  analogWrite(PIN_RGB_G, g);
  analogWrite(PIN_RGB_B, b);
}

static inline void setIndicators(bool led1, bool led2, bool led3) {
  digitalWrite(PIN_LED1, led1 ? HIGH : LOW);
  digitalWrite(PIN_LED2, led2 ? HIGH : LOW);
  digitalWrite(PIN_LED3, led3 ? HIGH : LOW);
}

void setup() {
  // Initialize pins
  pinMode(PIN_POT_SIG, INPUT);

  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_LED3, OUTPUT);

  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);

  // Initialization contract: all LEDs off at startup/reset
  // io_4..io_6 false; io_1..io_3 initial=0.
  setIndicators(false, false, false);
  setRGB(0, 0, 0);
}

void loop() {
  static uint32_t nextTick = millis();

  // Run continuously with a fixed 10 ms period.
  // (Exact scheduling isn't visible to the model, but preserves continuous 10ms updates.)
  uint32_t now = millis();
  if ((int32_t)(now - nextTick) < 0) return;
  nextTick += CONTROL_PERIOD_MS;

  // io_0: pot1 ADC count (f64 model). Arduino analogRead returns 0..1023.
  int adc = analogRead(PIN_POT_SIG);
  float io_0 = (float)adc;

  // Accepted model outputs (exact piecewise thresholds):
  // io_1 (R) = if io_0>=0 then (if io_0<=341 then 255 else 0) else 0
  // io_2 (G) = if io_0>=342 and io_0<=682 then 255 else 0
  // io_3 (B) = if io_0>=683 then 255 else 0
  // io_4 (led1) = io_0<=341
  // io_5 (led2) = io_0>=342 and io_0<=682
  // io_6 (led3) = io_0>=683

  float io_1 =
      (io_0 >= 0.0f) ? ((io_0 <= 341.0f) ? 255.0f : 0.0f) : 0.0f;
  float io_2 =
      ((io_0 >= 342.0f) && (io_0 <= 682.0f)) ? 255.0f : 0.0f;
  float io_3 =
      (io_0 >= 683.0f) ? 255.0f : 0.0f;

  bool io_4 = (io_0 <= 341.0f);
  bool io_5 = ((io_0 >= 342.0f) && (io_0 <= 682.0f));
  bool io_6 = (io_0 >= 683.0f);

  // Apply to hardware: PWM channels D5/D6/D7 with 0..255.
  setRGB((uint8_t)io_1, (uint8_t)io_2, (uint8_t)io_3);

  // Apply to hardware: digital indicators D2/D3/D4 active-high.
  setIndicators(io_4, io_5, io_6);
}