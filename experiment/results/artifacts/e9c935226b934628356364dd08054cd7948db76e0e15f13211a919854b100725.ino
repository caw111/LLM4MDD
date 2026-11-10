#ifdef ARDUINO
#include <Arduino.h>
#endif
// BEGIN COMPILED SYSML MODEL
#include <stdint.h>
#include <stdbool.h>
#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int32_t sysml_state_phase;
/* Global Variable define of  */
extern int32_t sysml_state_remainingMs;
/* Global Variable define of  */
extern int32_t sysml_state_roundNumber;
/* Global Variable define of  */
extern int32_t sysml_state_score;
/* Global Variable define of  */
extern int32_t sysml_state_selectedColor;
/* Global Variable define of  */
extern int32_t sysml_state_randomState;
/* Global Variable define of  */
extern bool sysml_state_seenRed;
/* Global Variable define of  */
extern bool sysml_state_seenYellow;
/* Global Variable define of  */
extern bool sysml_state_seenGreen;
/* Global Variable define of  */
extern bool sysml_state_previousStop;
/* Global Variable define of  */
extern bool sysml_state_previousBack;
/* Global Variable define of  */
extern bool sysml_state_previousForward;
/* Global Variable define of  */
extern bool sysml_state_started;
/* Global Variable define of  */
extern bool sysml_state_readyReport;
/* Global Variable define of  */
extern bool sysml_state_wrongReport;
/* Global Variable define of  */
extern bool sysml_state_timeoutReport;
/* Global Variable define of  */
extern bool sysml_state_successReport;
/* Global Variable define of  */
extern bool sysml_state_endReport;
/* Global Variable define of  */
extern bool sysml_state_beginRound;
/* Global Variable define of  */
extern bool sysml_state_finishRound;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, int32_t io_7, bool* io_0, bool* io_1, bool* io_2, int32_t* io_6, bool* io_8, bool* io_9, bool* io_10, bool* serial_0_trigger, int32_t* serial_0_value_0, bool* serial_1_trigger, int32_t* serial_1_value_0);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_phase = 0;
/* Global Variable define of  */
int32_t sysml_state_remainingMs = 3000;
/* Global Variable define of  */
int32_t sysml_state_roundNumber = 0;
/* Global Variable define of  */
int32_t sysml_state_score = 0;
/* Global Variable define of  */
int32_t sysml_state_selectedColor = 0;
/* Global Variable define of  */
int32_t sysml_state_randomState = 173;
/* Global Variable define of  */
bool sysml_state_seenRed = false;
/* Global Variable define of  */
bool sysml_state_seenYellow = false;
/* Global Variable define of  */
bool sysml_state_seenGreen = false;
/* Global Variable define of  */
bool sysml_state_previousStop = false;
/* Global Variable define of  */
bool sysml_state_previousBack = false;
/* Global Variable define of  */
bool sysml_state_previousForward = false;
/* Global Variable define of  */
bool sysml_state_started = false;
/* Global Variable define of  */
bool sysml_state_readyReport = false;
/* Global Variable define of  */
bool sysml_state_wrongReport = false;
/* Global Variable define of  */
bool sysml_state_timeoutReport = false;
/* Global Variable define of  */
bool sysml_state_successReport = false;
/* Global Variable define of  */
bool sysml_state_endReport = false;
/* Global Variable define of  */
bool sysml_state_beginRound = false;
/* Global Variable define of  */
bool sysml_state_finishRound = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phase = 0;
    sysml_state_remainingMs = 3000;
    sysml_state_roundNumber = 0;
    sysml_state_score = 0;
    sysml_state_selectedColor = 0;
    sysml_state_randomState = 173;
    sysml_state_seenRed = false;
    sysml_state_seenYellow = false;
    sysml_state_seenGreen = false;
    sysml_state_previousStop = false;
    sysml_state_previousBack = false;
    sysml_state_previousForward = false;
    sysml_state_started = false;
    sysml_state_readyReport = false;
    sysml_state_wrongReport = false;
    sysml_state_timeoutReport = false;
    sysml_state_successReport = false;
    sysml_state_endReport = false;
    sysml_state_beginRound = false;
    sysml_state_finishRound = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, int32_t io_7, bool* io_0, bool* io_1, bool* io_2, int32_t* io_6, bool* io_8, bool* io_9, bool* io_10, bool* serial_0_trigger, int32_t* serial_0_value_0, bool* serial_1_trigger, int32_t* serial_1_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_arithmetic_14 = 0;
    int32_t sysml_conditional_15 = 0;
    int32_t sysml_arithmetic_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    int32_t sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    int32_t sysml_conditional_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_lazy_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_lazy_33 = 0;
    bool sysml_lazy_34 = 0;
    bool sysml_lazy_35 = 0;
    bool sysml_lazy_36 = 0;
    bool sysml_lazy_37 = 0;
    bool sysml_lazy_38 = 0;
    bool sysml_lazy_39 = 0;
    bool sysml_lazy_40 = 0;
    bool sysml_lazy_41 = 0;
    bool sysml_lazy_42 = 0;
    bool sysml_lazy_43 = 0;
    bool sysml_lazy_44 = 0;
    bool sysml_lazy_45 = 0;
    bool sysml_lazy_46 = 0;
    sysml_state_readyReport = (!sysml_state_started);
    sysml_state_started = true;
    sysml_state_wrongReport = false;
    sysml_state_timeoutReport = false;
    sysml_state_successReport = false;
    sysml_state_endReport = false;
    sysml_state_beginRound = false;
    sysml_state_finishRound = false;
    if ((sysml_state_phase == 0)) {
        if ((io_7 >= sysml_state_remainingMs)) {
            sysml_state_remainingMs = 0;
            sysml_state_beginRound = true;
        }
        else {
            sysml_state_remainingMs = (sysml_state_remainingMs - io_7);
        }
    }
    else {
        if ((sysml_state_phase == 1)) {
            if ((io_7 >= sysml_state_remainingMs)) {
                sysml_state_timeoutReport = true;
                sysml_state_finishRound = true;
            }
            else {
                sysml_state_remainingMs = (sysml_state_remainingMs - io_7);
                sysml_lazy_0 = sysml_state_selectedColor == 0;
                if (sysml_lazy_0) {
                    sysml_lazy_1 = io_3;
                    if (sysml_lazy_1) {
                        sysml_lazy_1 = !sysml_state_previousStop;
                    }
                    sysml_lazy_0 = sysml_lazy_1;
                }
                sysml_lazy_2 = sysml_lazy_0;
                if (!sysml_lazy_2) {
                    sysml_lazy_3 = sysml_state_selectedColor == 1;
                    if (sysml_lazy_3) {
                        sysml_lazy_4 = io_4;
                        if (sysml_lazy_4) {
                            sysml_lazy_4 = !sysml_state_previousBack;
                        }
                        sysml_lazy_3 = sysml_lazy_4;
                    }
                    sysml_lazy_2 = sysml_lazy_3;
                }
                sysml_lazy_5 = sysml_lazy_2;
                if (!sysml_lazy_5) {
                    sysml_lazy_6 = sysml_state_selectedColor == 2;
                    if (sysml_lazy_6) {
                        sysml_lazy_7 = io_5;
                        if (sysml_lazy_7) {
                            sysml_lazy_7 = !sysml_state_previousForward;
                        }
                        sysml_lazy_6 = sysml_lazy_7;
                    }
                    sysml_lazy_5 = sysml_lazy_6;
                }
                if (sysml_lazy_5) {
                    sysml_state_score = (sysml_state_score + 1);
                    sysml_state_successReport = true;
                    sysml_state_finishRound = true;
                }
                else {
                    sysml_lazy_8 = io_3;
                    if (sysml_lazy_8) {
                        sysml_lazy_8 = !sysml_state_previousStop;
                    }
                    sysml_lazy_9 = sysml_lazy_8;
                    if (!sysml_lazy_9) {
                        sysml_lazy_10 = io_4;
                        if (sysml_lazy_10) {
                            sysml_lazy_10 = !sysml_state_previousBack;
                        }
                        sysml_lazy_9 = sysml_lazy_10;
                    }
                    sysml_lazy_11 = sysml_lazy_9;
                    if (!sysml_lazy_11) {
                        sysml_lazy_12 = io_5;
                        if (sysml_lazy_12) {
                            sysml_lazy_12 = !sysml_state_previousForward;
                        }
                        sysml_lazy_11 = sysml_lazy_12;
                    }
                    if (sysml_lazy_11) {
                        sysml_state_wrongReport = true;
                    }
                }
            }
        }
        else {
            if ((sysml_state_phase == 2)) {
                if ((io_7 >= sysml_state_remainingMs)) {
                    sysml_state_remainingMs = 0;
                    sysml_state_beginRound = true;
                }
                else {
                    sysml_state_remainingMs = (sysml_state_remainingMs - io_7);
                }
            }
        }
    }
    if (sysml_state_finishRound) {
        if ((sysml_state_roundNumber == 5)) {
            sysml_state_phase = 3;
            sysml_state_remainingMs = 0;
            sysml_state_endReport = true;
        }
        else {
            sysml_state_phase = 2;
            sysml_state_remainingMs = 2000;
        }
    }
    if (sysml_state_beginRound) {
        sysml_state_randomState = ((sysml_state_randomState * 25173) + 13849);
        if ((((double)sysml_state_randomState) / 65536) < ((double)((int32_t)(((double)sysml_state_randomState) / 65536)))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        sysml_arithmetic_14 = ((int32_t)(((double)sysml_state_randomState) / 65536)) - sysml_conditional_13;
        sysml_state_randomState = sysml_state_randomState - (65536 * sysml_arithmetic_14);
        if ((((double)sysml_state_randomState) / 3) < ((double)((int32_t)(((double)sysml_state_randomState) / 3)))) {
            sysml_conditional_15 = 1;
        }
        else {
            sysml_conditional_15 = 0;
        }
        sysml_arithmetic_16 = ((int32_t)(((double)sysml_state_randomState) / 3)) - sysml_conditional_15;
        sysml_state_selectedColor = sysml_state_randomState - (3 * sysml_arithmetic_16);
        if ((sysml_state_roundNumber < 3)) {
            sysml_lazy_17 = sysml_state_selectedColor == 0;
            if (sysml_lazy_17) {
                sysml_lazy_17 = sysml_state_seenRed;
            }
            sysml_lazy_18 = sysml_lazy_17;
            if (!sysml_lazy_18) {
                sysml_lazy_19 = sysml_state_selectedColor == 1;
                if (sysml_lazy_19) {
                    sysml_lazy_19 = sysml_state_seenYellow;
                }
                sysml_lazy_18 = sysml_lazy_19;
            }
            sysml_lazy_20 = sysml_lazy_18;
            if (!sysml_lazy_20) {
                sysml_lazy_21 = sysml_state_selectedColor == 2;
                if (sysml_lazy_21) {
                    sysml_lazy_21 = sysml_state_seenGreen;
                }
                sysml_lazy_20 = sysml_lazy_21;
            }
            if (sysml_lazy_20) {
                if (sysml_state_selectedColor == 2) {
                    sysml_conditional_22 = 0;
                }
                else {
                    sysml_conditional_22 = sysml_state_selectedColor + 1;
                }
                sysml_state_selectedColor = sysml_conditional_22;
            }
            sysml_lazy_23 = sysml_state_selectedColor == 0;
            if (sysml_lazy_23) {
                sysml_lazy_23 = sysml_state_seenRed;
            }
            sysml_lazy_24 = sysml_lazy_23;
            if (!sysml_lazy_24) {
                sysml_lazy_25 = sysml_state_selectedColor == 1;
                if (sysml_lazy_25) {
                    sysml_lazy_25 = sysml_state_seenYellow;
                }
                sysml_lazy_24 = sysml_lazy_25;
            }
            sysml_lazy_26 = sysml_lazy_24;
            if (!sysml_lazy_26) {
                sysml_lazy_27 = sysml_state_selectedColor == 2;
                if (sysml_lazy_27) {
                    sysml_lazy_27 = sysml_state_seenGreen;
                }
                sysml_lazy_26 = sysml_lazy_27;
            }
            if (sysml_lazy_26) {
                if (sysml_state_selectedColor == 2) {
                    sysml_conditional_28 = 0;
                }
                else {
                    sysml_conditional_28 = sysml_state_selectedColor + 1;
                }
                sysml_state_selectedColor = sysml_conditional_28;
            }
        }
        if ((sysml_state_selectedColor == 0)) {
            sysml_state_seenRed = true;
        }
        else {
            if ((sysml_state_selectedColor == 1)) {
                sysml_state_seenYellow = true;
            }
            else {
                sysml_state_seenGreen = true;
            }
        }
        sysml_state_roundNumber = (sysml_state_roundNumber + 1);
        sysml_state_remainingMs = 5000;
        sysml_state_phase = 1;
    }
    sysml_state_previousStop = io_3;
    sysml_state_previousBack = io_4;
    sysml_state_previousForward = io_5;
    bool sysml_value_0 = 0;
    sysml_lazy_29 = io_3;
    if (sysml_lazy_29) {
        sysml_lazy_29 = !sysml_state_previousStop;
    }
    sysml_value_0 = sysml_lazy_29;
    bool sysml_value_1 = 0;
    sysml_lazy_30 = io_4;
    if (sysml_lazy_30) {
        sysml_lazy_30 = !sysml_state_previousBack;
    }
    sysml_value_1 = sysml_lazy_30;
    bool sysml_value_2 = 0;
    sysml_lazy_31 = io_5;
    if (sysml_lazy_31) {
        sysml_lazy_31 = !sysml_state_previousForward;
    }
    sysml_value_2 = sysml_lazy_31;
    bool sysml_value_3 = 0;
    sysml_lazy_32 = sysml_state_selectedColor == 0;
    if (sysml_lazy_32) {
        sysml_lazy_32 = sysml_state_seenRed;
    }
    sysml_lazy_33 = sysml_lazy_32;
    if (!sysml_lazy_33) {
        sysml_lazy_34 = sysml_state_selectedColor == 1;
        if (sysml_lazy_34) {
            sysml_lazy_34 = sysml_state_seenYellow;
        }
        sysml_lazy_33 = sysml_lazy_34;
    }
    sysml_lazy_35 = sysml_lazy_33;
    if (!sysml_lazy_35) {
        sysml_lazy_36 = sysml_state_selectedColor == 2;
        if (sysml_lazy_36) {
            sysml_lazy_36 = sysml_state_seenGreen;
        }
        sysml_lazy_35 = sysml_lazy_36;
    }
    sysml_value_3 = sysml_lazy_35;
    bool sysml_value_4 = 0;
    sysml_lazy_37 = sysml_state_phase == 1;
    if (sysml_lazy_37) {
        sysml_lazy_37 = sysml_state_selectedColor == 0;
    }
    sysml_value_4 = sysml_lazy_37;
    bool sysml_value_5 = 0;
    sysml_lazy_38 = sysml_state_phase == 1;
    if (sysml_lazy_38) {
        sysml_lazy_38 = sysml_state_selectedColor == 1;
    }
    sysml_value_5 = sysml_lazy_38;
    bool sysml_value_6 = 0;
    sysml_lazy_39 = sysml_state_phase == 1;
    if (sysml_lazy_39) {
        sysml_lazy_39 = sysml_state_selectedColor == 2;
    }
    sysml_value_6 = sysml_lazy_39;
    int32_t sysml_value_7 = 0;
    sysml_value_7 = sysml_state_score;
    bool sysml_value_8 = 0;
    sysml_value_8 = sysml_state_readyReport;
    bool sysml_value_9 = 0;
    sysml_value_9 = sysml_state_wrongReport;
    bool sysml_value_10 = 0;
    sysml_value_10 = sysml_state_timeoutReport;
    bool sysml_value_11 = 0;
    sysml_value_11 = sysml_state_successReport;
    int32_t sysml_value_12 = 0;
    sysml_value_12 = sysml_state_score;
    bool sysml_value_13 = 0;
    sysml_value_13 = sysml_state_endReport;
    int32_t sysml_value_14 = 0;
    sysml_value_14 = sysml_state_score;
    bool sysml_value_15 = 0;
    sysml_lazy_40 = sysml_state_selectedColor == 0;
    if (sysml_lazy_40) {
        sysml_lazy_40 = sysml_value_0;
    }
    sysml_lazy_41 = sysml_lazy_40;
    if (!sysml_lazy_41) {
        sysml_lazy_42 = sysml_state_selectedColor == 1;
        if (sysml_lazy_42) {
            sysml_lazy_42 = sysml_value_1;
        }
        sysml_lazy_41 = sysml_lazy_42;
    }
    sysml_lazy_43 = sysml_lazy_41;
    if (!sysml_lazy_43) {
        sysml_lazy_44 = sysml_state_selectedColor == 2;
        if (sysml_lazy_44) {
            sysml_lazy_44 = sysml_value_2;
        }
        sysml_lazy_43 = sysml_lazy_44;
    }
    sysml_value_15 = sysml_lazy_43;
    bool sysml_value_16 = 0;
    sysml_lazy_45 = sysml_value_0;
    if (!sysml_lazy_45) {
        sysml_lazy_45 = sysml_value_1;
    }
    sysml_lazy_46 = sysml_lazy_45;
    if (!sysml_lazy_46) {
        sysml_lazy_46 = sysml_value_2;
    }
    sysml_value_16 = sysml_lazy_46;
    *io_0 = sysml_value_4;
    *io_1 = sysml_value_5;
    *io_2 = sysml_value_6;
    *io_6 = sysml_value_7;
    *io_8 = sysml_value_8;
    *io_9 = sysml_value_9;
    *io_10 = sysml_value_10;
    *serial_0_trigger = sysml_value_11;
    *serial_0_value_0 = sysml_value_12;
    *serial_1_trigger = sysml_value_13;
    *serial_1_value_0 = sysml_value_14;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:mega
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_MEGA2560)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static bool hw_value_io_0 = false;
static bool hw_value_io_1 = false;
static bool hw_value_io_2 = false;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4 = false;
static uint32_t hw_sampled_io_4 = 0;
static bool hw_attempted_io_4 = false;
static bool hw_value_io_5 = false;
static uint32_t hw_sampled_io_5 = 0;
static bool hw_attempted_io_5 = false;
static int32_t hw_value_io_6 = 0;
static void hw_shift595_io_6(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 10L;
  digitalWrite(A1, LOW);
  {
    uint8_t segments = valid && (true) ? digits[(value / 1L) % 10] : 0;
    uint8_t bits = 0;
    if (segments & 1) bits |= 1;
    if (segments & 2) bits |= 2;
    if (segments & 4) bits |= 4;
    if (segments & 8) bits |= 8;
    if (segments & 16) bits |= 16;
    if (segments & 32) bits |= 32;
    if (segments & 64) bits |= 64;
    bits = (uint8_t)~bits;
    shiftOut(A0, A2, MSBFIRST, bits);
  }
  digitalWrite(A1, HIGH);
}
static int32_t hw_shift595_previous_io_6 = INT32_MIN;
static int32_t hw_value_io_7 = 0;
static bool hw_value_io_8 = false;
static bool hw_value_io_9 = false;
static bool hw_value_io_10 = false;
static bool hw_value_serial_0_trigger = false;
static int32_t hw_value_serial_0_value_0 = 0;
static bool hw_value_serial_1_trigger = false;
static int32_t hw_value_serial_1_value_0 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_6 != hw_shift595_previous_io_6) {
    hw_shift595_io_6(hw_value_io_6); hw_shift595_previous_io_6 = hw_value_io_6;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(5) == LOW);
  }
  if (!hw_attempted_io_4 || (uint32_t)(hw_now - hw_sampled_io_4) >= 5UL) {
    hw_attempted_io_4 = true; hw_sampled_io_4 = hw_now;
    hw_value_io_4 = (digitalRead(6) == LOW);
  }
  if (!hw_attempted_io_5 || (uint32_t)(hw_now - hw_sampled_io_5) >= 5UL) {
    hw_attempted_io_5 = true; hw_sampled_io_5 = hw_now;
    hw_value_io_5 = (digitalRead(7) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_7 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_3, hw_value_io_4, hw_value_io_5, hw_value_io_7, &hw_value_io_0, &hw_value_io_1, &hw_value_io_2, &hw_value_io_6, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10, &hw_value_serial_0_trigger, &hw_value_serial_0_value_0, &hw_value_serial_1_trigger, &hw_value_serial_1_value_0);
  digitalWrite(2, hw_value_io_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2 ? HIGH : LOW);
  if (hw_value_io_6 != hw_shift595_previous_io_6) {
    hw_shift595_io_6(hw_value_io_6); hw_shift595_previous_io_6 = hw_value_io_6;
  }
  if (hw_value_io_8) Serial.println("Ready Go!");
  if (hw_value_io_9) Serial.println("Come On!");
  if (hw_value_io_10) Serial.println("Time's up!");
  if (hw_value_serial_0_trigger) { Serial.print("Good Job! x"); Serial.println(hw_value_serial_0_value_0); }
  if (hw_value_serial_1_trigger) { Serial.print("Game Over! Your score is "); Serial.println(hw_value_serial_1_value_0); }
}
