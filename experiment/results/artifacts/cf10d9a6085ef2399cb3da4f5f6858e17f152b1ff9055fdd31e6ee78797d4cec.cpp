#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_027\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <stdint.h>

// DHT11 minimal reader (single-wire, timing-based) for Arduino UNO.
// Works with 5V logic; expects data pin to be connected as per DHT11 requirements.
class DHT11 {
public:
  explicit DHT11(uint8_t pin) : _pin(pin) {}

  struct Reading {
    float temperatureC;
    float humidityRH;
    bool valid;
  };

  Reading read() {
    Reading r;
    r.temperatureC = 0.0f;
    r.humidityRH = 0.0f;
    r.valid = false;

    uint8_t data[5] = {0, 0, 0, 0, 0};

    // Start signal: pull low for at least 18ms (use ~20ms)
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delay(20);

    // Release and wait for response
    digitalWrite(_pin, HIGH);
    delayMicroseconds(30);
    pinMode(_pin, INPUT_PULLUP);

    // Helper lambdas to read timing windows
    auto waitForLevel = [&](uint8_t level, unsigned long timeout_us) -> bool {
      unsigned long start = micros();
      while (micros() - start < timeout_us) {
        if (digitalRead(_pin) == level) return true;
      }
      return false;
    };

    // Response: DHT pulls LOW then HIGH for handshake
    if (!waitForLevel(LOW, 1000)) return r;   // ~80us low
    if (!waitForLevel(HIGH, 1000)) return r;  // ~80us high
    if (!waitForLevel(LOW, 1000)) return r;   // ~50us low before bits

    // Read 40 bits
    for (int i = 0; i < 40; i++) {
      // Each bit starts with ~50us LOW
      if (!waitForLevel(LOW, 1000)) return r;

      // Then HIGH duration encodes 0 (~26-28us) vs 1 (~70us)
      // We'll measure until it goes LOW.
      unsigned long highStart = micros();
      if (!waitForLevel(HIGH, 1000)) return r; // ensure it goes high
      if (!waitForLevel(LOW, 1200)) return r;   // wait for end of pulse

      unsigned long highDur = micros() - highStart;

      // Threshold around 40us to distinguish 0/1
      uint8_t bit = (highDur > 40) ? 1 : 0;
      data[i / 8] <<= 1;
      data[i / 8] |= bit;
    }

    // Some DHT libraries shift differently; our packing above shifts MSB-first each time.
    // Validate checksum.
    uint8_t sum = (uint8_t)(data[0] + data[1] + data[2] + data[3]);
    if (sum != data[4]) return r;

    // Parse for DHT11: data[0]=RH int, data[2]=Temp int, bytes 1 and 3 are decimals (always 0 for DHT11)
    r.humidityRH = (float)data[0] + (float)data[1] / 10.0f;
    r.temperatureC = (float)data[2] + (float)data[3] / 10.0f;
    r.valid = true;
    return r;
  }

private:
  uint8_t _pin;
};

// Interface contract bindings (frozen):
// io_0: temperatureC (f64)
// io_1: humidityRH (f64)
// io_2: dht_valid (bool)
// io_3: elapsed_ms delta_since_previous_step (i32)
// serial_0_trigger: elapsedAccMs >= 5000 (bool)
// serial_0_value_0: lastTemp
// serial_0_value_1: lastHum
// serial_0_value_2: lastValid
//
// Controller state machine (ACCEPTED model) implemented exactly as given.

static const uint8_t DHT_PIN_D7 = 7; // D7 per contract (Arduino UNO digital pin 7)
static const unsigned long CONTROL_PERIOD_MS = 10;
static const unsigned long REPORT_PERIOD_MS = 5000;
static const uint32_t SERIAL_BAUD = 9600;

static DHT11 dht(DHT_PIN_D7);

int32_t elapsedAccMs = 0;
double lastTemp = 0.0;
double lastHum = 0.0;
bool lastValid = false;

// Timekeeping for io_3 (delta since previous control step).
static unsigned long lastStepMs = 0;

