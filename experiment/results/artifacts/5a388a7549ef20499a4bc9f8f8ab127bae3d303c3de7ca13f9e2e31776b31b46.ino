#include <Arduino.h>

//
// Frozen interface contract implementation (control period = 5 ms)
// Inputs (active_low pullups):
//   btn1: D5, btn2: D6, btn3: D7
// Outputs (active_high):
//   LED1 bit0: D2, LED2 bit1: D3, LED3 bit2: D4
// Bargraph segments (only one on for decimal value 0..7):
//   segment0..7 -> D8..D13, D44..D45
//

// Pin bindings
static constexpr uint8_t PIN_BTN1 = 5;
static constexpr uint8_t PIN_BTN2 = 6;
static constexpr uint8_t PIN_BTN3 = 7;

static constexpr uint8_t PIN_LED1 = 2; // valueBit0
static constexpr uint8_t PIN_LED2 = 3; // valueBit1
static constexpr uint8_t PIN_LED3 = 4; // valueBit2

// io_6_segment_0..7 as per contract
static constexpr uint8_t PIN_SEG0 = 8;   // A1
static constexpr uint8_t PIN_SEG1 = 9;   // A2
static constexpr uint8_t PIN_SEG2 = 10;  // A3
static constexpr uint8_t PIN_SEG3 = 11;  // A4
static constexpr uint8_t PIN_SEG4 = 12;  // A5
static constexpr uint8_t PIN_SEG5 = 13;  // A6
static constexpr uint8_t PIN_SEG6 = 44;  // A7
static constexpr uint8_t PIN_SEG7 = 45;  // A8

// Controller state (exact names/semantics from bundle)
static bool btn1PrevPressed = false;
static bool btn2PrevPressed = false;
static bool btn3PrevPressed = false;

static bool btn1StablePressed = false;
static bool btn2StablePressed = false;
static bool btn3StablePressed = false;

static int32_t btn1StableTimerMs = 0;
static int32_t btn2StableTimerMs = 0;
static int32_t btn3StableTimerMs = 0;

static bool valueBit0 = false; // led1
static bool valueBit1 = false; // led2
static bool valueBit2 = false; // led3

// Input signals io_0..io_2 (model semantics: true when button pressed => LOW at MCU)
static bool io_0 = false;
static bool io_1 = false;
static bool io_2 = false;

static bool io_3 = false;
static bool io_4 = false;
static bool io_5 = false;

static bool io_6_segment_0 = false;
static bool io_6_segment_1 = false;
static bool io_6_segment_2 = false;
static bool io_6_segment_3 = false;
static bool io_6_segment_4 = false;
static bool io_6_segment_5 = false;
static bool io_6_segment_6 = false;
static bool io_6_segment_7 = false;

static void writeOutputs()
{
  digitalWrite(PIN_LED1, io_3 ? HIGH : LOW);
  digitalWrite(PIN_LED2, io_4 ? HIGH : LOW);
  digitalWrite(PIN_LED3, io_5 ? HIGH : LOW);

  digitalWrite(PIN_SEG0, io_6_segment_0 ? HIGH : LOW);
  digitalWrite(PIN_SEG1, io_6_segment_1 ? HIGH : LOW);
  digitalWrite(PIN_SEG2, io_6_segment_2 ? HIGH : LOW);
  digitalWrite(PIN_SEG3, io_6_segment_3 ? HIGH : LOW);
  digitalWrite(PIN_SEG4, io_6_segment_4 ? HIGH : LOW);
  digitalWrite(PIN_SEG5, io_6_segment_5 ? HIGH : LOW);
  digitalWrite(PIN_SEG6, io_6_segment_6 ? HIGH : LOW);
  digitalWrite(PIN_SEG7, io_6_segment_7 ? HIGH : LOW);
}

