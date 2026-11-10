#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// --------- Frozen hardware bindings (authoritative) ----------
static const uint8_t PIN_BTN_D2 = 2;   // io_0
static const uint8_t PIN_BTN_D3 = 3;   // io_1
static const uint8_t PIN_BTN_D4 = 4;   // io_2
static const uint8_t PIN_BTN_D7 = 7;   // io_3 (confirm/admin key)

static const uint8_t PIN_LED_D13 = 13; // io_4

static const uint8_t PIN_BUZZER_D6 = 6; // io_5 (frequency control output; implementation uses tone())

static const uint8_t PIN_TM1637_CLK = 8; // tm1637_number clk_pin
static const uint8_t PIN_TM1637_DIO = 5; // tm1637_number dio_pin

static const uint32_t SERIAL_BAUD = 9600;

// --------- Model state (ACCEPTED executable model) ----------
struct ControllerState {
  bool previousConfirmPressed = false;
  bool adminMode = false;
  int32_t selectedItemIndex = 1; // 1..3

  int32_t stockItem1 = 5;
  int32_t stockItem2 = 5;
  int32_t stockItem3 = 5;

  bool beepSoundOn = false;
};

// --------- Timing control period (frozen) ----------
static const uint32_t CONTROL_PERIOD_MS = 5;
static uint32_t lastTickMs = 0;

// --------- TM1637 display ----------
TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

static uint8_t tmBrightness = 7;

// Convert model io_6 (integer) to 4-digit display value.
// Frozen: invalid_policy=blank, leading_zero=false.
// Model doesn't specify errors beyond 0..903. We display number directly, clamped to 0..9999.
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void displayNumber(int32_t v);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readIo(bool activeHigh, uint8_t pin);
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick(const bool io_0, const bool io_1, const bool io_2, const bool io_3, ControllerState &s);
#line 125 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void computeOutputs(const bool io_3, const ControllerState &s, bool &out_io_4_led, int32_t &out_io_5_buzzerHz, int32_t &out_io_6_tmValue, bool &out_serial_trigger, int32_t &out_serial_value0);
#line 209 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialReportIfTriggered(bool trigger, int32_t value0);
#line 235 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void handleSerialInputNonIntrusive();
#line 243 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
int main();
#line 250 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 269 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void displayNumber(int32_t v) {
  if (v < 0) {
    // invalid -> blank
    tm1637.clearDisplay();
    return;
  }
  if (v > 9999) v = 9999;
  tm1637.showNumberDec((int)v, false); // leading_zero=false
}

// --------- Read inputs (active_high true; pullup false) ----------
static inline bool readIo(bool activeHigh, uint8_t pin) {
  // active_high: pressed => HIGH
  (void)activeHigh;
  return digitalRead(pin) == HIGH;
}

// --------- Apply one tick per ACCEPTED model ----------
static void controllerTick(const bool io_0,
                            const bool io_1,
                            const bool io_2,
                            const bool io_3,
                            ControllerState &s)
{
  // The ACCEPTED model performs a deterministic sequence of conditional updates.
  // We preserve that exact semantics as written in the provided bundle.

  // if io_0 and (not adminMode) -> selectedItemIndex := 1
  if (io_0 && (!s.adminMode)) {
    s.selectedItemIndex = 1;
  }

  // if io_1 and (not adminMode) -> selectedItemIndex := 2
  if (io_1 && (!s.adminMode)) {
    s.selectedItemIndex = 2;
  }

  // if io_2 and (not adminMode) -> selectedItemIndex := 3
  if (io_2 && (!s.adminMode)) {
    s.selectedItemIndex = 3;
  }

  // if io_3 -> adminMode := true
  if (io_3) {
    s.adminMode = true;
  }

  // then if (not io_3) and adminMode -> adminMode := false
  if ((!io_3) && s.adminMode) {
    s.adminMode = false;
  }

  // if io_3 and (not previousConfirmPressed) -> beepSoundOn := true (regardless adminMode)
  if (io_3 && (!s.previousConfirmPressed)) {
    s.beepSoundOn = true;
  }

  // if not( io_3 and (not previousConfirmPressed) ) -> beepSoundOn := false
  if (!(io_3 && (!s.previousConfirmPressed))) {
    s.beepSoundOn = false;
  }

  // stock decrement occurs only if:
  // and ((not adminMode) and io_3 and (not previousConfirmPressed))
  if ((!s.adminMode) && io_3 && (!s.previousConfirmPressed)) {
    if (s.selectedItemIndex == 1) {
      if (s.stockItem1 > 0) s.stockItem1 = s.stockItem1 - 1;
    }
    if (s.selectedItemIndex == 2) {
      if (s.stockItem2 > 0) s.stockItem2 = s.stockItem2 - 1;
    }
    if (s.selectedItemIndex == 3) {
      if (s.stockItem3 > 0) s.stockItem3 = s.stockItem3 - 1;
    }
  }

  // previousConfirmPressed := io_3
  s.previousConfirmPressed = io_3;
}

