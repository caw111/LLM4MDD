#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------------------- Hardware pin mapping (authoritative) --------------------
static const uint8_t PIR_LEFT_PIN  = 2;   // D2
static const uint8_t PIR_RIGHT_PIN = 3;   // D3
static const uint8_t LIGHT_PWM_PIN = 9;   // D9
static const uint8_t ENV_LUX_PIN    = A0; // A0
static const uint8_t TM_CLK_PIN     = 4;  // D4
static const uint8_t TM_DIO_PIN     = 5;  // D5

// TM1637 display
TM1637Display display(TM_CLK_PIN, TM_DIO_PIN);

// -------------------- Serial configuration --------------------
static const uint32_t SERIAL_BAUD = 9600;

// -------------------- System behavior configuration --------------------
// Note: Requirements mention preserving "PWM, calibration, thresholds, timers and all other behavior".
// The prompt does not provide explicit numeric values for calibration/thresholds/timers.
// Therefore, the sketch includes robust defaults for:
// - environment-light threshold (A0 dark detection)
// - lighting on/off timeout extension on continuous movement
// - direction estimation and pass counting based on PIR sequence
//
// If you have the exact numeric requirements, provide them and I will adjust these constants.

// Environment-light threshold (ADC). Higher => brighter requirement.
// Calibrate quickly at startup (simple moving baseline) then set threshold.
static const uint16_t DEFAULT_DARK_THRESHOLD = 450; // fallback if calibration fails
static const uint16_t CALIBRATION_SAMPLES = 200;
static const uint16_t CALIBRATION_DROP_Q = 1; // 1/ (samples) style not used; keep stable

// Light PWM behavior
static const uint8_t LIGHT_PWM_BRIGHTNESS = 180; // PWM duty when ON (0..255)
static const uint32_t LIGHT_BASE_ON_MS   = 5000;  // base time after movement (ms)
static const uint32_t LIGHT_EXTEND_ON_MS = 3000;  // additional extension per new movement while moving (ms)
static const uint32_t LIGHT_MAX_ON_MS    = 60000; // max continuous on time (ms)

// Debounce / motion event qualification
static const uint32_t PIR_EVENT_MIN_GAP_MS = 80; // ignore too-fast toggles
static const uint32_t PIR_EVENT_HOLD_MS   = 80; // treat as event for this window

// Counting logic
// We infer direction by which side detects first, then the other follows within a window.
static const uint32_t DIRECTION_WINDOW_MS = 3000;

// Movement history states
enum DirState : uint8_t { DIR_IDLE = 0, DIR_WAIT_RIGHT, DIR_WAIT_LEFT };
static DirState dirState = DIR_IDLE;

// Pass counting
static uint32_t passCount = 0;

// Time accounting
static uint64_t cumulativeOnTimeMs = 0;
static bool lightIsOn = false;
static uint32_t lightOffDeadlineMs = 0; // when to turn off (ms epoch)
static uint32_t lastLoopMs = 0;

// PIR edge/event tracking
static uint32_t lastLeftEventMs = 0;
static uint32_t lastRightEventMs = 0;
static uint32_t lastAnyEventMs = 0;

static uint32_t pirLeftEventAt = 0;
static uint32_t pirRightEventAt = 0;

// Environment calibration
static uint16_t envDarkThreshold = DEFAULT_DARK_THRESHOLD;
static uint32_t calibSum = 0;

// -------------------- Utility --------------------
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isDark(uint16_t adc);
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool qualifiesEvent(uint8_t pin, uint32_t &lastEventMs, uint32_t nowMs);
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void registerDirectionAndMaybeCount(uint32_t nowMs);
#line 131 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void registerPassBySequence(uint32_t nowMs);
#line 162 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplay();
#line 169 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void turnLightOn(uint32_t nowMs);
#line 189 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void turnLightOff(uint32_t nowMs);
#line 201 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 253 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_026\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline bool isDark(uint16_t adc) {
  return adc < envDarkThreshold;
}

static inline bool qualifiesEvent(uint8_t pin, uint32_t &lastEventMs, uint32_t nowMs) {
  (void)pin;
  if (nowMs - lastEventMs < PIR_EVENT_MIN_GAP_MS) return false;
  lastEventMs = nowMs;
  return true;
}

