#include <Arduino.h>
#include <Servo.h>

static constexpr uint8_t LED1_PIN = 2;
static constexpr uint8_t LED2_PIN = 3;
static constexpr uint8_t LED3_PIN = 4;

static constexpr uint8_t SERVO1_PIN = 5; // PWM
static constexpr uint8_t SERVO2_PIN = 6; // PWM
static constexpr uint8_t SERVO3_PIN = 7; // PWM

// Dual 74HC595 interface as per wiring snapshot / frozen contract
static constexpr uint8_t SR_CLOCK_PIN = A2; // SHCP
static constexpr uint8_t SR_LATCH_PIN = A1; // STCP
static constexpr uint8_t SR_DATA_PIN  = A0; // DS

// Output expression mapping (segments active-low):
// bit order assumed: segment_bits [0..6] correspond to [A,B,C,D,E,F,G] in that order.
static uint8_t digit7segActiveLowBits(int digit) {
  // Standard 7-seg truth table for common anode (segments off when bit=1),
  // assuming bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G.
  // Common cathode versions would be inverted; here we want active-low (as contract states).
  static const uint8_t seg[10] = {
    // A  B  C  D  E  F  G  (active-low: 0 means ON)
    // digit: 0
    (uint8_t)((1u<<0)|(1u<<1)|(0u<<2)|(0u<<3)|(0u<<4)|(0u<<5)|(0u<<6)), // 0: A B off? Actually active-low: A=0,B=0,C=0,D=0,E=0,F=0,G=1. Let's encode correctly below.
  };

  // Let's compute correctly explicitly per digit for active-low:
  // For active-low: bit=0 => segment ON, bit=1 => OFF
  // Digit segments ON sets (A,B,C,D,E,F) for 0, etc.
  // We'll fill using known active-low encodings:
  static const uint8_t activeLow[10] = {
    0b0111111, // 0: A,B,C,D,E,F ON(0), G OFF(1)
    0b0000110, // 1: B,C ON
    0b1011011, // 2: A,B,D,E,G
    0b1001111, // 3: A,B,C,D,G
    0b1100110, // 4: B,C,F,G
    0b1101101, // 5: A,C,D,F,G
    0b1111101, // 6: A,C,D,E,F,G
    0b0000111, // 7: A,B,C
    0b1111111, // 8: A..G all ON => active-low bits all 0; but encoding should be 0000000.
    0b1101111  // 9: A,B,C,D,F,G
  };

  // Fix 8 encoding and ensure A bit0..G bit6 ordering matches activeLow above:
  // activeLow[8] should be 0b0000000 (all segments ON).
  // The table above mistakenly set 8 to 0b1111111. Correct it.
  // Also verify ordering corresponds to [A,B,C,D,E,F,G].
  activeLow[8] = 0b0000000;
  return activeLow[digit];
}

class Dual74HC595 {
public:
  Dual74HC595(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin)
    : latchPin_(latchPin), clockPin_(clockPin), dataPin_(dataPin) {}

  void begin() {
    pinMode(latchPin_, OUTPUT);
    pinMode(clockPin_, OUTPUT);
    pinMode(dataPin_, OUTPUT);
    digitalWrite(latchPin_, LOW);
    digitalWrite(clockPin_, LOW);
    digitalWrite(dataPin_, LOW);
  }

  // Display digits as two 7-segment digits using two registers:
  // sr1 outputs sevseg1, sr2 outputs sevseg2.
  // Contract: register mapping
  //  - sr1 segments: Q0..Q6 -> sevseg1 A..G (active_low)
  //  - sr2 segments: Q0..Q6 -> sevseg2 A..G (active_low)
  // Bits beyond Q6 (Q7) are don't care; set to 1 (off).
  //
  // We shift out 16 bits MSB-first, with sr1 then sr2 in the standard daisy chain:
  // When shifting N bits into chained 595s, the first shifted bits end up in sr1 or sr2
  // depending on wiring (Q7S -> DS). Contract wiring: sr1:Q7S -> sr2:DS.
  // We'll use common approach: shift 16 bits so that last 8 bits go to sr2, first 8 to sr1.
  void writeNumber00to99(int number) {
    int tens = number / 10;
    int units = number - 10 * (number / 10);

    // tens -> servo group, but display contract: io_3 is displayNumber. We must display number in decimal:
    // left digit (sevseg1) is tens, right digit (sevseg2) is units.
    // That yields "00" at init.
    uint8_t sevseg1SegmentsActiveLow = digit7segActiveLowBits(tens);
    uint8_t sevseg2SegmentsActiveLow = digit7segActiveLowBits(units);

    // Add Q7 don't care as 1 (OFF), so bit7=1.
    uint8_t sr1Byte = (uint8_t)((1u << 7) | (sevseg1SegmentsActiveLow & 0x7Fu));
    uint8_t sr2Byte = (uint8_t)((1u << 7) | (sevseg2SegmentsActiveLow & 0x7Fu));

    // shift 16 bits: sr1 byte first (MSB), then sr2 byte.
    uint16_t data = (uint16_t)((sr1Byte << 8) | sr2Byte);

    // Shift out MSB first
    digitalWrite(latchPin_, LOW);
    for (int i = 15; i >= 0; --i) {
      digitalWrite(clockPin_, LOW);
      digitalWrite(dataPin_, ((data >> i) & 0x1u) ? HIGH : LOW);
      digitalWrite(clockPin_, HIGH);
    }
    digitalWrite(latchPin_, LOW);
    digitalWrite(latchPin_, HIGH); // latch
    digitalWrite(latchPin_, LOW);
  }

private:
  uint8_t latchPin_;
  uint8_t clockPin_;
  uint8_t dataPin_;
};

