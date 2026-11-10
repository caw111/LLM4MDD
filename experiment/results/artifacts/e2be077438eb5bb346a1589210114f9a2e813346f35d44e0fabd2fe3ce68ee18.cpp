#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t PIN_LED_D13 = 13;

static const uint8_t PIN_BUZZER_D6 = 6;

static const uint8_t PIN_ULTRASONIC_TRIG_D8 = 8;
static const uint8_t PIN_ULTRASONIC_ECHO_D9 = 9;

static const uint8_t PIN_TM1637_CLK_D4 = 4;
static const uint8_t PIN_TM1637_DIO_D5 = 5;

// ---- Timing contract ----
static const uint32_t CONTROL_PERIOD_MS = 5;    // io_0/io_1/io_4 update granularity
static const uint32_t SENSOR_PERIOD_MS  = 60;   // io_2/io_3 update granularity
static const uint32_t ULTRASONIC_TIMEOUT_US = 30000UL;

// ---- Accepted model state (frozen semantics) ----
static double lastValidDistance = 0.0;   // io_2 when io_3 is true
static bool hasEverValidDistance = false;

// ---- IO shadow values ----
static double io_2_distance_cm = 0.0; // latest measured distance (or held)
static bool io_3_valid = false;        // latest validity

// ---- TM1637 ----
TM1637Display tm1637(PIN_TM1637_CLK_D4, PIN_TM1637_DIO_D5);

// ---- Buzzer tone generation mapping ----
// Model defines io_1 as Hz: slow (<15cm)=600, fast (<8cm)=1000, silence otherwise.
// We implement "tone_output" using Arduino tone() with frequency (Hz) and disable when 0.
// (Exact on/off pattern/timing was not further specified in the contract; silence vs tone frequency
// is sufficient to preserve the model outputs.)
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyBuzzerHz(int32_t hz);
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool sampleUltrasonicCm(double &outCm);
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick();
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerUpdateStateFromIoInputs();
#line 110 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 135 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_008\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyBuzzerHz(int32_t hz) {
  if (hz <= 0) {
    noTone(PIN_BUZZER_D6);
  } else {
    tone(PIN_BUZZER_D6, hz);
  }
}

// ---- Ultrasonic sampling (HC-SR04) ----
// Holds last valid measurement when invalid_policy=hold_last_valid.
// We implement io_3 as validity (true if echo received within timeout).
static bool sampleUltrasonicCm(double &outCm) {
  // Trigger: at least 10us high
  digitalWrite(PIN_ULTRASONIC_TRIG_D8, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_ULTRASONIC_TRIG_D8, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTRASONIC_TRIG_D8, LOW);

  unsigned long durationUs = pulseIn(PIN_ULTRASONIC_ECHO_D9, HIGH, ULTRASONIC_TIMEOUT_US);
  if (durationUs == 0UL) {
    return false; // invalid (timeout)
  }

  // Speed of sound ~343 m/s -> 0.0343 cm/us round-trip.
  // Distance (one-way) = duration_us * 0.0343 / 2
  double cm = (double)durationUs * 0.0343 / 2.0;
  outCm = cm;
  return true;
}

// ---- Control update: accepted model outputs ----
static void controllerTick() {
  // update outputs based on frozen model semantics:
  // io_0: hasEverValidDistance && lastValidDistance < 30.0
  bool io_0_led = hasEverValidDistance && (lastValidDistance < 30.0);

  // io_1: if not hasEverValidDistance then 0
  // else if lastValidDistance < 8.0 then 1000
  // else if lastValidDistance < 15.0 then 600
  // else 0
  int32_t io_1_hz;
  if (!hasEverValidDistance) {
    io_1_hz = 0;
  } else if (lastValidDistance < 8.0) {
    io_1_hz = 1000;
  } else if (lastValidDistance < 15.0) {
    io_1_hz = 600;
  } else {
    io_1_hz = 0;
  }

  // io_4: if hasEverValidDistance then floor(lastValidDistance) else 0
  int32_t io_4_display = hasEverValidDistance ? (int32_t)floor(lastValidDistance) : 0;

  // apply outputs
  digitalWrite(PIN_LED_D13, io_0_led ? HIGH : LOW);
  applyBuzzerHz(io_1_hz);
  tm1637.displayInt(io_4_display, false);
}

static void controllerUpdateStateFromIoInputs() {
  // Accepted model update:
  // if io_3 then
  //   lastValidDistance := io_2
  //   hasEverValidDistance := true
  if (io_3_valid) {
    lastValidDistance = io_2_distance_cm;
    hasEverValidDistance = true;
  }
}

static uint32_t nextControlDue = 0;
static uint32_t nextSensorDue = 0;

void setup() {
  pinMode(PIN_LED_D13, OUTPUT);
  pinMode(PIN_BUZZER_D6, OUTPUT);

  pinMode(PIN_ULTRASONIC_TRIG_D8, OUTPUT);
  pinMode(PIN_ULTRASONIC_ECHO_D9, INPUT);

  // Initialize IO according to model initial values
  lastValidDistance = 0.0;
  hasEverValidDistance = false;

  // Initialize hardware outputs to model initial outputs
  digitalWrite(PIN_LED_D13, LOW);
  applyBuzzerHz(0);
  tm1637.setBrightness(7); // brightness=7 as frozen contract
  tm1637.displayInt(0, false);

  io_2_distance_cm = 0.0;
  io_3_valid = false;

  uint32_t now = millis();
  nextSensorDue = now;                 // sample immediately
  nextControlDue = now;                // run control immediately
}

void loop() {
  uint32_t now = millis();

  // Sensor update (io_2/io_3) every SENSOR_PERIOD_MS
  if ((int32_t)(now - nextSensorDue) >= 0) {
    nextSensorDue += SENSOR_PERIOD_MS;

    double measured = 0.0;
    bool valid = sampleUltrasonicCm(measured);

    io_3_valid = valid;
    if (valid) {
      io_2_distance_cm = measured;
    } else {
      // invalid_policy=hold_last_valid for io_2, io_3 becomes false
      // keep io_2_distance_cm unchanged
    }
  }

  // Control update (io_0/io_1/io_4) every CONTROL_PERIOD_MS
  if ((int32_t)(now - nextControlDue) >= 0) {
    nextControlDue += CONTROL_PERIOD_MS;

    // state update per accepted model (uses current io_3/io_2 shadows)
    controllerUpdateStateFromIoInputs();

    // outputs per accepted model
    controllerTick();
  }

  // no extra behavior; keep loop responsive
}
