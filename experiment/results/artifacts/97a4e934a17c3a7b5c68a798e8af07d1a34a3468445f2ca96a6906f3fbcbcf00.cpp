#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PIN_LM35 = A1;
static const uint8_t PIN_FAN_PWM = 5;   // D5
static const uint8_t PIN_LED = 13;      // D13

// Frozen interface contract constants
static const unsigned long CONTROL_PERIOD_MS = 10;
static const uint32_t SERIAL_BAUD = 9600;
static const uint16_t SERIAL_IDLE_TIMEOUT_MS = 20;

// Model states (exact names/initials from bundle)
static double targetTempC = 25.0;
static int32_t minFanPwm = 30;
static double serialSampleTempC = 0.0;
static bool ledOverheatLatched = false;

// Model inputs (io_3_valid, io_3)
static int32_t io_3 = 0;
static bool io_3_valid = false;

// Internal: serial frame idle detection for "line_or_idle"
static unsigned long lastSerialByteMs = 0;
static bool frameHasAnyByte = false;

// Accepted serial tokens mapping from contract (exact i32 codes)
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool parseTokenAndProduceCommand(const String &token, int32_t &outCmd);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyCommand(int32_t cmd);
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static double computeFanPwmCommand();
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setOutputs(bool overheat, double pwmCmd);
#line 129 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialReport(bool trigger);
#line 144 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 154 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool parseTokenAndProduceCommand(const String &token, int32_t &outCmd) {
  // Contract framing: "line_or_idle" idle timeout 20ms
  // We'll accept tokens delimited by whitespace/newline, or complete line.
  if (token == "RESET") { outCmd = 31; return true; }
  if (token == "SAVE")  { outCmd = 32; return true; }

  if (token.length() >= 2) {
    char sign = token.charAt(0); // 'T' or 'F'
    if (sign == 'T' || sign == 'F') {
      // Token like "T+10" or "T-5" or "F+3"
      if (token.length() >= 4) {
        char op = token.charAt(1); // '+' or '-'
        int n = token.substring(2).toInt();
        if (sign == 'T') {
          // T+1..T+20 => 1..20; T-1..T-5 => 21..25
          if (op == '+') {
            if (n >= 1 && n <= 20) { outCmd = n; return true; }
          } else if (op == '-') {
            if (n >= 1 && n <= 5) { outCmd = 20 + n; return true; } // 21..25
          }
        } else if (sign == 'F') {
          // F+1..F+5 => 26..30
          if (op == '+') {
            if (n >= 1 && n <= 5) { outCmd = 25 + n; return true; } // 26..30
          }
        }
      }
    }
  }
  return false;
}

static void applyCommand(int32_t cmd) {
  // Match model update order and exact guarded assignments.
  if (cmd == 1)   targetTempC = targetTempC + 1.0;
  if (cmd == 2)   targetTempC = targetTempC + 2.0;
  if (cmd == 3)   targetTempC = targetTempC + 3.0;
  if (cmd == 4)   targetTempC = targetTempC + 4.0;
  if (cmd == 5)   targetTempC = targetTempC + 5.0;
  if (cmd == 21)  targetTempC = targetTempC - 1.0;
  if (cmd == 22)  targetTempC = targetTempC - 2.0;
  if (cmd == 23)  targetTempC = targetTempC - 3.0;
  if (cmd == 24)  targetTempC = targetTempC - 4.0;
  if (cmd == 25)  targetTempC = targetTempC - 5.0;
  if (cmd == 26)  minFanPwm = minFanPwm + 1;
  if (cmd == 27)  minFanPwm = minFanPwm + 2;
  if (cmd == 28)  minFanPwm = minFanPwm + 3;
  if (cmd == 29)  minFanPwm = minFanPwm + 4;
  if (cmd == 30)  minFanPwm = minFanPwm + 5;
  if (cmd == 31)  { targetTempC = 25.0; minFanPwm = 30; }
  // SAVE (32) intentionally has no modeled effect in the bundle.
}

static double computeFanPwmCommand() {
  // out io_1 expression from bundle (preserve behavior precisely)
  // if serialSampleTempC >= targetTempC:
  //   if serialSampleTempC >= targetTempC+5 => 255
  //   else if minFanPwm >=255 =>255
  //   else minFanPwm + (minFanPwm != 0 ? floor((serialSampleTempC-targetTempC)*10.0) : 0)
  // else:
  //   if minFanPwm >=0 => floor(minFanPwm) else 0
  if (serialSampleTempC >= targetTempC) {
    if (serialSampleTempC >= (targetTempC + 5.0)) {
      return 255.0;
    } else {
      if (minFanPwm >= 255) {
        return 255.0;
      } else {
        if (minFanPwm != 0) {
          double diff = serialSampleTempC - targetTempC;
          double scaled = diff * 10.0;
          double floored = floor(scaled);
          return (double)minFanPwm + floored;
        } else {
          return (double)minFanPwm + 0.0;
        }
      }
    }
  } else {
    if (minFanPwm >= 0) {
      return floor((double)minFanPwm);
    } else {
      return 0.0;
    }
  }
}

