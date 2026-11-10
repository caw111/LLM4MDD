#include <Arduino.h>
#include <Servo.h>

struct Controller {
  long servoPhaseMs;   // i32
  long ledPhaseMs;     // i32
  bool ledToggle;      // bool

  Controller() : servoPhaseMs(0), ledPhaseMs(0), ledToggle(false) {}

  void tick(int32_t io_5_delta_ms) {
    // updates
    ledPhaseMs   = ledPhaseMs + (long)io_5_delta_ms;
    servoPhaseMs = servoPhaseMs + (long)io_5_delta_ms;

    // if ledPhaseMs >= 2000
    if (ledPhaseMs >= 2000L) {
      ledPhaseMs = ledPhaseMs - 2000L;
      ledToggle = !ledToggle;
    }

    // if servoPhaseMs >= 4000
    if (servoPhaseMs >= 4000L) {
      servoPhaseMs = servoPhaseMs - 4000L;
    }
  }

  // outputs
  bool io_0_led1() const { return ledToggle; }

  bool io_1_led2() const { return ledToggle ? false : true; }

  double io_2_servo1_deg() const { return (servoPhaseMs < 2000L) ? 0.0 : 180.0; }

  double io_3_servo2_deg() const { return (servoPhaseMs < 2000L) ? 90.0 : 0.0; }

  double io_4_servo3_deg() const { return (servoPhaseMs < 2000L) ? 180.0 : 90.0; }
};

static const uint8_t LED1_PIN = 2; // D2
static const uint8_t LED2_PIN = 3; // D3

static const uint8_t SERVO1_PIN = 4; // D4 PWM
static const uint8_t SERVO2_PIN = 5; // D5 PWM
static const uint8_t SERVO3_PIN = 6; // D6 PWM

Servo servo1;
Servo servo2;
Servo servo3;

static Controller controller;

static uint32_t lastMs = 0;

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Initialization: both LEDs off; servos at 0°
  digitalWrite(LED1_PIN, LOW); // led1 false
  digitalWrite(LED2_PIN, LOW); // led2 false

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Ensure initial positions per contract: 0° for all at startup
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  lastMs = millis();
}

void loop() {
  // control_period_ms = 10, and io_5 is delta since previous step (i32 ms).
  uint32_t now = millis();
  uint32_t elapsed = now - lastMs;
  if (elapsed < 10) return; // keep sampling period close to 10ms
  lastMs = now;

  int32_t io_5 = (elapsed > 0x7FFFFFFF ? 0x7FFFFFFF : (int32_t)elapsed);

  controller.tick(io_5);

  // Apply outputs in the same control step (ordered update semantics already captured in controller.tick)
  bool led1Val = controller.io_0_led1();
  bool led2Val = controller.io_1_led2();

  digitalWrite(LED1_PIN, led1Val ? HIGH : LOW);
  digitalWrite(LED2_PIN, led2Val ? HIGH : LOW);

  servo1.write((int)controller.io_2_servo1_deg());
  servo2.write((int)controller.io_3_servo2_deg());
  servo3.write((int)controller.io_4_servo3_deg());
}