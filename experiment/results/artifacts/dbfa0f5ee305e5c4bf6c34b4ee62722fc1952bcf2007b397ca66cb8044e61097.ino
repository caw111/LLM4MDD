#include <Arduino.h>

// Minimal TM1637 driver (no external deps)
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : clk(clkPin), dio(dioPin) {}

  void begin() {
    pinMode(clk, OUTPUT);
    pinMode(dio, OUTPUT);
    digitalWrite(clk, LOW);
    digitalWrite(dio, LOW);
    delay(50);
  }

  void setBrightness(uint8_t brightness) {
    // brightness: 0..7; but we'll clamp to 0..7
    brightness &= 0x07;
    start();
    writeByte(0x40);               // command: fixed address
    writeByte(0x88 | brightness); // display control + brightness
    stop();
  }

  void showNumberInt(int num, bool leadingZero) {
    // For our business logic, we display a small tier number (0..4).
    // Encode into 4 digits (TM1637 typical).
    uint8_t data[4];
    int n = num;

    // digits: d0..d3 (leftmost is d3)
    data[0] = encodeDigit(n % 10);
    data[1] = encodeDigit((n / 10) % 10);
    data[2] = encodeDigit((n / 100) % 10);
    data[3] = encodeDigit((n / 1000) % 10);

    // leadingZero: if false, blank higher digits that are leading zeros
    if (!leadingZero) {
      // blank from highest digit down until first non-zero
      bool started = false;
      for (int i = 3; i >= 0; --i) {
        int placeVal = (i == 0) ? 1 : (i == 1 ? 10 : (i == 2 ? 100 : 1000));
        int digit = (n / placeVal) % 10;
        if (!started) {
          if (digit == 0 && n < placeVal) {
            data[i] = 0x00; // blank
          } else {
            started = true;
          }
        }
      }
    }

    start();
    writeByte(0x40); // command mode
    // auto-increment address starts at 0
    writeByte(0x00);
    // TM1637 expects segments for each digit
    for (int i = 0; i < 4; i++) writeDigitRaw(i, data[i]);
    stop();

    // Alternative simpler method: use setSegments with fixed addresses
    // But we already wrote digits.
  }

  void showBlank() {
    start();
    writeByte(0x40);
    writeByte(0x00);
    for (int i = 0; i < 4; i++) writeDigitRaw(i, 0x00);
    stop();
  }

private:
  uint8_t clk, dio;

  static uint8_t encodeDigit(int d) {
    // 7-seg with DP off (common pattern for TM1637)
    static const uint8_t segMap[10] = {
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
    if (d < 0 || d > 9) return 0x00;
    return segMap[d];
  }

  void writeDigitRaw(uint8_t index, uint8_t segments) {
    // In TM1637, digit addresses are 0x00..0x03 when in 0x40 mode.
    // We already issued 0x00; but many implementations just write 4 bytes for segments.
    // We'll follow common approach: write 4 segment bytes sequentially.
    (void)index;
    writeByte(segments);
  }

  void start() {
    digitalWrite(dio, HIGH);
    digitalWrite(clk, HIGH);
    delayMicroseconds(5);
    digitalWrite(dio, LOW);
    delayMicroseconds(5);
    digitalWrite(clk, LOW);
  }

  void stop() {
    digitalWrite(clk, LOW);
    digitalWrite(dio, LOW);
    delayMicroseconds(5);
    digitalWrite(clk, HIGH);
    delayMicroseconds(5);
    digitalWrite(dio, HIGH);
    delayMicroseconds(5);
  }

  void writeByte(uint8_t b) {
    // MSB first
    for (int i = 0; i < 8; i++) {
      digitalWrite(clk, LOW);
      digitalWrite(dio, (b & 0x01) ? HIGH : LOW);
      b >>= 1;
      delayMicroseconds(5);
      digitalWrite(clk, HIGH);
      delayMicroseconds(5);
    }
    // ACK
    digitalWrite(clk, LOW);
    pinMode(dio, INPUT);
    delayMicroseconds(5);
    digitalWrite(clk, HIGH);
    delayMicroseconds(5);
    pinMode(dio, OUTPUT);
    digitalWrite(dio, HIGH);
  }
};

