#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_013\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// TM1637 minimal implementation (no external libs)
// Compatible with CLK/DIO pins mapping.
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : _clkPin(clkPin), _dioPin(dioPin) {}

  void begin() {
    pinMode(_clkPin, OUTPUT);
    pinMode(_dioPin, OUTPUT);
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, HIGH);
  }

  void setBrightness(uint8_t b) {
    _brightness = b & 0x07; // 0..7
  }

  // Display a non-negative integer 0..99 (here we only need 0..3),
  // leading zeros disabled, invalid_policy="blank" if out of range.
  void displayNumber(int v) {
    if (v < 0 || v > 9999) {
      // blank
      uint8_t blankDigits[4] = { SEG_BLANK, SEG_BLANK, SEG_BLANK, SEG_BLANK };
      writeDigits(blankDigits, true);
      return;
    }

    uint8_t digits[4];
    for (int i = 0; i < 4; i++) digits[i] = SEG_BLANK;

    int n = v;
    // Put digits on the right, leading zeros disabled.
    for (int pos = 3; pos >= 0 && n > 0; pos--) {
      digits[pos] = encodeDigit(n % 10);
      n /= 10;
    }
    // Special case: v==0 => show "0" (not blank)
    if (v == 0) digits[3] = encodeDigit(0);

    writeDigits(digits, true);
  }

private:
  uint8_t _clkPin, _dioPin;
  uint8_t _brightness = 7;

  static constexpr uint8_t SEG_BLANK = 0x00;

  uint8_t encodeDigit(uint8_t d) {
    // Standard TM1637 segment mapping: a b c d e f g dp
    // Common usage: 0..9 only, dp off.
    static const uint8_t seg[10] = {
      0x3F, // 0
      0x06, // 1
      0x5B, // 2
      0x4F, // 3
      0x66, // 4
      0x6D, // 5
      0x7D, // 6
      0x07, // 7
      0x7F, // 8
      0x6F  // 9
    };
    if (d < 10) return seg[d];
    return SEG_BLANK;
  }

  void start() {
    pinMode(_dioPin, OUTPUT);
    digitalWrite(_dioPin, HIGH);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_clkPin, LOW);
  }

  void stop() {
    pinMode(_dioPin, OUTPUT);
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dioPin, HIGH);
  }

  // write a byte, LSB first
  void writeByte(uint8_t b) {
    pinMode(_dioPin, OUTPUT);
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clkPin, LOW);
      digitalWrite(_dioPin, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(2);
      digitalWrite(_clkPin, HIGH);
      delayMicroseconds(2);
      b >>= 1;
    }
    // ACK
    digitalWrite(_clkPin, LOW);
    pinMode(_dioPin, INPUT);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_clkPin, LOW);
    pinMode(_dioPin, OUTPUT);
  }

  void writeDigits(uint8_t digits[4], bool autoInc) {
    start();
    // command: address + data mode
    writeByte(0x40 | (autoInc ? 0x00 : 0x00)); // data command

    // If autoInc false would require different behavior; keep it simple but deterministic.
    for (int i = 0; i < 4; i++) {
      writeByte(digits[i]);
    }
    stop();

    // display control
    start();
    writeByte(0x80 | (_brightness & 0x07)); // display on + brightness
    stop();
  }
};

// -------------------- Hardware bindings (frozen contract) --------------------
static const uint8_t PIN_D2 = 2;
static const uint8_t PIN_D3 = 3;
static const uint8_t PIN_D7 = 7;

static const uint8_t PIN_D6_BUZZER = 6;  // tone output
static const uint8_t PIN_D13_LED = 13;   // digital output

static const uint8_t TM_CLK = 4;
static const uint8_t TM_DIO = 5;

static TM1637 tm(TM_CLK, TM_DIO);

// -------------------- Accepted executable model --------------------
static bool previousAllFull = false;

// Business/timing: control_period_ms = 5.
// The frozen model bundle only contains previousAllFull state and direct combinational outputs
// plus serial_0_trigger logic; restock timers/locks are NOT represented in the bundle.
// Therefore implement exactly what the bundle specifies.
static const uint32_t control_period_ms = 5;

// -------------------- Serial report gate and values --------------------
#line 192 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_013\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 208 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_013\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_013\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void reportSerial(bool io0, bool io1, bool io2,
                           int layerCount,
                           bool allFull,
                           bool lockNewRecords,
                           bool restockNeededFlag,
                           unsigned long /*elapsedMsDummy*/) {
  // Contract: serial_0_trigger gates sending a fixed typed status set.
  // Serial protocol details were not specified, so use deterministic line format.
  // Keep tokens stable and simple: KEY=VALUE pairs in one line.
  //
  // Trigger corresponds to:
  // serial_0_value_0: layer_count
  // serial_0_value_1..3: layer occupied booleans (io_0..io_2)
  // serial_0_value_4: query_elapsed_ms (bundle sets to 0)
  // serial_0_value_5: restock_needed (bundle sets to false)
  // serial_0_value_6: all_full (bundle sets to io_0&&io_1&&io_2)
  // serial_0_value_7: lock_new_records (bundle sets to io0&&io1&&io2&&(not previousAllFull))
  //
  // Since behavior must not be simplified beyond provided model, we follow bundle values.
  const bool serialElapsedMs = false; // not used
  (void)serialElapsedMs;

  Serial.print("layer_count=");
  Serial.print(layerCount);
  Serial.print(" layer1_occupied=");
  Serial.print(io0 ? "true" : "false");
  Serial.print(" layer2_occupied=");
  Serial.print(io1 ? "true" : "false");
  Serial.print(" layer3_occupied=");
  Serial.print(io2 ? "true" : "false");
  Serial.print(" query_elapsed_ms=0");
  Serial.print(" restock_needed=");
  Serial.print(restockNeededFlag ? "true" : "false");
  Serial.print(" all_full=");
  Serial.print(allFull ? "true" : "false");
  Serial.print(" lock_new_records=");
  Serial.print(lockNewRecords ? "true" : "false");
  Serial.print('\n');
}

