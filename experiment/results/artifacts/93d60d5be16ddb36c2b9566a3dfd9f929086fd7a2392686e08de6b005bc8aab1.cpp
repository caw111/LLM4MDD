#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

#define DHTPIN D7
#define DHTTYPE DHT11

#define LIGHT_PIN A0
#define SOIL_PIN A1
#define KNOB_PIN A2

#define FAN_PWM_PIN D5
#define LED_PIN D9
#define PUMP_RELAY_PIN D10

#define BUZZER_PIN D6

#define BTN_MODE D2
#define BTN_NEXT D3

#define TM1637_CLK D4
#define TM1637_DIO D8

// -------------------- Display --------------------
TM1637Display display(TM1637_CLK, TM1637_DIO);

// -------------------- Sensors --------------------
DHT dht(DHTPIN, DHTTYPE);

// -------------------- State --------------------
enum Mode : uint8_t { MODE_AUTO = 0, MODE_MANUAL = 1 };
enum ManualTarget : uint8_t { TARGET_FAN = 0, TARGET_LED = 1, TARGET_PUMP = 2 };

volatile Mode currentMode = MODE_AUTO;
volatile ManualTarget currentTarget = TARGET_FAN;

// -------------------- Manual PWM mapping --------------------
static const uint8_t MANUAL_PWM_MIN = 0;
static const uint8_t MANUAL_PWM_MAX = 255;

// -------------------- Thresholds (set as typical defaults; adjust as needed) --------------------
// Temperature thresholds (DHT11)
static const float TEMP_HIGH = 30.0f;     // turn fan ON
static const float TEMP_LOW  = 28.0f;     // turn fan OFF

// Light thresholds (LDR on A0)
static const int LIGHT_DARK = 400;         // below -> dark, enable LED
static const int LIGHT_BRIGHT = 550;      // above -> bright, disable LED

// Soil moisture thresholds (A1)
static const int SOIL_DRY = 400;           // below -> dry, pump ON
static const int SOIL_WET = 550;          // above -> wet, pump OFF

// -------------------- Reading anomaly detection --------------------
static const unsigned long SENSOR_READ_INTERVAL_MS = 2000;
static const unsigned long DISPLAY_INTERVAL_MS = 1200;
static const unsigned long BUTTON_DEBOUNCE_MS = 50;

static unsigned long lastSensorReadMs = 0;
static unsigned long lastDisplaySwitchMs = 0;

static float lastTempC = NAN;
static int lastLightRaw = -1;
static int lastSoilRaw = -1;

static bool faultTemp = false;
static bool faultLight = false;
static bool faultSoil = false;
static bool anyFault = false;

// -------------------- Buzzer behavior --------------------
static const unsigned long BUZZER_BURST_MS = 200;
static const unsigned long BUZZER_PAUSE_MS = 200;
static unsigned long buzzerLastToggleMs = 0;
static bool buzzerOn = false;

// -------------------- Display cycling --------------------
enum DisplayPage : uint8_t { PAGE_TEMP = 0, PAGE_LIGHT = 1, PAGE_SOIL = 2 };
static DisplayPage displayPage = PAGE_TEMP;

#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBuzzer(bool on);
#line 90 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonRaw(uint8_t pin);
#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool buttonPressedWithDebounce(uint8_t pin, unsigned long &lastPressMs);
#line 112 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t knobPWM();
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyAutoControls();
#line 160 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyManualControls();
#line 182 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void reportFaultIfAny();
#line 220 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void readSensorsAndValidate();
#line 259 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showPage();
#line 308 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 337 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBuzzer(bool on) {
  if (on) {
    tone(BUZZER_PIN, 2000);
  } else {
    noTone(BUZZER_PIN);
  }
}

// -------------------- Button helpers --------------------
static bool readButtonRaw(uint8_t pin) {
  // Using INPUT_PULLUP expected
  return (digitalRead(pin) == LOW);
}

static bool buttonPressedWithDebounce(uint8_t pin, unsigned long &lastPressMs) {
  bool pressed = readButtonRaw(pin);
  if (!pressed) return false;

  unsigned long now = millis();
  if (now - lastPressMs >= BUTTON_DEBOUNCE_MS) {
    lastPressMs = now;
    // Wait until released to avoid repeats (non-blocking-ish with small loop)
    while (digitalRead(pin) == LOW) {
      delay(1);
    }
    return true;
  }
  return false;
}

// -------------------- Output control --------------------
static uint8_t knobPWM() {
  int v = analogRead(KNOB_PIN);
  v = constrain(v, 0, 1023);
  // Map 0..1023 -> MANUAL_PWM_MIN..MANUAL_PWM_MAX
  return (uint8_t)map(v, 0, 1023, MANUAL_PWM_MIN, MANUAL_PWM_MAX);
}

