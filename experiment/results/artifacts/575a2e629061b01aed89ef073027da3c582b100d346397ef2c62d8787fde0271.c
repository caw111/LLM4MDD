#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_phaseIndex = 0;
/* Global Variable define of  */
bool sysml_state_direction = true;
/* Global Variable define of  */
bool sysml_state_justAdvanced = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phaseIndex = 0;
    sysml_state_direction = true;
    sysml_state_justAdvanced = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    if ((!sysml_state_justAdvanced)) {
        sysml_state_justAdvanced = true;
    }
    else {
        if (sysml_state_direction) {
            if (sysml_state_phaseIndex == 10) {
                sysml_conditional_1 = 9;
            }
            else {
                sysml_conditional_1 = sysml_state_phaseIndex + 1;
            }
            sysml_conditional_0 = sysml_conditional_1;
        }
        else {
            if (sysml_state_phaseIndex == 1) {
                sysml_conditional_2 = 2;
            }
            else {
                sysml_conditional_2 = sysml_state_phaseIndex - 1;
            }
            sysml_conditional_0 = sysml_conditional_2;
        }
        sysml_state_phaseIndex = sysml_conditional_0;
        if (sysml_state_direction) {
            if (sysml_state_phaseIndex == 10) {
                sysml_conditional_4 = false;
            }
            else {
                sysml_conditional_4 = true;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        else {
            if (sysml_state_phaseIndex == 1) {
                sysml_conditional_5 = true;
            }
            else {
                sysml_conditional_5 = false;
            }
            sysml_conditional_3 = sysml_conditional_5;
        }
        sysml_state_direction = sysml_conditional_3;
        sysml_state_justAdvanced = false;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = false;
    bool sysml_value_1 = 0;
    if (sysml_state_direction) {
        sysml_conditional_6 = sysml_state_phaseIndex == 1;
    }
    else {
        sysml_conditional_6 = sysml_state_phaseIndex == 9;
    }
    sysml_value_1 = sysml_conditional_6;
    bool sysml_value_2 = 0;
    if (sysml_state_direction) {
        sysml_conditional_7 = sysml_state_phaseIndex == 2;
    }
    else {
        sysml_conditional_7 = sysml_state_phaseIndex == 8;
    }
    sysml_value_2 = sysml_conditional_7;
    bool sysml_value_3 = 0;
    if (sysml_state_direction) {
        sysml_conditional_8 = sysml_state_phaseIndex == 3;
    }
    else {
        sysml_conditional_8 = sysml_state_phaseIndex == 7;
    }
    sysml_value_3 = sysml_conditional_8;
    bool sysml_value_4 = 0;
    if (sysml_state_direction) {
        sysml_conditional_9 = sysml_state_phaseIndex == 4;
    }
    else {
        sysml_conditional_9 = sysml_state_phaseIndex == 6;
    }
    sysml_value_4 = sysml_conditional_9;
    bool sysml_value_5 = 0;
    if (sysml_state_direction) {
        sysml_conditional_10 = sysml_state_phaseIndex == 5;
    }
    else {
        sysml_conditional_10 = sysml_state_phaseIndex == 5;
    }
    sysml_value_5 = sysml_conditional_10;
    bool sysml_value_6 = 0;
    if (sysml_state_direction) {
        sysml_conditional_11 = sysml_state_phaseIndex == 6;
    }
    else {
        sysml_conditional_11 = sysml_state_phaseIndex == 4;
    }
    sysml_value_6 = sysml_conditional_11;
    bool sysml_value_7 = 0;
    if (sysml_state_direction) {
        sysml_conditional_12 = sysml_state_phaseIndex == 7;
    }
    else {
        sysml_conditional_12 = sysml_state_phaseIndex == 3;
    }
    sysml_value_7 = sysml_conditional_12;
    bool sysml_value_8 = 0;
    if (sysml_state_direction) {
        sysml_conditional_13 = sysml_state_phaseIndex == 8;
    }
    else {
        sysml_conditional_13 = sysml_state_phaseIndex == 2;
    }
    sysml_value_8 = sysml_conditional_13;
    bool sysml_value_9 = 0;
    if (sysml_state_direction) {
        sysml_conditional_14 = sysml_state_phaseIndex == 9;
    }
    else {
        sysml_conditional_14 = sysml_state_phaseIndex == 1;
    }
    sysml_value_9 = sysml_conditional_14;
    bool sysml_value_10 = 0;
    if (sysml_state_direction) {
        sysml_conditional_15 = sysml_state_phaseIndex == 10;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_10 = sysml_conditional_15;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_8 = sysml_value_8;
    *io_9 = sysml_value_9;
    *io_10 = sysml_value_10;
}