#include <Arduino.h>

static const uint8_t PHOTORESISTOR_PIN = A2;

// Frozen interface contract / serial configuration
static const unsigned long SERIAL_BAUD = 9600;
static const uint32_t CONTROL_PERIOD_MS = 10;

// Model state (exactly as provided)
static bool reportParity = false;     // initial: false
static bool pendingReport = true;     // initial: true

// Timing for control step (10 ms) + 500 ms gating via model-level scheduling assumption
// Per requirements: delay 500ms in every loop.
// We'll run the control tick every 10ms, but only emit serial after 500ms since last report is due.
// The accepted model produces a trigger based on state; we keep those semantics,
// and add a 500ms wait between successive report-trigger emissions to match the requirement.
static unsigned long lastReportTriggerMs = 0;

// Outputs (computed)
static bool io_1 = false;
static bool serial_0_trigger = false;
static double io_0 = 0.0;     // raw ADC count

static inline int readPhotoresistorRaw() {
  // Raw ADC counts
  return analogRead(PHOTORESISTOR_PIN);
}

static void controllerTick() {
  // Implement the Accepted executable model updates and outputs precisely.

  // updates:
  // 1) if (not pendingReport) then [] else [pendingReport := (reportParity ? false : true);
  //                                       reportParity := (not reportParity)]
  if (!pendingReport) {
    // no-op
  } else {
    // pendingReport := (if reportParity then false else true)
    pendingReport = (reportParity ? false : true);
    // reportParity := (not reportParity)
    reportParity = !reportParity;
  }

  // 2) if (and(pendingReport, not reportParity)) then [] else []
  // (i.e., no additional state changes)
  // no-op
}

static void controllerComputeOutputs() {
  // outputs:
  // io_1 = pendingReport and reportParity
  // serial_0_trigger = pendingReport and reportParity
  io_1 = (pendingReport && reportParity);
  serial_0_trigger = (pendingReport && reportParity);
}

static void serialPublishReportIfTriggered() {
  // serial_text_output gate: io_1 gates "LDR Value: "
  // serial report control / value publish occurs when serial_0_trigger is true.
  // We must preserve ordering: prefix then numeric value on the same line.
  if (!(serial_0_trigger && io_1)) return;

  // Enforce "Delay 500ms in every loop" by only emitting when 500ms have elapsed
  // since the last emission, matching model-level scheduling constraint.
  unsigned long now = millis();
  if (lastReportTriggerMs != 0) {
    if (now - lastReportTriggerMs < 500UL) return;
  }

  // Publish prefix
  Serial.print("LDR Value: ");

  // Publish numeric value from current sampled io_0 (raw ADC counts)
  // io_0 is a double but originates from raw ADC; keep printing as a number.
  Serial.println((double)io_0);

  lastReportTriggerMs = now;
}

void setup() {
  pinMode(PHOTORESISTOR_PIN, INPUT);

  Serial.begin(SERIAL_BAUD);

  // Ensure state initials exactly as provided
  reportParity = false;
  pendingReport = true;

  lastReportTriggerMs = 0;
}

void loop() {
  static unsigned long lastControlMs = 0;
  unsigned long now = millis();

  // Control step every 10 ms (CONTROL_PERIOD_MS)
  // If we are behind, we "catch up" by ticking multiple times without blocking.
  while ((unsigned long)(now - lastControlMs) >= CONTROL_PERIOD_MS) {
    lastControlMs += CONTROL_PERIOD_MS;

    // Sample io_0 at this control step: read photoresistor raw ADC counts.
    io_0 = (double)readPhotoresistorRaw();

    // Tick controller and compute outputs.
    controllerTick();
    controllerComputeOutputs();
  }

  // Publish serial report when gated by model outputs (prefix gated by io_1).
  serialPublishReportIfTriggered();

  // Requirement: Delay 500ms in every loop.
  // Keep as stated, even though we already enforce 500ms between emissions.
  delay(500);
}