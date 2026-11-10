#include <Arduino.h>

//////////////////// Hardware bindings (from frozen contract) ////////////////////
static constexpr uint8_t BTN1_PIN = 2;   // io_0 (active_low=true => pressed => LOW)
static constexpr uint8_t BTN2_PIN = 3;   // io_1

static constexpr uint8_t POT1_PIN = A3;  // io_2 (f64 adc_count)
static constexpr uint8_t POT2_PIN = A4;  // io_3

static constexpr uint8_t SR_DS   = A0;   // shift595_number: data_pin
static constexpr uint8_t SR_SHCP = A2;   // clock_pin
static constexpr uint8_t SR_STCP = A1;   // latch_pin

// Segment wiring facts: segment bits are [A,B,C,D,E,F,G] => Q0..Q6 are A..G.
// active_low=true for shift595_number registers.
// For a given digit, output should be active-low segment pattern.
static const uint8_t digitSegmentsActiveLow[10] = {
  // segments lit: A,B,C,D,E,F (not G) => bits [0..6]=A..G => 0,1,1,1,1,1,1? careful:
  // We store active-low mask for [A..G]. bit=1 means segment OFF (since active_low), bit=0 means ON.
  // Common mapping for 7-seg digits (assuming standard cathode segments):
  // Digit -> ON segments:
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
  // We'll construct OFF mask for active_low: off=1, on=0.
  // bit0=A ... bit6=G
  // 0: on A,B,C,D,E,F => off G only => mask bits: A0 B0 C0 D0 E0 F0 G1 => 0b1000000 = 0x40? with bit6=G:
  // binary bit6..0 => bit6=1, others 0 => 0b1000000 = 64
  0x40,
  // 1: on B,C => off A,D,E,F,G => bits: A1 B0 C0 D1 E1 F1 G1 => bit0=1,1:0,2:0,3:1,4:1,5:1,6:1
  // => bit6..0: 1 1 1 1 1 0 0? Wait compute byte: bit0(A)=1 =>1
  // bit1(B)=0 =>0; bit2(C)=0 =>0; bit3(D)=1 =>8; bit4(E)=1 =>16; bit5(F)=1 =>32; bit6(G)=1 =>64 => sum=1+8+16+32+64=121
  121, // 0x79
  // 2: on A,B,D,E,G => off C,F => bits: A0 B0 C1 D0 E0 F1 G0
  // bit0=0, bit1=0, bit2=1(4), bit3=0, bit4=0, bit5=1(32), bit6=0 => 4+32=36 => 0x24
  36,  // 0x24
  // 3: on A,B,C,D,G => off E,F => bits A0 B0 C0 D0 E1 F1 G0
  // bit4=1(16) + bit5=1(32)=48 => 0x30
  48,  // 0x30
  // 4: on B,C,F,G => off A,D,E => bits A1 B0 C0 D1 E1 F0 G0
  // bit0=1 + bit3=1(8) + bit4=1(16) =>25 =>0x19
  25,  // 0x19
  // 5: on A,C,D,F,G => off B,E => bits A0 B1 C0 D0 E1 F0 G0
  // bit1=1(2)+bit4=1(16)=18 =>0x12
  18,  // 0x12
  // 6: on A,C,D,E,F,G => off B => bits A0 B1 C0 D0 E0 F0 G0
  // bit1=1(2) =>2
  2,   // 0x02
  // 7: on A,B,C => off D,E,F,G => bits A0 B0 C0 D1 E1 F1 G1
  // bit3=1(8)+bit4=1(16)+bit5=1(32)+bit6=1(64)=120 =>0x78
  120, // 0x78
  // 8: on A..G => off none => mask 0
  0,    // 0x00
  // 9: on A,B,C,D,F,G => off E => bit4=1(16)
  16   // 0x10
};

