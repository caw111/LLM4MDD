#include <Arduino.h>
#include <TM1637Display.h>

static const uint32_t CONTROL_PERIOD_MS = 5;

// Frozen interface contract bindings
// io_0: Button on D2 (active_high true/false levels per contract semantics)
static const uint8_t PIN_IO_0 = 2;

// io_1: LED on D13 (parity indicator)
static const uint8_t PIN_IO_1 = 13;

// io_2: TM1637 number display
static const uint8_t PIN_TCLK = 4; // clk_pin D4
static const uint8_t PIN_DIO  = 5; // dio_pin D5

// Controller state (exact as accepted model)
static int32_t count = 0;              // i32 initial 0
static bool previousPressed = false;  // bool initial false

// TM1637 display
static TM1637Display tm1637(PIN_TCLK, PIN_DIO);

// Keep scheduling to 5ms period
static uint32_t nextTickMs = 0;

static void controllerTick(bool io_0) {
  // Update logic (ordered updates and edge detection)
  // if (io_0 and (not previousPressed)) { count := count + 1; previousPressed := io_0; }
  // else { previousPressed := io_0; }
  if (io_0 && !previousPressed) {
    count = count + 1;
    previousPressed = io_0;
  } else {
    previousPressed = io_0;
  }

  // Outputs (exact expression):
  // io_1 = count != 2 * floor(count/2)
  // For integers: count != 2*(count/2) equivalent to count%2 != 0
  bool io_1 = (count != (2 * (count / 2)));

  // io_2 = count
  // Apply outputs
  digitalWrite(PIN_IO_1, io_1 ? HIGH : LOW);

  // TM1637 displayed count (no leading zeros; invalid_policy blank)
  // Use showNumberDec with width=0 to avoid forcing leading zeros.
  // Note: TM1637Display library requires an explicit length; width=0 is not valid.
  // Use a sufficient width without leading zeros by not padding.
  // However, to preserve behavior as closely as possible, use showNumberDec(count, false, len).
  // We choose len=1.. up to 6 by changing len dynamically.
  int32_t absCount = count < 0 ? -count : count;
  uint8_t len = 1;
  while (absCount >= 10) { len++; absCount /= 10; if (len > 6) break; }
  tm1637.showNumberDec(count, false, len);
}

void setup() {
  pinMode(PIN_IO_0, INPUT); // contract default: unspecified; keep Arduino default (no pullup forced)
  pinMode(PIN_IO_1, OUTPUT);

  tm1637.setBrightness(7); // frozen hardware contract brightness: 7
  tm1637.clear();

  // Initialize outputs based on initial controller state (count=0 => io_1 false, io_2=0)
  digitalWrite(PIN_IO_1, LOW);
  tm1637.showNumberDec(0, false, 1);

  nextTickMs = millis();
}

void loop() {
  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) >= 0) {
    // Fixed-period tick; if lagged, catch up by resyncing to nextTickMs + k*period
    // but still execute one tick per control period boundary relative to start.
    // For simplicity and determinism: advance nextTickMs by exactly one period each loop tick.
    nextTickMs += CONTROL_PERIOD_MS;

    // Sample inputs
    // io_0: model semantics true=HIGH false=LOW (active_high per contract)
    bool io_0 = (digitalRead(PIN_IO_0) == HIGH);

    // Run controller tick
    controllerTick(io_0);
  }
}