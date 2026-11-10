#include <Arduino.h>
#include <TM1637Display.h>

//// ---------------- Pin mapping (authoritative) ---------------- ////
// PIR: D2
static const uint8_t PIN_PIR = 2;
// Door magnetic switch: D3
static const uint8_t PIN_DOOR = 3;
// LDR analog: A0
static const uint8_t PIN_LDR = A0;
// Buzzer: D6
static const uint8_t PIN_BUZZER = 6;
// Alarm LED: D13
static const uint8_t PIN_ALARM_LED = 13;
// TM1637: CLK=D4, DIO=D5
static const uint8_t PIN_TM1637_CLK = 4;
static const uint8_t PIN_TM1637_DIO = 5;

static TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

//// ---------------- Serial protocol ---------------- ////
// Commands (line_or_idle framing). Accept line with optional whitespace.
// Also accept single token if idle occurs; simplest: read until newline or timeout.
static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;

// Command keywords
static const char* CMD_ARM    = "ARM";
static const char* CMD_DISARM  = "DISARM";
static const char* CMD_STATUS  = "STATUS";
static const char* CMD_RESET   = "RESET";

//// ---------------- System states / modes ---------------- ////
enum SystemState : uint8_t {
  STATE_DISARMED = 0,
  STATE_ARMED    = 1,
  STATE_ALARMED  = 2, // alarm output active (can be cleared by DISARM/RESET)
};

enum Mode : uint8_t {
  MODE_MANUAL = 0, // ARM/DISARM by commands
  MODE_NIGHT  = 1  // night auto-arm based on LDR threshold
};

static volatile SystemState g_state = STATE_DISARMED;
static volatile Mode g_mode = MODE_MANUAL;

// Night threshold (enter when light low below threshold)
static const uint16_t LDR_THRESHOLD = 400;

// Event counting
static uint32_t g_eventCount = 0;

// Alarm behavior timing
static const uint32_t BUZZER_ON_MS = 600;
static const uint32_t BUZZER_OFF_MS = 400;
static const uint8_t  BUZZER_REPEAT = 3; // total bursts per alarm trigger
static uint8_t g_buzzerBurstIdx = 0;
static uint32_t g_buzzerPhaseT0 = 0;
static bool g_buzzerActive = false;

// PIR trigger edge/rate control
static const uint32_t PIR_COOLDOWN_MS = 2000;
static uint32_t g_lastPirTriggerMs = 0;

// Door open latch control
static bool g_doorOpenLatched = false;
static uint32_t g_lastDoorStateChangeMs = 0;

// Alarm trigger lock to avoid repeated triggers for same door-open session
static bool g_alarmTriggeredForCurrentDoorOpen = false;

// Display
static const uint8_t TM1637_BRIGHTNESS = 7;

static void displayNumber(uint32_t value) {
  // TM1637Display supports up to 4 digits.
  // Show last 4 digits; in case count exceeds, display modulo.
  uint16_t shown = (uint16_t)(value % 10000UL);
  tm1637.showNumberDec(shown, false, 4, 0);
}

static void displayModeAndState() {
  // Simple encoding on 4 digits:
  // DISARMED: 0000
  // ARMED:    0001
  // ALARMED:  0002
  // Night mode indicates by adding +2000 (e.g., night-armed display 2001)
  uint16_t base = 0;
  switch (g_state) {
    case STATE_DISARMED: base = 0; break;
    case STATE_ARMED:    base = 1; break;
    case STATE_ALARMED:  base = 2; break;
  }
  uint16_t nightAdd = (g_mode == MODE_NIGHT) ? 2000 : 0;
  uint16_t shown = base + nightAdd;
  tm1637.showNumberDec(shown, false, 4, 0);
}

static void beepStartSequence() {
  g_state = STATE_ALARMED;
  g_buzzerActive = true;
  g_buzzerBurstIdx = 0;
  g_buzzerPhaseT0 = millis();

  // Start immediately with ON phase
  tone(PIN_BUZZER, 2000); // buzzer tone
  digitalWrite(PIN_ALARM_LED, HIGH);
}

