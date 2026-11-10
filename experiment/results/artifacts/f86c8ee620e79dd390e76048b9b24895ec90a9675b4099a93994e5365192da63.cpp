#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// TM1637 pins (per provided authoritative pin assignments)
static const uint8_t TM1637_CLK = D4;
static const uint8_t TM1637_DIO = D5;

// Button pin (per provided configuration)
static const uint8_t BUTTON_PIN = D2;

// Potentiometer analog input (per provided configuration)
static const uint8_t POT_PIN = A1;

// Display
TM1637Display display(TM1637_CLK, TM1637_DIO);

// Button handling
static bool lastButtonState = HIGH; // using INPUT_PULLUP
static unsigned long lastDebounceMs = 0;
static const unsigned long debounceDelayMs = 30;

static bool show8888 = false;

#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t mapToDisplayRange(int raw);
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_010\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t mapToDisplayRange(int raw)
{
  // Convert 0..1023 to 0..9999 (default no decimals)
  if (raw < 0) raw = 0;
  if (raw > 1023) raw = 1023;
  return (uint16_t)((long)raw * 9999L / 1023L);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // TM1637 display brightness: choose a safe default
  display.setBrightness(7, true);

  // Default: show potentiometer value
  show8888 = false;

  // Initialize last state
  lastButtonState = digitalRead(BUTTON_PIN);

  // Show something immediately
  int raw = analogRead(POT_PIN);
  uint16_t value = mapToDisplayRange(raw);
  display.showNumberDecEx(value, 0, true);
}

void loop() {
  // Read and debounce button
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState != lastButtonState) {
    lastDebounceMs = millis();
    lastButtonState = buttonState;
  }

  // After debounce, determine if pressed
  static bool stableButtonPressed = false;
  if ((millis() - lastDebounceMs) > debounceDelayMs) {
    bool pressed = (buttonState == LOW); // INPUT_PULLUP: LOW when pressed
    if (pressed != stableButtonPressed) {
      stableButtonPressed = pressed;
      show8888 = pressed; // pressed => show 8888, otherwise show pot value
    }
  }

  if (show8888) {
    // When button pressed, display 8888
    display.showNumberDecEx(8888, 0, true);
  } else {
    // Default: show potentiometer value (0..9999)
    int raw = analogRead(POT_PIN);
    uint16_t value = mapToDisplayRange(raw);
    display.showNumberDecEx(value, 0, true);
  }
}
