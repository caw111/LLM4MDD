#include <Arduino.h>

static inline int readPot8(uint8_t analogPin) {
  int v = analogRead(analogPin);          // 0..1023
  // Map to 0..255
  return (v * 255L) / 1023L;
}

static inline bool readBtnActiveLow(uint8_t digitalPin) {
  // active_low: pressed => LOW => true
  return digitalRead(digitalPin) == LOW;
}

int main() {
  init();

  // Pin bindings from contract / circuit
  const uint8_t LED_PWM_PIN = 2;  // D2 (PWM)
  const uint8_t BTN1_PIN    = 3;  // D3
  const uint8_t BTN2_PIN    = 4;  // D4
  const uint8_t POT1_PIN    = A3; // io_3
  const uint8_t POT2_PIN    = A4; // io_4
  const uint8_t POT3_PIN    = A5; // io_5

  pinMode(LED_PWM_PIN, OUTPUT);
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  // Initial state per model: modeIsAuto=false; previousBtn1=false; previousBtn2=false
  bool modeIsAuto = false;
  bool previousBtn1 = false;
  bool previousBtn2 = false;

  // Emit initial LED output: io_0 = floor(io_3) since modeIsAuto=false.
  // io_3 is pot1 mapped to adc_count->real; we output as uint8 brightness.
  // Control period is specified as 10ms; use a simple fixed schedule.
  unsigned long lastTickMs = millis();
  const unsigned long controlPeriodMs = 10;

  // Debounce requirement exists in prose, but the provided accepted model
  // has only edge-detection on raw io_1/io_2 without dwell/2s persistence.
  // Implement only the accepted executable model behavior exactly.
  for (;;) {
    unsigned long nowMs = millis();
    // Maintain a 10ms control period
    if (nowMs - lastTickMs < controlPeriodMs) continue;
    lastTickMs += controlPeriodMs;

    // Read inputs (io_1..io_5)
    bool io_1 = readBtnActiveLow(BTN1_PIN); // btn1 press request (true when pressed)
    bool io_2 = readBtnActiveLow(BTN2_PIN); // btn2 press request (true when pressed)

    // Pots: model uses io_3..io_5 as Real "adc_count" values; map to 0..255
    // to match output expectation on PWM. The model applies floor() to io_3,
    // and the auto computation also floor()s the interpolation that is constant
    // because (1.0/1.0) and the else/then forms collapse to either bound.
    int pot1 = readPot8(POT1_PIN); // io_3
    int pot2 = readPot8(POT2_PIN); // io_4
    int pot3 = readPot8(POT3_PIN); // io_5

    // Tick: edge detection
    if (io_1 && (!previousBtn1)) {
      modeIsAuto = true;
    }
    if (io_2 && (!previousBtn2)) {
      modeIsAuto = false;
    }

    previousBtn1 = io_1;
    previousBtn2 = io_2;

    // Output io_0 per model
    int pwmVal = 0;

    if (modeIsAuto) {
      // if io_5 > io_4 then floor(io_4 + (io_5-io_4)*(1/1)) else floor(io_5 + (io_4-io_5)*(1/1))
      // Both evaluate to floor(max(io_4,io_5)) and floor(min(...) respectively).
      // Using the exact structure:
      if (pot3 > pot2) {
        // floor(pot2 + (pot3-pot2)*1) = pot3
        pwmVal = pot3;
      } else {
        // floor(pot3 + (pot2-pot3)*1) = pot2
        pwmVal = pot2;
      }
    } else {
      // floor(io_3) => pot1
      pwmVal = pot1;
    }

    // Apply PWM output
    if (pwmVal < 0) pwmVal = 0;
    if (pwmVal > 255) pwmVal = 255;
    analogWrite(LED_PWM_PIN, pwmVal);
  }
}