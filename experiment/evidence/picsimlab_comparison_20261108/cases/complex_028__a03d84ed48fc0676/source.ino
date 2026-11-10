#include <Arduino.h>
#include <TM1637Display.h>

#define FAN_PWM_PIN 5    // D5
#define LM35_PIN A1      // A1
#define BUZZER_PIN 6     // D6
#define TM1637_CLK 4     // D4
#define TM1637_DIO 2     // D2

// ---- Temperature thresholds (°C) ----
// Requirement: four modes: OFF, low, middle, high.
// (Use calibration constants + thresholds as defined here.)
// If you need different thresholds, adjust these values.
static const float T_OFF_MAX    = 25.0;  // < = 25.0°C => OFF
static const float T_LOW_MAX    = 35.0;  // (25.0, 35.0] => LOW
static const float T_MID_MAX    = 45.0;  // (35.0, 45.0] => MID
// > 45.0 => HIGH

// ---- PWM levels for each mode ----
static const uint8_t PWM_OFF  = 0;
static const uint8_t PWM_LOW  = 90;   // ~35% duty
static const uint8_t PWM_MID  = 170;  // ~67% duty
static const uint8_t PWM_HIGH = 255;  // 100% duty

// ---- Display encoding for TM1637 ----
// Show current mode as 0..3: 0=OFF, 1=LOW, 2=MID, 3=HIGH
enum FanMode : uint8_t { MODE_OFF = 0, MODE_LOW = 1, MODE_MID = 2, MODE_HIGH = 3 };

// ---- LM35 calibration ----
static const float VREF = 5.0;      // UNO analog reference default
static const int   ADC_MAX = 1023;

// LM35 scale: 10mV/°C, so 1°C = 0.01V
// temperature = (Vout / 10mV) = (Vout * 100)
static float readTemperatureC()
{
  // Simple averaging to stabilize readings
  const uint8_t samples = 10;
  uint32_t sum = 0;
  for (uint8_t i = 0; i < samples; i++) {
    sum += analogRead(LM35_PIN);
    delay(5);
  }
  float adc = (float)sum / (float)samples;
  float vout = (adc * VREF) / (float)ADC_MAX; // volts
  float tempC = vout * 100.0f;               // °C (LM35: 10mV/°C)
  return tempC;
}

static TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

// Keep track of mode changes to trigger buzzer only when entering HIGH
static FanMode currentMode = MODE_OFF;
static bool buzzerArmed = true;
static unsigned long lastUpdateMs = 0;

// Buzzer behavior: short鸣提示 when entering HIGH.
// (Short beep: ON for ~150ms.)
static const uint16_t BUZZER_FREQ = 2000; // Hz
static const uint16_t BUZZER_BEEP_MS = 150;

void setup()
{
  pinMode(FAN_PWM_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  analogReference(DEFAULT);

  // Initialize outputs
  analogWrite(FAN_PWM_PIN, PWM_OFF);
  digitalWrite(BUZZER_PIN, LOW);

  // Init TM1637
  tm1637.setBrightness(0x0f, true); // max brightness, auto update
  tm1637.showNumberDec((int)currentMode, false); // show 0 initially
  tm1637.clearDisplay();

  // Show initial mode
  tm1637.showNumberDec((int)currentMode, false);

  lastUpdateMs = millis();
}

void loop()
{
  // Periodic update
  if (millis() - lastUpdateMs < 300) {
    return;
  }
  lastUpdateMs = millis();

  float t = readTemperatureC();

  FanMode newMode;
  if (t <= T_OFF_MAX) {
    newMode = MODE_OFF;
  } else if (t <= T_LOW_MAX) {
    newMode = MODE_LOW;
  } else if (t <= T_MID_MAX) {
    newMode = MODE_MID;
  } else {
    newMode = MODE_HIGH;
  }

  if (newMode != currentMode) {
    currentMode = newMode;

    // Update fan PWM based on mode
    uint8_t pwmVal = PWM_OFF;
    switch (currentMode) {
      case MODE_OFF:  pwmVal = PWM_OFF;  break;
      case MODE_LOW:  pwmVal = PWM_LOW;  break;
      case MODE_MID:  pwmVal = PWM_MID;  break;
      case MODE_HIGH: pwmVal = PWM_HIGH; break;
    }
    analogWrite(FAN_PWM_PIN, pwmVal);

    // Display current mode
    // 0=OFF, 1=LOW, 2=MID, 3=HIGH
    tm1637.showNumberDec((int)currentMode, false);

    // Buzzer: entering HIGH => short beep
    if (currentMode == MODE_HIGH) {
      // short鸣提示 (play once per entry)
      if (buzzerArmed) {
        tone(BUZZER_PIN, BUZZER_FREQ);
        delay(BUZZER_BEEP_MS);
        noTone(BUZZER_PIN);
        buzzerArmed = false;
      }
    } else {
      // re-arm buzzer once we leave HIGH
      buzzerArmed = true;
    }
  }

  // Even if mode doesn't change, keep display updated per requirement
  // (No harm in re-showing; but avoid flicker by only updating at same rate as sensor.)
  tm1637.showNumberDec((int)currentMode, false);
}