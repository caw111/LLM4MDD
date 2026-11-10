#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint32_t CONTROL_PERIOD_MS = 10;

// Hardware bindings (frozen)
static constexpr uint8_t PIN_LED = 13;   // D13
static constexpr uint8_t PIN_BEEP = 6;  // D6 (tone_output)
static constexpr uint8_t PIN_TRIG = 8;  // D8
static constexpr uint8_t PIN_ECHO = 9;  // D9
static constexpr uint8_t PIN_LDR = A0;  // A0

// ADC threshold for "dark"
static constexpr float LDR_DARK_THRESHOLD = 300.0f; // <= 300.0

// Ultrasonic trigger distance threshold
static constexpr float DIST_ALERT_CM = 20.0f; // <= 20.0

// HC-SR04 sampling contract
// timeout_us must be 30000us (range 25000..60000)
static constexpr uint32_t ULTRASONIC_TIMEOUT_US = 30000UL;

// IO sample periods from interface contract
static constexpr uint32_t IO0_SAMPLE_PERIOD_MS = 10; // A0
static constexpr uint32_t IO1_IO2_SAMPLE_PERIOD_MS = 60; // ultrasonic_distance + ultrasonic_valid

// Serial contract
static constexpr uint32_t SERIAL_BAUD = 9600;
static constexpr uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;

// Model state (controller states)
static bool alarmActive = false;
static bool ledState = false;
static bool beepState = false;

// Outputs cached for atomic update behavior within tick
static bool io_3_led_out = false;
static int32_t io_4_tone_hz = 0;
static bool serial_trigger_alarm = false;
static bool serial_value_alarm = false;

// Ultrasonic sensor memory (hold_last_valid)
static float last_ultrasonic_cm = 0.0f;
static bool last_ultrasonic_valid = false;

// Timers
static uint32_t lastTickMs = 0;
static uint32_t lastIo0Ms = 0;
static uint32_t lastUltrasonicMs = 0;

// Inputs (io_0..io_2)
static float io_0_ldr_adc = 0.0f;       // adc_count (scaled 1.0)
static float io_1_ultrasonic_cm = 0.0f; // cm
static bool io_2_ultrasonic_valid = false;

// Serial idle framing: line_or_idle with idle_timeout_ms.
// We implement "line" as: when alarm triggers, emit a single line "1\n" (or "0\n" on subsequent changes),
// but the contract requires only boolean value written, and trigger/value are produced at model tick.
// We'll output only when serial_trigger_alarm is true (trigger port), and emit the boolean value.
static bool last_serial_trigger_sent = false;

// Measure HC-SR04 (returns cm, valid via out parameter)
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float readUltrasonicCm(bool &valid);
#line 91 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyModelOutputs();
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void modelTick();
#line 138 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serviceSerial();
#line 166 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 198 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float readUltrasonicCm(bool &valid) {
  // Ensure trigger low briefly
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  // Trigger pulse 10us+
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Measure pulse width with timeout
  // pulseInLong available on recent cores; fallback to pulseIn with timeouts.
  // We use pulseIn with microsecond timeout semantics.
  unsigned long durationUs = pulseIn(PIN_ECHO, HIGH, ULTRASONIC_TIMEOUT_US);

  if (durationUs == 0) {
    valid = false;
    return last_ultrasonic_cm; // hold_last_valid behavior
  }

  // Convert duration to distance.
  // Speed of sound: 34300 cm/s => 0.0343 cm/us
  // distance cm = durationUs * 0.0343 / 2
  float cm = (float)durationUs * 0.0343f * 0.5f;

  valid = true;
  return cm;
}

static void applyModelOutputs() {
  // Outputs per model semantics:
  // io_3 = ledState and alarmActive
  io_3_led_out = (ledState && alarmActive);

  // io_4 = if alarmActive and beepState then 2000 else 0
  io_4_tone_hz = (alarmActive && beepState) ? 2000 : 0;

  // serial ports
  serial_trigger_alarm = alarmActive;
  serial_value_alarm = alarmActive;

  // Apply to hardware
  digitalWrite(PIN_LED, io_3_led_out ? HIGH : LOW);

  if (io_4_tone_hz == 0) {
    noTone(PIN_BEEP);
  } else {
    tone(PIN_BEEP, (unsigned int)io_4_tone_hz);
  }
}