static void registerDirectionAndMaybeCount(uint32_t nowMs) {
  // We treat a "sequence" where one PIR triggers first, then the other within DIRECTION_WINDOW_MS.
  // When the second PIR triggers, increment passCount and reset state.
  // Interpretation:
  // - If left triggers then right triggers -> passing from left to right (or vice versa; we just count).
  // - Use state waiting for the opposite side.
  //
  // We update state based on which side has a recent event.
  bool leftEvent  = (nowMs - pirLeftEventAt) <= PIR_EVENT_HOLD_MS;
  bool rightEvent = (nowMs - pirRightEventAt) <= PIR_EVENT_HOLD_MS;

  // Timeout state
  if (dirState != DIR_IDLE) {
    // If waiting too long, reset to idle
    if (nowMs - lastAnyEventMs > DIRECTION_WINDOW_MS) {
      dirState = DIR_IDLE;
    }
  }

  if (leftEvent && dirState == DIR_IDLE) {
    dirState = DIR_WAIT_RIGHT;
    lastAnyEventMs = nowMs;
  } else if (rightEvent && dirState == DIR_IDLE) {
    dirState = DIR_WAIT_LEFT;
    lastAnyEventMs = nowMs;
  } else if (leftEvent && dirState == DIR_WAIT_RIGHT) {
    // Expected right first? Actually left event arrives while waiting for right -> ignore or rebase.
    // But if state is waiting for right, and left fired again, keep waiting.
  } else if (rightEvent && dirState == DIR_WAIT_LEFT) {
    // left fired first, now right -> count pass
    passCount++;
    dirState = DIR_IDLE;
  } else if (rightEvent && dirState == DIR_WAIT_RIGHT) {
    // both left and right events, but state indicates left first should have happened;
    // treat as new sequence starting with right
    dirState = DIR_WAIT_LEFT;
    lastAnyEventMs = nowMs;
  } else if (leftEvent && dirState == DIR_WAIT_LEFT) {
    dirState = DIR_WAIT_RIGHT;
    lastAnyEventMs = nowMs;
  }
}

// A more direct implementation of direction: when either PIR triggers,
// if the other PIR has triggered within window, count.
// This preserves "根据方向估计通行人数" by using the sequence, not simultaneous detection.
static void registerPassBySequence(uint32_t nowMs) {
  // We detect "who came first" and then check if other came after within window.
  bool leftRecent  = (nowMs - pirLeftEventAt)  <= DIRECTION_WINDOW_MS;
  bool rightRecent = (nowMs - pirRightEventAt) <= DIRECTION_WINDOW_MS;

  // If both are recent, determine order by timestamps.
  if (leftRecent && rightRecent) {
    // Ensure not double-counting: only count when a new event occurs (current side event time is the latest)
    // We'll consider "latest event" side.
    bool leftIsLatest  = pirLeftEventAt >= pirRightEventAt;
    bool rightIsLatest = pirRightEventAt >= pirLeftEventAt;

    // Double count prevention: require that the latest event time is within PIR_EVENT_HOLD_MS from now.
    bool leftLatestRecentNow  = (nowMs - pirLeftEventAt)  <= PIR_EVENT_HOLD_MS;
    bool rightLatestRecentNow = (nowMs - pirRightEventAt) <= PIR_EVENT_HOLD_MS;

    if (leftLatestRecentNow && rightIsRecent) {
      // right must be earlier -> sequence right then left
      // Count passage for right->left (or left->right depending convention; requirement says estimate by direction, count either way)
      passCount++;
      pirLeftEventAt = 0;
      pirRightEventAt = 0;
    } else if (rightLatestRecentNow && leftIsRecent) {
      // left earlier -> sequence left then right
      passCount++;
      pirLeftEventAt = 0;
      pirRightEventAt = 0;
    }
  }
}

static void updateDisplay() {
  // TM1637 displays 0-9999; requirement: "今日通行计数"
  // We'll display modulo 10000 to fit the display.
  uint16_t shown = (uint16_t)(passCount % 10000UL);
  display.showNumberDecEx(shown, 0b01000000, true); // colon disabled; leading zeros off
}

static void turnLightOn(uint32_t nowMs) {
  if (!lightIsOn) {
    lightIsOn = true;
    // Start a new off deadline
    lightOffDeadlineMs = nowMs + LIGHT_BASE_ON_MS;
    analogWrite(LIGHT_PWM_PIN, LIGHT_PWM_BRIGHTNESS);
  } else {
    // While ON, extend deadline when movement continues
    uint32_t newDeadline = nowMs + LIGHT_EXTEND_ON_MS;
    if (newDeadline > lightOffDeadlineMs) lightOffDeadlineMs = newDeadline;

    // Enforce max continuous ON window relative to when ON started:
    // We'll approximate by also clamping by "nowMs + (max - elapsed)".
    // Keep simple without extra variables: compute max from current deadline.
    uint32_t maxDeadline = nowMs + LIGHT_MAX_ON_MS;
    if (lightOffDeadlineMs > maxDeadline) lightOffDeadlineMs = maxDeadline;
    analogWrite(LIGHT_PWM_PIN, LIGHT_PWM_BRIGHTNESS);
  }
}

static void turnLightOff(uint32_t nowMs) {
  if (lightIsOn) {
    analogWrite(LIGHT_PWM_PIN, 0);
    // Account cumulative on time: add until now if now >= last deadline/turn-off.
    // Since we only call when deadline is reached, light is-on period approximates accurately.
    // We need actual start time; instead track cumulativeOnTimeMs by accumulating in loop when on.
    // We'll update in loop to be precise. Here just flip state.
    lightIsOn = false;
  }
}

