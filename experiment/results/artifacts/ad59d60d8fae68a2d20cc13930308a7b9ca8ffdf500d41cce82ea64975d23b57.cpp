#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

#define CLK_PIN 4   // D4
#define DIO_PIN 5   // D5

#define BTN_ADD 2   // D2: add 10 seconds
#define BTN_START 3 // D3: start countdown

#define BUZZER_PIN 6 // D6: buzzer

TM1637Display display(CLK_PIN, DIO_PIN);

// ---------------- Timing/State ----------------
static const unsigned long ADD_INTERVAL_MS = 150;     // debounce / repeat throttle
static const unsigned long START_DEBOUNCE_MS = 150;

unsigned long remainingSeconds = 0;
bool started = false;
bool finished = false;

// For debounce / edge detection
bool lastAddReading = HIGH;
unsigned long lastAddChangeMs = 0;
unsigned long lastAddActionMs = 0;

bool lastStartReading = HIGH;
unsigned long lastStartChangeMs = 0;

// For buzzer
bool buzzerOn = false;
unsigned long buzzerStartMs = 0;

// ---------------- Helpers ----------------
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void showNumber(unsigned long value);
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool readButtonPressed(int pin);
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void startCountdown();
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void handleBuzzer(unsigned long nowMs);
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void showNumber(unsigned long value) {
  // Show remaining seconds on 4-digit TM1637, right-aligned.
  // If value exceeds 9999, it will be clipped by TM1637Display library display() behavior.
  display.showNumberDec((int)value, false, 4, 0);
}

bool readButtonPressed(int pin) {
  // Active-low with INPUT_PULLUP (assumption consistent with wiring typical for Arduino)
  return digitalRead(pin) == LOW;
}

void startCountdown() {
  if (!started) {
    started = true;
    finished = false;
    buzzerOn = false;
    remainingSeconds = remainingSeconds; // keep any previously added amount
  }
}

void handleBuzzer(unsigned long nowMs) {
  if (!finished) return;

  if (!buzzerOn) {
    buzzerOn = true;
    buzzerStartMs = nowMs;
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    if (nowMs - buzzerStartMs >= 2000UL) { // 2 seconds continuous
      digitalWrite(BUZZER_PIN, LOW);
      buzzerOn = false;
    }
  }
}

// ---------------- Setup/Loop ----------------
void setup() {
  pinMode(BTN_ADD, INPUT_PULLUP);
  pinMode(BTN_START, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  display.setBrightness(0x0f); // max brightness
  display.clear();

  // Initial display: show 0 seconds until started / added
  remainingSeconds = 0;
  showNumber(remainingSeconds);

  lastAddReading = digitalRead(BTN_ADD);
  lastAddChangeMs = millis();
  lastAddActionMs = 0;

  lastStartReading = digitalRead(BTN_START);
  lastStartChangeMs = millis();
}

void loop() {
  unsigned long nowMs = millis();

  // -------- Button D2: add 10 seconds --------
  bool addReading = digitalRead(BTN_ADD);

  if (addReading != lastAddReading) {
    lastAddChangeMs = nowMs;
    lastAddReading = addReading;
  }

  // If pressed and stable beyond debounce, apply action throttled
  if (!addReading) { // LOW pressed
    if ((nowMs - lastAddChangeMs) >= ADD_INTERVAL_MS) {
      if ((nowMs - lastAddActionMs) >= ADD_INTERVAL_MS) {
        // Increase by 10 seconds each press event
        remainingSeconds += 10;
        showNumber(remainingSeconds);
        lastAddActionMs = nowMs;

        // If countdown already finished, allow restarting with new value
        if (finished) {
          finished = false;
          started = false; // require D3 to start again
          digitalWrite(BUZZER_PIN, LOW);
        }
      }
    }
  }

  // -------- Button D3: start countdown --------
  bool startReading = digitalRead(BTN_START);

  if (startReading != lastStartReading) {
    lastStartChangeMs = nowMs;
    lastStartReading = startReading;
  }

  // Start on falling edge (press) with debounce
  static bool startLatched = false;
  if (!startReading) { // pressed
    if (!startLatched && (nowMs - lastStartChangeMs) >= START_DEBOUNCE_MS) {
      startLatched = true;
      startCountdown();
    }
  } else {
    startLatched = false;
  }

  // -------- Countdown --------
  static unsigned long lastTickMs = 0;
  if (started && !finished) {
    if (lastTickMs == 0) lastTickMs = nowMs;

    while (nowMs - lastTickMs >= 1000UL) {
      lastTickMs += 1000UL;

      if (remainingSeconds > 0) {
        remainingSeconds--;
      }

      if (remainingSeconds == 0) {
        finished = true;
        started = false; // stop counting once finished
        showNumber(0);
        break;
      } else {
        showNumber(remainingSeconds);
      }
    }
  } else {
    // Reset tick origin when not running
    lastTickMs = 0;
  }

  // -------- Buzzer on finish (2 seconds continuous) --------
  handleBuzzer(nowMs);
}
