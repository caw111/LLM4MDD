#include <Arduino.h>

static constexpr uint8_t BTN1_PIN = 2;   // io_0
static constexpr uint8_t BTN2_PIN = 3;   // io_1

static constexpr uint8_t RGB_R_PIN = 4;  // io_2
static constexpr uint8_t RGB_G_PIN = 5;  // io_3
static constexpr uint8_t RGB_B_PIN = 6;  // io_4

static constexpr uint8_t SR_DATA_PIN  = A0; // ds -> sr1 DS, sr2 DS (see wiring)
static constexpr uint8_t SR_SHCP_PIN  = A2; // shcp -> sr1 SHCP, shared to sr2
static constexpr uint8_t SR_STCP_PIN  = A1; // stcp -> sr1 STCP, shared to sr2

// SysML model uses:
// io_0/io_1 as "press event" booleans (debounced in SysML), but frozen contract says:
// digital input active_high=false, pullup=true; model semantics true_level=LOW.
// In code we implement "press event" as edge-triggered debounced active-low buttons.
// This matches: controller uses "and io_x and not previousPressed" to detect rising edge.

static inline void srPulseClock() {
  // SHCP: rising edge shifts
  digitalWrite(SR_SHCP_PIN, HIGH);
  digitalWrite(SR_SHCP_PIN, LOW);
}
static inline void srPulseLatch() {
  // STCP: rising edge latches
  digitalWrite(SR_STCP_PIN, HIGH);
  digitalWrite(SR_STCP_PIN, LOW);
}

static void shift595_write8(bool activeLowSegments[7], bool enable595ForDigit) {
  // Not used directly; included for completeness.
  (void)activeLowSegments;
  (void)enable595ForDigit;
}

static void writeSrBits(uint8_t bits_sr1, uint8_t bits_sr2) {
  // ds -> A0, shared to sr1 DS and sr2 DS (per wiring). We'll clock 8 bits:
  // In a cascaded shift-register chain, the first 8 bits clocked end up in the last device.
  // Wiring indicates sr1 inputs from mega, sr2 DS wired to sr1 Q7S, so cascade is: SR1 -> SR2.
  // Therefore: shift out bits for SR2 first? With cascade, the bitstream order is:
  // - Bits shifted first go to SR2 (furthest) after 16 clocks, then to SR1.
  // We'll shift exactly 16 clocks: first 8 bits for SR1 (closer), then 8 for SR2 (farther) is wrong.
  // Instead, standard: if SR1 feeds SR2, then bits shifted out first enter SR2 first.
  // To keep behavior consistent for each digit, we will clock 16 bits MSB-first as:
  // [SR2 bits][SR1 bits], 8 each.
  //
  // Segment wiring is active-low. We'll keep bit assignment:
  // SR Q0..Q6 -> segments A..G. Q7S not connected for digit bits (but cascade still works with 8 clocks).
  // We'll set Q7 to 0 (off) for both shift registers.
  const uint16_t combined = ((uint16_t)bits_sr2 << 8) | (uint16_t)bits_sr1;

  digitalWrite(SR_STCP_PIN, LOW);
  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR_DATA_PIN, (combined & (1u << i)) ? HIGH : LOW);
    srPulseClock();
  }
  srPulseLatch();
}

