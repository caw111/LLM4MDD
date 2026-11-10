#include <Arduino.h>
#include <Tone.h>

// Frozen control_period_ms = 5 (not explicitly used; behavior is continuous each loop)
static constexpr uint8_t BUTTON_PIN = 3;  // io_0
static constexpr uint8_t BUZZER_PIN  = 9;  // io_1 tone output

// io_0 is true when model sees "button pressed".
// Hardware reads LOW when pressed, HIGH when not pressed.
// With pullup enabled: pressed -> LOW, not pressed -> HIGH.
static constexpr uint8_t BUTTON_PRESSED_LEVEL = LOW;   // model true
static constexpr uint8_t BUTTON_RELEASED_LEVEL = HIGH; // model false

// Model constant: if io_0 then 262 Hz else 0 Hz
static constexpr int32_t PRESSED_TONE_HZ = 262;

static bool previousPressed = false; // controller state (initial false)

// Minimal "tone_output" device: play frequency when valid, silence for invalid_policy=silence (i.e., 0 Hz)
static void toneOutput(int32_t freqHz) {
  if (freqHz <= 0) {
    noTone(BUZZER_PIN);
  } else {
    // Continuous tone while button is pressed (gate-to-frequency mapping handled by model)
    tone(BUZZER_PIN, (unsigned int)freqHz);
  }
}

void setup() {
  // Register io_0 as digital_input with pullup=true (10k resistor to 5V in circuit)
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Register io_1 as tone_output on D9 (OUTPUT handled by tone() internally)
  pinMode(BUZZER_PIN, OUTPUT);

  // Ensure silent at power-up (io_1 initial=0)
  previousPressed = false;
  toneOutput(0);
}

void loop() {
  // === Sample io_0 (digital_input) ===
  int raw = digitalRead(BUTTON_PIN);
  bool io_0 = (raw == (int)BUTTON_PRESSED_LEVEL); // true when pressed

  // === Controller tick ===
  // Accepted executable model:
  // tick:
  //   if true then
  //     previousPressed := io_0
  //   else
  //     previousPressed := previousPressed
  // Notes: "if true" always true => previousPressed always becomes io_0 each tick.
  previousPressed = io_0;

  // outputs:
  // io_1 := (if io_0 ? 262 else 0)
  int32_t io_1 = io_0 ? PRESSED_TONE_HZ : 0;

  // Apply output to tone_output (invalid_policy=silence)
  toneOutput(io_1);

  // Control period is 5ms granularity; keep behavior consistent with scheduling note.
  delay(5);
}