#include <Arduino.h>

static const uint8_t LED_D13 = 13;   // ON/OFF indicator
static const uint8_t PWM_D9 = 9;     // PWM LED
static const uint8_t BUZZ_D6 = 6;     // Buzzer (tone)
static const uint8_t BTN_D2 = 2;      // Button input (uses internal pull-up)

static const unsigned long SERIAL_IDLE_TIMEOUT_MS = 20;
static const uint32_t SERIAL_BAUD = 9600;

// Execution states
static bool g_ledOn = false;        // D13 state
static bool g_blinking = false;    // BLINK mode active
static bool g_pwmMode = false;     // PWM mode active
static bool g_buzzMode = false;    // BUZZ mode active

static uint8_t g_pwmValue = 0;      // 0..255

// Blink scheduling
static unsigned long g_blinkLastMs = 0;
static bool g_blinkLedState = false;
static const unsigned long BLINK_INTERVAL_MS = 300; // preserves a clear visible behavior

// Button edge detection + debouncing
static bool g_prevBtnRaw = true;     // due to INPUT_PULLUP: released is HIGH
static unsigned long g_btnStableLastMs = 0;
static bool g_btnStable = true;
static const unsigned long BTN_DEBOUNCE_MS = 30;

// Serial command buffering
static String g_lineBuf = "";
static unsigned long g_lastCharMs = 0;

// Helpers
static void applyLedState(bool on) {
  g_ledOn = on;
  g_blinking = false; // setting ON/OFF cancels blink behavior
  digitalWrite(LED_D13, on ? HIGH : LOW);
}

static void setPwm(uint8_t value) {
  g_pwmValue = value;
  g_pwmMode = true;
  g_blinking = false;
  g_buzzMode = false;
  analogWrite(PWM_D9, g_pwmValue);
}

static void stopPwm() {
  g_pwmMode = false;
  analogWrite(PWM_D9, 0);
}

static void stopBuzzer() {
  g_buzzMode = false;
  noTone(BUZZ_D6);
}

static void startBlink() {
  g_blinking = true;
  g_pwmMode = false;
  g_buzzMode = false;
  stopPwm();
  stopBuzzer();

  // Start blinking immediately with current time
  g_blinkLastMs = millis();
  g_blinkLedState = false;
  digitalWrite(LED_D13, LOW);
}

static void startBuzzer(uint16_t freq) {
  g_buzzMode = true;
  g_pwmMode = false;
  g_blinking = false;
  stopPwm();

  // Tone runs; no explicit stop time required by spec.
  // Keep LED D13 consistent with current ON/OFF state.
  tone(BUZZ_D6, freq);
}

static String trimCopy(const String &s) {
  int start = 0;
  while (start < (int)s.length() && isspace((unsigned char)s[start])) start++;
  int end = (int)s.length() - 1;
  while (end >= start && isspace((unsigned char)s[end])) end--;
  if (end < start) return String("");
  return s.substring(start, end + 1);
}

