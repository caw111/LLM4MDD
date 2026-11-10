#include <Arduino.h>
#include <Servo.h>

static constexpr uint8_t PIN_LED1 = 2;   // io_0
static constexpr uint8_t PIN_LED2 = 3;   // io_1
static constexpr uint8_t PIN_SR_LATCH = A1; // SHCP/STCP latch pin (STCP)
static constexpr uint8_t PIN_SR_CLOCK = A2; // SHCP
static constexpr uint8_t PIN_SR_DATA = A0;  // DS
static constexpr uint8_t PIN_SERVO1 = 4;     // io_3
static constexpr uint8_t PIN_SERVO2 = 5;     // io_4

// 74HC595, outputs are active-low for segments as specified
// Segment bits mapping per frozen contract:
// segment_bits: [0,1,2,3,4,5,6] correspond to Q0..Q6 -> A..G.
// We'll use bit index order (A,B,C,D,E,F,G) -> Q0,Q1,Q2,Q3,Q4,Q5,Q6.
//
// Digit to segments (A,B,C,D,E,F,G), active LOW => store active-high pattern then invert for output.
// For a 7-seg common-anode/cathode ambiguity, we follow contract's "active_low": true.
static uint8_t digitToSegmentsActiveLow(uint8_t digit) {
  // Active-low outputs: segment is ON when output bit is 0.
  // We create an active-high mask of ON segments (1=ON), then invert.
  uint8_t onMask = 0;
  // Indices: 0:A,1:B,2:C,3:D,4:E,5:F,6:G
  switch (digit) {
    case 0: onMask = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5); break;            // A B C D E F
    case 1: onMask = (1<<1)|(1<<2); break;                                        // B C
    case 2: onMask = (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6); break;                  // A B D E G
    case 3: onMask = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6); break;                  // A B C D G
    case 4: onMask = (1<<1)|(1<<2)|(1<<5)|(1<<6); break;                          // B C F G
    case 5: onMask = (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6); break;                  // A C D F G
    case 6: onMask = (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6); break;          // A C D E F G
    case 7: onMask = (1<<0)|(1<<1)|(1<<2); break;                                  // A B C
    case 8: onMask = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6); break;  // A B C D E F G
    case 9: onMask = (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6); break;          // A B C D F G
    default: onMask = 0; break;
  }
  uint8_t allSegBits = 0x7F; // 7 segments
  uint8_t activeLowOut = (~onMask) & allSegBits; // ON => 0
  return activeLowOut;
}

static void shift595WriteSegmentsActiveLow(uint8_t segBitsActiveLow7) {
  // SR outputs Q0..Q6 go to segments A..G respectively.
  // Need to shift 8 bits to 74HC595; one extra bit (Q7) not used. We'll set it high (inactive).
  // Determine bit order: in many Arduino examples MSB-first. We must preserve mapping to Q0..Q6.
  // If shifting MSB-first and using 8-bit value where bit7->first shifted to Q7, bit0->last shifted to Q0.
  // This makes last shifted bit correspond to Q0. We'll follow that.
  uint8_t q = (uint8_t)((segBitsActiveLow7 & 0x7F) | (0x80)); // Q7 inactive-high => 1
  // Latch low before shifting
  digitalWrite(PIN_SR_LATCH, LOW);
  // Shift out MSB first
  for (int i = 7; i >= 0; --i) {
    digitalWrite(PIN_SR_CLOCK, LOW);
    digitalWrite(PIN_SR_DATA, (q & (1u << i)) ? HIGH : LOW);
    digitalWrite(PIN_SR_CLOCK, HIGH);
  }
  // Latch
  digitalWrite(PIN_SR_LATCH, HIGH);
}

Servo servo1;
Servo servo2;

// Frozen controller states (initial values)
static int32_t ledPhaseMs = 0;      // ledPhaseMs
static bool ledOnIsLed2 = false;     // ledOnIsLed2
static int32_t sevsegCount = 0;     // sevsegCount
static int32_t sevsegPhaseMs = 0;  // sevsegPhaseMs
static int32_t servo1PhaseMs = 0;   // servo1PhaseMs
static int32_t servo2PhaseMs = 0;   // servo2PhaseMs

