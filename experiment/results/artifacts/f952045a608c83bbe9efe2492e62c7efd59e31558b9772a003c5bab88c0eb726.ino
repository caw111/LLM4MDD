#include <Arduino.h>

// Arduino Mega 2560 wiring.
const uint8_t LED_PINS[3] = {2, 3, 4};       // Red, yellow, green
const uint8_t BUTTON_PINS[3] = {5, 6, 7};    // Stop, back, forward
const uint8_t DATA_PIN = A0;                // 74HC595 DS
const uint8_t LATCH_PIN = A1;               // 74HC595 STCP
const uint8_t CLOCK_PIN = A2;               // 74HC595 SHCP

const uint8_t ROUND_COUNT = 5;
const unsigned long PREPARATION_MS = 3000;
const unsigned long ROUND_MS = 5000;
const unsigned long INTERVAL_MS = 2000;
const unsigned long DEBOUNCE_MS = 25;

// Bit order: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G.
// The display common is wired to +5V, so segments are active LOW.
const uint8_t DIGIT_SEGMENTS[6] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101  // 5
};

struct ButtonState {
  uint8_t rawState;
  uint8_t stableState;
  unsigned long lastChange;
};

enum GameState {
  PREPARATION,
  ACTIVE_ROUND,
  ROUND_INTERVAL,
  GAME_OVER
};

ButtonState buttons[3];
GameState gameState = PREPARATION;
uint8_t roundOrder[ROUND_COUNT];
uint8_t roundIndex = 0;
uint8_t score = 0;
unsigned long phaseStarted = 0;

void turnOffLEDs() {
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(LED_PINS[i], LOW);
  }
}

void displayScore(uint8_t value) {
  const uint8_t outputBits = static_cast<uint8_t>(~DIGIT_SEGMENTS[value]);

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, outputBits);
  digitalWrite(LATCH_PIN, HIGH);
}

void makeRoundOrder() {
  // Include each color once, add two random colors, then shuffle.
  roundOrder[0] = 0;
  roundOrder[1] = 1;
  roundOrder[2] = 2;
  roundOrder[3] = static_cast<uint8_t>(random(3));
  roundOrder[4] = static_cast<uint8_t>(random(3));

  for (int8_t i = ROUND_COUNT - 1; i > 0; --i) {
    const uint8_t j = static_cast<uint8_t>(random(i + 1));
    const uint8_t temporary = roundOrder[i];
    roundOrder[i] = roundOrder[j];
    roundOrder[j] = temporary;
  }
}

// Return one bit per newly debounced press. Holding a button does not
// generate repeated presses or automatically answer the next round.
uint8_t readButtonPresses(unsigned long now) {
  uint8_t pressedMask = 0;

  for (uint8_t i = 0; i < 3; ++i) {
    const uint8_t reading = digitalRead(BUTTON_PINS[i]);

    if (reading != buttons[i].rawState) {
      buttons[i].rawState = reading;
      buttons[i].lastChange = now;
    }

    if (buttons[i].stableState != buttons[i].rawState &&
        now - buttons[i].lastChange >= DEBOUNCE_MS) {
      buttons[i].stableState = buttons[i].rawState;

      if (buttons[i].stableState == LOW) {
        pressedMask |= static_cast<uint8_t>(1U << i);
      }
    }
  }

  return pressedMask;
}

void startRound() {
  turnOffLEDs();
  digitalWrite(LED_PINS[roundOrder[roundIndex]], HIGH);
  phaseStarted = millis();
  gameState = ACTIVE_ROUND;
}

void finishRound() {
  turnOffLEDs();
  ++roundIndex;

  if (roundIndex >= ROUND_COUNT) {
    displayScore(score);
    gameState = GAME_OVER;
    Serial.print(F("Game Over! Your score is "));
    Serial.println(score);
  } else {
    phaseStarted = millis();
    gameState = ROUND_INTERVAL;
  }
}

void setup() {
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  turnOffLEDs();
  displayScore(0);
  Serial.begin(9600);

  // A3 is unused in the supplied circuit.
  randomSeed(static_cast<unsigned long>(analogRead(A3)) ^ micros());
  makeRoundOrder();

  const unsigned long now = millis();
  for (uint8_t i = 0; i < 3; ++i) {
    buttons[i].rawState = digitalRead(BUTTON_PINS[i]);
    buttons[i].stableState = buttons[i].rawState;
    buttons[i].lastChange = now;
  }

  Serial.println(F("Ready Go!"));
  phaseStarted = millis();
  gameState = PREPARATION;
}

void loop() {
  const unsigned long now = millis();
  const uint8_t pressedMask = readButtonPresses(now);

  switch (gameState) {
    case PREPARATION:
      if (now - phaseStarted >= PREPARATION_MS) {
        startRound();
      }
      break;

    case ACTIVE_ROUND: {
      if (now - phaseStarted >= ROUND_MS) {
        Serial.println(F("Time's up!"));
        finishRound();
        break;
      }

      if (pressedMask != 0) {
        const uint8_t correctMask =
            static_cast<uint8_t>(1U << roundOrder[roundIndex]);

        // Simultaneous correct and incorrect buttons count as incorrect.
        if (pressedMask == correctMask) {
          ++score;
          displayScore(score);
          Serial.print(F("Good Job! x"));
          Serial.println(score);
          finishRound();
        } else {
          Serial.println(F("Come On!"));
        }
      }
      break;
    }

    case ROUND_INTERVAL:
      if (now - phaseStarted >= INTERVAL_MS) {
        startRound();
      }
      break;

    case GAME_OVER:
      // Keep all LEDs off and retain the final score until reset.
      break;
  }
}