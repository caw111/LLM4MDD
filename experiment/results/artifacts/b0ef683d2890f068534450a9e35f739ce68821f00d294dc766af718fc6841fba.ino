#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 2;  // Button connects D2 to GND.
constexpr uint8_t LIGHT_PIN = A0;
constexpr uint8_t LM35_PIN = A1;
constexpr uint8_t BUZZER_PIN = 6;

constexpr unsigned long RELEASE_DEBOUNCE_MS = 30;
constexpr unsigned long BEEP_DURATION_MS = 100;
constexpr unsigned int BEEP_FREQUENCY_HZ = 2000;
constexpr float ADC_REFERENCE_VOLTS = 5.0f;

unsigned long sampleCount = 0;
uint8_t samplesSinceBeep = 0;

bool buttonArmed = true;
bool releaseTiming = false;
unsigned long releaseStartedAt = 0;

void takeSample() {
  const int lightRaw = analogRead(LIGHT_PIN);

  // Discard the first reading after switching ADC channels.
  analogRead(LM35_PIN);
  const int temperatureRaw = analogRead(LM35_PIN);
  const float temperatureC =
      temperatureRaw * (ADC_REFERENCE_VOLTS / 1023.0f) * 100.0f;

  ++sampleCount;
  ++samplesSinceBeep;

  if (samplesSinceBeep == 5) {
    samplesSinceBeep = 0;
    tone(BUZZER_PIN, BEEP_FREQUENCY_HZ, BEEP_DURATION_MS);
  }

  Serial.print(F("Sample="));
  Serial.print(sampleCount);
  Serial.print(F(", Light_ADC="));
  Serial.print(lightRaw);
  Serial.print(F(", Temperature_C="));
  Serial.println(temperatureC, 2);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(LM35_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  analogReference(DEFAULT);
  Serial.begin(9600);
  Serial.setTimeout(20);
}

void loop() {
  const unsigned long now = millis();
  const bool pressed = (digitalRead(BUTTON_PIN) == LOW);

  if (pressed) {
    releaseTiming = false;

    if (buttonArmed) {
      buttonArmed = false;
      takeSample();  // Sample immediately, without a press-debounce delay.
    }
  } else if (!buttonArmed) {
    // Require a stable release before accepting another press.
    if (!releaseTiming) {
      releaseTiming = true;
      releaseStartedAt = now;
    } else if (now - releaseStartedAt >= RELEASE_DEBOUNCE_MS) {
      buttonArmed = true;
      releaseTiming = false;
    }
  }
}