#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_phaseAccumulatedMs = 0;
/* Global Variable define of  */
int sysml_state_phaseIndex = 0;
/* Global Variable define of  */
bool sysml_state_led1State = false;
/* Global Variable define of  */
bool sysml_state_led2State = false;
/* Global Variable define of  */
bool sysml_state_led3State = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phaseAccumulatedMs = 0;
    sysml_state_phaseIndex = 0;
    sysml_state_led1State = false;
    sysml_state_led2State = false;
    sysml_state_led3State = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_4, bool* io_0, bool* io_1, bool* io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    sysml_state_phaseAccumulatedMs = (sysml_state_phaseAccumulatedMs + io_4);
    if ((sysml_state_phaseAccumulatedMs >= 2000)) {
        sysml_state_phaseAccumulatedMs = (sysml_state_phaseAccumulatedMs - 2000);
        if (sysml_state_phaseIndex == 3) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_phaseIndex + 1;
        }
        sysml_state_phaseIndex = sysml_conditional_0;
    }
    if ((sysml_state_phaseIndex == 0)) {
        sysml_state_led1State = true;
        sysml_state_led2State = false;
        sysml_state_led3State = false;
    }
    if ((sysml_state_phaseIndex == 1)) {
        sysml_state_led1State = false;
        sysml_state_led2State = true;
        sysml_state_led3State = false;
    }
    else {
        if ((sysml_state_phaseIndex == 2)) {
            sysml_state_led1State = false;
            sysml_state_led2State = false;
            sysml_state_led3State = true;
        }
        else {
            if ((sysml_state_phaseIndex == 3)) {
                sysml_state_led1State = false;
                sysml_state_led2State = false;
                sysml_state_led3State = false;
            }
            else {
                sysml_state_led1State = false;
                sysml_state_led2State = false;
                sysml_state_led3State = false;
            }
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_led1State;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_led2State;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_led3State;
    bool sysml_value_3 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_lazy_2 = sysml_state_phaseIndex == 0;
        if (!sysml_lazy_2) {
            sysml_lazy_2 = sysml_state_phaseIndex == 1;
        }
        sysml_lazy_3 = sysml_lazy_2;
        if (!sysml_lazy_3) {
            sysml_lazy_3 = sysml_state_phaseIndex == 2;
        }
        sysml_conditional_1 = sysml_lazy_3;
    }
    sysml_value_3 = sysml_conditional_1;
    bool sysml_value_4 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = sysml_state_phaseIndex == 1;
    }
    sysml_value_4 = sysml_conditional_4;
    bool sysml_value_5 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_5 = sysml_conditional_5;
    bool sysml_value_6 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_6 = sysml_conditional_6;
    bool sysml_value_7 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_7 = sysml_conditional_7;
    bool sysml_value_8 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_8 = sysml_conditional_8;
    bool sysml_value_9 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_9 = sysml_conditional_9;
    bool sysml_value_10 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_10 = sysml_conditional_10;
    bool sysml_value_11 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_11 = sysml_conditional_11;
    bool sysml_value_12 = 0;
    if (sysml_state_phaseIndex == 3) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_12 = sysml_conditional_12;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3_segment_0 = sysml_value_3;
    *io_3_segment_1 = sysml_value_4;
    *io_3_segment_2 = sysml_value_5;
    *io_3_segment_3 = sysml_value_6;
    *io_3_segment_4 = sysml_value_7;
    *io_3_segment_5 = sysml_value_8;
    *io_3_segment_6 = sysml_value_9;
    *io_3_segment_7 = sysml_value_10;
    *io_3_segment_8 = sysml_value_11;
    *io_3_segment_9 = sysml_value_12;
}