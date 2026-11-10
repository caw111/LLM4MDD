#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ----- Frozen interface contract pins -----
static const uint8_t PIN_IR_NEAR      = 2;   // D2
static const uint8_t PIN_VALVE_SOLENOID = 10; // D10 (digital output)
static const uint8_t PIN_LED_STATUS   = 13;  // D13 (digital output)
static const uint8_t PIN_BUZZER_TONE   = 6;   // D6  (tone output)

static const uint8_t PIN_TRIG          = 8;   // HC-SR04 Trig D8
static const uint8_t PIN_ECHO          = 9;   // HC-SR04 Echo D9

static const uint8_t TM1637_CLK        = 4;   // D4
static const uint8_t TM1637_DIO        = 5;   // D5

// ----- Contract timing -----
static const uint32_t CONTROL_PERIOD_MS = 10;

// ----- Ultrasonic contract -----
static const uint32_t US_TIMEOUT_US = 30000UL; // within 25000..60000
static const uint32_t ULTRASONIC_SAMPLE_PERIOD_MS = 60;

// ----- Display -----
TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

// Contract says io_4 is i32 but model also implies showing remaining water percentage and today's usage count
// Since exact encoding rules weren't specified, implement a safe deterministic behavior:
// - If water level valid: show remaining percent in the left 2 digits and units/tens of today usage in right 2 digits
// - If invalid: show "----" (blank) by writing 0x00 segments (tm library uses blanks for 0x00) for all digits.
// This preserves deterministic behavior and avoids guessing complex multiplexing rules.
// The controller output expression (io_4) is what matters for "remaining water percent and usage count";
// we compute display content accordingly without altering io_4 expression in model semantics.
static uint32_t lastDisplayUpdateMs = 0;

// ----- Model state (ACCEPTED) -----
static bool previousNear = false;
static int32_t todayUsageCount = 0;

// ----- Sensor I/O -----
static bool io_0 = false;       // IR near (digital input)
static float io_2 = 0.0f;      // ultrasonic distance cm (f64)
static bool io_3 = false;      // ultrasonic validity (bool)

static float lastValidDistanceCm = 0.0f;

// ----- Output I/O computed from model -----
static bool io_1 = false;      // valve command
static int32_t io_4 = 0;      // display value (model expression uses only io_2 and io_3; but contract says shows percent and today count)
static int32_t io_5 = 0;      // buzzer tone control (Hz)
static bool io_6 = false;      // LED

// ----- "serial_0_trigger" always false in model -----
static const bool serial_0_trigger = false;

// ----- Serial report gating protocol -----
// Not specified; since model states serial_0_trigger is always false, the device never outputs report.
// Still implement a minimal handler for robustness if host sends query; it will only respond if gate becomes true (never).
static String serialLineBuf;

// ----- Ultrasonic measurement implementing hold_last_valid invalid_policy -----
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readUltrasonicCm(float &distanceCmOut, bool &validOut);
#line 106 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void modelTick();
#line 161 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateDisplay();
#line 186 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateDisplayFallback();
#line 202 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 222 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void handleSerial();
#line 241 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_015\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readUltrasonicCm(float &distanceCmOut, bool &validOut) {
  // Ensure a deterministic, bounded timeout.
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  unsigned long startUs = micros();
  unsigned long timeoutUs = US_TIMEOUT_US;

  // Wait for echo HIGH
  while (digitalRead(PIN_ECHO) == LOW) {
    if ((micros() - startUs) > timeoutUs) {
      validOut = false;
      return false;
    }
  }

  // Measure HIGH pulse width
  unsigned long echoStart = micros();
  while (digitalRead(PIN_ECHO) == HIGH) {
    if ((micros() - echoStart) > timeoutUs) {
      validOut = false;
      return false;
    }
  }
  unsigned long echoEnd = micros();
  unsigned long pulseWidthUs = echoEnd - echoStart;

  // HC-SR04: distance (cm) = pulse_us / 58.0
  float dist = (float)pulseWidthUs / 58.0f;

  if (dist < 0.0f || dist > 100000.0f) {
    validOut = false;
    return false;
  }

  distanceCmOut = dist;
  validOut = true;
  return true;
}

// ----- Compute model outputs exactly as in Accepted executable model -----
static void modelTick() {
  // Inputs: io_0, io_2, io_3 already sampled/held per contract.

  // Update: tick action
  // perform action tick:
  // first start; then if (io_0 and not previousNear) assign todayUsageCount := todayUsageCount + 1;
  // then assign previousNear := io_0;
  if (io_0 && (!previousNear)) {
    todayUsageCount = todayUsageCount + 1;
  }
  previousNear = io_0;

  // Outputs:
  // io_1 = if (io_0 and io_3) then ( ((io_2 >= 10.0)) and io_3 ) else false
  if (io_0 && io_3) {
    io_1 = (io_2 >= 10.0f) && io_3;
  } else {
    io_1 = false;
  }

  // io_4:
  // if io_3 then if (io_2 < 2.0) 0
  // else if (io_2 >= 400.0) 100
  // else floor(100 - ((400 - io_2)/3.98))
  // else 0
  if (io_3) {
    if (io_2 < 2.0f) {
      io_4 = 0;
    } else if (io_2 >= 400.0f) {
      io_4 = 100;
    } else {
      float val = 100.0f - (((400.0f - io_2) / 3.98f)));
      // floor
      io_4 = (int32_t)floorf(val);
    }
  } else {
    io_4 = 0;
  }

  // io_5 = if (io_0 and io_3 and io_2 < 10.0) then 2000 else 0
  io_5 = (io_0 && io_3 && (io_2 < 10.0f)) ? 2000 : 0;

  // io_6 = if (io_0 and io_3 and io_2 >= 10.0) then true else false
  io_6 = (io_0 && io_3 && (io_2 >= 10.0f)) ? true : false;

  // serial_0_trigger = if false then true else false => always false
  // serial_0_value_0 = todayUsageCount
  // serial_0_value_1 = if io_3 then mapping else 0.0
  // serial_0_value_2 = io_3
  // Since trigger is always false, we do not emit anything over serial in steady state.
}