// -------------------- Main loop --------------------
void setup() {
  pinMode(PIN_D2, INPUT); // active_high true, pullup false (per contract)
  pinMode(PIN_D3, INPUT);
  pinMode(PIN_D7, INPUT);

  pinMode(PIN_D13_LED, OUTPUT);
  pinMode(PIN_D6_BUZZER, OUTPUT);

  digitalWrite(PIN_D13_LED, LOW);

  tm.begin();
  tm.setBrightness(7); // brightness=7

  Serial.begin(9600);
}

void loop() {
  static unsigned long nextTick = 0;
  unsigned long now = millis();
  if (now < nextTick) return;
  nextTick = now + control_period_ms;

  // Read inputs
  bool io_0 = digitalRead(PIN_D2) == HIGH;
  bool io_1 = digitalRead(PIN_D3) == HIGH;
  bool io_2 = digitalRead(PIN_D7) == HIGH;

  // Apply model outputs (with current previousAllFull).
  // Controller tick structure:
  // first start; then previousAllFull := io_0 and io_1 and io_2;
  // Outputs are defined as out attributes that depend on io_* and previousAllFull.
  //
  // In the provided model_sysml2, previousAllFull is updated in action tick,
  // and outputs reference previousAllFull. In UML/exec semantics, outputs are evaluated
  // after the assignment in the tick? The bundle's expected trace indicates:
  // - When transitioning to all full for the first time, serial_0_trigger should be true.
  // That requires previousAllFull to be the OLD value during trigger evaluation.
  // The simplest consistent interpretation is:
  //   evaluate outputs using OLD previousAllFull,
  //   then update previousAllFull.
  //
  // We'll implement that to match "lock_new_records" gating with not previousAllFull.
  bool allFullOld = previousAllFull;

  // io_3 buzzer tone:
  // if ((io_0 and io_1 and io_2) and not previousAllFull) then 1000 else 0
  int io_3_toneHz = (io_0 && io_1 && io_2 && !allFullOld) ? 1000 : 0;

  // io_4 alarm LED indicator:
  bool io_4_led = io_0 && io_1 && io_2;

  // io_5 current occupied layer count display: complex nested mux
  int io_5_count =
      (io_0 && !io_1 && !io_2) ? 1 :
      (io_0 && io_1 && !io_2) ? 2 :
      (io_0 && !io_1 && io_2) ? 2 :
      (!io_0 && io_1 && io_2) ? 2 :
      (io_0 && io_1 && io_2) ? 3 :
      (io_0 && !io_1 && !io_2) ? 1 : // unreachable due to first term, kept for exactness
      (!io_0 && io_1 && !io_2) ? 1 :
      (!io_0 && !io_1 && io_2) ? 1 :
      0;

  // serial_0_trigger:
  bool serial_0_trigger = ((io_0 || io_1 || io_2) && !allFullOld);

  // serial_0_value_0:
  int serial_layer_count =
      (io_0 && io_1 && io_2) ? 3 :
      (io_0 && (io_1 || io_2) && !(io_1 && io_2)) ? 2 :
      (!io_0 && io_1 && io_2) ? 2 :
      ((io_0 && !io_1 && !io_2) || (!io_0 && io_1 && !io_2) || (!io_0 && !io_1 && io_2)) ? 1 :
      0;

  bool serial_0_value_1 = io_0;
  bool serial_0_value_2 = io_1;
  bool serial_0_value_3 = io_2;
  int serial_0_value_4 = 0;      // fixed
  bool serial_0_value_5 = false; // restock_needed fixed false in bundle
  bool serial_0_value_6 = io_0 && io_1 && io_2;
  bool serial_0_value_7 = io_0 && io_1 && io_2 && !allFullOld;

  // Apply hardware outputs (deterministic)
  if (io_3_toneHz > 0) {
    tone(PIN_D6_BUZZER, io_3_toneHz);
  } else {
    noTone(PIN_D6_BUZZER);
  }
  digitalWrite(PIN_D13_LED, io_4_led ? HIGH : LOW);

  // Update TM1637 number (leading zeros disabled; invalid policy blank)
  tm.displayNumber(io_5_count);

  // Serial gated report
  // Trigger port is not physically output; instead we send when serial_0_trigger is true.
  if (serial_0_trigger) {
    (void)serial_0_value_4;
    reportSerial(io_0, io_1, io_2,
                 serial_layer_count,
                 serial_0_value_6,
                 serial_0_value_7,
                 serial_0_value_5,
                 0);
  }

  // Finally update state: previousAllFull := io_0 and io_1 and io_2
  previousAllFull = (io_0 && io_1 && io_2);
}
