#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_activeIsPot1 = true;
/* Global Variable define of  */
int sysml_state_debounceTimerMs = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_ledsLitCount = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_activeIsPot1 = true;
    sysml_state_debounceTimerMs = 0;
    sysml_state_previousPressed = false;
    sysml_state_ledsLitCount = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, int io_4, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_debounceTimerMs = 0;
    }
    if (io_0) {
        sysml_state_debounceTimerMs = (sysml_state_debounceTimerMs + io_4);
    }
    else {
        sysml_state_debounceTimerMs = 0;
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_debounceTimerMs >= 150;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_2) {
        sysml_state_activeIsPot1 = (!sysml_state_activeIsPot1);
        sysml_state_debounceTimerMs = 0;
    }
    sysml_state_previousPressed = io_0;
    if (sysml_state_activeIsPot1) {
        if (io_1 >= 1023.0) {
            sysml_conditional_4 = 10;
        }
        else {
            if (io_1 <= 0.0) {
                sysml_conditional_5 = 0;
            }
            else {
                if ((((double)(io_1 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_1 * 10.0)) / 1023.0)))) {
                    sysml_conditional_6 = 1;
                }
                else {
                    sysml_conditional_6 = 0;
                }
                sysml_conditional_5 = ((int)(((double)(io_1 * 10.0)) / 1023.0)) - sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        if (io_2 >= 1023.0) {
            sysml_conditional_7 = 10;
        }
        else {
            if (io_2 <= 0.0) {
                sysml_conditional_8 = 0;
            }
            else {
                if ((((double)(io_2 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_2 * 10.0)) / 1023.0)))) {
                    sysml_conditional_9 = 1;
                }
                else {
                    sysml_conditional_9 = 0;
                }
                sysml_conditional_8 = ((int)(((double)(io_2 * 10.0)) / 1023.0)) - sysml_conditional_9;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_3 = sysml_conditional_7;
    }
    sysml_state_ledsLitCount = sysml_conditional_3;
    bool sysml_value_0 = 0;
    if (sysml_state_ledsLitCount >= 1) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_0 = sysml_conditional_10;
    bool sysml_value_1 = 0;
    if (sysml_state_ledsLitCount >= 2) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_1 = sysml_conditional_11;
    bool sysml_value_2 = 0;
    if (sysml_state_ledsLitCount >= 3) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_2 = sysml_conditional_12;
    bool sysml_value_3 = 0;
    if (sysml_state_ledsLitCount >= 4) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_3 = sysml_conditional_13;
    bool sysml_value_4 = 0;
    if (sysml_state_ledsLitCount >= 5) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_4 = sysml_conditional_14;
    bool sysml_value_5 = 0;
    if (sysml_state_ledsLitCount >= 6) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_5 = sysml_conditional_15;
    bool sysml_value_6 = 0;
    if (sysml_state_ledsLitCount >= 7) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_6 = sysml_conditional_16;
    bool sysml_value_7 = 0;
    if (sysml_state_ledsLitCount >= 8) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_7 = sysml_conditional_17;
    bool sysml_value_8 = 0;
    if (sysml_state_ledsLitCount >= 9) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_8 = sysml_conditional_18;
    bool sysml_value_9 = 0;
    if (sysml_state_ledsLitCount >= 10) {
        sysml_conditional_19 = true;
    }
    else {
        sysml_conditional_19 = false;
    }
    sysml_value_9 = sysml_conditional_19;
    *io_3_segment_0 = sysml_value_0;
    *io_3_segment_1 = sysml_value_1;
    *io_3_segment_2 = sysml_value_2;
    *io_3_segment_3 = sysml_value_3;
    *io_3_segment_4 = sysml_value_4;
    *io_3_segment_5 = sysml_value_5;
    *io_3_segment_6 = sysml_value_6;
    *io_3_segment_7 = sysml_value_7;
    *io_3_segment_8 = sysml_value_8;
    *io_3_segment_9 = sysml_value_9;
}