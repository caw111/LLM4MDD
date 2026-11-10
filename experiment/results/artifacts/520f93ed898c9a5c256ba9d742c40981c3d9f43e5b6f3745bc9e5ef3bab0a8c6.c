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
bool sysml_state_debounceStable = false;
/* Global Variable define of  */
int sysml_state_debounceCounterMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_counter = 0;
    sysml_state_overflow = 0;
    sysml_state_previousPressed = false;
    sysml_state_debounceStable = false;
    sysml_state_debounceCounterMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* io_1, bool* io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    if (io_0) {
        if (sysml_state_previousPressed) {
            sysml_state_debounceCounterMs = (sysml_state_debounceCounterMs + 5);
        }
        else {
            sysml_state_debounceCounterMs = 0;
        }
        if ((sysml_state_debounceCounterMs >= 150)) {
            sysml_state_debounceStable = true;
        }
    }
    else {
        sysml_state_debounceStable = false;
        sysml_state_debounceCounterMs = 0;
    }
    sysml_lazy_0 = sysml_state_debounceStable;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_1) {
        if ((sysml_state_counter >= 10)) {
            sysml_state_counter = 0;
            sysml_state_overflow = (sysml_state_overflow + 1);
        }
        else {
            sysml_state_counter = (sysml_state_counter + 1);
        }
        sysml_state_debounceStable = false;
        sysml_state_debounceCounterMs = 0;
    }
    sysml_state_previousPressed = io_0;
    if ((sysml_state_overflow == 3)) {
        sysml_state_overflow = 0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_2 = sysml_state_overflow == 1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_overflow == 3;
    }
    sysml_value_0 = sysml_lazy_2;
    bool sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_overflow == 2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_overflow == 3;
    }
    sysml_value_1 = sysml_lazy_3;
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_counter >= 1);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_counter >= 2);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_counter >= 3);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_counter >= 4);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_counter >= 5);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_counter >= 6);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_counter >= 7);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_counter >= 8);
    bool sysml_value_10 = 0;
    sysml_value_10 = (sysml_state_counter >= 9);
    bool sysml_value_11 = 0;
    sysml_value_11 = (sysml_state_counter >= 10);
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