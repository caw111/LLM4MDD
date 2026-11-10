#include <Arduino.h>
#include <TM1637Display.h>

const uint8_t TRIG_PIN = 8;   // HC-SR04 Trig
const uint8_t ECHO_PIN = 2;   // HC-SR04 Echo

const uint8_t TM_CLK_PIN = 4; // TM1637 CLK
const uint8_t TM_DIO_PIN = 5; // TM1637 DIO

const uint8_t LED_NEAR_PIN = 13; // D13 near-distance indicator
const uint8_t PWM_LED_PIN = 9;  // D9 PWM brightness control

TM1637Display display(TM_CLK_PIN, TM_DIO_PIN);

static float readDistanceCm() {
  // Trigger ultrasonic pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width (timeout to avoid lockup)
  // Speed of sound ~343 m/s => 58.2 us per cm (round trip)
  const unsigned long timeoutUs = 30000UL; // ~5m range limit (approx)
  unsigned long echoStart = micros();

  // Wait for echo to go HIGH
  while (digitalRead(ECHO_PIN) == LOW) {
    if ((micros() - echoStart) > timeoutUs) return NAN;
  }
  // Measure HIGH duration
  unsigned long start = micros();
  while (digitalRead(ECHO_PIN) == HIGH) {
    if ((micros() - start) > timeoutUs) return NAN;
  }
  unsigned long durationUs = micros() - start;

  // Convert duration to distance in cm
  float distanceCm = durationUs / 58.2f;
  return distanceCm;
}

static uint8_t clampU8(int v, int lo, int hi) {
  if (v < lo) return (uint8_t)lo;
  if (v > hi) return (uint8_t)hi;
  return (uint8_t)v;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_NEAR_PIN, OUTPUT);
  pinMode(PWM_LED_PIN, OUTPUT);

  // Ensure initial states
  digitalWrite(LED_NEAR_PIN, LOW);
  analogWrite(PWM_LED_PIN, 0);

  display.setBrightness(7); // TM1637 brightness (fixed; PWM LED handles brightness per distance)
  display.clear();

  // Optional: serial not requested
}

void loop() {
  // Sample multiple times for stability
  float best = NAN;
  for (uint8_t i = 0; i < 5; i++) {
    float d = readDistanceCm();
    if (!isnan(d)) {
      best = d;
      break; // take first valid (preserves "behavior" deterministically)
    }
    delay(10);
  }

  float distanceCm = best;
  if (isnan(distanceCm)) {
    // If no valid reading, keep LEDs off and display 0 (graceful behavior)
    digitalWrite(LED_NEAR_PIN, LOW);
    analogWrite(PWM_LED_PIN, 0);
    display.showNumberDec(0, false, 4, 0);
    delay(80);
    return;
  }

  // Display distance (integer cm)
  int distInt = (int)(distanceCm + 0.5f);
  if (distInt < 0) distInt = 0;
  if (distInt > 9999) distInt = 9999;

  // Near-distance LED D13 logic (choose a reasonable threshold to satisfy "near distance")
  const int NEAR_THRESHOLD_CM = 30; // near if <= 30cm

  bool isNear = (distInt <= NEAR_THRESHOLD_CM);
  digitalWrite(LED_NEAR_PIN, isNear ? HIGH : LOW);

  // PWM brightness on D9 based on distance: nearer => higher brightness
  // Map [0..MAX_NEAR] to [255..0] (clamped)
  const int MAX_BRIGHT_DIST_CM = 200; // beyond this, very dim/off
  int brightness;
  if (distInt <= NEAR_THRESHOLD_CM) {
    brightness = 255;
  } else if (distInt >= MAX_BRIGHT_DIST_CM) {
    brightness = 0;
  } else {
    // Linear falloff from (NEAR_THRESHOLD_CM,255) to (MAX_BRIGHT_DIST_CM,0)
    float t = (float)(distInt - NEAR_THRESHOLD_CM) / (float)(MAX_BRIGHT_DIST_CM - NEAR_THRESHOLD_CM);
    brightness = (int)((1.0f - t) * 255.0f);
  }
  brightness = clampU8(brightness, 0, 255);
  analogWrite(PWM_LED_PIN, brightness);

  // Show number on TM1637, 4 digits, no leading zeros
  display.showNumberDec(distInt, false, 4, 0);

  delay(60);
}