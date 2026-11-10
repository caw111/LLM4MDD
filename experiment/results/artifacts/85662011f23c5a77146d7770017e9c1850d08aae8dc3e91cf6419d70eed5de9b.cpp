#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_007\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t TRIG_PIN = 8;
constexpr uint8_t ECHO_PIN = 9;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;

constexpr unsigned long SAMPLE_INTERVAL_MS = 100;
constexpr unsigned long ECHO_TIMEOUT_US = 25000UL;
constexpr unsigned long MIN_ECHO_US = 2UL * 58UL;
constexpr unsigned long MAX_ECHO_US = 400UL * 58UL;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

struct DistanceReading {
  bool valid;
  uint16_t centimeters;
};

unsigned long lastSampleStartMs = 0;

#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_007\\direct\\attempt_0\\sketch\\sketch.ino"
DistanceReading measureDistance();
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_007\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_007\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_007\\direct\\attempt_0\\sketch\\sketch.ino"
DistanceReading measureDistance() {
  DistanceReading reading = {false, 0};

  // A high input before triggering indicates a stale or stuck echo.
  const bool echoInitiallyHigh = (digitalRead(ECHO_PIN) == HIGH);

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Bound the entire echo wait, including stale-high and no-echo cases.
  const unsigned long echoUs = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (echoInitiallyHigh || echoUs == 0 ||
      echoUs < MIN_ECHO_US || echoUs > MAX_ECHO_US) {
    return reading;
  }

  reading.valid = true;
  reading.centimeters = static_cast<uint16_t>((echoUs + 29UL) / 58UL);
  return reading;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);

  display.setBrightness(7, true);
  display.clear();

  // Allow the first measurement to start immediately.
  lastSampleStartMs = millis() - SAMPLE_INTERVAL_MS;
}

void loop() {
  const unsigned long nowMs = millis();

  if (nowMs - lastSampleStartMs < SAMPLE_INTERVAL_MS) {
    return;
  }

  lastSampleStartMs = nowMs;
  const DistanceReading reading = measureDistance();

  if (reading.valid) {
    display.showNumberDec(reading.centimeters, false);
  } else {
    display.clear();
  }
}