static void tick()
{
  // === SysML/IR/IL order-preserving updates for "perform action tick" ===
  // Board control period: 5 ms (per contract). We increment timers by +5 each tick.
  // Start with inputs already sampled into io_0/io_1/io_2.

  // Update btn1 stable pressed
  if (!io_0) {
    btn1StablePressed = false;
    btn1StableTimerMs = 0;
  } else {
    if (btn1StablePressed) {
      // no change
    } else {
      btn1StableTimerMs = btn1StableTimerMs + 5;
      if (btn1StableTimerMs + 5 >= 150) {
        btn1StablePressed = true;
      }
    }
  }

  // Update btn2 stable pressed
  if (!io_1) {
    btn2StablePressed = false;
    btn2StableTimerMs = 0;
  } else {
    if (btn2StablePressed) {
      // no change
    } else {
      btn2StableTimerMs = btn2StableTimerMs + 5;
      if (btn2StableTimerMs + 5 >= 150) {
        btn2StablePressed = true;
      }
    }
  }

  // Update btn3 stable pressed
  if (!io_2) {
    btn3StablePressed = false;
    btn3StableTimerMs = 0;
  } else {
    if (btn3StablePressed) {
      // no change
    } else {
      btn3StableTimerMs = btn3StableTimerMs + 5;
      if (btn3StableTimerMs + 5 >= 150) {
        btn3StablePressed = true;
      }
    }
  }

  // Edge detection and toggles
  if ((btn1StablePressed) && (!btn1PrevPressed)) {
    valueBit0 = (valueBit0 == false ? true : false);
  }

  if ((btn2StablePressed) && (!btn2PrevPressed)) {
    valueBit1 = (valueBit1 == false ? true : false);
  }

  if ((btn3StablePressed) && (!btn3PrevPressed)) {
    valueBit2 = (valueBit2 == false ? true : false);
  }

  // Update prev pressed
  btn1PrevPressed = btn1StablePressed;
  btn2PrevPressed = btn2StablePressed;
  btn3PrevPressed = btn3StablePressed;

  // Outputs (expressions preserved)
  io_3 = valueBit0;
  io_4 = valueBit1;
  io_5 = valueBit2;

  io_6_segment_0 = ((!valueBit0) && (!valueBit1) && (!valueBit2)) ? true : false; // decimal 0
  io_6_segment_1 = (valueBit0 && (!valueBit1) && (!valueBit2)) ? true : false;  // decimal 1
  io_6_segment_2 = ((!valueBit0) && valueBit1 && (!valueBit2)) ? true : false;  // decimal 2
  io_6_segment_3 = (valueBit0 && valueBit1 && (!valueBit2)) ? true : false;     // decimal 3
  io_6_segment_4 = ((!valueBit0) && (!valueBit1) && valueBit2) ? true : false;  // decimal 4
  io_6_segment_5 = (valueBit0 && (!valueBit1) && valueBit2) ? true : false;     // decimal 5
  io_6_segment_6 = ((!valueBit0) && valueBit1 && valueBit2) ? true : false;     // decimal 6
  io_6_segment_7 = (valueBit0 && valueBit1 && valueBit2) ? true : false;        // decimal 7
}

void setup()
{
  // Inputs with pullups (active_high=false, pullup=true)
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_BTN3, INPUT_PULLUP);

  // Outputs
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_LED3, OUTPUT);

  pinMode(PIN_SEG0, OUTPUT);
  pinMode(PIN_SEG1, OUTPUT);
  pinMode(PIN_SEG2, OUTPUT);
  pinMode(PIN_SEG3, OUTPUT);
  pinMode(PIN_SEG4, OUTPUT);
  pinMode(PIN_SEG5, OUTPUT);
  pinMode(PIN_SEG6, OUTPUT);
  pinMode(PIN_SEG7, OUTPUT);

  // Initialization requirement: all LEDs and bargraph segments off
  valueBit0 = false;
  valueBit1 = false;
  valueBit2 = false;

  btn1PrevPressed = false;
  btn2PrevPressed = false;
  btn3PrevPressed = false;

  btn1StablePressed = false;
  btn2StablePressed = false;
  btn3StablePressed = false;

  btn1StableTimerMs = 0;
  btn2StableTimerMs = 0;
  btn3StableTimerMs = 0;

  io_3 = false;
  io_4 = false;
  io_5 = false;

  io_6_segment_0 = false;
  io_6_segment_1 = false;
  io_6_segment_2 = false;
  io_6_segment_3 = false;
  io_6_segment_4 = false;
  io_6_segment_5 = false;
  io_6_segment_6 = false;
  io_6_segment_7 = false;

  writeOutputs();
}

void loop()
{
  // Enforce control period of 5 ms
  static uint32_t lastMs = 0;
  uint32_t now = millis();
  if (now - lastMs < 5) return;
  lastMs = now;

  // Sample inputs and map to io_0..io_2:
  // active_high=false => model true means button pressed => MCU reads LOW.
  // Requirement: ignore presses shorter than 0.15 seconds handled in tick() via stable timer.
  io_0 = (digitalRead(PIN_BTN1) == LOW);
  io_1 = (digitalRead(PIN_BTN2) == LOW);
  io_2 = (digitalRead(PIN_BTN3) == LOW);

  tick();
  writeOutputs();
}