static inline void shift595WriteByte(uint8_t dataByte) {
  // SR_DS=A0, SR_SHCP=A2, SR_STCP=A1
  // Clocking order doesn't matter for latch model as long as we follow DS then pulse SHCP.
  // We shift out bits MSB->LSB? We need consistency with 74HC595 bit order Q0..Q7.
  // Contract extracted says digit register segment_bits [0..6] correspond to Q0..Q6 and active_low.
  // For the contract "segment_bits": [0..6] -> mapping implies we are directly writing Q0..Q6 bits.
  // We'll shift out with DS = (dataByte >> bit) & 1 for bit0..bit7 in increasing order,
  // so that the first shifted bit ends up at Q0 after 8 clocks (typical if shifting LSB first into Q0).
  // That matches "segment_bits assume digit 0 is sevseg1" and active_low copied exactly.
  // We'll implement LSB-first over 8 bits.
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR_DS, (dataByte >> i) & 0x01);
    digitalWrite(SR_SHCP, HIGH);
    digitalWrite(SR_SHCP, LOW);
  }
}

static inline void displayNumber(int number) {
  // Contract: io_4 outputs "number" to shift595_number with 2 digits:
  // digit 0 => sevseg1 => sr1, digit 1 => sevseg2 => sr2.
  // leading_zero=true, invalid_policy=blank. We'll always display both digits.
  if (number < 0) number = 0;
  number %= 100;

  int tens = number / 10; // digit0
  int ones = number % 10; // digit1

  uint8_t segMaskTens = digitSegmentsActiveLow[tens] & 0x7F; // bits for A..G
  uint8_t segMaskOnes = digitSegmentsActiveLow[ones] & 0x7F;

  // Shift-register chain: sr1 Q7S connected to sr2 DS (blue).
  // We must shift two bytes into the chain before latching:
  // first byte shifted ends up in sr2 or sr1 depending on ordering.
  // To ensure sr1 receives digit0 and sr2 receives digit1, shift SR chain such that
  // byte for sr1 is shifted first, then byte for sr2, with LSB-first inside each byte.
  // We'll send sr1 first, then sr2, then latch with STCP.
  uint8_t byteSr1 = segMaskTens; // A..G in Q0..Q6, Q7 unused
  uint8_t byteSr2 = segMaskOnes;

  digitalWrite(SR_STCP, LOW);

  // shift sr1 byte then sr2 byte
  shift595WriteByte(byteSr1);
  shift595WriteByte(byteSr2);

  digitalWrite(SR_STCP, HIGH);
  digitalWrite(SR_STCP, LOW);
}

//////////////////// Controller model (accepted executable) ////////////////////
static int number_ = 0;        // Controller.number
static bool prevBtn1_ = false; // Controller.prevBtn1
static bool prevBtn2_ = false; // Controller.prevBtn2
static int accIncMs_ = 0;      // Controller.accIncMs
static int accDecMs_ = 0;      // Controller.accDecMs
static bool initialized_ = false;

static unsigned long lastTickMicros_ = 0;
static unsigned long lastPotSampleMs_ = 0;

