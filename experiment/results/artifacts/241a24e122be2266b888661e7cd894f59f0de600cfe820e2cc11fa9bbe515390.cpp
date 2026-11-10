#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision38\\cases\\pceval_accelerometer\\arduino\\sketch\\sketch.ino"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision38\\cases\\pceval_accelerometer\\arduino\\sketch\\sketch.ino"
void setup();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision38\\cases\\pceval_accelerometer\\arduino\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\manual_code_repairs_20261106\\revision38\\cases\\pceval_accelerometer\\arduino\\sketch\\sketch.ino"
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
