#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

#define BTN_INC_PIN 2   // D2
#define BTN_DEC_PIN 3   // D3
#define LED_PIN     13  // D13
#define BUZZER_PIN  6   // D6
#define TM1637_CLK  4   // D4
#define TM1637_DIO  5   // D5

TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonStable(uint8_t pin);
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool waitButtonRelease(uint8_t pin);
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonStable(uint8_t pin) {
  // Simple stable read with small debounce delay
  if (digitalRead(pin) != LOW) return false;
  delay(20);
  return digitalRead(pin) == LOW;
}

static bool waitButtonRelease(uint8_t pin) {
  // Wait until button released (high)
  while (digitalRead(pin) == LOW) {
    // keep system responsive-ish
    delay(5);
  }
  return true;
}

void setup() {
  pinMode(BTN_INC_PIN, INPUT_PULLUP);
  pinMode(BTN_DEC_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);

  tm1637.setBrightness(0x0f, true);
  tm1637.clearDisplay();

  // Initialize count and state
  // Counter starts at 0 by default (even => LED ON)
  // Requirement doesn't specify reset behavior beyond "long press D3 to reset".
}

void loop() {
  static int count = 0;           // current value shown on 7-seg
  static bool locked = false;    // alarm locks counting at 15
  static bool buzzerOn = false;  // internal toggle to create audible alarm

  // Always handle display and LED based on current count (even/odd)
  // even => LED ON, odd => LED OFF
  digitalWrite(LED_PIN, (count % 2 == 0) ? HIGH : LOW);

  // Display current count (0..15)
  // TM1637Display supports digits 0-15.
  tm1637.showNumberDec(count, false, 1);

  // If locked, we beep and wait for long press D3 to reset
  if (locked) {
    // Audible alarm: continuous tone (with a simple on/off toggling for robustness)
    // Use 1000 Hz; keep it active until reset.
    tone(BUZZER_PIN, 1000);

    // Detect long press on D3: hold LOW for a duration
    // If not held long enough, ignore.
    if (digitalRead(BTN_DEC_PIN) == LOW) {
      unsigned long start = millis();
      while (digitalRead(BTN_DEC_PIN) == LOW) {
        delay(5);
      }
      unsigned long held = millis() - start;

      // long press threshold (ms)
      if (held >= 800) {
        // Reset
        locked = false;
        noTone(BUZZER_PIN);
        buzzerOn = false;
        count = 0;
      }
    }
    return;
  }

  // Not locked: D2 increments, D3 decrements
  // Trigger on button press (edge), with debounce.
  if (readButtonStable(BTN_INC_PIN)) {
    // Wait release to avoid auto-repeat from single press
    waitButtonRelease(BTN_INC_PIN);

    // Increment count (preserve behavior: stop and lock at 15)
    count++;
    if (count >= 15) {
      count = 15;

      // When count reaches 15: alarm and lock
      locked = true;
      noTone(BUZZER_PIN);
      // Next loop iteration will start tone and wait for long press
    }
  } else if (readButtonStable(BTN_DEC_PIN)) {
    // Wait release to avoid auto-repeat from single press
    waitButtonRelease(BTN_DEC_PIN);

    // Decrement count
    count--;
    if (count < 0) count = 0;
  }

  // Small delay to reduce switch bounce / loop jitter
  delay(5);
}
