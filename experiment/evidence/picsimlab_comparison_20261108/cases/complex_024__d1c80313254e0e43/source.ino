#include <Arduino.h>

const uint8_t PIN_BUTTON = 2;     // D2
const uint8_t PIN_LDR     = A0;    // 光敏电阻
const uint8_t PIN_LM35    = A1;    // LM35
const uint8_t PIN_BUZZER  = 6;     // D6

const unsigned long SERIAL_BAUD = 9600;

// Optional: keep debounce without changing required behavior (single sample per press).
const unsigned long DEBOUNCE_MS = 50;

static volatile uint32_t sampleCount = 0;
static unsigned long lastButtonChangeMs = 0;
static int lastButtonRead = HIGH;

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP); // assume button to GND when pressed
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  Serial.begin(SERIAL_BAUD);
}

static void buzzerShortPulse(uint16_t ms = 80) {
  digitalWrite(PIN_BUZZER, HIGH);
  delay(ms);
  digitalWrite(PIN_BUZZER, LOW);
}

void loop() {
  int buttonRead = digitalRead(PIN_BUTTON);

  // Debounce and edge detect (press is HIGH->LOW with INPUT_PULLUP)
  if (buttonRead != lastButtonRead) {
    unsigned long now = millis();
    if (now - lastButtonChangeMs >= DEBOUNCE_MS) {
      lastButtonChangeMs = now;
      lastButtonRead = buttonRead;

      if (buttonRead == LOW) { // pressed
        // Immediately sample once
        int ldrValue = analogRead(PIN_LDR);
        int lm35Adc  = analogRead(PIN_LM35);

        // Convert LM35 ADC to temperature (optional format; still derived from sampling)
        // UNO ADC: 5V reference, 10-bit => 1023 counts
        // LM35 typically outputs 10mV/°C. Temperature (°C) = Vout(mV)/10.
        // Vout = lm35Adc * 5.0 / 1023
        // Temp = (lm35Adc * 5.0 / 1023) * 1000 / 10 = lm35Adc * 500 / 1023
        float temperatureC = (lm35Adc * 500.0f) / 1023.0f;

        // Output via serial
        sampleCount++;
        Serial.print("SAMPLE ");
        Serial.print(sampleCount);
        Serial.print(": LDR=");
        Serial.print(ldrValue);
        Serial.print(" LM35_ADC=");
        Serial.print(lm35Adc);
        Serial.print(" TempC=");
        Serial.println(temperatureC, 2);

        // After every 5 samples, beep once
        if (sampleCount % 5 == 0) {
          buzzerShortPulse(80);
        }

        // Wait until button released to prevent multiple samples during same press
        while (digitalRead(PIN_BUTTON) == LOW) {
          delay(1);
        }
        lastButtonRead = HIGH;
      }
    }
  }
}