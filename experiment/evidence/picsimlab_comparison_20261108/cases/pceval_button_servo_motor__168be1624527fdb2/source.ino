#include <Servo.h>

static const uint8_t SERVO_PIN = 9;   // PWM
static const uint8_t BUTTON_PIN = 3;  // active-low with pull-up

static const int SERVO_ANGLE_PRESSED_DEG = 180;
static const int SERVO_ANGLE_NOT_PRESSED_DEG = 90;

Servo servoMotor;

static bool readButtonPressedActiveLow() {
  // Model semantics: true when pressed (true_level = LOW), pullup enabled.
  // If the button is pressed, the input reads LOW => return true.
  return (digitalRead(BUTTON_PIN) == LOW);
}

void setup() {
  Serial.begin(9600);

  servoMotor.attach(SERVO_PIN);

  pinMode(BUTTON_PIN, INPUT); // pull-up wiring is provided externally (10k resistor)
  // Ensure initial behavior: not pressed -> 90 degrees on startup.
  servoMotor.write(SERVO_ANGLE_NOT_PRESSED_DEG);
}

void loop() {
  // control_period_ms = 5 (best-effort; use delay to align with requirement)
  // Read input first
  bool io_1_pressed = readButtonPressedActiveLow();

  // Outputs according to accepted executable model:
  // io_0 = (if io_1 then 180.0 else 90.0)
  // serial_0_trigger = true (always)
  // serial_0_value_0 = (if io_1 then 180 else 90)
  int servo_angle_deg = io_1_pressed ? SERVO_ANGLE_PRESSED_DEG : SERVO_ANGLE_NOT_PRESSED_DEG;

  servoMotor.write(servo_angle_deg);

  // Continuous report gate: serial_0_trigger is true every step.
  Serial.println(servo_angle_deg);

  delay(5);
}