#include <Arduino.h>

// TM1637
// Use a lightweight implementation to avoid external dependencies.
class TM1637 {
public:
  TM1637(uint8_t clk, uint8_t dio) : _clk(clk), _dio(dio) {}

  void begin() {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, LOW);
    _brightness = 7;
  }

  void setBrightness(uint8_t b) {
    _brightness = b & 0x7;
    // Display control command
    start();
    writeByte(0x40); // CMD1: data command + starting address mode
    stop();
    start();
    writeByte(0x88 | _brightness); // CMD2: display control
    stop();
  }

  void showNumberInt(int num, bool leadingZero) {
    // Show a signed int up to 4 digits; if leadingZero=false, suppress zeros.
    uint8_t seg[4];
    for (int i = 0; i < 4; i++) seg[i] = 0;

    bool neg = (num < 0);
    unsigned int x = neg ? (unsigned int)(-num) : (unsigned int)num;

    // Determine digits
    int digits[4] = {0, 0, 0, 0};
    for (int i = 3; i >= 0; --i) {
      digits[i] = (int)(x % 10);
      x /= 10;
    }

    // Decide suppression
    bool started = false;
    for (int i = 0; i < 4; i++) {
      int d = digits[i];
      if (d != 0 || started || leadingZero || i == 3) {
        started = true;
        seg[i] = digitToSeg(d);
      } else {
        seg[i] = 0;
      }
    }

    if (neg) {
      // Put '-' in the first position
      // If leadingZero suppression turned first positions off, we still set minus there.
      seg[0] = DASH;
    }

    displaySegments(seg, 4);
  }

private:
  uint8_t _clk, _dio;
  uint8_t _brightness{7};

  static constexpr uint8_t ZERO = 0x3F;
  static constexpr uint8_t ONE = 0x06;
  static constexpr uint8_t TWO = 0x5B;
  static constexpr uint8_t THREE = 0x4F;
  static constexpr uint8_t FOUR = 0x66;
  static constexpr uint8_t FIVE = 0x6D;
  static constexpr uint8_t SIX = 0x7D;
  static constexpr uint8_t SEVEN = 0x07;
  static constexpr uint8_t EIGHT = 0x7F;
  static constexpr uint8_t NINE = 0x6F;
  static constexpr uint8_t DASH = 0x40;

  uint8_t digitToSeg(uint8_t d) const {
    switch (d) {
      case 0: return ZERO;
      case 1: return ONE;
      case 2: return TWO;
      case 3: return THREE;
      case 4: return FOUR;
      case 5: return FIVE;
      case 6: return SIX;
      case 7: return SEVEN;
      case 8: return EIGHT;
      case 9: return NINE;
      default: return 0;
    }
  }

  void start() {
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, LOW);
    delayMicroseconds(5);
    digitalWrite(_clk, LOW);
  }

  void stop() {
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, LOW);
    delayMicroseconds(5);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(5);
    digitalWrite(_dio, HIGH);
    delayMicroseconds(5);
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clk, LOW);
      delayMicroseconds(3);
      digitalWrite(_dio, (b & 0x01) ? HIGH : LOW);
      b >>= 1;
      delayMicroseconds(5);
      digitalWrite(_clk, HIGH);
      delayMicroseconds(5);
    }
    digitalWrite(_clk, LOW);
    pinMode(_dio, INPUT_PULLUP);
    delayMicroseconds(5);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(5);
    pinMode(_dio, OUTPUT);
  }

  void displaySegments(const uint8_t segs[], uint8_t length) {
    // Command: auto-increment starting at address 0xC0
    start();
    writeByte(0xC0); // set address
    for (uint8_t i = 0; i < length; i++) {
      writeByte(segs[i]);
    }
    stop();

    // Display control command
    start();
    writeByte(0x80 | _brightness);
    stop();
  }
};

// ---- Frozen model mapping (Accepted executable model) ----
// io_0: environment temperature from A0
// io_1: evaporator temperature from A1
// io_2: door open (D2 with pullup, active high)
// io_3: fan control (D5) = io_0 < 30.0
// io_4: defrost relay (D10) = io_1 >= 4.0 and io_1 < 6.0
// io_5: buzzer frequency command (D6 tone) = if io_0 >= 30.0 then 2000 else (if io_1 >= 6.0 then 2500 else 0)
// io_6: TM1637 number = if io_0 >= 30.0 then 3 else (if io_1 >= 6.0 then 2 else (if io_1 >= 4.0 then 1 else 0))
//
// serial_0_trigger = (io_0 != 0.0) or (io_1 != 0.0) or doorOpenRising
// serial_0_value_0 = io_0
// serial_0_value_1 = io_1
// serial_0_value_2 = io_2
// serial_0_value_3 = same alarm stage encoding as io_6
//
// Model controller state:
// prevDoorOpen (bool, initial false)
// doorOpenRising = io_2 and (not prevDoorOpen)
// prevDoorOpen := io_2

