#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_42\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

struct SevsegShiftRegister {
  // Two 7-seg displays are driven via two separate 74HC595 chains (sr1, sr2).
  // This struct provides a simple way to set segments for one digit.
  uint8_t shcpPin;
  uint8_t stcpPin;
  uint8_t dsPin;     // serial data
  uint8_t oePin;     // OE is tied together and to GND in the circuit, so unused
  uint8_t mrPin;     // MR tied to VCC through SR VCC-MR link in circuit, so unused

  void begin() {
    pinMode(shcpPin, OUTPUT);
    pinMode(stcpPin, OUTPUT);
    pinMode(dsPin, OUTPUT);
    // oePin/mrPin are not controlled (wired to keep enabled)
  }

  void setSegments(uint8_t segmentMask) {
    // segmentMask uses bits: A=0, B=1, C=2, D=3, E=4, F=5, G=6, DP=7 (we don't use DP)
    // Hardware mapping from Q outputs to sevseg segments per circuit:
    // srX:Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G, Q7S->DP (not connected)
    uint8_t bits[8];

    // Q7S is connected to COM.1? Actually Q7S -> DS of next chain; not to DP.
    // So we only shift 7 bits for A..G. We'll still clock 8 bits with DP=0 to keep chain length consistent.
    for (int i = 0; i < 8; i++) {
      bits[i] = (segmentMask >> i) & 0x01;
    }

    // Shift out LSB first to match typical 74HC595 where Q0 corresponds to first shifted bit.
    // The circuit wiring indicates: sr1:Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G.
    // We therefore map A..G to shifted bits 0..6.
    digitalWrite(stcpPin, LOW);
    for (int i = 0; i < 8; i++) {
      digitalWrite(shcpPin, LOW);
      digitalWrite(dsPin, bits[i] ? HIGH : LOW);
      digitalWrite(shcpPin, HIGH);
    }
    digitalWrite(stcpPin, HIGH);
    digitalWrite(stcpPin, LOW);
  }
};

// Pins based on the circuit:
// SR1: SHCP=A2, STCP=A1, DS=A0
// SR2 shares SHCP and STCP and uses DS from SR1: Q7S (cascade). In the circuit DS of sr2 is sr1:Q7S,
// so we only need to shift enough bits through sr1 to program both digits.
// To preserve behavior, we still conceptually treat each digit separately; however the actual hardware is a cascade:
// shifting 8 bits into sr1 will update both sr1 and sr2 Q outputs for A..G (sr1 and sr2 digit segments).
//
// Therefore, we implement a cascade write that shifts 16 bits total (for sr1 and sr2):
// - First 8 bits go into sr1 (digit1 segments -> sevseg1)
// - Next 8 bits go into sr2 (digit2 segments -> sevseg2)
// Order depends on cascade wiring; with Q7S -> sr2 DS, the second byte becomes the next-stage output.
// We will shift LSB-first for each byte and send sr2 segments as the next byte.
//
// This approach keeps the two-digit update atomic and synchronized.

static const uint8_t segmentMaskForDigit[10] = {
  // bits: A,B,C,D,E,F,G = bits 0..6
  // Using standard common segment mapping (1 = segment ON).
  // 0: A B C D E F
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),
  // 1: B C
  (1 << 1) | (1 << 2),
  // 2: A B D E G
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),
  // 3: A B C D G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),
  // 4: B C F G
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),
  // 5: A C D F G
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),
  // 6: A C D E F G
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 7: A B C
  (1 << 0) | (1 << 1) | (1 << 2),
  // 8: A B C D E F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 9: A B C D F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)
};

static const uint8_t SHCP_PIN = A2; // mega:A2 -> sr1:SHCP
static const uint8_t STCP_PIN = A1; // mega:A1 -> sr1:STCP
static const uint8_t DS_PIN   = A0; // mega:A0 -> sr1:DS

// Bargraph pins:
// mega:2..11 -> bargraph1:A1..A10 (green)
// bargraph1:C1..C10 all to mega:GND.1 (black). We drive only A side.
static const uint8_t BAR_PINS[10] = {2,3,4,5,6,7,8,9,10,11};

// Servos:
static const uint8_t SERVO1_PIN = 12; // mega:12 -> servo1:PWM
static const uint8_t SERVO2_PIN = 13; // mega:13 -> servo2:PWM
static const uint8_t SERVO3_PIN = 44; // mega:44 -> servo3:PWM

Servo servo1;
Servo servo2;
Servo servo3;

#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_42\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllBargraphOff();
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_42\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBargraphTens(uint8_t number);
#line 117 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_42\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void writeTwoDigitSevseg(uint8_t tensDigit, uint8_t unitsDigit);
#line 145 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_42\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setServosForUnits(uint8_t unitsDigit);
#line 156 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_42\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 187 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_42\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_42\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllBargraphOff() {
  for (int i = 0; i < 10; i++) digitalWrite(BAR_PINS[i], LOW);
}

static void setBargraphTens(uint8_t number) {
  uint8_t tens = number / 10;
  if (tens > 10) tens = 10; // safety; number range is 0..99
  // Light up a number of LEDs equal to the tens digit.
  for (int i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], (i < tens) ? HIGH : LOW);
  }
}

