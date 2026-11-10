#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_piano_keyboard\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Piano Keyboard: Do, Re, Mi, Fa
// Buttons: D2, D3, D4, D5
// Buzzers:  D9, D10, D11, D12
// Uses internal behavior: if pressed -> HIGH -> play corresponding tone

const uint8_t buttonPins[4] = {2, 3, 4, 5};
const uint8_t buzzerPins[4] = {9, 10, 11, 12};

const unsigned int freqs[4] = {
  262, // Do
  294, // Re
  330, // Mi
  349  // Fa
};

#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_piano_keyboard\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_piano_keyboard\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_piano_keyboard\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  for (uint8_t i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT);   // External pull-down via 10k resistor to GND3 when not pressed
    pinMode(buzzerPins[i], OUTPUT); // Drive buzzer pins
    digitalWrite(buzzerPins[i], LOW); // Keep silent at power-up
  }
}

void loop() {
  bool anyPressed = false;

  for (uint8_t i = 0; i < 4; i++) {
    if (digitalRead(buttonPins[i]) == HIGH) {
      anyPressed = true;

      // Only one button is pressed at a time (per requirements).
      // Play tone on the corresponding buzzer; keep others silent.
      for (uint8_t j = 0; j < 4; j++) {
        if (j == i) {
          tone(buzzerPins[j], freqs[j]);
        } else {
          noTone(buzzerPins[j]);
          digitalWrite(buzzerPins[j], LOW);
        }
      }
      break;
    }
  }

  if (!anyPressed) {
    for (uint8_t i = 0; i < 4; i++) {
      noTone(buzzerPins[i]);
      digitalWrite(buzzerPins[i], LOW);
    }
  }
}