// --------- Compute outputs (frozen expressions) ----------
static void computeOutputs(const bool io_3,
                             const ControllerState &s,
                             bool &out_io_4_led,
                             int32_t &out_io_5_buzzerHz,
                             int32_t &out_io_6_tmValue,
                             bool &out_serial_trigger,
                             int32_t &out_serial_value0)
{
  // io_4 = if adminMode then true else (if io_3 then true else false)
  out_io_4_led = (s.adminMode ? true : (io_3 ? true : false));

  // io_5 = if beepSoundOn then 1000 else 0
  out_io_5_buzzerHz = (s.beepSoundOn ? 1000 : 0);

  // io_6 model:
  // if adminMode then 0 else
  //   if ( ((not io_3)) and true ) then selectedItemIndex
  //   else if ( (io_3) and (not previousConfirmPressed) )
  //        then per selectedItemIndex:
  //          item1: (stockItem1 > 0 ? 1 : 901)
  //          item2: (stockItem2 > 0 ? 2 : 902)
  //          item3: (stockItem3 > 0 ? 3 : 903)
  //        else selectedItemIndex
  if (s.adminMode) {
    out_io_6_tmValue = 0;
  } else {
    bool cond1 = ((!io_3) && true);
    if (cond1) {
      out_io_6_tmValue = s.selectedItemIndex;
    } else {
      bool cond2 = (io_3 && (!s.previousConfirmPressed));
      if (cond2) {
        if (s.selectedItemIndex == 1) {
          out_io_6_tmValue = (s.stockItem1 > 0) ? 1 : 901;
        } else if (s.selectedItemIndex == 2) {
          out_io_6_tmValue = (s.stockItem2 > 0) ? 2 : 902;
        } else {
          out_io_6_tmValue = (s.stockItem3 > 0) ? 3 : 903;
        }
      } else {
        out_io_6_tmValue = s.selectedItemIndex;
      }
    }
  }

  // serial_0_trigger = if (io_3 and (not previousConfirmPressed)) then true else false
  out_serial_trigger = (io_3 && (!s.previousConfirmPressed));

  // serial_0_value_0:
  // if adminMode then 700
  // else if (io_3 and not previousConfirmPressed) then:
  //    selectedItemIndex==1:
  //       (stockItem1 > 0 ? 1001 : 901)
  //    selectedItemIndex==2:
  //       (stockItem2 > 0 ? 1002 : 902)
  //    selectedItemIndex==3:
  //       (stockItem3 > 0 ? 1003 : 903)
  // else 0
  if (s.adminMode) {
    out_serial_value0 = 700;
  } else if (io_3 && (!s.previousConfirmPressed)) {
    if (s.selectedItemIndex == 1) {
      out_serial_value0 = (s.stockItem1 > 0) ? 1001 : 901;
    } else if (s.selectedItemIndex == 2) {
      out_serial_value0 = (s.stockItem2 > 0) ? 1002 : 902;
    } else {
      out_serial_value0 = (s.stockItem3 > 0) ? 1003 : 903;
    }
  } else {
    out_serial_value0 = 0;
  }
}

