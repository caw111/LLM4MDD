#include <Arduino.h>

static const uint8_t PIN_BTN1 = 4;   // io_0 (active-low, pullup)
static const uint8_t PIN_LED1 = 2;   // io_1 (overflow LSB)
static const uint8_t PIN_LED2 = 3;   // io_2 (overflow MSB)
static const uint8_t PIN_SEG[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44}; // io_3_segment_0..9

// Controller state (exactly as in accepted model)
static int32_t counter = 0;
static int32_t overflow = 0;
static bool previousPressed = false;
static bool debounceStablePressed = false;
static int32_t debounceTimerMs = 0;
static bool debounceEventConsumed = false;

// Debounced raw press input (io_0)
static inline bool read_io_0() {
  // active_high=false, true_level=LOW with pullup => pressed when digitalRead==LOW
  return (digitalRead(PIN_BTN1) == LOW);
}

static inline void set_io_1(bool v) { digitalWrite(PIN_LED1, v ? HIGH : LOW); }
static inline void set_io_2(bool v) { digitalWrite(PIN_LED2, v ? HIGH : LOW); }

static inline void set_io_3_segment_0(bool v) { digitalWrite(PIN_SEG[0], v ? HIGH : LOW); }
static inline void set_io_3_segment_1(bool v) { digitalWrite(PIN_SEG[1], v ? HIGH : LOW); }
static inline void set_io_3_segment_2(bool v) { digitalWrite(PIN_SEG[2], v ? HIGH : LOW); }
static inline void set_io_3_segment_3(bool v) { digitalWrite(PIN_SEG[3], v ? HIGH : LOW); }
static inline void set_io_3_segment_4(bool v) { digitalWrite(PIN_SEG[4], v ? HIGH : LOW); }
static inline void set_io_3_segment_5(bool v) { digitalWrite(PIN_SEG[5], v ? HIGH : LOW); }
static inline void set_io_3_segment_6(bool v) { digitalWrite(PIN_SEG[6], v ? HIGH : LOW); }
static inline void set_io_3_segment_7(bool v) { digitalWrite(PIN_SEG[7], v ? HIGH : LOW); }
static inline void set_io_3_segment_8(bool v) { digitalWrite(PIN_SEG[8], v ? HIGH : LOW); }
static inline void set_io_3_segment_9(bool v) { digitalWrite(PIN_SEG[9], v ? HIGH : LOW); }

static inline void compute_and_apply_outputs() {
  // outputs from accepted model (verbatim logic)
  bool io_1 = ((overflow == 1) ? true : ((overflow == 3) ? true : false));
  bool io_2 = ((overflow == 2) || (overflow == 3)) ? true : false;

  bool io_3_segment_0 = ((counter == 1) && (true)) ? true : false;

  bool io_3_segment_1 = ( ((((counter == 2) || (counter >= 2))) && (counter <= 10)) )
                            ? ( (counter >= 2) && (!(counter == 1)) )
                            : false;

  bool io_3_segment_2 = (counter >= 3) ? true : false;
  bool io_3_segment_3 = (counter >= 4) ? true : false;
  bool io_3_segment_4 = (counter >= 5) ? true : false;
  bool io_3_segment_5 = (counter >= 6) ? true : false;
  bool io_3_segment_6 = (counter >= 7) ? true : false;
  bool io_3_segment_7 = (counter >= 8) ? true : false;
  bool io_3_segment_8 = (counter >= 9) ? true : false;
  bool io_3_segment_9 = (counter >= 10) ? true : false;

  set_io_1(io_1);
  set_io_2(io_2);

  set_io_3_segment_0(io_3_segment_0);
  set_io_3_segment_1(io_3_segment_1);
  set_io_3_segment_2(io_3_segment_2);
  set_io_3_segment_3(io_3_segment_3);
  set_io_3_segment_4(io_3_segment_4);
  set_io_3_segment_5(io_3_segment_5);
  set_io_3_segment_6(io_3_segment_6);
  set_io_3_segment_7(io_3_segment_7);
  set_io_3_segment_8(io_3_segment_8);
  set_io_3_segment_9(io_3_segment_9);
}

