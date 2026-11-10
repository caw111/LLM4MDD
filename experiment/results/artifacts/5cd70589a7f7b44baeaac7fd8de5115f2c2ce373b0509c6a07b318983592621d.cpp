#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_85\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560, common-cathode RGB LED.
constexpr uint8_t BTN1_PIN = 2;
constexpr uint8_t BTN2_PIN = 3;
constexpr uint8_t RED_PIN = 4;
constexpr uint8_t GREEN_PIN = 5;
constexpr uint8_t BLUE_PIN = 6;
constexpr uint8_t POT_PIN = A3;

constexpr unsigned long DEBOUNCE_MS = 25;

enum ColorMode : uint8_t {
  MODE_OFF,
  MODE_PRIMARY,
  MODE_SECONDARY
};

struct DebouncedButton {
  uint8_t pin;
  uint8_t lastReading;
  uint8_t stableState;
  unsigned long lastChangeTime;

  void begin(uint8_t inputPin) {
    pin = inputPin;
    pinMode(pin, INPUT_PULLUP);
    lastReading = digitalRead(pin);
    stableState = HIGH;
    lastChangeTime = millis();
  }

  // Reports one event per debounced press; holding does not repeat.
  bool pressed(unsigned long now) {
    const uint8_t reading = digitalRead(pin);

    if (reading != lastReading) {
      lastReading = reading;
      lastChangeTime = now;
    }

    if (reading != stableState &&
        now - lastChangeTime >= DEBOUNCE_MS) {
      stableState = reading;
      return stableState == LOW;
    }

    return false;
  }
};

DebouncedButton btn1;
DebouncedButton btn2;

ColorMode mode = MODE_PRIMARY;
uint8_t colorIndex = 0;

// Channel order: red, green, blue.
const uint8_t primaryColors[3][3] = {
  {1, 0, 0},  // Red
  {0, 1, 0},  // Green
  {0, 0, 1}   // Blue
};

const uint8_t secondaryColors[3][3] = {
  {1, 1, 0},  // Yellow
  {0, 1, 1},  // Cyan
  {1, 0, 1}   // Magenta
};

#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_85\\direct\\attempt_0\\sketch\\sketch.ino"
void updateLed();
#line 94 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_85\\direct\\attempt_0\\sketch\\sketch.ino"
void handleButton1();
#line 106 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_85\\direct\\attempt_0\\sketch\\sketch.ino"
void handleButton2();
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_85\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 133 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_85\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_85\\direct\\attempt_0\\sketch\\sketch.ino"
void updateLed() {
  const uint8_t brightness =
      static_cast<uint8_t>(map(analogRead(POT_PIN), 0, 1023, 0, 255));

  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;

  if (mode != MODE_OFF) {
    const uint8_t *color = (mode == MODE_PRIMARY)
                              ? primaryColors[colorIndex]
                              : secondaryColors[colorIndex];

    red = color[0] ? brightness : 0;
    green = color[1] ? brightness : 0;
    blue = color[2] ? brightness : 0;
  }

  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void handleButton1() {
  if (mode == MODE_SECONDARY) {
    mode = MODE_OFF;
  } else if (mode == MODE_PRIMARY) {
    colorIndex = (colorIndex + 1) % 3;
  } else {
    // From off, btn1 starts the primary sequence at red.
    mode = MODE_PRIMARY;
    colorIndex = 0;
  }
}

void handleButton2() {
  if (mode == MODE_PRIMARY) {
    mode = MODE_OFF;
  } else if (mode == MODE_SECONDARY) {
    colorIndex = (colorIndex + 1) % 3;
  } else {
    // From off, btn2 starts the secondary sequence at yellow.
    mode = MODE_SECONDARY;
    colorIndex = 0;
  }
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  btn1.begin(BTN1_PIN);
  btn2.begin(BTN2_PIN);

  // Initialize to red at the brightness selected by the potentiometer.
  mode = MODE_PRIMARY;
  colorIndex = 0;
  updateLed();
}

void loop() {
  const unsigned long now = millis();
  const bool pressed1 = btn1.pressed(now);
  const bool pressed2 = btn2.pressed(now);

  if (pressed1 && pressed2) {
    // Simultaneous press events deterministically turn the LED off.
    mode = MODE_OFF;
  } else if (pressed1) {
    handleButton1();
  } else if (pressed2) {
    handleButton2();
  }

  // Continuously track brightness without blocking button handling.
  updateLed();
}
