#include <Arduino.h>

static const uint8_t SR1_SHCP = A2;   // gray: SHCP
static const uint8_t SR1_STCP = A1;   // purple: STCP
static const uint8_t SR1_DS   = A0;    // blue: DS

// Buttons (per requirement)
static const uint8_t BTN_INC = 2; // D2 +1
static const uint8_t BTN_DEC = 3; // D3 -1
static const uint8_t BTN_RST = 4; // D4 clear

// Potentiometer ADC input
static const uint8_t POT_ADC = A3; // A3 linear mapping

// Segment map for common-cathode 7-seg using 74HC595 outputs (Q0..Q7 => A..G + dp?)
// Wiring specifies sr1:Q0->A, Q1->B, ... Q5->F, Q6->G, Q7S->(not used here as DS of sr2 via Q7S)
static const uint8_t digitToSeg[10] = {
  // pgfedcba? No, our mapping is A..G:
  // We'll set bits for A,B,C,D,E,F,G respectively in Q0..Q6, and ignore Q7S.
  // Bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G, bit7=DP(0)
  // 0: A B C D E F (no G)
  (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5),
  // 1: B C
  (1<<1) | (1<<2),
  // 2: A B D E G
  (1<<0) | (1<<1) | (1<<3) | (1<<4) | (1<<6),
  // 3: A B C D G
  (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<6),
  // 4: B C F G
  (1<<1) | (1<<2) | (1<<5) | (1<<6),
  // 5: A C D F G
  (1<<0) | (1<<2) | (1<<3) | (1<<5) | (1<<6),
  // 6: A C D E F G
  (1<<0) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6),
  // 7: A B C
  (1<<0) | (1<<1) | (1<<2),
  // 8: A B C D E F G
  (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6),
  // 9: A B C D F G
  (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<5) | (1<<6)
};

static inline void shiftOutByteBoth(uint8_t msbForSr1, uint8_t lsbForSr2) {
  // Daisy-chained: data from MCU goes to sr1 DS->Q? then shifted to sr2 through Q7S.
  // To load both simultaneously, we shift 16 bits: first 8 bits for sr1, next 8 bits for sr2.
  // However the circuit uses SHCP/STCP shared; and Q7S -> sr2 DS.
  // We'll clock 16 bits: for each bit from MSB to LSB? Need consistent order.
  // In Wokwi 74hc595, shift register shifts MSB first when using shiftOut style.
  // We'll implement bit-banging MSB first.
  uint16_t word = (uint16_t(msbForSr1) << 8) | uint16_t(lsbForSr2);

  // Ensure STCP low before shifting
  digitalWrite(SR1_STCP, LOW);

  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR1_SHCP, LOW);
    digitalWrite(SR1_DS, (word & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR1_SHCP, HIGH);
  }

  // Latch
  digitalWrite(SR1_STCP, HIGH);
  // Optional short hold then low
  digitalWrite(SR1_STCP, LOW);
}

static inline uint8_t readPotIntervalMs() {
  // Acceptance: A3原始ADC线性映射重复间隔 100+floor(ADC*900/1023) 毫秒.
  // Range: 100..1000ms
  int adc = analogRead(POT_ADC);
  // ensure 0..1023
  if (adc < 0) adc = 0;
  if (adc > 1023) adc = 1023;
  int interval = 100 + (adc * 900) / 1023; // floor
  return (uint8_t)interval;
}

static inline void displayNumber(int value) {
  value %= 100;
  if (value < 0) value += 100;

  int tens = value / 10;
  int ones = value % 10;

  uint8_t segTens = digitToSeg[tens];
  uint8_t segOnes = digitToSeg[ones];

  shiftOutByteBoth(segTens, segOnes);
}

struct ButtonRepeat {
  bool prevStable = false;     // previous stable press state (pressed=LOW)
  bool pressed = false;        // current stable pressed state
  bool armedNext = false;     // not used, but helps clarity

  uint32_t lastStepTime = 0;  // last step timestamp
};

static inline bool readButtonPressed(uint8_t pin) {
  // INPUT_PULLUP; pressed => LOW
  return digitalRead(pin) == LOW;
}

static void doOneStepIncDec(bool inc) {
  static int number = 0;
  if (inc) {
    number++;
    if (number > 99) number = 0;
  } else {
    number--;
    if (number < 0) number = 99;
  }
  displayNumber(number);
}

static void doReset() {
  static int number = 0;
  number = 0;
  displayNumber(number);
}

// Keep a single number state across calls:
static int g_number = 0;

