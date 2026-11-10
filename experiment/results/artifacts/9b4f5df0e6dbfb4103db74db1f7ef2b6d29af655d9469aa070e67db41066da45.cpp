#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t TRIG_PIN = 8; // D8
static const uint8_t ECHO_PIN = 9; // D9

// HC-SR04 timing
static const unsigned long PULSE_TIMEOUT_US = 30000UL; // ~5m max (conservative)
static const float SPEED_OF_SOUND_CM_PER_US = 0.0343f;   // cm/us (20-25C approx)

// Serial settings
static const unsigned long IDLE_TIMEOUT_MS = 20;          // provided requirement (not strictly required for Arduino parsing)
static const uint32_t BAUD = 9600;

static unsigned long lastRxMs = 0;

#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readLineCommand(String &outCmd);
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float measureDistanceCm();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readLineCommand(String &outCmd) {
  // Reads a command consisting of consecutive characters with line_or_idle behavior.
  // - If newline is received, return immediately.
  // - Otherwise if idle timeout elapses, treat the accumulated buffer as a complete command.
  static String buf;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastRxMs = millis();

    if (c == '\n' || c == '\r') {
      if (buf.length() > 0) {
        outCmd = buf;
        buf = "";
        return true;
      }
      // ignore empty lines
    } else {
      // accumulate
      buf += c;
    }
  }

  // idle timeout handling
  if (buf.length() > 0) {
    unsigned long now = millis();
    if (now - lastRxMs >= IDLE_TIMEOUT_MS) {
      outCmd = buf;
      buf = "";
      return true;
    }
  }

  return false;
}

static float measureDistanceCm() {
  // Trigger: 10us HIGH pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width
  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, PULSE_TIMEOUT_US);
  if (durationUs == 0) {
    // Timeout/no echo; return an impossible/invalid value (consistent behavior for READ)
    return -1.0f;
  }

  // distance = (time * speed_of_sound) / 2
  float distanceCm = (durationUs * SPEED_OF_SOUND_CM_PER_US) / 2.0f;
  return distanceCm;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  Serial.begin(BAUD);
  // No extra framing options available on UNO; "line_or_idle" and idle timeout handled in software.
  lastRxMs = millis();
}

void loop() {
  String cmd;
  if (readLineCommand(cmd)) {
    cmd.trim();

    if (cmd.equalsIgnoreCase("READ")) {
      float d = measureDistanceCm();
      // Output HC-SR04 distance as line
      // Use fixed formatting to keep output predictable.
      if (d < 0.0f) {
        Serial.println("ERR");
      } else {
        Serial.print(d, 2);
        Serial.println(" cm");
      }
    }
    // Ignore unknown commands
  }
}
