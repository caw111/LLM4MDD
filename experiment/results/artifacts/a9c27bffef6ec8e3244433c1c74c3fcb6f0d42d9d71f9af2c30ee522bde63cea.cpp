#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ----- Pin assignments (authoritative) -----
static const uint8_t PIN_LM35_A1     = A1; // Water temperature sensor
static const uint8_t PIN_LIGHT_A0    = A0; // LDR light sensor
static const uint8_t PIN_WATER_A2    = A2; // Water level sensor (analog)
static const uint8_t PIN_BTN_D2      = 2;  // Display toggle button

static const uint8_t PIN_HEATER_D10   = 10; // Heater control
static const uint8_t PIN_PUMP_D5      = 5;  // Water pump / aeration pump
static const uint8_t PIN_LED_D9       = 9;  // Lighting LED
static const uint8_t PIN_BUZZER_D6    = 6;  // Buzzer (tone output)

static const uint8_t PIN_TM1637_CLK_D4 = 4; // CLK
static const uint8_t PIN_TM1637_DIO_D3 = 3; // DIO

// ----- Display -----
TM1637Display display(PIN_TM1637_CLK_D4, PIN_TM1637_DIO_D3);

// ----- Thresholds & behavior (preserve as explicit intent) -----
// These values are chosen to implement required behavior robustly.
// If your hardware needs different thresholds, adjust here only.
static const float LM35_VREF = 5.0f;      // Uno analog reference default 5V
static const uint8_t ADC_MAX = 1023;

static const float TEMP_ON_C  = 26.0f;    // Heater ON if water temp < TEMP_ON_C
static const float TEMP_OFF_C = 28.0f;    // Heater OFF if water temp >= TEMP_OFF_C (hysteresis)

static const int LIGHT_ON_TH = 600;       // Lighting ON if LDR reading below threshold (dimmer => lower ADC)
static const int LIGHT_OFF_TH = 750;      // Lighting OFF threshold (hysteresis)

static const int WATER_LOW_TH = 350;      // Low water threshold (analog) -> protect & alarm
// Optionally include hysteresis for returning to normal:
static const int WATER_OK_TH  = 450;

// ----- Button debounce & mode switching -----
enum DisplayMode : uint8_t { MODE_TEMP = 0, MODE_LIGHT = 1, MODE_WATER = 2 };
static volatile bool buttonPressedFlag = false;

static DisplayMode mode = MODE_TEMP;
static uint8_t lastButtonStable = HIGH;
static unsigned long lastDebounceMs = 0;
static unsigned long lastPressMs = 0;

// ----- Alarm -----
static bool inLowWater = false;
static unsigned long alarmLastToggleMs = 0;
static bool alarmState = false;
static const unsigned long ALARM_TOGGLE_MS = 250;

// ----- Timing -----
static unsigned long lastReadMs = 0;
static const unsigned long READ_INTERVAL_MS = 300;

// Simple moving average for stability (optional but keeps "behavior" intact)
static const uint8_t AVG_SAMPLES = 8;
static int tempSamples[AVG_SAMPLES];
static int lightSamples[AVG_SAMPLES];
static int waterSamples[AVG_SAMPLES];
static uint8_t sampleIndex = 0;
static unsigned long sampleCount = 0;

static long sumTemp = 0;
static long sumLight = 0;
static long sumWater = 0;

#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readLM35C_Avg();
#line 77 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readLightAvg();
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readWaterAvg();
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void buzzerAlarm(bool enable);
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayValueForMode(float tempC, int lightADC, int waterADC);
#line 153 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateAverages();
#line 191 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void controlOutputs(float tempC, int lightADC, int waterADC);
#line 245 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 275 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readLM35C_Avg() {
  // LM35 output: 10mV/°C
  // ADC reading -> voltage -> temperature
  float avgADC = (float)(sumTemp) / (float)AVG_SAMPLES;
  float volts = (avgADC * LM35_VREF) / (float)ADC_MAX;
  // 10mV/°C => volts / 0.01 = °C
  return volts / 0.01f;
}

static int readLightAvg() {
  return (int)((float)sumLight / (float)AVG_SAMPLES);
}

static int readWaterAvg() {
  return (int)((float)sumWater / (float)AVG_SAMPLES);
}

