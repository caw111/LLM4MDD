// Arduino Mega 2560 sketch
// Controls three single-color LEDs (led1, led2, led3) and an RGB LED (rgb1)
// using a slide potentiometer (pot1) to set brightness and indicator LEDs.

const int potPin = A3;

// Single-color LEDs
const int led1Pin = 2;
const int led2Pin = 3;
const int led3Pin = 4;

// RGB LED pins (common cathode)
const int rgbRPin = 5;
const int rgbGPin = 6;
const int rgbBPin = 7;

void setup() {
  pinMode(potPin, INPUT);

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

  pinMode(rgbRPin, OUTPUT);
  pinMode(rgbGPin, OUTPUT);
  pinMode(rgbBPin, OUTPUT);

  // Initialize all LEDs OFF
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  digitalWrite(led3Pin, LOW);

  // Common cathode: OFF = PWM value 0
  analogWrite(rgbRPin, 0);
  analogWrite(rgbGPin, 0);
  analogWrite(rgbBPin, 0);
}

void loop() {
  int potValue = analogRead(potPin); // 0..1023

  // Indicator LEDs based on thirds
  // Lower third: 0-341 => led1
  // Middle third: 342-682 => led2
  // Upper third: 683-1023 => led3
  bool led1On = (potValue >= 0 && potValue <= 341);
  bool led2On = (potValue >= 342 && potValue <= 682);
  bool led3On = (potValue >= 683 && potValue <= 1023);

  digitalWrite(led1Pin, led1On ? HIGH : LOW);
  digitalWrite(led2Pin, led2On ? HIGH : LOW);
  digitalWrite(led3Pin, led3On ? HIGH : LOW);

  // Map potentiometer to brightness 0..255 for each channel, then apply "max channel" rule:
  int brightness = map(potValue, 0, 1023, 0, 255);

  // RGB behavior:
  // - In lower third: red at max brightness
  // - In middle third: green at max brightness
  // - In upper third: blue at max brightness
  // Other channels remain 0.
  // Note: For common cathode, PWM value represents brightness directly.
  if (potValue <= 341) {
    analogWrite(rgbRPin, 255);
    analogWrite(rgbGPin, 0);
    analogWrite(rgbBPin, 0);
  } else if (potValue <= 682) {
    analogWrite(rgbRPin, 0);
    analogWrite(rgbGPin, 255);
    analogWrite(rgbBPin, 0);
  } else {
    analogWrite(rgbRPin, 0);
    analogWrite(rgbGPin, 0);
    analogWrite(rgbBPin, 255);
  }

  delay(5); // small delay to reduce flicker while updating continuously
}