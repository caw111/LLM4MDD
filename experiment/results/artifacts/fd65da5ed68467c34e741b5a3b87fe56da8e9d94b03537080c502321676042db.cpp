#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_017\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static uint8_t tm1637_digits[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

// Minimal TM1637 implementation (fixed mapping, no simplification of model logic)
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : clkPin_(clkPin), dioPin_(dioPin) {}

  void begin() {
    pinMode(clkPin_, OUTPUT);
    pinMode(dioPin_, OUTPUT);
    digitalWrite(clkPin_, LOW);
    digitalWrite(dioPin_, HIGH);
    delayMicroseconds(50);
    // Set display to off then set brightness externally when displaying
    writeByte_(0x40); // data command
    writeByte_(0x00);
    displayClear_();
  }

  void setBrightness(uint8_t brightness) {
    brightness &= 0x07; // 0..7
    writeCommand_(0x88 | brightness); // brightness command
  }

  // Display a single number 0..99; here model requires only 0..4
  void displayNumber(uint8_t number, bool leadingZero = false) {
    (void)leadingZero;

    // Prepare data: position 0 = rightmost digit, position 1 = left digit
    uint8_t tens = number / 10;
    uint8_t ones = number % 10;

    uint8_t seg0 = encodeDigit_(ones);
    uint8_t seg1 = encodeDigit_(tens);

    uint8_t data[2] = { seg0, seg1 };

    writeCommand_(0x40); // set data command

    // write digit 0 (rightmost)
    writeData_(data[0]);
    // write digit 1 (leftmost)
    writeData_(data[1]);

    // finalize
    writeCommand_(0x44); // display control (auto mode)
  }

  void clear() {
    displayClear_();
  }

private:
  uint8_t clkPin_, dioPin_;

  void start_() {
    digitalWrite(dioPin_, HIGH);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    digitalWrite(dioPin_, LOW);
    delayMicroseconds(5);
    digitalWrite(clkPin_, LOW);
    delayMicroseconds(5);
  }

  void stop_() {
    digitalWrite(clkPin_, LOW);
    digitalWrite(dioPin_, LOW);
    delayMicroseconds(5);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    digitalWrite(dioPin_, HIGH);
    delayMicroseconds(5);
  }

  void writeByte_(uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
      digitalWrite(clkPin_, LOW);
      delayMicroseconds(2);
      digitalWrite(dioPin_, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(5);
      digitalWrite(clkPin_, HIGH);
      delayMicroseconds(5);
      b >>= 1;
    }
    // ack
    digitalWrite(clkPin_, LOW);
    pinMode(dioPin_, INPUT);
    delayMicroseconds(5);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    pinMode(dioPin_, OUTPUT);
    digitalWrite(dioPin_, HIGH);
    delayMicroseconds(5);
  }

  void writeCommand_(uint8_t cmd) {
    start_();
    writeByte_(cmd & 0xFF);
    stop_();
  }

  void writeData_(uint8_t data) {
    // data command uses start/stop only once; emulate as continuous by toggling ack-less
    // We'll follow standard approach: writeByte_ handles ack by setting input briefly.
    writeByte_(data);
  }

  uint8_t encodeDigit_(uint8_t digit) {
    // Standard TM1637 segment codes for digits 0..9.
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

    if (digit <= 9) return seg[digit];
    return 0x00;
  }

  void displayClear_() {
    // Clear both digits (set data)
    writeCommand_(0x40);
    writeData_(0x00);
    writeData_(0x00);
    writeCommand_(0x44); // display on auto mode
  }
};

// Hardware pin bindings (authoritative)
static const uint8_t PIN_D2 = 2;
static const uint8_t PIN_D3 = 3;
static const uint8_t PIN_D4 = 4; // TM1637 CLK
static const uint8_t PIN_D5 = 5; // TM1637 DIO
static const uint8_t PIN_D6 = 6; // buzzer
static const uint8_t PIN_D7 = 7;
static const uint8_t PIN_D8 = 8;
static const uint8_t PIN_D9 = 9;  // reset long-press
static const uint8_t PIN_D13 = 13; // LED

static const unsigned long CONTROL_PERIOD_MS = 5;
static const unsigned long RESET_HOLD_MS = 3000;

TM1637 tm(PIN_D4, PIN_D5);

// Controller state per Accepted executable model
static bool previousPressed1 = false;
static bool previousPressed2 = false;
static bool previousPressed3 = false;
static bool previousPressed4 = false;
static bool locked = false;
static int winnerId = 0;
static long resetAccumulatedMs = 0;
static bool resetting = false;
static bool resetRequest = false;
static bool serialTriggerNext = false;
static int serialTimestampMsNext = 0;

// Model inputs (read each cycle)
static bool io_0 = false; // D2 (player1)
static bool io_1 = false; // D3 (player2)
static bool io_2 = false; // D7 (player3)
static bool io_3 = false; // D8 (player4)
static bool io_4 = false; // D9 reset button

// Model outputs
static bool io_5 = false; // locked -> D13
static int io_6 = 0;      // buzzer frequency placeholder fixed to 0 by model
static int io_7 = 0;      // winnerId -> TM + serial
static bool serial_0_trigger = false;
static int serial_0_value_0 = 0; // timestamp
static int serial_0_value_1 = 0; // winnerId