static uint8_t digitToSegmentsActiveLow(uint8_t digit) {
  // For common-cathode 7seg display, segments are active-high.
  // However shift register outputs are wired directly to segment pins (with active_low required in contract).
  // So we must provide active-low segment pattern to shift registers:
  // bit0=Q0 -> segment A, bit1=Q1 -> B, ..., bit6=Q6 -> G.
  //
  // Active-low patterns for digits 0-9 (A B C D E F G):
  // Digit: 0 => A,B,C,D,E,F on (0), G off (1) => 0b1111110? careful order.
  // We'll compute by explicit mapping:
  // Standard active-low for 7-seg (common cathode? but here contract says active_low=true, so use active-low):
  // Commonly used active-low patterns for segments a..g (bit0=a ... bit6=g):
  // 0: 0x3F (0011 1111) for active-high? For active-low, that's 0x3F? Let's instead use reliable list:
  // Active-low bitset (0=on,1=off) for a..g:
  // 0 -> a,b,c,d,e,f on, g off: [0,0,0,0,0,0,1] => bits: b6..b0 = 1 0 0 0 0 0 0 => value with bit0=a is:
  // bit0(a)=0, bit1(b)=0, bit2(c)=0, bit3(d)=0, bit4(e)=0, bit5(f)=0, bit6(g)=1 => 0b1000000 = 0x40
  // 1 -> b,c on: [1,0,0,1,1,1,1] => bit0=1,bit1=0,bit2=0,bit3=1,bit4=1,bit5=1,bit6=1 => 0b11111001=0x79? with bit6..0 => 1 1 1 1 1 0 0 1 not 7 bits; compute 7 bits:
  // value = bit0 +2*bit1 +4*bit2 +8*bit3 +16*bit4 +32*bit5 +64*bit6
  // =1 +0 +0 +8 +16 +32 +64 =121 = 0x79
  //
  // We'll use these computed active-low values for a..g.
  switch (digit) {
    case 0: return 0x40; // g off
    case 1: return 0x79;
    case 2: return 0x24;
    case 3: return 0x30;
    case 4: return 0x19;
    case 5: return 0x12;
    case 6: return 0x02;
    case 7: return 0x78; // a,b,c? Actually 7 has a,b,c on: [0,0,1,0,1,1,1] -> value?
    case 8: return 0x00;
    case 9: return 0x10;
    default: return 0x00;
  }
}

static void displayNumber(uint8_t value) {
  uint8_t tens = value / 10;
  uint8_t ones = value % 10;

  // Active-low segments for each digit.
  // bits_srX: Q0..Q6 = segments A..G, active-low means 0 lights a segment.
  uint8_t seg_tens = digitToSegmentsActiveLow(tens);
  uint8_t seg_ones = digitToSegmentsActiveLow(ones);

  // Shift registers likely need Q7 as don't-care; keep it 1 (off) to avoid accidental latching effects.
  // But our write is 8 bits per SR; contract states segment_bits [0..6] only.
  // We'll set bit7=1 (active-low off) so that corresponding output won't light any segment.
  uint8_t bits_sr1 = (uint8_t)((seg_tens & 0x7F) | (1u << 7));
  uint8_t bits_sr2 = (uint8_t)((seg_ones & 0x7F) | (1u << 7));

  writeSrBits(bits_sr1, bits_sr2);
}

static void applyRgbForDisplayValue(int32_t displayValue) {
  // Model outputs: io_2=R true for [0..33], io_3=G true for [34..66], io_4=B true for [67..99]
  bool r = (displayValue >= 0 && displayValue <= 33);
  bool g = (displayValue >= 34 && displayValue <= 66);
  bool b = (displayValue >= 67 && displayValue <= 99);

  // Contract says RGB LED common cathode; and model outputs are active-high.
  // We drive each pin HIGH/LOW accordingly.
  digitalWrite(RGB_R_PIN, r ? HIGH : LOW);
  digitalWrite(RGB_G_PIN, g ? HIGH : LOW);
  digitalWrite(RGB_B_PIN, b ? HIGH : LOW);
}

struct DebouncedPressEdge {
  // Implements "io_x" as press event boolean, debounced.
  // Active-low buttons with INPUT_PULLUP.
  bool stableLevel = true;     // not pressed -> HIGH
  bool lastStableLevel = true;
  bool event = false;
  uint32_t lastChangeMs = 0;
  uint32_t stableSinceMs = 0;
  bool init = false;
};

static DebouncedPressEdge dbBtn1, dbBtn2;