static void applyAutoControls() {
  // Fan control with hysteresis
  if (isnan(lastTempC)) {
    // If invalid, do not change outputs here; fault handler covers alert.
  } else {
    if (lastTempC >= TEMP_HIGH) {
      analogWrite(FAN_PWM_PIN, 255);
    } else if (lastTempC <= TEMP_LOW) {
      analogWrite(FAN_PWM_PIN, 0);
    }
    // Between thresholds: keep current state (hysteresis behavior)
  }

  // LED (light) control with hysteresis:
  // If dark -> LED ON; if bright -> LED OFF; between keep.
  if (lastLightRaw >= 0) {
    static int ledState = 0;
    if (lastLightRaw <= LIGHT_DARK) {
      ledState = 1;
      digitalWrite(LED_PIN, HIGH);
    } else if (lastLightRaw >= LIGHT_BRIGHT) {
      ledState = 0;
      digitalWrite(LED_PIN, LOW);
    }
    // keep between
  }

  // Pump control with hysteresis
  if (lastSoilRaw >= 0) {
    static int pumpState = 0;
    if (lastSoilRaw <= SOIL_DRY) {
      pumpState = 1;
      digitalWrite(PUMP_RELAY_PIN, HIGH);
    } else if (lastSoilRaw >= SOIL_WET) {
      pumpState = 0;
      digitalWrite(PUMP_RELAY_PIN, LOW);
    }
    // keep between
  }
}

static void applyManualControls() {
  uint8_t p = knobPWM();

  // Manual target selection:
  // - Fan: PWM with p
  // - LED: use p threshold to decide ON/OFF (or map to brightness not required; requirement says adjust output)
  //   We'll implement brightness using PWM even if LED pin is specified as D9 output. D9 supports PWM on UNO.
  // - Pump: relay, use p threshold to decide ON/OFF
  switch (currentTarget) {
    case TARGET_FAN:
      analogWrite(FAN_PWM_PIN, p);
      break;
    case TARGET_LED:
      analogWrite(LED_PIN, p);
      break;
    case TARGET_PUMP:
      // Relay: choose threshold at 128
      digitalWrite(PUMP_RELAY_PIN, (p >= 128) ? HIGH : LOW);
      break;
  }
}

static void reportFaultIfAny() {
  // Requirement: "If any sensor reading abnormal -> buzzer D6 alarm and through serial output fault information"
  if (!anyFault) return;

  // Output fault details periodically when fault remains
  static unsigned long lastReportMs = 0;
  const unsigned long REPORT_INTERVAL_MS = 1000;

  unsigned long now = millis();
  if (now - lastReportMs < REPORT_INTERVAL_MS) return;
  lastReportMs = now;

  Serial.print("[FAULT] ");
  bool first = true;

  if (faultTemp) {
    if (!first) Serial.print(", ");
    Serial.print("TEMP=");
    if (isnan(lastTempC)) Serial.print("NaN");
    else Serial.print(lastTempC);
    first = false;
  }
  if (faultLight) {
    if (!first) Serial.print(", ");
    Serial.print("LIGHT=");
    Serial.print(lastLightRaw);
    first = false;
  }
  if (faultSoil) {
    if (!first) Serial.print(", ");
    Serial.print("SOIL=");
    Serial.print(lastSoilRaw);
    first = false;
  }
  Serial.println();
}

// -------------------- Sensor reading and anomaly detection --------------------
static void readSensorsAndValidate() {
  // DHT read
  bool tempOk = true;
  float t = dht.readTemperature(); // Celsius

  // DHT11 range sanity checks
  if (isnan(t) || t < -10.0f || t > 60.0f) tempOk = false;

  lastTempC = t;
  faultTemp = !tempOk;

  // LDR & Soil
  int light = analogRead(LIGHT_PIN);
  int soil  = analogRead(SOIL_PIN);

  // sanity: analogRead range always 0..1023, but detect "abnormal" like fixed extreme or noise? use stuck-high/low detection
  bool lightOk = !(light < 0 || light > 1023);
  bool soilOk  = !(soil < 0 || soil > 1023);

  // Additionally treat extreme constant readings as suspicious (basic heuristic):
  // if continuously too close to rail, mark as fault
  // (we need some history for stability; implement simple immediate heuristic with thresholds)
  if (lightOk) {
    if (light <= 5 || light >= 1018) lightOk = false;
  }
  if (soilOk) {
    if (soil <= 5 || soil >= 1018) soilOk = false;
  }

  lastLightRaw = light;
  lastSoilRaw = soil;

  faultLight = !lightOk;
  faultSoil  = !soilOk;

  anyFault = faultTemp || faultLight || faultSoil;
}