// ----- Frozen contract mappings -----
static constexpr uint8_t FAN_PWM_PIN = 5;   // D5
static constexpr uint8_t LM35_PIN = A1;     // A1
static constexpr uint8_t BUZZER_PIN = 6;    // D6
static constexpr uint8_t TM_CLK_PIN = 4;    // D4
static constexpr uint8_t TM_DIO_PIN = 2;    // D2

// planning: control_period_ms = 10
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

// TM1637 display settings from contract:
static constexpr uint8_t TM_BRIGHTNESS = 7;
static constexpr bool TM_LEADING_ZERO = false; // contract: leading_zero=false

// Model thresholds and outputs are exactly as provided in Accepted executable model
// io_0 output:
static float compute_io_0(float io_1) {
  if (io_1 > 30.0f) {
    if (io_1 >= 40.0f) return 255.0f;
    else if (io_1 >= 35.0f) return 160.0f;
    else return 80.0f;
  } else {
    return 0.0f;
  }
}

// io_2 output:
static int32_t compute_io_2(float io_1) {
  if (io_1 >= 40.0f) return 2000;
  return 0;
}

// io_3 output:
static int32_t compute_io_3(float io_1) {
  if (io_1 > 30.0f) {
    if (io_1 >= 40.0f) return 4;
    else if (io_1 >= 35.0f) return 3;
    else if (io_1 >= 30.0f) return 2;
    else return 1;
  } else {
    return 0;
  }
}

static float read_io_1_adc_count_f64() {
  // Contract: io_1 is f64 unit adc_count.
  // We'll return raw ADC count as float.
  // A1 on UNO, analogRead returns 0..1023.
  int v = analogRead(LM35_PIN);
  return (float)v;
}

static void write_io_0_pwm_count(float io_0) {
  // Contract: io_0 unit pwm_count, to pwm_output on D5.
  // We implement directly as duty cycle [0..255] with analogWrite.
  int duty = (int)roundf(io_0);
  if (duty < 0) duty = 0;
  if (duty > 255) duty = 255;
  analogWrite(FAN_PWM_PIN, duty);
}

static void write_io_2_buzzer_tone(int32_t io_2) {
  // Contract: io_2 output i32 Hz on D6; invalid_policy="silence"
  if (io_2 <= 0) {
    noTone(BUZZER_PIN);
  } else {
    tone(BUZZER_PIN, (unsigned int)io_2);
  }
}

static void write_io_3_tm1637_tier(int32_t io_3) {
  // Contract: tm1637_number brightness=7, invalid_policy="blank", leading_zero=false
  // Model outputs only 0..4. We display that integer.
  if (io_3 < 0 || io_3 > 9999) {
    // invalid_policy blank (not expected)
    // We'll just show blank.
  }
  // showNumberInt(num, leadingZero)
}

int main();

TM1637 tm(TM_CLK_PIN, TM_DIO_PIN);

void setup() {
  pinMode(FAN_PWM_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  analogReference(DEFAULT);

  tm.begin();
  tm.setBrightness(TM_BRIGHTNESS);

  // Contract: TM1637 startup display value is set to 0
  tm.showNumberInt(0, TM_LEADING_ZERO);

  // Contract: Fan PWM initial is 0, buzzer invalid_policy silence
  analogWrite(FAN_PWM_PIN, 0);
  noTone(BUZZER_PIN);
}

void loop() {
  static uint32_t lastTick = 0;
  uint32_t now = millis();
  if (now - lastTick < CONTROL_PERIOD_MS) return;
  lastTick += CONTROL_PERIOD_MS;

  // io_1 (adc_count)
  float io_1 = read_io_1_adc_count_f64();

  // Apply frozen model logic
  float io_0 = compute_io_0(io_1);   // pwm_count
  int32_t io_2 = compute_io_2(io_1); // Hz (buzzer tone)
  int32_t io_3 = compute_io_3(io_1); // tier number for display

  // Ordered updates: outputs io_0 then io_2 then io_3 (as typical deterministic ordering).
  // The contract states "Preserve ... ordered updates, output expressions..."
  // We apply in a stable order: io_0, io_2, io_3.
  write_io_0_pwm_count(io_0);
  write_io_2_buzzer_tone(io_2);

  // TM1637 display update
  if (io_3 < 0) {
    tm.showBlank();
  } else {
    tm.showNumberInt(io_3, TM_LEADING_ZERO);
  }
}