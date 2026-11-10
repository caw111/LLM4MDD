#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

#define LM35_PIN A1
#define DOOR_MAGNET_PIN 2      // D2
#define RELAY_FAN_PIN 10        // D10
#define BUZZER_PIN 6           // D6
#define LED_PIN 13            // D13

// TM1637 CLK=D4, DIO=D5
#define TM1637_CLK_PIN 4
#define TM1637_DIO_PIN 5

// ---- Behavior constants (preserved as required) ----
static const float TEMP_C_UPPER = 8.0f;     // upper limit for critical cooling loss
static const float TEMP_C_LOWER = 4.0f;     // lower limit for hysteresis
static const uint32_t DOOR_ALARM_MS = 10000; // door continuous open > 10 seconds
static const uint16_t ALARM_BLINK_MS = 500;  // LED blink interval during alarm

// LM35: 10mV/°C, 5V reference, ADC resolution 1024
static const float ADC_VREF = 5.0f;
static const int ADC_MAX = 1023;
static const float LM35_MV_PER_C = 10.0f;

TM1637Display tm1637(TM1637_CLK_PIN, TM1637_DIO_PIN);

// Door tracking
static bool doorWasOpen = false;
static bool alarmActive = false;
static uint32_t doorOpenStartMs = 0;
static uint32_t alarmBlinkPrevMs = 0;
static bool alarmLedState = false;
static bool relayOn = false;

// Utility: read temperature from LM35
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayTemperatureAndDoorTime(float tempC, bool doorOpen, uint32_t doorOpenMs);
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 138 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_007\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC()
{
  int adc = analogRead(LM35_PIN);
  float v = (adc * ADC_VREF) / (float)ADC_MAX;      // volts
  float tempC = (v * 1000.0f) / LM35_MV_PER_C;     // °C
  return tempC;
}

// Display temperature (one decimal) and door time status
// Requirement: TM1637 display current temperature and door opened duration when door is open
static void displayTemperatureAndDoorTime(float tempC, bool doorOpen, uint32_t doorOpenMs)
{
  // TM1637 typical: brightness 0..7
  tm1637.setBrightness(4);

  if (!doorOpen) {
    // Show temperature only: 4-digit; use 1 decimal if possible
    // Format: e.g., -3.2 => "-3.2", 25.6 => "25.6"
    // TM1637Display::showNumberDecEx supports decimal point control via dots
    int intTemp = (int)(tempC * 10.0f); // tenths
    // Clamp to displayable range (-99.9 .. 199.9 depending on settings). We'll clamp to ±99.9 for safety.
    if (intTemp > 999) intTemp = 999;
    if (intTemp < -999) intTemp = -999;

    // showNumberDecEx expects value without decimal; decimal handled by dots
    // We'll use showNumberDecEx with 2 decimals pattern disabled; easiest: use number with one decimal using dots.
    int absVal = abs(intTemp);
    int tens = absVal / 10;
    int tenths = absVal % 10;

    // Build a value scaled by 10 to represent with dot at tenths place:
    // We'll compute digits ourselves by using showNumberDecEx:
    // Place tenths digit at least significant, dot at second least significant.
    // For simplicity, use showNumberDecEx on intTemp with dots enabled.
    // dots: bitmask for digits [0..3] - in library, decimal points use 0x80.. depending.
    // TM1637Display: 0x40 controls decimal point on first digit? Common approach: use 0x40 for 1 dot.
    // We'll rely on library's showNumberDecEx formatting by using four digits and enabling dot at digit 2.
    // Use 0x40 for decimal point on digit 2 (commonly LSB is digit 0).
    // If your library version maps dots differently, visual may shift; behavior otherwise remains.
    tm1637.showNumberDecEx(intTemp, (0b01000000), true); // dot at tenths position
  } else {
    // Door is open: show door open time (seconds) and temperature simultaneously
    // We'll show temperature with one decimal in left part and door seconds in right part.
    // 4-digit display: [T].[t] is hard; we will show door time in seconds (0-99) and temperature tenths sign not possible.
    // But requirement says display current temperature and door opened duration when door is open.
    // Approach: show temperature on first two digits and door duration seconds (0-99) on last two digits.
    // Example: temp=5.6C => "5.6" and door time=12s => "12" => displayed as "5 6 1 2" with one dot between first and second digit.
    // We'll map: digit0 digit1 for temperature (with dot), digit2 digit3 for door seconds.
    int tempTenth = (int)(tempC * 10.0f);
    bool neg = tempTenth < 0;
    int tempAbs = abs(tempTenth);
    int tempInt = tempAbs / 10;
    int tempFrac = tempAbs % 10;

    if (tempInt > 9) tempInt = 9; // clamp to fit two digits (single-digit integer part)
    uint32_t doorSec = doorOpenMs / 1000;
    if (doorSec > 99) doorSec = 99;

    uint8_t d0 = (uint8_t)(neg ? 0x0A : (tempInt % 10)); // 0x0A special? library may not support; use minus sign separately if needed
    uint8_t d1 = (uint8_t)tempFrac;
    uint8_t d2 = (uint8_t)(doorSec / 10);
    uint8_t d3 = (uint8_t)(doorSec % 10);

    // Render via showSegments if possible; however library compatibility varies.
    // We'll use showNumberDecEx for temperature and then overlay for door time using raw segments if supported.
    // To preserve behavior reliably, we will show door time in lower two digits and temperature in upper two digits using setSegments.

    // Segment map is handled by library; easiest: showNumberDecEx with integer that places digits.
    // We'll create a 4-digit number: tempInt(1 digit) + frac(1 digit) + door tens + door ones
    // Note: negative handling: showNumberDecEx doesn't show minus in middle reliably; so ignore minus symbol to ensure compilation.
    int combined = (tempInt * 1000) + (tempFrac * 100) + ((int)d2 * 10) + (int)d3;

    // Show combined with decimal point after first digit (digit1 dot)
    tm1637.showNumberDecEx(combined, 0b01000000, true);
    (void)d0;
  }
}

