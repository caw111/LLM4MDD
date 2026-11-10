#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_stateIndex = 0;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Global Variable define of  */
bool sysml_state_debounceStable = false;
/* Global Variable define of  */
int sysml_state_debounceMs = 0;
/* Global Variable define of  */
int sysml_state_lockMs = 0;
/* Global Variable define of  */
int sysml_state_maintainMs = 2000;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stateIndex = 0;
    sysml_state_prevPressed = false;
    sysml_state_debounceStable = false;
    sysml_state_debounceMs = 0;
    sysml_state_lockMs = 0;
    sysml_state_maintainMs = 2000;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double* io_1, double* io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    if ((sysml_state_lockMs > 0)) {
        sysml_state_lockMs = 0;
    }
    if ((sysml_state_lockMs > 0)) {
    }
    else {
        if (sysml_state_debounceStable) {
            sysml_lazy_0 = sysml_state_prevPressed;
            if (sysml_lazy_0) {
                sysml_lazy_0 = !sysml_state_debounceStable;
            }
            if (sysml_lazy_0) {
                sysml_state_debounceStable = false;
            }
        }
        if (io_0) {
            if ((sysml_state_debounceMs >= 15)) {
                sysml_state_debounceStable = true;
            }
            else {
                sysml_state_debounceMs = (sysml_state_debounceMs + 10);
            }
        }
        else {
            sysml_state_debounceMs = 0;
            sysml_state_debounceStable = false;
        }
        sysml_lazy_1 = sysml_state_debounceStable;
        if (sysml_lazy_1) {
            sysml_lazy_1 = io_0;
        }
        sysml_lazy_2 = sysml_lazy_1;
        if (sysml_lazy_2) {
            sysml_lazy_2 = !sysml_state_prevPressed;
        }
        if (sysml_lazy_2) {
            if (sysml_state_stateIndex == 0) {
                sysml_conditional_3 = 1;
            }
            else {
                if (sysml_state_stateIndex == 1) {
                    sysml_conditional_4 = 2;
                }
                else {
                    sysml_conditional_4 = 0;
                }
                sysml_conditional_3 = sysml_conditional_4;
            }
            sysml_state_stateIndex = sysml_conditional_3;
            sysml_state_lockMs = 2000;
            sysml_state_maintainMs = 2000;
        }
    }
    sysml_lazy_5 = sysml_state_maintainMs > 0;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !(sysml_state_lockMs == 0);
    }
    if (sysml_lazy_5) {
        sysml_state_maintainMs = (sysml_state_maintainMs - 10);
    }
    else {
        if ((sysml_state_maintainMs > 0)) {
            sysml_state_maintainMs = (sysml_state_maintainMs - 10);
        }
    }
    if ((sysml_state_lockMs > 0)) {
        sysml_state_lockMs = (sysml_state_lockMs - 10);
        sysml_lazy_6 = (sysml_state_lockMs - 10) <= 0;
        if (sysml_lazy_6) {
            sysml_lazy_6 = sysml_state_maintainMs > 0;
        }
        if (sysml_lazy_6) {
            sysml_state_lockMs = 0;
        }
    }
    sysml_state_prevPressed = io_0;
    double sysml_value_0 = 0;
    if (sysml_state_stateIndex == 0) {
        sysml_conditional_7 = 0.0;
    }
    else {
        if (sysml_state_stateIndex == 1) {
            sysml_conditional_8 = 90.0;
        }
        else {
            sysml_conditional_8 = 180.0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    sysml_value_0 = sysml_conditional_7;
    double sysml_value_1 = 0;
    if (sysml_state_stateIndex == 0) {
        sysml_conditional_9 = 180.0;
    }
    else {
        if (sysml_state_stateIndex == 1) {
            sysml_conditional_10 = 90.0;
        }
        else {
            sysml_conditional_10 = 0.0;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_1 = sysml_conditional_9;
    bool sysml_value_2 = 0;
    sysml_lazy_11 = sysml_state_stateIndex == 1;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_stateIndex == 2;
    }
    sysml_value_2 = sysml_lazy_11;
    bool sysml_value_3 = 0;
    sysml_lazy_12 = sysml_state_stateIndex == 1;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_stateIndex == 2;
    }
    sysml_value_3 = sysml_lazy_12;
    bool sysml_value_4 = 0;
    sysml_lazy_13 = sysml_state_stateIndex == 1;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_stateIndex == 2;
    }
    sysml_value_4 = sysml_lazy_13;
    bool sysml_value_5 = 0;
    sysml_lazy_14 = sysml_state_stateIndex == 1;
    if (!sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_stateIndex == 2;
    }
    sysml_value_5 = sysml_lazy_14;
    bool sysml_value_6 = 0;
    sysml_lazy_15 = sysml_state_stateIndex == 1;
    if (!sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_stateIndex == 2;
    }
    sysml_value_6 = sysml_lazy_15;
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_stateIndex == 2);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_stateIndex == 2);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_stateIndex == 2);
    bool sysml_value_10 = 0;
    sysml_value_10 = (sysml_state_stateIndex == 2);
    bool sysml_value_11 = 0;
    sysml_value_11 = (sysml_state_stateIndex == 2);
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