#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_125\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t BUTTON_PIN = 5;
const uint8_t INDICATOR_PINS[3] = {2, 3, 4};

// Arduino Mega 2560 PWM pins, ordered red, green, blue.
// All RGB LEDs are common-cathode: 0 = off, 255 = full brightness.
const uint8_t RGB_PINS[3][3] = {
  {6, 7, 8},
  {9, 10, 11},
  {12, 13, 44}
};

const unsigned long DEBOUNCE_MS = 30;
const unsigned long MIN_MODE_DURATION_MS = 2000;
const unsigned long RAINBOW_STEP_MS = 2000;
const unsigned long RAINBOW_FRAME_MS = 10;

struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

const Color RAINBOW_COLORS[3] = {
  {255, 255, 0},  // Yellow
  {0, 255, 255},  // Cyan
  {255, 0, 255}   // Purple
};

// Mode 0 is the initial all-off state; button presses select modes 1–4.
uint8_t currentMode = 0;
unsigned long modeStartedAt = 0;
unsigned long lastRainbowFrameAt = 0;

uint8_t lastRawButtonState = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long buttonChangedAt = 0;

#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_125\\direct\\attempt_0\\sketch\\sketch.ino"
void setRgb(uint8_t index, const Color &color);
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_125\\direct\\attempt_0\\sketch\\sketch.ino"
void turnAllOff();
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_125\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t interpolateChannel(uint8_t from, uint8_t to, unsigned long progress);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_125\\direct\\attempt_0\\sketch\\sketch.ino"
void updateRainbow(unsigned long now);
#line 91 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_125\\direct\\attempt_0\\sketch\\sketch.ino"
void enterMode(uint8_t mode, unsigned long now);
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_125\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 155 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_125\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_125\\direct\\attempt_0\\sketch\\sketch.ino"
void setRgb(uint8_t index, const Color &color) {
  analogWrite(RGB_PINS[index][0], color.red);
  analogWrite(RGB_PINS[index][1], color.green);
  analogWrite(RGB_PINS[index][2], color.blue);
}

void turnAllOff() {
  const Color off = {0, 0, 0};

  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(INDICATOR_PINS[i], LOW);
    setRgb(i, off);
  }
}

uint8_t interpolateChannel(uint8_t from, uint8_t to,
                           unsigned long progress) {
  return static_cast<uint8_t>(
    static_cast<long>(from) +
    (static_cast<long>(to) - static_cast<long>(from)) *
      static_cast<long>(progress) /
      static_cast<long>(RAINBOW_STEP_MS)
  );
}

void updateRainbow(unsigned long now) {
  const unsigned long cycleDuration = 3UL * RAINBOW_STEP_MS;
  const unsigned long elapsed = (now - modeStartedAt) % cycleDuration;

  for (uint8_t i = 0; i < 3; ++i) {
    const unsigned long phase =
      (elapsed + static_cast<unsigned long>(i) * RAINBOW_STEP_MS) %
      cycleDuration;

    const uint8_t fromIndex = phase / RAINBOW_STEP_MS;
    const uint8_t toIndex = (fromIndex + 1) % 3;
    const unsigned long progress = phase % RAINBOW_STEP_MS;

    const Color &from = RAINBOW_COLORS[fromIndex];
    const Color &to = RAINBOW_COLORS[toIndex];

    const Color blended = {
      interpolateChannel(from.red, to.red, progress),
      interpolateChannel(from.green, to.green, progress),
      interpolateChannel(from.blue, to.blue, progress)
    };

    setRgb(i, blended);
  }
}

void enterMode(uint8_t mode, unsigned long now) {
  currentMode = mode;
  modeStartedAt = now;

  turnAllOff();

  // One indicator per active mode: led1, led2, then led3.
  if (mode >= 1 && mode <= 3) {
    digitalWrite(INDICATOR_PINS[mode - 1], HIGH);
  }

  switch (mode) {
    case 1: {
      const Color red = {255, 0, 0};
      const Color green = {0, 255, 0};
      const Color blue = {0, 0, 255};

      setRgb(0, red);
      setRgb(1, green);
      setRgb(2, blue);
      break;
    }

    case 2:
      // Begin at yellow, cyan, purple, then smoothly rotate the colors.
      updateRainbow(now);
      lastRainbowFrameAt = now;
      break;

    case 3: {
      // Equal RGB values, as explicitly required for the white mode.
      const Color white = {180, 180, 180};
      for (uint8_t i = 0; i < 3; ++i) {
        setRgb(i, white);
      }
      break;
    }

    case 4:
    default:
      // All outputs were already turned off.
      break;
  }
}

void setup() {
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(INDICATOR_PINS[i], LOW);
    pinMode(INDICATOR_PINS[i], OUTPUT);

    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(RGB_PINS[i][channel], LOW);
      pinMode(RGB_PINS[i][channel], OUTPUT);
    }
  }

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  turnAllOff();

  lastRawButtonState = digitalRead(BUTTON_PIN);
  stableButtonState = lastRawButtonState;
  buttonChangedAt = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawButtonState = digitalRead(BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastRawButtonState = rawButtonState;
    buttonChangedAt = now;
  }

  if ((now - buttonChangedAt >= DEBOUNCE_MS) &&
      (rawButtonState != stableButtonState)) {
    stableButtonState = rawButtonState;

    // Accept only a debounced press edge; holding does not repeat.
    if (stableButtonState == LOW) {
      if (currentMode == 0 ||
          now - modeStartedAt >= MIN_MODE_DURATION_MS) {
        const uint8_t nextMode =
          (currentMode == 4) ? 1 : currentMode + 1;
        enterMode(nextMode, now);
      }
      // Presses during the minimum dwell time are ignored, not queued.
    }
  }

  if (currentMode == 2 &&
      now - lastRainbowFrameAt >= RAINBOW_FRAME_MS) {
    lastRainbowFrameAt = now;
    updateRainbow(now);
  }
}
