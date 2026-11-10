#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_023\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t TRIG_PIN = 8;
constexpr uint8_t ECHO_PIN = 2;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;
constexpr uint8_t NEAR_LED_PIN = 13;
constexpr uint8_t PWM_LED_PIN = 9;

// Distance thresholds and brightness mapping, in centimeters.
constexpr float MIN_DISTANCE_CM = 2.0f;
constexpr float MAX_DISTANCE_CM = 400.0f;
constexpr float NEAR_DISTANCE_CM = 20.0f;

// Adjust these values if distance calibration is required.
constexpr float DISTANCE_SCALE = 1.0f;
constexpr float DISTANCE_OFFSET_CM = 0.0f;

constexpr unsigned long MEASUREMENT_INTERVAL_MS = 70;
constexpr unsigned long ECHO_TIMEOUT_US = 30000;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

const uint8_t NO_READING_SEGMENTS[] = {
  SEG_G, SEG_G, SEG_G, SEG_G
};

unsigned long lastMeasurementMs = 0;

#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_023\\direct\\attempt_0\\sketch\\sketch.ino"
void showNoReading();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_023\\direct\\attempt_0\\sketch\\sketch.ino"
bool readDistanceCm(float &distanceCm);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_023\\direct\\attempt_0\\sketch\\sketch.ino"
void updateOutputs(float distanceCm);
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_023\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_023\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_023\\direct\\attempt_0\\sketch\\sketch.ino"
void showNoReading() {
  display.setSegments(NO_READING_SEGMENTS);
  digitalWrite(NEAR_LED_PIN, LOW);
  analogWrite(PWM_LED_PIN, 0);
}

bool readDistanceCm(float &distanceCm) {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long durationUs =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (durationUs == 0) {
    return false;
  }

  distanceCm =
      (durationUs * 0.0343f * 0.5f) * DISTANCE_SCALE
      + DISTANCE_OFFSET_CM;

  return distanceCm >= MIN_DISTANCE_CM &&
         distanceCm <= MAX_DISTANCE_CM;
}

void updateOutputs(float distanceCm) {
  const int displayedCm = static_cast<int>(distanceCm + 0.5f);
  display.showNumberDec(displayedCm, false);

  digitalWrite(NEAR_LED_PIN,
               distanceCm <= NEAR_DISTANCE_CM ? HIGH : LOW);

  float brightness =
      255.0f * (MAX_DISTANCE_CM - distanceCm) /
      (MAX_DISTANCE_CM - MIN_DISTANCE_CM);

  if (brightness < 0.0f) {
    brightness = 0.0f;
  } else if (brightness > 255.0f) {
    brightness = 255.0f;
  }

  analogWrite(PWM_LED_PIN, static_cast<int>(brightness + 0.5f));
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(NEAR_LED_PIN, OUTPUT);
  pinMode(PWM_LED_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);

  display.setBrightness(5, true);
  showNoReading();

  lastMeasurementMs = millis() - MEASUREMENT_INTERVAL_MS;
}

void loop() {
  const unsigned long now = millis();
  if (now - lastMeasurementMs < MEASUREMENT_INTERVAL_MS) {
    return;
  }
  lastMeasurementMs = now;

  float distanceCm;
  if (readDistanceCm(distanceCm)) {
    updateOutputs(distanceCm);
  } else {
    showNoReading();
  }
}