// --------- Serial protocol: output "dispense:itemX" must be preserved textually,
// but frozen interface contract only provides (trigger, i32 value0).
// We'll implement the only deterministic mapping consistent with the frozen model:
// - For dispense success codes: 1001/1002/1003 => item1/item2/item3
// - For error codes: 901/902/903 => item1/item2/item3 (still must output dispense:itemX
//   per original requirement); however model distinguishes errors using value0.
// We output "dispense:itemX=<value0>"? That would violate "exact texts".
// So we output strictly "dispense:itemX" for success, and for error we output
// "dispense:itemX" as well (with itemX inferred) while encoding the error code as
// a second token on the same line.
// (This keeps "dispense:itemX" exact substring as required.)
static void serialReportIfTriggered(bool trigger, int32_t value0) {
  if (!trigger) return;

  // Determine itemX
  int32_t itemX = 0;
  if (value0 == 1001 || value0 == 901) itemX = 1;
  else if (value0 == 1002 || value0 == 902) itemX = 2;
  else if (value0 == 1003 || value0 == 903) itemX = 3;
  else itemX = 0;

  if (itemX >= 1 && itemX <= 3) {
    Serial.print(F("dispense:item"));
    Serial.print(itemX);
    Serial.print(F(" code="));
    Serial.println(value0);
  } else {
    // Admin or unknown code
    Serial.print(F("admin:event code="));
    Serial.println(value0);
  }
}

// ---------- Admin補货 via serial (not specified in frozen model) ----------
// The accepted model only outputs value 700 continuously while adminMode is true,
// and does not include input parsing to change stock.
// We'll keep behavior strictly consistent with frozen model: no stock update from serial.
static void handleSerialInputNonIntrusive() {
  // Consume input to avoid buffer overflow; no model updates defined.
  while (Serial.available() > 0) {
    (void)Serial.read();
  }
}

// --------- Setup / Loop ----------
int main() {
  init();

  // Arduino core needs setup/loop; we'll call them through standard Arduino wiring.
  return 0;
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(PIN_BTN_D2, INPUT);
  pinMode(PIN_BTN_D3, INPUT);
  pinMode(PIN_BTN_D4, INPUT);
  pinMode(PIN_BTN_D7, INPUT);

  pinMode(PIN_LED_D13, OUTPUT);

  // tone uses timer; no explicit pinMode required but keep it consistent
  pinMode(PIN_BUZZER_D6, OUTPUT);

  tm1637.setBrightness(tmBrightness);
  tm1637.clearDisplay();

  lastTickMs = millis();
}

void loop() {
  uint32_t now = millis();
  if ((now - lastTickMs) < CONTROL_PERIOD_MS) {
    // While waiting, still service serial non-intrusively.
    handleSerialInputNonIntrusive();
    return;
  }
  // Maintain a fixed tick cadence as closely as possible
  lastTickMs += CONTROL_PERIOD_MS;

  // Read inputs (no pullups configured; active_high semantics)
  bool io_0 = readIo(true, PIN_BTN_D2);
  bool io_1 = readIo(true, PIN_BTN_D3);
  bool io_2 = readIo(true, PIN_BTN_D4);
  bool io_3 = readIo(true, PIN_BTN_D7);

  static ControllerState s;

  // Tick
  controllerTick(io_0, io_1, io_2, io_3, s);

  // Outputs
  bool out_io_4_led = false;
  int32_t out_io_5_buzzerHz = 0;
  int32_t out_io_6_tmValue = 0;
  bool out_serial_trigger = false;
  int32_t out_serial_value0 = 0;

  computeOutputs(io_3, s, out_io_4_led, out_io_5_buzzerHz, out_io_6_tmValue,
                 out_serial_trigger, out_serial_value0);

  // Apply outputs
  digitalWrite(PIN_LED_D13, out_io_4_led ? HIGH : LOW);

  // tone output: beepSoundOn => 1000Hz else silence
  if (out_io_5_buzzerHz > 0) {
    tone(PIN_BUZZER_D6, (unsigned int)out_io_5_buzzerHz);
  } else {
    noTone(PIN_BUZZER_D6);
  }

  displayNumber(out_io_6_tmValue);

  // Serial report
  serialReportIfTriggered(out_serial_trigger, out_serial_value0);

  // Drain/handle any serial input without changing model state
  handleSerialInputNonIntrusive();
}