static inline void stepInc() {
  g_number++;
  if (g_number > 99) g_number = 0;
  displayNumber(g_number);
}
static inline void stepDec() {
  g_number--;
  if (g_number < 0) g_number = 99;
  displayNumber(g_number);
}
static inline void stepReset() {
  g_number = 0;
  displayNumber(g_number);
}

void setup() {
  pinMode(SR1_DS, OUTPUT);
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);

  // Initialize control pins low
  digitalWrite(SR1_DS, LOW);
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);

  pinMode(BTN_INC, INPUT_PULLUP);
  pinMode(BTN_DEC, INPUT_PULLUP);
  pinMode(BTN_RST, INPUT_PULLUP);

  // Pot is analog input
  analogReference(DEFAULT);

  // Initial display "00"
  g_number = 0;
  displayNumber(0);
}

void loop() {
  // Per acceptance: allow timing tolerance ~20ms; we'll debounce lightly.
  // Debounce: small sample interval
  static uint32_t lastDebounceCheck = 0;
  if (millis() - lastDebounceCheck >= 10) {
    lastDebounceCheck = millis();
  }

  // Read current states (low = pressed)
  bool incPressed = readButtonPressed(BTN_INC);
  bool decPressed = readButtonPressed(BTN_DEC);
  bool rstPressed = readButtonPressed(BTN_RST);

  // Priority/interaction rules:
  // - Clear has priority and resets + resets repeat timer.
  // - Add and subtract simultaneously: no step.
  // - Button held: repeat with interval controlled by pot, based on time since last step.
  // - On a press edge: immediately step once, then start repeat timing from that step time.

  // State for repeat timing (single active mode at a time)
  static bool active = false;     // any of inc/dec pressed (excluding simultaneous)
  static bool activeInc = true;  // if active, inc direction
  static uint32_t lastStepTime = 0;

  // Determine effective mode
  bool incDecConflict = (incPressed && decPressed);
  bool shouldInc = (incPressed && !decPressed);
  bool shouldDec = (decPressed && !incPressed);
  bool shouldActive = (shouldInc || shouldDec);

  // For edge detection we store previous effective states
  static bool prevIncEff = false;
  static bool prevDecEff = false;

  bool incEff = shouldInc;
  bool decEff = shouldDec;

  // Clear has absolute priority
  if (rstPressed) {
    // On any time rst is pressed, we should step once on edge, and also keep repeat?
    // Requirement: "清零优先并重置重复计时" and immediate step when pressed.
    // It doesn't ask for repeating clear while held; but "based on button held down" implies clear could repeat too.
    // However it says "清零优先并重置重复计时" (reset repeat timing). We'll implement repeating clear on hold to preserve behavior
    // that number stays 00 anyway; repeat would be redundant. We'll still handle edge for immediate.
    static bool prevRst = false;
    if (!prevRst) {
      stepReset();
    }
    // Reset repeat timers
    active = false;
    lastStepTime = 0;
    prevIncEff = false;
    prevDecEff = false;
    prevRst = true;
    return;
  } else {
    static bool prevRst = false;
    prevRst = false;
  }

  // Handle immediate action on press edge for inc/dec (only when not conflicting)
  uint32_t now = millis();

  // If conflict (both pressed), no step and do not run repeats; also reset timers when entering conflict.
  if (incDecConflict) {
    active = false;
    lastStepTime = 0;
    prevIncEff = incEff;
    prevDecEff = decEff;
    return;
  }

  // When not conflicting and one is pressed
  if (shouldActive) {
    // determine if this is a new press edge
    bool incEdge = incEff && !prevIncEff;
    bool decEdge = decEff && !prevDecEff;

    // Update active direction
    active = true;
    activeInc = shouldInc;

    if (incEdge) {
      stepInc();
      lastStepTime = now; // repeat counts from this step
    } else if (decEdge) {
      stepDec();
      lastStepTime = now;
    } else {
      // Held repeat
      uint32_t interval = readPotIntervalMs(); // changes immediately; affects next due check
      if (lastStepTime == 0) {
        // If somehow missing, align
        lastStepTime = now;
      } else {
        uint32_t elapsed = now - lastStepTime;
        if (elapsed >= interval) {
          if (activeInc) stepInc();
          else stepDec();
          lastStepTime = now; // "间隔从上一次步进算起"
        }
      }
    }

    prevIncEff = incEff;
    prevDecEff = decEff;
    return;
  } else {
    // No inc/dec pressed: stop repeats; clear repeat timers.
    active = false;
    lastStepTime = 0;
    prevIncEff = incEff; // false
    prevDecEff = decEff; // false
    return;
  }
}