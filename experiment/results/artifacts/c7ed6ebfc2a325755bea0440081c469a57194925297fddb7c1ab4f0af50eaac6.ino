void setup() {
  Serial.begin(9600);
}

void loop() {
  static String line;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    // Framing: line_or_idle
    // Accumulate until newline, otherwise handle idle timeout.
    if (c == '\n' || c == '\r') {
      if (line.length() > 0) {
        Serial.print("Received: ");
        Serial.println(line);
        line = "";
      }
    } else {
      line += c;
    }
  }

  // Idle timeout handling: if no new data arrives for 20ms, echo whatever we have.
  static unsigned long lastByteTime = 0;
  if (Serial.available() > 0) {
    lastByteTime = millis();
  }

  if (line.length() > 0) {
    unsigned long now = millis();
    if ((now - lastByteTime) >= 20) { // idle_timeout_ms=20
      Serial.print("Received: ");
      Serial.println(line);
      line = "";
    }
  }
}