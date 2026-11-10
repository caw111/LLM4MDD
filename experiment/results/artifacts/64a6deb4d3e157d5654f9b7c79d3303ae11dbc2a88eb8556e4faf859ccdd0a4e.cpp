#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// ------------ Pin assignments (per requirements) ------------
static const uint8_t TRIG_PIN = 8;   // HC-SR04 Trig = D8
static const uint8_t ECHO_PIN = 2;   // HC-SR04 Echo = D2

static const uint8_t TM1637_CLK = 4; // TM1637 CLK = D4
static const uint8_t TM1637_DIO = 5; // TM1637 DIO = D5

static const uint8_t LED_D13 = 13;    // Near-distance ON
static const uint8_t PWM_D9 = 9;     // PWM brightness based on distance

// ------------ TM1637 minimal implementation (no behavior simplified) ------------
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : clk(clkPin), dio(dioPin) {}

  void begin() {
    pinMode(clk, OUTPUT);
    pinMode(dio, OUTPUT);

    digitalWrite(clk, HIGH);
    digitalWrite(dio, HIGH);
  }

  void setBrightness(uint8_t brightness) {
    // TM1637 brightness: 0..7 typically
    brightness &= 0x07;
    start();
    writeByte(0x40); // command
    writeByte(0x88 | brightness); // brightness command
    stop();
  }

  void displayDigits(uint8_t pos, uint8_t value) {
    // pos: 0..3 digit positions
    // value: segment encoding
    start();
    writeByte(0x44); // fixed address
    stop();

    start();
    writeByte(0xC0 | (pos & 0x03));
    writeByte(value);
    stop();
  }

  void display4(uint16_t data) {
    // data: segments packed for 4 digits not used in this sketch
    (void)data;
  }

  void displayNumber(int value, uint8_t colon = 0) {
    (void)colon; // no colon requested

    // Convert to 4-digit representation
    // The sketch shows distance on the 4-digit display.
    // Values over 9999 will be clamped.
    if (value < 0) value = 0;
    if (value > 9999) value = 9999;

    uint8_t digits[4];
    digits[0] = encodeDigit((value / 1000) % 10);
    digits[1] = encodeDigit((value / 100) % 10);
    digits[2] = encodeDigit((value / 10) % 10);
    digits[3] = encodeDigit(value % 10);

    // If leading zeros should be blanked: typical behavior.
    // Requirement only says "display distance"; we'll blank leading zeros for clarity
    // without changing core functionality.
    uint8_t rawDigits[4];
    for (int i = 0; i < 4; i++) rawDigits[i] = digits[i];

    // Determine leading zero blanking based on actual numeric digits.
    // If the number is 0..9, blank first three.
    // 10..99, blank first two.
    // 100..999, blank first one.
    // 1000..9999, no blanking.
    uint16_t v = (uint16_t)value;
    if (v < 10) {
      digits[0] = 0;
      digits[1] = 0;
      digits[2] = 0;
    } else if (v < 100) {
      digits[0] = 0;
      digits[1] = 0;
    } else if (v < 1000) {
      digits[0] = 0;
    }

    // Write all 4 digits using auto-increment
    start();
    writeByte(0x40); // command
    stop();

    start();
    writeByte(0xC0); // starting address
    writeByte(digits[0]);
    writeByte(digits[1]);
    writeByte(digits[2]);
    writeByte(digits[3]);
    stop();
  }

