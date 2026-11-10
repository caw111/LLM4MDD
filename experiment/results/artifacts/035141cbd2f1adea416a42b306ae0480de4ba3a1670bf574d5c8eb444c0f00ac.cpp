#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// ---------- Pin mapping (from provided wiring) ----------
// Shift register 1 (sr1) -> sevseg1
static const uint8_t SR1_SHCP = A2; // gray
static const uint8_t SR1_STCP = A1; // purple
static const uint8_t SR1_DS   = A0; // blue
static const uint8_t SR1_OE   = 0;  // tied LOW in circuit (sr1:OE -> GND)

// Shift register 2 (sr2) is chained
static const uint8_t SR2_SHCP = A2; // same SHCP line (gray between sr1 and sr2)
static const uint8_t SR2_STCP = A1; // same STCP line (purple between sr1 and sr2)
static const uint8_t SR2_DS   = 0;  // driven by sr1 Q7S (wired), so no direct MCU pin

// Buttons
static const uint8_t BTN1_PIN = 2; // mega:2 <-> btn1
static const uint8_t BTN2_PIN = 3; // mega:3 <-> btn2

// Potentiometers
static const uint8_t POT1_PIN = A3; // pot1:SIG
static const uint8_t POT2_PIN = A4; // pot2:SIG

// ---------- Globals ----------
volatile uint8_t currentNumber = 0; // 0..99

// Debounce + hold behavior
static const uint16_t DEBOUNCE_MS = 30;
static const uint16_t REPEAT_POLL_MS = 5; // loop polling cadence for repeats

struct ButtonState {
  uint8_t pin;
  bool stableLevel;         // current stable reading (HIGH if not pressed with pull-up)
  bool lastRawLevel;
  bool lastDebounceRaw;
  uint32_t lastDebounceTime;

  bool pressedEventPending; // for immediate press action (edge)
  bool isHeld;
  uint32_t lastRepeatTime;  // time of last repeat action while held

  void begin(uint8_t p) {
    pin = p;
    pinMode(pin, INPUT_PULLUP);
    stableLevel = digitalRead(pin);
    lastRawLevel = stableLevel;
    lastDebounceRaw = stableLevel;
    lastDebounceTime = millis();
    pressedEventPending = false;
    isHeld = (stableLevel == LOW);
    lastRepeatTime = 0;
  }
};

static ButtonState btnInc;
static ButtonState btnDec;

// Display digit patterns for common-cathode assumption (Wokwi sevseg compatible)
static const uint8_t DIGIT_SEG[10] = {
  // a b c d e f g  (Wokwi uses bit order: A=bit0 ... G=bit6; Q0..Q6 wired to A..G)
  // 0: a b c d e f on, g off -> 0b0111111 = 0x3F
  // 1: b c -> 0b0000110 = 0x06
  // 2: a b d e g -> 0b1011011 = 0x5B
  // 3: a b c d g -> 0b1001111 = 0x4F
  // 4: b c f g -> 0b1100110 = 0x66
  // 5: a c d f g -> 0b1101101 = 0x6D
  // 6: a c d e f g -> 0b1111101 = 0x7D
  // 7: a b c -> 0b0000111 = 0x07
  // 8: a b c d e f g -> 0b1111111 = 0x7F
  // 9: a b c d f g -> 0b1101111 = 0x6F
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

// ---------- Helpers ----------
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float potToSeconds(uint16_t adcValue);
#line 82 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t readPotDigit(uint8_t potPin, uint8_t maxDigit);
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftWriteTwoDigits(uint8_t tens, uint8_t units);
#line 133 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplayFromNumber(uint8_t n);
#line 139 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void pollButton(ButtonState &b);
#line 169 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleButtonAction(ButtonState &b, bool isIncrement);
#line 213 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 239 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_78\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float potToSeconds(uint16_t adcValue) {
  // Map ADC range to speed between 0.1 and 1.0 seconds.
  // pot full travel -> min/max. We'll map linearly.
  // 10-bit ADC: 0..1023
  float x = constrain(adcValue, 0, 1023) / 1023.0f; // 0..1
  return 0.1f + x * (1.0f - 0.1f);
}

static uint8_t readPotDigit(uint8_t potPin, uint8_t maxDigit) {
  // Read pot and map to 0..maxDigit.
  uint16_t v = analogRead(potPin);
  // maxDigit=9 => map 0..1023 to 0..9
  return (uint8_t)((uint32_t)v * (maxDigit + 1) / 1024u);
}

