/**
  Motion counter that detects movement with an MPU6050 gyroscope and displays the count on a 1-digit 7-segment display
  03 MAY 2025 by Blind Author
*/

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

int count = 0;
bool moved = false;

const int segmentPins[7] = {2, 3, 4, 5, 6, 7, 8};

const byte digits[10][7] = {
  {1,1,1,1,1,1,0}, 
  {0,1,1,0,0,0,0}, 
  {1,1,0,1,1,0,1}, 
  {1,1,1,1,0,0,1},
  {0,1,1,0,0,1,1},
  {1,0,1,1,1,1,1}, 
  {1,1,1,0,0,0,0}, 
  {1,1,1,1,1,1,1}, 
  {1,1,1,1,0,1,1}  
};

void setup() {
  Wire.begin();
  mpu.initialize();

  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 failed!");
    while (1);
  }

  displayDigit(0);
}

void loop() {
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);

  if (abs(gy) > 15000 && !moved) {
    count++;
    if (count > 9999) count = 0;

    displayDigit(count % 10);
    moved = true;
  }

  if (abs(gy) < 2000) {
    moved = false;
  }

  delay(50);
}

void displayDigit(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], digits[num][i]);
  }
}
