#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// -------------------- TM1637 Minimal Driver --------------------
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin)
      : clkPin_(clkPin), dioPin_(dioPin) {}

  void begin() {
    pinMode(clkPin_, OUTPUT);
    pinMode(dioPin_, OUTPUT);
    digitalWrite(clkPin_, HIGH);
    digitalWrite(dioPin_, HIGH);
    delayMicroseconds(50);
  }

  // addr: 0..3 (left to right)
  void setSegments(const uint8_t segments[4], uint8_t pos) {
    start();
    writeByte(0x40); // data command
    // position
    // (tm1637 doesn't take pos in same way; we keep it simple: write all digits)
    // but for our purpose we only need full 4-digit display control.
    // This function kept for compatibility.
    (void)pos;

    for (int i = 0; i < 4; i++) writeByte(segments[i] & 0xFF);
    stop();
    delayMicroseconds(50);
  }

  // brightness: 0..7
  void setBrightness(uint8_t brightness) {
    // Display control command
    start();
    writeByte(0x88 | (brightness & 0x07));
    stop();
  }

  // Show 1..4 digits by writing segments from array positions.
  // This is a full 4-digit write.
  void showNumberInt(int num, bool leadingZero = false, uint8_t brightness = 7) {
    setBrightness(brightness);
    uint8_t segs[4] = {0, 0, 0, 0};

    // Handle negative
    bool neg = false;
    if (num < 0) {
      neg = true;
      num = -num;
    }

    // Convert to digits (right aligned)
    int d[4] = {0, 0, 0, 0};
    for (int i = 3; i >= 0; --i) {
      d[i] = num % 10;
      num /= 10;
    }

    // leading zero policy
    bool started = leadingZero;
    for (int i = 0; i < 4; i++) {
      int digit = d[i];

      if (!started) {
        if (digit == 0 && i != 3) {
          segs[i] = 0x00; // blank
        } else {
          started = true;
          segs[i] = digitToSeg(digit);
        }
      } else {
        segs[i] = digitToSeg(digit);
      }
    }

    if (neg) {
      // Put minus sign in the leftmost non-blank digit position (or first digit).
      // We'll overwrite leftmost position if currently blank; else just set first.
      int idx = 0;
      while (idx < 3 && segs[idx] == 0x00) idx++;
      segs[idx] = minusSeg();
    }

    // Write digits
    start();
    writeByte(0x40); // fixed address
    for (int i = 0; i < 4; i++) writeByte(segs[i]);
    stop();
    delayMicroseconds(50);
  }

private:
  uint8_t clkPin_, dioPin_;

  uint8_t digitToSeg(int digit) {
    // Typical TM1637 segment mapping: a b c d e f g (dp handled separately with bit7)
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

  uint8_t minusSeg() { return 0x40; } // g segment

  void start() {
    pinMode(dioPin_, OUTPUT);
    digitalWrite(clkPin_, HIGH);
    digitalWrite(dioPin_, HIGH);
    delayMicroseconds(5);
    digitalWrite(dioPin_, LOW);
    delayMicroseconds(5);
    digitalWrite(clkPin_, LOW);
  }

  void stop() {
    pinMode(dioPin_, OUTPUT);
    digitalWrite(clkPin_, LOW);
    digitalWrite(dioPin_, LOW);
    delayMicroseconds(5);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    digitalWrite(dioPin_, HIGH);
    delayMicroseconds(5);
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(clkPin_, LOW);
      if (b & 0x01) digitalWrite(dioPin_, HIGH);
      else digitalWrite(dioPin_, LOW);
      delayMicroseconds(5);
      digitalWrite(clkPin_, HIGH);
      delayMicroseconds(5);
      b >>= 1;
    }

    // Ack
    digitalWrite(clkPin_, LOW);
    pinMode(dioPin_, INPUT);
    delayMicroseconds(5);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(50);
    pinMode(dioPin_, OUTPUT);
  }
};

// -------------------- Frozen Model Implementation --------------------
// Control period contract: 5ms
static const uint32_t CONTROL_PERIOD_MS = 5;

