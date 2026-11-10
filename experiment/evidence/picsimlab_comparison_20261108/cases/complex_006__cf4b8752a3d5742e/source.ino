#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// --- Pin assignments (fixed: use Arduino numeric pin constants) ---
static const uint8_t DHT_PIN  = 7;   // D7  -> Arduino digital 7
static const uint8_t LED_PIN  = 13;  // D13 -> Arduino digital 13
static const uint8_t BUZZ_PIN = 6;   // D6  -> Arduino digital 6
static const uint8_t TM1637_CLK = 4;  // D4  -> Arduino digital 4
static const uint8_t TM1637_DIO = 5;  // D5  -> Arduino digital 5
static const uint8_t POT_PIN  = A0;  // A0  -> potentiometer

// --- Constants / behavior ---
static const uint8_t DHT_TYPE = DHT11;

// Potentiometer mapping to temperature threshold (0~50°C)
static const int THRESH_MIN_C = 0;
static const int THRESH_MAX_C = 50;

static const unsigned long READ_INTERVAL_MS = 1000;

// Buzzer: periodic alarm when temp > threshold
static const unsigned int BUZZ_FREQ_HZ = 2000;
static const unsigned long BUZZ_ON_MS  = 300;
static const unsigned long BUZZ_OFF_MS = 700;

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
  int raw = analogRead(POT_PIN); // 0..1023
  long mapped = map(raw, 0, 1023, THRESH_MIN_C, THRESH_MAX_C);
  if (mapped < THRESH_MIN_C) mapped = THRESH_MIN_C;
  if (mapped > THRESH_MAX_C) mapped = THRESH_MAX_C;
  return (int)mapped;
}

static void updateBuzzerAndLed(bool alarm) {
  // LED: constant ON when alarm, OFF otherwise
  digitalWrite(LED_PIN, alarm ? HIGH : LOW);

  if (!alarm) {
    noTone(BUZZ_PIN);
    buzzerStateOn = false;
    return;
  }

  unsigned long now = millis();
  unsigned long period = buzzerStateOn ? BUZZ_ON_MS : BUZZ_OFF_MS;

  if (now - lastBuzzerToggleMs >= period) {
    lastBuzzerToggleMs = now;
    buzzerStateOn = !buzzerStateOn;

    if (buzzerStateOn) tone(BUZZ_PIN, BUZZ_FREQ_HZ);
    else noTone(BUZZ_PIN);
  }
}

static void displayTemps(int tempC, int threshC, bool alarm) {
  // Alternate display between current temperature and threshold
  static bool showTemp = true;
  static unsigned long lastSwitchMs = 0;

  unsigned long now = millis();
  if (now - lastSwitchMs >= DISPLAY_INTERVAL_MS) {
    lastSwitchMs = now;
    showTemp = !showTemp;
  }

  display.setBrightness(alarm ? 7 : 4);

  if (showTemp) {
    // Show current temperature
    display.showNumberDec(tempC, false, 4, 0);
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

  thresholdC = readThresholdC();

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

    // Threshold continuously adjustable
    thresholdC = readThresholdC();

    // Read temperature from DHT11 (Celsius)
    float t = dht.readTemperature();
    if (!isnan(t)) lastTempC = t;

    int tempC_int = isnan(lastTempC) ? 0 : (int)round(lastTempC);
    bool alarm = (tempC_int > thresholdC);

    updateBuzzerAndLed(alarm);

    if (now - lastDisplayMs >= 10) {
      lastDisplayMs = now;
      displayTemps(tempC_int, thresholdC, alarm);
    }
  } else {
    // Keep buzzer timing running even between sensor reads
    int tempC_int = isnan(lastTempC) ? 0 : (int)round(lastTempC);
    bool alarm = (tempC_int > thresholdC);

    updateBuzzerAndLed(alarm);

    if (now - lastDisplayMs >= 10) {
      lastDisplayMs = now;
      displayTemps(tempC_int, thresholdC, alarm);
    }
  }
}