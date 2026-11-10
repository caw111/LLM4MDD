#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_022\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t TRIG_PIN = 8;
constexpr uint8_t ECHO_PIN = 9;
constexpr uint8_t CLK_PIN  = 4;
constexpr uint8_t DIO_PIN  = 5;
constexpr uint8_t LED_PIN  = 13;

constexpr unsigned long SAMPLE_INTERVAL_MS = 80;
constexpr unsigned long ECHO_TIMEOUT_US = 25000;
constexpr unsigned long MIN_VALID_ECHO_US = 2UL * 58UL;
constexpr unsigned long MAX_VALID_ECHO_US = 400UL * 58UL;
constexpr unsigned long LED_THRESHOLD_US = 10UL * 58UL;

const uint8_t DIGIT_SEGMENTS[10] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66,
  0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

unsigned long lastSampleStartMs = 0;

#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_022\\direct\\attempt_0\\sketch\\sketch.ino"
void tmDelay();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_022\\direct\\attempt_0\\sketch\\sketch.ino"
void tmStart();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_022\\direct\\attempt_0\\sketch\\sketch.ino"
void tmStop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_022\\direct\\attempt_0\\sketch\\sketch.ino"
void tmWriteByte(uint8_t value);
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_022\\direct\\attempt_0\\sketch\\sketch.ino"
void displaySegments(const uint8_t segments[4]);
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_022\\direct\\attempt_0\\sketch\\sketch.ino"
void displayBlank();
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_022\\direct\\attempt_0\\sketch\\sketch.ino"
void displayDistance(uint16_t centimeters);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_022\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_022\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_022\\direct\\attempt_0\\sketch\\sketch.ino"
void tmDelay() {
  delayMicroseconds(5);
}

void tmStart() {
  digitalWrite(DIO_PIN, HIGH);
  digitalWrite(CLK_PIN, HIGH);
  tmDelay();
  digitalWrite(DIO_PIN, LOW);
  tmDelay();
  digitalWrite(CLK_PIN, LOW);
}

void tmStop() {
  digitalWrite(CLK_PIN, LOW);
  digitalWrite(DIO_PIN, LOW);
  tmDelay();
  digitalWrite(CLK_PIN, HIGH);
  tmDelay();
  digitalWrite(DIO_PIN, HIGH);
  tmDelay();
}

void tmWriteByte(uint8_t value) {
  for (uint8_t bit = 0; bit < 8; ++bit) {
    digitalWrite(CLK_PIN, LOW);
    digitalWrite(DIO_PIN, (value & 0x01) ? HIGH : LOW);
    tmDelay();
    digitalWrite(CLK_PIN, HIGH);
    tmDelay();
    digitalWrite(CLK_PIN, LOW);
    value >>= 1;
  }

  // Release DIO for the display's acknowledgement clock.
  pinMode(DIO_PIN, INPUT_PULLUP);
  tmDelay();
  digitalWrite(CLK_PIN, HIGH);
  tmDelay();
  digitalWrite(CLK_PIN, LOW);
  tmDelay();
  digitalWrite(DIO_PIN, LOW);
  pinMode(DIO_PIN, OUTPUT);
}

void displaySegments(const uint8_t segments[4]) {
  tmStart();
  tmWriteByte(0x40);  // Write data with automatic address increment.
  tmStop();

  tmStart();
  tmWriteByte(0xC0);
  for (uint8_t i = 0; i < 4; ++i) {
    tmWriteByte(segments[i]);
  }
  tmStop();

  tmStart();
  tmWriteByte(0x8F);  // Display enabled, maximum brightness.
  tmStop();
}

void displayBlank() {
  const uint8_t blank[4] = {0, 0, 0, 0};
  displaySegments(blank);
}

void displayDistance(uint16_t centimeters) {
  uint8_t segments[4] = {0, 0, 0, 0};
  int8_t position = 3;

  do {
    segments[position--] = DIGIT_SEGMENTS[centimeters % 10];
    centimeters /= 10;
  } while (centimeters != 0 && position >= 0);

  displaySegments(segments);
}

void setup() {
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(CLK_PIN, HIGH);
  digitalWrite(DIO_PIN, HIGH);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(DIO_PIN, OUTPUT);

  displayBlank();
  lastSampleStartMs = millis() - SAMPLE_INTERVAL_MS;
}

void loop() {
  const unsigned long now = millis();
  if (now - lastSampleStartMs < SAMPLE_INTERVAL_MS) {
    return;
  }
  lastSampleStartMs = now;

  // A high ECHO before triggering cannot be a valid new response.
  const bool echoInitiallyLow = (digitalRead(ECHO_PIN) == LOW);

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Timeout also handles an absent echo or an ECHO line stuck high.
  const unsigned long echoUs =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  const bool valid =
      echoInitiallyLow &&
      echoUs != 0 &&
      echoUs >= MIN_VALID_ECHO_US &&
      echoUs <= MAX_VALID_ECHO_US;

  if (!valid) {
    digitalWrite(LED_PIN, LOW);
    displayBlank();
    return;
  }

  // Compare the unrounded measurement, not the displayed integer.
  digitalWrite(LED_PIN, echoUs < LED_THRESHOLD_US ? HIGH : LOW);

  const uint16_t centimeters =
      static_cast<uint16_t>((echoUs + 29UL) / 58UL);
  displayDistance(centimeters);
}
