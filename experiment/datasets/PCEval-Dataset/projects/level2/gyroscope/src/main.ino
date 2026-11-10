/**
  Measure 3-axis rotation with gyroscope
  10 APR 2025 by Blind Author
*/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup(void) {
    Serial.begin(115200);

    while (!mpu.begin()) {
        Serial.println("MPU6050 not connected!");
        delay(1000);
    }
    Serial.println("MPU6050 ready!");
}

sensors_event_t event;

void loop() {
    mpu.getGyroSensor()->getEvent(&event);

    Serial.print("X: ");
    Serial.print(event.gyro.x);
    Serial.print(", Y: ");
    Serial.print(event.gyro.y);
    Serial.print(", Z: ");
    Serial.print(event.gyro.z);
    Serial.println(" rad/s");
    delay(500);
}