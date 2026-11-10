#include <Arduino.h>

// --- Wiring from circuit (Arduino Mega 2560) ---
// Shift registers (74HC595) control 7-seg displays via cascaded registers.
// sr1: A2=SHCP, A1=STCP, A0=DS, OE tied to GND (so we don't drive it)
// sr2 is cascaded: sr1.Q7S -> sr2.DS, and SHCP/STCP shared.

static const uint8_t SR1_SHCP = A2;   // gray
static const uint8_t SR1_STCP = A1;   // purple
static const uint8_t SR1_DS   = A0;   // blue (sr1:DS)

static const uint8_t BTN1_PIN = 2;    // btn1:1.l (green) to mega:2 ; btn1:2.r to GND
static const uint8_t BTN2_PIN = 3;    // btn2:1.l (green) to mega:3 ; btn2:2.r to GND

static const uint8_t POT1_PIN = A3;   // pot1:SIG -> mega:A3
static const uint8_t POT2_PIN = A4;   // pot2:SIG -> mega:A4

// 7-segment patterns for digits 0-9.
// Assumption: "segment ON = 1" as used by prior sketch.
static const uint8_t DIGIT_ON[10] = {
  // bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G, bit7=DP(0)
  0b01111110, // 0
  0b00110000, // 1
  0b01101101, // 2
  0b01111001, // 3
  0b00110011, // 4
  0b01011011, // 5
  0b01011111, // 6
  0b01110000, // 7
  0b01111111, // 8
  0b01111011  // 9
};

static inline uint8_t digitToPattern(uint8_t d) {
  return DIGIT_ON[d % 10];
}

static inline uint8_t readPotDigit(uint8_t pin) {
  // Map 0..1023 -> 0..9
  int v = analogRead(pin);
  v = constrain(v, 0, 1023);
  return (uint8_t)((uint32_t)v * 10UL / 1024UL);
}

static inline unsigned long potToPeriodMs(uint8_t pin) {
  // Map to speed 0.1..1.0 seconds (100..1000 ms)
  int v = analogRead(pin);
  v = constrain(v, 0, 1023);
  float dt = 0.1f + ((float)v / 1023.0f) * 0.9f; // 0.1..1.0
  return (unsigned long)constrain((unsigned long)lroundf(dt * 1000.0f), 100UL, 1000UL);
}

static inline bool readButton(uint8_t pin) {
  // Buttons wired to GND; other side to input pin, use pullup => pressed = LOW
  return digitalRead(pin) == LOW;
}

static void shiftOutTwoDigits(uint8_t tens, uint8_t units) {
  // Cascaded 74HC595:
  // - sr1 drives sevseg1 (tens)
  // - sr2 drives sevseg2 (units)
  //
  // We shift 16 bits MSB-first on SR1_DS, clocking SR1_SHCP.
  // Order: [sr2 byte][sr1 byte] for correct cascade behavior.
  uint8_t sr2Byte = digitToPattern(units);
  uint8_t sr1Byte = digitToPattern(tens);

  uint16_t data = ((uint16_t)sr2Byte << 8) | sr1Byte;

  digitalWrite(SR1_DS, LOW);

  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR1_DS, ((data >> i) & 0x01) ? HIGH : LOW);
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(2);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(2);
  }

  // Latch
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(5);
  digitalWrite(SR1_STCP, LOW);
}

void setup() {
  pinMode(SR1_DS, OUTPUT);
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);

  digitalWrite(SR1_DS, LOW);
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  // Initial display from potentiometers (pot1=tens, pot2=units), 00..99.
  uint8_t tens = readPotDigit(POT1_PIN);
  uint8_t units = readPotDigit(POT2_PIN);

  shiftOutTwoDigits(tens, units);
}

void loop() {
  static int number = 0; // 0..99

  static bool lastBtn1 = false;
  static bool lastBtn2 = false;

  static unsigned long nextRepeat1 = 0;
  static unsigned long nextRepeat2 = 0;

  // Ensure initialization matches "upon powering on or resetting"
  // (some environments may not run setup display fast enough—keep it robust).
  static bool initialized = false;
  if (!initialized) {
    uint8_t tens = readPotDigit(POT1_PIN);
    uint8_t units = readPotDigit(POT2_PIN);
    number = (int)tens * 10 + (int)units;
    shiftOutTwoDigits((uint8_t)(number / 10), (uint8_t)(number % 10));
    initialized = true;
  }

  // Potentiometer adjustment must immediately update display.
  static uint8_t lastTensDigit = 0xFF;
  static uint8_t lastUnitsDigit = 0xFF;

  uint8_t tensDigit = readPotDigit(POT1_PIN);
  uint8_t unitsDigit = readPotDigit(POT2_PIN);

  if (tensDigit != lastTensDigit || unitsDigit != lastUnitsDigit) {
    lastTensDigit = tensDigit;
    lastUnitsDigit = unitsDigit;

    number = (int)tensDigit * 10 + (int)unitsDigit;
    number = constrain(number, 0, 99);

    shiftOutTwoDigits((uint8_t)(number / 10), (uint8_t)(number % 10));
  }

  bool btn1 = readButton(BTN1_PIN);
  bool btn2 = readButton(BTN2_PIN);

  unsigned long now = millis();

  // Edge: single step immediately on press
  if (btn1 && !lastBtn1) {
    number++;
    if (number > 99) number = 0;
    shiftOutTwoDigits((uint8_t)(number / 10), (uint8_t)(number % 10));

    nextRepeat1 = now + potToPeriodMs(POT1_PIN);
  }

  if (btn2 && !lastBtn2) {
    number--;
    if (number < 0) number = 99;
    shiftOutTwoDigits((uint8_t)(number / 10), (uint8_t)(number % 10));

    nextRepeat2 = now + potToPeriodMs(POT2_PIN);
  }

  // Repeat while held down (speed from corresponding potentiometer)
  if (btn1 && now >= nextRepeat1) {
    number++;
    if (number > 99) number = 0;
    shiftOutTwoDigits((uint8_t)(number / 10), (uint8_t)(number % 10));

    nextRepeat1 = now + potToPeriodMs(POT1_PIN);
  }

  if (btn2 && now >= nextRepeat2) {
    number--;
    if (number < 0) number = 99;
    shiftOutTwoDigits((uint8_t)(number / 10), (uint8_t)(number % 10));

    nextRepeat2 = now + potToPeriodMs(POT2_PIN);
  }

  lastBtn1 = btn1;
  lastBtn2 = btn2;

  delay(5);
}