// ----- Display encoding helper (deterministic) -----
// We'll show percent (io_4) in left two digits and todayUsageCount%100 in right two digits.
// If io_3 false, blank all digits.
static void updateDisplay() {
  if (!io_3) {
    // Blank all digits
    tm1637.setBrightness(7);
    // TM1637Display: showNumberDec can’t blank reliably; use raw segments if available via library.
    // We'll write 0 to each digit using setSegments with empty patterns.
    uint8_t blank[4] = {0, 0, 0, 0};
    tm1637.setSegments(blank);
    return;
  }

  tm1637.setBrightness(7);

  int percent = (int)io_4; // 0..100
  int usage = todayUsageCount % 100;

  // Compose: PPUU
  // Use colon false; show as 4 digits without leading zeros.
  int combined = (percent * 100) + usage;
  tm1637.showNumberDecEx(combined, 0b01000000, true, 4, 0); // dp unused, adjust: library supports flags varies
}

// Some TM1637Display versions may not support showNumberDecEx with these params.
// Provide a fallback compile-time safe approach using showNumberDecEx overloads we know.
// We'll instead use showNumberDec for 4 digits.
static void updateDisplayFallback() {
  if (!io_3) {
    uint8_t blank[4] = {0, 0, 0, 0};
    tm1637.setSegments(blank);
    return;
  }
  tm1637.setBrightness(7);

  int percent = (int)io_4;             // 0..100
  int usage = todayUsageCount % 100;  // 0..99

  // show PP UU as 4 digits
  int combined = percent * 100 + usage;
  tm1637.showNumberDec(combined, false, 4, 0);
}

void setup() {
  pinMode(PIN_IR_NEAR, INPUT);           // contract: digital_input, active_high, pullup false
  pinMode(PIN_VALVE_SOLENOID, OUTPUT);  // D10
  pinMode(PIN_LED_STATUS, OUTPUT);     // D13
  pinMode(PIN_BUZZER_TONE, OUTPUT);    // D6

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_VALVE_SOLENOID, LOW);
  digitalWrite(PIN_LED_STATUS, LOW);
  noTone(PIN_BUZZER_TONE);

  tm1637.setBrightness(7);

  Serial.begin(9600);
  serialLineBuf.reserve(64);
}

// Process serial line queries; gate is always false, so suppress output.
static void handleSerial() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\n' || c == '\r') {
      if (serialLineBuf.length() > 0) {
        // Interpret any query; but since serial_0_trigger is always false,
        // no report is sent. This avoids changing model behavior.
        // If needed, could log or ignore deterministically.
        serialLineBuf = "";
      }
    } else {
      if (serialLineBuf.length() < 120) serialLineBuf += c;
    }
  }

  // Optionally clear idle; contract framing uses idle_timeout_ms=20, but MCU doesn't have exact model.
  // Deterministic: do nothing else.
}

void loop() {
  static uint32_t nextControlMs = millis();
  static uint32_t lastUltrasonicMs = 0;

  uint32_t nowMs = millis();
  // Keep 10ms control period
  if ((int32_t)(nowMs - nextControlMs) < 0) {
    handleSerial();
    return;
  }
  nextControlMs += CONTROL_PERIOD_MS;

  // Sample inputs:
  // io_0: IR proximity D2 (digital input)
  io_0 = (digitalRead(PIN_IR_NEAR) == HIGH);

  // io_2 / io_3: ultrasonic distance + validity at 60ms sample period
  if (nowMs - lastUltrasonicMs >= ULTRASONIC_SAMPLE_PERIOD_MS) {
    lastUltrasonicMs = nowMs;

    float distCm = 0.0f;
    bool valid = false;
    bool ok = readUltrasonicCm(distCm, valid);

    // invalid_policy: hold_last_valid
    if (ok && valid) {
      io_2 = distCm;
      io_3 = true;
      lastValidDistanceCm = distCm;
    } else {
      io_2 = lastValidDistanceCm;
      io_3 = false;
    }
  }

  // Tick model and compute outputs (exact update ordering for previousNear/todayUsageCount)
  modelTick();

  // Drive outputs:
  digitalWrite(PIN_VALVE_SOLENOID, io_1 ? HIGH : LOW);
  digitalWrite(PIN_LED_STATUS, io_6 ? HIGH : LOW);

  if (io_5 != 0) {
    // Contract tone output: alarm when too low; model outputs integer "Hz"
    tone(PIN_BUZZER_TONE, (unsigned int)io_5);
  } else {
    noTone(PIN_BUZZER_TONE);
  }

  // Update TM1637 display deterministically at control period (could be every time; contract sample_period_ms=10 for io_4)
  // i.e., aligned with control_period_ms (10ms)
  updateDisplayFallback();

  // Serial gating: model serial_0_trigger always false => no report emitted.
  // Still handle serial input deterministically.
  handleSerial();

  // Ensure we don't drift too far if loop is slow
  uint32_t cur = millis();
  if ((int32_t)(cur - nextControlMs) > (int32_t)CONTROL_PERIOD_MS) {
    nextControlMs = cur;
  }
}
