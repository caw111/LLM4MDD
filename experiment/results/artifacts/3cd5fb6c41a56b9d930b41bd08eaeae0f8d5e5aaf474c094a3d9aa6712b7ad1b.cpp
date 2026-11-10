#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_016\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>
#include <math.h>

constexpr uint8_t CLK_PIN = 4;
constexpr uint8_t DIO_PIN = 5;
constexpr uint8_t NEXT_PIN = 2;
constexpr uint8_t CONFIRM_PIN = 3;
constexpr uint8_t DHT_PIN = 7;
constexpr uint8_t POT_PIN = A0;

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long DHT_INTERVAL_MS = 2000;
constexpr unsigned long DISPLAY_INTERVAL_MS = 150;

TM1637Display display(CLK_PIN, DIO_PIN);
DHT dht(DHT_PIN, DHT11);

enum MenuItem : uint8_t {
  TEMPERATURE,
  HUMIDITY,
  POTENTIOMETER,
  MENU_COUNT
};

struct Button {
  uint8_t pin;
  uint8_t stableState;
  uint8_t lastReading;
  unsigned long changedAt;

  void begin() {
    pinMode(pin, INPUT_PULLUP);
    stableState = digitalRead(pin);
    lastReading = stableState;
    changedAt = millis();
  }

  bool pressed(unsigned long now) {
    const uint8_t reading = digitalRead(pin);

    if (reading != lastReading) {
      lastReading = reading;
      changedAt = now;
    }

    if (reading != stableState &&
        (unsigned long)(now - changedAt) >= DEBOUNCE_MS) {
      stableState = reading;
      return stableState == LOW;
    }

    return false;
  }
};

Button nextButton = {NEXT_PIN, HIGH, HIGH, 0};
Button confirmButton = {CONFIRM_PIN, HIGH, HIGH, 0};

MenuItem selectedItem = TEMPERATURE;
bool showingValue = false;
bool displayDirty = true;
bool sensorAttempted = false;

float temperature = NAN;
float humidity = NAN;

unsigned long lastDhtRead = 0;
unsigned long lastDisplayUpdate = 0;

// Seven-segment approximations: "tEnP", "HUn ", "Pot ".
const uint8_t MENU_LABELS[MENU_COUNT][4] = {
  {
    SEG_D | SEG_E | SEG_F | SEG_G,
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,
    SEG_C | SEG_E | SEG_G,
    SEG_A | SEG_B | SEG_E | SEG_F | SEG_G
  },
  {
    SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
    SEG_C | SEG_E | SEG_G,
    0
  },
  {
    SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,
    SEG_C | SEG_D | SEG_E | SEG_G,
    SEG_D | SEG_E | SEG_F | SEG_G,
    0
  }
};

#line 94 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_016\\direct\\attempt_0\\sketch\\sketch.ino"
void showWaiting();
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_016\\direct\\attempt_0\\sketch\\sketch.ino"
void showSensorError();
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_016\\direct\\attempt_0\\sketch\\sketch.ino"
void renderDisplay();
#line 143 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_016\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 160 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_016\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 94 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_016\\direct\\attempt_0\\sketch\\sketch.ino"
void showWaiting() {
  const uint8_t segments[] = {SEG_G, SEG_G, SEG_G, SEG_G};
  display.setSegments(segments);
}

void showSensorError() {
  const uint8_t segments[] = {
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,
    SEG_E | SEG_G,
    SEG_E | SEG_G,
    0
  };
  display.setSegments(segments);
}

void renderDisplay() {
  if (!showingValue) {
    display.setSegments(MENU_LABELS[selectedItem]);
    return;
  }

  if (selectedItem == POTENTIOMETER) {
    display.showNumberDec(analogRead(POT_PIN), false);
    return;
  }

  if (!sensorAttempted) {
    showWaiting();
    return;
  }

  const float value =
      (selectedItem == TEMPERATURE) ? temperature : humidity;

  if (isnan(value)) {
    showSensorError();
    return;
  }

  const int roundedValue = (int)roundf(value);
  display.clear();
  display.showNumberDec(roundedValue, false, 3, 0);

  const uint8_t unit = (selectedItem == TEMPERATURE)
      ? (SEG_A | SEG_D | SEG_E | SEG_F)           // C
      : (SEG_B | SEG_C | SEG_E | SEG_F | SEG_G); // H
  display.setSegments(&unit, 1, 3);
}

void setup() {
  nextButton.begin();
  confirmButton.begin();

  pinMode(POT_PIN, INPUT);
  dht.begin();

  display.setBrightness(5, true);
  display.clear();

  // Allow the DHT11 to stabilize before its first reading.
  lastDhtRead = millis();
  renderDisplay();
  displayDirty = false;
  lastDisplayUpdate = millis();
}

void loop() {
  unsigned long now = millis();

  const bool nextPressed = nextButton.pressed(now);
  const bool confirmPressed = confirmButton.pressed(now);

  if (nextPressed) {
    selectedItem = static_cast<MenuItem>(
        (static_cast<uint8_t>(selectedItem) + 1) % MENU_COUNT);
    showingValue = false;
    displayDirty = true;
  }

  // If both buttons are pressed together, menu switching takes priority.
  if (confirmPressed && !nextPressed) {
    showingValue = true;
    displayDirty = true;
  }

  if (showingValue &&
      selectedItem != POTENTIOMETER &&
      (unsigned long)(now - lastDhtRead) >= DHT_INTERVAL_MS) {
    lastDhtRead = now;
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    sensorAttempted = true;
    displayDirty = true;
  }

  now = millis();
  if (displayDirty ||
      (showingValue &&
       (unsigned long)(now - lastDisplayUpdate) >= DISPLAY_INTERVAL_MS)) {
    renderDisplay();
    lastDisplayUpdate = millis();
    displayDirty = false;
  }
}
