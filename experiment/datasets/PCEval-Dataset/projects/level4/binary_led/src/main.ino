/**
  Use 5 LEDs to display the binary form of a number received over serial
  09 MAY 2025 by Blind Author
*/

const int ledPins[5] = {2, 3, 4, 5, 6};

void setup() {
    Serial.begin(9600);
    for (int i = 0; i < 5; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
}

void loop() {
    if (Serial.available() > 0) {
        int number = Serial.parseInt();

        while (Serial.available()) {
            Serial.read();
        }

        for (int i = 0; i < 5; i++) {
            int bitValue = (number >> i) & 1;
            digitalWrite(ledPins[i], bitValue);
        }

        Serial.print("Binary of ");
        Serial.print(number);
        Serial.print(" is: ");
        for (int i = 4; i >= 0; i--) {
            Serial.print((number >> i) & 1);
        }
        Serial.println();
    }
}
