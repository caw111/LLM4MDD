#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_counter = 0;
/* Global Variable define of  */
int sysml_state_overflow = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_debounceStablePressed = false;
/* Global Variable define of  */
int sysml_state_debounceTimerMs = 0;
/* Global Variable define of  */
bool sysml_state_debounceEventConsumed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_counter = 0;
    sysml_state_overflow = 0;
    sysml_state_previousPressed = false;
    sysml_state_debounceStablePressed = false;
    sysml_state_debounceTimerMs = 0;
    sysml_state_debounceEventConsumed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* io_1, bool* io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_conditional_25 = 0;
    bool sysml_conditional_26 = 0;
    bool sysml_conditional_27 = 0;
    if (io_0) {
        if (sysml_state_debounceStablePressed) {
        }
        sysml_lazy_0 = !sysml_state_debounceStablePressed;
        if (sysml_lazy_0) {
            sysml_lazy_0 = !sysml_state_debounceEventConsumed;
        }
        if (sysml_lazy_0) {
            sysml_state_debounceTimerMs = sysml_state_debounceTimerMs;
        }
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_debounceStablePressed;
    }
    if (sysml_lazy_1) {
        sysml_state_debounceTimerMs = sysml_state_debounceTimerMs;
    }
    sysml_lazy_2 = io_0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_debounceTimerMs >= 150;
    }
    if (sysml_lazy_2) {
        sysml_state_debounceStablePressed = true;
        sysml_state_debounceEventConsumed = sysml_state_debounceEventConsumed;
    }
    sysml_lazy_3 = sysml_state_debounceStablePressed;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_debounceEventConsumed;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_5 = io_0;
        if (sysml_lazy_5) {
            sysml_lazy_5 = !sysml_state_previousPressed;
        }
        sysml_lazy_4 = sysml_lazy_5;
    }
    if (sysml_lazy_4) {
        sysml_state_debounceEventConsumed = true;
        if (sysml_state_counter >= 10) {
            sysml_conditional_6 = 0;
        }
        else {
            sysml_conditional_6 = sysml_state_counter + 1;
        }
        sysml_state_counter = sysml_conditional_6;
        if (sysml_state_counter >= 10) {
            if (sysml_state_overflow == 3) {
                sysml_conditional_8 = 0;
            }
            else {
                sysml_conditional_8 = sysml_state_overflow + 1;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        else {
            sysml_conditional_7 = sysml_state_overflow;
        }
        sysml_state_overflow = sysml_conditional_7;
    }
    sysml_lazy_9 = !io_0;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_previousPressed;
    }
    if (sysml_lazy_9) {
        sysml_state_debounceStablePressed = false;
        sysml_state_debounceTimerMs = 0;
        sysml_state_debounceEventConsumed = false;
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    if (sysml_state_overflow == 1) {
        sysml_conditional_10 = true;
    }
    else {
        if (sysml_state_overflow == 3) {
            sysml_conditional_11 = true;
        }
        else {
            sysml_conditional_11 = false;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    sysml_value_0 = sysml_conditional_10;
    bool sysml_value_1 = 0;
    sysml_lazy_12 = sysml_state_overflow == 2;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_overflow == 3;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_1 = sysml_conditional_13;
    bool sysml_value_2 = 0;
    sysml_lazy_14 = sysml_state_counter == 1;
    if (sysml_lazy_14) {
        sysml_lazy_14 = true;
    }
    if (sysml_lazy_14) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_2 = sysml_conditional_15;
    bool sysml_value_3 = 0;
    sysml_lazy_16 = sysml_state_counter == 2;
    if (!sysml_lazy_16) {
        sysml_lazy_16 = sysml_state_counter >= 2;
    }
    sysml_lazy_17 = sysml_lazy_16;
    if (sysml_lazy_17) {
        sysml_lazy_17 = sysml_state_counter <= 10;
    }
    if (sysml_lazy_17) {
        sysml_lazy_19 = sysml_state_counter >= 2;
        if (sysml_lazy_19) {
            sysml_lazy_19 = !(sysml_state_counter == 1);
        }
        sysml_conditional_18 = sysml_lazy_19;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_3 = sysml_conditional_18;
    bool sysml_value_4 = 0;
    if (sysml_state_counter >= 3) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_4 = sysml_conditional_20;
    bool sysml_value_5 = 0;
    if (sysml_state_counter >= 4) {
        sysml_conditional_21 = true;
    }
    else {
        sysml_conditional_21 = false;
    }
    sysml_value_5 = sysml_conditional_21;
    bool sysml_value_6 = 0;
    if (sysml_state_counter >= 5) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_6 = sysml_conditional_22;
    bool sysml_value_7 = 0;
    if (sysml_state_counter >= 6) {
        sysml_conditional_23 = true;
    }
    else {
        sysml_conditional_23 = false;
    }
    sysml_value_7 = sysml_conditional_23;
    bool sysml_value_8 = 0;
    if (sysml_state_counter >= 7) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_8 = sysml_conditional_24;
    bool sysml_value_9 = 0;
    if (sysml_state_counter >= 8) {
        sysml_conditional_25 = true;
    }
    else {
        sysml_conditional_25 = false;
    }
    sysml_value_9 = sysml_conditional_25;
    bool sysml_value_10 = 0;
    if (sysml_state_counter >= 9) {
        sysml_conditional_26 = true;
    }
    else {
        sysml_conditional_26 = false;
    }
    sysml_value_10 = sysml_conditional_26;
    bool sysml_value_11 = 0;
    if (sysml_state_counter >= 10) {
        sysml_conditional_27 = true;
    }
    else {
        sysml_conditional_27 = false;
    }
    sysml_value_11 = sysml_conditional_27;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3_segment_0 = sysml_value_2;
    *io_3_segment_1 = sysml_value_3;
    *io_3_segment_2 = sysml_value_4;
    *io_3_segment_3 = sysml_value_5;
    *io_3_segment_4 = sysml_value_6;
    *io_3_segment_5 = sysml_value_7;
    *io_3_segment_6 = sysml_value_8;
    *io_3_segment_7 = sysml_value_9;
    *io_3_segment_8 = sysml_value_10;
    *io_3_segment_9 = sysml_value_11;
}