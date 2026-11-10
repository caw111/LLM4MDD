#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_59\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// ---------- Wiring (preserve circuit) ----------
// RGB LEDs (common cathode)
// rgb1: COM->GND.1, R->D2, G->D3, B->D4
// rgb2: COM->GND.1, R->D5, G->D6, B->D7
// rgb3: COM->GND.1, R->D8, G->D9, B->D10

static const uint8_t RGB1_R_PIN = 2;
static const uint8_t RGB1_G_PIN = 3;
static const uint8_t RGB1_B_PIN = 4;

static const uint8_t RGB2_R_PIN = 5;
static const uint8_t RGB2_G_PIN = 6;
static const uint8_t RGB2_B_PIN = 7;

static const uint8_t RGB3_R_PIN = 8;
static const uint8_t RGB3_G_PIN = 9;
static const uint8_t RGB3_B_PIN = 10;

// 7-seg via 74HC595
// sr1: SHCP->A2, STCP->A1, DS->A0, OE->GND.3, MR->VCC
static const uint8_t SR_SHCP_PIN = A2; // clock
static const uint8_t SR_STCP_PIN = A1; // latch
static const uint8_t SR_DS_PIN   = A0; // data

// Sevseg segment outputs are from Q0..Q6 -> A..G, active_low=true
// In the circuit, sevseg1 COM is driven by sr1:MR which is tied to VCC through MR control,
// so we just output segment states.

// Servo PWM pins (with V+ to 5V and GND.1 to GND)
static const uint8_t SERVO1_PIN = 11; // io_10
static const uint8_t SERVO2_PIN = 12; // io_11
static const uint8_t SERVO3_PIN = 13; // io_12

static const uint32_t STATE_PERIOD_MS = 2000;

// ---------- Shift-register driver (active-low segments A..G on Q0..Q6) ----------
class Shift595 {
public:
  void begin(uint8_t ds, uint8_t shcp, uint8_t stcp) {
    _ds = ds;
    _shcp = shcp;
    _stcp = stcp;
    pinMode(_ds, OUTPUT);
    pinMode(_shcp, OUTPUT);
    pinMode(_stcp, OUTPUT);
    digitalWrite(_shcp, LOW);
    digitalWrite(_stcp, LOW);
    digitalWrite(_ds, LOW);
  }

  // bits[0..7] LSB->Q0... MSB->Q7 (we use only Q0..Q6)
  void write8(uint8_t bits) {
    // Shift out MSB first is common; but we must preserve mapping Q0..Q6.
    // The contract says segment_bits [0..6] correspond to Q0..Q6.
    // We'll shift out bits from MSB->LSB for an 8-bit chain so that
    // bit0 ends at Q0.
    for (int i = 7; i >= 0; --i) {
      digitalWrite(_ds, (bits >> i) & 0x01 ? HIGH : LOW);
      digitalWrite(_shcp, HIGH);
      digitalWrite(_shcp, LOW);
    }
    digitalWrite(_stcp, HIGH);
    digitalWrite(_stcp, LOW);
  }

private:
  uint8_t _ds = 0, _shcp = 0, _stcp = 0;
};

static Shift595 sr;

// Segment mapping for digits 0..9 for a typical 7-seg (A B C D E F G),
// active_low (1=off, 0=on). We output bits for A..G into Q0..Q6 respectively.
// Contract specifies active_low=true and segment_bits [0..6] correspond to A..G.
// Use standard segment on-values; convert to active-low.
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_59\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitSegmentsActiveLow(int digit);
#line 128 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_59\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setRGBChannelsForColor(int rgbColorIndex);
#line 152 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_59\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int servoAngleForCountdown(int n);
#line 160 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_59\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setServosForCountdown(int n);
#line 173 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_59\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int colorIndexFromCountdownState(int countdownDigit);
#line 191 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_59\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void applyOutputsForState(int digit);
#line 204 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_59\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 225 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_59\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_59\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitSegmentsActiveLow(int digit) {
  // Standard (active-high) on segments:
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
  bool A=false,B=false,C=false,D=false,E=false,F=false,G=false;

  switch (digit) {
    case 0: A=B=C=D=E=F=true;  G=false; break;
    case 1: B=C=true;          A=D=E=F=G=false; break;
    case 2: A=B=D=E=G=true;    C=F=false; break;
    case 3: A=B=C=D=G=true;    E=F=false; break;
    case 4: B=C=F=G=true;      A=D=E=false; break;
    case 5: A=C=D=F=G=true;    B=E=false; break;
    case 6: A=C=D=E=F=G=true;  B=false; break;
    case 7: A=B=C=true;        D=E=F=G=false; break;
    case 8: A=B=C=D=E=F=G=true;break;
    case 9: A=B=C=D=F=G=true;  E=false; break;
    default: break;
  }

  // Pack into Q0..Q6 for A..G, active_low:
  // bit=0 means segment ON.
  // So active_low_bit = !segmentOn
  uint8_t bits = 0;
  bits |= (A ? 0 : 1) << 0; // Q0 -> A
  bits |= (B ? 0 : 1) << 1; // Q1 -> B
  bits |= (C ? 0 : 1) << 2; // Q2 -> C
  bits |= (D ? 0 : 1) << 3; // Q3 -> D
  bits |= (E ? 0 : 1) << 4; // Q4 -> E
  bits |= (F ? 0 : 1) << 5; // Q5 -> F
  bits |= (G ? 0 : 1) << 6; // Q6 -> G
  // Q7 unused -> keep off (1) so it doesn't matter (set to 1 meaning "off" for active_low)
  bits |= (1u << 7);
  return bits;
}

