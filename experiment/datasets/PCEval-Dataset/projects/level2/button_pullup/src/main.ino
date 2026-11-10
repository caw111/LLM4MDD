/**
  Manage pull up registor button input
  1 APR 2025 by Blind Author
*/

const int inputPin = 4;

void setup()
{
    Serial.begin(9600);
    pinMode(inputPin, INPUT);
}

void loop()
{
    int val = digitalRead(inputPin);

    if (val == LOW)
    {
        Serial.println("Button pressed");
    } else
    {
        Serial.println("Button not pressed");
    }

    delay(1000);
}