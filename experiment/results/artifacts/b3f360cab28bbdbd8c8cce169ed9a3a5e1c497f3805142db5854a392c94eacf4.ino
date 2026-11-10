#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600);

  // Arduino Uno hardware I2C: SDA = A4, SCL = A5.
  // Sensor power: VCC = 3.3V, GND = Arduino GND.
  Wire.begin();

  while (!mpu.begin()) {
    Serial.println("Failed to initialize MPU6050. Check wiring.");
    delay(500);
  }

  Serial.println("MPU6050 ready!");
}

void loop() {
  sensors_event_t acceleration;
  sensors_event_t gyro;
  sensors_event_t temperature;

  mpu.getEvent(&acceleration, &gyro, &temperature);

  // Adafruit Unified Sensor acceleration values are already in m/s^2.
  Serial.print("X: ");
  Serial.print(acceleration.acceleration.x, 2);
  Serial.print(", Y: ");
  Serial.print(acceleration.acceleration.y, 2);
  Serial.print(", Z: ");
  Serial.print(acceleration.acceleration.z, 2);
  Serial.println(" m/s^2");

  delay(500);
}