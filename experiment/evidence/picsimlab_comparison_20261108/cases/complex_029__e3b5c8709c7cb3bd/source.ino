#include <Arduino.h>
#include <TM1637Display.h>

#define BTN_INC_PIN 2   // D2
#define BTN_DEC_PIN 3   // D3
#define LED_PIN     13  // D13
#define BUZZER_PIN  6   // D6
#define TM1637_CLK  4   // D4
#define TM1637_DIO  5   // D5

TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

static bool readButtonStable(uint8_t pin) {
  // Simple stable read with small debounce delay
  if (digitalRead(pin) != LOW) return false;
  delay(20);
  return digitalRead(pin) == LOW;
}

static void waitButtonRelease(uint8_t pin) {
  // Wait until button released (high)
  while (digitalRead(pin) == LOW) {
    delay(5);
  }
}

void setup() {
  pinMode(BTN_INC_PIN, INPUT_PULLUP);
  pinMode(BTN_DEC_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);

  tm1637.setBrightness(0x0f, true);
  // No clearDisplay() in this library version; just set initial number.
  tm1637.showNumberDec(0, false, 1);
}

void loop() {
  static int count = 0;           // current value shown on 7-seg (0..15)
  static bool locked = false;    // alarm locks counting at 15
  static bool resetArmed = true; // internal guard for long-press reset

  // LED and display always reflect current count.
  digitalWrite(LED_PIN, (count % 2 == 0) ? HIGH : LOW);
  tm1637.showNumberDec(count, false, 1);

  if (locked) {
    // Alarm: keep buzzing until long-press D3 resets.
    tone(BUZZER_PIN, 1000);

    if (digitalRead(BTN_DEC_PIN) == LOW) {
      if (resetArmed) {
        resetArmed = false;
        unsigned long start = millis();
        while (digitalRead(BTN_DEC_PIN) == LOW) {
          delay(5);
        }
        unsigned long held = millis() - start;

        // long press threshold (ms)
        if (held >= 800) {
          locked = false;
          noTone(BUZZER_PIN);
          resetArmed = true;
          count = 0;
        } else {
          // Short press does nothing in locked state.
          resetArmed = true;
        }
      }
    } else {
      resetArmed = true;
    }

    return;
  }

  noTone(BUZZER_PIN);

  // Not locked: D2 increments, D3 decrements with debounce and wait-for-release.
  if (readButtonStable(BTN_INC_PIN)) {
    waitButtonRelease(BTN_INC_PIN);

    count++;
    if (count >= 15) {
      count = 15;
      locked = true; // next loop iteration will start tone and wait for long-press reset
    }
  } else if (readButtonStable(BTN_DEC_PIN)) {
    waitButtonRelease(BTN_DEC_PIN);

    count--;
    if (count < 0) count = 0;
  }

  delay(5);
}