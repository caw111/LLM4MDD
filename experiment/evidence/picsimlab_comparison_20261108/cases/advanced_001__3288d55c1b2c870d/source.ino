#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

#define DHTPIN 7
#define DHTTYPE DHT11

// Analog inputs
#define LIGHT_PIN A0
#define SOIL_PIN  A1
#define KNOB_PIN  A2

// PWM / digital outputs
#define FAN_PWM_PIN       5   // D5
#define LED_PIN           9   // D9 (PWM-capable on UNO)
#define PUMP_RELAY_PIN    10  // D10 (relay)

// Alarm buzzer
#define BUZZER_PIN        6   // D6

// Buttons
#define BTN_MODE         2   // D2
#define BTN_NEXT         3   // D3

// TM1637
#define TM1637_CLK       4   // D4
#define TM1637_DIO       8   // D8

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
static const float TEMP_HIGH = 30.0f;     // turn fan ON
static const float TEMP_LOW  = 28.0f;     // turn fan OFF

static const int LIGHT_DARK = 400;         // below -> dark, enable LED
static const int LIGHT_BRIGHT = 550;      // above -> bright, disable LED

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

// Forward declarations
static void showPage();
static void reportFaultIfAny();

// -------------------- Buzzer helpers --------------------
static void setBuzzer(bool on) {
  if (on) tone(BUZZER_PIN, 2000);
  else noTone(BUZZER_PIN);
}

// -------------------- Button helpers --------------------
static bool readButtonRaw(uint8_t pin) {
  return (digitalRead(pin) == LOW);
}

static bool buttonPressedWithDebounce(uint8_t pin, unsigned long &lastPressMs) {
  bool pressed = readButtonRaw(pin);
  if (!pressed) return false;

  unsigned long now = millis();
  if (now - lastPressMs >= BUTTON_DEBOUNCE_MS) {
    lastPressMs = now;

    // Wait until released to avoid repeats
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
  return (uint8_t)map(v, 0, 1023, MANUAL_PWM_MIN, MANUAL_PWM_MAX);
}

static void applyAutoControls() {
  // Fan control with hysteresis
  if (!isnan(lastTempC)) {
    if (lastTempC >= TEMP_HIGH) {
      analogWrite(FAN_PWM_PIN, 255);
    } else if (lastTempC <= TEMP_LOW) {
      analogWrite(FAN_PWM_PIN, 0);
    }
  }

  // LED control with hysteresis
  if (lastLightRaw >= 0) {
    static int ledState = 0;
    if (lastLightRaw <= LIGHT_DARK) {
      ledState = 1;
      digitalWrite(LED_PIN, HIGH);
    } else if (lastLightRaw >= LIGHT_BRIGHT) {
      ledState = 0;
      digitalWrite(LED_PIN, LOW);
    }
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
  }
}

static void applyManualControls() {
  uint8_t p = knobPWM();

  // Manual target selection:
  // - Fan: PWM with p
  // - LED: use p with PWM
  // - Pump: relay threshold at 128
  switch (currentTarget) {
    case TARGET_FAN:
      analogWrite(FAN_PWM_PIN, p);
      break;
    case TARGET_LED:
      analogWrite(LED_PIN, p);
      break;
    case TARGET_PUMP:
      digitalWrite(PUMP_RELAY_PIN, (p >= 128) ? HIGH : LOW);
      break;
  }
}

// -------------------- Sensor reading and validation --------------------
static void readSensorsAndValidate() {
  // DHT11
  float t = dht.readTemperature(); // Celsius
  bool tempOk = !(isnan(t) || t < -10.0f || t > 60.0f);

  lastTempC = t;
  faultTemp = !tempOk;

  // LDR & Soil
  int light = analogRead(LIGHT_PIN);
  int soil  = analogRead(SOIL_PIN);

  bool lightOk = !(light < 0 || light > 1023);
  bool soilOk  = !(soil  < 0 || soil  > 1023);

  // Basic stuck extreme heuristic
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

// -------------------- Fault reporting --------------------
static void reportFaultIfAny() {
  if (!anyFault) return;

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

// -------------------- Display --------------------
static void showPage() {
  // Display behavior kept consistent with original sketch intent:
  // - Temperature: show integer temperature (no decimal to avoid library segment handling issues)
  // - Light/Soil: show raw integer
  // - If fault, show zeros (placeholder behavior as in original)
  if (displayPage == PAGE_TEMP) {
    if (faultTemp || isnan(lastTempC)) {
      display.showNumberDec(0, false, 4, 0);
      display.setSegments((uint8_t*)"\x40\x40\x40\x40");
    } else {
      display.showNumberDec((int)round(lastTempC), false, 4, 0);
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

// -------------------- Arduino setup/loop --------------------
void setup() {
  Serial.begin(9600);

  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);

  pinMode(FAN_PWM_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  setBuzzer(false);

  // Safe initial outputs
  analogWrite(FAN_PWM_PIN, 0);
  analogWrite(LED_PIN, 0);
  digitalWrite(PUMP_RELAY_PIN, LOW);

  dht.begin();
  display.setBrightness(7, true);

  lastSensorReadMs = 0;
  lastDisplaySwitchMs = 0;
  buzzerLastToggleMs = millis();
}

void loop() {
  static unsigned long lastPressModeMs = 0;
  static unsigned long lastPressNextMs = 0;

  // D2: toggle auto/manual
  if (buttonPressedWithDebounce(BTN_MODE, lastPressModeMs)) {
    currentMode = (currentMode == MODE_AUTO) ? MODE_MANUAL : MODE_AUTO;
  }

  // D3: in manual, cycle target fan -> led -> pump -> fan...
  if (buttonPressedWithDebounce(BTN_NEXT, lastPressNextMs)) {
    if (currentMode == MODE_MANUAL) {
      currentTarget = (ManualTarget)((currentTarget + 1) % 3);
    }
  }

  unsigned long now = millis();

  // Periodic sensors
  if (now - lastSensorReadMs >= SENSOR_READ_INTERVAL_MS) {
    lastSensorReadMs = now;
    readSensorsAndValidate();
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
  if (currentMode == MODE_AUTO) applyAutoControls();
  else applyManualControls();

  // TM1637 cycling display: temperature -> light -> soil
  if (now - lastDisplaySwitchMs >= DISPLAY_INTERVAL_MS) {
    lastDisplaySwitchMs = now;
    displayPage = (DisplayPage)((displayPage + 1) % 3);
  }
  showPage();
}