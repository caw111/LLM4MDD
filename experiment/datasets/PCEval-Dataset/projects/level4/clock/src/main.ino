/**
  Show current time in 4-digit 7-segment display
  04 May 2025 by Blind Author
*/

#include "RTClib.h"

RTC_DS1307 rtc;

const int digitPins[4] = {2, 3, 4, 5};
const int segmentPins[7] = {6, 7, 8, 9, 10, 11, 12};

const byte digits[10][7] = {
  {1,1,1,1,1,1,0}, 
  {0,1,1,0,0,0,0}, 
  {1,1,0,1,1,0,1},
  {1,1,1,1,0,0,1}, 
  {0,1,1,0,0,1,1}, 
  {1,0,1,1,0,1,1},
  {1,0,1,1,1,1,1},
  {1,1,1,0,0,0,0},
  {1,1,1,1,1,1,1},
  {1,1,1,1,0,1,1}
};

void setup() {
  Serial.begin(9600);
  for (int i=0; i<4; i++) {
    pinMode(digitPins[i], OUTPUT);
  }

  for (int i=0; i<7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
}

void loop() {
  DateTime now = rtc.now();

  int times[4] = {
    now.hour() / 10,
    now.hour() % 10,
    now.minute() / 10,
    now.minute() % 10
  };

  for (int i=0; i<4; i++) {
    for (int j=0; j<7; j++) {
      digitalWrite(segmentPins[j], digits[times[i]][j] ? LOW : HIGH);
    }
    digitalWrite(digitPins[i], HIGH);
    delay(5);
    digitalWrite(digitPins[i], LOW);
  }

}