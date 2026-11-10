#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
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

#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readAndValidate(float &outTempC, float &outHumidity);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_021\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readAndValidate(float &outTempC, float &outHumidity) {
  // DHT11 library reads as integer-like values but returns float.
  // Validate against NaN (common failure indicator).
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

  // TM1637 display setup
  display.setBrightness(7, true); // 0..7 typical; set to 7. auto on affects some variants.

  // Ensure initial state
  lastSwitchMs = millis();
  showTemperature = true;

  // Initial clear
  display.showNumberDec(0, false, 4, 0);
}

void loop() {
  unsigned long now = millis();

  // Attempt read each cycle (still preserves required behavior: on failure, show 0000 + LED blink + serial error)
  float tC, h;
  bool ok = readAndValidate(tC, h);

  if (!ok) {
    // Error mode: show 0000, LED blink, serial output sensor error.
    // Output at an interval to avoid flooding; still outputs error for failure.
    static unsigned long lastErrMs = 0;
    static bool ledState = false;

    // Display 0000 continuously
    display.showNumberDec(0, false, 4, 0);

    // Blink LED D13
    if (now - lastErrMs >= 200UL) { // 200ms blink
      lastErrMs = now;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW;

      // Serial output on each blink toggle (still "sensor error" on read failure).
      Serial.println("sensor error");
    }

    return; // keep error mode active until a valid read occurs
  }

  // Valid read: LED off and normal alternating display temperature/humidity
  digitalWrite(LED_PIN, LOW);

  if (now - lastSwitchMs >= DISPLAY_SWITCH_INTERVAL_MS) {
    lastSwitchMs = now;
    showTemperature = !showTemperature;
  }

  lastTempC = tC;
  lastHumidity = h;

  // TM1637 requirement: "normal time alternately display temperature and humidity".
  // Display as 4 digits without decimal (DHT11 is integer values typically).
  // Temperature: show temperature as integer (rounded)
  // Humidity: show humidity as integer (rounded)
  if (showTemperature) {
    int tempInt = (int)lround(lastTempC);
    // Ensure non-negative display; DHT11 typically 0..50
    if (tempInt < 0) tempInt = 0;
    display.showNumberDec(tempInt, false, 4, 0);
  } else {
    int humInt = (int)lround(lastHumidity);
    if (humInt < 0) humInt = 0;
    display.showNumberDec(humInt, false, 4, 0);
  }
}