static void modelTick() {
  // Controller tick:
  // alarmActive := (io_0 <= 300.0) AND io_2 AND (io_1 <= 20.0)
  bool next_alarmActive = (io_0_ldr_adc <= LDR_DARK_THRESHOLD) && io_2_ultrasonic_valid && (io_1_ultrasonic_cm <= DIST_ALERT_CM);

  // If alarmActive then toggle ledState and beepState
  // Note: update semantics in the provided model:
  // "then if alarmActive { first start; then assign ledState := not ledState; then assign beepState := not beepState; }"
  // This implies toggling based on the newly assigned alarmActive in the tick.
  // We'll compute next_alarmActive first, then toggle if true.
  bool next_ledState = ledState;
  bool next_beepState = beepState;

  if (next_alarmActive) {
    next_ledState = !ledState;
    next_beepState = !beepState;
  }

  alarmActive = next_alarmActive;
  ledState = next_ledState;
  beepState = next_beepState;

  applyModelOutputs();
}

static void serviceSerial() {
  // Contract: output "alarm" boolean (serial_0_value_0) when serial_0_trigger is true.
  // Also includes idle framing with idle_timeout_ms; we keep it simple:
  // - Only emit when trigger is true.
  // - Emit a single line "alarm=1" or "alarm=0" would "invent" protocol text.
  // So we emit only "1\n" when alarm is true at the instant. No output when trigger is false.
  // This matches "fixed meaning boolean value" while avoiding extra invented text/protocol.
  // Also avoid flooding if alarm remains true across many ticks:
  // We'll emit when trigger transitions from false->true, and on each tick where trigger is true but value differs.
  bool current_trigger = serial_trigger_alarm;
  bool current_value = serial_value_alarm;

  if (current_trigger) {
    // Emit whenever we haven't emitted for this continuous alarm phase start,
    // or if the value differs (shouldn't, since value == trigger).
    if (!last_serial_trigger_sent || (current_value != serial_value_alarm)) {
      Serial.println(current_value ? 1 : 0);
      last_serial_trigger_sent = true;
    }
  } else {
    // When alarm deasserts, allow next trigger emission.
    if (last_serial_trigger_sent) {
      // Optional: no need to print 0 because trigger is false; contract doesn't demand it.
      last_serial_trigger_sent = false;
    }
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BEEP, OUTPUT);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Initial levels
  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_TRIG, LOW);
  noTone(PIN_BEEP);

  // Initialize timers and sample clocks
  uint32_t now = millis();
  lastTickMs = now;
  lastIo0Ms = now;
  lastUltrasonicMs = now;

  // Initialize inputs according to initial values in contract:
  io_0_ldr_adc = 0.0f;
  io_1_ultrasonic_cm = 0.0f;
  io_2_ultrasonic_valid = false;

  last_ultrasonic_cm = 0.0f;
  last_ultrasonic_valid = false;

  // Initial model output
  applyModelOutputs();
}

void loop() {
  uint32_t now = millis();

  // Run periodic control tick at 10ms.
  // Ensure stable scheduling: if behind, execute at most once per loop to avoid multiple ticks without IO updates.
  if ((uint32_t)(now - lastTickMs) >= CONTROL_PERIOD_MS) {
    // Update IO samples according to their individual sample periods before modelTick.
    if ((uint32_t)(now - lastIo0Ms) >= IO0_SAMPLE_PERIOD_MS) {
      // io_0: analog_input scale 1.0 offset 0.0 pin A0
      int adc = analogRead(PIN_LDR);
      io_0_ldr_adc = (float)adc;
      lastIo0Ms = now;
    }

    if ((uint32_t)(now - lastUltrasonicMs) >= IO1_IO2_SAMPLE_PERIOD_MS) {
      bool valid = false;
      float cm = readUltrasonicCm(valid);

      // invalid_policy: hold_last_valid
      if (valid) {
        last_ultrasonic_cm = cm;
        last_ultrasonic_valid = true;
      } else {
        // keep last values; io_2 becomes false per contract ("无效时保持上次距离并将此为false")
        // distance holds last valid distance
      }

      io_1_ultrasonic_cm = last_ultrasonic_cm;
      io_2_ultrasonic_valid = valid ? true : false;

      lastUltrasonicMs = now;
    }

    // Tick model exactly once per period
    modelTick();

    // Serial service (trigger/value ports)
    // Serial idle timeout is not strictly required for correctness with our strategy.
    serviceSerial();

    // advance tick
    lastTickMs += CONTROL_PERIOD_MS;
  }
}
