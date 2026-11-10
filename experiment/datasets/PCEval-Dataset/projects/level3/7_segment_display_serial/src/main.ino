/**
  Show the serial input with one digit 7-segment display
  7 APR 2025 by Blind Author
*/

const int segmentPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

const byte digits[10][8] = {
    {0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 1, 1, 1, 1, 1},
    {0, 0, 1, 0, 0, 1, 0, 1},
    {0, 0, 0, 0, 1, 1, 0, 1}, 
    {1, 0, 0, 1, 1, 0, 0, 1},
    {0, 1, 0, 0, 1, 0, 0, 1},
    {0, 1, 0, 0, 0, 0, 0, 1}, 
    {0, 0, 0, 1, 1, 1, 1, 1}, 
    {0, 0, 0, 0, 0, 0, 0, 1}, 
    {0, 0, 0, 0, 1, 0, 0, 1} 
};

void setup() {
    for (int i = 0; i < 8; i++) {
        pinMode(segmentPins[i], OUTPUT);
        digitalWrite(segmentPins[i], HIGH);
    }
    Serial.begin(9600);
}

void loop() {
    if (Serial.available()) {
        char input = Serial.read();
        if (input >= '0' && input <= '9') {
        int num = input - '0';
        displayDigit(num);
        }
    }
}

void displayDigit(int num) {
    for (int i = 0; i < 8; i++) {
        digitalWrite(segmentPins[i], digits[num][i] ? HIGH : LOW);
    }
}


