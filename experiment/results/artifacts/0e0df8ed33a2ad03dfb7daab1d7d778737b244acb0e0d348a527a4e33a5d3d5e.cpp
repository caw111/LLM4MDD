#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// HC-SR04: Trig=D8, Echo=D9
// LED: D13
// Buzzer: D6 (tone)

const byte TRIG_PIN = 8;
const byte ECHO_PIN = 9;

const byte LED_PIN = 13;
const byte BUZZER_PIN = 6;

const int DIST_THRESHOLD_CM = 20;

// HC-SR04 timing constants
// Speed of sound ~343 m/s => 0.0343 cm/us
// Distance (cm) = pulse_us * 0.0343 / 2
const unsigned long MAX_ECHO_US = 30000UL; // ~5m max-ish guard

// Read distance in cm. Returns -1 on timeout/no echo.
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static long readDistanceCm();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static long readDistanceCm() {
  // Trigger a 10us pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width
  // Timeout prevents blocking forever.
  unsigned long pulseWidth = pulseIn(ECHO_PIN, HIGH, MAX_ECHO_US);
  if (pulseWidth == 0) return -1; // timeout or no echo

  // Convert to distance
  // cm = pulseWidth(us) * 0.0343 / 2
  // Use float-free integer math approximation:
  // 0.0343/2 = 0.01715
  // distance_cm ~= pulse_us * 1715 / 100000
  unsigned long distance_cm = (pulseWidth * 1715UL) / 100000UL;
  return (long)distance_cm;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_PIN, OUTPUT);

  // Use tone() for buzzer pin
  // (tone() will take over pin toggling; pinMode OUTPUT is sufficient on AVR UNO)
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
}

void loop() {
  long dist = readDistanceCm();

  if (dist >= 0 && dist < DIST_THRESHOLD_CM) {
    digitalWrite(LED_PIN, HIGH);
    // Alarm beeping
    // Keep tone on while alarm condition holds.
    tone(BUZZER_PIN, 2000); // 2kHz
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }

  delay(60);
}
