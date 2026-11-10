#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Pin mapping from provided (authoritative) configuration
static const uint8_t PIN_ANALOG_POT = A1; // A0 requirement interpreted via wiring: authoritative pin is A1
static const uint8_t PIN_BUTTON    = D2;  // button
static const uint8_t PIN_TM1637_CLK  = D4; // TM1637 CLK
static const uint8_t PIN_TM1637_DIO  = D5; // TM1637 DIO

// TM1637
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

static const uint8_t TM1637_BRIGHTNESS = 7; // sensible default (visible on UNO)

// Read/format behavior
// - Default: show potentiometer value
// - Button pressed: show "8888"
static const int POT_SAMPLES = 10;

#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readPotValueSmoothed();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isButtonPressed();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readPotValueSmoothed() {
  long acc = 0;
  for (int i = 0; i < POT_SAMPLES; i++) {
    acc += analogRead(PIN_ANALOG_POT);
    delay(2);
  }
  int avg = (int)(acc / POT_SAMPLES);

  // Map 0..1023 to 0..9999 for 4-digit display.
  // Use integer mapping to preserve deterministic behavior.
  int mapped = (avg * 9999L) / 1023L;
  if (mapped < 0) mapped = 0;
  if (mapped > 9999) mapped = 9999;
  return mapped;
}

static bool isButtonPressed() {
  // D2 input: use internal pull-up, press assumed active-low unless circuit specifies otherwise.
  // Typical wiring: button shorts to GND when pressed.
  // If your circuit is opposite, invert this logic.
  return digitalRead(PIN_BUTTON) == LOW;
}

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  display.setBrightness(TM1637_BRIGHTNESS);
  display.clear();

  // Default display: potentiometer value
  int potVal = readPotValueSmoothed();
  display.showNumberDecEx(potVal, 0b0000, true);
}

void loop() {
  if (isButtonPressed()) {
    display.showNumberDecEx(8888, 0b0000, true);
  } else {
    int potVal = readPotValueSmoothed();
    display.showNumberDecEx(potVal, 0b0000, true);
  }

  delay(30);
}
