#include <Arduino.h>
#include <stdint.h>

// ---- TM1637 driver (minimal, self-contained) ----
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : _clkPin(clkPin), _dioPin(dioPin) {}

  void begin() {
    pinMode(_clkPin, OUTPUT);
    pinMode(_dioPin, OUTPUT);
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dioPin, HIGH);
    delay(50);
  }

  void setBrightness(uint8_t brightness) {
    // brightness: 0..7
    // command 0x88 | brightness
    start();
    writeByte(0x40); // command
    writeByte(0x88 | (brightness & 0x07));
    stop();
  }

  // Display up to 4 digits; if leading_zero is false, higher digits blanked.
  // Here we use it for single value (0..60+), leaving rest blank.
  void displayNumber(int number, bool leadingZero = false) {
    uint8_t digits[4] = {0, 0, 0, 0};

    if (number < 0) number = 0;
    digits[0] = encodeDigit(number % 10);
    digits[1] = encodeDigit((number / 10) % 10);
    digits[2] = encodeDigit((number / 100) % 10);
    digits[3] = encodeDigit((number / 1000) % 10);

    // blank leading digits if not leadingZero
    if (!leadingZero) {
      // Determine highest non-zero digit position
      int n = number;
      if (n < 10) {
        digits[1] = segBlank();
        digits[2] = segBlank();
        digits[3] = segBlank();
      } else if (n < 100) {
        digits[2] = segBlank();
        digits[3] = segBlank();
      } else if (n < 1000) {
        digits[3] = segBlank();
      }
    }

    // write to display
    start();
    writeByte(0x40); // data command
    // use fixed address 0x00, then 4 bytes
    writeByte(0x00);
    for (int i = 0; i < 4; i++) {
      writeByte(digits[i]);
    }
    stop();
  }

private:
  uint8_t _clkPin, _dioPin;

  static uint8_t segBlank() { return 0x00; }

  // Segment map for common usage (a,b,c,d,e,f,g,dp) with standard TM1637
  static uint8_t encodeDigit(int d) {
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
    if (d < 0 || d > 9) return segBlank();
    return seg[d];
  }

  void start() {
    digitalWrite(_dioPin, HIGH);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(5);
    digitalWrite(_clkPin, LOW);
  }

  void stop() {
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(5);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(_dioPin, HIGH);
    delayMicroseconds(5);
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clkPin, LOW);
      delayMicroseconds(5);
      digitalWrite(_dioPin, (b & 0x01) ? HIGH : LOW);
      b >>= 1;
      delayMicroseconds(5);
      digitalWrite(_clkPin, HIGH);
      delayMicroseconds(5);
    }
    // ack
    digitalWrite(_clkPin, LOW);
    pinMode(_dioPin, INPUT);
    delayMicroseconds(5);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(5);
    pinMode(_dioPin, OUTPUT);
  }
};

// ---- Hardware bindings (from frozen contract) ----
static constexpr uint8_t PIN_START_PAUSE = 2; // D2, active-high input
static constexpr uint8_t PIN_RESET = 3;       // D3, active-high input
static constexpr uint8_t PIN_BUZZER = 6;      // D6 tone output
static constexpr uint8_t PIN_TM_CLK = 4;       // D4
static constexpr uint8_t PIN_TM_DIO = 5;       // D5

// ---- Controller State (exact names/semantics as model) ----
static bool running = false;                 // initial false
static bool paused = false;                  // initial false
static int32_t seconds = 0;                // initial 0
static bool previousStartPausePressed = false; // initial false
static bool previousResetPressed = false;  // initial false
static bool buzzerOn = false;              // initial false
static int32_t buzzerRemainingMs = 0;     // initial 0
static int32_t toneHz = 0;                // initial 0
static int32_t displayValue = 0;        // initial 0

// ---- Timing / scheduling ----
static constexpr uint32_t CONTROL_PERIOD_MS = 5;

TM1637 tm(PIN_TM_CLK, PIN_TM_DIO);

// ---- apply outputs exactly as model: io_2=toneHz, io_3=displayValue ----
static void applyOutputs() {
  // io_3: display seconds count (brightness=7, leading_zero=false, invalid_policy="blank")
  tm.displayNumber(displayValue, false);

  // io_2: toneHz to buzzer (invalid_policy="silence")
  if (toneHz <= 0 || !buzzerOn) {
    noTone(PIN_BUZZER);
  } else {
    // Use tone() with frequency toneHz; duration not specified by model outputs.
    // Model controls buzzerOn and toneHz and we keep updating every 5ms.
    tone(PIN_BUZZER, (unsigned int)toneHz);
  }
}

void setup() {
  pinMode(PIN_START_PAUSE, INPUT); // pullup disabled per contract (hardware pullup=false)
  pinMode(PIN_RESET, INPUT);       // pullup disabled per contract (hardware pullup=false)
  pinMode(PIN_BUZZER, OUTPUT);

  tm.begin();
  tm.setBrightness(7);

  // Initial model outputs
  applyOutputs();
}

void loop() {
  static uint32_t lastControlMs = 0;
  uint32_t now = millis();
  if (now - lastControlMs < CONTROL_PERIOD_MS) return;
  lastControlMs = now;

  // Sample inputs (active-high)
  bool io_0 = (digitalRead(PIN_START_PAUSE) == HIGH); // start/pause
  bool io_1 = (digitalRead(PIN_RESET) == HIGH);       // reset

  // ---- Controller tick: implement updates in the exact order from model ----
  // Perform: tick { first start; then ...; ... assign previous... }
  // 1) if io_0 and not previousStartPausePressed => paused := not paused
  if (io_0 && !previousStartPausePressed) {
    paused = (!paused);
  }

  // 2) if io_1 and not previousResetPressed => reset everything and clear display/tone states
  if (io_1 && !previousResetPressed) {
    running = false;
    paused = false;
    seconds = 0;
    buzzerOn = false;
    buzzerRemainingMs = 0;
    toneHz = 0;
    displayValue = 0;
  }

  // 3) if buzzerRemainingMs <= 0 => buzzerOn=false, toneHz=0
  if (buzzerRemainingMs <= 0) {
    buzzerOn = false;
    toneHz = 0;
  }

  // 4) if not paused and buzzerOn and buzzerRemainingMs > 0
  // then assign buzzerRemainingMs := buzzerRemainingMs; toneHz := toneHz;
  if (!paused && buzzerOn && buzzerRemainingMs > 0) {
    buzzerRemainingMs = buzzerRemainingMs;
    toneHz = toneHz;
  }

  // 5) if buzzerOn and buzzerRemainingMs <= 0 => buzzerOn=false; toneHz=0
  if (buzzerOn && buzzerRemainingMs <= 0) {
    buzzerOn = false;
    toneHz = 0;
  }

  // 6) if seconds >= 60 and not buzzerOn => buzzerOn=true; buzzerRemainingMs=1000; toneHz=1000
  if (seconds >= 60 && !buzzerOn) {
    buzzerOn = true;
    buzzerRemainingMs = 1000;
    toneHz = 1000;
  }

  // 7) if not paused:
  //    seconds := seconds; displayValue := seconds
  // else:
  //    displayValue := seconds
  if (!paused) {
    seconds = seconds;
    displayValue = seconds;
  } else {
    displayValue = seconds;
  }

  // 8) assign previousStartPausePressed := io_0
  previousStartPausePressed = io_0;

  // 9) assign previousResetPressed := io_1
  previousResetPressed = io_1;

  // Note: Model includes no decrement of buzzerRemainingMs, no increment of seconds.
  // We must not "fix" behavior. Outputs follow model.
  applyOutputs();
}