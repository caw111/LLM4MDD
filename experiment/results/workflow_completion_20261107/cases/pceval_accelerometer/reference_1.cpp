#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
bool sensorReady = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  sensorReady = mpu.begin();
  if (sensorReady) {
    Serial.println("MPU6050 ready!");
  }
}

void loop() {
  if (sensorReady) {
    sensors_event_t acceleration;
    sensors_event_t gyro;
    sensors_event_t temperature;
    mpu.getEvent(&acceleration, &gyro, &temperature);
    Serial.print("X: ");
    Serial.print(acceleration.acceleration.x, 2);
    Serial.print(", Y: ");
    Serial.print(acceleration.acceleration.y, 2);
    Serial.print(", Z: ");
    Serial.print(acceleration.acceleration.z, 2);
    Serial.println(" m/s^2");
  }
  delay(500);
}
