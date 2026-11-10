#include <Arduino.h>

// Board: Arduino Mega 2560

// LEDs
const uint8_t LED_RED    = 2; // mega:2 -> led_red:A
const uint8_t LED_YELLOW = 3; // mega:3 -> led_yellow:A
const uint8_t LED_GREEN  = 4; // mega:4 -> led_green:A

// Buttons (wired as given)
const uint8_t BTN_STOP     = 5; // mega:5  -> btn_stop:1.l
const uint8_t BTN_BACK     = 6; // mega:6  -> btn_back:1.l
const uint8_t BTN_FORWARD  = 7; // mega:7  -> btn_forward:1.l

// 7-seg via 74HC595 (wired as given)
const uint8_t SR_SHCP = A2; // mega:A2 -> sr1:SHCP
const uint8_t SR_STCP = A1; // mega:A1 -> sr1:STCP
const uint8_t SR_DS   = A0; // mega:A0 -> sr1:DS
// OE is tied to GND; MR tied to VCC (per wiring)

// 74HC595 outputs Q0..Q6 -> sevseg1 segments A..G respectively
// Mapping for common usage: segments A..G bits correspond to Q0..Q6.
// Bit order for shifting we'll use Q0 as bit0 (LSB), Q6 as bit6.

static const uint8_t DIGIT_TO_SEG[10] = {
  // a b c d e f g
  // Using typical 7-seg for common anode/cathode? In most setups, segment control is active HIGH
  // (Wokwi default for 7-seg uses segment on when driven high).
  // If the physical module is opposite polarity, user would need inversion; requirements didn't mention it.
  0b0111111, // 0: a b c d e f on, g off  (a=1,b=1,c=1,d=1,e=1,f=1,g=0) -> 0x3F = 0b0111111
  0b0000110, // 1: b c
  0b1011011, // 2: a b d e g
  0b1001111, // 3: a b c d g
  0b1100110, // 4: b c f g
  0b1101101, // 5: a c d f g
  0b1111101, // 6: a c d e f g
  0b0000111, // 7: a b c
  0b1111111, // 8: all
  0b1101111  // 9: a b c d f g
};

// Debounce
const uint32_t DEBOUNCE_MS = 20;

struct DebouncedButton {
  uint8_t pin;
  bool lastStable;
  bool lastReading;
  uint32_t lastChangeMs;

  void begin(uint8_t p) {
    pin = p;
    pinMode(pin, INPUT_PULLUP); // since buttons wired to GND via other side; matches typical wiring
    lastReading = digitalRead(pin);
    lastStable = lastReading;
    lastChangeMs = millis();
  }

  bool update() {
    bool reading = digitalRead(pin);
    uint32_t now = millis();

    if (reading != lastReading) {
      lastReading = reading;
      lastChangeMs = now;
    }

    if ((now - lastChangeMs) >= DEBOUNCE_MS) {
      if (reading != lastStable) {
        lastStable = reading;
      }
    }
    return lastStable;
  }

  bool isPressed() const {
    // With INPUT_PULLUP, pressed typically pulls to LOW
    return lastStable == LOW;
  }
};

DebouncedButton btnStop, btnBack, btnForward;

// Serial baud framing as specified (handled by Arduino core; default line ending ok)
static void showDigit(uint8_t d) {
  if (d > 9) d = 0;

  uint8_t segBits = DIGIT_TO_SEG[d]; // bit0=A(Q0), bit1=B(Q1), ... bit6=G(Q6)
  // Shift out bit0..bit7 (we only use first 7 bits; Q7 ignored).
  // We'll shift MSB first? We'll explicitly set DS for each bit in sequence and pulse SHCP.

  // Ensure latch low while shifting
  digitalWrite(SR_STCP, LOW);

  // Shift 8 bits, with bit0 mapped to first shifted position that ends up at Q0
  // Using manual shifting: for a typical 74HC595, the first bit shifted ends up at Q0
  // when shifting 8 bits with no inversions and using DS sampled on rising edge.
  // To be safe, we will shift LSB first into the register so that Q0 receives bit0.
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t bitVal = (i < 7) ? ((segBits >> i) & 0x01) : 0; // Q7 unused
    digitalWrite(SR_DS, bitVal ? HIGH : LOW);

    // Rising edge on SHCP
    digitalWrite(SR_SHCP, LOW);
    delayMicroseconds(2);
    digitalWrite(SR_SHCP, HIGH);
    delayMicroseconds(2);
  }

  // Latch
  digitalWrite(SR_STCP, HIGH);
  delayMicroseconds(2);
  digitalWrite(SR_STCP, LOW);
}

static inline void setAllLEDsOff() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
}

static inline void ledRedOn() {
  setAllLEDsOff();
  digitalWrite(LED_RED, HIGH);
}

static inline void ledYellowOn() {
  setAllLEDsOff();
  digitalWrite(LED_YELLOW, HIGH);
}

static inline void ledGreenOn() {
  setAllLEDsOff();
  digitalWrite(LED_GREEN, HIGH);
}

static bool waitForCorrectOrWrongWithin(uint8_t correctBtnPinType,
                                         bool (*isCorrectPressed)(),
                                         bool (*isWrongPressed)(),
                                         uint32_t timeoutMs,
                                         uint32_t *timeOutAtMs) {
  // correctBtnPinType not used directly; kept to preserve behavior signature if extended
  (void)correctBtnPinType;
  uint32_t start = millis();

  while (true) {
    uint32_t now = millis();
    if (timeOutAtMs) *timeOutAtMs = now;

    // Update buttons
    btnStop.update();
    btnBack.update();
    btnForward.update();

    if (isCorrectPressed()) {
      return true;
    }
    if (isWrongPressed()) {
      return false; // wrong pressed -> immediate feedback; allow retry within remaining time
      // However spec: "player can try again within the remaining time."
      // Therefore we cannot return immediately on wrong; we must only signal wrong and continue.
    }

    if ((now - start) >= timeoutMs) {
      return false; // time's up (caller will distinguish)
    }

    // Small sleep to limit CPU; still responsive
    delay(5);
  }
}

