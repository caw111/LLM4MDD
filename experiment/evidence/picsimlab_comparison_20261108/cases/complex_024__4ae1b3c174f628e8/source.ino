#include <Arduino.h>

const uint8_t PIN_BUTTON = 2;   // D2
const uint8_t PIN_LDR    = A0;  // 光敏电阻
const uint8_t PIN_LM35   = A1;  // LM35
const uint8_t PIN_BUZZER = 6;   // D6

const unsigned long BUZZER_SHORT_MS = 100;   // 短鸣提示
const uint8_t SAMPLES_PER_BEEP = 5;

const uint32_t SERIAL_BAUD = 9600;

static unsigned long lastButtonChangeMs = 0;
static uint8_t lastButtonStableState = HIGH; // use pull-up; pressed -> LOW

// Basic debounce
uint8_t readButtonDebounced()
{
  uint8_t raw = digitalRead(PIN_BUTTON);

  // If changed, reset debounce timer
  if (raw != lastButtonStableState) {
    lastButtonChangeMs = millis();
    lastButtonStableState = raw;
    // Note: we'll confirm below
  }

  // Confirm stable state
  // (We track stability by comparing against a delayed read)
  static uint8_t lastRaw = HIGH;
  static unsigned long lastRawChangeMs = 0;

  if (raw != lastRaw) {
    lastRaw = raw;
    lastRawChangeMs = millis();
  }

  const uint16_t debounceDelayMs = 25;
  if ((millis() - lastRawChangeMs) >= debounceDelayMs) {
    return raw;
  }
  return !raw; // if not stable yet, return inverted to avoid triggering (handled below)
}

void beepOnce()
{
  digitalWrite(PIN_BUZZER, HIGH);
  delay(BUZZER_SHORT_MS);
  digitalWrite(PIN_BUZZER, LOW);
}

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP); // D2, button to GND
  pinMode(PIN_BUZZER, OUTPUT);       // D6
  digitalWrite(PIN_BUZZER, LOW);

  Serial.begin(SERIAL_BAUD);
  // Optional: wait for serial to be ready (not required)
}

void loop() {
  static bool prevPressed = false;
  uint8_t btn = digitalRead(PIN_BUTTON);

  // Detect press event (transition from not pressed to pressed)
  bool pressed = (btn == LOW);

  // Simple debounce for event detection
  static unsigned long pressDebounceMs = 0;
  static bool pending = false;

  if (pressed != prevPressed) {
    lastButtonChangeMs = millis();
  }

  const uint16_t debounceDelayMs = 25;

  if (pressed && !prevPressed) {
    // rising into pressed; start debounce window
    pending = true;
    pressDebounceMs = millis();
  }

  if (pending) {
    if (millis() - pressDebounceMs >= debounceDelayMs) {
      // confirm still pressed
      if (digitalRead(PIN_BUTTON) == LOW) {
        // Button confirmed: sample immediately
        static uint32_t sampleCount = 0;
        sampleCount++;

        // Read sensors
        int ldrRaw = analogRead(PIN_LDR);
        int tempRaw = analogRead(PIN_LM35);

        // Convert LM35: 10mV/°C; with 5V/10-bit ADC => (raw * 5.0/1023)/0.01
        // (Keep behavior deterministic with standard Arduino UNO defaults)
        float tempC = (tempRaw * (5.0f / 1023.0f)) / 0.01f;

        // Output via serial (one line per sampling event)
        // Include count, raw values, and computed temperature for clarity.
        Serial.print("SAMPLE ");
        Serial.print(sampleCount);
        Serial.print(": LDR_RAW=");
        Serial.print(ldrRaw);
        Serial.print(", TEMP_RAW=");
        Serial.print(tempRaw);
        Serial.print(", TEMP_C=");
        Serial.println(tempC, 2);

        // After every 5 samples, beep once briefly
        if ((sampleCount % SAMPLES_PER_BEEP) == 0) {
          beepOnce();
        }

        pending = false;
      } else {
        pending = false;
      }
    }
  }

  prevPressed = pressed;
}