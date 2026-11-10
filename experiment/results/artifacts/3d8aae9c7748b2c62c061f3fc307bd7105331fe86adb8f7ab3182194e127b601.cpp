#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const float threshold = 30.0f;

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_temperature_sensor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);

  // Declare the threshold value
  Serial.println(threshold);

  dht.begin();
}

void loop() {
  // Continuously read temperature
  float tempC = dht.readTemperature(); // Celsius

  // If reading fails, retry without printing compare result
  if (isnan(tempC)) {
    return;
  }

  // Compare and print result
  if (tempC < threshold) {
    Serial.println("Temperature is below the threshold");
  } else if (tempC > threshold) {
    Serial.println("Temperature is above the threshold");
  } else {
    // Exact equality: not specified; keep deterministic behavior
    Serial.println("Temperature is above the threshold");
  }
}
