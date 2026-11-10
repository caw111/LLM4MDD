#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

#define DHT_PIN 7
#define DHT_TYPE DHT11

#define TM1637_CLK_PIN 4
#define TM1637_DIO_PIN 5

static DHT dht(DHT_PIN, DHT_TYPE);
static TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

static const uint8_t LED_PIN = 13;

static const unsigned long DISPLAY_SWITCH_INTERVAL_MS = 2000UL;

static float lastTempC = NAN;
static float lastHumidity = NAN;

static unsigned long lastSwitchMs = 0;
static bool showTemperature = true;

static bool readAndValidate(float &outTempC, float &outHumidity) {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  if (isnan(h) || isnan(t)) return false;

  outTempC = t;
  outHumidity = h;
  return true;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);

  dht.begin();

  display.setBrightness(7, true);

  lastSwitchMs = millis();
  showTemperature = true;

  display.showNumberDec(0, false, 4, 0);
}

void loop() {
  unsigned long now = millis();

  float tC, h;
  bool ok = readAndValidate(tC, h);

  if (!ok) {
    static unsigned long lastErrMs = 0;
    static bool ledState = false;

    display.showNumberDec(0, false, 4, 0);

    if (now - lastErrMs >= 200UL) { // 200ms blink
      lastErrMs = now;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);

      Serial.println("sensor error");
    }
    return; // stay in error mode until valid read
  }

  digitalWrite(LED_PIN, LOW);

  if (now - lastSwitchMs >= DISPLAY_SWITCH_INTERVAL_MS) {
    lastSwitchMs = now;
    showTemperature = !showTemperature;
  }

  lastTempC = tC;
  lastHumidity = h;

  if (showTemperature) {
    int tempInt = (int)lround(lastTempC);
    if (tempInt < 0) tempInt = 0;
    display.showNumberDec(tempInt, false, 4, 0);
  } else {
    int humInt = (int)lround(lastHumidity);
    if (humInt < 0) humInt = 0;
    display.showNumberDec(humInt, false, 4, 0);
  }
}