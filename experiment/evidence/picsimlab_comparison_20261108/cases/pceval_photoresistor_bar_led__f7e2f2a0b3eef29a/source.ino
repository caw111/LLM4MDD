#include <Arduino.h>

// ---- Frozen interface contract bindings ----
// io_0: analog input (f64 adc_count) from A0
static constexpr uint8_t PIN_PHOTORESISTOR_AO = A0;

// io_1..io_10: bar graph LED anode segments 1..10 on D2..D11, active-high
static constexpr uint8_t LED_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

static constexpr unsigned long CONTROL_PERIOD_MS = 10;

// ---- Controller state (from ACCEPTED model) ----
static int32_t mappedValue = 0;

// ---- Internal scheduling for 10ms control period ----
static unsigned long nextTickMs = 0;

// Discrete tick: read io_0, update mappedValue per model, then update outputs
static void tick() {
  // io_0 read: model semantics treat io_0 as Real in [0..1023]
  // Arduino analogRead returns int 0..1023
  const float io_0 = (float)analogRead(PIN_PHOTORESISTOR_AO);

  // Controller update:
  // mappedValue := (if io_0 <= 0.0 ? 10
  //                  else (if io_0 >= 1023.0 ? 0
  //                        else floor((1023.0 - io_0) * 10.0 / 1023.0)));
  if (io_0 <= 0.0f) {
    mappedValue = 10;
  } else if (io_0 >= 1023.0f) {
    mappedValue = 0;
  } else {
    const float expr = (1023.0f - io_0) * 10.0f / 1023.0f;
    mappedValue = (int32_t)floorf(expr);
  }

  // Outputs:
  // io_k (k=1..10) is true iff mappedValue >= k
  // Then drive pins with HIGH for true, LOW for false (active_high=true).
  for (int i = 0; i < 10; ++i) {
    const int32_t k = i + 1; // segment number
    const bool on = (mappedValue >= k);
    digitalWrite(LED_PINS[i], on ? HIGH : LOW);
  }
}

void setup() {
  // At power-up:
  // 1. Set bar graph anode pins 2..11 as OUTPUT
  for (uint8_t i = 0; i < 10; ++i) {
    pinMode(LED_PINS[i], OUTPUT);
    // Initial digital output values set to true as per contract initial=true.
    digitalWrite(LED_PINS[i], HIGH);
  }

  // 2. Set photoresistor signal pin A0 as INPUT
  pinMode(PIN_PHOTORESISTOR_AO, INPUT);

  // Initialize scheduling
  nextTickMs = millis(); // start immediately; first tick may occur as soon as loop runs
}

void loop() {
  const unsigned long now = millis();

  // Execute tick every 10ms (control_period_ms = 10)
  // Preserve deterministic update ordering from model by performing:
  // tick() => update mappedValue then update all outputs (io_1..io_10)
  if ((long)(now - nextTickMs) >= 0) {
    // If we fell behind, catch up with minimal jitter while preserving tick rate.
    // (Still executes tick with the model exactly per period boundaries as best effort.)
    do {
      tick();
      nextTickMs += CONTROL_PERIOD_MS;
    } while ((long)(now - nextTickMs) >= 0);
  }
}