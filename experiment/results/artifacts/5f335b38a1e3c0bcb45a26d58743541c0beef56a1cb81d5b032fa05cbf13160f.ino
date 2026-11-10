#include <Arduino.h>

// -------------------- TM1637 (minimal implementation) --------------------
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin, uint8_t brightness = 7)
    : _clk(clkPin), _dio(dioPin), _brightness(brightness) {}

  void begin() {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
    setBrightness(_brightness);
    clearDisplay();
  }

  // invalid_policy='blank', leading_zero=false
  // We'll always display count; if count doesn't fit, blank.
  void displayNumber(int32_t number) {
    if (number < 0 || number > 999999) { // policy: blank on invalid
      clearDisplay();
      return;
    }

    uint8_t seg[4];
    bool showLeadingZero = false; // leading_zero=false

    uint16_t v = (uint16_t)number;
    for (int i = 3; i >= 0; --i) {
      uint8_t digit = (uint8_t)(v % 10);
      v /= 10;

      // If digit is leading zero and we don't show leading zeros, blank it.
      bool isLeading = true;
      // Determine if any non-zero digit exists to the left (in the original number width).
      // We'll compute leading-ness by checking higher digits already processed.
      // For i from 3..0, digits to left are those with smaller i indexes; easier:
      // track whether we've encountered first non-zero from MSB side.
      // We'll build segments after we know first non-zero position:
    }

    // Recompute digits for leading-zero suppression with MSB scan
    uint8_t digits[4];
    int32_t tmp = number;
    for (int i = 3; i >= 0; --i) { digits[i] = (uint8_t)(tmp % 10); tmp /= 10; }

    bool seenNonZero = false;
    for (int i = 0; i < 4; i++) {
      if (digits[i] != 0) seenNonZero = true;
      if (!seenNonZero && digits[i] == 0) seg[i] = 0x00; // blank
      else seg[i] = encodeDigit(digits[i]);
    }

    uint8_t data[4] = { seg[0], seg[1], seg[2], seg[3] };
    writeSegments(data);
  }

  void clearDisplay() {
    uint8_t data[4] = { 0x00, 0x00, 0x00, 0x00 };
    writeSegments(data);
  }

private:
  uint8_t _clk, _dio, _brightness;

  static uint8_t encodeDigit(uint8_t d) {
    // Segments for common 7-seg (a b c d e f g dp not used)
    // Bit mapping: 0=a,1=b,2=c,3=d,4=e,5=f,6=g
    // 0x3F for 0: a b c d e f on, g off
    static const uint8_t table[10] = {
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
    if (d > 9) return 0x00;
    return table[d];
  }

  void setBrightness(uint8_t b) {
    _brightness = (b & 0x07);
    // TM1637 brightness command
    start();
    writeByte(0x40);      // command
    stop();
    start();
    writeByte(0x80 | _brightness);
    stop();
  }

  void start() {
    digitalWrite(_dio, HIGH);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, LOW);
    digitalWrite(_clk, LOW);
  }

  void stop() {
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, LOW);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clk, LOW);
      digitalWrite(_dio, (b & 0x01) ? HIGH : LOW);
      b >>= 1;
      digitalWrite(_clk, HIGH);
    }
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, HIGH); // ACK
    digitalWrite(_clk, HIGH);
  }

  void writeSegments(const uint8_t segs[4]) {
    // command: 0x40 write data fixed address
    start();
    writeByte(0x40);
    stop();

    start();
    writeByte(0xC0); // address auto-increment
    for (int i = 0; i < 4; i++) {
      writeByte(segs[i]);
    }
    stop();
    // brightness left as configured
  }
};

// -------------------- Serial command parsing --------------------
// "line_or_idle" with idle timeout 20ms.
// Commands are mapped as: BEEP=1, LIGHT_ON=6, COUNT_RESET=3, LIGHT_OFF=5.
class SerialLineCommandParser {
public:
  void begin(unsigned long baud) {
    Serial.begin(baud);
    _buf[0] = '\0';
    _len = 0;
    _lastRxMs = 0;
  }

  // Returns true when a complete command frame was received.
  bool poll(int &outCommandValue) {
    bool valid = false;

    while (Serial.available() > 0) {
      char c = (char)Serial.read();
      _lastRxMs = millis();

      if (c == '\r' || c == '\n') {
        // finalize
        if (_len > 0) {
          _buf[_len] = '\0';
          valid = decode(_buf, outCommandValue);
        }
        _len = 0;
        _buf[0] = '\0';
        return valid;
      }

      if (_len < (int)sizeof(_buf) - 1) {
        _buf[_len++] = c;
      }
    }

    // idle timeout finalize
    if (_len > 0 && (_lastRxMs != 0) && (millis() - _lastRxMs >= _idleTimeoutMs)) {
      _buf[_len] = '\0';
      valid = decode(_buf, outCommandValue);
      _len = 0;
      _buf[0] = '\0';
      _lastRxMs = 0;
      return valid;
    }

    return false;
  }

  void setIdleTimeoutMs(uint32_t ms) { _idleTimeoutMs = ms; }

private:
  static bool equalsToken(const char* s, const char* token) {
    // Exact match, no extra spaces.
    return strcmp(s, token) == 0;
  }

