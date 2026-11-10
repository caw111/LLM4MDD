#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_121\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t LED1_PIN = 2;
static const uint8_t LED2_PIN = 3;
static const uint8_t LED3_PIN = 4;

static const uint8_t BTN1_PIN = 5;
static const uint8_t BTN2_PIN = 6;
static const uint8_t BTN3_PIN = 7;

static const uint8_t POT1_PIN = A3;
static const uint8_t POT2_PIN = A4;
static const uint8_t POT3_PIN = A5;

static const uint32_t STABLE_MS = 2000;

static uint8_t ledBrightness[3] = {0, 0, 0};
static uint32_t ledSetTimeMs[3] = {0, 0, 0};

#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_121\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t readLedBrightnessFromPot(uint8_t potPin);
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_121\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_121\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_121\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t readLedBrightnessFromPot(uint8_t potPin) {
  int potVal = analogRead(potPin);          // 0..1023
  if (potVal < 0) potVal = 0;
  if (potVal > 1023) potVal = 1023;
  uint8_t brightness = (uint32_t)potVal * 255UL / 1023UL;
  return brightness;
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // Pots are analog inputs; no special init required beyond defaults.

  // Initialize: all LEDs off.
  analogWrite(LED1_PIN, 0);
  analogWrite(LED2_PIN, 0);
  analogWrite(LED3_PIN, 0);

  ledBrightness[0] = 0;
  ledBrightness[1] = 0;
  ledBrightness[2] = 0;
  ledSetTimeMs[0] = millis();
  ledSetTimeMs[1] = millis();
  ledSetTimeMs[2] = millis();
}

void loop() {
  const uint32_t now = millis();

  // Buttons are wired with one side to GND and the other to Arduino pins.
  // With INPUT_PULLUP, pressed => LOW, released => HIGH.
  const bool btn1Pressed = (digitalRead(BTN1_PIN) == LOW);
  const bool btn2Pressed = (digitalRead(BTN2_PIN) == LOW);
  const bool btn3Pressed = (digitalRead(BTN3_PIN) == LOW);

  // When a button is pressed, update its corresponding LED brightness
  // from its potentiometer, and keep that value stable for at least 2 seconds.
  // Other LEDs remain unchanged.
  if (btn1Pressed) {
    if (now - ledSetTimeMs[0] >= STABLE_MS) {
      ledBrightness[0] = readLedBrightnessFromPot(POT1_PIN);
      ledSetTimeMs[0] = now;
    }
    analogWrite(LED1_PIN, ledBrightness[0]);
  } else {
    analogWrite(LED1_PIN, ledBrightness[0]);
  }

  if (btn2Pressed) {
    if (now - ledSetTimeMs[1] >= STABLE_MS) {
      ledBrightness[1] = readLedBrightnessFromPot(POT2_PIN);
      ledSetTimeMs[1] = now;
    }
    analogWrite(LED2_PIN, ledBrightness[1]);
  } else {
    analogWrite(LED2_PIN, ledBrightness[1]);
  }

  if (btn3Pressed) {
    if (now - ledSetTimeMs[2] >= STABLE_MS) {
      ledBrightness[2] = readLedBrightnessFromPot(POT3_PIN);
      ledSetTimeMs[2] = now;
    }
    analogWrite(LED3_PIN, ledBrightness[2]);
  } else {
    analogWrite(LED3_PIN, ledBrightness[2]);
  }

  // No additional behavior required.
}
