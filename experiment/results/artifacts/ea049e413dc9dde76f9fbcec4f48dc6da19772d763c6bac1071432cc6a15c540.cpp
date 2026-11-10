#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// ---------------- TM1637 (minimal implementation) ----------------
class TM1637Display {
public:
  TM1637Display(uint8_t clk, uint8_t dio) : _clk(clk), _dio(dio) {}
  void setBrightness(uint8_t br) {
    br &= 0x07;
    _brightness = br;
  }
  void begin() {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
  }
  // display integer (0..9999) without leading zeros (blank if 0 and blank policy desired)
  void showNumber(int num, bool leadingZero = false) {
    // TM1637 uses segments; map digits
    uint8_t seg[4] = {0, 0, 0, 0};

    if (num < 0) num = 0;
    int n = num;

    int d0 = n % 10; n /= 10;
    int d1 = n % 10; n /= 10;
    int d2 = n % 10; n /= 10;
    int d3 = n % 10;

    seg[0] = segMap(d0);
    seg[1] = segMap(d1);
    seg[2] = segMap(d2);
    seg[3] = segMap(d3);

    if (!leadingZero) {
      // blank leading zeros
      bool any = false;
      for (int i = 3; i >= 0; --i) {
        uint8_t digitSeg = seg[i];
        int digit = digitFromSeg(digitSeg);
        if (digit == 0 && !any) {
          seg[i] = 0x00; // blank
        } else {
          any = true;
        }
      }
    }

    start();
    writeByte(0x40); // command data
    writeData(seg[0]);
    writeData(seg[1]);
    writeData(seg[2]);
    writeData(seg[3]);
    stop();

    start();
    writeByte(0x88 | (_brightness & 0x07)); // display control
    stop();
  }

private:
  uint8_t _clk, _dio;
  uint8_t _brightness = 7;

  // segment encoding (a,b,c,d,e,f,g,dp) but dp not used
  uint8_t segMap(int digit) {
    static const uint8_t map[10] = {
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
    if (digit < 0 || digit > 9) return 0x00;
    return map[digit];
  }

  int digitFromSeg(uint8_t s) {
    // reverse lookup for 0..9, otherwise -1; used only for leading-zero blanking
    static const uint8_t map[10] = {
      0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F
    };
    for (int i = 0; i < 10; i++) if (map[i] == s) return i;
    return -1;
  }

  void start() {
    digitalWrite(_dio, HIGH);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dio, LOW);
    delayMicroseconds(2);
    digitalWrite(_clk, LOW);
  }
  void stop() {
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, LOW);
    delayMicroseconds(2);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dio, HIGH);
    delayMicroseconds(2);
  }
  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clk, LOW);
      if (b & 0x01) digitalWrite(_dio, HIGH);
      else digitalWrite(_dio, LOW);
      b >>= 1;
      delayMicroseconds(2);
      digitalWrite(_clk, HIGH);
      delayMicroseconds(2);
    }
    // ack
    digitalWrite(_clk, LOW);
    pinMode(_dio, INPUT);
    digitalWrite(_dio, HIGH);
    delayMicroseconds(2);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(2);
    pinMode(_dio, OUTPUT);
  }
  void writeData(uint8_t d) { writeByte(d); }
};

// ---------------- Hardware pin bindings (authoritative) ----------------
static const uint8_t PIN_VIBRATION = D2; // io_4 input (vibration)
static const uint8_t PIN_BUTTON    = D3; // io_0 input (confirm)
static const uint8_t PIN_TM_CLK     = D4; // io_3 tm1637 clk
static const uint8_t PIN_TM_DIO     = D5; // io_3 tm1637 dio
static const uint8_t PIN_BUZZER     = D6; // io_2 tone output (we use noTone for 0)
static const uint8_t PIN_LED        = D13; // io_1 output (blink control)

static const uint32_t CONTROL_PERIOD_MS = 10; // frozen contract control_period_ms=10
static const unsigned long SERIAL_IDLE_TIMEOUT_MS = 20;

// ---------------- Generated model controller (ACCEPTED) ----------------
struct Controller {
  int32_t inWashStage = 0;     // io_3
  bool prevPressed = false;    // internal
  bool faultActive = false;    // internal
  bool endConfirmed = false;   // internal
};

