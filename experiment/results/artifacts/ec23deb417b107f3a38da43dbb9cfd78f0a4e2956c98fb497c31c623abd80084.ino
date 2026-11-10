#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// 7-segment wiring (common cathode)
const uint8_t SEG_PINS[7] = {
  5,  // A
  3,  // B
  2,  // C
  4,  // D
  8,  // E
  7,  // F
  6   // G
};
const uint8_t COM_PIN = 10; // Not used; COM is tied to gnd2 in circuit (can't control)

static const uint8_t digitToSegments[10] = {
  // A, B, C, D, E, F, G (1 = segment ON)
  0b1111110, // 0: A B C D E F on, G off
  0b0110000, // 1: A off, B C on, D E F G off
  0b1101101, // 2
  0b1111001, // 3
  0b0110011, // 4
  0b1011011, // 5
  0b1011111, // 6
  0b1110000, // 7
  0b1111111, // 8
  0b1111011  // 9
};

// Map bits to the segment pins order above.
// We'll treat digitToSegments bit positions as: bit6=A, bit5=B, bit4=C, bit3=D, bit2=E, bit1=F, bit0=G.
static inline uint8_t maskBitAtoG(uint8_t val, int bitIndexAtoG) {
  // bitIndexAtoG: 0..6 corresponds to A..G
  // A is bit6, B bit5 ... G bit0
  return (val >> (6 - bitIndexAtoG)) & 0x01;
}

static inline void displayDigit(uint8_t d) {
  // common cathode: segments should be driven HIGH to turn ON
  uint8_t segVal = digitToSegments[d % 10];

  // A..G corresponding to SEG_PINS: A,B,C,D,E,F,G
  for (int i = 0; i < 7; i++) {
    uint8_t on = maskBitAtoG(segVal, i);
    digitalWrite(SEG_PINS[i], on ? HIGH : LOW);
  }
}

int mainCounter = 0;
bool canIncrement = true;

void setup() {
  // 1. Set pins for 7-segments as OUTPUT
  for (int i = 0; i < 7; i++) {
    pinMode(SEG_PINS[i], OUTPUT);
    digitalWrite(SEG_PINS[i], LOW);
  }

  // 2. Check the connection of mpu6050
  Wire.begin();
  mpu.initialize();

  // If MPU is not connected, this sketch will keep running,
  // but won't update count correctly. We still follow requirement to check connection.
  // (No requirement given for how to indicate failure.)
  // Typical check:
  if (!mpu.testConnection()) {
    // Leave count at 0 and display 0 as required.
  }

  // 3. Display number 4? Requirement says display number 0 on power-up
  mainCounter = 0;
  canIncrement = true;
  displayDigit(0);
}

void loop() {
  // 4. Continuously read the Y rotation value of mpu6050
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // The requirement says "Y-axis rotation value" and threshold 15000/2000.
  // MPU6050 gyroscope raw values are int16_t typically; still apply the thresholds as specified.
  int32_t yRot = (int32_t)gy;

  // 5. Count increases when absolute Y exceeds 15000,
  //    and it does not increase again until abs drops below 2000.
  int32_t absY = (yRot >= 0) ? yRot : -yRot;

  if (canIncrement) {
    if (absY > 15000) {
      mainCounter++;
      canIncrement = false;
    }
  } else {
    if (absY < 2000) {
      canIncrement = true;
    }
  }

  // 6. Continuously display the count on the 1-digit 7-segment display
  displayDigit((uint8_t)(mainCounter % 10));

  delay(10);
}