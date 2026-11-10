#include <Servo.h>

Servo servoMotor;

const int potPin = A0;   // potentiometer SIG -> A0
const int servoPin = 9;  // servo PWM -> 9

void setup() {
  // 1. Generate Servo object (already done above by global instance)
  // 2. Set potentiometer signal pin as INPUT
  pinMode(potPin, INPUT);

  // 5. Serial monitor output (allowed/expected behavior; baud 9600 specified)
  Serial.begin(9600);

  // Attach servo to PWM pin
  servoMotor.attach(servoPin);
}

void loop() {
  // 3. Continuously read potentiometer value (0..1023)
  int potValue = analogRead(potPin);

  // Map to servo range 0..180
  int servoValue = map(potValue, 0, 1023, 0, 180);

  // Ensure bounds in case of any edge conditions
  if (servoValue < 0) servoValue = 0;
  if (servoValue > 180) servoValue = 180;

  // 4. Write the mapped value to the servo motor
  servoMotor.write(servoValue);

  // 5. Continuously read the servo value and print it to the serial monitor
  // (We print the value we are writing as the "servo value".)
  Serial.println(servoValue);

  // Small delay to match serial "line_or_idle" behavior
  delay(20);
}