#include <Arduino.h>

static const uint8_t SR1_SHCP = A2;   // gray
static const uint8_t SR1_STCP = A1;   // purple
static const uint8_t SR1_DS   = A0;    // blue (sr1:DS)

static const uint8_t SR2_SHCP = A2;   // wired same as sr1
static const uint8_t SR2_STCP = A1;   // wired same as sr1
static const uint8_t SR2_DS   = A0;    // not used directly; sr1:Q7S -> sr2:DS

static const uint8_t SR_OE = A3;      // not used; OE tied to GND (sr1:OE to GND, sr2:OE to GND)
                                    // We'll drive nothing here.

static const uint8_t BTN1_PIN = 2;    // btn1:1.l (green) to mega:2 ; btn1:2.r to GND
static const uint8_t BTN2_PIN = 3;    // btn2:1.l (green) to mega:3 ; btn2:2.r to GND

static const uint8_t POT1_PIN = A3;   // pot1:SIG -> mega:A3
static const uint8_t POT2_PIN = A4;   // pot2:SIG -> mega:A4

// 7-seg segment patterns for digits 0-9, assuming common anode display would be inverted.
// In the provided wiring, outputs map to segments without mention of inversion.
// We'll generate both patterns and pick at runtime based on observed stability is impossible.
// So we assume "segments ON = 1" with digit map below (common cathode behavior).
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

// Optional inverted patterns (common anode behavior), unused by default.
static const uint8_t DIGIT_OFF[10] = {
  (uint8_t)~DIGIT_ON[0],
  (uint8_t)~DIGIT_ON[1],
  (uint8_t)~DIGIT_ON[2],
  (uint8_t)~DIGIT_ON[3],
  (uint8_t)~DIGIT_ON[4],
  (uint8_t)~DIGIT_ON[5],
  (uint8_t)~DIGIT_ON[6],
  (uint8_t)~DIGIT_ON[7],
  (uint8_t)~DIGIT_ON[8],
  (uint8_t)~DIGIT_ON[9]
};

static uint8_t digitToPattern(uint8_t d) {
  return DIGIT_ON[d % 10];
}

static uint8_t readPotSpeedIndex(uint8_t pin) {
  // 0..1023 -> speed 0.1 to 1.0 seconds
  int v = analogRead(pin);
  if (v < 0) v = 0;
  if (v > 1023) v = 1023;
  // Map to 0.1..1.0
  // dt = 0.1 + (v/1023)*(0.9)
  // Use float to preserve mapping behavior closely.
  float dt = 0.1f + ( (float)v / 1023.0f ) * 0.9f;
  // store in milliseconds with rounding
  return (uint8_t)constrain((int)lroundf(dt * 1000.0f), 100, 1000);
}

static unsigned long potToPeriodMs(uint8_t pin) {
  int v = analogRead(pin);
  if (v < 0) v = 0;
  if (v > 1023) v = 1023;
  float dt = 0.1f + ( (float)v / 1023.0f ) * 0.9f; // 0.1..1.0
  return (unsigned long)constrain((unsigned long)lroundf(dt * 1000.0f), 100UL, 1000UL);
}

static void shiftOutTwoDigits(uint8_t tens, uint8_t units) {
  // Data shifting with cascaded 74HC595:
  // - sr1 output Q7S -> sr2 DS
  // - We need to shift out 16 bits total in order.
  //
  // With two cascaded shift registers:
  // If we clock bits MSB-first through a chain, the first 8 bits shifted end up in the last register (sr2),
  // and the last 8 bits end up in the first register (sr1).
  //
  // Typically: shift 16 bits where the first byte is for sr2 and second for sr1.
  // We'll do: [sr2 byte][sr1 byte].
  //
  // sr1 drives sevseg1 (tens digit), sr2 drives sevseg2 (units digit) as per wiring:
  // sr1:* -> sevseg1 segments A-G
  // sr2:* -> sevseg2 segments A-G
  uint8_t sr2Byte = digitToPattern(units); // right display
  uint8_t sr1Byte = digitToPattern(tens);  // left display

  // We will use manual shifting with SPI-like bit-banging on the same DS (sr1:DS) line.
  // SHCP and STCP are shared in the circuit.
  // Ensure pins are in correct idle state.
  digitalWrite(SR1_DS, LOW);

  // Shift out 16 bits, MSB first
  // sr2Byte first (goes to sr2), then sr1Byte.
  uint16_t data = ((uint16_t)sr2Byte << 8) | sr1Byte;

  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR1_DS, (data >> i) & 0x01 ? HIGH : LOW);
    // SHCP rising edge
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

