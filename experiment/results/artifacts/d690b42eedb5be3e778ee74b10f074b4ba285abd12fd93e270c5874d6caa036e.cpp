#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_121\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t CHANNEL_COUNT = 3;
constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long HOLD_MS = 2000;

const uint8_t LED_PINS[CHANNEL_COUNT] = {2, 3, 4};
const uint8_t BUTTON_PINS[CHANNEL_COUNT] = {5, 6, 7};
const uint8_t POT_PINS[CHANNEL_COUNT] = {A3, A4, A5};

struct ChannelState {
  uint8_t rawButton;
  uint8_t stableButton;
  unsigned long rawChangedAt;
  unsigned long brightnessSetAt;
  bool holding;
  bool pending;
  uint8_t pendingBrightness;
};

ChannelState channels[CHANNEL_COUNT];

#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_121\\direct\\attempt_0\\sketch\\sketch.ino"
void setBrightness(uint8_t index, uint8_t brightness, unsigned long now);
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_121\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_121\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_121\\direct\\attempt_0\\sketch\\sketch.ino"
void setBrightness(uint8_t index, uint8_t brightness, unsigned long now) {
  analogWrite(LED_PINS[index], brightness);
  channels[index].brightnessSetAt = now;
  channels[index].holding = true;
}

void setup() {
  for (uint8_t i = 0; i < CHANNEL_COUNT; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
    analogWrite(LED_PINS[i], 0);

    // Buttons connect to ground when pressed.
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
    pinMode(POT_PINS[i], INPUT);

    channels[i].rawButton = HIGH;
    channels[i].stableButton = HIGH;
    channels[i].rawChangedAt = millis();
    channels[i].brightnessSetAt = 0;
    channels[i].holding = false;
    channels[i].pending = false;
    channels[i].pendingBrightness = 0;
  }
}

void loop() {
  for (uint8_t i = 0; i < CHANNEL_COUNT; ++i) {
    ChannelState &channel = channels[i];
    const unsigned long now = millis();

    if (channel.holding &&
        now - channel.brightnessSetAt >= HOLD_MS) {
      channel.holding = false;
    }

    const uint8_t reading = digitalRead(BUTTON_PINS[i]);

    if (reading != channel.rawButton) {
      channel.rawButton = reading;
      channel.rawChangedAt = now;
    }

    if (channel.stableButton != channel.rawButton &&
        now - channel.rawChangedAt >= DEBOUNCE_MS) {
      channel.stableButton = channel.rawButton;

      // Capture once per debounced press, not continuously while held.
      if (channel.stableButton == LOW) {
        const int potValue = analogRead(POT_PINS[i]);
        const uint8_t brightness =
            static_cast<uint8_t>(map(potValue, 0, 1023, 0, 255));

        if (!channel.holding) {
          channel.pending = false;
          setBrightness(i, brightness, now);
        } else {
          // Preserve the current level for its full two seconds.
          // If pressed repeatedly during this interval, retain the
          // most recently requested brightness for this channel.
          channel.pendingBrightness = brightness;
          channel.pending = true;
        }
      }
    }

    if (!channel.holding && channel.pending) {
      const uint8_t brightness = channel.pendingBrightness;
      channel.pending = false;
      setBrightness(i, brightness, now);
    }
  }
}