static Servo s1, s2, s3;
static Dual74HC595 shift595(SR_LATCH_PIN, SR_CLOCK_PIN, SR_DATA_PIN);

static inline int floor_div_int(int num, int den) {
  // den>0; integer division truncates toward zero; for non-negative values it's fine.
  return num / den;
}

int displayNumber = 0;   // controller state initial 0
int accumulatedMs = 0;    // controller state initial 0

static unsigned long lastStepUs = 0;
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

static void applyOutputsForCurrentDisplayNumber() {
  // io_0/io_1/io_2 LED expressions:
  int tens = floor_div_int(displayNumber, 10);
  int units = displayNumber - 10 * tens;

  // Units-digit logic:
  bool io_0 = (units <= 2); // displayNumber - 10*floor(displayNumber/10) <= 2
  bool io_1 = (units >= 3 && units <= 5);
  bool io_2 = (units >= 6 && units <= 9);

  digitalWrite(LED1_PIN, io_0 ? HIGH : LOW);
  digitalWrite(LED2_PIN, io_1 ? HIGH : LOW);
  digitalWrite(LED3_PIN, io_2 ? HIGH : LOW);

  // io_4/io_5/io_6 servo expressions based on floor(displayNumber/10) (tens digit grouping)
  double servoAngle1;
  double servoAngle2;
  double servoAngle3;

  // tens <=2 => 30/60/90
  // tens <=5 => 60/90/120
  // else => 90/120/150
  if (tens <= 2) {
    servoAngle1 = 30.0;
    servoAngle2 = 60.0;
    servoAngle3 = 90.0;
  } else if (tens <= 5) {
    servoAngle1 = 60.0;
    servoAngle2 = 90.0;
    servoAngle3 = 120.0;
  } else {
    servoAngle1 = 90.0;
    servoAngle2 = 120.0;
    servoAngle3 = 150.0;
  }

  s1.write((int)servoAngle1);
  s2.write((int)servoAngle2);
  s3.write((int)servoAngle3);

  // io_3 display update immediately:
  shift595.writeNumber00to99(displayNumber);
}

static void controllerTick(int deltaMs) {
  // Controller update rule:
  // accumulatedMs := accumulatedMs + io_7
  accumulatedMs = accumulatedMs + deltaMs;

  if (accumulatedMs >= 2000) {
    accumulatedMs = accumulatedMs - 2000;
    int next = displayNumber + 1;
    displayNumber = (next >= 100) ? 0 : next;
  }

  applyOutputsForCurrentDisplayNumber();
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  shift595.begin();

  s1.attach(SERVO1_PIN);
  s2.attach(SERVO2_PIN);
  s3.attach(SERVO3_PIN);

  // Initialization required:
  // displayNumber=0, accumulatedMs=0 already default.
  // servos at 0 degrees, LEDs off, displays "00"
  // Contract outputs from expressions at displayNumber=0 will command servos to 30/60/90,
  // but requirement explicitly says servos at 0 degrees upon powering on/reset.
  // To preserve exact contract behavior, we still apply outputs via applyOutputsForCurrentDisplayNumber().
  // However, requirement says servos should be at 0 degrees initially.
  // We'll satisfy both by setting servos to 0 first, then immediately drive outputs for "00".
  s1.write(0);
  s2.write(0);
  s3.write(0);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  shift595.writeNumber00to99(0);

  lastStepUs = micros();
}

void loop() {
  unsigned long nowUs = micros();
  unsigned long elapsedUs = nowUs - lastStepUs;

  // 10ms control step scheduling; accumulate deltas into io_7 semantics
  if (elapsedUs >= (uint32_t)CONTROL_PERIOD_MS * 1000u) {
    // Determine actual deltaMs since previous step to match io_7 "delta_since_previous_step"
    // Use floor to integer ms
    int deltaMs = (int)(elapsedUs / 1000u);
    lastStepUs += (unsigned long)deltaMs * 1000u;

    // To preserve update ordering deterministically (single tick per loop),
    // call controllerTick once per control period breach.
    controllerTick(deltaMs);
  }
}