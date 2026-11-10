#include <Arduino.h>

// Frozen interface contract bindings
// btn1: D2 (io_0), btn2: D3 (io_1), btn3: D4 (io_2) all INPUT_PULLUP (pressed = LOW)
// pot1: A3 (io_3), shift-register chain: sr1 via DS=A0, SHCP=A2, STCP=A1
// sr1->sr2 daisy chain handled by wiring (STCP/SHCP shared, sr2 input is sr1 Q7S -> Q7S to sr2 DS)
// Displays use active-low segment bits. Leading zeros preserved, sr1=tens, sr2=ones.

static constexpr uint8_t PIN_BTN1 = 2;   // io_0
static constexpr uint8_t PIN_BTN2 = 3;   // io_1
static constexpr uint8_t PIN_BTN3 = 4;   // io_2
static constexpr uint8_t PIN_POT  = A3;  // io_3

static constexpr uint8_t PIN_DS   = A0;  // sr1 DS  (data)
static constexpr uint8_t PIN_STCP = A1;  // sr1 STCP (latch)
static constexpr uint8_t PIN_SHCP = A2;  // sr1 SHCP (clock)

// 74HC595 typical: MR held inactive by sr1.VCC to MR.
// OE tied to GND (sr1 OE). We only drive DS/SHCP/STCP.

static constexpr bool ACTIVE_LOW_SEGMENTS = true;

static int32_t number_model = 0;
static bool prevBtn1 = false;
static bool prevBtn2 = false;
static bool prevBtn3 = false;
static int32_t accumMs = 0;

static uint32_t lastStepUs = 0;

// Segment encoding for digits 0-9 on a common mapping where segments are active-low.
// We must respect wiring: sr outputs Q0..Q6 correspond to segments A..G.
static uint8_t digitSegmentsActiveLow[10] = {
  // bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G (1 means segment ON for active-high).
  // We'll convert to active-low at send time.
  0b0111111, // 0: A B C D E F on, G off
  0b0000110, // 1: B C
  0b1011011, // 2: A B D E G
  0b1001111, // 3: A B C D G
  0b1100110, // 4: B C F G
  0b1101101, // 5: A C D F G
  0b1111101, // 6: A C D E F G
  0b0000111, // 7: A B C
  0b1111111, // 8: all
  0b1101111  // 9: A B C D F G
};

// Send a single 7-segment digit into one 74HC595 segment register.
// active_low=true: output bit should be 0 when segment should be on.
static void writeDigitToRegister(uint8_t digitIndex, uint8_t sevMaskActiveHighAtoG, bool active_low) {
  (void)digitIndex;

  // Build 8-bit payload for the 'digit' register:
  // Q0..Q6 map to segments A..G respectively, Q7 is unused (Q7S) but present.
  // We'll set Q7=0 (off) to avoid affecting anything.
  uint8_t payload = 0;
  // payload bits represent what we shift into Q outputs:
  // We want segments ON when active_low==true -> output bit LOW.
  // Let's interpret payload bit=1 means output HIGH at Qx.
  // Convert active-high mask (1 means segment ON) to Q output levels:
  // If active_low: output level = !segOn  (segment ON -> output LOW => bit 0)
  // If not: output level = segOn.
  for (uint8_t seg = 0; seg < 7; seg++) {
    bool segOn = (sevMaskActiveHighAtoG >> seg) & 0x1;
    bool outLevel = active_low ? !segOn : segOn;
    payload |= (outLevel ? (1u << seg) : 0u);
  }
  // Q7 unused: set to 0 output level (don't care)
  // Keeping as 0 in payload.
  payload &= 0x7Fu;

  // Shift out 8 bits into the daisy chain:
  // We are sending two registers worth (sr1 then sr2).
  // This function only fills one digit (either tens into sr1 or ones into sr2).
  // Caller will send combined payload; so this function is unused for combined.
  (void)payload;
}

static void updateDisplays(int32_t value) {
  // io_5 -> number (0..99)
  if (value < 0 || value > 99) return; // invalid_policy="blank" not specified; keep behavior safe.

  uint8_t tens = (uint8_t)(value / 10);
  uint8_t ones = (uint8_t)(value % 10);

  // Need to preserve leading zeros: tens always shown.
  uint8_t maskT = digitSegmentsActiveLow[tens]; // active-high mask A..G for digit
  uint8_t maskO = digitSegmentsActiveLow[ones]; // active-high mask A..G for digit

  // Build 8-bit payload per register in shift order:
  // DS connected to sr1 DS; chain is sr1 -> sr2 via Q7S.
  // When clocking, first shifted bits end up in sr2 after 16 clocks (depending on implementation).
  // For daisy-chain of 2x74HC595 with sr1 connected to sr2 DS:
  // - Shift register closest to DS (sr1) receives the earliest shifted bits.
  // - After 16 clocks, sr2 will contain the last 8 bits shifted.
  // We'll therefore shift: [sr1_payload][sr2_payload] in that order (8+8 bits), and then latch.
  auto buildPayload = [&](uint8_t segOnMaskActiveHighAtoG) -> uint8_t {
    uint8_t payload = 0;
    for (uint8_t seg = 0; seg < 7; seg++) {
      bool segOn = (segOnMaskActiveHighAtoG >> seg) & 0x1;
      bool outLevel = ACTIVE_LOW_SEGMENTS ? !segOn : segOn;
      if (outLevel) payload |= (1u << seg);
    }
    payload &= 0x7Fu;
    return payload;
  };

  uint8_t sr1_payload = buildPayload(maskT); // tens digit
  uint8_t sr2_payload = buildPayload(maskO); // ones digit

  // Ensure pins are in known states
  // DS line on A0, we directly use digitalWrite for simplicity.
  for (int i = 7; i >= 0; --i) {
    digitalWrite(PIN_DS, ((sr1_payload >> i) & 0x1) ? HIGH : LOW);
    digitalWrite(PIN_SHCP, HIGH);
    // short hold
    digitalWrite(PIN_SHCP, LOW);
  }
  for (int i = 7; i >= 0; --i) {
    digitalWrite(PIN_DS, ((sr2_payload >> i) & 0x1) ? HIGH : LOW);
    digitalWrite(PIN_SHCP, HIGH);
    digitalWrite(PIN_SHCP, LOW);
  }

  // Latch both registers
  digitalWrite(PIN_STCP, HIGH);
  // small hold
  digitalWrite(PIN_STCP, LOW);
}