static uint8_t readPotDigit(uint8_t pin) {
  // Map pot 0..1023 -> 0..9
  int v = analogRead(pin);
  v = constrain(v, 0, 1023);
  return (uint8_t)((uint32_t)v * 10UL / 1024UL);
}

static bool readButton(uint8_t pin) {
  // Buttons wired to GND; other side to input pin.
  // Use internal pullup so pressed -> LOW.
  return digitalRead(pin) == LOW;
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

  analogReadResolution(10);

  // Initial display determined by potentiometers:
  // pot1 -> tens, pot2 -> units.
  uint8_t tens = readPotDigit(POT1_PIN);
  uint8_t units = readPotDigit(POT2_PIN);

  // Ensure number between 00 and 99 (guaranteed by digit range)
  // Update display immediately.
  shiftOutTwoDigits(tens, units);
}

void loop() {
  static int number = 0; // 0..99
  static bool initialized = false;

  static bool lastBtn1 = false;
  static bool lastBtn2 = false;

  static unsigned long nextRepeat1 = 0;
  static unsigned long nextRepeat2 = 0;

  static unsigned long lastPotRead1Ms = 0;
  static unsigned long lastPotRead2Ms = 0;

  // Initialize number from potentiometers once (after setup, first loop).
  if (!initialized) {
    uint8_t tens = readPotDigit(POT1_PIN);
    uint8_t units = readPotDigit(POT2_PIN);
    number = (int)tens * 10 + (int)units;
    shiftOutTwoDigits((uint8_t)(number / 10), (uint8_t)(number % 10));
    initialized = true;
  }

  // Read current potentiometer digits and update display immediately to reflect changes.
  // Requirement: "immediately update ... after each ... potentiometer adjustment."
  // We will treat any change in mapped digit as adjustment.
  static uint8_t lastTensDigit = 0xFF;
  static uint8_t lastUnitsDigit = 0xFF;

  uint8_t tensDigit = readPotDigit(POT1_PIN);
  uint8_t unitsDigit = readPotDigit(POT2_PIN);

  if (tensDigit != lastTensDigit || unitsDigit != lastUnitsDigit) {
    lastTensDigit = tensDigit;
    lastUnitsDigit = unitsDigit;
    number = (int)tensDigit * 10 + (int)unitsDigit;

    // Keep within 00..99
    if (number < 0) number = 0;
    if (number > 99) number = 99;

    shiftOutTwoDigits((uint8_t)(number / 10), (uint8_t)(number % 10));
  }

  bool btn1 = readButton(BTN1_PIN);
  bool btn2 = readButton(BTN2_PIN);

  // On press edges: increment/decrement once immediately.
  unsigned long now = millis();

  if (btn1 && !lastBtn1) {
    number++;
    if (number > 99) number = 0;
    shiftOutTwoDigits((uint8_t)(number / 10), (uint8_t)(number % 10));

    // Schedule repeating at speed determined by pot1 (held down)
    unsigned long period = potToPeriodMs(POT1_PIN);
    nextRepeat1 = now + period;
  }
  if (!btn1 && lastBtn1) {
    // stop repeating
  }

  if (btn2 && !lastBtn2) {
    number--;
    if (number < 0) number = 99;
    shiftOutTwoDigits((uint8_t)(number / 10), (uint8_t)(number % 10));

    unsigned long period = potToPeriodMs(POT2_PIN);
    nextRepeat2 = now + period;
  }

  // Handle repeat while held down:
  // speed continuously determined by potentiometer values (per rules).
  // We'll compute period fresh at each repeat firing, not only once.
  if (btn1 && now >= nextRepeat1) {
    number++;
    if (number > 99) number = 0;
    shiftOutTwoDigits((uint8_t)(number / 10), (uint8_t)(number % 10));

    unsigned long period = potToPeriodMs(POT1_PIN);
    nextRepeat1 = now + period;
  }
  if (btn2 && now >= nextRepeat2) {
    number--;
    if (number < 0) number = 99;
    shiftOutTwoDigits((uint8_t)(number / 10), (uint8_t)(number % 10));

    unsigned long period = potToPeriodMs(POT2_PIN);
    nextRepeat2 = now + period;
  }

  lastBtn1 = btn1;
  lastBtn2 = btn2;

  // Small delay for stability
  delay(5);
}