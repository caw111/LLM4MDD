#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_0\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// ---------- Pin mapping from provided circuit (Mega 2560) ----------
static const uint8_t LED_RED    = 2;  // mega:2 -> led_red:A
static const uint8_t LED_YELLOW = 3;  // mega:3 -> led_yellow:A
static const uint8_t LED_GREEN  = 4;  // mega:4 -> led_green:A

static const uint8_t BTN_STOP     = 5; // mega:5 -> btn_stop:1.l
static const uint8_t BTN_BACK     = 6; // mega:6 -> btn_back:1.l
static const uint8_t BTN_FORWARD  = 7; // mega:7 -> btn_forward:1.l

// 74HC595 (sr1) wiring
static const uint8_t SR_DS   = A0; // mega:A0 -> sr1:DS
static const uint8_t SR_STCP = A1; // mega:A1 -> sr1:STCP
static const uint8_t SR_SHCP = A2; // mega:A2 -> sr1:SHCP

// sevseg is 7-seg with segments A..G and using 595 outputs Q0..Q6.
// Connections given:
static const uint8_t SEG_A = 0; // Q0 -> A (Q0 to sevseg1:A)
static const uint8_t SEG_B = 1; // Q1 -> B
static const uint8_t SEG_C = 2; // Q2 -> C
static const uint8_t SEG_D = 3; // Q3 -> D
static const uint8_t SEG_E = 4; // Q4 -> E
static const uint8_t SEG_F = 5; // Q5 -> F
static const uint8_t SEG_G = 6; // Q6 -> G

// ---------- 74HC595 -> Segment mapping assumptions ----------
// From circuit: sr1:MR tied to VCC (reset inactive), OE tied to GND (enabled).
// We'll send bits for segments that should be ON.
// Because the display polarity can vary by wiring, we use "common assumption" (segments ON with HIGH).
// If your display is inverted, you may need to invert segment bits.
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_0\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t segMaskForDigit(uint8_t d);
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_0\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWrite7(uint8_t bits7);
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_0\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showDigit(uint8_t d);
#line 91 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_0\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isButtonPressedRaw(uint8_t pin);
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_0\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool waitForCorrectWithin(uint8_t correctBtnPin, uint32_t windowMs, bool &pressedAny, bool &correctPressed);
#line 146 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_0\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t randomLedIndex();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_0\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 182 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_0\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_0\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t segMaskForDigit(uint8_t d) {
  // Bits correspond to segments A..G mapped to Q0..Q6.
  // Standard 7-seg (no DP). Digit patterns.
  // Order: A B C D E F G.
  // 0: A B C D E F
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  uint8_t A=0,B=1,C=2,D=3,E=4,F=5,G=6;

  auto bit = [&](int seg)->uint8_t { return (1u << seg); };

  uint8_t m = 0;
  switch (d) {
    case 0: m = bit(A)|bit(B)|bit(C)|bit(D)|bit(E)|bit(F); break;
    case 1: m = bit(B)|bit(C); break;
    case 2: m = bit(A)|bit(B)|bit(D)|bit(E)|bit(G); break;
    case 3: m = bit(A)|bit(B)|bit(C)|bit(D)|bit(G); break;
    case 4: m = bit(B)|bit(C)|bit(F)|bit(G); break;
    case 5: m = bit(A)|bit(C)|bit(D)|bit(F)|bit(G); break;
    case 6: m = bit(A)|bit(C)|bit(D)|bit(E)|bit(F)|bit(G); break;
    case 7: m = bit(A)|bit(B)|bit(C); break;
    case 8: m = bit(A)|bit(B)|bit(C)|bit(D)|bit(E)|bit(F)|bit(G); break;
    case 9: m = bit(A)|bit(B)|bit(C)|bit(D)|bit(F)|bit(G); break;
    default: m = 0; break;
  }
  return m;
}

// Push 8-bit value to 74HC595; we only use Q0..Q6.
static void srWrite7(uint8_t bits7) {
  // Shift out MSB first for typical wiring; but our bit positions are Q0..Q6.
  // We'll place bit0 -> Q0, bit1 -> Q1, ..., bit6 -> Q6.
  // We'll shift out 8 bits with MSB first (bit7 to bit0). bit7 is don't care (0).
  uint8_t out = (uint8_t)(bits7 & 0x7F); // keep 7 bits; bit7=0

  digitalWrite(SR_STCP, LOW);
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_SHCP, LOW);
    digitalWrite(SR_DS, (out & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR_SHCP, HIGH);
  }
  digitalWrite(SR_STCP, HIGH);
}

static void showDigit(uint8_t d) {
  uint8_t mask = segMaskForDigit(d);
  // If your hardware uses inverse logic, uncomment the following line:
  // mask = (uint8_t)(~mask) & 0x7F;
  srWrite7(mask);
}

// ---------- Button handling ----------
static bool isButtonPressedRaw(uint8_t pin) {
  // Circuit likely uses internal pull-down? Not shown explicitly.
  // Wiring: btn pins: .1.l connected to mega digital pins, .2.r to GND.
  // That suggests button closes to ground when pressed -> reading LOW when pressed.
  // We'll enable INPUT_PULLUP to make released=HIGH and pressed=LOW.
  return (digitalRead(pin) == LOW);
}

