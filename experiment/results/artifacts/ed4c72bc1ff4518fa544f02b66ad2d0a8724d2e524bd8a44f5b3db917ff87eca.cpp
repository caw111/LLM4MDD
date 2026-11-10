#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Wiring (from provided circuit):
// Potentiometers: pot1 -> A3, pot2 -> A4
// RGB LEDs (common cathode):
//   rgb1: R=2, G=3, B=4
//   rgb2: R=5, G=6, B=7
// Bar graph (green segments A1..A8, common C1..C8 to GND):
//   A1=8, A2=9, A3=10, A4=11, A5=12, A6=13, A7=44, A8=45

static const uint8_t RGB1_R = 2;
static const uint8_t RGB1_G = 3;
static const uint8_t RGB1_B = 4;

static const uint8_t RGB2_R = 5;
static const uint8_t RGB2_G = 6;
static const uint8_t RGB2_B = 7;

static const uint8_t BAR_A[8] = {8, 9, 10, 11, 12, 13, 44, 45};

static const uint8_t POT1_PIN = A3;
static const uint8_t POT2_PIN = A4;

// Off states at power-on:
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBOff();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBColor(uint8_t rPin, uint8_t gPin, uint8_t bPin, int value0to1023);
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBargraphFromAverage(int avg0to1023);
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBOff()
{
  // Common cathode: LEDs light when pin is HIGH, off when LOW.
  digitalWrite(RGB1_R, LOW);
  digitalWrite(RGB1_G, LOW);
  digitalWrite(RGB1_B, LOW);

  digitalWrite(RGB2_R, LOW);
  digitalWrite(RGB2_G, LOW);
  digitalWrite(RGB2_B, LOW);
}

static void setRGBColor(uint8_t rPin, uint8_t gPin, uint8_t bPin, int value0to1023)
{
  // Lower third: 0..340 => red (inclusive boundaries per spec)
  // Middle third: 341..681 => green
  // Upper third: 682..1023 => blue
  if (value0to1023 <= 340) {
    digitalWrite(rPin, HIGH);
    digitalWrite(gPin, LOW);
    digitalWrite(bPin, LOW);
  } else if (value0to1023 <= 681) {
    digitalWrite(rPin, LOW);
    digitalWrite(gPin, HIGH);
    digitalWrite(bPin, LOW);
  } else {
    digitalWrite(rPin, LOW);
    digitalWrite(gPin, LOW);
    digitalWrite(bPin, HIGH);
  }
}

static void setBargraphFromAverage(int avg0to1023)
{
  // Number of lit LEDs proportional to average.
  // Example given: avg=512 (about half) => 4 LEDs.
  // Use floor(avg * 8 / 1023 + small epsilon via integer math) with proper bounds.
  // avg=0 => 0 LEDs; avg=1023 => 8 LEDs.
  long num = (long)avg0to1023 * 8L;
  int lit = (int)(num / 1023L); // floor
  if (lit < 0) lit = 0;
  if (lit > 8) lit = 8;

  for (int i = 0; i < 8; i++) {
    digitalWrite(BAR_A[i], (i < lit) ? HIGH : LOW);
  }
}

void setup()
{
  // Pot pins are inputs (analog)
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  // RGB pins
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // Bar graph segment pins
  for (int i = 0; i < 8; i++) pinMode(BAR_A[i], OUTPUT);

  // Initialize all components in OFF state
  setRGBOff();
  for (int i = 0; i < 8; i++) digitalWrite(BAR_A[i], LOW);
}

void loop()
{
  // Read potentiometers
  int pot1 = analogRead(POT1_PIN); // 0..1023
  int pot2 = analogRead(POT2_PIN); // 0..1023

  // Update RGB LEDs immediately based on potentiometer thirds
  setRGBColor(RGB1_R, RGB1_G, RGB1_B, pot1);
  setRGBColor(RGB2_R, RGB2_G, RGB2_B, pot2);

  // Compute average and update bar graph
  int avg = (pot1 + pot2) / 2;
  setBargraphFromAverage(avg);

  // Maintain each state for at least 2 seconds if potentiometers remain stationary.
  // Since updates are continuous, this behavior is naturally satisfied: if readings don't change,
  // outputs remain unchanged. Still, add a small delay to reduce flicker/jitter while preserving
  // real-time responsiveness.
  delay(50);
}
