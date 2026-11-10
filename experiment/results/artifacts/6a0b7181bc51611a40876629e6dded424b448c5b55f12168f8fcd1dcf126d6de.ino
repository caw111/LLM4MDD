#include <Arduino.h>
#include <TM1637Display.h>

#define BTN_INC_PIN 2
#define BTN_DEC_PIN 3

#define BUZZER_PIN 6

#define TM1637_CLK_PIN 4
#define TM1637_DIO_PIN 5

// Counting range
static const int COUNT_MIN = 0;
static const int COUNT_MAX = 20;

TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

// Simple debouncing
static const unsigned long DEBOUNCE_MS = 30;

int countValue = 0;
bool alarmActive = false;

int lastBtnIncState = HIGH;
int lastBtnDecState = HIGH;

unsigned long lastIncChangeMs = 0;
unsigned long lastDecChangeMs = 0;

unsigned long lastAlarmToggleMs = 0;
bool alarmBuzzerOn = false;

// 7-seg digit mapping (use TM1637 library's default digit handling)
void updateDisplay() {
  display.showNumberDecEx(countValue, 0b01000000, true, 2); 
  // Above: display.showNumberDecEx(num, dots, leadingZeros=false, length)
  // Many TM1637 libs use length as digits; set to 2 for 0-20.
}

void startAlarm() {
  alarmActive = true;
  // Force buzzer immediate behavior on next update
  lastAlarmToggleMs = 0;
  alarmBuzzerOn = false;
  digitalWrite(BUZZER_PIN, LOW);
}

void stopAlarm() {
  alarmActive = false;
  alarmBuzzerOn = false;
  digitalWrite(BUZZER_PIN, LOW);
}

void handleBuzzerAlarm() {
  if (!alarmActive) return;

  // Beep pattern while at boundary: toggle every 200ms (on/off)
  const unsigned long PERIOD_MS = 200;

  if (lastAlarmToggleMs == 0) {
    lastAlarmToggleMs = millis();
    // Start with ON
    alarmBuzzerOn = true;
    digitalWrite(BUZZER_PIN, HIGH);
    return;
  }

  unsigned long now = millis();
  if (now - lastAlarmToggleMs >= PERIOD_MS) {
    lastAlarmToggleMs = now;
    alarmBuzzerOn = !alarmBuzzerOn;
    digitalWrite(BUZZER_PIN, alarmBuzzerOn ? HIGH : LOW);
  }
}

bool readButtonFallingEdge(int pin, int &lastState, unsigned long &lastChangeMs) {
  // Returns true once when button press (HIGH->LOW) is detected with debounce
  int current = digitalRead(pin);

  unsigned long now = millis();
  if (current != lastState) {
    lastChangeMs = now;
    lastState = current;
  }

  // Debounce: accept state only if stable for DEBOUNCE_MS
  // We'll check the pin again after debounce window by relying on lastChangeMs
  if ((now - lastChangeMs) >= DEBOUNCE_MS) {
    // If stable and currently LOW, treat it as pressed.
    // We must ensure it's a falling edge: compare with previous stable state
    // To keep logic simple and reliable, use lastState to detect edge:
    // lastState tracks the raw state, so we use a one-shot gating.
  }

  return false;
}

// More robust edge detection with one-shot gate per button
bool incPressedOnce() {
  static bool incArmed = true;
  int current = digitalRead(BTN_INC_PIN);

  if (incArmed && current == LOW) {
    // debounce: wait for stability
    unsigned long start = millis();
    while (millis() - start < DEBOUNCE_MS) {
      if (digitalRead(BTN_INC_PIN) != LOW) break;
      delay(1);
    }
    // Confirm still pressed
    if (digitalRead(BTN_INC_PIN) == LOW) {
      incArmed = false;
      return true;
    }
  }

  if (current == HIGH) {
    incArmed = true;
  }

  return false;
}

bool decPressedOnce() {
  static bool decArmed = true;
  int current = digitalRead(BTN_DEC_PIN);

  if (decArmed && current == LOW) {
    unsigned long start = millis();
    while (millis() - start < DEBOUNCE_MS) {
      if (digitalRead(BTN_DEC_PIN) != LOW) break;
      delay(1);
    }
    if (digitalRead(BTN_DEC_PIN) == LOW) {
      decArmed = false;
      return true;
    }
  }

  if (current == HIGH) {
    decArmed = true;
  }

  return false;
}

void applyAlarmLogicAfterChange() {
  // Alarm whenever count hits boundary, and forbid further out-of-range movement.
  if (countValue <= COUNT_MIN) {
    countValue = COUNT_MIN;
    startAlarm();
  } else if (countValue >= COUNT_MAX) {
    countValue = COUNT_MAX;
    startAlarm();
  } else {
    stopAlarm();
  }
}

void handleIncrement() {
  // If already at max, alarm and do not change value.
  if (countValue >= COUNT_MAX) {
    countValue = COUNT_MAX;
    startAlarm();
    return;
  }
  countValue++;
  applyAlarmLogicAfterChange();
}

void handleDecrement() {
  // If already at min, alarm and do not change value.
  if (countValue <= COUNT_MIN) {
    countValue = COUNT_MIN;
    startAlarm();
    return;
  }
  countValue--;
  applyAlarmLogicAfterChange();
}

void setup() {
  pinMode(BTN_INC_PIN, INPUT_PULLUP);
  pinMode(BTN_DEC_PIN, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  display.setBrightness(0x0f, true); // max-ish brightness, fixed

  updateDisplay();
  stopAlarm();
}

void loop() {
  // Real-time display update (ensures immediate reflect if count changes)
  updateDisplay();

  // Button handling with edge + debounce
  if (incPressedOnce()) {
    handleIncrement();
  }

  if (decPressedOnce()) {
    handleDecrement();
  }

  // Buzzer alarm behavior at boundaries
  handleBuzzerAlarm();

  // If alarm is active, continue beeping until count moves away from boundary.
  // That is enforced in handleIncrement/handleDecrement via applyAlarmLogicAfterChange().
}