#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_10\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t potPin = A3;   // pot1:SIG -> mega:A3
static const uint8_t btnPin = 12;   // btn1:1.l -> mega:12 (left)
static const uint8_t ledPins[10] = {
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11
}; // bargraph1 A1..A10 -> mega:2..11 (A1=pin2, A10=pin11)

static bool reversedOrder = false;

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_10\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int mapPotToLEDCount(int potValue);
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_10\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarGraph(int litCount, bool reverse);
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_10\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_10\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_10\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int mapPotToLEDCount(int potValue) {
  // Bar graph LEDs: 0..10 lit depending on potentiometer
  // Ensure "off on power-up": we will set based on current reading, but update in loop.
  // Map 0..1023 -> 0..10
  if (potValue < 0) potValue = 0;
  if (potValue > 1023) potValue = 1023;
  long scaled = (long)potValue * 10L;
  return (int)(scaled / 1023L);
}

static void setBarGraph(int litCount, bool reverse) {
  if (litCount < 0) litCount = 0;
  if (litCount > 10) litCount = 10;

  for (int i = 0; i < 10; i++) {
    digitalWrite(ledPins[i], LOW);
  }

  if (!reverse) {
    // Original sequence: start from first LED (A1 -> pin2), fill upwards
    for (int i = 0; i < litCount; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
  } else {
    // Reversed sequence: start from last LED (A10 -> pin11), fill downwards
    for (int i = 0; i < litCount; i++) {
      digitalWrite(ledPins[9 - i], HIGH);
    }
  }
}

void setup() {
  for (int i = 0; i < 10; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // off at power-up/reset
  }

  pinMode(btnPin, INPUT_PULLUP); // button to GND at btn1:2.r -> mega:GND.1
  // No pullups needed elsewhere; bargraph uses separate GND pins.
}

void loop() {
  // Read button with debounce and toggle order on press
  static bool lastStableState = HIGH; // since INPUT_PULLUP: not pressed = HIGH
  static bool lastReading = HIGH;
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceMs = 30;

  bool reading = digitalRead(btnPin);
  if (reading != lastReading) {
    lastDebounceTime = millis();
    lastReading = reading;
  }

  if (millis() - lastDebounceTime >= debounceMs) {
    if (reading != lastStableState) {
      lastStableState = reading;
      // Button pressed: transition to LOW
      if (lastStableState == LOW) {
        reversedOrder = !reversedOrder;
      }
    }
  }

  // Potentiometer interaction: increase -> more LEDs, decrease -> fewer LEDs
  int potValue = analogRead(potPin);
  int litCount = mapPotToLEDCount(potValue);

  // Apply bar graph state according to current potentiometer value and current order
  setBarGraph(litCount, reversedOrder);
}