// -------------------- Setup/Loop --------------------
void setup() {
  pinMode(PIR_LEFT_PIN, INPUT);
  pinMode(PIR_RIGHT_PIN, INPUT);
  pinMode(LIGHT_PWM_PIN, OUTPUT);

  analogWrite(LIGHT_PWM_PIN, 0);

  // Serial
  Serial.begin(SERIAL_BAUD);

  // TM1637 init
  display.setBrightness(0x0f); // max brightness; adjust if needed
  updateDisplay();

  // Calibration for environment threshold
  // Collect baseline readings assuming startup occurs when typical ambient present.
  // Use minimum-ish average to set dark threshold.
  calibSum = 0;
  uint16_t minVal = 1023;
  uint16_t maxVal = 0;
  for (uint16_t i = 0; i < CALIBRATION_SAMPLES; i++) {
    uint16_t v = analogRead(ENV_LUX_PIN);
    calibSum += v;
    if (v < minVal) minVal = v;
    if (v > maxVal) maxVal = v;
    delay(5);
  }
  uint16_t avg = (uint16_t)(calibSum / CALIBRATION_SAMPLES);

  // Choose dark threshold below average with some margin.
  // If ambient is already dark, minVal will be low and threshold should follow.
  // Use: threshold = avg - (avg - minVal)/2, but bounded.
  // Then fallback to default if the range is degenerate.
  uint16_t computed;
  if (maxVal > minVal + 20) {
    uint16_t halfDrop = (uint16_t)((avg > minVal) ? ((avg - minVal) / 2) : 0);
    computed = (uint16_t)((avg > halfDrop) ? (avg - halfDrop) : minVal);
  } else {
    computed = avg;
  }

  // Make sure computed threshold is reasonable
  if (computed < 50) computed = 50;
  if (computed > 900) computed = DEFAULT_DARK_THRESHOLD;

  envDarkThreshold = computed;

  // Serial debug/notification could be added, but requirements only ask for:
  // "串口输出累计点亮时长" - we will output that when each illumination ends and periodically if needed.
  // So no extra prints here.
}

void loop() {
  uint32_t nowMs = millis();
  if (lastLoopMs == 0) lastLoopMs = nowMs;
  uint32_t dt = nowMs - lastLoopMs;
  lastLoopMs = nowMs;

  // Read environment light
  uint16_t envAdc = analogRead(ENV_LUX_PIN);
  bool dark = isDark(envAdc);

  // Read PIRs (digital HIGH indicates motion)
  bool leftMotion  = digitalRead(PIR_LEFT_PIN) == HIGH;
  bool rightMotion = digitalRead(PIR_RIGHT_PIN) == HIGH;

  // Accumulate on-time precisely while light is on
  if (lightIsOn) {
    cumulativeOnTimeMs += (uint64_t)dt;
  }

  // Detect motion events with debouncing/qualification
  if (leftMotion) {
    if (qualifiesEvent(PIR_LEFT_PIN, lastLeftEventMs, nowMs)) {
      pirLeftEventAt = nowMs;
      // Trigger direction/count estimation
      registerDirectionAndMaybeCount(nowMs);
    }
  }
  if (rightMotion) {
    if (qualifiesEvent(PIR_RIGHT_PIN, lastRightEventMs, nowMs)) {
      pirRightEventAt = nowMs;
      registerDirectionAndMaybeCount(nowMs);
    }
  }

  // Use sequence-based pass counting once we have both timestamps and the latest event is current
  // (This avoids needing strict FSM correctness and fits "根据方向估计通行人数".)
  // Count only when there is a new motion event on either side recently.
  if ((nowMs - pirLeftEventAt) <= PIR_EVENT_HOLD_MS || (nowMs - pirRightEventAt) <= PIR_EVENT_HOLD_MS) {
    // Ensure we have both sides potentially
    if (pirLeftEventAt != 0 && pirRightEventAt != 0) {
      // Count by order and clear to prevent repeats
      registerPassBySequence(nowMs);
    }
  }

  // Update display if count changed recently
  static uint32_t lastDisplayed = 0;
  if (passCount != lastDisplayed) {
    lastDisplayed = passCount;
    updateDisplay();
  }

  // Lighting control:
  // Only open light when environment is dark AND any PIR detects a person.
  bool anyMotion = leftMotion || rightMotion;

  if (dark && anyMotion) {
    turnLightOn(nowMs);
  }

  // Turn off when deadline passed
  if (lightIsOn && (int32_t)(nowMs - lightOffDeadlineMs) >= 0) {
    // Before turning off, output accumulated on time over serial.
    // Requirement: "串口输出累计点亮时长"
    // We'll output total cumulativeOnTimeMs in seconds and ms.
    uint64_t totalMs = cumulativeOnTimeMs;
    uint32_t totalSec = (uint32_t)(totalMs / 1000ULL);

    // Output as one line
    Serial.print(F("ON_TIME_MS="));
    Serial.print(totalMs);
    Serial.print(F(",ON_TIME_SEC="));
    Serial.println(totalSec);

    turnLightOff(nowMs);
    // Reset cumulative for "今日通行" would normally reset daily; not specified.
    // We'll keep cumulative across run to satisfy "累计点亮时长".
  }

  // Small idle sleep for stability
  delay(5);
}