static inline int clampInt(int v, int lo, int hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

// Emulate RealFunctions::floor for non-negative values.
static inline int floorToInt(double x) {
  if (x <= 0.0) return 0; // contract never uses negatives for floor terms
  return (int)x; // trunc towards zero == floor for positive
}

static inline double readPotRawF64(uint8_t pin) {
  // ios: f64 with scale=1 offset=0; adc_count in [0..1023]
  int raw = analogRead(pin);
  return (double)raw;
}

static inline bool readBtnModelLevel(uint8_t pin) {
  // model semantics: true_level=LOW false_level=HIGH (active_high=false, pullup=true)
  // pressed => LOW => true
  return (digitalRead(pin) == LOW);
}

static inline int speedFromPotF64(double potRaw) {
  // Contract expression: floor(io * 99 / 1023)
  // Then map:
  // if s==0 => 100
  // else if s>=99 => 1000
  // else => floor(100 + floor(io*900/1023) * 1.0)
  int s = floorToInt((potRaw * 99.0) / 1023.0);
  if (s == 0) return 100;
  if (s >= 99) return 1000;
  int inner = floorToInt((potRaw * 900.0) / 1023.0);
  return floorToInt(100.0 + inner * 1.0);
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  pinMode(SR_DS, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);
  pinMode(SR_STCP, OUTPUT);

  digitalWrite(SR_DS, LOW);
  digitalWrite(SR_SHCP, LOW);
  digitalWrite(SR_STCP, LOW);

  analogReadResolution(10); // default on AVR; harmless.

  lastTickMicros_ = micros();
}

void loop() {
  // Control period is 10ms.
  unsigned long now = micros();
  unsigned long elapsed = now - lastTickMicros_;
  if (elapsed < 10000UL) return; // 10ms
  lastTickMicros_ += 10000UL;

  // io_5 is delta time since previous control step (ms).
  // Contract uses io_5 advancing by 10ms per step; we use 10ms constant.
  int io_5 = 10;

  // Sample inputs
  bool io_0 = readBtnModelLevel(BTN1_PIN);
  bool io_1 = readBtnModelLevel(BTN2_PIN);
  double io_2 = readPotRawF64(POT1_PIN);
  double io_3 = readPotRawF64(POT2_PIN);

  // ---- tick ----
  // first start;
  if (!initialized_) {
    // assign number := (if io_2>=0 then floor(io_3*99/1023*10)+floor(io_2*99/1023) else 0)
    if (io_2 >= 0.0) {
      int tensD = floorToInt((io_3 * 99.0) / 1023.0);
      int onesD = floorToInt((io_2 * 99.0) / 1023.0);
      number_ = tensD * 10 + onesD;
    } else {
      number_ = 0;
    }
    // Ensure in range 00..99 as contract says between 00 and 99.
    number_ = clampInt(number_, 0, 99);

    initialized_ = true;
    prevBtn1_ = io_0;
    prevBtn2_ = io_1;
  }

  // assign accIncMs := (if (io_0 && !prevBtn1_) then 0 else (io_0 ? accIncMs + io_5 : 0))
  if (io_0 && (!prevBtn1_)) {
    accIncMs_ = 0;
  } else {
    accIncMs_ = (io_0 ? (accIncMs_ + io_5) : 0);
  }

  // assign accDecMs := (if (io_1 && !prevBtn2_) then 0 else (io_1 ? accDecMs + io_5 : 0))
  if (io_1 && (!prevBtn2_)) {
    accDecMs_ = 0;
  } else {
    accDecMs_ = (io_1 ? (accDecMs_ + io_5) : 0);
  }

  // if (io_0 && !prevBtn1_) number := (number==99 ? 0 : number+1)
  if (io_0 && (!prevBtn1_)) {
    number_ = (number_ == 99) ? 0 : (number_ + 1);
  }

  // if (io_0 && (accIncMs_ >= speedIncMs)) { accIncMs_ := accIncMs_ - speedIncMs; number wrap +1 }
  // speedIncMs expression uses io_3 (pot1). Note in contract: io_3 is pot1 and io_2 is pot2.
  // Here: io_3 corresponds to pot1 (tens/speed for inc). Match contract carefully.
  int speedIncMs = speedFromPotF64(io_3);
  if (io_0 && (accIncMs_ >= speedIncMs)) {
    accIncMs_ = accIncMs_ - speedIncMs;
    number_ = (number_ == 99) ? 0 : (number_ + 1);
  }

  // if (io_1 && !prevBtn2_) number := (number==0 ? 99 : number-1)
  if (io_1 && (!prevBtn2_)) {
    number_ = (number_ == 0) ? 99 : (number_ - 1);
  }

  // if (io_1 && (accDecMs_ >= speedDecMs)) { accDecMs_ := accDecMs_ - speedDecMs; number wrap -1 }
  // speedDecMs uses io_2 (pot2).
  int speedDecMs = speedFromPotF64(io_2);
  if (io_1 && (accDecMs_ >= speedDecMs)) {
    accDecMs_ = accDecMs_ - speedDecMs;
    number_ = (number_ == 0) ? 99 : (number_ - 1);
  }

  prevBtn1_ = io_0;
  prevBtn2_ = io_1;

  // Output io_4 := number
  displayNumber(number_);
}