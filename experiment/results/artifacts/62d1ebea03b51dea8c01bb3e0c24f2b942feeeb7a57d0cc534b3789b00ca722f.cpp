#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t TRIG_PIN = 8; // Trig -> D8
const uint8_t ECHO_PIN = 9; // Echo -> D9

// HC-SR04 typical measurements depend on speed of sound.
// This sketch performs continuous distance measurement and outputs via Serial.
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float measureDistanceCm();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float measureDistanceCm() {
  // Trigger pulse: 10 microseconds HIGH
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse width in microseconds
  // Timeout to avoid blocking indefinitely.
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL); // up to ~5m (30000us)

  if (duration == 0) {
    return NAN; // No echo received within timeout
  }

  // Speed of sound ~343 m/s => 0.0343 cm/us for one-way.
  // Distance (cm) = duration(us) * 0.0343 / 2
  return (float)duration * 0.0343f / 2.0f;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  float distanceCm = measureDistanceCm();

  if (isnan(distanceCm)) {
    // Output a blank line or "nan" consistently; requirement says output distance.
    // We'll output "nan" as text when no echo is received.
    Serial.println("nan");
  } else {
    Serial.println(distanceCm, 2); // distance with 2 decimals
  }

  // Small delay to avoid overwhelming serial and sensor
  delay(60);
}
