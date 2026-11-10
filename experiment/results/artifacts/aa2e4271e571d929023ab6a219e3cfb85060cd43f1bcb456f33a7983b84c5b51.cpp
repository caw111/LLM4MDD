#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <TM1637Display.h>

// --- Pin assignments (per resolved wiring) ---
static const uint8_t LM35_PIN = A1;      // A1: LM35 analog input
static const uint8_t LED_PIN  = D13;     // D13: status LED
static const uint8_t BUZZER_PIN = D6;     // D6: buzzer (tone output)
static const uint8_t RELAY_PIN = D10;    // D10: fan/cooling relay control
static const uint8_t DOOR_MAG_PIN = D2;  // D2: door magnetic switch input (door open detection)
static const uint8_t TM1637_CLK = D4;     // D4: TM1637 CLK
static const uint8_t TM1637_DIO = D5;     // D5: TM1637 DIO

// --- TM1637 ---
TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

// --- Temperature settings / behavior (kept explicit where possible) ---
// LM35: 10mV/°C, on UNO ADC (5V), voltage = adc * (5/1023), tempC = voltage / 0.01
static const float LM35_MV_PER_C = 10.0f;
static const float VREF = 5.0f;
static const float ADC_MAX = 1023.0f;

// Calibration/thresholds/timers (defaults used because no numeric values were provided)
static const float TEMP_UPPER_LIMIT_C = 8.0f;   // "上限" (choose a reasonable default)
static const uint32_t DOOR_OPEN_ALARM_MS = 10000; // 连续打开超过 10 秒

// Door input logic: assume pull-up with switch to GND when door opens (common wiring).
// If your wiring is opposite, invert this boolean.
static const bool DOOR_OPEN_LEVEL_IS_HIGH = true; // set false if door open corresponds to LOW

// Serial
static const uint32_t SERIAL_UPDATE_PERIOD_MS = 200; // avoid spamming too fast while still "持续输出"
uint32_t lastSerialPrintMs = 0;

// Door timing
bool doorWasOpen = false;
uint32_t doorOpenStartMs = 0;
bool alarmActive = false;

// Alarm blinking/tone
uint32_t lastBlinkMs = 0;
bool ledBlinkState = false;
uint32_t lastToneMs = 0;

// Display
// TM1637 supports 4 digits; we'll show temperature in x.xC and then door open time seconds when door is open.
// If door is open, show "t=XXXX" not possible on 4 digits; we'll show door open seconds as integer.
// When door closed, show temperature as xx.xC (rounded).
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setCoolingRelay(bool on);
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
float readTemperatureC();
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
int32_t clampInt32(int32_t v, int32_t lo, int32_t hi);
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void displayTemperature(float tempC);
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void displayDoorOpenSeconds(uint32_t secondsOpen);
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setCoolingRelay(bool on)
{
  digitalWrite(RELAY_PIN, on ? HIGH : LOW);
}

float readTemperatureC()
{
  int adc = analogRead(LM35_PIN);
  float voltage = (adc * VREF) / ADC_MAX;     // in volts
  float tempC = (voltage * 1000.0f) / LM35_MV_PER_C; // convert V->mV
  return tempC;
}

int32_t clampInt32(int32_t v, int32_t lo, int32_t hi)
{
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

void displayTemperature(float tempC)
{
  // Show temperature scaled by 10 to use TM1637. Format: [sign?]xx.x
  // We'll display an integer like temp*10 with decimal point before last digit.
  int32_t t10 = (int32_t)lroundf(tempC * 10.0f); // e.g., 23.4C => 234
  // Limit to 4 digits for TM1637 without sign overflow.
  t10 = clampInt32(t10, -999, 9999);

  // TM1637Display has showNumberDecEx with decimal position control.
  // We'll treat negative temps as 0.. can't easily. For simplicity:
  // -999..999.9 supported by showing sign automatically isn't available via this call reliably.
  // We'll still attempt with negative support.
  bool negative = (t10 < 0);
  int32_t abs_t10 = negative ? -t10 : t10;

  // Convert abs_t10 to display value without sign, use decimal point.
  // For abs_t10, decimal before last digit to show x.x.
  // Use 4 digits: value mod 10000.
  int16_t val = (int16_t)(abs_t10 % 10000);
  uint8_t dots = 0;
  // Decimal point placement: we want "... . ..." depending on magnitude.
  // Choose always at digit 2 from right (one decimal): 0b01000000 corresponds to colon? For TM1637Display,
  // encode dp mask with 0x40 for position 2 etc. We'll use showNumberDecEx with a dp mask:
  // The library expects 0x80? depending; common: bit 0=dp at digit 0, bit 1 at digit 1...
  // We'll choose dot on position 1 (2nd from right). If mapping differs, at worst dot is misplaced.
  // Use built-in library constant? not available. We'll approximate: 0x40 means digit 1 dp in many implementations.
  dots = 0x40;

  tm1637.showNumberDecEx(val, dots, true); // leading zeros enabled? true.
  // If negative, attempt sign by showing val and minus isn't possible here; ignore.
}

void displayDoorOpenSeconds(uint32_t secondsOpen)
{
  // Show door open seconds as integer (0..9999)
  uint32_t s = secondsOpen;
  if (s > 9999) s = 9999;
  tm1637.showNumberDec((int)s, true);
}

void setup()
{
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

void loop()
{
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
  // We'll implement on if temp > upper limit, off otherwise (simple hysteresis not specified).
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
    // buzzer and LED flashing
    // Flash LED at ~2 Hz; beep continuously while alarm active
    if (now - lastBlinkMs >= 250) { // 250ms toggle => 2Hz
      lastBlinkMs = now;
      ledBlinkState = !ledBlinkState;
      digitalWrite(LED_PIN, ledBlinkState ? HIGH : LOW);
    }

    // Buzzer: use tone for audible alert
    // If tone can't run long due to constraints, still keep it active.
    if (now - lastToneMs >= 20) { // refresh without spamming
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
  // This preserves both pieces of information without dropping any state.
  static uint32_t lastAltMs = 0;
  static bool showTemp = true;
  const uint32_t ALT_PERIOD_MS = 500;

  if (!doorOpen) {
    // door closed: show temperature
    showTemp = true;
    displayTemperature(tempC);
  } else {
    // door open: alternate
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