static void beepUpdateSequence() {
  if (!g_buzzerActive) return;

  uint32_t now = millis();
  uint32_t elapsed = now - g_buzzerPhaseT0;

  static bool s_toneOn = true;

  if (s_toneOn) {
    if (elapsed >= BUZZER_ON_MS) {
      noTone(PIN_BUZZER);
      s_toneOn = false;
      g_buzzerPhaseT0 = now;
    }
  } else {
    if (elapsed >= BUZZER_OFF_MS) {
      g_buzzerBurstIdx++;
      if (g_buzzerBurstIdx >= BUZZER_REPEAT) {
        // Finish alarm pattern but keep state as ALARMED until DISARM/RESET.
        noTone(PIN_BUZZER);
        g_buzzerActive = false;
        s_toneOn = true;
        digitalWrite(PIN_ALARM_LED, HIGH); // keep alarm LED on
        // Keep display as event count.
        displayNumber(g_eventCount);
        return;
      } else {
        tone(PIN_BUZZER, 2000);
        s_toneOn = true;
        g_buzzerPhaseT0 = now;
      }
    }
  }
}

static void stopAlarmOutputsIfNeeded() {
  // Called on DISARM/RESET.
  g_buzzerActive = false;
  noTone(PIN_BUZZER);
  digitalWrite(PIN_ALARM_LED, LOW);
  displayModeAndState();
}

static void triggerAlarmEvent() {
  // Trigger alarm: increment event count, sound, LED on, show event count.
  g_eventCount++;
  displayNumber(g_eventCount);
  beepStartSequence();
  // Door/session lock:
  g_alarmTriggeredForCurrentDoorOpen = true;
}

//// ---------------- Serial command parsing ---------------- ////
static String readSerialTokenWithIdle() {
  static String buf;
  static uint32_t lastCharMs = 0;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\r' || c == '\n') {
      String token = buf;
      buf = "";
      return token;
    }
    lastCharMs = millis();

    if (c == ' ' || c == '\t') {
      buf += ' ';
    } else {
      buf += c;
    }
  }

  // Handle idle timeout: if buffer has something and no new char within idle timeout, finalize token.
  if (buf.length() > 0) {
    uint32_t now = millis();
    if (now - lastCharMs >= SERIAL_IDLE_TIMEOUT_MS) {
      String token = buf;
      buf = "";
      return token;
    }
  }
  return String();
}

static String normalizeToken(String s) {
  s.trim();
  s.toUpperCase();

  // Remove internal spaces
  String out;
  out.reserve(s.length());
  for (size_t i = 0; i < s.length(); i++) {
    char c = s[i];
    if (c >= 'A' && c <= 'Z') out += c;
  }
  return out;
}

static void sendStatus() {
  Serial.print("STATE=");
  if (g_state == STATE_DISARMED) Serial.print("DISARMED");
  else if (g_state == STATE_ARMED) Serial.print("ARMED");
  else Serial.print("ALARMED");

  Serial.print(",MODE=");
  Serial.print((g_mode == MODE_NIGHT) ? "NIGHT" : "MANUAL");

  Serial.print(",EVENTS=");
  Serial.println(g_eventCount);
}

static void processCommand(const String& token) {
  String t = normalizeToken(token);
  if (t.length() == 0) return;

  if (t == CMD_ARM) {
    // ARM command forces manual mode armed.
    g_mode = MODE_MANUAL;
    g_state = STATE_ARMED;

    // Cancel alarm outputs if any.
    stopAlarmOutputsIfNeeded();
    g_alarmTriggeredForCurrentDoorOpen = false;
    g_doorOpenLatched = false;

    displayModeAndState();
    Serial.println("OK");
    return;
  }

  if (t == CMD_DISARM) {
    // DISARM stops alarms; manual mode disarmed.
    g_mode = MODE_MANUAL;
    g_state = STATE_DISARMED;

    stopAlarmOutputsIfNeeded();
    g_alarmTriggeredForCurrentDoorOpen = false;
    g_doorOpenLatched = false;

    displayModeAndState();
    Serial.println("OK");
    return;
  }

  if (t == CMD_RESET) {
    // RESET clears event count and returns to DISARMED.
    g_eventCount = 0;
    g_mode = MODE_MANUAL;
    g_state = STATE_DISARMED;

    stopAlarmOutputsIfNeeded();
    g_alarmTriggeredForCurrentDoorOpen = false;
    g_doorOpenLatched = false;

    displayModeAndState();
    Serial.println("OK");
    return;
  }

  if (t == CMD_STATUS) {
    sendStatus();
    return;
  }

  Serial.println("ERR");
}

