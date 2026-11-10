#include <Arduino.h>
#include <TM1637Display.h>

// Authoritative pin assignments (remapped as required by configuration)
static const uint8_t PIN_ANALOG_POT = A1; // pot input (config: analog_input pin A1)
static const uint8_t PIN_BUTTON     = 2;  // D2 button (config: digital_input pin D2)
static const uint8_t PIN_TM1637_CLK = 4;  // D4 TM1637 CLK
static const uint8_t PIN_TM1637_DIO = 5;  // D5 TM1637 DIO

// TM1637
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// Display brightness
static const uint8_t TM1637_BRIGHTNESS = 7;

// Read/format behavior
static const int POT_SAMPLES = 10;

static int readPotValueSmoothed() {
  long acc = 0;
  for (int i = 0; i < POT_SAMPLES; i++) {
    acc += analogRead(PIN_ANALOG_POT);
    delay(2);
  }
  int avg = (int)(acc / POT_SAMPLES);

  // Map 0..1023 to 0..9999 for 4-digit display.
  int mapped = (avg * 9999L) / 1023L;
  if (mapped < 0) mapped = 0;
  if (mapped > 9999) mapped = 9999;
  return mapped;
}

static bool isButtonPressed() {
  // D2 uses internal pull-up; typical wiring: button shorts to GND when pressed.
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