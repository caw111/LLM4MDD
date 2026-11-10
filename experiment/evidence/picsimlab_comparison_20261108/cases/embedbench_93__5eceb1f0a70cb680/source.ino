#include <Arduino.h>
#include <Servo.h>

static constexpr uint8_t LED1_PIN = 2;   // io_0
static constexpr uint8_t LED2_PIN = 3;   // io_1
static constexpr uint8_t POT_PIN  = A3;  // io_2
static constexpr uint8_t SERVO_PIN = 4;  // io_3

// ACCEPTED executable model semantics (frozen contract)
//
// - io_0 (led1) is always false
// - io_1 (led2) is always false
// - io_3 (servo angle command) is mapped from io_2 (pot ADC counts) to 0..180 deg
// - controller update order per tick:
//     1) holdMsCounter := 0
//     2) prevAngle := io_3 (mapped angle)
//   (prevAngle/holdMsCounter do not affect outputs in this model)
static Servo servo1;

// Controller state matching the model
static double prevAngle = 0.0;      // initial 0.0
static int32_t holdMsCounter = 0;   // initial 0

// Control period from contract: 5 ms
static constexpr uint32_t CONTROL_PERIOD_MS = 5;

static double mapPotToAngle(double potCounts) {
  // EXACT mapping expression from frozen model:
  // (if io_2 >= 0 then (if io_2 <= 1023 then io_2*180/1023
  // else (if io_2 > 1023 then 180 else 0))
  // else (if io_2 < 0 then 0 else io_2*180/1023))
  if (potCounts >= 0.0) {
    if (potCounts <= 1023.0) {
      return potCounts * 180.0 / 1023.0;
    } else {
      if (potCounts > 1023.0) {
        return 180.0;
      } else {
        return 0.0;
      }
    }
  } else {
    if (potCounts < 0.0) {
      return 0.0;
    } else {
      return potCounts * 180.0 / 1023.0;
    }
  }
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Initialization requirement: LEDs OFF; servo at 0 degrees.
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  servo1.attach(SERVO_PIN);
  servo1.write(0);
}

void loop() {
  static uint32_t lastTickMs = 0;
  uint32_t now = millis();
  if (now - lastTickMs < CONTROL_PERIOD_MS) return;
  lastTickMs = now;

  // Read io_2 (analog_input, scale 1.0, offset 0.0) from A3
  int raw = analogRead(POT_PIN);          // 0..1023 on AVR
  double io_2 = static_cast<double>(raw); // adc_count

  // controller updates (ordered)
  holdMsCounter = 0; // assign holdMsCounter := 0

  // Compute io_3 according to output expression
  double io_3 = mapPotToAngle(io_2);

  // assign prevAngle := io_3
  prevAngle = io_3;

  // Outputs:
  // io_0 (led1) is false; io_1 (led2) is false (model does not implement direction logic)
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  // Drive servo angle command (invalid_policy: hold_last_valid)
  // Here io_3 is always produced by the mapping expression (finite), so always update.
  servo1.write((int)io_3);
}