static void handleCommand(const String &raw) {
  String cmdline = trimCopy(raw);
  if (cmdline.length() == 0) return;

  // Support forms:
  // ON
  // OFF
  // BLINK
  // PWM:<value>
  // BUZZ  (with optional parameters? spec only says BUZZ)
  // STATUS
  //
  // Robust parsing for "PWM:数值"
  // and ignoring any surrounding whitespace.
  if (cmdline == "ON") {
    g_pwmMode = false;
    g_buzzMode = false;
    stopPwm();
    stopBuzzer();
    g_blinking = false;
    digitalWrite(LED_D13, HIGH);
    g_ledOn = true;
    return;
  }

  if (cmdline == "OFF") {
    g_pwmMode = false;
    g_buzzMode = false;
    stopPwm();
    stopBuzzer();
    g_blinking = false;
    digitalWrite(LED_D13, LOW);
    g_ledOn = false;
    return;
  }

  if (cmdline == "BLINK") {
    startBlink();
    return;
  }

  if (cmdline.startsWith("PWM:") || cmdline.startsWith("PWM:")) {
    String numStr = cmdline.substring(4);
    numStr = trimCopy(numStr);
    long v = 0;
    if (numStr.length() > 0) {
      // Accept decimal; constrain to 0..255
      v = numStr.toInt();
    }
    if (v < 0) v = 0;
    if (v > 255) v = 255;
    setPwm((uint8_t)v);
    return;
  }

  if (cmdline == "BUZZ") {
    // Spec doesn't provide frequency; use a reasonable fixed frequency.
    // Keep behavior deterministic.
    stopPwm();
    stopBuzzer(); // ensure known start
    startBuzzer(2000); // 2 kHz
    return;
  }

  if (cmdline == "STATUS") {
    // Button press should output current LED state and PWM value,
    // but STATUS should also provide similar information.
    Serial.print("LED:");
    Serial.print(g_ledOn ? 1 : 0);
    Serial.print(",PWM:");
    Serial.println(g_pwmValue);
    return;
  }

  // Unknown command: ignore (do not output anything).
}

static void flushIfIdle() {
  unsigned long now = millis();
  if (g_lineBuf.length() == 0) return;
  if (now - g_lastCharMs >= SERIAL_IDLE_TIMEOUT_MS) {
    handleCommand(g_lineBuf);
    g_lineBuf = "";
  }
}

static void receiveSerial() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    // Treat newline or carriage return as end-of-line
    if (c == '\n' || c == '\r') {
      if (g_lineBuf.length() > 0) {
        handleCommand(g_lineBuf);
        g_lineBuf = "";
      } else {
        // ignore empty lines
      }
      continue;
    }

    g_lineBuf += c;
    g_lastCharMs = millis();
  }
}

static void handleButton() {
  bool btnRaw = digitalRead(BTN_D2); // HIGH when released, LOW when pressed

  // Debounce: track stable value
  if (btnRaw != g_prevBtnRaw) {
    g_btnStableLastMs = millis();
    g_prevBtnRaw = btnRaw;
  }

  unsigned long now = millis();
  if (now - g_btnStableLastMs >= BTN_DEBOUNCE_MS && btnRaw != g_btnStable) {
    g_btnStable = btnRaw;

    // Button "pressed" edge: for pull-up input, pressed => LOW
    if (g_btnStable == LOW) {
      // Output current LED state and PWM value via serial
      Serial.print("LED:");
      Serial.print(g_ledOn ? 1 : 0);
      Serial.print(",PWM:");
      Serial.println(g_pwmValue);
    }
  }
}

void setup() {
  pinMode(LED_D13, OUTPUT);
  pinMode(PWM_D9, OUTPUT);   // PWM output
  pinMode(BUZZ_D6, OUTPUT);  // tone() controls timer, but pin must be configured
  pinMode(BTN_D2, INPUT_PULLUP);

  // Initial states: OFF, PWM=0, buzzer stopped
  g_ledOn = false;
  g_blinking = false;
  g_pwmMode = false;
  g_buzzMode = false;
  g_pwmValue = 0;
  digitalWrite(LED_D13, LOW);
  analogWrite(PWM_D9, 0);
  noTone(BUZZ_D6);

  Serial.begin(SERIAL_BAUD);
  g_lineBuf = "";
  g_lastCharMs = millis();
}

void loop() {
  // 1) Process incoming serial
  receiveSerial();
  flushIfIdle();

  // 2) Button event
  handleButton();

  // 3) Blink mode execution
  if (g_blinking) {
    unsigned long now = millis();
    if (now - g_blinkLastMs >= BLINK_INTERVAL_MS) {
      g_blinkLastMs = now;
      g_blinkLedState = !g_blinkLedState;
      g_ledOn = g_blinkLedState;
      digitalWrite(LED_D13, g_blinkLedState ? HIGH : LOW);
    }
  }

  // 4) If buzzer mode is active, tone() continues automatically.
  // No extra scheduling required unless OFF/ON/PWM/BLINK cancels it.
}