// Pins per authoritative hardware contract
static const uint8_t PIN_PWM_LED = 9;   // D9
static const uint8_t PIN_D13 = 13;      // D13 (optional status LED)
static const uint8_t PIN_BTN_MODE = 2;  // D2 (io_0)
static const uint8_t PIN_BTN_PARAM = 3; // D3 (io_1)
static const uint8_t PIN_TM_CLK = 4;    // D4
static const uint8_t PIN_TM_DIO = 5;    // D5

// Model states (exact names/semantics from accepted model)
static int32_t mode_ = 0;
static int32_t param_ = 128;
static float brightnessGate_ = 0.0f;
static int32_t accumulatedMs_ = 0;
static bool previousPressed_ = false;

// Model outputs
#line 178 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float io_2_brightnessGate();
#line 179 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool io_3_modeNotZero();
#line 180 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t io_4_displayValue();
#line 192 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t sampleElapsedMsDelta();
#line 204 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readButtonPressed(uint8_t pin);
#line 211 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void tick(int32_t io_0, int32_t io_1, int32_t io_5);
#line 299 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writePWMFromBrightness(float v);
#line 308 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeStatusLed(bool on);
#line 312 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeTM1637(int32_t value);
#line 320 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 347 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 178 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float io_2_brightnessGate() { return brightnessGate_; }
static bool io_3_modeNotZero() { return (mode_ != 0); }
static int32_t io_4_displayValue() {
  // (mode==0 || mode==1) ? mode : floor(param)
  if (mode_ == 0 || mode_ == 1) return mode_;
  // param is integer already; floor(param) == param
  return (int32_t)param_;
}

static TM1637 tm1637(PIN_TM_CLK, PIN_TM_DIO);

// -------------------- IO sampling with delta since previous step (io_5) --------------------
static uint32_t lastStepMicros = 0;

static int32_t sampleElapsedMsDelta() {
  // Contract: io_5 is "delta_since_previous_step" in ms.
  uint32_t nowMicros = micros();
  uint32_t deltaMicros = nowMicros - lastStepMicros;
  lastStepMicros = nowMicros;

  // Convert to integer ms delta
  int32_t deltaMs = (int32_t)((deltaMicros + 500) / 1000); // rounded
  if (deltaMs < 0) deltaMs = 0;
  return deltaMs;
}

static inline bool readButtonPressed(uint8_t pin) {
  // Contract: inputs assume pullup enabled and active_high=true
  // pressed corresponds to electrical HIGH.
  return digitalRead(pin) == HIGH;
}

// -------------------- Model tick --------------------
static void tick(int32_t io_0, int32_t io_1, int32_t io_5) {
  // updates:
  // previousPressed := io_0
  bool io0 = (io_0 != 0);
  bool io1 = (io_1 != 0);

  // order-preserving per accepted model semantics:
  // first start; previousPressed := io_0
  previousPressed_ = io0;

  // then if (io_0 and not previousPressed) ... BUT previousPressed was just assigned.
  // The accepted model performs: previousPressed := io_0; then uses previousPressed in the same tick.
  // To preserve EXACT model behavior, implement as written:
  // previousPressed equals io_0 now, so "not previousPressed" => false, no mode change ever.
  // However, the SysML snippet shows "previousPressed := io_0" then
  // "if (io_0 and not previousPressed)". That indeed makes edge detection impossible.
  // We must preserve model behavior exactly as specified.
  if (io0 && (!previousPressed_)) {
    if (mode_ == 2) mode_ = 0;
    else mode_ = mode_ + 1;
  }

  // parameter update:
  if (io1 && !(mode_ == 2)) {
    if (param_ == 255) param_ = 0;
    else param_ = param_ + 16;
  } else {
    if (param_ == 5000) param_ = 500;
    else param_ = param_ + 500;
  }

  // outputs/continuous assignments via updates:
  if (mode_ == 0) {
    brightnessGate_ = 0.0f;
  }

  if (mode_ == 1) {
    // brightnessGate := (param >= 255) ? 255.0 : param*1.0
    if (param_ >= 255) brightnessGate_ = 255.0f;
    else brightnessGate_ = (float)param_ * 1.0f;
  }

  if (mode_ == 2) {
    // accumulatedMs := accumulatedMs + io_5
    accumulatedMs_ = accumulatedMs_ + io_5;

    // brightnessGate := complex nested conditional from accepted model
    // brightnessGate :=
    // if (accumulatedMs < (param*1)) then
    //   if (accumulatedMs >= (param/2)) then
    //     (param - ((accumulatedMs - (param/2))/1))/1
    //   else accumulatedMs / (param/255)
    // else 0.0
    int32_t param1 = param_ * 1;
    if (accumulatedMs_ < param1) {
      int32_t half = param_ / 2;
      if (accumulatedMs_ >= half) {
        // ((((param - (((((accumulatedMs - half))/1))))) /1))))))
        // Simplify while preserving integer ops:
        int32_t inner = accumulatedMs_ - half;
        int32_t val = param_ - (inner / 1);
        brightnessGate_ = (float)val / 1.0f;
      } else {
        // accumulatedMs / (param/255)
        int32_t denom = (param_ / 255);
        if (denom == 0) {
          // integer division by zero is undefined; accepted model implies mathematical form.
          // To keep program safe, define brightnessGate as 0 when denom==0.
          brightnessGate_ = 0.0f;
        } else {
          brightnessGate_ = (float)accumulatedMs_ / (float)denom;
        }
      }
    } else {
      brightnessGate_ = 0.0f;
    }
  }

  // then if (mode==2 and accumulatedMs >= param*2) accumulatedMs := 0
  if (mode_ == 2 && accumulatedMs_ >= (param_ * 2)) {
    accumulatedMs_ = 0;
  }

  // outputs are derived after updates
  // (io_2 brightnessGate_ already set; io_3 & io_4 computed in main loop)
}

