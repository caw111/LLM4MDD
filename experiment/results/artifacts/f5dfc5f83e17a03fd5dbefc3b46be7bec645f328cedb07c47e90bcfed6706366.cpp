#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiplexer_photoresistor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t MUX_SIG_PIN = A0; // COM -> A0
static const uint8_t S0_PIN = 2;       // S0 -> D2
static const uint8_t S1_PIN = 3;       // S1 -> D3
static const uint8_t S2_PIN = 4;       // S2 -> D4
static const uint8_t S3_PIN = 5;       // S3 -> D5

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiplexer_photoresistor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setMuxChannel(uint8_t ch);
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiplexer_photoresistor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiplexer_photoresistor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_multiplexer_photoresistor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setMuxChannel(uint8_t ch)
{
  // CD74HC4067 uses S3..S0 as address lines for channels 0..15
  digitalWrite(S0_PIN, (ch >> 0) & 0x01);
  digitalWrite(S1_PIN, (ch >> 1) & 0x01);
  digitalWrite(S2_PIN, (ch >> 2) & 0x01);
  digitalWrite(S3_PIN, (ch >> 3) & 0x01);
}

void setup()
{
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);

  Serial.begin(9600);

  // Initialize to channel 0 (optional, but deterministic)
  setMuxChannel(0);
  delay(5);
}

void loop()
{
  for (uint8_t channel = 0; channel <= 5; channel++)
  {
    setMuxChannel(channel);
    delay(2); // allow mux switching to settle

    int lightValue = analogRead(MUX_SIG_PIN);
    Serial.print("Sensor ");
    Serial.print(channel);
    Serial.print(": ");
    Serial.println(lightValue);
  }
}
