#include <Arduino.h>

constexpr uint8_t TRIG_PIN = 8;
constexpr uint8_t ECHO_PIN = 9;
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t LED_PIN = 13;

constexpr unsigned long SERIAL_BAUD = 9600;
constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long MEASUREMENT_INTERVAL_MS = 100;
constexpr unsigned long ECHO_TIMEOUT_US = 25000;
constexpr unsigned long BEEP_DURATION_MS = 50;
constexpr unsigned int BUZZER_FREQUENCY_HZ = 2000;

bool muted = false;
bool distanceValid = false;
bool buzzerActive = false;
bool beepScheduled = false;

bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;

unsigned long buttonChangedAt = 0;
unsigned long lastMeasurementAt = 0;
unsigned long lastBeepStartedAt = 0;
unsigned long beepIntervalMs = 1200;

float distanceCm = 0.0f;

void stopBuzzer() {
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
  buzzerActive = false;
}

void updateButton() {
  const unsigned long now = millis();
  const bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastButtonReading = reading;
    buttonChangedAt = now;
  }

  if (reading != stableButtonState &&
      now - buttonChangedAt >= DEBOUNCE_MS) {
    stableButtonState = reading;

    // 按钮接在 D2 与 GND 之间，每次按下切换静音状态。
    if (stableButtonState == LOW) {
      muted = !muted;
      digitalWrite(LED_PIN, muted ? HIGH : LOW);
      beepScheduled = false;

      if (muted) {
        stopBuzzer();
      }
    }
  }
}

void measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long echoDuration =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  const bool previouslyValid = distanceValid;
  distanceValid = (echoDuration != 0);

  if (distanceValid) {
    distanceCm = echoDuration * 0.0343f / 2.0f;

    // 距离越近，提示越密集：
    // <= 5 cm：每 100 ms 一次；>= 200 cm：每 1200 ms 一次。
    float limitedDistance = distanceCm;
    if (limitedDistance < 5.0f) {
      limitedDistance = 5.0f;
    } else if (limitedDistance > 200.0f) {
      limitedDistance = 200.0f;
    }

    beepIntervalMs = 100UL +
        (unsigned long)((limitedDistance - 5.0f) * 1100.0f / 195.0f);

    if (!previouslyValid) {
      beepScheduled = false;
    }

    Serial.print(F("Distance: "));
    Serial.print(distanceCm, 1);
    Serial.print(F(" cm"));
  } else {
    stopBuzzer();
    beepScheduled = false;
    Serial.print(F("Distance: no echo"));
  }

  // 每条信息以换行结束；静音不影响测距及串口输出。
  Serial.print(F(" | Muted: "));
  Serial.println(muted ? F("YES") : F("NO"));
}

void updateBuzzer() {
  const unsigned long now = millis();

  if (muted || !distanceValid) {
    if (buzzerActive) {
      stopBuzzer();
    }
    beepScheduled = false;
    return;
  }

  if (buzzerActive &&
      now - lastBeepStartedAt >= BEEP_DURATION_MS) {
    stopBuzzer();
  }

  if (!buzzerActive &&
      (!beepScheduled || now - lastBeepStartedAt >= beepIntervalMs)) {
    tone(BUZZER_PIN, BUZZER_FREQUENCY_HZ, BEEP_DURATION_MS);
    buzzerActive = true;
    beepScheduled = true;
    lastBeepStartedAt = now;
  }
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  stopBuzzer();

  Serial.begin(SERIAL_BAUD);

  lastButtonReading = digitalRead(BUTTON_PIN);
  buttonChangedAt = millis();

  // 上电默认未静音，并立即开始第一次测距。
  lastMeasurementAt = millis() - MEASUREMENT_INTERVAL_MS;
}

void loop() {
  updateButton();
  updateBuzzer();

  const unsigned long now = millis();
  if (now - lastMeasurementAt >= MEASUREMENT_INTERVAL_MS) {
    lastMeasurementAt = now;
    measureDistance();

    // 测距有超时上限；完成后立即继续处理按钮和提示。
    updateButton();
    updateBuzzer();
  }
}