// SysML tick update and outputs exactly as in Accepted model
#line 152 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick(Controller &c, bool io_0, bool io_4);
#line 175 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool out_io_1_led(bool faultActive, int32_t inWashStage, bool endConfirmed);
#line 179 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t out_io_2_buzzerHz(bool faultActive, int32_t inWashStage, bool endConfirmed);
#line 183 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t out_io_3_display(int32_t inWashStage);
#line 187 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool out_serial_trigger(bool faultActive);
#line 191 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t out_serial_value(bool faultActive);
#line 199 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void maybeReportSerial(bool trigger, int32_t value);
#line 214 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 229 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 152 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void controllerTick(Controller &c, bool io_0, bool io_4) {
  if (!io_4) {
    c.faultActive = false;
    c.endConfirmed = false;
    c.inWashStage = 0;
    c.prevPressed = false;
  } else {
    if (c.faultActive) {
      c.inWashStage = 0;
      c.endConfirmed = c.endConfirmed; // unchanged
    }
    if (io_0 && !c.prevPressed) {
      c.endConfirmed = true;
    }
    if ((!c.faultActive) && (!c.endConfirmed)) {
      c.inWashStage = 3;
    } else {
      c.inWashStage = c.inWashStage; // unchanged
    }
    c.prevPressed = io_4;
  }
}

static inline bool out_io_1_led(bool faultActive, int32_t inWashStage, bool endConfirmed) {
  return faultActive ? false : ((inWashStage == 3) && (!endConfirmed)) ? true : false;
}

static inline int32_t out_io_2_buzzerHz(bool faultActive, int32_t inWashStage, bool endConfirmed) {
  return faultActive ? 0 : ((inWashStage == 3) && (!endConfirmed)) ? 1000 : 0;
}

static inline int32_t out_io_3_display(int32_t inWashStage) {
  return inWashStage;
}

static inline bool out_serial_trigger(bool faultActive) {
  return faultActive;
}

static inline int32_t out_serial_value(bool faultActive) {
  return faultActive ? 1 : 0;
}

// ---------------- Serial report trigger behavior ----------------
// Contract says: serial_0_trigger = faultActive
// Implement: when faultActive true, output line with numeric value (state=1) on trigger;
// when false do nothing (to preserve trigger semantics).
static void maybeReportSerial(bool trigger, int32_t value) {
  static bool lastTrigger = false;
  // line_or_idle with idle timeout is not explicitly needed; just write a line.
  if (trigger && !lastTrigger) {
    // Output placeholder: typed数值报告（i32 state=0）作为占位
    // Here value should be 1 when faultActive is set.
    Serial.print("wash_fault_state=");
    Serial.println(value);
  }
  lastTrigger = trigger;
}

// ---------------- Main ----------------
TM1637Display tm(PIN_TM_CLK, PIN_TM_DIO);

void setup() {
  pinMode(PIN_BUTTON, INPUT);     // io_0 digital_input
  pinMode(PIN_VIBRATION, INPUT); // io_4 digital_input

  pinMode(PIN_LED, OUTPUT);      // io_1 digital_output
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_BUZZER, OUTPUT);  // io_2 tone_output (policy invalid_policy=silence, but model drives via hz)

  tm.begin();
  tm.setBrightness(7); // contract brightness=7

  Serial.begin(9600);
}

void loop() {
  static unsigned long lastTickMs = 0;
  unsigned long now = millis();
  if (now - lastTickMs < CONTROL_PERIOD_MS) return;
  lastTickMs += CONTROL_PERIOD_MS;

  // Read inputs (model semantics: true_level HIGH, false_level LOW)
  bool io_0 = (digitalRead(PIN_BUTTON) == HIGH);     // io_0: button
  bool io_4 = (digitalRead(PIN_VIBRATION) == HIGH);  // io_4: vibration

  static Controller c;

  // Perform update action tick
  controllerTick(c, io_0, io_4);

  // Compute outputs exactly as model
  bool io_1 = out_io_1_led(c.faultActive, c.inWashStage, c.endConfirmed);
  int32_t io_2 = out_io_2_buzzerHz(c.faultActive, c.inWashStage, c.endConfirmed);
  int32_t io_3 = out_io_3_display(c.inWashStage);

  bool serial_trigger = out_serial_trigger(c.faultActive);
  int32_t serial_value = out_serial_value(c.faultActive);

  // Apply outputs
  digitalWrite(PIN_LED, io_1 ? HIGH : LOW);

  // io_2: tone_output i32 Hz; model uses 1000 or 0
  if (io_2 > 0) {
    // Use Arduino tone; frequency set to io_2.
    tone(PIN_BUZZER, (unsigned int)io_2);
  } else {
    noTone(PIN_BUZZER);
    digitalWrite(PIN_BUZZER, LOW);
  }

  // io_3: display stage number; invalid_policy=blank, leading_zero=false
  // For 0, display blank by turning leading zeros off; our showNumber blanks leading zeros
  // but may still show 0 if it's the least significant digit. To match "blank" for invalid only,
  // we assume 0 is valid and should show 0; model test expects initial value 0.
  tm.showNumber((int)io_3, false);

  // Serial report
  maybeReportSerial(serial_trigger, serial_value);
}
