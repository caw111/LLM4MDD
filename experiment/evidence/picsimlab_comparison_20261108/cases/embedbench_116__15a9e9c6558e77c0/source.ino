#include <Arduino.h>

// ===== Pin bindings (preserve circuit wiring / frozen interface contract) =====
// led1: D2
static const uint8_t PIN_LED1   = 2;

// rgb1: D3 (R), D4 (G), D5 (B)
static const uint8_t PIN_RGB_R  = 3;
static const uint8_t PIN_RGB_G  = 4;
static const uint8_t PIN_RGB_B  = 5;

// shift registers (both sr1 and sr2 chained)
static const uint8_t PIN_SR_DS    = A0; // A0
static const uint8_t PIN_SR_SHCP  = A2; // A2
static const uint8_t PIN_SR_STCP  = A1; // A1
// OE is tied to GND (black) per circuit, MR is tied to VCC for both SRs (red).
// Therefore no OE/MR control is needed.

// ===== 7-seg shift register mapping contract =====
// shift595_number: active_low=true for segments A..G,
// segment_bits = [0,1,2,3,4,5,6] correspond to segments [A,B,C,D,E,F,G].
// We write one byte with bit positions 0..6 for segments A..G.
// Active low means: segment ON => output bit = 0; segment OFF => output bit = 1.
static constexpr uint8_t segBitA = 0;
static constexpr uint8_t segBitB = 1;
static constexpr uint8_t segBitC = 2;
static constexpr uint8_t segBitD = 3;
static constexpr uint8_t segBitE = 4;
static constexpr uint8_t segBitF = 5;
static constexpr uint8_t segBitG = 6;

static uint8_t encode7segActiveLow(uint8_t digit) {
  // Standard 7-seg segments (A,B,C,D,E,F,G) for hexadecimal-style digits.
  // We only need 0..9 for counting 00..99.
  // segs: 1 means segment is lit; we'll invert for active_low.
  bool A=false,B=false,C=false,D=false,E=false,F=false,G=false;

  switch (digit) {
    case 0: A=true; B=true; C=true; D=true; E=true; F=true;  G=false; break;
    case 1: A=false;B=true; C=true; D=false;E=false;F=false; G=false; break;
    case 2: A=true; B=true; C=false;D=true; E=true; F=false; G=true;  break;
    case 3: A=true; B=true; C=true; D=true; E=false;F=false; G=true;  break;
    case 4: A=false;B=true; C=true; D=false;E=false;F=true;  G=true;  break;
    case 5: A=true; B=false;C=true; D=true; E=false;F=true;  G=true;  break;
    case 6: A=true; B=false;C=true; D=true; E=true; F=true;  G=true;  break;
    case 7: A=true; B=true; C=true; D=false;E=false;F=false; G=false; break;
    case 8: A=true; B=true; C=true; D=true; E=true; F=true;  G=true;  break;
    case 9: A=true; B=true; C=true; D=true; E=false;F=true;  G=true;  break;
    default: A=false;B=false;C=false;D=false;E=false;F=false; G=false; break;
  }

  // Build active_low bitfield: bit=0 for ON, bit=1 for OFF.
  uint8_t out = 0;
  out |= (A ? 0 : (1u << segBitA));
  out |= (B ? 0 : (1u << segBitB));
  out |= (C ? 0 : (1u << segBitC));
  out |= (D ? 0 : (1u << segBitD));
  out |= (E ? 0 : (1u << segBitE));
  out |= (F ? 0 : (1u << segBitF));
  out |= (G ? 0 : (1u << segBitG));
  return out;
}

static void shift595WriteByte(uint8_t value) {
  // Clock in MSB-first is typical for 74HC595 and yields correct bit-to-pin mapping
  // for a direct segment-bit ordering. We'll use MSB-first.
  for (int i = 7; i >= 0; --i) {
    digitalWrite(PIN_SR_DS, (value & (1u << i)) ? HIGH : LOW);
    digitalWrite(PIN_SR_SHCP, HIGH);
    // small settle
    delayMicroseconds(2);
    digitalWrite(PIN_SR_SHCP, LOW);
    delayMicroseconds(2);
  }
}

