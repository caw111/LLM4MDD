#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

#define CLK_PIN 4   // D4
#define DIO_PIN 5   // D5
#define BTN_START_PAUSE 2 // D2
#define BTN_RESET 3        // D3
#define BUZZER_PIN 6       // D6

// TM1637: digits 0..4, we'll display seconds (0..99). For this app we only reach 0..60+.
TM1637Display display(CLK_PIN, DIO_PIN);

// Debounce parameters
static const unsigned long DEBOUNCE_MS = 30;

struct DebouncedButton {
  uint8_t pin;
  bool stableState;          // last stable reading (true=pressed if using INPUT_PULLUP)
  bool lastRawState;
  unsigned long lastChangeMs;

  void begin(uint8_t p) {
    pin = p;
    pinMode(pin, INPUT_PULLUP); // active-low buttons
    stableState = (digitalRead(pin) == LOW);
    lastRawState = stableState;
    lastChangeMs = millis();
  }

  // Returns true on rising edge of "pressed" event (i.e., button becomes pressed)
  bool update() {
    bool rawPressed = (digitalRead(pin) == LOW);

    if (rawPressed != lastRawState) {
      lastRawState = rawPressed;
      lastChangeMs = millis();
    }

    if ((millis() - lastChangeMs) >= DEBOUNCE_MS) {
      if (rawPressed != stableState) {
        stableState = rawPressed;
        // pressed event when transitions to pressed
        return stableState;
      }
    }
    return false;
  }
};

DebouncedButton btnStartPause;
DebouncedButton btnReset;

bool running = false;
unsigned long lastTickMs = 0;
uint32_t secondsCount = 0;

// Beep behavior: when reaching 60 seconds, buzzer sounds 1 second and then keeps counting.
// "Reached 60 seconds" interpreted as on each transition into the 60-second mark while running.
bool beeping = false;
unsigned long beepStartMs = 0;
bool beepArmedFor60 = true; // will re-arm once second count wraps below 60

#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void startBeep1s();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void stopBeep();
#line 75 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void startBeep1s() {
  beeping = true;
  beepStartMs = millis();
  // Use tone for consistent 1-second audible beep
  tone(BUZZER_PIN, 2000); // frequency choice; behavior preserved as "beep 1 second"
}

void stopBeep() {
  noTone(BUZZER_PIN);
  beeping = false;
}

void setup() {
  display.setBrightness(0x0f);

  btnStartPause.begin(BTN_START_PAUSE);
  btnReset.begin(BTN_RESET);

  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  // Initialize display to 0 seconds
  display.showNumberDec((int)secondsCount, false, 4, 0); // no leading zeros, width 4
  lastTickMs = millis();
}

void loop() {
  // Handle button events
  if (btnStartPause.update()) {
    running = !running;
    // No additional side effects specified
    if (running) {
      // Align tick start to avoid immediate extra tick from stale lastTickMs
      lastTickMs = millis();
    }
  }

  if (btnReset.update()) {
    running = false;
    secondsCount = 0;
    // Reset beep state as clearing timer
    stopBeep();
    beepArmedFor60 = true;
    display.showNumberDec((int)secondsCount, false, 4, 0);
    lastTickMs = millis();
  }

  // Buzzer management
  if (beeping) {
    if (millis() - beepStartMs >= 1000UL) {
      stopBeep();
    }
  }

  // Time counting
  if (running) {
    unsigned long now = millis();
    if (now - lastTickMs >= 1000UL) {
      // Handle possible drift by incrementing in steps of 1 second
      unsigned long elapsed = (now - lastTickMs) / 1000UL;
      lastTickMs += elapsed * 1000UL;

      // Update seconds per elapsed seconds
      for (unsigned long i = 0; i < elapsed; i++) {
        secondsCount++;

        // Display current seconds
        display.showNumberDec((int)(secondsCount % 100), false, 4, 0);

        // When reaching 60 seconds, beep 1 second, continue counting afterwards.
        // If user leaves it running long-term, it should beep each time it reaches 60 again.
        // secondsCount increases unbounded; interpret "到达 60 秒" as each time count hits multiples of 60.
        // If you want strictly "the first time", set beepArmedFor60 logic accordingly.
        // Here we implement: beep when secondsCount % 60 == 0 AND not currently beeping.
        if ((secondsCount % 60UL) == 0UL && !beeping) {
          startBeep1s();
        }

        // Keep arming logic for potential future extension (not strictly required with modulo logic)
        // Provided to preserve semantics if modulo logic is later changed.
        if ((secondsCount % 60UL) != 0UL) {
          beepArmedFor60 = true;
        } else {
          beepArmedFor60 = false;
        }
      }
    }
  }
}
