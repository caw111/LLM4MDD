#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_gyroscope\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_gyroscope\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_gyroscope\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_gyroscope\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);

  // Arduino Uno hardware I2C: SDA = A4, SCL = A5.
  // Connect MPU6050 VCC to 3.3V and GND to the wired Uno GND.
  Wire.begin();

  if (!mpu.begin()) {
    Serial.println("Failed to initialize MPU6050!");
    while (true) {
      delay(500);
    }
  }

  Serial.println("MPU6050 ready!");
}

void loop() {
  sensors_event_t acceleration;
  sensors_event_t gyro;
  sensors_event_t temperature;

  mpu.getEvent(&acceleration, &gyro, &temperature);

  // Adafruit Unified Sensor gyroscope readings are in rad/s.
  Serial.print("X: ");
  Serial.print(gyro.gyro.x, 2);
  Serial.print(", Y: ");
  Serial.print(gyro.gyro.y, 2);
  Serial.print(", Z: ");
  Serial.print(gyro.gyro.z, 2);
  Serial.println(" rad/s");

  delay(500);
}