// More faithful handling: correct/wrong/time's-up within 5 seconds,
// where wrong prints "Come On!" and player can try again until time ends.
static bool playRound(bool (*isCorrectPressed)(), bool (*isWrongPressed)(), uint32_t timeoutMs, bool &timedOut) {
  uint32_t start = millis();
  timedOut = false;

  while (true) {
    btnStop.update();
    btnBack.update();
    btnForward.update();

    if (isCorrectPressed()) {
      return true;
    }
    if (isWrongPressed()) {
      Serial.println("Come On!");
      // Continue loop so the player can try again if time remains
    }

    if ((millis() - start) >= timeoutMs) {
      timedOut = true;
      Serial.println("Time's up!");
      return false;
    }

    delay(5);
  }
}

static bool isStopPressed()    { return btnStop.isPressed(); }
static bool isBackPressed()    { return btnBack.isPressed(); }
static bool isForwardPressed() { return btnForward.isPressed(); }

static bool isWrongForStop() {
  // Wrong if back or forward pressed while red round expects stop
  return isBackPressed() || isForwardPressed();
}
static bool isWrongForBack() {
  // Wrong if stop or forward pressed while yellow round expects back
  return isStopPressed() || isForwardPressed();
}
static bool isWrongForForward() {
  // Wrong if stop or back pressed while green round expects forward
  return isStopPressed() || isBackPressed();
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  pinMode(SR_SHCP, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_DS, OUTPUT);

  digitalWrite(SR_SHCP, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_DS, LOW);

  setAllLEDsOff();

  btnStop.begin(BTN_STOP);
  btnBack.begin(BTN_BACK);
  btnForward.begin(BTN_FORWARD);

  showDigit(0);

  // Preparation phase: 3 seconds, leds off, score 0, serial "Ready Go!"
  setAllLEDsOff();
  showDigit(0);
  delay(3000);
  Serial.println("Ready Go!");
}

void loop() {
  // Run the full game once: 5 rounds, each round has 2-second interval between LED activations.
  // Requirements don't ask for restarting; after completion we keep LEDs off and show final score.

  static bool gameStarted = false;
  static bool gameDone = false;
  static int score = 0;

  if (gameDone) {
    // Keep showing final score; do nothing else.
    delay(100);
    return;
  }

  if (!gameStarted) {
    gameStarted = true;
    score = 0;

    const uint32_t roundDuration = 5000;  // LEDs stay lit for 5 seconds
    const uint32_t intervalBetween = 2000; // 2 seconds interval between LED activations

    // Prepare RNG
    randomSeed(analogRead(A3));

    // Ensure each LED lights at least once within 5 rounds.
    // We'll create a 5-round sequence by:
    // - Start with one of each LED (3 rounds),
    // - For the remaining 2 rounds, pick randomly among 3 LEDs.
    // This guarantees each LED appears at least once.
    uint8_t rounds[5];
    rounds[0] = 0; // 0=red,1=yellow,2=green
    rounds[1] = 1;
    rounds[2] = 2;
    // Shuffle first three
    for (uint8_t i = 0; i < 3; i++) {
      uint8_t j = random(0, 3);
      uint8_t t = rounds[i];
      rounds[i] = rounds[j];
      rounds[j] = t;
    }
    // Remaining two random
    rounds[3] = random(0, 3);
    rounds[4] = random(0, 3);

    // Run rounds
    for (uint8_t r = 0; r < 5; r++) {
      // Interval between LED activations:
      // For r=0, we are starting immediately after preparation (as per spec after Ready Go).
      // The spec says "each round having a 2-second interval between LED activations".
      // We'll insert 2 seconds before round 0? That would extend beyond spec.
      // So we apply interval before rounds r>0.
      if (r > 0) {
        setAllLEDsOff();
        showDigit(score);
        delay(intervalBetween);
      }

      // Turn on the selected LED and play
      if (rounds[r] == 0) { // red
        ledRedOn();
        bool timedOut = false;
        bool correct = playRound(isStopPressed, isWrongForStop, roundDuration, timedOut);
        if (correct) {
          Serial.print("Good Job! x");
          Serial.println(score);
          score++;
        }
        (void)timedOut;
      } else if (rounds[r] == 1) { // yellow
        ledYellowOn();
        bool timedOut = false;
        bool correct = playRound(isBackPressed, isWrongForBack, roundDuration, timedOut);
        if (correct) {
          Serial.print("Good Job! x");
          Serial.println(score);
          score++;
        }
        (void)timedOut;
      } else { // green
        ledGreenOn();
        bool timedOut = false;
        bool correct = playRound(isForwardPressed, isWrongForForward, roundDuration, timedOut);
        if (correct) {
          Serial.print("Good Job! x");
          Serial.println(score);
          score++;
        }
        (void)timedOut;
      }

      // After each round ends (correct or time's up), turn off LEDs before next interval/round
      setAllLEDsOff();
      showDigit(score);
    }

    // Game End
    setAllLEDsOff();
    showDigit(score);
    Serial.print("Game Over! Your score is ");
    Serial.println(score);

    gameDone = true;
  }

  delay(50);
}