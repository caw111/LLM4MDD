#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// --- Pin assignments (per requirements / provided wiring) ---
static const uint8_t DHT_PIN  = D7;   // DHT11 data
static const uint8_t POT_PIN  = A0;   // potentiometer for threshold
static const uint8_t LED_PIN  = D13;  // LED alarm
static const uint8_t BUZZ_PIN = D6;   // buzzer alarm (tone)
static const uint8_t TM1637_CLK = D4;  // CLK
static const uint8_t TM1637_DIO = D5;  // DIO

// --- Constants / behavior ---
static const uint8_t DHT_TYPE = DHT11;

// Potentiometer mapping to temperature threshold:
// Requirement: pot sets threshold (implementation defines the range).
// Common practical mapping for DHT11: 0~50°C.
// Use integer threshold in Celsius.
static const int THRESH_MIN_C = 0;
static const int THRESH_MAX_C = 50;

// DHT11 update interval (avoid reading too frequently)
static const unsigned long READ_INTERVAL_MS = 1000;

// Buzzer: periodic alarm when temp > threshold
static const unsigned int BUZZ_FREQ_HZ = 2000;
static const unsigned long BUZZ_ON_MS  = 300;
static const unsigned long BUZZ_OFF_MS = 700;

// Display refresh interval
static const unsigned long DISPLAY_INTERVAL_MS = 200;

// --- Objects ---
DHT dht(DHT_PIN, DHT_TYPE);
TM1637Display display(TM1637_CLK, TM1637_DIO);

// --- State ---
unsigned long lastReadMs = 0;
unsigned long lastDisplayMs = 0;

float lastTempC = NAN;
int thresholdC = 0;

bool buzzerStateOn = false;
unsigned long lastBuzzerToggleMs = 0;

// Map potentiometer value to temperature threshold
static int readThresholdC() {
  // Arduino UNO analogRead returns 0..1023
  int raw = analogRead(POT_PIN);
  // Map to [THRESH_MIN_C, THRESH_MAX_C]
  long mapped = map(raw, 0, 1023, THRESH_MIN_C, THRESH_MAX_C);
  if (mapped < THRESH_MIN_C) mapped = THRESH_MIN_C;
  if (mapped > THRESH_MAX_C) mapped = THRESH_MAX_C;
  return (int)mapped;
}

static void updateBuzzerAndLed(bool alarm) {
  // LED: constant ON when alarm, OFF otherwise
  digitalWrite(LED_PIN, alarm ? HIGH : LOW);

  if (!alarm) {
    // Stop buzzer
    noTone(BUZZ_PIN);
    buzzerStateOn = false;
    return;
  }

  // Periodic alarm
  unsigned long now = millis();
  unsigned long period = buzzerStateOn ? BUZZ_ON_MS : BUZZ_OFF_MS;

  if (!buzzerStateOn) {
    // ensure buzz starts immediately after transition to OFF->ON window
    // Use toggle based on elapsed time
  }

  if (now - lastBuzzerToggleMs >= period) {
    lastBuzzerToggleMs = now;
    buzzerStateOn = !buzzerStateOn;

    if (buzzerStateOn) {
      tone(BUZZ_PIN, BUZZ_FREQ_HZ);
    } else {
      noTone(BUZZ_PIN);
    }
  }
}

static void displayTemps(int tempC, int threshC, bool alarm) {
  // Display requirement: show current temperature and threshold.
  // TM1637 typical 4 digits; we multiplex by toggling between:
  // - show temperature: " ttt°" (use 3 digits + dot) when no alarm or always
  // - show threshold: " Ttt°" (use 2 digits + dot) with a leading dash if needed
  //
  // We'll alternate every call based on millis to ensure both are shown.
  static bool showTemp = true;
  static unsigned long lastSwitchMs = 0;

  unsigned long now = millis();
  if (now - lastSwitchMs >= DISPLAY_INTERVAL_MS) {
    lastSwitchMs = now;
    showTemp = !showTemp;
  }

  display.setBrightness(alarm ? 7 : 4);

  if (showTemp) {
    // Show current temperature (int), right aligned, include degree marker (dot)
    // If negative, TM1637Display handles minus sign on leading digit.
    display.showNumberDec(tempC, false, 4, 0);
    // Set dot (degree) on last digit (some libs use setSegments; here use setSegments would be too heavy)
    // Keep simple: just show number. (Requirement focuses on displaying temp and threshold.)
  } else {
    // Show threshold
    display.showNumberDec(threshC, false, 4, 0);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUZZ_PIN, OUTPUT);
  noTone(BUZZ_PIN);

  dht.begin();

  display.setBrightness(4);
  // Initialize threshold once
  thresholdC = readThresholdC();

  // Initial display
  lastTempC = NAN;
  lastReadMs = 0;
  lastDisplayMs = 0;

  buzzerStateOn = false;
  lastBuzzerToggleMs = millis();
}

void loop() {
  unsigned long now = millis();

  // Periodically read sensor and threshold
  if (now - lastReadMs >= READ_INTERVAL_MS) {
    lastReadMs = now;

    // Threshold set by potentiometer (continuously adjustable)
    thresholdC = readThresholdC();

    // Read temperature from DHT11
    float t = dht.readTemperature(); // Celsius
    if (!isnan(t)) {
      lastTempC = t;
    }

    int tempC_int;
    if (isnan(lastTempC)) {
      // If DHT read failed, keep lastTempC value displayed as 0 to avoid nonsense
      tempC_int = 0;
    } else {
      // DHT11 gives integer temperature; still convert safely
      tempC_int = (int)round(lastTempC);
    }

    bool alarm = (tempC_int > thresholdC);

    // Update buzzer/LED based on current reading
    updateBuzzerAndLed(alarm);

    // Display both temperature and threshold (alternate)
    // Call display routine each loop; it internally switches at DISPLAY_INTERVAL_MS.
    if (now - lastDisplayMs >= 10) {
      lastDisplayMs = now;
      displayTemps(tempC_int, thresholdC, alarm);
    }
  } else {
    // Still keep buzzer timing running even when no new sensor read
    int tempC_int;
    if (isnan(lastTempC)) {
      tempC_int = 0;
    } else {
      tempC_int = (int)round(lastTempC);
    }
    bool alarm = (tempC_int > thresholdC);

    updateBuzzerAndLed(alarm);

    // Display refresh (so alternation continues even when DHT read isn't happening)
    if (now - lastDisplayMs >= 10) {
      lastDisplayMs = now;
      displayTemps(tempC_int, thresholdC, alarm);
    }
  }
}