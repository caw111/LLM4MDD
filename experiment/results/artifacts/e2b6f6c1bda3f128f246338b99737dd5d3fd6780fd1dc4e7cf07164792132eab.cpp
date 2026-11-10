#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_028\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

#define FAN_PWM_PIN 5    // D5
#define LM35_PIN A1      // A1
#define BUZZER_PIN 6     // D6
#define TM1637_CLK 4     // D4
#define TM1637_DIO 2     // D2

// ---- Temperature thresholds (°C) ----
static const float T_OFF_MAX = 25.0;  // <= 25.0 => OFF
static const float T_LOW_MAX = 35.0;  // (25.0, 35.0] => LOW
static const float T_MID_MAX = 45.0;  // (35.0, 45.0] => MID
// > 45.0 => HIGH

// ---- PWM levels for each mode ----
static const uint8_t PWM_OFF = 0;
static const uint8_t PWM_LOW = 90;     // ~35% duty
static const uint8_t PWM_MID = 170;    // ~67% duty
static const uint8_t PWM_HIGH = 255;   // 100% duty

// ---- Display encoding for TM1637 ----
enum FanMode : uint8_t { MODE_OFF = 0, MODE_LOW = 1, MODE_MID = 2, MODE_HIGH = 3 };

// ---- LM35 calibration ----
static const float VREF = 5.0;      // UNO analog reference default
static const int ADC_MAX = 1023;

// LM35 scale: 10mV/°C => 1°C = 0.01V => tempC = (Vout * 100)
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_028\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static float readTemperatureC();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_028\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint8_t pwmForMode(FanMode m);
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_028\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_028\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_028\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static float readTemperatureC() {
  const uint8_t samples = 10;
  uint32_t sum = 0;
  for (uint8_t i = 0; i < samples; i++) {
    sum += analogRead(LM35_PIN);
    delay(5);
  }
  float adc = (float)sum / (float)samples;
  float vout = (adc * VREF) / (float)ADC_MAX; // volts
  float tempC = vout * 100.0f;               // °C
  return tempC;
}

static TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

// Keep track of mode changes to trigger buzzer only when entering HIGH
static FanMode currentMode = MODE_OFF;
static bool buzzerArmed = true;
static unsigned long lastUpdateMs = 0;

// Buzzer behavior: short鸣提示 when entering HIGH (ON ~150ms)
static const uint16_t BUZZER_FREQ = 2000; // Hz
static const uint16_t BUZZER_BEEP_MS = 150;

static uint8_t pwmForMode(FanMode m) {
  switch (m) {
    case MODE_OFF: return PWM_OFF;
    case MODE_LOW: return PWM_LOW;
    case MODE_MID: return PWM_MID;
    case MODE_HIGH: return PWM_HIGH;
    default: return PWM_OFF;
  }
}

void setup() {
  pinMode(FAN_PWM_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  analogReference(DEFAULT);

  // Initialize outputs
  analogWrite(FAN_PWM_PIN, PWM_OFF);
  digitalWrite(BUZZER_PIN, LOW);

  // Init TM1637
  tm1637.setBrightness(0x0f, true);  // max brightness, auto update
  tm1637.showNumberDec((int)currentMode, false); // show 0 initially

  lastUpdateMs = millis();
}

void loop() {
  // Periodic update
  if (millis() - lastUpdateMs < 300) return;
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
    analogWrite(FAN_PWM_PIN, pwmForMode(currentMode));

    // Update display
    // 0=OFF, 1=LOW, 2=MID, 3=HIGH
    tm1637.showNumberDec((int)currentMode, false);

    // Buzzer: entering HIGH => short beep
    if (currentMode == MODE_HIGH) {
      if (buzzerArmed) {
        tone(BUZZER_PIN, BUZZER_FREQ);
        delay(BUZZER_BEEP_MS);
        noTone(BUZZER_PIN);
        buzzerArmed = false;
      }
    } else {
      buzzerArmed = true;
    }
  }

  // Keep display updated at the sensor update rate
  tm1637.showNumberDec((int)currentMode, false);
}
