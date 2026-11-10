#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Pin assignments (per requirements)
static const uint8_t PIN_TRIG = D8;   // HC-SR04 TRIG = D8
static const uint8_t PIN_ECHO = D9;   // HC-SR04 ECHO = D9
static const uint8_t PIN_CLK  = D4;   // TM1637 CLK = D4
static const uint8_t PIN_DIO  = D5;   // TM1637 DIO = D5
static const uint8_t PIN_LED  = D13;  // LED D13, HIGH = ON

TM1637Display display(PIN_CLK, PIN_DIO);

// Requirements
static const unsigned long ECHO_TIMEOUT_US = 25000UL;   // 每次等待回波必须有25000us超时
static const uint16_t VALID_CM_MIN = 2;                  // 有效范围 2..400cm
static const uint16_t VALID_CM_MAX = 400;                // 有效范围 2..400cm
static const uint16_t LED_THRESHOLD_CM = 10;             // <10cm LED亮，否则灭

// Sampling start interval must be 60..160ms; update visible within 250ms.
// We'll poll non-blocking for the echo only within timeout, but that may exceed interval;
// ensure we don't intentionally wait beyond timeout.
static const unsigned long START_INTERVAL_MS_MIN = 60;
static const unsigned long START_INTERVAL_MS_MAX = 160;

// For integer display (quantization tolerance 1cm), display integer cm.
// However, LED decision must use measured distance (not display rounding).
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void clearDisplay();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showBlank();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void triggerUltrasonic();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool measureEchoTimeUS(unsigned long &echoTimeUs);
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void clearDisplay() {
  // Clear (blank). TM1637Display::setSegments with 0x00 on both digits works as blank
  // but number of digits can be set; we can just show blank via display.showNumberDecEx.
  // Use "leading zero" off and "blank" behavior.
  display.showNumberDecEx(0, 0b01000000, true, 4, 0); // last params are ignored by older libs; robust alternative below
}

// More robust blank for typical TM1637Display library:
static void showBlank() {
  // 4-digit blank: setSegments with 0 for all segments.
  // TM1637Display has showSegments but not always visible; we'll use showNumberDecEx with invalid.
  // We'll explicitly blank by turning off segments: write 0x00 for each digit.
  uint8_t blankSegs[4] = {0x00, 0x00, 0x00, 0x00};
  display.setSegments(blankSegs);
}

static inline void triggerUltrasonic() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(PIN_TRIG, LOW);
}

static inline bool measureEchoTimeUS(unsigned long &echoTimeUs) {
  // Wait for echo start (HIGH) with timeout
  unsigned long startWait = micros();
  while (digitalRead(PIN_ECHO) == LOW) {
    if ((micros() - startWait) >= ECHO_TIMEOUT_US) {
      return false; // no echo / too long
    }
  }

  // Once HIGH, measure width with timeout
  unsigned long startMeasure = micros();
  while (digitalRead(PIN_ECHO) == HIGH) {
    if ((micros() - startMeasure) >= ECHO_TIMEOUT_US) {
      return false; // stuck high / too long
    }
  }

  echoTimeUs = micros() - startMeasure;
  return true;
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_TRIG, LOW);
  digitalWrite(PIN_LED, LOW); // 初始 LED 灭

  // TM1637 display init
  display.setBrightness(0x0f);

  // 初始显示为空白
  showBlank();

  // Randomize start interval within required range (60..160ms)
  // (No serial output required)
  randomSeed(analogRead(A0));
}

void loop() {
  static unsigned long nextStartMs = 0;
  static bool first = true;

  if (first) {
    nextStartMs = millis();
    first = false;
  }

  unsigned long nowMs = millis();
  if ((long)(nowMs - nextStartMs) >= 0) {
    // Schedule next start in 60..160ms
    unsigned long interval = (unsigned long)random(START_INTERVAL_MS_MIN, START_INTERVAL_MS_MAX + 1);
    nextStartMs = nowMs + interval;

    // Perform one measurement attempt
    digitalWrite(PIN_LED, LOW); // default: OFF until valid <10cm

    unsigned long echoTimeUs = 0;
    bool ok = measureEchoTimeUS(echoTimeUs);

    // If no echo / stuck high / timeout: blank display and LED off, then continue
    if (!ok) {
      showBlank();
      digitalWrite(PIN_LED, LOW);
      return;
    }

    // Convert echo time to distance in cm: cm = us / 58
    // Use measured distance (not display quantized) for LED decision.
    // echoTimeUs is integer microseconds, so compute as float for strict threshold.
    float distCmMeasured = (float)echoTimeUs / 58.0f;

    // Validate strict range 2..400cm (based on measured distance)
    bool valid = (distCmMeasured >= (float)VALID_CM_MIN) && (distCmMeasured <= (float)VALID_CM_MAX);

    if (!valid) {
      showBlank();
      digitalWrite(PIN_LED, LOW);
      return;
    }

    // LED threshold uses measured distance, not rounded display value:
    // <10cm => LED ON; >=10cm => LED OFF
    if (distCmMeasured < (float)LED_THRESHOLD_CM) {
      digitalWrite(PIN_LED, HIGH);
    } else {
      digitalWrite(PIN_LED, LOW);
    }

    // Display integer centimeters (quantized).
    // "前导零或空白均可": we can right-align and allow leading zeros off.
    // Use rounding to nearest integer to approximate 1cm quantization tolerance.
    int distCmInt = (int)lround(distCmMeasured);

    // Ensure shown value is within expected integer range; if lround pushes outside,
    // keep behavior consistent with measured validity check (still blank not needed).
    if (distCmInt < 0 || distCmInt > 9999) {
      showBlank();
      digitalWrite(PIN_LED, LOW);
      return;
    }

    // Show integer centimeters
    // Use 4 digits; leading zeros off.
    display.showNumberDecEx(distCmInt, 0, true);

    // Note: Requirement says include measurement waiting + display transmission within 250ms.
    // This loop iteration may already include up to 25ms timeout for echo; typical flow is within limit.
    // No additional delay here.
  }
}