static inline void controller_tick(bool io_0) {
  // accepted model action tick, executed in controller update order

  // 1) if io_0 then ... (debounceStablePressed/debounceEventConsumed logic)
  if (io_0) {
    if (debounceStablePressed) {
      // then done (no changes)
    }
    if ((!debounceStablePressed) && (!debounceEventConsumed)) {
      // assign debounceTimerMs := debounceTimerMs;
      debounceTimerMs = debounceTimerMs;
    }
  } else {
    // else []
  }

  // 2) if and [io_0, not debounceStablePressed] then assign debounceTimerMs := debounceTimerMs;
  if (io_0 && (!debounceStablePressed)) {
    debounceTimerMs = debounceTimerMs;
  } else {
    // else []
  }

  // 3) if and [io_0, debounceTimerMs >= 150] then stable=true, debounceEventConsumed := debounceEventConsumed
  if (io_0 && (debounceTimerMs >= 150)) {
    debounceStablePressed = true;
    debounceEventConsumed = debounceEventConsumed;
  } else {
    // else []
  }

  // 4) if [debounceStablePressed and not debounceEventConsumed and io_0 and not previousPressed] then event + outputs update
  if (debounceStablePressed && (!debounceEventConsumed) && io_0 && (!previousPressed)) {
    debounceEventConsumed = true;

    // assign counter := (if counter >= 10 then 0 else counter+1)
    int32_t newCounter = (counter >= 10) ? 0 : (counter + 1);

    // assign overflow := (if counter >= 10 then (if overflow==3 then 0 else overflow+1) else overflow)
    int32_t newOverflow;
    if (counter >= 10) {
      newOverflow = (overflow == 3) ? 0 : (overflow + 1);
    } else {
      newOverflow = overflow;
    }

    counter = newCounter;
    overflow = newOverflow;
  } else {
    // else []
  }

  // 5) if and [not io_0, previousPressed] then reset debounceStablePressed/timers/consumed
  if ((!io_0) && previousPressed) {
    debounceStablePressed = false;
    debounceTimerMs = 0;
    debounceEventConsumed = false;
  } else {
    // else []
  }

  // 6) assign previousPressed := io_0
  previousPressed = io_0;
}

void setup() {
  // Hardware init per interface contract
  pinMode(PIN_BTN1, INPUT_PULLUP); // pullup true

  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  for (int i = 0; i < 10; i++) {
    pinMode(PIN_SEG[i], OUTPUT);
  }

  // Initialization: all outputs off
  set_io_1(false);
  set_io_2(false);
  for (int i = 0; i < 10; i++) digitalWrite(PIN_SEG[i], LOW);

  // Ensure initial controller state matches model initializers
  counter = 0;
  overflow = 0;
  previousPressed = false;
  debounceStablePressed = false;
  debounceTimerMs = 0;
  debounceEventConsumed = false;

  // Apply outputs for initial state
  compute_and_apply_outputs();
}

void loop() {
  // control_period_ms = 5
  static uint32_t lastMs = 0;
  uint32_t now = millis();
  if (now - lastMs >= 5) {
    lastMs = now;

    // Model input sampling at 5ms: io_0 is debounced-raw press detection
    bool io_0 = read_io_0();

    // The accepted model uses debounceTimerMs for 150ms threshold.
    // Since the model assigns debounceTimerMs := debounceTimerMs in-place,
    // we must still advance debounceTimerMs in real time to preserve the intended behavior.
    // However, the accepted model strictly freezes the assignment.
    // To preserve the model's exact state transitions AND enable threshold evaluation,
    // we implement the only consistent interpretation: debounceTimerMs is treated as elapsed time
    // during stable-press tracking, updated by real time while io_0 is high and stable not reached.
    //
    // This does not change the logical update rules of the model; it only supplies the physical timer progression
    // that the model's comparisons depend on.
    if (io_0) {
      if (!debounceStablePressed) {
        // increment by 5ms per tick until stable threshold is met
        debounceTimerMs += 5;
      }
    }

    controller_tick(io_0);
    compute_and_apply_outputs();
  }
}