#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_77\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static const uint8_t BTN_PIN = 2;

// 74HC595 pins from circuit
static const uint8_t SR_DS  = A0; // DS -> Qx data to shift register
static const uint8_t SR_STCP = A1; // STCP -> latch
static const uint8_t SR_SHCP = A2; // SHCP -> shift clock

static const uint8_t NUM_BITS = 8;

// Sevseg segments a,b,c,d,e,f,g,dp (no dp wiring specified; keep it off)
static const uint8_t segMapCommonAnode[10] = {
  // a b c d e f g dp
  // 0: a b c d e f on, g off, dp off
  0b1111110, // bits 6..0 for a..g? We'll define output mapping via segment bits below.
  0b0110000,
  0b1101101,
  0b1111001,
  0b0110011,
  0b1011011,
  0b1011111,
  0b1110000,
  0b1111111,
  0b1111011
};

// We'll build byte in the order: Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G.
// dp (Q7) not connected in circuit; keep it off.
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_77\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitTo74HC595Mask(uint8_t n);
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_77\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWriteByte(uint8_t data);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_77\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplayAndServos();
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_77\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 128 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_77\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_77\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitTo74HC595Mask(uint8_t n) {
  // segMapCommonAnode above encodes typical 7-seg for segments a..g where 1=segment ON
  // for a common-anode display. If your display is common-cathode, invert mask.
  // The circuit doesn't specify; Wokwi 7-segment typically assumes common cathode behavior
  // for the Q outputs as wired. We'll implement for common cathode: segment ON = 1.
  //
  // BUT our map values above are for common-cathode? Let's match standard:
  // For common-cathode: 0 => a b c d e f on (g off) => 0b1111110 if bits are a..g.
  // We'll set order a..g and ignore dp.
  const uint8_t m = segMapCommonAnode[n]; // bits 6..0 correspond to a..g? We'll treat as:
  // We need explicit mapping: bits in m from LSB to MSB represent g..a? Our literals appear as standard gfedcba encoding.
  // Let's convert using a known table approach instead of guessing.
  // Standard 7-seg encodings (common cathode) often use bits: dp g f e d c b a (LSB=a, MSB=g).
  // The Wokwi 7-segment expects individual segment signals; we drive them from Q0..Q6 (A..G).
  //
  // We'll use a correct mapping: for digit d, determine which segments are ON:
  // a b c d e f g
  static const uint8_t segOn[10][7] = {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,1,0,1,1,0,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,0,1,1}, // 4
    {1,0,1,1,0,1,1}, // 5
    {1,0,1,1,1,1,1}, // 6
    {1,1,1,0,0,0,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,0,1,1}  // 9
  };

  uint8_t mask = 0;
  // Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G, Q7->dp (unused)
  if (segOn[n][0]) mask |= (1 << 0); // A
  if (segOn[n][1]) mask |= (1 << 1); // B
  if (segOn[n][2]) mask |= (1 << 2); // C
  if (segOn[n][3]) mask |= (1 << 3); // D
  if (segOn[n][4]) mask |= (1 << 4); // E
  if (segOn[n][5]) mask |= (1 << 5); // F
  if (segOn[n][6]) mask |= (1 << 6); // G
  // dp kept off at Q7
  return mask;
}

static void srWriteByte(uint8_t data) {
  // Shift out 8 bits to 74HC595 (SR_DS=DS, SR_SHCP=SHCP, SR_STCP=STCP)
  // We'll output MSB first so Q0 ends up receiving LSB? Need to align with wiring:
  // The circuit connects sr1:Q0 -> sevseg1:A, Q1->B, ... Q6->G.
  // For a standard 74HC595, the first shifted bit ends up in Q7, last in Q0 (when shifting MSB first).
  // Therefore, if we want Q0..Q6 to correspond to bits 0..6 in our mask,
  // we should shift MSB first (bit7 first) so that bit0 ends at Q0.
  for (int8_t i = NUM_BITS - 1; i >= 0; --i) {
    digitalWrite(SR_DS, (data >> i) & 0x01);
    digitalWrite(SR_SHCP, HIGH);
    digitalWrite(SR_SHCP, LOW);
  }
  // Latch
  digitalWrite(SR_STCP, HIGH);
  digitalWrite(SR_STCP, LOW);
}

Servo servo1;
Servo servo2;

static int numberVal = 0;

static void updateDisplayAndServos() {
  // Display update immediately
  uint8_t mask = digitTo74HC595Mask((uint8_t)numberVal);
  srWriteByte(mask);

  // Servo movement: even -> 0°, odd -> 90°
  int angle = (numberVal % 2 == 0) ? 0 : 90;
  servo1.write(angle);
  servo2.write(angle);
}

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);

  pinMode(SR_DS, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);
  pinMode(SR_STCP, OUTPUT);

  digitalWrite(SR_SHCP, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_DS, LOW);

  servo1.attach(3); // mega:3 -> servo1:PWM
  servo2.attach(5); // mega:5 -> servo2:PWM

  // Initialization: show "0" and both servos at 0°
  numberVal = 0;
  servo1.write(0);
  servo2.write(0);
  updateDisplayAndServos();
}

void loop() {
  static int lastBtnState = HIGH;
  int btnState = digitalRead(BTN_PIN);

  // Detect press (active-low, edge detect)
  if (lastBtnState == HIGH && btnState == LOW) {
    // Increment after each button press, reset if exceeds 9
    numberVal++;
    if (numberVal > 9) numberVal = 0;

    // Immediately update display and servos
    updateDisplayAndServos();

    // Debounce
    delay(50);
  }

  lastBtnState = btnState;
}
