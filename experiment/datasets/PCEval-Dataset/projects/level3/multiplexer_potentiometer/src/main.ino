/**
  Read multiple potentiometer value with analog multiplexer
  05 MAY 2025 by Blind Author
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

void loop() {
  for (int i = 0; i < 5; i++) {
    selectMuxChannel(i);
    delay(500);

    int sensorReading = analogRead(SIG);
    Serial.print("sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(sensorReading);
  }
}

void selectMuxChannel(int channel) {
  digitalWrite(s0, bitRead(channel, 0));
  digitalWrite(s1, bitRead(channel, 1));
  digitalWrite(s2, bitRead(channel, 2));
  digitalWrite(s3, bitRead(channel, 3));
}