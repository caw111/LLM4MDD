#include <Arduino.h>

static const uint8_t muxSIG = A0; // COM/SIG from CD74HC4067 to Arduino A0

// Control pins S0..S3 connected to Arduino D2..D5
static const uint8_t muxS0 = 2;
static const uint8_t muxS1 = 3;
static const uint8_t muxS2 = 4;
static const uint8_t muxS3 = 5;

static const uint8_t channelCount = 6; // I0..I5 used (photoresistor1..6)

void setup() {
  pinMode(muxS0, OUTPUT);
  pinMode(muxS1, OUTPUT);
  pinMode(muxS2, OUTPUT);
  pinMode(muxS3, OUTPUT);

  Serial.begin(9600);
}

static void selectMuxChannel(uint8_t ch) {
  // ch: 0..15; we use 0..5
  digitalWrite(muxS0, (ch & 0x01) ? HIGH : LOW);
  digitalWrite(muxS1, (ch & 0x02) ? HIGH : LOW);
  digitalWrite(muxS2, (ch & 0x04) ? HIGH : LOW);
  digitalWrite(muxS3, (ch & 0x08) ? HIGH : LOW);
}

void loop() {
  for (uint8_t ch = 0; ch < channelCount; ch++) {
    selectMuxChannel(ch);
    // small settling delay to allow mux switching before reading
    delay(2);

    int lightValue = analogRead(muxSIG);
    Serial.print("Sensor ");
    Serial.print(ch);
    Serial.print(": ");
    Serial.println(lightValue);
  }
}