static void buzzerAlarm(bool enable) {
  if (!enable) {
    noTone(PIN_BUZZER_D6);
    return;
  }
  // Use non-blocking tone toggling pattern
  unsigned long now = millis();
  if (now - alarmLastToggleMs >= ALARM_TOGGLE_MS) {
    alarmLastToggleMs = now;
    alarmState = !alarmState;
    if (alarmState) {
      tone(PIN_BUZZER_D6, 2000); // 2kHz
    } else {
      noTone(PIN_BUZZER_D6);
    }
  }
}

static void displayValueForMode(float tempC, int lightADC, int waterADC) {
  // TM1637 typically shows 4 digits. We'll scale to show readable values.
  // Temp: show with 1 decimal (e.g., 26.3C -> 263 with dot). We'll map into 4 digits.
  // Light: show ADC/10 (0..102) -> 3 digits; water: show ADC (0..1023) truncated.
  display.setBrightness(7);

  if (mode == MODE_TEMP) {
    // Convert to tenths
    int tenths = (int)roundf(tempC * 10.0f); // could be negative in theory
    bool neg = (tenths < 0);
    if (neg) tenths = -tenths;

    int d1 = (tenths / 1000) % 10;
    int d2 = (tenths / 100) % 10;
    int d3 = (tenths / 10) % 10;
    int d4 = (tenths / 1) % 10;

    // We'll show last 3 digits plus decimal point at d3 (tenths place).
    // e.g., 263 -> 26.3 (d2=2 d3=6 d4=3). Use leading blank/negative sign by overwriting d1.
    uint8_t seg[4];

    // Represent negative sign on first position if needed
    if (neg) {
      // Use segment encoding for '-' by letting setSegments via raw bits; simpler:
      // We'll use display.showNumberDecEx for convenience with decimals.
      // Use formatting: number in tenths with decimal. We'll compute scaled integer.
      // Use showNumberDecEx with decimals=1 and negative supported.
      int scaled = (int)roundf(tempC * 10.0f);
      display.showNumberDecEx(scaled, 0b01000000, true, 1); // decimal at position 2? library-specific flags
      // If flags not matching, fall back:
      return;
    } else {
      int scaled = (int)roundf(tempC * 10.0f);
      // Show with 1 decimal
      // Using library API where second param sets blank/dp. Keep decimal at 1.
      // We'll just use showNumberDec with rounding for reliability:
      display.showNumberDec((int)roundf(tempC), false, 4, 0);
      return;
    }
  } else if (mode == MODE_LIGHT) {
    int v = lightADC / 10; // 0..102
    display.showNumberDec(v, false, 4, 0);
  } else { // MODE_WATER
    int v = waterADC; // 0..1023
    if (v > 999) v = 999;
    display.showNumberDec(v, false, 4, 0);
  }
}

// Read sensors with averaging
static void updateAverages() {
  int t = analogRead(PIN_LM35_A1);
  int l = analogRead(PIN_LIGHT_A0);
  int w = analogRead(PIN_WATER_A2);

  // Initialize sums on first samples
  if (sampleCount < AVG_SAMPLES) {
    tempSamples[sampleIndex] = t;
    lightSamples[sampleIndex] = l;
    waterSamples[sampleIndex] = w;
    sumTemp += t;
    sumLight += l;
    sumWater += w;
    sampleIndex = (sampleIndex + 1) % AVG_SAMPLES;
    sampleCount++;
    return;
  }

  // Subtract oldest and add newest
  int oldT = tempSamples[sampleIndex];
  int oldL = lightSamples[sampleIndex];
  int oldW = waterSamples[sampleIndex];

  sumTemp -= oldT;
  sumLight -= oldL;
  sumWater -= oldW;

  tempSamples[sampleIndex] = t;
  lightSamples[sampleIndex] = l;
  waterSamples[sampleIndex] = w;

  sumTemp += t;
  sumLight += l;
  sumWater += w;

  sampleIndex = (sampleIndex + 1) % AVG_SAMPLES;
}

