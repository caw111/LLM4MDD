#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_020\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Wiring:
// Button: D2 to GND (internal pull-up enabled)
// Potentiometer: wiper to A0, outer terminals to 5V and GND
// TM1637: CLK to D4, DIO to D5, VCC to 5V, GND to GND

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t POT_PIN = A0;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long COUNT_INTERVAL_MS = 1000;
constexpr unsigned long POT_SAMPLE_MS = 50;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

enum DisplayMode : uint8_t {
  MODE_COUNTER,
  MODE_POTENTIOMETER,
  MODE_FIXED,
  MODE_COUNT
};

DisplayMode mode = MODE_COUNTER;

uint16_t counterValue = 0;
int potentiometerValue = 0;
int lastDisplayedValue = -1;

uint8_t lastButtonReading = HIGH;
uint8_t stableButtonState = HIGH;

unsigned long buttonChangedAt = 0;
unsigned long lastCountAt = 0;
unsigned long lastPotSampleAt = 0;

bool displayDirty = true;

#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_020\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_020\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_020\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);

  display.setBrightness(7, true);
  display.clear();

  const unsigned long now = millis();
  lastCountAt = now;
  lastPotSampleAt = now;
  buttonChangedAt = now;

  potentiometerValue = analogRead(POT_PIN);

  display.showNumberDec(counterValue, false);
  lastDisplayedValue = counterValue;
  displayDirty = false;
}

void loop() {
  const unsigned long now = millis();

  // Debounce the active-low button and advance once per press.
  const uint8_t buttonReading = digitalRead(BUTTON_PIN);

  if (buttonReading != lastButtonReading) {
    lastButtonReading = buttonReading;
    buttonChangedAt = now;
  }

  if ((unsigned long)(now - buttonChangedAt) >= DEBOUNCE_MS &&
      buttonReading != stableButtonState) {
    stableButtonState = buttonReading;

    if (stableButtonState == LOW) {
      mode = static_cast<DisplayMode>(
          (static_cast<uint8_t>(mode) + 1) % MODE_COUNT);
      displayDirty = true;
    }
  }

  // Count once per second, continuing even while another mode is displayed.
  const unsigned long elapsed = now - lastCountAt;
  if (elapsed >= COUNT_INTERVAL_MS) {
    const unsigned long steps = elapsed / COUNT_INTERVAL_MS;
    lastCountAt += steps * COUNT_INTERVAL_MS;
    counterValue = static_cast<uint16_t>(
        (counterValue + steps) % 10000UL);
  }

  // Display the raw potentiometer ADC value, from 0 to 1023.
  if ((unsigned long)(now - lastPotSampleAt) >= POT_SAMPLE_MS) {
    lastPotSampleAt = now;
    potentiometerValue = analogRead(POT_PIN);
  }

  int valueToDisplay;
  switch (mode) {
    case MODE_COUNTER:
      valueToDisplay = counterValue;
      break;

    case MODE_POTENTIOMETER:
      valueToDisplay = potentiometerValue;
      break;

    case MODE_FIXED:
    default:
      valueToDisplay = 1234;
      break;
  }

  if (displayDirty || valueToDisplay != lastDisplayedValue) {
    display.showNumberDec(valueToDisplay, false);
    lastDisplayedValue = valueToDisplay;
    displayDirty = false;
  }
}