// Serial: 9600 baud, line_or_idle (use newline framing)
#line 184 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_017\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void sendSerialReport(int timestampMs, int winner);
#line 191 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_017\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs_AndSideEffects();
#line 211 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_017\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 232 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_017\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 184 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_017\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void sendSerialReport(int timestampMs, int winner) {
  Serial.print(timestampMs);
  Serial.print(',');
  Serial.println(winner);
}

// Small debounce edge detector is part of model semantics via previousPressed*
static void applyOutputs_AndSideEffects() {
  digitalWrite(PIN_D13, io_5 ? HIGH : LOW);

  // TM1637 displays io_7 (winnerId). When winnerId == 0 (after reset), show 0.
  tm.setBrightness(7);
  tm.displayNumber((uint8_t)io_7, false);

  // serial_0_trigger is defined as false in model outputs; however model assigns serialTriggerNext true
  // which is then forced false in updates (then assign serialTriggerNext := false). So we preserve exact output:
  serial_0_trigger = false;

  // Beepr: model outputs io_6=0 and never triggers buzzer in model.
  // We preserve model outputs exactly (no additional behavior).
  (void)io_6;

  // serial values per model are set, but trigger remains false; no report.
  (void)serial_0_value_0;
  (void)serial_0_value_1;
}

void setup() {
  pinMode(PIN_D2, INPUT);  // active_high per contract, default no pullup
  pinMode(PIN_D3, INPUT);
  pinMode(PIN_D7, INPUT);
  pinMode(PIN_D8, INPUT);
  pinMode(PIN_D9, INPUT);

  pinMode(PIN_D13, OUTPUT);
  pinMode(PIN_D6, OUTPUT);

  Serial.begin(9600);

  tm.begin();
  // initial show 0
  tm.setBrightness(7);
  tm.displayNumber(0, false);

  digitalWrite(PIN_D13, LOW);
  digitalWrite(PIN_D6, LOW);
}

void loop() {
  static unsigned long lastTick = 0;
  unsigned long now = millis();
  if (now - lastTick < CONTROL_PERIOD_MS) return;
  lastTick += CONTROL_PERIOD_MS;

  // Read inputs
  io_0 = (digitalRead(PIN_D2) == HIGH);
  io_1 = (digitalRead(PIN_D3) == HIGH);
  io_2 = (digitalRead(PIN_D7) == HIGH);
  io_3 = (digitalRead(PIN_D8) == HIGH);
  io_4 = (digitalRead(PIN_D9) == HIGH);

  // ACCEPTED model update sequence (preserve order and assignments)
  // if io_4 then resetting=true; resetAccumulatedMs=0
  if (io_4) {
    resetting = true;
    resetAccumulatedMs = 0;
  }

  // if not io_4 then resetting=false; resetAccumulatedMs=0
  if (!io_4) {
    resetting = false;
    resetAccumulatedMs = 0;
  }

  // if io_4 and (resetAccumulatedMs >= 3000) then resetRequest=true else resetRequest=false
  if (io_4 && (resetAccumulatedMs >= (long)RESET_HOLD_MS)) {
    resetRequest = true;
  } else {
    resetRequest = false;
  }

  // if resetRequest then locked=false; winnerId=0; serialTriggerNext=false
  if (resetRequest) {
    locked = false;
    winnerId = 0;
    serialTriggerNext = false;
  }

  // if not resetRequest then handle winner capture logic; else branch only affects serialTriggerNext
  if (!resetRequest) {
    if (!locked) {
      if (io_0 && !previousPressed1) {
        locked = true;
        winnerId = 1;
        serialTriggerNext = true;
        serialTimestampMsNext = 0;
      }
      if (io_1 && !previousPressed2) {
        locked = true;
        winnerId = 2;
        serialTriggerNext = true;
        serialTimestampMsNext = 0;
      }
      if (io_2 && !previousPressed3) {
        locked = true;
        winnerId = 3;
        serialTriggerNext = true;
        serialTimestampMsNext = 0;
      }
      if (io_3 && !previousPressed4) {
        locked = true;
        winnerId = 4;
        serialTriggerNext = true;
        serialTimestampMsNext = 0;
      }
    } else {
      serialTriggerNext = false;
    }
  } else {
    // In model, this else corresponds to "if resetRequest then ... else ..." and then
    // inner else in that structured sequence sets only serialTriggerNext=false.
    // Already handled above; keep exact behavior by ensuring it is false here too.
    serialTriggerNext = false;
  }

  // then assign serialTriggerNext := false
  serialTriggerNext = false;

  // then assign previousPressed* := io_*
  previousPressed1 = io_0;
  previousPressed2 = io_1;
  previousPressed3 = io_2;
  previousPressed4 = io_3;

  // outputs mapping (exact)
  io_5 = locked;
  io_6 = 0;
  io_7 = winnerId;
  serial_0_trigger = false;
  serial_0_value_0 = 0;
  serial_0_value_1 = winnerId;

  // Apply outputs; preserve model-prescribed side effects (none for buzzer/serial trigger).
  applyOutputs_AndSideEffects();

  // NOTE: Even though the textual requirement mentions buzzer + serial reporting on valid event,
  // the Accepted executable model explicitly sets serial_0_trigger=false and io_6=0 and contains
  // no elapsed-time accumulation. To preserve the provided authoritative model semantics,
  // we do not emit serial reports and do not drive the buzzer.
}
