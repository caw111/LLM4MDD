#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_013\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_013\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_013\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_013\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float tempC = dht.readTemperature(); // temperature in Celsius

  // If reading fails, DHT library returns NaN; do not simplify behavior:
  // output the value (NaN prints as "nan" by Serial).
  Serial.println(tempC);

  delay(1000); // every second
}
