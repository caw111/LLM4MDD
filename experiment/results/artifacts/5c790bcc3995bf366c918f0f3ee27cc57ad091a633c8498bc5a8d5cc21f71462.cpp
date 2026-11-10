#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t TRIG_PIN = 8;   // Trig = D8
const uint8_t ECHO_PIN = 9;   // Echo = D9
const uint8_t LED_PIN  = 13;  // D13

// HC-SR04 distance measurement
// Speed of sound ~343 m/s => ~0.0343 cm/us
// Distance (cm) = (pulseWidth_us * 0.0343) / 2
// => Distance (cm) = pulseWidth_us * 0.01715
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float measureDistanceCm();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float measureDistanceCm()
{
  // Ensure a clean trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // 10us trigger pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width (use timeout to avoid blocking forever)
  // Max distance 400cm => echo ~ 400/0.0343*2 = ~23311 us
  const unsigned long timeoutUs = 30000UL;

  unsigned long pulseWidth = pulseIn(ECHO_PIN, HIGH, timeoutUs);
  if (pulseWidth == 0) {
    // No echo received within timeout; return a sentinel large distance
    return 9999.0f;
  }

  return (float)pulseWidth * 0.01715f;
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(TRIG_PIN, LOW);

  // No Serial required by requirements.
}

void loop()
{
  float distanceCm = measureDistanceCm();

  if (distanceCm < 20.0f) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(50);
}
