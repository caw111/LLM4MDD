#include <Arduino.h>

static constexpr uint8_t LED_PIN = 2;     // PWM output
static constexpr uint8_t POT1_PIN = A3;    // io_1
static constexpr uint8_t POT2_PIN = A4;    // io_2
static constexpr uint8_t POT3_PIN = A5;    // io_3

static constexpr unsigned long CONTROL_PERIOD_MS = 10;
static constexpr unsigned long SERIAL_BAUD = 9600;

static float adcToUnit(unsigned int adc) {
  // Normalize ADC [0..1023] to [0..1] (scale=1, offset=0 as per contract)
  return (float)adc / 1023.0f;
}

static void writeSerialF64Label(const char *label, float value) {
  // No fixed serial protocol specified; emit a typed numeric report as 4 lines per update.
  Serial.print(label);
  Serial.print('=');
  // Use a reasonable precision without altering behavior (value semantics preserved)
  Serial.println(value, 6);
}

void setup() {
  // LED off on initialization/reset
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  Serial.begin(SERIAL_BAUD);

  // Control loop timing
  // (No additional state is required; controller led_brightness_internal starts at 0.0)
}

void loop() {
  static unsigned long lastTickMs = 0;

  unsigned long now = millis();
  if (now - lastTickMs < CONTROL_PERIOD_MS) {
    return;
  }
  lastTickMs = now;

  // Sample analog inputs (io_1..io_3)
  unsigned int raw1 = analogRead(POT1_PIN);
  unsigned int raw2 = analogRead(POT2_PIN);
  unsigned int raw3 = analogRead(POT3_PIN);

  float io_1 = adcToUnit(raw1); // pot1 upper bound in [0..1]
  float io_2 = adcToUnit(raw2); // pot2 lower bound in [0..1]
  float io_3 = adcToUnit(raw3); // pot3 in [0..1] (model treats as request scaled within bounds)

  // Controller tick:
  // led_brightness_internal := (if io_1 >= io_2 then io_2 + (io_3 * (io_1 - io_2)) else 0.0)
  float led_brightness_internal = 0.0f;
  if (io_1 >= io_2) {
    led_brightness_internal = io_2 + (io_3 * (io_1 - io_2));
  } else {
    led_brightness_internal = 0.0f;
  }

  // Output io_0 = led_brightness_internal via PWM on D2 with scale 255, offset 0
  // Keep semantics: pwm_count is led_brightness_internal * 255.
  float pwmFloat = led_brightness_internal * 255.0f;
  int pwmCount = (int)(pwmFloat + 0.5f); // round to nearest

  // Typical Arduino PWM expects [0..255]; clamp to be safe while preserving intended mapping.
  if (pwmCount < 0) pwmCount = 0;
  if (pwmCount > 255) pwmCount = 255;

  analogWrite(LED_PIN, (uint8_t)pwmCount);

  // serial_0_trigger is true each tick
  // serial_0_value_0..3: pot1_upper, pot2_lower, pot3_mapped, led_brightness
  // Here pot3_mapped is io_3 (as model defines output serial_0_value_2 = io_3).
  if (true) {
    writeSerialF64Label("pot1_upper", io_1);
    writeSerialF64Label("pot2_lower", io_2);
    writeSerialF64Label("pot3_mapped", io_3);
    writeSerialF64Label("led_brightness", led_brightness_internal);
  }
}