/**
  Read 6 LDR sensor values with 16 channel analog multiplexer
  03 MAY 2025 by Blind Author
*/

const int s0 = 2, s1 = 3, s2 = 4, s3 = 5;
const int SIG = A0;

int selectPins[4] = {s0, s1, s2, s3};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(selectPins[i], OUTPUT);
  }
}

int readMux(int channel) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(selectPins[i], (channel >> i) & 1);
  }
  delay(100);
  return analogRead(SIG);
}

void loop() {
  for (int i = 0; i < 6; i++) {
    int lightValue = readMux(i);
    Serial.print("sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(lightValue);
  }
  delay(1000);
}