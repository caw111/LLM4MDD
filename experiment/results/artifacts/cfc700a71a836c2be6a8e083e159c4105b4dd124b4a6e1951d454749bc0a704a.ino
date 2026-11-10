#include <TM1637Display.h>

// ---------------- Pin assignments (UNO) ----------------
// Note: Arduino core does not define symbolic constants like D13/D10/etc.
// Use numeric pins per resolved wiring: D2,D4,D5,D6,D10,D13.
static const uint8_t LM35_PIN     = A1; // A1: LM35 analog input
static const uint8_t LED_PIN      = 13; // D13: status LED
static const uint8_t BUZZER_PIN   = 6;  // D6: buzzer (tone output)
static const uint8_t RELAY_PIN     = 10; // D10: fan/cooling relay control
static const uint8_t DOOR_MAG_PIN = 2;  // D2: door magnetic switch input
static const uint8_t TM1637_CLK    = 4;  // D4: TM1637 CLK
static const uint8_t TM1637_DIO    = 5;  // D5: TM1637 DIO

// ---------------- TM1637 ----------------
TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

// ---------------- Temperature conversion ----------------
// LM35: 10mV/°C, on UNO ADC (5V), voltage = adc * (5/1023), tempC = voltage / 0.01
static const float LM35_MV_PER_C = 10.0f;
static const float VREF = 5.0f;
static const float ADC_MAX = 1023.0f;

// ---------------- Thresholds / timers (kept explicit defaults) ----------------
static const float TEMP_UPPER_LIMIT_C = 8.0f;   // "上限" (default)
static const uint32_t DOOR_OPEN_ALARM_MS = 10000; // 连续打开超过 10 秒

// Door input logic: assume pull-up with switch to GND when door opens (common wiring).
// If your wiring is opposite, invert this boolean.
static const bool DOOR_OPEN_LEVEL_IS_HIGH = true;

// Serial (baud 9600; line_or_idle; idle_timeout_ms handled by PrintLine semantics)
static const uint32_t SERIAL_UPDATE_PERIOD_MS = 200; // avoid too fast while "持续输出"
uint32_t lastSerialPrintMs = 0;

// Door timing
bool doorWasOpen = false;
uint32_t doorOpenStartMs = 0;
bool alarmActive = false;

// Alarm blinking/tone
uint32_t lastBlinkMs = 0;
bool ledBlinkState = false;
uint32_t lastToneMs = 0;

// Display alternate
static uint32_t lastAltMs = 0;
static bool showTemp = true;
const uint32_t ALT_PERIOD_MS = 500;

// ---------------- Helpers ----------------
static void setCoolingRelay(bool on) {
  digitalWrite(RELAY_PIN, on ? HIGH : LOW);
}

static float readTemperatureC() {
  int adc = analogRead(LM35_PIN);
  float voltage = (adc * VREF) / ADC_MAX;           // volts
  float tempC = (voltage * 1000.0f) / LM35_MV_PER_C; // (V->mV)/mV-per-C
  return tempC;
}

static int32_t clampInt32(int32_t v, int32_t lo, int32_t hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

static void displayTemperature(float tempC) {
  // Format: show x.x using scaling by 10 and decimal point via dots mask.
  // TM1637Display showNumberDecEx(val, dots, leading_zero)
  // We'll attempt: decimal point on second from right using dots=0x40 (commonly used mapping).
  int32_t t10 = (int32_t)lroundf(tempC * 10.0f); // e.g., 23.4C => 234
  t10 = clampInt32(t10, -999, 9999);

  bool negative = (t10 < 0);
  int32_t abs_t10 = negative ? -t10 : t10;

  int16_t val = (int16_t)(abs_t10 % 10000);
  uint8_t dots = 0x40;

  tm1637.showNumberDecEx(val, dots, true);
}

static void displayDoorOpenSeconds(uint32_t secondsOpen) {
  uint32_t s = secondsOpen;
  if (s > 9999) s = 9999;
  tm1637.showNumberDec((int)s, true);
}

// ---------------- Arduino setup/loop ----------------
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(DOOR_MAG_PIN, INPUT_PULLUP); // default pull-up; invert logic with DOOR_OPEN_LEVEL_IS_HIGH if needed

  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
  setCoolingRelay(false);

  tm1637.setBrightness(0x0f);
  tm1637.clear();

  Serial.begin(9600);
}

void loop() {
  // --- Read door sensor ---
  int doorRaw = digitalRead(DOOR_MAG_PIN);
  bool doorOpen = DOOR_OPEN_LEVEL_IS_HIGH ? (doorRaw == HIGH) : (doorRaw == LOW);

  uint32_t now = millis();

  // --- Update door open timing state machine ---
  if (doorOpen) {
    if (!doorWasOpen) {
      doorOpenStartMs = now;
      doorWasOpen = true;
    }
  } else {
    doorWasOpen = false;
    doorOpenStartMs = 0;
    alarmActive = false;
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);
  }

  uint32_t doorOpenElapsedMs = 0;
  uint32_t doorOpenElapsedSec = 0;
  if (doorOpen && doorOpenStartMs != 0) {
    doorOpenElapsedMs = now - doorOpenStartMs;
    doorOpenElapsedSec = doorOpenElapsedMs / 1000UL;
  }

  // --- Determine alarm ---
  if (doorOpen && doorOpenElapsedMs > DOOR_OPEN_ALARM_MS) {
    alarmActive = true;
  }

  // --- Temperature control relay ---
  float tempC = readTemperatureC();

  // Behavior:
  // - "系统根据温度控制制冷继电器启停"
  //   on if temp > upper limit, off otherwise (no hysteresis specified).
  // - If temperature high AND door not closed => serial critical cooling loss
  bool tempHigh = (tempC > TEMP_UPPER_LIMIT_C);
  bool doorNotClosed = doorOpen; // "门未关闭" means door open

  if (tempHigh) {
    setCoolingRelay(true);
  } else {
    setCoolingRelay(false);
  }

  // --- Alarm actions when door open too long ---
  if (alarmActive) {
    // Flash LED at ~2 Hz; beep continuously while alarm active
    if (now - lastBlinkMs >= 250) { // 250ms toggle => 2Hz
      lastBlinkMs = now;
      ledBlinkState = !ledBlinkState;
      digitalWrite(LED_PIN, ledBlinkState ? HIGH : LOW);
    }

    // Buzzer refresh without spamming too frequently.
    if (now - lastToneMs >= 20) { // refresh tone
      lastToneMs = now;
      tone(BUZZER_PIN, 2000); // 2kHz alert
    }
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);
  }

  // --- Serial "critical cooling loss" continuous when conditions met ---
  if (tempHigh && doorNotClosed) {
    if (now - lastSerialPrintMs >= SERIAL_UPDATE_PERIOD_MS) {
      lastSerialPrintMs = now;
      Serial.println("critical cooling loss");
    }
  }

  // --- TM1637 display ---
  // Requirements: "TM1637 显示当前温度和门打开时长。"
  // We'll display temperature when door is closed; when door is open,
  // alternate between temperature and door open time.
  if (!doorOpen) {
    showTemp = true;
    displayTemperature(tempC);
  } else {
    if (now - lastAltMs >= ALT_PERIOD_MS) {
      lastAltMs = now;
      showTemp = !showTemp;
    }
    if (showTemp) {
      displayTemperature(tempC);
    } else {
      displayDoorOpenSeconds(doorOpenElapsedSec);
    }
  }
}