static void shiftWriteTwoDigits(uint8_t tens, uint8_t units) {
  tens = (tens > 9) ? 0 : tens;
  units = (units > 9) ? 0 : units;

  // Chain order:
  // sr1 is first shift register; sr2 is second.
  // Wires: sr1:Q7S -> sr2:DS, so the last 8 bits shifted in correspond to sr2.
  // For two digits: shift out sr1 first (tens), then sr2 (units).
  //
  // We write 16 bits total MSB-first as typical shift-register usage:
  // - For sr1 (first), its bits are loaded from the first 8 shifted bits.
  // - For sr2, loaded from the last 8 shifted bits.
  //
  // We'll implement by shifting bits from MSB to LSB for each register sequentially.
  uint8_t sr1Byte = DIGIT_SEG[tens];  // patterns to Q0..Q6, ignoring Q7 (unused)
  uint8_t sr2Byte = DIGIT_SEG[units];

  // Ensure Q7 doesn't matter; still shift full 8 bits.
  noInterrupts();

  // Latch low -> shift
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);

  // Shift 16 bits: sr1Byte then sr2Byte, MSB first
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR1_DS, (sr1Byte >> i) & 0x01);
    // Pulse SHCP
    digitalWrite(SR1_SHCP, HIGH);
    digitalWrite(SR1_SHCP, LOW);
  }
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR1_DS, (sr2Byte >> i) & 0x01);
    digitalWrite(SR1_SHCP, HIGH);
    digitalWrite(SR1_SHCP, LOW);
  }

  // Latch to outputs
  digitalWrite(SR1_STCP, HIGH);
  digitalWrite(SR1_STCP, LOW);

  interrupts();
}

static void updateDisplayFromNumber(uint8_t n) {
  uint8_t tens = n / 10;
  uint8_t units = n % 10;
  shiftWriteTwoDigits(tens, units);
}

static void pollButton(ButtonState &b) {
  uint32_t now = millis();
  bool raw = digitalRead(b.pin);

  if (raw != b.lastRawLevel) {
    b.lastDebounceRaw = raw;
    b.lastDebounceTime = now;
    b.lastRawLevel = raw;
  }

  if ((now - b.lastDebounceTime) >= DEBOUNCE_MS) {
    if (b.stableLevel != b.lastDebounceRaw) {
      b.stableLevel = b.lastDebounceRaw;

      // Active LOW: pressed => LOW
      if (b.stableLevel == LOW) {
        // Rising edge into "pressed" state
        b.pressedEventPending = true;
        b.isHeld = true;
        b.lastRepeatTime = now; // start repeat timing from press moment
      } else {
        // Released
        b.isHeld = false;
      }
    }
  }
}

// Repeat engine with potentiometer speed that can vary while holding.
// Each repeat uses current pot-derived interval.
static void handleButtonAction(ButtonState &b, bool isIncrement) {
  // One-time press action
  if (b.pressedEventPending) {
    b.pressedEventPending = false;

    if (isIncrement) {
      uint8_t next = currentNumber + 1;
      if (next > 99) next = 0;
      currentNumber = next;
    } else {
      if (currentNumber == 0) currentNumber = 99;
      else currentNumber = currentNumber - 1;
    }
    updateDisplayFromNumber(currentNumber);
  }

  // Held repeats
  if (b.isHeld) {
    uint32_t now = millis();
    float intervalSec = isIncrement ? potToSeconds(analogRead(POT1_PIN)) : potToSeconds(analogRead(POT2_PIN));
    uint32_t intervalMs = (uint32_t)(intervalSec * 1000.0f + 0.5f);
    if (intervalMs < 100) intervalMs = 100; // safety (0.1s)
    if (intervalMs > 1000) intervalMs = 1000; // safety (1.0s)

    if (b.lastRepeatTime == 0) b.lastRepeatTime = now;

    if ((now - b.lastRepeatTime) >= intervalMs) {
      // Perform one step per interval
      b.lastRepeatTime += intervalMs; // keep cadence even if loop delayed

      if (isIncrement) {
        uint8_t next = currentNumber + 1;
        if (next > 99) next = 0;
        currentNumber = next;
      } else {
        if (currentNumber == 0) currentNumber = 99;
        else currentNumber = currentNumber - 1;
      }
      updateDisplayFromNumber(currentNumber);
    }
  }
}

// ---------- Arduino sketch ----------
void setup() {
  // Shift register control pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // Buttons
  btnInc.begin(BTN1_PIN);
  btnDec.begin(BTN2_PIN);

  // Initialize number from potentiometers:
  // pot1 -> tens digit, pot2 -> units digit, both 00..99.
  analogRead(POT1_PIN);
  analogRead(POT2_PIN);

  uint8_t tens = readPotDigit(POT1_PIN, 9);
  uint8_t units = readPotDigit(POT2_PIN, 9);

  currentNumber = tens * 10 + units;
  updateDisplayFromNumber(currentNumber);
}

void loop() {
  static uint32_t lastPoll = 0;
  uint32_t now = millis();

  if (now - lastPoll >= REPEAT_POLL_MS) {
    lastPoll = now;

    pollButton(btnInc);
    pollButton(btnDec);

    handleButtonAction(btnInc, true);
    handleButtonAction(btnDec, false);
  }
}
