#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_024\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Wiring per requirements:
// Button on D2
// LED on D13
// TM1637: CLK D4, DIO D5
const uint8_t BTN_PIN = 2;
const uint8_t LED_PIN = 13;

const uint8_t TM1637_CLK_PIN = 4;
const uint8_t TM1637_DIO_PIN = 5;

TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

static volatile bool btnLatched = false;

// Debounce parameters
static const unsigned long DEBOUNCE_MS = 30;
static unsigned long lastDebounceTime = 0;
static int lastButtonState = HIGH;

// Counter
static long counterValue = 0;

#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_024\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_024\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_024\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  display.setBrightness(0x0f);
  display.showNumberDec(0, false, 4);
}

void loop() {
  int reading = digitalRead(BTN_PIN);

  // Detect button press (falling edge since INPUT_PULLUP)
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
    lastButtonState = reading;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    // Stable state
    // Button pressed when reading == LOW
    static bool prevStable = false; // false: not pressed, true: pressed
    bool stablePressed = (reading == LOW);

    if (stablePressed && !prevStable) {
      // Button just pressed
      counterValue++;

      // LED parity toggle based on odd/even
      if (counterValue & 1L) {
        digitalWrite(LED_PIN, HIGH);
      } else {
        digitalWrite(LED_PIN, LOW);
      }

      // Update TM1637 display
      // Show as signed decimal (4 digits). If overflow, TM1637 truncates.
      display.showNumberDec((int)counterValue, true, 4);
    }
    prevStable = stablePressed;
  }
}
