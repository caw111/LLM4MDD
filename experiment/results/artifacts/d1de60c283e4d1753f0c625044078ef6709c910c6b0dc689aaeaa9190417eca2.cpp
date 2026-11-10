#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <DHT.h>

#define BTN_PIN 2

#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define TRIG_PIN 8
#define ECHO_PIN 9

// Simple debounced toggle state
static bool showDistance = false; // false: Temperature, true: Distance
static int lastBtnReading = HIGH;
static int stableBtnReading = HIGH;
static unsigned long lastDebounceTime = 0;
const unsigned long debounceDelayMs = 50;

// Button edge detect
static bool lastStableBtnReading = HIGH;

#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static long readDistanceCm();
#line 66 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  dht.begin();

  Serial.begin(9600);
}

static float readTemperatureC() {
  float h = dht.readHumidity();
  (void)h; // humidity not displayed by requirement, but keep reading for DHT11 stability

  float t = dht.readTemperature(); // Celsius
  // If reading fails, DHT returns NaN; keep behavior stable by retry once.
  if (isnan(t)) {
    delay(20);
    t = dht.readTemperature();
  }
  return t;
}

static long readDistanceCm() {
  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width with timeout
  // timeout in microseconds: ~38ms for 6.5m; adjust to reasonable.
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 38000UL);
  if (duration == 0) return -1; // timeout / no echo

  // Speed of sound: 343 m/s => 0.0343 cm/us
  // distance = (duration * 0.0343) / 2
  long distCm = (long)((duration * 0.0343) / 2.0);
  return distCm;
}

void loop() {
  // --- Button debounced toggle ---
  int reading = digitalRead(BTN_PIN);

  if (reading != lastBtnReading) {
    lastDebounceTime = millis();
    lastBtnReading = reading;
  }

  if ((millis() - lastDebounceTime) >= debounceDelayMs) {
    stableBtnReading = reading;
  }

  // Detect falling edge (because INPUT_PULLUP: pressed -> LOW)
  if (lastStableBtnReading == HIGH && stableBtnReading == LOW) {
    showDistance = !showDistance;
  }
  lastStableBtnReading = stableBtnReading;

  // --- Display output over Serial (since no display hardware specified) ---
  // Preserve behavior: switch between temperature and distance based on button.
  if (!showDistance) {
    float tempC = readTemperatureC();
    if (isnan(tempC)) {
      Serial.println("Temperature: NaN");
    } else {
      Serial.print("Temperature: ");
      Serial.print(tempC, 1);
      Serial.println(" C");
    }
  } else {
    long distCm = readDistanceCm();
    if (distCm < 0) {
      Serial.println("Distance: out of range");
    } else {
      Serial.print("Distance: ");
      Serial.print(distCm);
      Serial.println(" cm");
    }
  }

  delay(250);
}
