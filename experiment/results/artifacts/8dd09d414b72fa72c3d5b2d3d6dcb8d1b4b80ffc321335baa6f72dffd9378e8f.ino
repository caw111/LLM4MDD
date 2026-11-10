#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  while (!Serial) { /* wait for serial */ }

  Wire.begin(); // Uses A4 (SDA) and A5 (SCL) by default on Uno

  if (!mpu.begin()) {
    Serial.println("MPU6050 not ready!");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("MPU6050 ready!");
}

void loop() {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;

  mpu.getEvent(&accel, &gyro, &temp);

  // Adafruit_MPU6050 returns gyro in rad/s (per library docs).
  Serial.print("X: ");
  Serial.print(gyro.gyro.x, 2);
  Serial.print(", Y: ");
  Serial.print(gyro.gyro.y, 2);
  Serial.print(", Z: ");
  Serial.print(gyro.gyro.z, 2);
  Serial.println(" rad/s");

  delay(500);
}