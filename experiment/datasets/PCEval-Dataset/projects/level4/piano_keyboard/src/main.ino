/**
  Creating a keyboard using push buttons and buzzers.
  02 MAY 2025 by Blind Author
*/

const int BUTTONS[4] = {2, 3, 4, 5};
const int BUZZERS[4] = {9, 10, 11, 12};
const int TONES[4] = {262, 294, 330, 349};

int lastPressed = -1;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(BUTTONS[i], INPUT);
    pinMode(BUZZERS[i], OUTPUT);
  }
}

void loop() {
  int pressed = -1;
  for (int i = 0; i < 4; i++) {
    if (digitalRead(BUTTONS[i]) == HIGH) {
      pressed = i;
      break;
    }
  }

  if (pressed != lastPressed) {
    if (lastPressed != -1) noTone(BUZZERS[lastPressed]);
    if (pressed != -1) tone(BUZZERS[pressed], TONES[pressed]);

    lastPressed = pressed;
  }
}