  bool decode(const char* raw, int &out) {
    // Trim leading/trailing spaces
    const char* s = raw;
    while (*s == ' ' || *s == '\t') s++;
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == ' ' || s[n-1] == '\t')) n--;
    char tmp[32];
    if (n >= sizeof(tmp)) return false;
    memcpy(tmp, s, n);
    tmp[n] = '\0';

    if (equalsToken(tmp, "BEEP")) { out = 1; return true; }
    if (equalsToken(tmp, "LIGHT_OFF")) { out = 5; return true; }
    if (equalsToken(tmp, "LIGHT_ON")) { out = 6; return true; }
    if (equalsToken(tmp, "COUNT_RESET")) { out = 3; return true; }
    // Unknown -> invalid (do not assert valid)
    return false;
  }

  static constexpr uint8_t BUFSZ = 32;
  char _buf[BUFSZ];
  int _len = 0;
  uint32_t _lastRxMs = 0;
  uint32_t _idleTimeoutMs = 20;
};

// -------------------- Frozen Controller implementation --------------------
static const uint8_t PIN_D2 = 2;  // io_0 input
static const uint8_t PIN_D3 = 3;  // io_1 input
static const uint8_t PIN_D13 = 13; // io_2 output
static const uint8_t PIN_D6 = 6;   // io_3 tone_output
static const uint8_t PIN_D4 = 4;   // tm1637 clk
static const uint8_t PIN_D5 = 5;   // tm1637 dio

TM1637 tm(PIN_D4, PIN_D5, 7);
SerialLineCommandParser parser;

// Controller state (must preserve exact initial values)
static bool lightOn = false;       // initial false
static int32_t count = 0;         // initial 0
static bool previousIo0 = false;  // initial false
static bool previousIo1 = false;  // initial false

// Control period is 5ms, but logic itself updates each tick; preserve ordered updates by coding tick accordingly.
static const uint32_t CONTROL_PERIOD_MS = 5;

static uint32_t nextTickMs = 0;

void applyOutputs(bool io_2, int32_t io_3_hz, int32_t io_4_count, bool io_4_valid_unused) {
  // io_2 -> LED D13 (model true drives LED on)
  digitalWrite(PIN_D13, io_2 ? HIGH : LOW);

  // io_3 -> beeper tone_output invalid_policy='silence'
  if (io_3_hz == 1000) {
    tone(PIN_D6, (unsigned int)io_3_hz);
  } else {
    noTone(PIN_D6);
  }

  // io_4 -> TM1637 displays count (invalid_policy='blank')
  tm.displayNumber(io_4_count);
}

void setup() {
  pinMode(PIN_D2, INPUT);   // catalog default (no explicit pullup/polarity provided)
  pinMode(PIN_D3, INPUT);
  pinMode(PIN_D13, OUTPUT);

  tm.begin();
  // Ensure silence and LED off at start per controller initial outputs
  digitalWrite(PIN_D13, LOW);
  noTone(PIN_D6);
  tm.displayNumber(count);

  parser.begin(9600);
  parser.setIdleTimeoutMs(20);

  nextTickMs = millis();
}

void loop() {
  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) < 0) return;
  // Tick every 5ms (catch up if needed, but keep deterministic order per tick)
  nextTickMs += CONTROL_PERIOD_MS;

  // Read inputs (io_0, io_1) as digital model semantics true_level=HIGH, false_level=LOW, active_high=true.
  bool io_0 = digitalRead(PIN_D2) == HIGH;
  bool io_1 = digitalRead(PIN_D3) == HIGH;

  // Serial command input: io_5_valid and io_5
  int io_5 = 0;
  bool io_5_valid = parser.poll(io_5);

  // -------------------- Tick: exact update order from model --------------------
  // perform action tick {
  //  if ((io_5_valid) and (true)) { ... }
  //  if ((io_0) and (not previousIo0)) { ... }
  //  if ((io_1) and (not previousIo1)) { ... }
  //  previousIo0 := io_0;
  //  previousIo1 := io_1;
  // }

  if (io_5_valid && true) {
    if (io_5 == 6) {
      lightOn = true;
      count = count + 1;
    }
    if (io_5 == 5) {
      lightOn = false;
      count = count + 1;
    }
    if (io_5 == 1) {
      count = count + 1;
    }
    if (io_5 == 3) {
      count = 0;
    }
  }

  if (io_0 && (!previousIo0)) {
    lightOn = true;
    count = count + 1;
  }

  if (io_1 && (!previousIo1)) {
    lightOn = false;
    count = count + 1;
  }

  previousIo0 = io_0;
  previousIo1 = io_1;

  // Outputs:
  // io_2 = lightOn
  bool io_2 = lightOn;

  // io_3 = (if ((io_5_valid) and (io_5 == 1)) ? 1000 else 0)
  int32_t io_3 = (io_5_valid && (io_5 == 1)) ? 1000 : 0;

  // io_4 = count
  int32_t io_4 = count;

  // serial_0_trigger: io_5_valid
  // serial_0_value_0: count
  // serial_0_value_1: (if lightOn then 1 else 0)
  if (io_5_valid) {
    Serial.print("COUNT:");
    Serial.println((long)count);
    Serial.print("LIGHT_STATE:");
    Serial.println(lightOn ? 1 : 0);
  }

  // Apply hardware outputs for this tick
  applyOutputs(io_2, io_3, io_4, true);
}