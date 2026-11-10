#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_87\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t BUTTON_PINS[3] = {2, 3, 4};
const uint8_t RGB_PINS[3] = {5, 6, 7};

// Bar graph terminals A1-A8 are wired to these Mega digital pins,
// not to the Arduino analog input pins.
const uint8_t BAR_PINS[8] = {8, 9, 10, 11, 12, 13, 44, 45};

const unsigned long DEBOUNCE_MS = 30;

uint8_t intensities[3] = {0, 0, 0};

struct ButtonState {
  uint8_t lastReading;
  uint8_t stableState;
  unsigned long lastChangeTime;
};

ButtonState buttons[3];

#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_87\\direct\\attempt_0\\sketch\\sketch.ino"
void updateRGB();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_87\\direct\\attempt_0\\sketch\\sketch.ino"
void updateBarGraph(uint8_t intensity);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_87\\direct\\attempt_0\\sketch\\sketch.ino"
void handlePress(uint8_t colorIndex);
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_87\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_87\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_87\\direct\\attempt_0\\sketch\\sketch.ino"
void updateRGB() {
  for (uint8_t i = 0; i < 3; ++i) {
    analogWrite(RGB_PINS[i], intensities[i]);
  }
}

void updateBarGraph(uint8_t intensity) {
  const uint8_t litSegments =
      static_cast<uint8_t>((static_cast<uint16_t>(intensity) + 31U) / 32U);

  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(BAR_PINS[i], i < litSegments ? HIGH : LOW);
  }
}

void handlePress(uint8_t colorIndex) {
  const uint16_t next =
      static_cast<uint16_t>(intensities[colorIndex]) + 32U;

  if (intensities[colorIndex] == 255) {
    intensities[colorIndex] = 0;
  } else {
    intensities[colorIndex] =
        static_cast<uint8_t>(next > 255U ? 255U : next);
  }

  updateRGB();
  updateBarGraph(intensities[colorIndex]);
}

void setup() {
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(RGB_PINS[i], LOW);
    pinMode(RGB_PINS[i], OUTPUT);
    analogWrite(RGB_PINS[i], 0);
  }

  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }

  for (uint8_t i = 0; i < 3; ++i) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
    buttons[i].lastReading = HIGH;
    buttons[i].stableState = HIGH;
    buttons[i].lastChangeTime = millis();
  }
}

void loop() {
  const unsigned long now = millis();

  for (uint8_t i = 0; i < 3; ++i) {
    const uint8_t reading = digitalRead(BUTTON_PINS[i]);

    if (reading != buttons[i].lastReading) {
      buttons[i].lastReading = reading;
      buttons[i].lastChangeTime = now;
    }

    if (now - buttons[i].lastChangeTime >= DEBOUNCE_MS &&
        reading != buttons[i].stableState) {
      buttons[i].stableState = reading;

      // Active-low buttons: trigger once per debounced press.
      if (reading == LOW) {
        handlePress(i);
      }
    }
  }
}
