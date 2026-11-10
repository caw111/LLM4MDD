#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_distance_sensor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t TRIG_PIN = 3;
const uint8_t ECHO_PIN = 2;

// Requirement: divide sensor value by 58 to convert to centimeters.
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_distance_sensor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static long readDistanceCm();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_distance_sensor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_distance_sensor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_distance_sensor_basic\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static long readDistanceCm() {
  // Trigger: LOW -> HIGH for 10us -> LOW
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse width in microseconds
  unsigned long duration = pulseIn(ECHO_PIN, HIGH);

  // Convert to cm (as specified)
  // If duration is 0 (timeout), cm will compute to 0 which preserves behavior.
  long cm = (long)(duration / 58UL);
  return cm;
}

void setup() {
  // 1. Initialize serial communication (baud=9600; framing=line_or_idle)
  // In Arduino terms: Serial defaults to 8N1 and line-oriented printing via println.
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Ensure known initial state
  digitalWrite(TRIG_PIN, LOW);
}

void loop() {
  // 2. Continuously trigger and read the Ultrasonic distance sensor
  // 3. Print format: "Distance in CM: {value}"
  long cm = readDistanceCm();
  Serial.print("Distance in CM: ");
  Serial.println(cm);

  // Small delay to prevent flooding while still continuously reading
  delay(50);
}
