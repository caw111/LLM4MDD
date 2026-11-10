/**
  Press the button to change the timezone
  02 MAY 2025 by Blind Author
*/

#include "RTClib.h"

RTC_DS1307 rtc;

const int BUTTONS[4] = {2, 3, 4, 5};
const int TIMEZONE[4] = {9, 4, -10, -5};

void setup () {
  Serial.begin(115200);

  for (int i=0; i<4; i++) {
    pinMode(BUTTONS[i], INPUT);
  }

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
}

void loop () {
  DateTime now = rtc.now();

  for (int i=0; i<4; i++) {
    if (digitalRead(BUTTONS[i]) == HIGH) {
      now = now + TimeSpan(0, TIMEZONE[i], 0, 0);
      break;
    }
  }

  Serial.print(now.timestamp());

  Serial.println();
  delay(500);
}
