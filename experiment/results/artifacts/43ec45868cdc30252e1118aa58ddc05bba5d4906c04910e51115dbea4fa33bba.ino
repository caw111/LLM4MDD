#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <math.h>

const uint8_t DHT_PIN = 7;
const uint8_t POT_PIN = A0;
const uint8_t DISPLAY_CLK_PIN = 4;
const uint8_t DISPLAY_DIO_PIN = 5;
const uint8_t BUZZER_PIN = 6;
const uint8_t LED_PIN = 13;

const int MIN_THRESHOLD_C = 0;
const int MAX_THRESHOLD_C = 50;

const unsigned long DHT_INTERVAL_MS = 2000;
const unsigned long POT_INTERVAL_MS = 50;
const unsigned long DISPLAY_INTERVAL_MS = 100;
const unsigned long ALARM_PERIOD_MS = 1000;
const unsigned long ALARM_ON_MS = 250;
const unsigned int ALARM_FREQUENCY_HZ = 2000;

DHT dht(DHT_PIN, DHT11);
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

float temperatureC = NAN;
int thresholdC = MIN_THRESHOLD_C;
bool temperatureValid = false;
bool alarmActive = false;
bool buzzerActive = false;

unsigned long lastDhtReadMs = 0;
unsigned long lastPotReadMs = 0;
unsigned long lastDisplayMs = 0;
unsigned long alarmCycleStartMs = 0;

int readThreshold()
{
  return (int)map(analogRead(POT_PIN), 0, 1023,
                  MIN_THRESHOLD_C, MAX_THRESHOLD_C);
}

void updateDisplay()
{
  // Left two digits: current temperature; right two: threshold (degrees C).
  // The center colon separates them. "--" indicates unavailable sensor data.
  uint8_t segments[4];

  if (temperatureValid) {
    int shownTemperature = constrain((int)round(temperatureC), 0, 99);
    segments[0] = display.encodeDigit(shownTemperature / 10);
    segments[1] = display.encodeDigit(shownTemperature % 10);
  } else {
    segments[0] = 0x40;
    segments[1] = 0x40;
  }

  segments[1] |= 0x80;
  segments[2] = display.encodeDigit(thresholdC / 10);
  segments[3] = display.encodeDigit(thresholdC % 10);
  display.setSegments(segments);
}

void updateAlarm(unsigned long now)
{
  // Compare the actual measured temperature, not the rounded display value.
  const bool shouldAlarm = temperatureValid && temperatureC > thresholdC;

  if (shouldAlarm && !alarmActive) {
    alarmCycleStartMs = now;
  }
  alarmActive = shouldAlarm;
  digitalWrite(LED_PIN, alarmActive ? HIGH : LOW);

  bool shouldSound = false;
  if (alarmActive) {
    unsigned long elapsed = now - alarmCycleStartMs;
    if (elapsed >= ALARM_PERIOD_MS) {
      alarmCycleStartMs +=
          (elapsed / ALARM_PERIOD_MS) * ALARM_PERIOD_MS;
      elapsed = now - alarmCycleStartMs;
    }
    shouldSound = elapsed < ALARM_ON_MS;
  }

  if (shouldSound != buzzerActive) {
    buzzerActive = shouldSound;
    if (buzzerActive) {
      tone(BUZZER_PIN, ALARM_FREQUENCY_HZ);
    } else {
      noTone(BUZZER_PIN);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

void setup()
{
  pinMode(POT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  noTone(BUZZER_PIN);

  dht.begin();
  display.setBrightness(5, true);
  display.clear();

  thresholdC = readThreshold();
  updateDisplay();

  // Allow the DHT11 to stabilize before the first measurement.
  const unsigned long now = millis();
  lastDhtReadMs = now;
  lastPotReadMs = now;
  lastDisplayMs = now;
}

void loop()
{
  unsigned long now = millis();

  if (now - lastPotReadMs >= POT_INTERVAL_MS) {
    lastPotReadMs = now;
    thresholdC = readThreshold();
  }

  if (now - lastDhtReadMs >= DHT_INTERVAL_MS) {
    lastDhtReadMs = now;
    const float reading = dht.readTemperature();
    temperatureValid = !isnan(reading);
    temperatureC = temperatureValid ? reading : NAN;
  }

  now = millis();
  updateAlarm(now);

  if (now - lastDisplayMs >= DISPLAY_INTERVAL_MS) {
    lastDisplayMs = now;
    updateDisplay();
  }
}