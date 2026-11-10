#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint8_t BTN1_PIN = 2;   // io_0
static constexpr uint8_t BTN2_PIN = 3;   // io_1
static constexpr uint8_t BTN3_PIN = 4;   // io_2

// RGB1 pins (io_3..io_5): D5/D6/D7 common cathode
static constexpr uint8_t RGB1_R = 5; // io_3
static constexpr uint8_t RGB1_G = 6; // io_4
static constexpr uint8_t RGB1_B = 7; // io_5

// RGB2 pins (io_6..io_8): D8/D9/D10 common cathode
static constexpr uint8_t RGB2_R = 8;  // io_6
static constexpr uint8_t RGB2_G = 9;  // io_7
static constexpr uint8_t RGB2_B = 10; // io_8

// 74HC595 pins for sevseg1 via frozen binding:
// clock_pin A2 (SHCP) , data_pin A0 (DS), latch_pin A1 (STCP)
static constexpr uint8_t SR_SHCP = A2; // clock
static constexpr uint8_t SR_STCP = A1; // latch
static constexpr uint8_t SR_DS   = A0; // data

static constexpr uint8_t SR_OE = 255;   // OE is tied to GND in circuit
static constexpr uint8_t SR_MR = 255;   // MR is tied to VCC in circuit

// Controller period: 5 ms (used as delay budget)
static constexpr uint32_t CONTROL_PERIOD_MS = 5;

// -------------------- Frozen controller state --------------------
static int32_t count_ = 0;
static bool prevBtn0_ = false;
static bool prevBtn1_ = false;
static bool prevBtn2_ = false;

static bool debounceStable0_ = false;
static bool debounceStable1_ = false;
static bool debounceStable2_ = false;

static bool debounceCand0_ = false;
static bool debounceCand1_ = false;
static bool debounceCand2_ = false;

static int32_t debounceCnt0_ = 0;
static int32_t debounceCnt1_ = 0;
static int32_t debounceCnt2_ = 0;

// -------------------- Hardware helpers --------------------
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeRGB(bool r_on, bool g_on, bool b_on, uint8_t r_pin, uint8_t g_pin, uint8_t b_pin);
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void srPulseClock();
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitSegmentsActiveLow_AtoG(uint8_t d);
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void sevsegWriteDigit0to9(uint8_t digit);
#line 140 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateOutputsFromModel();
#line 174 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 212 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_89\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void writeRGB(bool r_on, bool g_on, bool b_on,
                             uint8_t r_pin, uint8_t g_pin, uint8_t b_pin) {
  // Common cathode LEDs: active-high to turn ON.
  digitalWrite(r_pin, r_on ? HIGH : LOW);
  digitalWrite(g_pin, g_on ? HIGH : LOW);
  digitalWrite(b_pin, b_on ? HIGH : LOW);
}

static inline void srPulseClock() {
  digitalWrite(SR_SHCP, HIGH);
  // short rise time
  digitalWrite(SR_SHCP, LOW);
}

// Map digit -> active-low segment bits for segments A..G (1..7segment with A..G).
// For a typical 7-seg common cathode/anode? Here the circuit uses shift register driving segments directly
// and the frozen contract states active_low=true for all segments.
// Therefore: output bit = 0 means segment ON (active-low).
// Standard mapping (abcdefg), active-low for 0..9:
static uint8_t digitSegmentsActiveLow_AtoG(uint8_t d) {
  // Bits order for shift-register Q0..Q6 mapped to sevseg segments A..G.
  // We'll create a 7-bit value where bit0 corresponds to Q0->segment A, ..., bit6->Q6->segment G.
  // active-low encoding:
  // 0: a b c d e f on, g off => a..f=0, g=1 => 0b1111110? careful with bit positions.
  // We'll provide as [g..a]? Let's compute directly with explicit active states.
  // We want: bit=0 for ON, bit=1 for OFF.
  // Segment ON set for digit:
  // 0: A,B,C,D,E,F on; G off
  // 1: B,C on
  // 2: A,B,D,E,G on
  // 3: A,B,C,D,G on
  // 4: B,C,F,G on
  // 5: A,C,D,F,G on
  // 6: A,C,D,E,F,G on
  // 7: A,B,C on
  // 8: A,B,C,D,E,F,G on
  // 9: A,B,C,D,F,G on
  bool A = false, B = false, C = false, D = false, E = false, F = false, G = false;
  switch (d) {
    case 0: A=B=C=D=E=F=true; G=false; break;
    case 1: A=false; B=C=true; D=E=F=false; G=false; break;
    case 2: A=B=true; C=false; D=true; E=true; F=false; G=true; break;
    case 3: A=B=C=D=true; E=false; F=false; G=true; break;
    case 4: A=false; B=C=true; D=false; E=false; F=true; G=true; break;
    case 5: A=true; B=false; C=true; D=true; E=false; F=true; G=true; break;
    case 6: A=true; B=false; C=true; D=true; E=true; F=true; G=true; break;
    case 7: A=true; B=true; C=true; D=false; E=false; F=false; G=false; break;
    case 8: A=B=C=D=E=F=G=true; break;
    case 9: A=true; B=true; C=true; D=true; E=false; F=true; G=true; break;
    default: A=false; B=false; C=false; D=false; E=false; F=false; G=false; break;
  }

  // Convert to active-low bits for A..G
  // bit0 -> A, bit1 -> B, ... bit6 -> G
  uint8_t bits = 0;
  bits |= (A ? 0 : 1) << 0;
  bits |= (B ? 0 : 1) << 1;
  bits |= (C ? 0 : 1) << 2;
  bits |= (D ? 0 : 1) << 3;
  bits |= (E ? 0 : 1) << 4;
  bits |= (F ? 0 : 1) << 5;
  bits |= (G ? 0 : 1) << 6;
  return bits;
}

