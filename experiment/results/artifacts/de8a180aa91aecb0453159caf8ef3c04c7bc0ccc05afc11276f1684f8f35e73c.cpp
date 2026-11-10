#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_012\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t LED_PIN = 13;
constexpr uint8_t POT_PIN = A0;
constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;

// Button connects D2 to GND; the internal pull-up supplies its idle level.
// The display shows the complete blink period in milliseconds (100–5000).
constexpr uint16_t MIN_PERIOD_MS = 100;
constexpr uint16_t MAX_PERIOD_MS = 5000;
constexpr uint32_t POT_SAMPLE_MS = 20;
constexpr uint32_t DEBOUNCE_MS = 30;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

bool running = false;
bool ledOn = false;
bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;

uint16_t periodMs = MIN_PERIOD_MS;
uint32_t lastPotSample = 0;
uint32_t buttonChangedAt = 0;
uint32_t lastLedTransition = 0;

#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_012\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t readPeriod();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_012\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_012\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_012\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t readPeriod()
{
  // Ten-millisecond resolution provides a readable, less jittery display.
  const int value = analogRead(POT_PIN);
  return static_cast<uint16_t>(
      map(value, 0, 1023, MIN_PERIOD_MS / 10, MAX_PERIOD_MS / 10) * 10);
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(POT_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  display.setBrightness(7, true);
  periodMs = readPeriod();
  display.showNumberDec(periodMs, false);

  lastButtonReading = digitalRead(BUTTON_PIN);
  stableButtonState = lastButtonReading;

  const uint32_t now = millis();
  lastPotSample = now;
  buttonChangedAt = now;
  lastLedTransition = now;
}

void loop()
{
  const uint32_t now = millis();

  // Configuration remains available while stopped; stopping never resets it.
  if (now - lastPotSample >= POT_SAMPLE_MS) {
    lastPotSample = now;
    const uint16_t newPeriod = readPeriod();

    if (newPeriod != periodMs) {
      periodMs = newPeriod;
      display.showNumberDec(periodMs, false);
    }
  }

  const bool buttonReading = digitalRead(BUTTON_PIN);

  if (buttonReading != lastButtonReading) {
    lastButtonReading = buttonReading;
    buttonChangedAt = now;
  }

  if (now - buttonChangedAt >= DEBOUNCE_MS &&
      stableButtonState != buttonReading) {
    stableButtonState = buttonReading;

    // Toggle only on a debounced press, not on release or while held.
    if (stableButtonState == LOW) {
      running = !running;
      ledOn = running;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
      lastLedTransition = now;
    }
  }

  if (running) {
    const uint32_t halfPeriodMs = periodMs / 2;
    const uint32_t elapsed = now - lastLedTransition;

    if (elapsed >= halfPeriodMs) {
      const uint32_t transitions = elapsed / halfPeriodMs;
      lastLedTransition += transitions * halfPeriodMs;

      if (transitions & 1UL) {
        ledOn = !ledOn;
        digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
      }
    }
  }
}
