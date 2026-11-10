#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>

static const uint8_t DHT_PIN = 2;        // DHT22 SDA -> Arduino Uno digital pin 2
static const uint8_t DHT_TYPE = DHT22;   // DHT22 sensor type

DHT dht(DHT_PIN, DHT_TYPE);

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);

  dht.begin();
}

void loop() {
  float temperatureC = dht.readTemperature(); // Celsius by default

  // If reading fails, dht.readTemperature() returns NAN.
  // Still print according to the required format.
  Serial.print("Current temperature: ");
  Serial.println(temperatureC);

  delay(2000);
}
