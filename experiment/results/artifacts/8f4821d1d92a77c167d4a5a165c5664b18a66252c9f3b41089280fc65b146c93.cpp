#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_68\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring:
// Buttons: btn1=D2, btn2=D3, btn3=D4 (active LOW).
// Potentiometers: pot1=A3, pot2=A4, pot3=A5.
// Common-cathode RGB LED: R=D5, G=D6, B=D7.
constexpr unsigned long PRESS_DEBOUNCE_MS = 150UL;

struct ColorChannel {
  uint8_t buttonPin;
  uint8_t potentiometerPin;
  uint8_t ledPin;
  uint8_t intensity;
  bool trackingPress;
  bool pressAccepted;
  unsigned long pressStartedAt;
};

ColorChannel channels[] = {
  {2, A3, 5, 0, false, false, 0},
  {3, A4, 6, 0, false, false, 0},
  {4, A5, 7, 0, false, false, 0}
};

#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_68\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_68\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_68\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  for (ColorChannel &channel : channels) {
    pinMode(channel.buttonPin, INPUT_PULLUP);
    pinMode(channel.potentiometerPin, INPUT);
    pinMode(channel.ledPin, OUTPUT);

    channel.intensity = 0;
    analogWrite(channel.ledPin, 0);
  }
}

void loop() {
  const unsigned long now = millis();

  for (ColorChannel &channel : channels) {
    const bool pressed = (digitalRead(channel.buttonPin) == LOW);

    if (!pressed) {
      // Stop adjusting immediately on release and retain the last intensity.
      // Any subsequent press must qualify for another full 150 ms.
      channel.trackingPress = false;
      channel.pressAccepted = false;
      continue;
    }

    if (!channel.trackingPress) {
      channel.trackingPress = true;
      channel.pressStartedAt = now;
    }

    if (!channel.pressAccepted &&
        (unsigned long)(now - channel.pressStartedAt) >= PRESS_DEBOUNCE_MS) {
      channel.pressAccepted = true;
    }

    if (channel.pressAccepted) {
      const uint8_t newIntensity =
          static_cast<uint8_t>(map(analogRead(channel.potentiometerPin),
                                   0, 1023, 0, 255));

      if (newIntensity != channel.intensity) {
        channel.intensity = newIntensity;
        analogWrite(channel.ledPin, channel.intensity);
      }
    }
  }
}
