#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ===================== Pin Mapping (authoritative) =====================
static const uint8_t PIN_LM35_A1       = A1;   // water temperature sensor
static const uint8_t PIN_LIGHT_A0      = A0;   // LDR (environment light)
static const uint8_t PIN_WATERLEVEL_A2  = A2;   // water level sensor

static const uint8_t PIN_BUTTON_D2     = 2;    // toggle display mode

static const uint8_t PIN_LED_D9         = 9;    // aquarium light (LED)
static const uint8_t PIN_HEATER_D10     = 10;   // heater (use digital on/off)
static const uint8_t PIN_BUZZER_D6     = 6;    // buzzer alarm output

// TM1637: CLK=D4, DIO=D5 (note: D5 is also pump in requirement, but conflict resolved/remapped)
static const uint8_t PIN_TM1637_CLK     = 4;
static const uint8_t PIN_TM1637_DIO     = 5;
TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// Requirement mentions pump on D5, but wiring configuration explicitly remaps:
// "CLK=D4，DIO=D5". Therefore the D5 pin is used by TM1637 DIO.
// Pump control is not assigned to a hardware pin in the provided remapped config.
// We will keep pump behavior protection logic (based on water level),
// but the actual pump output pin is not specified by the authoritative pin mapping.
// If you have the pump connected to another pin, provide its pin assignment.
// For now, we only alarm and shut off "related devices" we can control
// (heater/lighting) and keep pump action as a no-op.

static const uint8_t PIN_PUMP_NOOP = 255; // unavailable due to conflict resolution

// ===================== Display/Control Settings =====================
// Button debounce
static const uint16_t BUTTON_DEBOUNCE_MS = 40;

// Display update
static const uint16_t DISPLAY_REFRESH_MS = 250;

// LM35 calibration & conversion
// LM35 output: 10 mV/°C => Vout = 0.01 * T (V)
static const float ADC_REF_VOLT = 5.0f;
static const int ADC_MAX = 1023;

// Convert ADC reading to temperature in Celsius
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readLightADC();
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readWaterLevelADC();
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showTempC(float tC);
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showLightADC(int adc);
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showWaterADC(int adc);
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBuzzer(bool on);
#line 129 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateWaterProtection();
#line 140 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void controlHeaterAndLightingAndPump();
#line 190 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateAlarm();
#line 222 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleButtonToggle();
#line 244 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 269 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC() {
  int adc = analogRead(PIN_LM35_A1);
  float v = (adc * ADC_REF_VOLT) / (float)ADC_MAX;
  // 10 mV/°C => 0.01 V/°C
  float tC = v / 0.01f;
  return tC;
}

// Read light as raw ADC
static int readLightADC() {
  return analogRead(PIN_LIGHT_A0);
}

// Read water level as raw ADC
static int readWaterLevelADC() {
  return analogRead(PIN_WATERLEVEL_A2);
}

// Thresholds (must be preserved; not provided in prompt).
// Use conservative defaults typical for such setups.
// If you have specific thresholds, replace these values accordingly.
static const float TEMP_HEAT_ON_C  = 24.0f; // turn heater ON when below this
static const float TEMP_HEAT_OFF_C = 26.0f; // turn heater OFF when above this (hysteresis)

static const int LIGHT_ON_ADC  = 500; // turn LED ON when ambient is below this
static const int LIGHT_OFF_ADC = 700; // turn LED OFF when ambient is above this (hysteresis)

static const int WATERLEVEL_LOW_ADC = 400; // if below this => water level too low (protect)
static const int WATERLEVEL_OK_ADC  = 500; // if above this => water level ok (hysteresis)

// Alarm behavior
static const uint16_t ALARM_BEEP_INTERVAL_MS = 300;
static const uint16_t ALARM_BEEP_DURATION_MS = 120;
static const uint16_t NORMAL_BEEP_FREQ_HZ  = 2200; // buzzer frequency during alarm

// ===================== System State =====================
enum DisplayMode : uint8_t { MODE_TEMP = 0, MODE_LIGHT = 1, MODE_WATER = 2 };
static volatile DisplayMode displayMode = MODE_TEMP;

static uint32_t lastDisplayUpdateMs = 0;

// Button debounce
static bool lastButtonStable = HIGH;
static bool lastButtonReading = HIGH;
static uint32_t lastDebounceMs = 0;

// Heater and light state with hysteresis
static bool heaterOn = false;
static bool ledOn = false;

// Water level protection latch (with hysteresis)
static bool waterLow = false;

// Alarm timing
static uint32_t lastAlarmToggleMs = 0;
static bool alarmBeepOn = false;

// ===================== Helper: Display =====================
// Show number on TM1637; adapt formatting to mode (integer scaling)
static void showTempC(float tC) {
  // show tenths resolution as integer? not specified.
  // We'll display with 0.1°C resolution using scaling 10.
  // Example: 25.3 => 253
  int16_t scaled = (int16_t)lround(tC * 10.0f);

  tm1637.showNumberDecEx(scaled, 0b01000000, true, 4, 0); // negative supported
  // Note: TM1637Display supports colon/dots depending library; we just show scaled number.
}

static void showLightADC(int adc) {
  // display 0-1023 as 4 digits (truncate to fit)
  // Use direct value for clarity.
  tm1637.showNumberDec(adc, true, 4, 0);
}

static void showWaterADC(int adc) {
  tm1637.showNumberDec(adc, true, 4, 0);
}

// ===================== Helper: Alarm =====================
static void setBuzzer(bool on) {
  digitalWrite(PIN_BUZZER_D6, on ? HIGH : LOW);
}

