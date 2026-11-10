#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_photoresistor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t muxSIG = A0; // COM/SIG from CD74HC4067 to Arduino A0

// Control pins S0..S3 connected to Arduino D2..D5
static const uint8_t muxS0 = 2;
static const uint8_t muxS1 = 3;
static const uint8_t muxS2 = 4;
static const uint8_t muxS3 = 5;

static const uint8_t channelCount = 6; // I0..I5 used (photoresistor1..6)

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_photoresistor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_photoresistor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void selectMuxChannel(uint8_t ch);
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_photoresistor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_photoresistor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
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