//// ---------------- Sensors helpers ---------------- ////
static bool readPIR() {
  return digitalRead(PIN_PIR) == HIGH;
}

static bool readDoorOpen() {
  // Assume HIGH indicates open.
  return digitalRead(PIN_DOOR) == HIGH;
}

static bool isNightLightLow() {
  int v = analogRead(PIN_LDR);
  return (uint16_t)v < LDR_THRESHOLD;
}

//// ---------------- Setup / Loop ---------------- ////
void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_DOOR, INPUT);

  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_ALARM_LED, OUTPUT);

  digitalWrite(PIN_ALARM_LED, LOW);
  noTone(PIN_BUZZER);

  tm1637.setBrightness(TM1637_BRIGHTNESS);
  displayModeAndState();
}

void loop() {
  // Serial command handling
  String tok = readSerialTokenWithIdle();
  if (tok.length() > 0) {
    processCommand(tok);
  }

  // Alarm buzzer sequence maintenance
  beepUpdateSequence();

  // Night auto-arm logic (enter night mode when light low)
  // Also keep manual mode otherwise; do not override DISARM explicitly beyond mode logic.
  if (g_mode == MODE_NIGHT) {
    if (!isNightLightLow()) {
      // Light is high: go to manual disarmed? Keep behavior consistent with "夜间自动布防" only:
      // if light rises, leave night mode to manual and disarm.
      g_mode = MODE_MANUAL;
      g_state = STATE_DISARMED;
      stopAlarmOutputsIfNeeded();
      g_alarmTriggeredForCurrentDoorOpen = false;
      g_doorOpenLatched = false;
      displayModeAndState();
    }
  } else {
    if (isNightLightLow()) {
      // Enter night mode and arm automatically
      g_mode = MODE_NIGHT;
      g_state = STATE_ARMED;
      stopAlarmOutputsIfNeeded(); // ensures outputs are cleared before arming
      g_alarmTriggeredForCurrentDoorOpen = false;
      g_doorOpenLatched = false;
      displayModeAndState();
    }
  }

  // In ARMED state, monitor door+PIR for alarm trigger
  bool doorOpen = readDoorOpen();

  // Door open latch/session handling to avoid repeated triggers during same door-open period.
  // Latch becomes true when door opens; reset when door closes.
  if (doorOpen && !g_doorOpenLatched) {
    g_doorOpenLatched = true;
    g_lastDoorStateChangeMs = millis();
    g_alarmTriggeredForCurrentDoorOpen = false; // new door-open session
  } else if (!doorOpen && g_doorOpenLatched) {
    g_doorOpenLatched = false;
    g_lastDoorStateChangeMs = millis();
    g_alarmTriggeredForCurrentDoorOpen = false;
  }

  if (g_state == STATE_ARMED) {
    // Door must be open AND PIR must detect motion (with cooldown)
    if (g_doorOpenLatched && !g_alarmTriggeredForCurrentDoorOpen) {
      uint32_t now = millis();
      if (readPIR()) {
        if (now - g_lastPirTriggerMs >= PIR_COOLDOWN_MS) {
          g_lastPirTriggerMs = now;
          triggerAlarmEvent();
        }
      }
    }
  }

  // If alarmed, no further triggers until DISARM/RESET (handled by commands).
}