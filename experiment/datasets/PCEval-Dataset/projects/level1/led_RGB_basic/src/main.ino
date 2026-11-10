/**
  Turns on a RGB LED connected to a specified pin.
  3 APR 2025 by Blind Author
*/

const int R = 4;
const int G = 3;
const int B = 2;

void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

  analogWrite(R, 255);
  analogWrite(G, 255);
  analogWrite(B, 0);
}

void loop() {
}
