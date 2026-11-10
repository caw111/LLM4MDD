#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  while (!Serial) { /* wait for serial (optional on Uno) */ }

  if (mpu.begin()) {
    Serial.println("MPU6050 ready!");
  } else {
    // If initialization fails, keep behavior deterministic and do not crash.
    Serial.println("Failed to initialize MPU6050");
  }
}

void loop() {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;

  mpu.getEvent(&accel, &gyro, &temp);

  // Print acceleration in m/s^2 as requested.
  Serial.print("X: ");
  Serial.print(accel.acceleration.x, 2);
  Serial.print(", Y: ");
  Serial.print(accel.acceleration.y, 2);
  Serial.print(", Z: ");
  Serial.print(accel.acceleration.z, 2);
  Serial.println(" m/s^2");

  delay(500);
}