static inline bool readBtnPressed(uint8_t pin) {
  // INPUT_PULLUP: pressed LOW => model true when physical LOW
  return digitalRead(pin) == LOW;
}

static int32_t readIntervalDeltaMs() {
  uint32_t nowUs = micros();
  if (lastStepUs == 0) {
    lastStepUs = nowUs;
    return 0;
  }
  uint32_t deltaUs = nowUs - lastStepUs;
  lastStepUs = nowUs;

  // Convert to ms (integer), matching io_4 integer semantics.
  return (int32_t)(deltaUs / 1000UL);
}

static double readPotADC() {
  // Arduino returns [0..1023]
  int raw = analogRead(PIN_POT);
  return (double)raw;
}

void setup() {
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_BTN3, INPUT_PULLUP);

  pinMode(PIN_DS, OUTPUT);
  pinMode(PIN_SHCP, OUTPUT);
  pinMode(PIN_STCP, OUTPUT);

  digitalWrite(PIN_DS, LOW);
  digitalWrite(PIN_SHCP, LOW);
  digitalWrite(PIN_STCP, LOW);

  // Initialize accepted model state:
  // number=0, prevBtn1=false, prevBtn2=false, prevBtn3=false, accumMs=0
  number_model = 0;
  prevBtn1 = false;
  prevBtn2 = false;
  prevBtn3 = false;
  accumMs = 0;

  // Force display to "00" on startup
  updateDisplays(number_model);

  lastStepUs = 0;
}

void loop() {
  // control_period_ms = 10 requested. We'll run at ~10ms cadence with delta fed back.
  static uint32_t lastControlMs = 0;
  uint32_t nowMs = millis();
  if (nowMs - lastControlMs < 10) {
    return;
  }
  lastControlMs = nowMs;

  // io inputs
  bool io_0 = readBtnPressed(PIN_BTN1);
  bool io_1 = readBtnPressed(PIN_BTN2);
  bool io_2 = readBtnPressed(PIN_BTN3);
  double io_3 = readPotADC(); // adc_count mapped; model expects Real
  int32_t io_4 = readIntervalDeltaMs(); // delta_since_previous_step in ms (integer)

  // Accepted executable model tick
  // IMPORTANT: preserve exact update ordering as in the frozen SysML.
  // Controller tick:
  // first start;
  // then if io_2 { number=0; accumMs=0; }
  if (io_2) {
    number_model = 0;
    accumMs = 0;
  }

  // then if (not io_2) and io_0 and (not prevBtn1) => edge increment, accumMs=0
  if ((!io_2) && io_0 && (!prevBtn1)) {
    if (number_model >= 99) number_model = 0;
    else number_model = number_model + 1;
    accumMs = 0;
  }

  // then if (not io_2) and io_1 and (not prevBtn2) => edge decrement
  if ((!io_2) && io_1 && (!prevBtn2)) {
    if (number_model == 0) number_model = 99;
    else number_model = number_model - 1;
    accumMs = 0;
  }

  // then if (not io_2) and (not io_0) and (not io_1) accumMs=0
  if ((!io_2) && (!io_0) && (!io_1)) {
    accumMs = 0;
  }

  // then if (not io_2) and io_0 and (not io_1) { accumMs=accumMs+io_4; if accumMs >= (100+floor(...)) then step ++ and accumMs=0 }
  if ((!io_2) && io_0 && (!io_1)) {
    accumMs = accumMs + io_4;
    // Compute threshold exactly: 100 + floor(io_3*900.0/1023.0)
    int32_t thresh = 100 + (int32_t)floor((io_3 * 900.0) / 1023.0);
    if (accumMs >= thresh) {
      if (number_model >= 99) number_model = 0;
      else number_model = number_model + 1;
      accumMs = 0;
    }
  }

  // then if (not io_2) and io_1 and (not io_0) similarly decrement
  if ((!io_2) && io_1 && (!io_0)) {
    accumMs = accumMs + io_4;
    int32_t thresh = 100 + (int32_t)floor((io_3 * 900.0) / 1023.0);
    if (accumMs >= thresh) {
      if (number_model == 0) number_model = 99;
      else number_model = number_model - 1;
      accumMs = 0;
    }
  }

  // then assign prevBtn1 := io_0; prevBtn2 := io_1; prevBtn3 := io_2
  prevBtn1 = io_0;
  prevBtn2 = io_1;
  prevBtn3 = io_2;

  // Output io_5 := number (update display every tick so immediately reflects new number)
  updateDisplays(number_model);
}