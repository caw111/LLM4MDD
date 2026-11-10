/**
  Use one digit 7-segment display to make a counter
  7 APR 2025 by Blind Author
*/

const int segA = 2;
const int segB = 3;
const int segC = 4;
const int segD = 5;
const int segE = 6;
const int segF = 7;
const int segG = 8;
const int segDP = 9;

unsigned long startTime;

void setup() {
    pinMode(segA, OUTPUT);
    pinMode(segB, OUTPUT);
    pinMode(segC, OUTPUT);
    pinMode(segD, OUTPUT);
    pinMode(segE, OUTPUT);
    pinMode(segF, OUTPUT);
    pinMode(segG, OUTPUT);
    pinMode(segDP, OUTPUT);

    startTime = millis();
}

void loop() {
    unsigned long currentTime = millis() - startTime;

    if (currentTime >= 1000 && currentTime < 2000) {
        digitalWrite(segA, HIGH);
        digitalWrite(segB, LOW);
        digitalWrite(segC, LOW);
        digitalWrite(segD, HIGH);
        digitalWrite(segE, HIGH);
        digitalWrite(segF, HIGH);
        digitalWrite(segG, HIGH);
        digitalWrite(segDP, HIGH);
    } else if (currentTime >= 2000 && currentTime < 3000) {
        digitalWrite(segA, LOW);
        digitalWrite(segB, LOW);
        digitalWrite(segC, HIGH);
        digitalWrite(segD, LOW);
        digitalWrite(segE, LOW);
        digitalWrite(segF, HIGH);
        digitalWrite(segG, LOW);
        digitalWrite(segDP, HIGH);
    } else if (currentTime >= 3000 && currentTime < 4000) {
        digitalWrite(segA, LOW);
        digitalWrite(segB, LOW);
        digitalWrite(segC, LOW);
        digitalWrite(segD, LOW);
        digitalWrite(segE, HIGH);
        digitalWrite(segF, HIGH);
        digitalWrite(segG, LOW);
        digitalWrite(segDP, HIGH);
    }
}