static void updateDebouncedPressEdge(DebouncedPressEdge &db, uint8_t pin, uint32_t nowMs) {
  bool raw = digitalRead(pin); // HIGH not pressed, LOW pressed
  if (!db.init) {
    db.init = true;
    db.stableLevel = raw;
    db.lastStableLevel = raw;
    db.lastChangeMs = nowMs;
    db.stableSinceMs = nowMs;
    db.event = false;
    return;
  }

  if (raw != db.stableLevel) {
    // Start debounce timer when level changes
    db.lastChangeMs = nowMs;
    db.stableLevel = raw; // tentatively treat as changed (common approach)
  }

  // Determine stable when raw has remained at the same level for debounce duration.
  // Since we only stored stableLevel, we approximate by checking current raw vs stored stableLevel changes;
  // more robust: keep candidate. We'll keep simple: require raw==stored stableLevel AND time since lastChange >= threshold.
  const uint32_t debounceMs = 25;
  if (raw == db.stableLevel && (nowMs - db.lastChangeMs) >= debounceMs) {
    // Now stable at current level.
    bool pressedStable = (db.stableLevel == LOW);
    bool previouslyPressedStable = (db.lastStableLevel == LOW);

    db.event = (pressedStable && !previouslyPressedStable);
    db.lastStableLevel = db.stableLevel;
  } else {
    db.event = false;
  }
}

static inline int32_t wrap99plus(int32_t v) { // for increment when v>=99 =>0 else v+1
  if (v >= 99) return 0;
  return v + 1;
}
static inline int32_t wrap99minus(int32_t v) { // for decrement when v<=0 =>99 else v-1
  if (v <= 0) return 99;
  return v - 1;
}

int32_t displayValue = 0;                 // model state
bool previousBtn1Pressed = false;         // model state
bool previousBtn2Pressed = false;         // model state

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_SHCP_PIN, OUTPUT);
  pinMode(SR_STCP_PIN, OUTPUT);

  digitalWrite(SR_DATA_PIN, LOW);
  digitalWrite(SR_SHCP_PIN, LOW);
  digitalWrite(SR_STCP_PIN, LOW);

  // Initialization: displays "00", RGB off.
  displayValue = 0;
  previousBtn1Pressed = false;
  previousBtn2Pressed = false;

  // Contract: RGB off at startup means all channels false.
  digitalWrite(RGB_R_PIN, LOW);
  digitalWrite(RGB_G_PIN, LOW);
  digitalWrite(RGB_B_PIN, LOW);

  displayNumber(0);
}

void loop() {
  static uint32_t lastTickMs = 0;
  const uint32_t periodMs = 5;

  uint32_t now = millis();
  if ((now - lastTickMs) < periodMs) return;
  lastTickMs = now;

  // Debounced press events for io_0/io_1:
  updateDebouncedPressEdge(dbBtn1, BTN1_PIN, now);
  updateDebouncedPressEdge(dbBtn2, BTN2_PIN, now);

  bool io_0 = dbBtn1.event; // btn1 press event boolean
  bool io_1 = dbBtn2.event; // btn2 press event boolean

  // Controller.tick() in order-preserving manner.
  // first start;
  // then if (io_0 and not previousBtn1Pressed) displayValue = displayValue>=99?0:displayValue+1;
  if (io_0 && !previousBtn1Pressed) {
    displayValue = wrap99plus(displayValue);
  }

  // then if (io_1 and not previousBtn2Pressed) displayValue = displayValue<=0?99:displayValue-1;
  if (io_1 && !previousBtn2Pressed) {
    displayValue = wrap99minus(displayValue);
  }

  // then assign previousBtn1Pressed := io_0; previousBtn2Pressed := io_1;
  previousBtn1Pressed = io_0;
  previousBtn2Pressed = io_1;

  // Outputs based on updated displayValue.
  // Display update should happen after each button press; updating every tick is acceptable (doesn't simplify behavior).
  displayNumber((uint8_t)displayValue);
  applyRgbForDisplayValue(displayValue);
}