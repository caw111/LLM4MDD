/**
  Show 1, 2, 3, and 4 with 4 digit 7-segment display
  7 APR 2025 by Blind Author
*/

const int segmentPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
const int digitPins[4] = {10, 11, 12, 13};

const byte digits[10][8] = {
  {1,1,1,1,1,1,0,0},  
  {0,1,1,0,0,0,0,0},  
  {1,1,0,1,1,0,1,0},  
  {1,1,1,1,0,0,1,0},  
  {0,1,1,0,0,1,1,0},  
  {1,0,1,1,0,1,1,0},  
  {1,0,1,1,1,1,1,0},
  {1,1,1,0,0,0,0,0},
  {1,1,1,1,1,1,1,0},
  {1,1,1,1,0,1,1,0}
};

void setup() {
  for (int i = 0; i < 8; i++) pinMode(segmentPins[i], OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], LOW);
  }
}

void loop() {
  int nums[4] = {1, 2, 3, 4};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 8; j++) digitalWrite(segmentPins[j], digits[nums[i]][j] ? LOW : HIGH);
    digitalWrite(digitPins[i], HIGH);
    delay(800);
    digitalWrite(digitPins[i], LOW);
  }
}