static void writeTwoDigitSevseg(uint8_t tensDigit, uint8_t unitsDigit) {
  // Cascade 74HC595: sr1 drives sevseg1, sr2 drives sevseg2.
  // We shift 16 bits: first byte for sr1(sevseg1), second byte for sr2(sevseg2).
  uint8_t seg1 = segmentMaskForDigit[tensDigit];  // sevseg1
  uint8_t seg2 = segmentMaskForDigit[unitsDigit]; // sevseg2

  digitalWrite(STCP_PIN, LOW);

  // Shift out byte for sr1 (LSB-first)
  for (int i = 0; i < 8; i++) {
    digitalWrite(SHCP_PIN, LOW);
    digitalWrite(DS_PIN, (seg1 >> i) & 0x01 ? HIGH : LOW);
    digitalWrite(SHCP_PIN, HIGH);
  }

  // Shift out byte for sr2 (LSB-first), DS comes from sr1:Q7S automatically for cascaded stage.
  // However, the hardware cascade requires shifting more bits; the second byte must be present
  // on the same DS stream after the first byte, so we continue shifting with seg2 bits.
  for (int i = 0; i < 8; i++) {
    digitalWrite(SHCP_PIN, LOW);
    digitalWrite(DS_PIN, (seg2 >> i) & 0x01 ? HIGH : LOW);
    digitalWrite(SHCP_PIN, HIGH);
  }

  digitalWrite(STCP_PIN, HIGH);
  digitalWrite(STCP_PIN, LOW);
}

static void setServosForUnits(uint8_t unitsDigit) {
  uint8_t d = unitsDigit % 10;
  int angle1 = d * 18;
  int angle2 = d * 18 + 60;
  int angle3 = d * 18 + 120;

  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);
}

void setup() {
  // 74HC595 control pins
  pinMode(SHCP_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(DS_PIN, OUTPUT);
  digitalWrite(SHCP_PIN, LOW);
  digitalWrite(STCP_PIN, LOW);
  digitalWrite(DS_PIN, LOW);

  // Bargraph pins
  for (int i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialization requirements
  // - sevseg shows "00"
  // - bargraph off
  // - servos at 0 degrees
  writeTwoDigitSevseg(0, 0);
  setAllBargraphOff();

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
}

void loop() {
  static uint8_t number = 0;              // 0..99
  static unsigned long lastUpdateMs = 0; // for 2s increment

  unsigned long now = millis();

  // Special servo behavior when the displayed number has units digit 9:
  // "When sevseg displays 9, hold this state for 2 seconds, then rotate servo 1, servo 2, servo 3 to 0 degrees."
  //
  // Interpret as: whenever the incremented number reaches x9, i.e., units digit is 9, we:
  // 1) set servos according to units digit (which yields angles based on 9)
  // 2) hold for 2 seconds (during which the number remains displayed at x9 and other components reflect it)
  // 3) then rotate all servos to 0 degrees
  //
  // Number increment is every 2 seconds, and display should update immediately after each number increment.
  // To preserve both behaviors:
  // - At normal numbers, advance every 2 seconds.
  // - At units digit 9, advance after 2 seconds as well, but immediately after that 2s window,
  //   set servos to 0 degrees before the next number update.
  static bool justEnteredNine = false;
  static bool nineServoZeroed = false;

  // Ensure components are consistent whenever number changes.
  // We'll update components at each loop iteration when appropriate.
  // Use flags tied to number timing.
  if (now - lastUpdateMs >= 2000) {
    lastUpdateMs = now;

    uint8_t tens = number / 10;
    uint8_t units = number % 10;

    // Update display and bargraph immediately (and at initialization, already done).
    writeTwoDigitSevseg(tens, units);
    setBargraphTens(number);

    // Servo updates:
    if (units == 9) {
      // Enter/refresh state at x9
      if (!justEnteredNine) {
        // On reaching x9, set servos for units digit 9 and hold for 2 seconds (this block runs after 2s).
        // However, the number increment schedule already makes the system re-evaluate at 2s boundaries.
        // We'll set servos for 9 right now, and then schedule zeroing on the next 2s boundary.
        setServosForUnits(9);
        justEnteredNine = true;
        nineServoZeroed = false;
      } else {
        // This is the 2-second hold expiration moment: rotate servos to 0 degrees.
        if (!nineServoZeroed) {
          servo1.write(0);
          servo2.write(0);
          servo3.write(0);
          nineServoZeroed = true;
        }
        // Prepare to increment number normally after this moment.
      }
    } else {
      // For non-9 units digit, allow normal servo positioning.
      justEnteredNine = false;
      nineServoZeroed = false;
      setServosForUnits(units);
    }

    // Now increment number for the next display interval, unless currently at x9 and we need to keep it for 2s.
    // The timing already consumed a 2s interval. The number should increment by 1 every 2 seconds.
    // Therefore, increment here every 2s boundary.
    number++;
    if (number > 99) number = 0;
  } else {
    // For tighter consistency: update servos only on number changes.
    // No action needed between 2s boundaries.
  }

  // Additionally, when entering loop first time, we must apply servo position and bargraph/segs for number=0.
  // The setup already did "00" and bargraph off and servos 0 degrees, matching requirements.
  // Servo for 0 should also be 0 degrees by formula: units 0 => servo1 0, servo2 60, servo3 120,
  // but requirement explicitly says initialize all servos at 0 degrees.
  // The formula-based behavior is intended for ongoing operation; however "Servo Control" says angles are calculated
  // based on units digit of current number. To preserve both: after the first 2s increment, behavior follows formula.
  // We already do that by setting servos at each 2s boundary.
}
