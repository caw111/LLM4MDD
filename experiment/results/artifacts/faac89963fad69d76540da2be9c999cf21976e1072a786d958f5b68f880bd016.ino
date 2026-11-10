// Generated Arduino sketch from ACCEPTED executable model + frozen hardware contract.
// Control period: 10 ms (implemented with fixed periodic loop timing).

const uint8_t PIN_D2 = 2;   // Button ON
const uint8_t PIN_D3 = 3;   // Button OFF
const uint8_t PIN_D13 = 13; // LED output

static inline void applyModel(bool io_0, bool io_1) {
  // Model output: io_2 = (io_1 ? false : (io_0 ? true : false))
  bool io_2 = (!io_1) && io_0;
  digitalWrite(PIN_D13, io_2 ? HIGH : LOW); // active_high true_level
}

void setup() {
  pinMode(PIN_D2, INPUT);     // active_high, pullup=false (contract default)
  pinMode(PIN_D3, INPUT);     // active_high, pullup=false (contract default)
  pinMode(PIN_D13, OUTPUT);   // active_high

  // Initialize from model with initial sampled values in first loop tick.
  // Contract says io_2 initial=false; we enforce it immediately.
  digitalWrite(PIN_D13, LOW);

  // Ensure deterministic timing reference for 10 ms control period.
  // (No interrupts used.)
}

void loop() {
  static unsigned long nextTick = 0;
  const unsigned long periodMs = 10;

  unsigned long now = millis();
  if (nextTick == 0) nextTick = now;

  // Wait until nextTick to preserve 10ms control period behavior.
  if (now < nextTick) return;
  nextTick += periodMs;

  // Sample inputs (active_high semantics: pressed => HIGH => model=true).
  bool io_0 = (digitalRead(PIN_D2) == HIGH);
  bool io_1 = (digitalRead(PIN_D3) == HIGH);

  // Apply model output to LED.
  applyModel(io_0, io_1);
}