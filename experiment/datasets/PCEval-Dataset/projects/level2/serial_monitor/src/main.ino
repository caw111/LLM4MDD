/**
  Read and write to serial monitor
  1 APR 2025 by Blind Author
*/

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available()) {
        char ch = Serial.read();
        Serial.print("Received: ");
        Serial.println(ch);
    }
}