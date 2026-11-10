#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// ----- Pin assignments (as required) -----
static const uint8_t PIN_LDR = A0;   // 光敏电阻 A0
static const uint8_t PIN_BTN = D2;   // 按钮 D2
static const uint8_t PIN_DHT = D7;    // DHT11 D7
static const uint8_t PIN_TM_CLK = D4; // TM1637 CLK = D4
static const uint8_t PIN_TM_DIO = D5; // TM1637 DIO = D5

// ----- Devices -----
static DHT dht(PIN_DHT, DHT11);
static TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);

// ----- System state -----
enum DisplayMode : uint8_t {
  MODE_LIGHT = 0,
  MODE_TEMP  = 1,
  MODE_HUMID  = 2
};

static DisplayMode mode = MODE_LIGHT;
static bool lastBtnStable = HIGH;
static bool lastBtnRead = HIGH;
static unsigned long lastDebounceMs = 0;
static const unsigned long debounceDelayMs = 35;

// Thresholds (for abnormal prompt)
static const int LIGHT_TOO_LOW_THRESHOLD = 300;  // ADC threshold (0-1023)
static const float TEMP_TOO_HIGH_THRESHOLD = 30.0; // °C

// Serial timing
static unsigned long lastSerialMs = 0;
static const unsigned long serialPeriodMs = 2000;

// ----- Helper functions -----
static int readLightADC() {
  return analogRead(PIN_LDR);
}

static float readTemperatureC() {
  float t = dht.readTemperature(); // default: Celsius
  if (isnan(t)) return NAN;
  return t;
}

static float readHumidityRH() {
  float h = dht.readHumidity();
  if (isnan(h)) return NAN;
}

static void showOnTM1637(float value, bool isTempOrHum) {
  // TM1637Display has showNumberDecEx. We'll scale to keep 1 decimal digit.
  // If value is NaN, show dashes.
  if (isnan(value)) {
    tm1637.showNumberDecEx(0, 0b01000000, true); // show "----" style (approx)
    return;
  }

  // Clamp to reasonable display range (-99.9..199.9)
  if (value > 199.9) value = 199.9;
  if (value < -99.9) value = -99.9;

  bool negative = value < 0;
  float absV = negative ? -value : value;

  // Scale by 10 for one decimal place.
  int scaled = (int)round(absV * 10.0f); // e.g., 23.4 -> 234
  int displayInt = scaled;

  // tm1637 segments: showNumberDecEx expects int without decimals handling by bitmask.
  // We'll show with decimal point at correct position.
  // Choose decimal point bit for 1 decimal place:
  // For 4 digits: dp position bitmask typically 0b01000000 = show dot at digit 2 (implementation-specific).
  // We'll use a common mapping for library:
  // bit0 corresponds to digit0 (rightmost), etc. Many versions:
  // showNumberDecEx uses 0b01000000 for middle dot. We'll choose 0b01000000.
  uint8_t dots = 0b01000000;

  // For temperature/humidity display: show one decimal.
  // For light (ADC) display: show integer without decimal; handle separately in caller.
  (void)isTempOrHum;

  // If negative, tm1637 showNumberDecEx supports minus sign on leftmost digit.
  // We'll pass displayInt as signed with negative.
  if (negative) displayInt = -displayInt;

  tm1637.showNumberDecEx(displayInt, dots, true);
}

static void showLightADCOnTM1637(int adc) {
  // Display ADC as integer (up to 1023). Use clamping to 4 digits.
  if (adc < 0) adc = 0;
  if (adc > 9999) adc = 9999;
  tm1637.showNumberDec(adc, false);
}

static bool readButtonEdge() {
  // Debounced falling edge (button press assumed active LOW with pullup).
  bool reading = digitalRead(PIN_BTN);

  if (reading != lastBtnRead) {
    lastDebounceMs = millis();
    lastBtnRead = reading;
  }

  if (millis() - lastDebounceMs > debounceDelayMs) {
    if (reading != lastBtnStable) {
      lastBtnStable = reading;
      // active press: transition to LOW
      if (lastBtnStable == LOW) return true;
    }
  }
  return false;
}

// ----- Arduino setup/loop -----
void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP);

  analogReference(DEFAULT);
  dht.begin();

  tm1637.setBrightness(0x0f);

  // Initialize state
  lastBtnStable = digitalRead(PIN_BTN);
  lastBtnRead = lastBtnStable;

  // Initial display
  int light = readLightADC();
  showLightADCOnTM1637(light);

  Serial.begin(9600);
}

void loop() {
  // 1) Button: each press cycles display mode
  if (readButtonEdge()) {
    mode = (DisplayMode)(((uint8_t)mode + 1) % 3);

    // Update TM1637 immediately upon mode change
    if (mode == MODE_LIGHT) {
      showLightADCOnTM1637(readLightADC());
    } else if (mode == MODE_TEMP) {
      float t = readTemperatureC();
      showOnTM1637(t, true);
    } else {
      float h = readHumidityRH();
      showOnTM1637(h, true);
    }
  }

  // 2) Periodic serial output every 2 seconds:
  // Output three complete data items, and abnormal prompt when light too low and temp too high.
  unsigned long now = millis();
  if (now - lastSerialMs >= serialPeriodMs) {
    lastSerialMs = now;

    int lightADC = readLightADC();
    float tempC = readTemperatureC();
    float humRH = readHumidityRH();

    // Format: LightADC,TempC,HumRH (one line per 2 seconds)
    Serial.print("LightADC=");
    Serial.print(lightADC);

    Serial.print(", TempC=");
    if (isnan(tempC)) Serial.print("NaN");
    else Serial.print(tempC, 1);

    Serial.print(", HumRH=");
    if (isnan(humRH)) Serial.print("NaN");
    else Serial.print(humRH, 1);

    Serial.println();

    // Abnormal prompt condition: 光照过低且温度过高
    bool abnormal = (lightADC < LIGHT_TOO_LOW_THRESHOLD) && (!isnan(tempC)) && (tempC > TEMP_TOO_HIGH_THRESHOLD);

    if (abnormal) {
      Serial.println("环境异常提示：光照过低且温度过高");
    }

    // Keep TM1637 updated continuously for the selected mode
    if (mode == MODE_LIGHT) {
      showLightADCOnTM1637(lightADC);
    } else if (mode == MODE_TEMP) {
      showOnTM1637(tempC, true);
    } else {
      showOnTM1637(humRH, true);
    }
  }
}