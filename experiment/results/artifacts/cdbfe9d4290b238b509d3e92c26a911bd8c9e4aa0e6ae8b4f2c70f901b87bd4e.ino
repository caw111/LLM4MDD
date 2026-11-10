// Reads LM35 temperature sensor connected to A2 and outputs Celsius via Serial.

void setup() {
  Serial.begin(9600);
}

void loop() {
  // LM35: 10mV per °C => 0.01 V/°C
  // Arduino UNO ADC: 5V reference, 10-bit ADC => 4.88mV per count
  int adc = analogRead(A2);
  float voltage = (adc * 5.0f) / 1023.0f;     // Volts
  float tempC = voltage * 100.0f;            // (V) * (100 °C/V) => °C

  Serial.println(tempC, 2); // Celsius value with 2 decimal places
}