void setup()
{
  pinMode(LM35_PIN, INPUT);
  pinMode(DOOR_MAGNET_PIN, INPUT_PULLUP); // door magnet switch D2 to input with pullup

  pinMode(RELAY_FAN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(RELAY_FAN_PIN, LOW); // OFF
  digitalWrite(BUZZER_PIN, LOW);   // buzzer off
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);
  tm1637.setBrightness(4);

  // Initialize door state tracking
  doorWasOpen = false;
  alarmActive = false;
  doorOpenStartMs = 0;
  alarmBlinkPrevMs = millis();
  alarmLedState = false;
}

void loop()
{
  uint32_t now = millis();

  // Door sensor: assume magnet switch closes when door is open, otherwise open.
  // With INPUT_PULLUP: closed => LOW, open => HIGH.
  // We'll interpret "door open" when input is LOW.
  bool doorOpen = (digitalRead(DOOR_MAGNET_PIN) == LOW);

  // Temperature reading
  float tempC = readTemperatureC();

  // --- Door open continuous logic (>10s) ---
  if (doorOpen) {
    if (!doorWasOpen) {
      // door just opened
      doorOpenStartMs = now;
      alarmActive = false; // will activate after threshold
      alarmBlinkPrevMs = now;
      alarmLedState = false;
    } else {
      // door has been open continuously
      if (!alarmActive && (now - doorOpenStartMs) >= DOOR_ALARM_MS) {
        alarmActive = true;
        // Start alarm blinking immediately
        alarmBlinkPrevMs = now;
        alarmLedState = false;
      }
    }
  } else {
    // door closed -> reset alarm and timers
    alarmActive = false;
    alarmLedState = false;
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  // Alarm output: buzzer and LED blink when door open >10s
  if (alarmActive) {
    // LED blinking
    if (now - alarmBlinkPrevMs >= ALARM_BLINK_MS) {
      alarmBlinkPrevMs = now;
      alarmLedState = !alarmLedState;
      digitalWrite(LED_PIN, alarmLedState ? HIGH : LOW);
    } else {
      // keep state as is
    }
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    // If not active, ensure buzzer off (unless door just closed which already handled)
    digitalWrite(BUZZER_PIN, LOW);
    // LED handled in door closed branch; but ensure off if alarm inactive
    if (!doorOpen) digitalWrite(LED_PIN, LOW);
  }

  // --- Temperature control with hysteresis for relay ---
  // Control: based on temperature, start/stop cooling relay.
  // Keep behavior deterministic with hysteresis:
  // - If relay currently off and temp >= upper => turn on
  // - If relay currently on and temp <= lower => turn off
  if (!relayOn && tempC >= TEMP_C_UPPER) {
    relayOn = true;
    digitalWrite(RELAY_FAN_PIN, HIGH); // ON
  } else if (relayOn && tempC <= TEMP_C_LOWER) {
    relayOn = false;
    digitalWrite(RELAY_FAN_PIN, LOW); // OFF
  }

  // --- Critical cooling loss serial output ---
  // If temperature above upper limit and door not closed -> continuously output critical cooling loss.
  // "门未关闭" interpreted as doorOpen == true.
  static uint32_t criticalPrevMs = 0;
  static const uint32_t CRITICAL_PERIOD_MS = 200; // continuous (every 200ms) to avoid flooding while still continuous
  if (tempC > TEMP_C_UPPER && doorOpen) {
    if (now - criticalPrevMs >= CRITICAL_PERIOD_MS) {
      criticalPrevMs = now;
      Serial.println("critical cooling loss");
    }
  } else {
    // reset rate limiter
    criticalPrevMs = now;
  }

  // --- TM1637 display ---
  uint32_t doorOpenMs = doorOpen ? (now - doorOpenStartMs) : 0;
  displayTemperatureAndDoorTime(tempC, doorOpen, doorOpenMs);

  // Update door state
  doorWasOpen = doorOpen;
}
