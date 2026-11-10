#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t BTN1_PIN = 2;
static const uint8_t BTN2_PIN = 3;
static const uint8_t BTN3_PIN = 4;

static const uint8_t RGB_R_PIN = 5;
static const uint8_t RGB_G_PIN = 6;
static const uint8_t RGB_B_PIN = 7;

// Bargraph A1-A8 on analog pins A1-A8 (Mega has A1..A8)
static const uint8_t BAR_A_PINS[8] = {A1, A2, A3, A4, A5, A6, A7, A8};
static const uint8_t BAR_C_PIN = 44; // C pins all tied to GND in circuit; but keep wiring behavior by controlling A pins only.

static const uint16_t STEP = 32; // 0,32,64,96,128,160,192,224,256->wrap to 0
static const uint8_t MAX_INTENSITY = 255;

static uint8_t redVal = 0;
static uint8_t greenVal = 0;
static uint8_t blueVal = 0;

static uint8_t lastPressedColor = 0; // 1=red,2=green,3=blue

// Debounce
static const uint16_t DEBOUNCE_MS = 50;
static bool btnStableState[3] = {false, false, false}; // for input reads (active HIGH assumed because buttons connect to 5V via pull? wiring shows direct to pin with other side to GND through; so use INPUT_PULLUP invert)
static bool btnLastRead[3] = {false, false, false};
static uint32_t btnLastChangeMs[3] = {0, 0, 0};

#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonPressed(uint8_t idx);
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyRgb();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBargraphForIntensity(uint8_t intensity);
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBargraphForLastAdjustedColor();
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t stepAndWrap(uint8_t v);
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonPressed(uint8_t idx) {
  // Buttons are wired to GND on the other side in the circuit:
  // btnX: one pin to Mega GND via connection, other pin to Mega digital pin.
  // In Wokwi this usually means the pin is pulled high when not pressed, low when pressed.
  // We'll use INPUT_PULLUP and treat LOW as pressed.
  uint8_t pin = (idx == 0) ? BTN1_PIN : (idx == 1) ? BTN2_PIN : BTN3_PIN;
  return (digitalRead(pin) == LOW);
}

static void applyRgb() {
  // Common cathode: higher pin current lights (analogWrite compatible).
  analogWrite(RGB_R_PIN, redVal);
  analogWrite(RGB_G_PIN, greenVal);
  analogWrite(RGB_B_PIN, blueVal);
}

static void setBargraphForIntensity(uint8_t intensity) {
  // Rule: 32 -> 1 LED, 64 -> 2 LEDs, ... 255 -> 8 LEDs.
  // We'll map thresholds at 32,64,...,256.
  uint8_t leds = intensity / 32; // 0..7 for 0..224, 255/32=7
  // Requirement: 255 => 8 LEDs. Adjust:
  if (intensity >= 256) leds = 8; // unreachable
  if (intensity >= 225) leds = 8; // ensures 255 -> 8

  for (uint8_t i = 0; i < 8; i++) {
    // If intensity LED i (1-based) should be lit when i < leds.
    // bargraph1 wired with A pins to Mega; C pins tied to GND.
    digitalWrite(BAR_A_PINS[i], (i < leds) ? HIGH : LOW);
  }
}

static void updateBargraphForLastAdjustedColor() {
  uint8_t intensity = 0;
  if (lastPressedColor == 1) intensity = redVal;
  else if (lastPressedColor == 2) intensity = greenVal;
  else if (lastPressedColor == 3) intensity = blueVal;
  else intensity = 0;

  setBargraphForIntensity(intensity);
}

static uint8_t stepAndWrap(uint8_t v) {
  // Increase by 32; if would exceed 255, wrap to 0.
  uint16_t nv = (uint16_t)v + STEP;
  if (nv > MAX_INTENSITY) return 0;
  return (uint8_t)nv;
}

void setup() {
  // Buttons with debounced logic
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // Initialize debounce states
  for (uint8_t i = 0; i < 3; i++) {
    bool pressed = readButtonPressed(i);
    btnStableState[i] = pressed;
    btnLastRead[i] = pressed;
    btnLastChangeMs[i] = millis();
  }

  // RGB LED pins
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Bargraph pins A1-A8: control by setting A pins; C is tied to GND in the circuit.
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW); // bargraph off at start
  }

  // Off at system start
  redVal = 0;
  greenVal = 0;
  blueVal = 0;
  lastPressedColor = 0;

  // For RGB: analogWrite(0) => off
  applyRgb();

  // For bargraph: intensity 0 => all off
  setBargraphForIntensity(0);
}

void loop() {
  for (uint8_t i = 0; i < 3; i++) {
    bool pressedNow = readButtonPressed(i);
    bool lastRead = btnLastRead[i];

    if (pressedNow != lastRead) {
      btnLastRead[i] = pressedNow;
      btnLastChangeMs[i] = millis();
    } else {
      // If stable long enough and changed from stable state, handle it
      if ((millis() - btnLastChangeMs[i]) >= DEBOUNCE_MS) {
        if (pressedNow != btnStableState[i]) {
          btnStableState[i] = pressedNow;

          // We trigger on press event (transition to pressed)
          if (pressedNow) {
            if (i == 0) { // btn1 -> red
              redVal = stepAndWrap(redVal);
              lastPressedColor = 1;
            } else if (i == 1) { // btn2 -> green
              greenVal = stepAndWrap(greenVal);
              lastPressedColor = 2;
            } else { // btn3 -> blue
              blueVal = stepAndWrap(blueVal);
              lastPressedColor = 3;
            }

            applyRgb();
            updateBargraphForLastAdjustedColor();
          }
        }
      }
    }
  }
}
