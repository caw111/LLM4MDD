#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiplexer_potentiometer\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t MUX_EN = 10; // Arduino digital pin 10 unused in wiring, will be overridden below
// Wiring per circuit:
// S0->2, S1->3, S2->4, S3->5, COM->A0, EN->GND.2, GND->GND.2, VCC->5V

const uint8_t S0_PIN = 2;   // multiplexer S0
const uint8_t S1_PIN = 3;   // multiplexer S1
const uint8_t S2_PIN = 4;   // multiplexer S2
const uint8_t S3_PIN = 5;   // multiplexer S3
const uint8_t MUX_COM = A0;  // multiplexer COM

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiplexer_potentiometer\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void selectMuxChannel(uint8_t channel);
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiplexer_potentiometer\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiplexer_potentiometer\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiplexer_potentiometer\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void selectMuxChannel(uint8_t channel) {
  // CD4067 uses S0..S3 to select channels I0..I15.
  // We only use channels 0..4.
  channel &= 0x0F;

  digitalWrite(S0_PIN, (channel & 0x01) ? HIGH : LOW);
  digitalWrite(S1_PIN, (channel & 0x02) ? HIGH : LOW);
  digitalWrite(S2_PIN, (channel & 0x04) ? HIGH : LOW);
  digitalWrite(S3_PIN, (channel & 0x08) ? HIGH : LOW);
}

void setup() {
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);

  // Initialize Serial Monitor for communication
  Serial.begin(9600);

  // Ensure a deterministic starting selection
  selectMuxChannel(0);
}

void loop() {
  for (uint8_t channel = 0; channel <= 4; channel++) {
    selectMuxChannel(channel);

    // Small settling time for the analog multiplexer output
    delayMicroseconds(50);

    int value = analogRead(MUX_COM);

    Serial.print("Sensor ");
    Serial.print(channel);
    Serial.print(": ");
    Serial.println(value);
  }
}