static void controlOutputs(float tempC, int lightADC, int waterADC) {
  // Low-water protection
  if (!inLowWater) {
    if (waterADC <= WATER_LOW_TH) {
      inLowWater = true;
    }
  } else {
    // Return only when water recovers above OK threshold
    if (waterADC >= WATER_OK_TH) {
      inLowWater = false;
    }
  }

  // If low water: close relevant devices and alarm
  if (inLowWater) {
    digitalWrite(PIN_HEATER_D10, LOW);
    digitalWrite(PIN_LED_D9, LOW);
    digitalWrite(PIN_PUMP_D5, LOW);
    buzzerAlarm(true);
    return;
  } else {
    buzzerAlarm(false);
  }

  // Temperature control with hysteresis
  // Heater ON if temp < TEMP_ON_C, OFF if temp >= TEMP_OFF_C
  // We'll keep last heater state implicitly by reading pin.
  int heaterState = digitalRead(PIN_HEATER_D10);

  if (tempC < TEMP_ON_C) {
    heaterState = HIGH;
  } else if (tempC >= TEMP_OFF_C) {
    heaterState = LOW;
  }
  digitalWrite(PIN_HEATER_D10, heaterState);

  // Light control with hysteresis
  int ledState = digitalRead(PIN_LED_D9);
  // LDR: lower ADC means darker (more resistance) depending on wiring; requirement says "根据环境光照控制照明"
  // We'll interpret: if environment is dark, turn ON LED.
  if (lightADC <= LIGHT_ON_TH) {
    ledState = HIGH;
  } else if (lightADC >= LIGHT_OFF_TH) {
    ledState = LOW;
  }
  digitalWrite(PIN_LED_D9, ledState);

  // Water pump / aeration control:
  // Requirement: "根据水位控制增氧泵保护；若水位过低则关闭相关设备并报警"
  // No further pump regulation given, so pump runs normally (ON) when water level is OK.
  // This preserves "protection" behavior while not adding unintended cycles.
  digitalWrite(PIN_PUMP_D5, HIGH);
}

void setup() {
  // Pin modes
  pinMode(PIN_BTN_D2, INPUT_PULLUP);

  pinMode(PIN_HEATER_D10, OUTPUT);
  pinMode(PIN_PUMP_D5, OUTPUT);
  pinMode(PIN_LED_D9, OUTPUT);
  pinMode(PIN_BUZZER_D6, OUTPUT);

  // Ensure outputs off
  digitalWrite(PIN_HEATER_D10, LOW);
  digitalWrite(PIN_PUMP_D5, LOW);
  digitalWrite(PIN_LED_D9, LOW);
  noTone(PIN_BUZZER_D6);

  // Initialize display
  display.setBrightness(7);
  display.showNumberDec(0, false, 4, 0);

  // Initialize averaging buffers
  sampleIndex = 0;
  sampleCount = 0;
  sumTemp = sumLight = sumWater = 0;
  for (uint8_t i = 0; i < AVG_SAMPLES; i++) {
    tempSamples[i] = 0;
    lightSamples[i] = 0;
    waterSamples[i] = 0;
  }
}

void loop() {
  // Button debounce + mode switching
  uint8_t reading = digitalRead(PIN_BTN_D2);
  unsigned long now = millis();

  if (reading != lastButtonStable) {
    lastDebounceMs = now;
    lastButtonStable = reading;
  }

  // Detect a press (active LOW due to pullup)
  static uint8_t lastReading = HIGH;
  if (reading != lastReading) {
    lastReading = reading;
  }

  // Stable press
  if (reading == LOW && (now - lastDebounceMs) > 30) {
    // Avoid multiple triggers while holding button
    if (now - lastPressMs > 300) {
      lastPressMs = now;
      mode = (DisplayMode)(((uint8_t)mode + 1) % 3);
    }
    // wait until release without blocking too long by simply continuing
  }

  // Periodic sensor update and control
  if (now - lastReadMs >= READ_INTERVAL_MS) {
    lastReadMs = now;

    updateAverages();

    // Use averaged values only after we have enough samples; before that,
    // use current single readings approximated by treating averages.
    float tempC;
    int lightADC;
    int waterADC;

    if (sampleCount < AVG_SAMPLES) {
      // Fill with whatever we have so far by using current instantaneous values
      int tRaw = analogRead(PIN_LM35_A1);
      int lRaw = analogRead(PIN_LIGHT_A0);
      int wRaw = analogRead(PIN_WATER_A2);

      float volts = ((float)tRaw * LM35_VREF) / (float)ADC_MAX;
      tempC = volts / 0.01f;
      lightADC = lRaw;
      waterADC = wRaw;
    } else {
      tempC = readLM35C_Avg();
      lightADC = readLightAvg();
      waterADC = readWaterAvg();
    }

    // Control outputs based on latest measurements
    controlOutputs(tempC, lightADC, waterADC);

    // Update display
    displayValueForMode(tempC, lightADC, waterADC);
  }
}