// ===================== Core Logic =====================
static void updateWaterProtection() {
  int wl = readWaterLevelADC();

  // Hysteresis for waterLow latch
  if (!waterLow && wl < WATERLEVEL_LOW_ADC) {
    waterLow = true;
  } else if (waterLow && wl > WATERLEVEL_OK_ADC) {
    waterLow = false;
  }
}

static void controlHeaterAndLightingAndPump() {
  // If water is too low: protect
  if (waterLow) {
    // Shutdown related devices
    if (heaterOn) {
      heaterOn = false;
      digitalWrite(PIN_HEATER_D10, LOW);
    }
    if (ledOn) {
      ledOn = false;
      digitalWrite(PIN_LED_D9, LOW);
    }

    // Pump control is not available due to pin conflict with TM1637 mapping (D5 used by TM1637 DIO).
    // Keep as no-op; alarm will indicate protection.
    if (PIN_PUMP_NOOP != 255) {
      digitalWrite(PIN_PUMP_NOOP, LOW);
    }
  } else {
    // Normal operation

    // Water temperature control for heater with hysteresis
    float tC = readTemperatureC();
    if (!heaterOn && tC < TEMP_HEAT_ON_C) {
      heaterOn = true;
      digitalWrite(PIN_HEATER_D10, HIGH);
    } else if (heaterOn && tC > TEMP_HEAT_OFF_C) {
      heaterOn = false;
      digitalWrite(PIN_HEATER_D10, LOW);
    }

    // Light control for aquarium LED with hysteresis
    int light = readLightADC();
    if (!ledOn && light < LIGHT_ON_ADC) {
      ledOn = true;
      digitalWrite(PIN_LED_D9, HIGH);
    } else if (ledOn && light > LIGHT_OFF_ADC) {
      ledOn = false;
      digitalWrite(PIN_LED_D9, LOW);
    }

    // Pump control (if pin provided); otherwise no-op.
    if (PIN_PUMP_NOOP != 255) {
      // Requirement says: "根据水位控制增氧泵保护；若水位过低则关闭相关设备并报警。"
      // We interpret: pump is allowed to run only when water level is OK.
      digitalWrite(PIN_PUMP_NOOP, HIGH);
    }
  }
}

static void updateAlarm() {
  if (!waterLow) {
    alarmBeepOn = false;
    setBuzzer(false);
    return;
  }

  uint32_t now = millis();

  // Beep periodically: on for duration, off in interval
  if (now - lastAlarmToggleMs >= ALARM_BEEP_INTERVAL_MS) {
    lastAlarmToggleMs = now;
  }

  // Use modulo within interval for duration
  uint32_t within = now % ALARM_BEEP_INTERVAL_MS;
  if (within < ALARM_BEEP_DURATION_MS) {
    if (!alarmBeepOn) {
      alarmBeepOn = true;
      // Use tone for better beep if possible; buzzer is digital-output only required.
      // We'll use tone() while low, then noTone().
      tone(PIN_BUZZER_D6, NORMAL_BEEP_FREQ_HZ);
    }
  } else {
    if (alarmBeepOn) {
      alarmBeepOn = false;
      noTone(PIN_BUZZER_D6);
      setBuzzer(false);
    }
  }
}

static void handleButtonToggle() {
  bool reading = digitalRead(PIN_BUTTON_D2);

  if (reading != lastButtonReading) {
    lastDebounceMs = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastDebounceMs) > BUTTON_DEBOUNCE_MS) {
    if (reading != lastButtonStable) {
      lastButtonStable = reading;

      // Assuming button pressed pulls signal LOW (use INPUT_PULLUP)
      if (lastButtonStable == LOW) {
        // Cycle modes: Temp -> Light -> Water
        displayMode = (DisplayMode)((displayMode + 1) % 3);
      }
    }
  }
}

// ===================== Arduino Setup/Loop =====================
void setup() {
  pinMode(PIN_BUTTON_D2, INPUT_PULLUP);

  pinMode(PIN_LED_D9, OUTPUT);
  pinMode(PIN_HEATER_D10, OUTPUT);

  pinMode(PIN_BUZZER_D6, OUTPUT);
  noTone(PIN_BUZZER_D6);

  digitalWrite(PIN_LED_D9, LOW);
  digitalWrite(PIN_HEATER_D10, LOW);

  tm1637.setBrightness(7, true); // max brightness

  // Init states
  waterLow = false;
  heaterOn = false;
  ledOn = false;

  lastAlarmToggleMs = millis();
  lastButtonStable = digitalRead(PIN_BUTTON_D2);
  lastButtonReading = lastButtonStable;
  lastDebounceMs = millis();
}

void loop() {
  handleButtonToggle();

  // Update protection and device control frequently
  updateWaterProtection();
  controlHeaterAndLightingAndPump();
  updateAlarm();

  // Refresh display at set interval
  uint32_t now = millis();
  if (now - lastDisplayUpdateMs >= DISPLAY_REFRESH_MS) {
    lastDisplayUpdateMs = now;

    switch (displayMode) {
      case MODE_TEMP: {
        float tC = readTemperatureC();
        // Display scaled tenths with 4 digits
        // If too large, it may overflow; library will show best effort.
        showTempC(tC);
        break;
      }
      case MODE_LIGHT: {
        int light = readLightADC();
        showLightADC(light);
        break;
      }
      case MODE_WATER: {
        int wl = readWaterLevelADC();
        showWaterADC(wl);
        break;
      }
      default:
        break;
    }
  }
}