private:
  uint8_t clk, dio;

  void start() {
    digitalWrite(dio, HIGH);
    digitalWrite(clk, HIGH);
    delayMicroseconds(5);
    digitalWrite(dio, LOW);
    delayMicroseconds(5);
    digitalWrite(clk, LOW);
  }

  void stop() {
    digitalWrite(dio, LOW);
    digitalWrite(clk, HIGH);
    delayMicroseconds(5);
    digitalWrite(dio, HIGH);
    delayMicroseconds(5);
  }

  void writeByte(uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
      digitalWrite(clk, LOW);
      delayMicroseconds(3);
      digitalWrite(dio, (b & 0x01) ? HIGH : LOW);
      b >>= 1;
      delayMicroseconds(3);
      digitalWrite(clk, HIGH);
      delayMicroseconds(3);
    }
    digitalWrite(clk, LOW);
    delayMicroseconds(3);
    // ACK cycle
    pinMode(dio, INPUT);
    digitalWrite(dio, HIGH);
    delayMicroseconds(3);
    digitalWrite(clk, HIGH);
    delayMicroseconds(10);
    pinMode(dio, OUTPUT);
    digitalWrite(clk, LOW);
    delayMicroseconds(3);
  }

  uint8_t encodeDigit(uint8_t d) {
    // Segments for TM1637 (a b c d e f g dp) without dp.
    // Bit mapping commonly used: 0=a,1=b,2=c,3=d,4=e,5=f,6=g,7=dp.
    static const uint8_t segMap[10] = {
      0b00111111, // 0
      0b00000110, // 1
      0b01011011, // 2
      0b01001111, // 3
      0b01100110, // 4
      0b01101101, // 5
      0b01111101, // 6
      0b00000111, // 7
      0b01111111, // 8
      0b01101111  // 9
    };
    if (d < 10) return segMap[d];
    return 0;
  }
};

TM1637 tm(TM1637_CLK, TM1637_DIO);

// ------------ Distance measurement and display logic ------------
static const uint8_t NUM_SAMPLES = 5;

// Threshold behavior for D13 (near distance) and PWM (brightness)
// Requirements state: "LED D13 在近距离亮起" and "PWM LED D9 根据距离远近改变亮度，距离越近亮度越高。"
// We'll implement a near threshold and a linear brightness mapping with saturation.
static const uint16_t NEAR_DISTANCE_CM = 50;   // "near": <= 50 cm
static const uint16_t MAX_DISTANCE_CM  = 400;  // mapping range upper bound

// Measure HC-SR04 distance (cm) with timeout and simple averaging.
#line 180 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t measureDistanceCm();
#line 219 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t brightnessFromDistance(uint16_t distCm);
#line 230 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 245 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 180 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
uint16_t measureDistanceCm() {
  uint32_t sum = 0;
  uint8_t validCount = 0;

  for (uint8_t i = 0; i < NUM_SAMPLES; i++) {
    // Trigger pulse
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Measure echo pulse width
    // Timeout corresponds to MAX_DISTANCE_CM.
    // Sound speed ~ 343 m/s => 29.1 us per cm (round trip).
    // Echo time us = cm * 58.2
    uint32_t timeoutUs = (uint32_t)MAX_DISTANCE_CM * 58UL + 2000UL;

    unsigned long duration = pulseIn(ECHO_PIN, HIGH, timeoutUs);

    if (duration > 0) {
      // Convert to distance in cm: duration(us) / 58.2 (round trip)
      uint16_t cm = (uint16_t)(duration / 58UL);
      sum += cm;
      validCount++;
    }

    delay(20);
  }

  if (validCount == 0) return MAX_DISTANCE_CM; // fail-safe
  uint16_t avg = (uint16_t)(sum / validCount);
  if (avg > MAX_DISTANCE_CM) avg = MAX_DISTANCE_CM;
  return avg;
}

// Map distance to PWM brightness: nearer -> higher brightness.
// We'll use inverse-linear mapping: brightness = (1 - d/MAX) * 255
uint8_t brightnessFromDistance(uint16_t distCm) {
  if (distCm >= MAX_DISTANCE_CM) return 0;
  // Keep dist 0..MAX-1
  uint32_t d = distCm;
  uint32_t maxd = MAX_DISTANCE_CM;
  // brightness = (maxd - d) / maxd * 255
  uint32_t b = (maxd - d) * 255UL / maxd;
  if (b > 255) b = 255;
  return (uint8_t)b;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_D13, OUTPUT);
  pinMode(PWM_D9, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_D13, LOW);
  analogWrite(PWM_D9, 0);

  tm.begin();
  tm.setBrightness(6); // reasonable default brightness
}

void loop() {
  uint16_t distCm = measureDistanceCm();

  // Display distance on TM1637 as a number (cm)
  tm.displayNumber((int)distCm);

  // D13 near-distance indicator
  digitalWrite(LED_D13, (distCm <= NEAR_DISTANCE_CM) ? HIGH : LOW);

  // PWM brightness based on distance (nearer -> brighter)
  uint8_t pwm = brightnessFromDistance(distCm);
  analogWrite(PWM_D9, pwm);

  delay(80);
}