// io_5: delta_since_previous_step, control_period_ms=10
static int32_t io_5_delta = 0;

static uint32_t lastTickUs = 0;

static void controllerTick(int32_t deltaMs) {
  // perform action tick with exact ordered updates
  // first start; (no effect)
  // assign ledPhaseMs := ledPhaseMs + io_5
  ledPhaseMs += deltaMs;

  // then if ledPhaseMs >= 2000 { ledPhaseMs := ledPhaseMs - 2000; ledOnIsLed2 := not ledOnIsLed2 }
  if (ledPhaseMs >= 2000) {
    ledPhaseMs = ledPhaseMs - 2000;
    ledOnIsLed2 = !ledOnIsLed2;
  }

  // assign sevsegPhaseMs := sevsegPhaseMs + io_5
  sevsegPhaseMs += deltaMs;

  // then if sevsegPhaseMs >= 2000 { sevsegPhaseMs := sevsegPhaseMs - 2000; sevsegCount := (sevsegCount==9 ? 0 : sevsegCount+1) }
  if (sevsegPhaseMs >= 2000) {
    sevsegPhaseMs = sevsegPhaseMs - 2000;
    sevsegCount = (sevsegCount == 9) ? 0 : (sevsegCount + 1);
  }

  // assign servo1PhaseMs := servo1PhaseMs + io_5
  servo1PhaseMs += deltaMs;
  // assign servo2PhaseMs := servo2PhaseMs + io_5
  servo2PhaseMs += deltaMs;

  // if servo1PhaseMs >= 4000 { servo1PhaseMs := servo1PhaseMs - 4000 }
  if (servo1PhaseMs >= 4000) {
    servo1PhaseMs = servo1PhaseMs - 4000;
  }

  // if servo2PhaseMs >= 4000 { servo2PhaseMs := servo2PhaseMs - 4000 }
  if (servo2PhaseMs >= 4000) {
    servo2PhaseMs = servo2PhaseMs - 4000;
  }

  // outputs computed afterwards, per contract, in loop()
}

static double computeServo1Angle() {
  // io_3 expression copied structurally from frozen contract.
  // if servo1PhaseMs < 2000 then { if servo1PhaseMs < 200 then 0.0
  // else { if servo1PhaseMs < 2200 then { if servo1PhaseMs <= 1800 then ((servo1PhaseMs - 200)*180/1800) else 180 }
  // else { if servo1PhaseMs < 2200 then 180 else { if servo1PhaseMs < 2400 then 180 else { if servo1PhaseMs < 4000 then { if servo1PhaseMs <= 3800 then (180 - ((servo1PhaseMs-2400)*180/1600)) else 0 } else 0 }}}}
  // }
  // else { if servo1PhaseMs < 2200 then 180 else { if servo1PhaseMs < 2400 then 180 else { if servo1PhaseMs <= 3800 then (180 - ((servo1PhaseMs-2400)*180/1600)) else 0 }}}

  const double sp = (double)servo1PhaseMs;
  if (servo1PhaseMs < 2000) {
    if (servo1PhaseMs < 200) return 0.0;
    else {
      if (servo1PhaseMs < 2200) {
        if (servo1PhaseMs <= 1800) return ((sp - 200.0) * 180.0 / 1800.0);
        else return 180.0;
      } else {
        // servo1PhaseMs < 2200 condition cannot be true here, but keep exact structure
        if (servo1PhaseMs < 2200) return 180.0;
        else {
          if (servo1PhaseMs < 2400) return 180.0;
          else {
            if (servo1PhaseMs < 4000) {
              if (servo1PhaseMs <= 3800) return (180.0 - ((sp - 2400.0) * 180.0 / 1600.0));
              else return 0.0;
            } else return 0.0;
          }
        }
      }
    }
  } else {
    if (servo1PhaseMs < 2200) return 180.0;
    else {
      if (servo1PhaseMs < 2400) return 180.0;
      else {
        if (servo1PhaseMs <= 3800) return (180.0 - ((sp - 2400.0) * 180.0 / 1600.0));
        else return 0.0;
      }
    }
  }
}