static void sevsegWriteDigit0to9(uint8_t digit) {
  // Frozen contract: one shift595_number output drives segments Q0..Q6 mapped to A..G.
  // Active_low=true for segments. Invalid_policy="blank" but we only provide 0..9 (ones digit for count>9).
  uint8_t segBits_AtoG = digitSegmentsActiveLow_AtoG(digit);

  // shift-register has 8 outputs Q0..Q7; only Q0..Q6 used. Set Q7 to 1 (inactive) for safety.
  // We'll shift out MSB first? Depends on hardware; for 74HC595 typical expectation is shift out MSB first.
  // However, since mapping is Q0..Q6, many sketches use MSB-first with bit7 first => Q7 gets first shifted bit.
  // We'll explicitly pack so that after shifting 8 bits with MSB-first, Q0 corresponds to LSB.
  // We'll choose byte where bit0 -> Q0, ..., bit6 -> Q6.
  uint8_t dataByte = (segBits_AtoG & 0x7F) | (1u << 7); // Q7 inactive

  // Latch low-to-high at STCP after shifting.
  digitalWrite(SR_STCP, LOW);

  // Shift out MSB first
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_DS, (dataByte >> i) & 0x1 ? HIGH : LOW);
    srPulseClock();
  }

  // Latch
  digitalWrite(SR_STCP, HIGH);
  // small hold
  digitalWrite(SR_STCP, LOW);
}

static void updateOutputsFromModel() {
  // RGB outputs (io_3..io_8 are Real but represent 0.0/1.0)
  // io_3: RGB1 red intensity: count < 5 ? 1 : 0
  // io_4: RGB1 green intensity: 5..9 =>1; 10..15 =>1; else 0
  // io_5: RGB1 blue intensity: count <5 =>1; 10..15 =>1; else 0
  // io_6: RGB2 red intensity: count>=5 && count<=9 =>1 ; else 0
  // io_7: RGB2 green intensity: count>=5 && count<=9 =>1 ; 10..15 =>1 else 0
  // io_8: RGB2 blue intensity: count <5 =>1 ; 10..15 =>1 else 0
  bool c_lt_5 = (count_ < 5);
  bool c_5_9 = (count_ >= 5 && count_ <= 9);
  bool c_10_15 = (count_ >= 10 && count_ <= 15);

  // io_3..io_5 for rgb1
  bool io_3 = c_lt_5 ? true : false;
  bool io_4 = (c_5_9 ? true : (c_10_15 ? true : false));
  bool io_5 = (c_lt_5 ? true : (c_10_15 ? true : false));

  // io_6..io_8 for rgb2
  bool io_6 = (c_5_9 ? true : (false));
  bool io_7 = (c_lt_5 ? false : (c_5_9 ? true : (c_10_15 ? true : false)));
  bool io_8 = (c_lt_5 ? true : (c_5_9 ? false : (c_10_15 ? true : false)));

  writeRGB(io_3, io_4, io_5, RGB1_R, RGB1_G, RGB1_B);
  writeRGB(io_6, io_7, io_8, RGB2_R, RGB2_G, RGB2_B);

  // io_9: 7-seg count display (if count > 15 then 0 else count)
  int32_t io_9 = (count_ > 15) ? 0 : count_;
  // Frozen contract: model handles only ones digit for counts 10..15 and invalid_policy blank otherwise.
  // We'll display io_9 % 10 but only for 0..15 (since io_9 itself is 0..15).
  uint8_t digit = (uint8_t)(io_9 % 10);
  sevsegWriteDigit0to9(digit);
}