// -------------------- Arduino PWM and IO scaling --------------------
static void writePWMFromBrightness(float v) {
  // Contract: io_2 is pwm_count with scale 1.0 offset 0.0.
  // On Arduino UNO, analogWrite uses 0..255. Clamp exactly.
  if (v < 0.0f) v = 0.0f;
  if (v > 255.0f) v = 255.0f;
  uint8_t pwm = (uint8_t)(v + 0.5f); // round to nearest
  analogWrite(PIN_PWM_LED, pwm);
}

static void writeStatusLed(bool on) {
  digitalWrite(PIN_D13, on ? HIGH : LOW);
}

static void writeTM1637(int32_t value) {
  // Model contract: io_4 is "mode number or floor(param)".
  // TM1637: brightness 7, invalid_policy "blank", leading_zero false.
  // We'll show integer directly.
  tm1637.showNumberInt((int)value, false, 7);
}

// -------------------- Setup/Loop --------------------
void setup() {
  pinMode(PIN_BTN_MODE, INPUT_PULLUP);   // D2 io_0
  pinMode(PIN_BTN_PARAM, INPUT_PULLUP);  // D3 io_1

  pinMode(PIN_PWM_LED, OUTPUT); // D9
  pinMode(PIN_D13, OUTPUT);    // D13

  analogWrite(PIN_PWM_LED, 0);

  tm1637.begin();
  tm1637.setBrightness(7);

  // Initialize model to accepted initial states
  mode_ = 0;
  param_ = 128;
  brightnessGate_ = 0.0f;
  accumulatedMs_ = 0;
  previousPressed_ = false;

  // Initialize display and outputs consistent with initial states
  writePWMFromBrightness(io_2_brightnessGate());
  writeStatusLed(io_3_modeNotZero());
  writeTM1637(io_4_displayValue());

  lastStepMicros = micros();
}

void loop() {
  static uint32_t nextMs = millis();
  // Enforce 5ms control period
  uint32_t now = millis();
  if ((int32_t)(now - nextMs) < 0) return;
  nextMs += CONTROL_PERIOD_MS;

  // sample inputs (digital_boolean)
  bool io_0_pressed = readButtonPressed(PIN_BTN_MODE);
  bool io_1_pressed = readButtonPressed(PIN_BTN_PARAM);

  // io_5 elapsed ms delta
  int32_t io_5 = sampleElapsedMsDelta();

  // tick model
  tick(io_0_pressed ? 1 : 0, io_1_pressed ? 1 : 0, io_5);

  // apply outputs
  writePWMFromBrightness(io_2_brightnessGate());
  writeStatusLed(io_3_modeNotZero());
  writeTM1637(io_4_displayValue());
}