// Serial line protocol: not specified beyond numeric fields; provide single-line report per trigger.
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_027\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialPrintFrame(double tempC, double humRH, bool valid);
#line 125 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_027\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 138 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_027\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_027\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialPrintFrame(double tempC, double humRH, bool valid) {
  // Include labels for robustness; each trigger prints exactly one line.
  // Format: "DHT t=<tempC>C h=<humRH>% valid=<0|1>"
  Serial.print(F("DHT "));
  Serial.print(F("t="));
  Serial.print(tempC, 1);
  Serial.print(F("C "));
  Serial.print(F("h="));
  Serial.print(humRH, 1);
  Serial.print(F("% "));
  Serial.print(F("valid="));
  Serial.println(valid ? 1 : 0);
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(DHT_PIN_D7, INPUT_PULLUP);
  lastStepMs = millis();

  // Controller initial state per model:
  elapsedAccMs = 0;
  lastTemp = 0.0;
  lastHum = 0.0;
  lastValid = false;
}

void loop() {
  unsigned long nowMs = millis();
  unsigned long elapsedSinceLast = nowMs - lastStepMs;

  // Enforce control period of 10ms; model updates run in control steps.
  if (elapsedSinceLast < CONTROL_PERIOD_MS) {
    // Idle briefly
    delay(1);
    return;
  }

  // Catch up if loop delayed more than one period: run steps for each full 10ms slice.
  // The model uses io_3 as "delta since previous step", so each step should use actual elapsed.
  // We'll compute delta as min(actual, CONTROL_PERIOD_MS) per step to preserve accumulation semantics closely.
  while ((nowMs - lastStepMs) >= CONTROL_PERIOD_MS) {
    unsigned long stepStartMs = lastStepMs;
    lastStepMs += CONTROL_PERIOD_MS;
    int32_t io_3 = (int32_t)(lastStepMs - stepStartMs); // should be 10ms each scheduled step

    // ====== Controller tick action ======
    // Model update order:
    // 1) elapsedAccMs := elapsedAccMs + io_3
    elapsedAccMs = elapsedAccMs + io_3;

    // 2) if io_2 then ... (io_2 comes from DHT read validity)
    // In the model, io_0/io_1/io_2 are read from sensor subsystem before tick.
    // Here, we perform the DHT11 read every control step? Requirement says every 5 seconds.
    // However, the model's structure implies io_2 is only expected meaningful at 5s moments.
    // To preserve behavior exactly, we implement DHT reads at 5-second cadence (requirement),
    // and set io_2 false otherwise with held-last-valid? Contract says invalid reads retain last valid
    // and validity is reported via dht_valid. We therefore:
    // - Only attempt a DHT11 read on scheduled 5-second boundaries (aligned with controller trigger).
    // - Otherwise io_2=false (no update to lastTemp/lastHum/lastValid), matching hold-last-valid.
    //
    // But the model's update 2 executes "if io_2" using current io_2 value.
    // To match "hold_last_valid", we should set io_2=true only when a new valid DHT read occurs.
    // We'll compute whether a new DHT read is due by checking the controller's report trigger BEFORE value update.
    bool reportTriggerNow = (elapsedAccMs >= (int32_t)REPORT_PERIOD_MS);

    bool io_2 = false;
    double io_0 = lastTemp;
    double io_1 = lastHum;

    // Only read DHT when report period boundary is reached/exceeded in this control step.
    if (reportTriggerNow) {
      DHT11::Reading rd = dht.read();
      io_2 = rd.valid;
      io_0 = rd.temperatureC;
      io_1 = rd.humidityRH;
    }

    if (io_2) {
      lastTemp = io_0;
      lastHum = io_1;
      lastValid = true;
    }

    // 3) if (elapsedAccMs >= 5000) then elapsedAccMs := elapsedAccMs - 5000; then if io_2 ... else lastValid=false
    if (elapsedAccMs >= (int32_t)REPORT_PERIOD_MS && true) {
      elapsedAccMs = elapsedAccMs - (int32_t)REPORT_PERIOD_MS;

      if (io_2) {
        lastTemp = io_0;
        lastHum = io_1;
        lastValid = true;
      } else {
        lastValid = false;
      }
    }

    // ====== Outputs ======
    bool serial_0_trigger = (elapsedAccMs >= (int32_t)REPORT_PERIOD_MS);
    double serial_0_value_0 = lastTemp;
    double serial_0_value_1 = lastHum;
    bool serial_0_value_2 = lastValid;

    // The contract states serial_0_trigger is true when report period elapses and a frame should be sent.
    // We'll send exactly on the trigger state computed after updates.
    if (serial_0_trigger) {
      serialPrintFrame(serial_0_value_0, serial_0_value_1, serial_0_value_2);
    }

    // Loop for next catch-up period.
    nowMs = millis();
  }
}
