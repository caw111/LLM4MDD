#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_012\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Frozen interface contract: 10ms control period
static const uint32_t CONTROL_PERIOD_MS = 10;

// Pins per resolved hardware contract
static const uint8_t PIN_LED_D13 = 13;
static const uint8_t PIN_BTN_D2   = 2;   // active-high, no pullup override specified
static const uint8_t PIN_POT_A0   = A0;  // analog input

static const uint8_t PIN_TM1637_CLK = 4; // D4
static const uint8_t PIN_TM1637_DIO = 5; // D5

// Controller state (exactly as in Accepted executable model)
static bool running = false;           // state: running
static bool previousPressed = false;  // state: previousPressed
static int32_t periodMs = 500;        // state: periodMs (i32)
static int32_t blinkPhase = 0;        // state: blinkPhase (i32)

// TM1637 setup
static TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// Helper: Arduino analogRead gives 0..1023
// Model input io_0 is Real (adc_count). We map directly as float(adc_count).
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_012\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float readPotIo0();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_012\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool computeIo2();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_012\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t computeIo3();
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_012\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_012\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_012\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline float readPotIo0() {
  return (float)analogRead(PIN_POT_A0);
}

// Output expressions
static inline bool computeIo2() {
  return running && (blinkPhase < (periodMs / 2));
}

static inline int32_t computeIo3() {
  return periodMs;
}

void setup() {
  pinMode(PIN_LED_D13, OUTPUT);
  // Contract says pullup=false for digital_input on D2; default is no pullup.
  pinMode(PIN_BTN_D2, INPUT);

  tm1637.setBrightness(7); // brightness: 7 per contract
  // invalid_policy: blank, leading_zero: false
  // We'll just display the integer; with leading_zero=false default TM1637Display behavior
  // typically doesn't force leading zeros. No special handling needed.
  digitalWrite(PIN_LED_D13, LOW);

  // Initialize display with initial periodMs (500)
  tm1637.showNumberI(computeIo3(), false);
}

void loop() {
  static uint32_t lastTickMs = 0;
  uint32_t now = millis();
  if ((uint32_t)(now - lastTickMs) < CONTROL_PERIOD_MS) {
    return;
  }
  lastTickMs += CONTROL_PERIOD_MS;

  // === Sample inputs (io_0, io_1) with period 10ms ===
  const float io_0 = readPotIo0();               // f64, unit adc_count
  const bool io_1 = (digitalRead(PIN_BTN_D2) == HIGH); // bool, active_high true_level=HIGH

  // === Execute controller tick: preserving update ordering and semantics ===
  // Tick action:
  // first start;
  // then if io_1 and not previousPressed { running = not running; blinkPhase = 0 }
  if (io_1 && !previousPressed) {
    running = !running;
    blinkPhase = 0;
  }

  // then assign previousPressed = io_1
  previousPressed = io_1;

  // then assign periodMs = clamp(floor(io_0), 1..2000)
  // Model: if floor(io_0) <= 0 then 1 else (if floor(io_0) >= 2000 then 2000 else floor(io_0))
  const int32_t floored = (int32_t)floorf(io_0);
  if (floored <= 0) {
    periodMs = 1;
  } else if (floored >= 2000) {
    periodMs = 2000;
  } else {
    periodMs = floored;
  }

  // then if running { blinkPhase = blinkPhase + 10 }
  if (running) {
    blinkPhase = blinkPhase + 10;
  }

  // then if running and blinkPhase >= periodMs { blinkPhase = blinkPhase - periodMs }
  if (running && blinkPhase >= periodMs) {
    blinkPhase = blinkPhase - periodMs;
  }

  // === Compute outputs in order of "outputs" mapping ===
  const bool io_2 = computeIo2();
  const int32_t io_3 = computeIo3();

  // io_2: when stopped, LED must be off
  digitalWrite(PIN_LED_D13, io_2 ? HIGH : LOW);

  // io_3: display current blink period value
  // leading_zero: false; invalid_policy: blank
  tm1637.showNumberI(io_3, false);
}
