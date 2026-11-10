#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t S0_PIN = 2;
static const uint8_t S1_PIN = 3;
static const uint8_t S2_PIN = 4;
static const uint8_t S3_PIN = 5;
static const uint8_t ANALOG_PIN = A0;

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void selectChannel(uint8_t channel);
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);

  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);

  // Ensure a known state at startup
  digitalWrite(S0_PIN, LOW);
  digitalWrite(S1_PIN, LOW);
  digitalWrite(S2_PIN, LOW);
  digitalWrite(S3_PIN, LOW);
}

static void selectChannel(uint8_t channel) {
  // CD74HC4067 uses S0..S3 to select I0..I15.
  // Channel is expected to be 0..15.
  digitalWrite(S0_PIN, (channel & 0x01) ? HIGH : LOW);
  digitalWrite(S1_PIN, (channel & 0x02) ? HIGH : LOW);
  digitalWrite(S2_PIN, (channel & 0x04) ? HIGH : LOW);
  digitalWrite(S3_PIN, (channel & 0x08) ? HIGH : LOW);
}

void loop() {
  for (uint8_t channel = 0; channel <= 4; channel++) {
    selectChannel(channel);

    // Allow the analog mux to settle before reading
    delayMicroseconds(10);

    int value = analogRead(ANALOG_PIN);
    Serial.print("Sensor ");
    Serial.print(channel);
    Serial.print(": ");
    Serial.println(value);
  }
}
