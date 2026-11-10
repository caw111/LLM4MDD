#include <Arduino.h>

static const uint8_t PIN_FAN_PWM = 5;   // D5 (PWM output)
static const uint8_t PIN_LED     = 13;  // D13 (overheat LED)
static const uint8_t PIN_LM35    = A1;  // A1 (LM35 analog input)

static const long SERIAL_BAUD = 9600;
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;

// ---- Temperature & Fan Control Parameters (configurable via serial) ----
static float g_targetTempC = 35.0f;     // default target threshold
static int   g_minFanPWM   = 80;        // default minimum fan PWM (0..255)
static int   g_maxFanPWM   = 255;       // keep full speed as upper bound (fixed)
static float g_fanGainC    = 10.0f;     // how fast fan ramps above threshold

// ---- Calibration ----
// LM35 output: 10mV/°C => 0.01 V/°C
// ADC: 10-bit on UNO, reference 5V by default.
static const float ADC_VREF = 5.0f;
static const float LM35_MV_PER_C = 10.0f; // 10 mV/°C

// ---- State ----
static bool g_overheat = false;

static String g_lineBuf;

// Convert analog reading to temperature (°C)
static float readTemperatureC() {
  int raw = analogRead(PIN_LM35); // 0..1023
  float v = (static_cast<float>(raw) * ADC_VREF) / 1023.0f; // volts
  float tempC = (v * 1000.0f) / LM35_MV_PER_C;              // mV / (10mV/°C)
  return tempC;
}