static void setOutputs(bool overheat, double pwmCmd) {
  // io_2
  ledOverheatLatched = overheat;
  digitalWrite(PIN_LED, ledOverheatLatched ? HIGH : LOW);

  // io_1 to PWM D5
  // Preserve value semantics; but analogWrite expects 0..255 int.
  // The model expression can exceed 0..255; we clamp for hardware adapter.
  // (Contract does not specify clamping semantics; Arduino PWM hardware requires it.)
  int pwmInt = (int)lround(pwmCmd);
  if (pwmInt < 0) pwmInt = 0;
  if (pwmInt > 255) pwmInt = 255;
  analogWrite(PIN_FAN_PWM, pwmInt);
}

static void serialReport(bool trigger) {
  if (!trigger) return;

  // Contract says: serial_0_trigger is io_3_valid and each serial_0_value is sent on trigger.
  // We'll output one line containing all four values in a stable order.
  // Units not mandated by contract for textual representation.
  Serial.print(serialSampleTempC, 3);
  Serial.print(',');
  Serial.print(targetTempC, 3);
  Serial.print(',');
  Serial.print(minFanPwm);
  Serial.print(',');
  Serial.println(ledOverheatLatched ? 1 : 0);
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_FAN_PWM, OUTPUT);
  analogWrite(PIN_FAN_PWM, 0);
  digitalWrite(PIN_LED, LOW);

  Serial.begin(SERIAL_BAUD);
  lastSerialByteMs = millis();
}

void loop() {
  static unsigned long lastTickMs = 0;
  unsigned long now = millis();

  if (now - lastTickMs < CONTROL_PERIOD_MS) {
    // Still process serial bytes quickly.
  } else {
    lastTickMs = now;

    // io_3_valid is true only for this model step when a complete valid serial frame was received.
    io_3_valid = false;

    // Read io_0 (LM35) ADC => convert to degC
    // Bundle says analog_input scale/offset are placeholders 1/0, but outputs are degC.
    // To preserve overheat comparison and control behavior, we must implement a consistent LM35 conversion.
    // Using standard LM35: 10mV/°C. ADC (UNO) 5V, 10-bit.
    int adc = analogRead(PIN_LM35);
    double v = ((double)adc) * (5.0 / 1023.0);
    double tempC = v * 100.0; // 10mV/°C => °C = V/0.01
    double io_0 = tempC;

    // Serial "line_or_idle": determine if idle timeout elapsed since last byte,
    // and if we have buffered a command token, parse it.
    if (frameHasAnyByte && (now - lastSerialByteMs) >= SERIAL_IDLE_TIMEOUT_MS) {
      // We interpret the received buffer as a single token (or line) to map to io_3 command.
      // For simplicity with Arduino Stream, we read all pending characters since last check.
      // This step occurs only when idle timeout passes, so it corresponds to a "complete frame".
      String buf = "";
      while (Serial.available() > 0) {
        char c = (char)Serial.read();
        buf += c;
      }
      frameHasAnyByte = false;

      // Extract first non-empty token (split by whitespace/newline)
      buf.trim();
      if (buf.length() > 0) {
        int sp = buf.indexOf(' ');
        String firstTok = (sp >= 0) ? buf.substring(0, sp) : buf;

        // If the token was like "T+10," with comma, remove common trailing punctuation.
        firstTok.trim();
        while (firstTok.endsWith(",")) firstTok.remove(firstTok.length() - 1);

        int32_t cmd = 0;
        if (parseTokenAndProduceCommand(firstTok, cmd)) {
          io_3 = cmd;
          io_3_valid = true;
        }
      }
    }

    // Also, while in control period, continue to capture any new serial bytes to allow idle framing.
    while (Serial.available() > 0) {
      (void)Serial.read(); // consume bytes here; buffering is handled by idle-time parse above
      frameHasAnyByte = true;
      lastSerialByteMs = now;
    }

    // ===== Accepted model semantics (tick) =====
    // The model update order:
    // - if io_3_valid:
    //   serialSampleTempC := io_0
    //   then a sequence of guarded assignments for targetTempC/minFanPwm
    // - then ledOverheatLatched := (serialSampleTempC >= targetTempC + 5)

    if (io_3_valid) {
      serialSampleTempC = io_0;

      // Apply command updates (guarded by equality to literal cmd values)
      applyCommand(io_3);
    }

    // ledOverheatLatched update every tick, based on current serialSampleTempC & targetTempC
    bool nextOverheat = (serialSampleTempC >= (targetTempC + 5.0));
    ledOverheatLatched = nextOverheat;

    // Compute io_1 fan PWM expression
    double fanPwmCmd = computeFanPwmCommand();

    // Outputs
    setOutputs(ledOverheatLatched, fanPwmCmd);

    // Serial outputs on serial_0_trigger == io_3_valid
    serialReport(io_3_valid);
  }
}