// -------------------- Arduino lifecycle --------------------
void setup() {
  // Inputs with pullups (active-low in model_semantics: true_level=LOW)
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // RGB pins
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // Shift register pins
  pinMode(SR_SHCP, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_DS, OUTPUT);

  digitalWrite(SR_SHCP, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_DS, LOW);

  // Initial outputs per model initial state:
  // count=0, rgb outputs all 0 (per frozen outputs), io_9 = 0.
  count_ = 0;
  prevBtn0_ = prevBtn1_ = prevBtn2_ = false;
  debounceStable0_ = debounceStable1_ = debounceStable2_ = false;
  debounceCand0_ = debounceCand1_ = debounceCand2_ = false;
  debounceCnt0_ = debounceCnt1_ = debounceCnt2_ = 0;

  // Force initial RGB off (model for count<5 says rgb1 red=1, rgb2 blue=1, etc),
  // but the prose says both RGB LEDs off at initialization.
  // Frozen contract states outputs as above; implement contract exactly: it would turn some on for count=0.
  // The accepted model explicitly gives those outputs; so we follow the frozen output equations.
  updateOutputsFromModel();
}

void loop() {
  uint32_t start = millis();

  // Read inputs and convert to io_0/io_1/io_2 booleans as per model_semantics:
  // true_level = LOW (active-low). Therefore: io_i = (digitalRead == LOW)
  bool io_0 = (digitalRead(BTN1_PIN) == LOW);
  bool io_1 = (digitalRead(BTN2_PIN) == LOW);
  bool io_2 = (digitalRead(BTN3_PIN) == LOW);

  // -------------------- Frozen controller tick (ordered updates) --------------------
  // io_0 debouncing
  if (io_0) {
    if (debounceCand0_) {
      // if (debounceStable0 == true) then done else nothing
      if (debounceStable0_ == true) {
        // done
      }
    } else {
      debounceCand0_ = true;
      debounceCnt0_ = 0;
    }
  } else {
    if (!debounceCand0_) {
      if (debounceStable0_ == true) {
        // done
      }
    } else {
      debounceCand0_ = false;
      debounceCnt0_ = 0;
    }
  }

  if (debounceCand0_) {
    if (debounceStable0_ == false) {
      debounceCnt0_ = debounceCnt0_ + 1;
    } else {
      // nothing
    }
  } else {
    if (debounceStable0_ == true) {
      debounceCnt0_ = debounceCnt0_ + 1;
    } else {
      // nothing
    }
  }

  if (debounceCnt0_ >= 2) {
    debounceStable0_ = debounceCand0_;
    debounceCnt0_ = 0;
  }

  // io_1 debouncing
  if (io_1) {
    if (debounceCand1_) {
      // done
    } else {
      debounceCand1_ = true;
      debounceCnt1_ = 0;
    }
  } else {
    if (!debounceCand1_) {
      // done
    } else {
      debounceCand1_ = false;
      debounceCnt1_ = 0;
    }
  }

  if (debounceCand1_) {
    if (debounceStable1_ == false) {
      debounceCnt1_ = debounceCnt1_ + 1;
    } else {
      // nothing
    }
  } else {
    if (debounceStable1_ == true) {
      debounceCnt1_ = debounceCnt1_ + 1;
    } else {
      // nothing
    }
  }

  if (debounceCnt1_ >= 2) {
    debounceStable1_ = debounceCand1_;
    debounceCnt1_ = 0;
  }

  // io_2 debouncing
  if (io_2) {
    if (debounceCand2_) {
      // done
    } else {
      debounceCand2_ = true;
      debounceCnt2_ = 0;
    }
  } else {
    if (!debounceCand2_) {
      // done
    } else {
      debounceCand2_ = false;
      debounceCnt2_ = 0;
    }
  }

  if (debounceCand2_) {
    if (debounceStable2_ == false) {
      debounceCnt2_ = debounceCnt2_ + 1;
    } else {
      // nothing
    }
  } else {
    if (debounceStable2_ == true) {
      debounceCnt2_ = debounceCnt2_ + 1;
    } else {
      // nothing
    }
  }

  if (debounceCnt2_ >= 2) {
    debounceStable2_ = debounceCand2_;
    debounceCnt2_ = 0;
  }

  // Rising edges on debounced stable values against prev
  if (debounceStable0_ && (!prevBtn0_)) {
    if (count_ > 15) {
      count_ = 0;
    }
    count_ = ((count_ + 1 > 15) ? 0 : count_ + 1);
  }

  if (debounceStable1_ && (!prevBtn1_)) {
    if (count_ > 15) {
      count_ = 0;
    }
    count_ = ((count_ + 1 > 15) ? 0 : count_ + 1);
  }

  if (debounceStable2_ && (!prevBtn2_)) {
    if (count_ > 15) {
      count_ = 0;
    }
    count_ = ((count_ + 1 > 15) ? 0 : count_ + 1);
  }

  prevBtn0_ = debounceStable0_;
  prevBtn1_ = debounceStable1_;
  prevBtn2_ = debounceStable2_;

  // Update outputs immediately after tick
  updateOutputsFromModel();

  // Wait remaining time to enforce 5ms period (best-effort)
  uint32_t elapsed = millis() - start;
  if (elapsed < CONTROL_PERIOD_MS) {
    delay(CONTROL_PERIOD_MS - elapsed);
  }
}