static int clampInt(int x, int lo, int hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

static uint8_t fanPWMForTemp(float tC) {
  // Requirement:
  // - If temperature exceeds threshold by 5°C or more -> LED on and "overheat" warning
  // - System automatically adjusts fan based on real-time temperature
  //
  // We'll implement:
  //   - Below or equal to threshold: fan = minFanPWM (to ensure minimum rotation)
  //   - From (threshold, threshold + 5): fan ramps from minFanPWM up towards max
  //   - At (threshold + 5) and above: fan reaches maxFanPWM (and overheat condition)
  //
  // Uses g_fanGainC to map the (tC - target) beyond threshold to extra PWM.
  float delta = tC - g_targetTempC;

  int pwm = g_minFanPWM;

  if (delta > 0.0f) {
    // Ramp: extra based on delta. At delta >= 5 => reach max.
    // Scale so that 5°C maps to maxFanPWM.
    float ramp = delta / g_fanGainC;
    if (g_fanGainC <= 0.0f) ramp = 1.0f;
    int extra = static_cast<int>((static_cast<long>(g_maxFanPWM - g_minFanPWM) * ramp));
    pwm = g_minFanPWM + extra;
  }

  pwm = clampInt(pwm, 0, g_maxFanPWM);
  return static_cast<uint8_t>(pwm);
}

// Minimal serial parser:
// Commands supported (case-insensitive):
//   T=<float> : set target temp threshold (°C)
//   F=<int>   : set minimum fan speed PWM (0..255)
//   ?         : print current config + temperature
//
// Line endings: newline or idle timeout (line_or_idle).
static void processCommand(const String &lineRaw) {
  String line = lineRaw;
  line.trim();
  if (line.length() == 0) return;

  // Make lowercase copy
  String lower = line;
  for (size_t i = 0; i < lower.length(); i++) lower[i] = (char)tolower(lower[i]);

  if (lower == "?" || lower == "help" || lower == "h") {
    float t = readTemperatureC();
    Serial.print("OK ");
    Serial.print("T="); Serial.print(g_targetTempC, 2);
    Serial.print("C ");
    Serial.print("MIN_PWM="); Serial.print(g_minFanPWM);
    Serial.print(" PWM ");
    Serial.print("TEMP="); Serial.print(t, 2);
    Serial.println("C");
    return;
  }

  // Allow "T=xx" or "TEMP=xx"
  if (lower.startsWith("t=") || lower.startsWith("temp=")) {
    int eq = line.indexOf('=');
    if (eq >= 0) {
      String val = line.substring(eq + 1);
      val.trim();
      float newT = val.toFloat();
      g_targetTempC = newT;
      Serial.print("OK ");
      Serial.print("TARGET_TEMP_C="); Serial.println(g_targetTempC, 2);
    }
    return;
  }

  // Allow "F=xx" or "MIN_PWM=xx"
  if (lower.startsWith("f=") || lower.startsWith("min_pwm=") || lower.startsWith("minfan=") || lower.startsWith("fanmin=")) {
    int eq = line.indexOf('=');
    if (eq >= 0) {
      String val = line.substring(eq + 1);
      val.trim();
      int newF = val.toInt();
      g_minFanPWM = clampInt(newF, 0, g_maxFanPWM);
      Serial.print("OK ");
      Serial.print("MIN_FAN_PWM="); Serial.println(g_minFanPWM);
    }
    return;
  }

  // Allow "SET T=xx F=yy" (space-separated)
  if (lower.startsWith("set")) {
    // Attempt to parse T and F anywhere in the line
    // Find T=
    int tpos = lower.indexOf("t=");
    if (tpos < 0) tpos = lower.indexOf("temp=");
    if (tpos >= 0) {
      int eq = line.indexOf('=', tpos);
      if (eq >= 0) {
        int end = line.indexOf(' ', eq + 1);
        if (end < 0) end = line.length();
        String val = line.substring(eq + 1, end);
        val.trim();
        float newT = val.toFloat();
        g_targetTempC = newT;
      }
    }
    int fpos = lower.indexOf("f=");
    if (fpos < 0) fpos = lower.indexOf("min_pwm=");
    if (fpos < 0) fpos = lower.indexOf("minfan=");
    if (fpos < 0) fpos = lower.indexOf("fanmin=");
    if (fpos >= 0) {
      int eq = line.indexOf('=', fpos);
      if (eq >= 0) {
        int end = line.indexOf(' ', eq + 1);
        if (end < 0) end = line.length();
        String val = line.substring(eq + 1, end);
        val.trim();
        int newF = val.toInt();
        g_minFanPWM = clampInt(newF, 0, g_maxFanPWM);
      }
    }

    Serial.print("OK ");
    Serial.print("T="); Serial.print(g_targetTempC, 2);
    Serial.print("C ");
    Serial.print("MIN_PWM="); Serial.println(g_minFanPWM);
    return;
  }

  // Unknown command
  Serial.print("ERR Unknown: ");
  Serial.println(line);
}

void setup() {
  pinMode(PIN_FAN_PWM, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  analogReference(DEFAULT);

  // Fan off initially; then loop will set based on temp.
  analogWrite(PIN_FAN_PWM, 0);
  digitalWrite(PIN_LED, LOW);

  Serial.begin(SERIAL_BAUD);
  g_lineBuf.reserve(64);
}

void loop() {
  // ---- Serial line collection with idle timeout (line_or_idle) ----
  static uint32_t lastByteTime = 0;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastByteTime = millis();

    // Accept both newline and carriage return as terminators.
    if (c == '\n' || c == '\r') {
      if (g_lineBuf.length() > 0) {
        processCommand(g_lineBuf);
        g_lineBuf = "";
      }
    } else {
      // If user sends long data, avoid unlimited buffer growth.
      if (g_lineBuf.length() < 120) g_lineBuf += c;
    }
  }

  // If no new bytes arrived recently, treat idle as end-of-line.
  if (g_lineBuf.length() > 0 && (millis() - lastByteTime) >= SERIAL_IDLE_TIMEOUT_MS) {
    processCommand(g_lineBuf);
    g_lineBuf = "";
  }

  // ---- Read temperature and control fan ----
  float tC = readTemperatureC();

  // Overheat rule: LED on and output "overheat" warning if temp >= threshold + 5°C
  bool newOverheat = (tC >= (g_targetTempC + 5.0f));
  if (newOverheat != g_overheat) {
    g_overheat = newOverheat;
    digitalWrite(PIN_LED, g_overheat ? HIGH : LOW);

    if (g_overheat) {
      Serial.print("overheat ");
      Serial.print("TEMP="); Serial.print(tC, 2);
      Serial.print("C ");
      Serial.print("THRESH="); Serial.print(g_targetTempC, 2);
      Serial.println("C");
    } else {
      Serial.println("OK overheat_cleared");
    }
  }

  uint8_t pwm = fanPWMForTemp(tC);
  analogWrite(PIN_FAN_PWM, pwm);

  // Small delay to stabilize ADC and reduce serial spam
  delay(50);
}