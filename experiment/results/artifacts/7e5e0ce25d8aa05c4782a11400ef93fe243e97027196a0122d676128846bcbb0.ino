// Parking Spot Monitor
// Ultrasonic sensor + RGB LED (common cathode)

const int TRIG_PIN = 8;
const int ECHO_PIN = 9;

// RGB LED pins through 220 ohm resistors (Arduino pins 4,3,2)
const int RED_PIN   = 4; // R -> resistor1 -> pin4
const int GREEN_PIN = 3; // G -> resistor2 -> pin3
const int BLUE_PIN  = 2; // B -> resistor3 -> pin2

const int DIST_THRESHOLD_CM = 50;

void setup() {
  // Requirement 1: set pin8, pin2~pin4 as OUTPUT, and pin9 as INPUT
  pinMode(TRIG_PIN, OUTPUT);   // pin8
  pinMode(ECHO_PIN, INPUT);    // pin9

  pinMode(RED_PIN, OUTPUT);    // pin4
  pinMode(GREEN_PIN, OUTPUT);  // pin3
  pinMode(BLUE_PIN, OUTPUT);   // pin2

  // Ensure LED is off initially (common cathode: HIGH turns off, LOW turns on)
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
}

static long readDistanceCm() {
  // Requirement 2: continuously trigger and read the distance sensor value
  // HC-SR04: send a 10us pulse to TRIG, measure pulse width on ECHO.
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width (microseconds)
  // Timeout to avoid getting stuck if no echo.
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL); // up to ~5m

  if (duration == 0) {
    // If no echo, treat as far away
    return 32767L;
  }

  // Requirement 4 note: convert to cm by dividing by 58
  long distanceCm = (long)(duration / 58UL);
  return distanceCm;
}

void loop() {
  long distanceCm = readDistanceCm(); // Requirement 3

  // Requirement 4 & 5:
  // If distance is smaller than 50, turn on red
  // If distance is greater than or equal to 50, turn on green
  if (distanceCm < DIST_THRESHOLD_CM) {
    // Red ON, others OFF (common cathode)
    digitalWrite(RED_PIN, LOW);     // on
    digitalWrite(GREEN_PIN, HIGH); // off
    digitalWrite(BLUE_PIN, HIGH);  // off
  } else {
    // Green ON, others OFF (common cathode)
    digitalWrite(RED_PIN, HIGH);   // off
    digitalWrite(GREEN_PIN, LOW); // on
    digitalWrite(BLUE_PIN, HIGH);  // off
  }

  delay(50);
}