// Displays two-digit number using chained shift registers:
// digit 0 (tens) -> sevseg1 via sr1
// digit 1 (units) -> sevseg2 via sr2
//
// Chaining behavior: bits shifted into the chain first emerge at the farthest register.
// With two chained 8-bit registers, we must send sr1 byte first? Let's account:
// When shifting 16 bits into Q outputs, the last 8 bits shifted correspond to the first register (sr1),
// while the earlier 8 bits correspond to the second register (sr2).
//
// To ensure sr1 gets tens and sr2 gets units, we shift in: sr2(units) first, then sr1(tens).
static void shift595ShowTwoDigits(uint8_t tens, uint8_t units) {
  uint8_t sr2_byte = encode7segActiveLow(units);
  uint8_t sr1_byte = encode7segActiveLow(tens);

  // Shift 16 bits with proper order for chained registers
  shift595WriteByte(sr2_byte); // goes to sr2 (farther in chain)
  shift595WriteByte(sr1_byte); // then to sr1

  // Latch both
  digitalWrite(PIN_SR_STCP, HIGH);
  delayMicroseconds(5);
  digitalWrite(PIN_SR_STCP, LOW);
}

// ===== Controller logic (ACCEPTED behavior model) =====
static const uint32_t PERIOD_MS = 2000;

static bool led1_on = false;

static const uint8_t rgb_sequence_count = 7;
static const uint8_t rgb_sequence[rgb_sequence_count][3] = {
  // Red, Green, Blue, Yellow, Cyan, Magenta, White
  {1,0,0}, // Red
  {0,1,0}, // Green
  {0,0,1}, // Blue
  {1,1,0}, // Yellow
  {0,1,1}, // Cyan
  {1,0,1}, // Magenta
  {1,1,1}  // White
};
static uint8_t rgb_index = 0;

static uint8_t count = 0;

static uint32_t t_led = 0;
static uint32_t t_rgb = 0;
static uint32_t t_disp = 0;

static void applyOutputs(bool io0, bool r, bool g, bool b, uint8_t dispValue) {
  // io_0 -> led1 command
  digitalWrite(PIN_LED1, io0 ? HIGH : LOW);

  // rgb commands
  digitalWrite(PIN_RGB_R, r ? HIGH : LOW);
  digitalWrite(PIN_RGB_G, g ? HIGH : LOW);
  digitalWrite(PIN_RGB_B, b ? HIGH : LOW);

  // sevseg output through shift registers
  // shift595_number leading_zero=true and two digits.
  uint8_t tens = (dispValue / 10) % 10;
  uint8_t units = dispValue % 10;
  shift595ShowTwoDigits(tens, units);
}

void setup() {
  // IO pins
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);

  pinMode(PIN_SR_DS, OUTPUT);
  pinMode(PIN_SR_SHCP, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);

  // Initialize shift register lines low
  digitalWrite(PIN_SR_DS, LOW);
  digitalWrite(PIN_SR_SHCP, LOW);
  digitalWrite(PIN_SR_STCP, LOW);

  // Frozen interface contract initial outputs:
  // io_0=false, io_1=false, io_2=false, io_3=false, io_4=0
  led1_on = false;
  rgb_index = 0; // will remain off initially until first 2s tick updates RGB
  count = 0;

  // Set initial outputs precisely
  applyOutputs(false, false, false, false, 0);

  uint32_t now = millis();
  t_led = now;
  t_rgb = now;
  t_disp = now;
}

void loop() {
  uint32_t now = millis();

  // Update each at its own 2-second interval, all aligned to "every 2 seconds"
  // from their initial application time.
  if ((uint32_t)(now - t_led) >= PERIOD_MS) {
    // Monochrome LED: 2s on/off forever, starting from OFF.
    t_led += PERIOD_MS;
    led1_on = !led1_on;
  }

  if ((uint32_t)(now - t_rgb) >= PERIOD_MS) {
    // RGB: change color every 2 seconds in order:
    // Red, Green, Blue, Yellow, Cyan, Magenta, White
    t_rgb += PERIOD_MS;
    rgb_index = (uint8_t)((rgb_index + 1) % rgb_sequence_count);
  }

  if ((uint32_t)(now - t_disp) >= PERIOD_MS) {
    // 7-seg count: 00..99, increment every 2 seconds, reset to 00 after 99.
    t_disp += PERIOD_MS;
    if (count >= 99) count = 0;
    else count = (uint8_t)(count + 1);
  }

  // Compute current output values and apply.
  bool r = rgb_sequence[rgb_index][0];
  bool g = rgb_sequence[rgb_index][1];
  bool b = rgb_sequence[rgb_index][2];

  applyOutputs(led1_on, r, g, b, count);

  // Control period is 10ms in the frozen contract; keep a small cadence without
  // altering the 2-second model timing.
  delay(10);
}