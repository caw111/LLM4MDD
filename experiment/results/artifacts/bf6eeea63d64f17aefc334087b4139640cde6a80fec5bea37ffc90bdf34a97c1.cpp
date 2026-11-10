#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_017\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Buttons connect between their assigned pins and GND.
constexpr uint8_t PLAYER_PINS[4] = {2, 3, 7, 8};
constexpr uint8_t RESET_PIN = 9;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t LED_PIN = 13;

constexpr unsigned long DEBOUNCE_MS = 25;
constexpr unsigned long RESET_HOLD_MS = 3000;
constexpr unsigned int CONFIRM_FREQUENCY_HZ = 2400;
constexpr unsigned long CONFIRM_DURATION_MS = 100;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

struct DebouncedButton {
  uint8_t pin;
  bool rawState;
  bool stableState;
  bool pressed;
  bool released;
  unsigned long changedAt;

  void begin(uint8_t assignedPin) {
    pin = assignedPin;
    pinMode(pin, INPUT_PULLUP);
    rawState = HIGH;
    stableState = HIGH;
    pressed = false;
    released = false;
    changedAt = millis();
  }

  void update(unsigned long now) {
    pressed = false;
    released = false;

    bool reading = digitalRead(pin);
    if (reading != rawState) {
      rawState = reading;
      changedAt = now;
    }

    if (rawState != stableState &&
        (unsigned long)(now - changedAt) >= DEBOUNCE_MS) {
      stableState = rawState;
      pressed = (stableState == LOW);
      released = (stableState == HIGH);
    }
  }
};

DebouncedButton playerButtons[4];
DebouncedButton resetButton;

bool playerEligible[4] = {true, true, true, true};
bool roundLocked = false;
bool resetHeld = false;
bool statisticsClearedThisHold = false;

uint8_t winner = 0;
unsigned long resetPressedAt = 0;
unsigned long playerWins[4] = {0, 0, 0, 0};
unsigned long totalRounds = 0;

#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_017\\direct\\attempt_0\\sketch\\sketch.ino"
void showReady();
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_017\\direct\\attempt_0\\sketch\\sketch.ino"
void resetRound();
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_017\\direct\\attempt_0\\sketch\\sketch.ino"
void printStatistics();
#line 100 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_017\\direct\\attempt_0\\sketch\\sketch.ino"
void clearStatistics();
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_017\\direct\\attempt_0\\sketch\\sketch.ino"
void recordWinner(uint8_t index, unsigned long timestamp);
#line 129 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_017\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 149 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_017\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_017\\direct\\attempt_0\\sketch\\sketch.ino"
void showReady() {
  const uint8_t dashes[4] = {SEG_G, SEG_G, SEG_G, SEG_G};
  display.setSegments(dashes);
}

void resetRound() {
  roundLocked = false;
  winner = 0;
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  showReady();

  for (uint8_t i = 0; i < 4; ++i) {
    playerEligible[i] = false;
  }
}

void printStatistics() {
  Serial.print(F("STATS total="));
  Serial.print(totalRounds);

  for (uint8_t i = 0; i < 4; ++i) {
    Serial.print(F(" P"));
    Serial.print(i + 1);
    Serial.print('=');
    Serial.print(playerWins[i]);
  }
  Serial.println();
}

void clearStatistics() {
  totalRounds = 0;
  for (uint8_t i = 0; i < 4; ++i) {
    playerWins[i] = 0;
  }

  Serial.print(F("STATISTICS_CLEARED timestamp_ms="));
  Serial.println(millis());
  printStatistics();
}

void recordWinner(uint8_t index, unsigned long timestamp) {
  // Lock before performing display or serial operations.
  roundLocked = true;
  winner = index + 1;
  ++playerWins[index];
  ++totalRounds;

  display.showNumberDec(winner, false);
  digitalWrite(LED_PIN, HIGH);
  tone(BUZZER_PIN, CONFIRM_FREQUENCY_HZ, CONFIRM_DURATION_MS);

  Serial.print(F("BUZZ timestamp_ms="));
  Serial.print(timestamp);
  Serial.print(F(" player="));
  Serial.println(winner);
  printStatistics();
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  for (uint8_t i = 0; i < 4; ++i) {
    playerButtons[i].begin(PLAYER_PINS[i]);
  }
  resetButton.begin(RESET_PIN);

  display.setBrightness(7, true);
  showReady();

  Serial.println(F("READY: P1=D2 P2=D3 P3=D7 P4=D8 RESET=D9"));
  Serial.println(F("Short reset: new round; hold reset 3 seconds: clear statistics."));
}

void loop() {
  const unsigned long now = millis();

  resetButton.update(now);
  for (uint8_t i = 0; i < 4; ++i) {
    playerButtons[i].update(now);
  }

  // Reset always takes priority over contestant inputs.
  if (resetButton.pressed) {
    resetHeld = true;
    statisticsClearedThisHold = false;
    resetPressedAt = resetButton.changedAt;
    resetRound();

    Serial.print(F("ROUND_RESET timestamp_ms="));
    Serial.println(resetPressedAt);
  }

  if (resetHeld &&
      resetButton.rawState == LOW &&
      resetButton.stableState == LOW &&
      !statisticsClearedThisHold &&
      (unsigned long)(now - resetPressedAt) >= RESET_HOLD_MS) {
    statisticsClearedThisHold = true;
    clearStatistics();
  }

  if (resetButton.released) {
    resetHeld = false;

    // A contestant holding a button through reset must release it first.
    for (uint8_t i = 0; i < 4; ++i) {
      playerEligible[i] =
          playerButtons[i].stableState == HIGH &&
          playerButtons[i].rawState == HIGH;
    }
    Serial.println(F("READY"));
  }

  if (resetHeld ||
      resetButton.rawState == LOW ||
      resetButton.stableState == LOW) {
    return;
  }

  for (uint8_t i = 0; i < 4; ++i) {
    if (playerButtons[i].released) {
      playerEligible[i] = true;
    }
  }

  if (roundLocked) {
    return;
  }

  int8_t firstPlayer = -1;
  unsigned long greatestAge = 0;

  for (uint8_t i = 0; i < 4; ++i) {
    if (playerEligible[i] && playerButtons[i].pressed) {
      const unsigned long age = now - playerButtons[i].changedAt;

      // Earliest debounced press wins. Same-millisecond ties use player order.
      if (firstPlayer < 0 || age > greatestAge) {
        firstPlayer = i;
        greatestAge = age;
      }
    }
  }

  if (firstPlayer >= 0) {
    recordWinner(
        static_cast<uint8_t>(firstPlayer),
        playerButtons[firstPlayer].changedAt);
  }
}
