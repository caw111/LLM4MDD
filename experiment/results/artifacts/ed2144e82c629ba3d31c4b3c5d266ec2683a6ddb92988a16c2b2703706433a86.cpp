#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

enum Mode {
  MODE_RED = 1,
  MODE_GREEN = 2,
  MODE_BLUE = 3,
  MODE_ALL = 4
};

// Pin mapping (from circuit)
static const uint8_t PIN_POT1 = A3; // pot1:SIG
static const uint8_t PIN_POT2 = A4; // pot2:SIG
static const uint8_t PIN_POT3 = A5; // pot3:SIG

static const uint8_t PIN_BTN1 = 2;  // btn1:1.l/2.r
static const uint8_t PIN_BTN2 = 3;  // btn2:1.l/2.r

static const uint8_t PIN_LED_R = 4; // rgb1:R
static const uint8_t PIN_LED_G = 5; // rgb1:G
static const uint8_t PIN_LED_B = 6; // rgb1:B

static const uint16_t ADC_MAX = 1023;

// Simple button handling with debounce
struct DebouncedButton {
  uint8_t pin;
  bool stableState;
  bool lastReading;
  uint32_t lastChangeMs;

  void begin(uint8_t p) {
    pin = p;
    pinMode(pin, INPUT_PULLUP); // buttons connected to GND when pressed (active LOW)
    stableState = digitalRead(pin) == HIGH;
    lastReading = stableState;
    lastChangeMs = millis();
  }

  // Call frequently; returns true once per press (transition from not-pressed to pressed)
  bool wasPressed() {
    bool reading = digitalRead(pin) == HIGH;

    if (reading != lastReading) {
      lastChangeMs = millis();
      lastReading = reading;
    }

    if ((millis() - lastChangeMs) >= 25) {
      if (stableState != reading) {
        stableState = reading;
        // pressed when stable state becomes LOW (INPUT_PULLUP)
        return (stableState == false);
      }
    }
    return false;
  }
};

static DebouncedButton btn1;
static DebouncedButton btn2;

static Mode currentMode = MODE_RED;

#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t readPotByte(uint8_t potPin);
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyLed(uint8_t rVal, uint8_t gVal, uint8_t bVal);
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void turnOffLed();
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 122 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t readPotByte(uint8_t potPin) {
  uint16_t v = analogRead(potPin);
  // Map 0..1023 to 0..255
  return (uint32_t)v * 255UL / ADC_MAX;
}

static void applyLed(uint8_t rVal, uint8_t gVal, uint8_t bVal) {
  // Always update immediately based on current mode
  uint8_t rOut = 0, gOut = 0, bOut = 0;

  switch (currentMode) {
    case MODE_RED:
      rOut = rVal;
      break;
    case MODE_GREEN:
      gOut = gVal;
      break;
    case MODE_BLUE:
      bOut = bVal;
      break;
    case MODE_ALL:
      rOut = rVal;
      gOut = gVal;
      bOut = bVal;
      break;
    default:
      break;
  }

  // rgb1 is common cathode -> drive pins HIGH to increase brightness
  analogWrite(PIN_LED_R, rOut);
  analogWrite(PIN_LED_G, gOut);
  analogWrite(PIN_LED_B, bOut);
}

static void turnOffLed() {
  analogWrite(PIN_LED_R, 0);
  analogWrite(PIN_LED_G, 0);
  analogWrite(PIN_LED_B, 0);
}

void setup() {
  // RGB pins
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);

  // Buttons
  btn1.begin(PIN_BTN1);
  btn2.begin(PIN_BTN2);

  // Potentiometers are analog input pins; no need to configure beyond defaults

  // Initialization: RGB LED off
  turnOffLed();
  currentMode = MODE_RED; // defined reset behavior; LED remains off until updated
}

void loop() {
  bool p1 = btn1.wasPressed();
  bool p2 = btn2.wasPressed();

  // Read potentiometer values (so update reflects current potentiometers after any button press)
  uint8_t rVal = readPotByte(PIN_POT1);
  uint8_t gVal = readPotByte(PIN_POT2);
  uint8_t bVal = readPotByte(PIN_POT3);

  if (p1) {
    // btn1 cycles modes: 1->2->3->4->1...
    switch (currentMode) {
      case MODE_RED:   currentMode = MODE_GREEN; break;
      case MODE_GREEN: currentMode = MODE_BLUE;  break;
      case MODE_BLUE:  currentMode = MODE_ALL;   break;
      case MODE_ALL:   currentMode = MODE_RED;   break;
      default:         currentMode = MODE_RED;   break;
    }
    applyLed(rVal, gVal, bVal);
    return;
  }

  if (p2) {
    // btn2 resets to off and sets mode to Mode 1
    currentMode = MODE_RED;
    turnOffLed();
    // Immediately update after reset as well (reflect potentiometer values in Mode 1)
    // Requirement: "The RGB LED should immediately update to reflect the current mode and potentiometer values after each button press"
    applyLed(rVal, gVal, bVal);
    return;
  }

  // Display Update: update immediately to reflect potentiometer adjustments as well
  // (Always applying keeps response immediate to analog changes.)
  applyLed(rVal, gVal, bVal);
}
