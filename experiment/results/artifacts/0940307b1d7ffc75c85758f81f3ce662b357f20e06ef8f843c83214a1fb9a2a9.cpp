#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint8_t IO_0_PIN = 2;   // D2 -> S0
static constexpr uint8_t IO_1_PIN = 3;   // D3 -> S1
static constexpr uint8_t IO_2_PIN = 4;   // D4 -> S2
static constexpr uint8_t IO_3_PIN = 5;   // D5 -> S3
static constexpr uint8_t IO_4_PIN = A0;  // A0 -> COM

static constexpr uint32_t CONTROL_PERIOD_MS = 10;
static constexpr uint32_t SERIAL_BAUD = 9600;

static_assert(CONTROL_PERIOD_MS == 10, "Contract requires control_period_ms = 10");

#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setMuxSelect(uint8_t framePhase);
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t floorToI32(float x);
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t readIo4AsI32();
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void printAllSensors();
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_multiplexer_potentiometer\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void setMuxSelect(uint8_t framePhase) {
  // ACCEPTED model outputs:
  // io_0 = framePhase == 0 or framePhase == 2 or framePhase == 4
  // io_1 = framePhase == 1 or framePhase == 2
  // io_2 = framePhase == 3 or framePhase == 4
  // io_3 = false
  const bool io_0 = (framePhase == 0 || framePhase == 2 || framePhase == 4);
  const bool io_1 = (framePhase == 1 || framePhase == 2);
  const bool io_2 = (framePhase == 3 || framePhase == 4);
  const bool io_3 = false;

  digitalWrite(IO_0_PIN, io_0 ? HIGH : LOW);
  digitalWrite(IO_1_PIN, io_1 ? HIGH : LOW);
  digitalWrite(IO_2_PIN, io_2 ? HIGH : LOW);
  digitalWrite(IO_3_PIN, io_3 ? HIGH : LOW);
}

static inline int32_t floorToI32(float x) {
  // RealFunctions::floor(io_4), io_4 is f64 in contract but reads ADC counts.
  // We implement floor and cast to 32-bit signed integer.
  return (int32_t)((x >= 0.0f) ? floorf(x) : ceilf(x - 1.0f)); // robust for negatives
  // Note: ADC counts are non-negative, so this behaves as floorf(x).
}

int32_t pot0 = 0;
int32_t pot1 = 0;
int32_t pot2 = 0;
int32_t pot3 = 0;
int32_t pot4 = 0;
int32_t framePhase = 0;

uint32_t nextTickMs = 0;

static inline int32_t readIo4AsI32() {
  // io_4 is f64 adc_count, scale=1.0 offset=0.0.
  // Raw ADC counts 0..1023.
  const int raw = analogRead(IO_4_PIN);
  // Equivalent to floor(raw) as float.
  return (int32_t)raw;
}

static inline void printAllSensors() {
  // serial_0_trigger is true exactly when framePhase == 4.
  // Print each sensor value in format: "Sensor {channel}: {value}"
  Serial.print("Sensor 0: ");
  Serial.println(pot0);

  Serial.print("Sensor 1: ");
  Serial.println(pot1);

  Serial.print("Sensor 2: ");
  Serial.println(pot2);

  Serial.print("Sensor 3: ");
  Serial.println(pot3);

  Serial.print("Sensor 4: ");
  Serial.println(pot4);
}

void setup() {
  pinMode(IO_0_PIN, OUTPUT);
  pinMode(IO_1_PIN, OUTPUT);
  pinMode(IO_2_PIN, OUTPUT);
  pinMode(IO_3_PIN, OUTPUT);

  Serial.begin(SERIAL_BAUD);

  // Initialize mux selection according to initial framePhase = 0.
  setMuxSelect((uint8_t)framePhase);

  nextTickMs = millis();
}

void loop() {
  // Control period scheduling granularity = 10 ms.
  const uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) < 0) {
    return;
  }
  nextTickMs += CONTROL_PERIOD_MS;

  // Tick corresponds to ACCEPTED model 'perform action tick { ... }'.
  // Updates happen conditionally based on current framePhase.
  const float io4 = (float)readIo4AsI32();

  if (framePhase == 0) {
    pot0 = floorToI32(io4);
    framePhase = 1;
  } else if (framePhase == 1) {
    pot1 = floorToI32(io4);
    framePhase = 2;
  } else if (framePhase == 2) {
    pot2 = floorToI32(io4);
    framePhase = 3;
  } else if (framePhase == 3) {
    pot3 = floorToI32(io4);
    framePhase = 4;
  } else if (framePhase == 4) {
    pot4 = floorToI32(io4);
    framePhase = 0;
  } else {
    // Should not happen; keep deterministic by wrapping.
    framePhase = 0;
  }

  // Apply mux select outputs for the new framePhase (outputs depend on framePhase).
  setMuxSelect((uint8_t)framePhase);

  // serial_0_trigger is true when framePhase == 4 (out attribute).
  // With the model's tick ordering, we must ensure we trigger exactly once per full frame.
  //
  // Since framePhase is updated inside tick:
  // - When previously framePhase == 4, we read pot4 and then set framePhase := 0.
  // The output serial_0_trigger uses current framePhase; however the ACCEPTED model
  // specifies serial_0_trigger = (framePhase == 4).
  //
  // To preserve behavior, we must trigger at the moment the model reaches framePhase==4
  // during the tick cycle. The simplest deterministic way that matches the state-machine
  // semantics is: trigger when we just updated pot4 (i.e., the tick case framePhase==4).
  //
  // Therefore, infer trigger from the previous phase value:
  //
  // We can detect by checking which case we executed. Reconstruct by using the fact that
  // after reading, we advanced from 4 to 0.
  //
  // So trigger if the new framePhase is 0 AND pot4 was updated from the previous phase 4.
  // Because framePhase only becomes 0 from the previous phase 4, this is deterministic.
  //
  // Implement via pot4 update flag:
  static bool dummy = false; (void)dummy; // keep compilation unit clean

  // The above static dummy isn't needed; instead, we compute trigger directly:
  // We can compare mux outputs now: serial trigger should have been emitted exactly once
  // per acquisition frame. We'll implement by using a local static last framePhase snapshot.
  static int32_t prevFramePhase = 0;

  // prevFramePhase is the framePhase before the tick.
  // We already updated prevFramePhase at end of loop.
  if (prevFramePhase == 4) {
    printAllSensors();
  }

  prevFramePhase = framePhase;
}