static double computeServo2Angle() {
  const double sp = (double)servo2PhaseMs;
  // Copy structurally from frozen contract.
  if (servo2PhaseMs < 2000) {
    if (servo2PhaseMs < 200) return 180.0;
    else {
      if (servo2PhaseMs < 2200) {
        if (servo2PhaseMs <= 1800) return (180.0 - ((sp - 200.0) * 180.0 / 1800.0));
        else return 0.0;
      } else {
        if (servo2PhaseMs < 2200) return 0.0;
        else {
          if (servo2PhaseMs < 2400) return 0.0;
          else {
            if (servo2PhaseMs < 4000) {
              if (servo2PhaseMs <= 3800) return (((sp - 2400.0) * 180.0 / 1600.0));
              else return 180.0;
            } else return 180.0;
          }
        }
      }
    }
  } else {
    if (servo2PhaseMs < 2200) return 0.0;
    else {
      if (servo2PhaseMs < 2400) return 0.0;
      else {
        if (servo2PhaseMs <= 3800) return (((sp - 2400.0) * 180.0 / 1600.0));
        else return 180.0;
      }
    }
  }
}

void setup() {
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  pinMode(PIN_SR_LATCH, OUTPUT);
  pinMode(PIN_SR_CLOCK, OUTPUT);
  pinMode(PIN_SR_DATA, OUTPUT);

  digitalWrite(PIN_SR_LATCH, LOW);
  digitalWrite(PIN_SR_CLOCK, LOW);
  digitalWrite(PIN_SR_DATA, LOW);

  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);

  // Initialize outputs to match initial state values:
  // ledOnIsLed2=false => io_0 true, io_1 false
  digitalWrite(PIN_LED1, HIGH);
  digitalWrite(PIN_LED2, LOW);

  // sevsegCount=0
  uint8_t seg = digitToSegmentsActiveLow((uint8_t)sevsegCount);
  shift595WriteSegmentsActiveLow(seg);

  // servo phases initial 0 => compute angles
  double a1 = computeServo1Angle();
  double a2 = computeServo2Angle();
  servo1.write((int)lround(a1));
  servo2.write((int)lround(a2));

  lastTickUs = micros();
}

void loop() {
  // control period 10ms, compute delta since previous step
  uint32_t nowUs = micros();
  uint32_t elapsedUs = nowUs - lastTickUs;

  // If we haven't reached 10ms, wait a little.
  if (elapsedUs < 10000UL) {
    return;
  }

  // Determine deltaMs as elapsed rounded down to ms steps, but contract expects delta_since_previous_step
  // under 10ms periodic control. We'll use exact elapsedUs/1000 as deltaMs.
  int32_t deltaMs = (int32_t)(elapsedUs / 1000UL);
  if (deltaMs <= 0) deltaMs = 1;
  // Advance lastTickUs by the amount corresponding to whole deltaMs (keep period stable)
  lastTickUs += (uint32_t)deltaMs * 1000UL;

  controllerTick(deltaMs);

  // Outputs:
  // io_0 = (if ledOnIsLed2 ? false : true)
  digitalWrite(PIN_LED1, ledOnIsLed2 ? LOW : HIGH);
  digitalWrite(PIN_LED2, ledOnIsLed2 ? HIGH : LOW);

  // io_2 = sevsegCount
  uint8_t seg = digitToSegmentsActiveLow((uint8_t)sevsegCount);
  shift595WriteSegmentsActiveLow(seg);

  // io_3, io_4: servo angles (real) -> Servo.write expects int degrees
  double a1 = computeServo1Angle();
  double a2 = computeServo2Angle();
  servo1.write((int)lround(a1));
  servo2.write((int)lround(a2));
}