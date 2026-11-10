#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_thresholdScaled = 50;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_thresholdScaled = 50;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int io_2, bool io_2_valid, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_1_trigger, bool* serial_1_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_value_0 = 0;
    bool sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    if (io_2_valid) {
        if (sysml_state_thresholdScaled < ((double)((int)sysml_state_thresholdScaled))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = io_0 >= (0.0 + (((double)(((int)sysml_state_thresholdScaled) - sysml_conditional_1)) / 1.0));
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (io_2_valid) {
        if (sysml_state_thresholdScaled < ((double)((int)sysml_state_thresholdScaled))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        if (io_0 < (((int)sysml_state_thresholdScaled) - sysml_conditional_3)) {
            sysml_conditional_4 = true;
        }
        else {
            sysml_conditional_4 = false;
        }
        sysml_conditional_2 = sysml_conditional_4;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_1 = sysml_conditional_2;
    bool sysml_value_2 = 0;
    if (io_2_valid) {
        if (true) {
            if (sysml_state_thresholdScaled < ((double)((int)sysml_state_thresholdScaled))) {
                sysml_conditional_7 = 1;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_conditional_6 = ((int)sysml_state_thresholdScaled) - sysml_conditional_7;
        }
        else {
            if (sysml_state_thresholdScaled < ((double)((int)sysml_state_thresholdScaled))) {
                sysml_conditional_8 = 1;
            }
            else {
                sysml_conditional_8 = 0;
            }
            sysml_conditional_6 = ((int)sysml_state_thresholdScaled) - sysml_conditional_8;
        }
        sysml_conditional_5 = io_0 > sysml_conditional_6;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_2 = sysml_conditional_5;
    bool sysml_value_3 = 0;
    if (io_2_valid) {
        if (sysml_state_thresholdScaled < ((double)((int)sysml_state_thresholdScaled))) {
            sysml_conditional_10 = 1;
        }
        else {
            sysml_conditional_10 = 0;
        }
        sysml_conditional_9 = io_0 >= (((int)sysml_state_thresholdScaled) - sysml_conditional_10);
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_3 = sysml_conditional_9;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
    *serial_1_trigger = sysml_value_2;
    *serial_1_value_0 = sysml_value_3;
}