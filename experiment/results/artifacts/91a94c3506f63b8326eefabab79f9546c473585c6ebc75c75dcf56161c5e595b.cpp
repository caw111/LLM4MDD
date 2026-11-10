#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_gyroscope\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_gyroscope\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_gyroscope\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_gyroscope\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial port to connect (needed for some boards)
  }

  Wire.begin(); // uses SDA=A4, SCL=A5 by default on Arduino Uno

  if (!mpu.begin()) {
    Serial.println("MPU6050 not ready!");
    while (true) {
      delay(500);
    }
  }

  Serial.println("MPU6050 ready!");
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Gyroscope values are in rad/s
  Serial.print("X: ");
  Serial.print(g.gyro.x, 2);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y, 2);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z, 2);
  Serial.println(" rad/s");

  delay(500);
}