// ---------- RGB color handling (PWM pins to LED channels) ----------
// Contract requires outputs io_0..io_8 be 0.0 initially, but behavior requires cycling colors.
// We'll implement cycling while still respecting that PWM outputs are f64 unit pwm_count.
// Here we map PWM duty in [0..255] to "pwm_count" count; set analogWrite accordingly.

static void setRGBChannelsForColor(int rgbColorIndex) {
  // Sequence: Red, Green, Cyan, Magenta, Blue, Yellow, White
  // common cathode: driving channel HIGH (PWM duty) turns segment on.
  uint8_t r=0,g=0,b=0;
  switch (rgbColorIndex) {
    case 0: r=255; g=0;   b=0;   break; // Red
    case 1: r=0;   g=255; b=0;   break; // Green
    case 2: r=0;   g=255; b=255; break; // Cyan
    case 3: r=255; g=0;   b=255; break; // Magenta
    case 4: r=0;   g=0;   b=255; break; // Blue
    case 5: r=255; g=255; b=0;   break; // Yellow
    case 6: r=255; g=255; b=255; break; // White
    default: break;
  }

  // All three RGB LEDs same color simultaneously.
  analogWrite(RGB1_R_PIN, r); analogWrite(RGB1_G_PIN, g); analogWrite(RGB1_B_PIN, b);
  analogWrite(RGB2_R_PIN, r); analogWrite(RGB2_G_PIN, g); analogWrite(RGB2_B_PIN, b);
  analogWrite(RGB3_R_PIN, r); analogWrite(RGB3_G_PIN, g); analogWrite(RGB3_B_PIN, b);
}

// ---------- Servo handling ----------
static Servo servo1, servo2, servo3;

static int servoAngleForCountdown(int n) {
  // n is countdown digit 9..0
  if (n >= 7 && n <= 9) return 0;
  if (n >= 4 && n <= 6) return 60;
  if (n >= 1 && n <= 3) return 120;
  return 180; // n == 0
}

static void setServosForCountdown(int n) {
  int ang = servoAngleForCountdown(n);
  servo1.write(ang);
  servo2.write(ang);
  servo3.write(ang);
}

// ---------- Countdown & combined synchronization ----------
// We must keep everything in sync: same 2s duration per "state".
// State index corresponds to countdown digit n = 9..0 repeatedly.
// Additionally, RGB colors cycle every 2 seconds in sequence: 7 colors.
// There is no explicit stated phase relationship other than synchronization and each state lasts 2 seconds.
// We'll advance RGB color each 2 seconds along with countdown decrement.
static int colorIndexFromCountdownState(int countdownDigit) {
  // Since updates are simultaneous each state, we can implement with a separate color step counter.
  // For deterministic mapping, we use initial visible state: countdown starts at 9, RGB starts on Red.
  // Then after each decrement (every 2s), both countdown digit and RGB color advance.
  // Let initial step index s=0 at digit 9 -> color Red index 0.
  // For digit n, s = 9 - n, so colorIndex = s mod 7.
  int s = 9 - countdownDigit;
  int idx = s % 7;
  return idx;
}

// ---------- Model timing (10ms control period in contract) ----------
// We'll run a loop that updates on elapsed time; actuation only when 2s state boundary crosses.
static uint32_t lastStepMs = 0;
static bool initialized = false;

static int countdown = 9;

void applyOutputsForState(int digit) {
  // RGB color
  int colorIdx = colorIndexFromCountdownState(digit);
  setRGBChannelsForColor(colorIdx);

  // 7-seg
  uint8_t segBits = digitSegmentsActiveLow(digit);
  sr.write8(segBits);

  // Servos
  setServosForCountdown(digit);
}

void setup() {
  // Pin modes for RGB pins as PWM outputs
  pinMode(RGB1_R_PIN, OUTPUT); pinMode(RGB1_G_PIN, OUTPUT); pinMode(RGB1_B_PIN, OUTPUT);
  pinMode(RGB2_R_PIN, OUTPUT); pinMode(RGB2_G_PIN, OUTPUT); pinMode(RGB2_B_PIN, OUTPUT);
  pinMode(RGB3_R_PIN, OUTPUT); pinMode(RGB3_G_PIN, OUTPUT); pinMode(RGB3_B_PIN, OUTPUT);

  // Shift register pins
  sr.begin(SR_DS_PIN, SR_SHCP_PIN, SR_STCP_PIN);

  // Servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initial visible state assumed: countdown 9 and RGB starts on Red, servos at 0 degrees.
  // Apply immediately, and align step timer.
  applyOutputsForState(countdown);
  lastStepMs = millis();
  initialized = true;
}

void loop() {
  if (!initialized) return;

  uint32_t now = millis();
  // Keep a deterministic 2-second state duration.
  if ((uint32_t)(now - lastStepMs) >= STATE_PERIOD_MS) {
    lastStepMs += STATE_PERIOD_MS; // preserve ordering of updates by step cadence

    // Countdown from 9 to 0 then reset to 9
    if (countdown > 0) countdown--;
    else countdown = 9;

    // Actuation for the new synchronized state
    applyOutputsForState(countdown);
  }

  // No inputs; nothing else to do.
}