static bool waitForCorrectWithin(uint8_t correctBtnPin, uint32_t windowMs, bool &pressedAny, bool &correctPressed) {
  pressedAny = false;
  correctPressed = false;

  const uint32_t start = millis();
  while (millis() - start < windowMs) {
    // Check each button
    bool pStop = isButtonPressedRaw(BTN_STOP);
    bool pBack = isButtonPressedRaw(BTN_BACK);
    bool pForward = isButtonPressedRaw(BTN_FORWARD);

    if (pStop || pBack || pForward) {
      pressedAny = true;

      // Small debounce to avoid misreads (and to respect "within remaining time")
      delay(15);
      bool pStop2 = isButtonPressedRaw(BTN_STOP);
      bool pBack2 = isButtonPressedRaw(BTN_BACK);
      bool pForward2 = isButtonPressedRaw(BTN_FORWARD);

      bool any2 = pStop2 || pBack2 || pForward2;
      if (any2) {
        uint8_t pressedPin = 255;
        if (pStop2) pressedPin = BTN_STOP;
        else if (pBack2) pressedPin = BTN_BACK;
        else if (pForward2) pressedPin = BTN_FORWARD;

        if (pressedPin == correctBtnPin) {
          correctPressed = true;
        } else {
          correctPressed = false;
        }

        // Wait for release so we don't immediately count again
        while (isButtonPressedRaw(BTN_STOP) || isButtonPressedRaw(BTN_BACK) || isButtonPressedRaw(BTN_FORWARD)) {
          delay(5);
        }
        return true;
      }
    }
    delay(1);
  }

  return false; // timed out
}

// ---------- Game logic ----------
static uint8_t randomLedIndex() {
  // 0: red, 1: yellow, 2: green
  return (uint8_t)(random(0, 3));
}

void setup() {
  // LEDs
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);

  // Buttons
  pinMode(BTN_STOP, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);
  pinMode(BTN_FORWARD, INPUT_PULLUP);

  // 74HC595 control pins
  pinMode(SR_DS, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);
  digitalWrite(SR_DS, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_SHCP, LOW);

  Serial.begin(9600);

  // Initial state: LEDs off, display shows "0"
  showDigit(0);

  // Seed RNG
  randomSeed(analogRead(A3) ^ micros());
}

void loop() {
  // Preparation Phase
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
  showDigit(0);

  Serial.println("Ready Go!");
  uint32_t prepStart = millis();
  while (millis() - prepStart < 3000UL) {
    // keep state; showDigit remains "0"
    showDigit(0);
    delay(5);
  }

  int score = 0;

  // 5 rounds; each round has a 2-second interval between LED activations.
  // That implies: after the first LED activation ends (or timeouts), we wait 2 seconds before next activation.
  for (int round = 0; round < 5; ++round) {
    uint8_t ledIndex = randomLedIndex();

    // Ensure each LED lights at least once during 5 rounds:
    // We'll track usage, and if nearing the end, force missing LEDs when possible.
    // (This preserves required behavior while guaranteeing the constraint.)
    // To implement this without simplifying away the game: selection logic adapts only.
    // Track across rounds:
    static bool used[3] = {false, false, false};
    used[ledIndex] = true;

    // If we can still guarantee all LEDs appear at least once, adjust selection on the fly.
    // If we're in last rounds and some LED not used, force one of missing LEDs.
    int roundsLeft = 5 - round;
    int missingCount = 0;
    for (int i = 0; i < 3; ++i) if (!used[i]) missingCount++;
    if (missingCount > 0 && roundsLeft <= missingCount) {
      // Force choose from missing LEDs
      uint8_t pick = ledIndex;
      // Gather missing
      uint8_t missing[3];
      int k = 0;
      for (int i = 0; i < 3; ++i) if (!used[i]) missing[k++] = (uint8_t)i;
      pick = missing[random(0, k)];
      ledIndex = pick;
      used[ledIndex] = true;
    }

    // Turn on chosen LED; others off.
    digitalWrite(LED_RED, (ledIndex == 0) ? HIGH : LOW);
    digitalWrite(LED_YELLOW, (ledIndex == 1) ? HIGH : LOW);
    digitalWrite(LED_GREEN, (ledIndex == 2) ? HIGH : LOW);

    uint8_t correctBtnPin = BTN_STOP;
    if (ledIndex == 1) correctBtnPin = BTN_BACK;
    if (ledIndex == 2) correctBtnPin = BTN_FORWARD;

    // Keep LED lit for 5 seconds; player must press correct within this time.
    bool pressedAny = false, correctPressed = false;
    bool gotPress = waitForCorrectWithin(correctBtnPin, 5000UL, pressedAny, correctPressed);

    if (gotPress) {
      if (correctPressed) {
        Serial.print("Good Job! x");
        Serial.println(score);
        score += 1;
        if (score > 9) score = 9; // display supports single digit; game scoring per rules is unbounded but display is 0..9
        showDigit((uint8_t)score);
      } else {
        Serial.println("Come On!");
      }
    } else {
      Serial.println("Time's up!");
    }

    // Turn off all LEDs after this activation resolves.
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);

    // 2-second interval between LED activations
    uint32_t t0 = millis();
    while (millis() - t0 < 2000UL) {
      delay(5);
    }

    // Update display during rounds (even if no score change) to reflect current score.
    showDigit((uint8_t)min(score, 9));
  }

  // Game End
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
  showDigit((uint8_t)min(score, 9));
  Serial.print("Game Over! Your score is ");
  Serial.println(score);

  // Stop running forever after game end (no further behavior specified)
  while (true) {
    delay(1000);
  }
}