// Timing contract: control_period_ms = 10
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

// Hardware pins
static constexpr uint8_t PIN_LM35_ENV = A0;
static constexpr uint8_t PIN_LM35_EVA = A1;

static constexpr uint8_t PIN_DOOR = D2;       // io_2 input with pullup
static constexpr uint8_t PIN_FAN = D5;        // io_3 output
static constexpr uint8_t PIN_DEFROST = D10;   // io_4 output

static constexpr uint8_t PIN_BUZZER = D6;    // io_5 tone
static constexpr uint8_t PIN_TM_CLK = D4;    // TM1637 CLK
static constexpr uint8_t PIN_TM_DIO = D3;    // TM1637 DIO

// ADC conversion to degC for LM35:
// LM35: 10mV/°C, typical Vref=5.0V
// But original contract binds scale/offset as 1.0, and model treats io_0/io_1 as degC.
// We'll compute degC using standard LM35 formula with assumed 5V reference.
static constexpr float ADC_VREF = 5.0f;
static constexpr float ADC_COUNTS = 1023.0f;
static constexpr float LM35_MV_PER_C = 10.0f;

TM1637 tm(PIN_TM_CLK, PIN_TM_DIO);

// Controller state
bool prevDoorOpen = false;

// Alarm stage encoding per model
static int stageFromTemps(float io0, float io1) {
  if (io0 >= 30.0f) return 3;
  if (io1 >= 6.0f) return 2;
  if (io1 >= 4.0f) return 1;
  return 0;
}

static float readLM35DegC(uint8_t pin) {
  int raw = analogRead(pin);
  float v = (raw * ADC_VREF) / ADC_COUNTS;       // volts
  float mv = v * 1000.0f;                       // millivolts
  float degC = mv / LM35_MV_PER_C;              // LM35 conversion
  return degC;
}

int main() {
  init();

  // setup()
  pinMode(PIN_DOOR, INPUT_PULLUP); // pullup=true, active_high=true
  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_DEFROST, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  digitalWrite(PIN_FAN, LOW);
  digitalWrite(PIN_DEFROST, LOW);
  noTone(PIN_BUZZER);

  tm.begin();
  tm.setBrightness(7); // brightness=7

  Serial.begin(9600);

  uint32_t lastTick = millis();

  while (true) {
    // loop() with fixed control period (10ms)
    uint32_t now = millis();
    if (now - lastTick >= CONTROL_PERIOD_MS) {
      lastTick += CONTROL_PERIOD_MS;

      // Sample inputs
      float io_0 = readLM35DegC(PIN_LM35_ENV); // env temp
      float io_1 = readLM35DegC(PIN_LM35_EVA); // evap temp
      bool io_2 = digitalRead(PIN_DOOR) == HIGH; // door open = true

      // Update state in order (perform tick)
      bool doorOpenRising = io_2 && (!prevDoorOpen);
      prevDoorOpen = io_2;

      // Outputs (exact expressions)
      bool io_3 = (io_0 < 30.0f); // fan output
      bool io_4 = (io_1 >= 4.0f && io_1 < 6.0f); // defrost relay

      int io_5;
      if (io_0 >= 30.0f) {
        io_5 = 2000;
      } else {
        io_5 = (io_1 >= 6.0f) ? 2500 : 0;
      }

      int io_6 = stageFromTemps(io_0, io_1);

      // serial_0_trigger = or( io_0 != 0.0, io_1 != 0.0, doorOpenRising )
      bool serial_0_trigger = (io_0 != 0.0f) || (io_1 != 0.0f) || doorOpenRising;
      int serial_0_value_3 = stageFromTemps(io_0, io_1);

      // Drive hardware outputs
      digitalWrite(PIN_FAN, io_3 ? HIGH : LOW);
      digitalWrite(PIN_DEFROST, io_4 ? HIGH : LOW);

      if (io_5 > 0) {
        tone(PIN_BUZZER, io_5);
      } else {
        noTone(PIN_BUZZER);
      }

      tm.showNumberInt(io_6, false); // leading_zero=false

      // Serial reporting (triggered)
      if (serial_0_trigger) {
        // Contract doesn't fix a text protocol; output compact line with the required fields.
        Serial.print("t,");
        Serial.print(io_0, 2);
        Serial.print(",v,");
        Serial.print(io_1, 2);
        Serial.print(",door,");
        Serial.print(io_2 ? 1 : 0);
        Serial.print(",lvl,");
        Serial.println(serial_0_value_3);
      }

      // ensure we don't drift too much
    }
  }
}