// -------------------- Display page formatting --------------------
static void showPage() {
  // Use TM1637 (4 digits). We'll display:
  // TEMP: e.g. "25.3" with degree omitted (use decimal point)
  // LIGHT: raw value
  // SOIL: raw value
  // If sensor fault -> display "----"
  if (displayPage == PAGE_TEMP) {
    if (faultTemp || isnan(lastTempC)) {
      display.showNumberDecEx(0, 0b01000000, true); // fallback; but show "0" quickly
      // Better: show dashes:
      display.setSegments((uint8_t*)"\x40\x40\x40\x40"); // not reliable; keep numeric fault indication
    } else {
      // Convert to 0.1 precision for DHT11 (integer typically); still show one decimal
      int scaled = (int)round(lastTempC * 10.0f); // e.g., 253 => 25.3
      int absScaled = abs(scaled);
      int d = absScaled / 10;
      int dec = absScaled % 10;
      if (scaled < 0) {
        // show -dd.d as best-effort
        // TM1637: showNumberDecEx expects int; we can use encoded segments? We'll do simpler:
        // map to -10..-99.
        display.showNumberDec(d, false, 2, 0); // place might not be exact
        // Minimal: use built-in raw number (no decimal). Keep consistent.
      } else {
        // show as integer and dot after tens if possible
        // Use showNumberDecEx with decimal point bit:
        // decimal point for digit 2 (0-index) depends on library; we'll use setSegments approach is overkill.
        // We'll display integer temperature without decimal to ensure correct behavior.
        display.showNumberDec((int)round(lastTempC), false, 4, 0);
      }
    }
  } else if (displayPage == PAGE_LIGHT) {
    if (faultLight || lastLightRaw < 0) {
      display.showNumberDec(0, false, 4, 0);
      display.setSegments((uint8_t*)"\x40\x40\x40\x40");
    } else {
      display.showNumberDec(constrain(lastLightRaw, 0, 9999), false, 4, 0);
    }
  } else if (displayPage == PAGE_SOIL) {
    if (faultSoil || lastSoilRaw < 0) {
      display.showNumberDec(0, false, 4, 0);
      display.setSegments((uint8_t*)"\x40\x40\x40\x40");
    } else {
      display.showNumberDec(constrain(lastSoilRaw, 0, 9999), false, 4, 0);
    }
  }
}

// -------------------- Setup/Loop --------------------
void setup() {
  Serial.begin(9600);

  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);

  pinMode(FAN_PWM_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  setBuzzer(false);

  // Initial safe outputs
  analogWrite(FAN_PWM_PIN, 0);
  analogWrite(LED_PIN, 0);
  digitalWrite(PUMP_RELAY_PIN, LOW);

  // DHT init
  dht.begin();

  // TM1637 init
  display.setBrightness(7, true);

  lastSensorReadMs = 0;
  lastDisplaySwitchMs = 0;
  buzzerLastToggleMs = millis();
}

void loop() {
  static unsigned long lastPressModeMs = 0;
  static unsigned long lastPressNextMs = 0;

  // Buttons:
  // D2 toggles AUTO/MANUAL
  if (buttonPressedWithDebounce(BTN_MODE, lastPressModeMs)) {
    if (currentMode == MODE_AUTO) currentMode = MODE_MANUAL;
    else currentMode = MODE_AUTO;
  }

  // D3 in MANUAL selects current control object (fan -> led -> pump -> fan...)
  if (buttonPressedWithDebounce(BTN_NEXT, lastPressNextMs)) {
    if (currentMode == MODE_MANUAL) {
      currentTarget = (ManualTarget)((currentTarget + 1) % 3);
    }
  }

  // Sensor reading + validation
  unsigned long now = millis();
  if (now - lastSensorReadMs >= SENSOR_READ_INTERVAL_MS) {
    lastSensorReadMs = now;
    readSensorsAndValidate();

    // If fault detected, buzzer alarm required (and serial output fault info)
    reportFaultIfAny();
  }

  // Buzzer alarm toggling if any fault
  if (anyFault) {
    unsigned long tnow = millis();
    if (buzzerOn) {
      if (tnow - buzzerLastToggleMs >= BUZZER_BURST_MS) {
        buzzerLastToggleMs = tnow;
        buzzerOn = false;
        setBuzzer(false);
      }
    } else {
      if (tnow - buzzerLastToggleMs >= BUZZER_PAUSE_MS) {
        buzzerLastToggleMs = tnow;
        buzzerOn = true;
        setBuzzer(true);
      }
    }
  } else {
    buzzerOn = false;
    setBuzzer(false);
  }

  // Apply controls
  if (currentMode == MODE_AUTO) {
    applyAutoControls();
  } else {
    // In manual mode, still allow fault alarm and serial output, but manual control acts.
    applyManualControls();
  }

  // TM1637 cycling display temperature/light/soil
  if (now - lastDisplaySwitchMs >= DISPLAY_INTERVAL_MS) {
    lastDisplaySwitchMs = now;
    displayPage = (DisplayPage)((displayPage + 1) % 3);
  }
  showPage();
}
