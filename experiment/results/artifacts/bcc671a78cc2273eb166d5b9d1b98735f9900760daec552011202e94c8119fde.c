#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_countdown = 9;
/* Global Variable define of  */
int sysml_state_phaseMs = 0;
/* Global Variable define of  */
int sysml_state_ledStep = 0;
/* Global Variable define of  */
int sysml_state_rgbStep = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_countdown = 9;
    sysml_state_phaseMs = 0;
    sysml_state_ledStep = 0;
    sysml_state_rgbStep = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, int* io_10) {
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
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    sysml_state_phaseMs = (sysml_state_phaseMs + io_0);
    if ((sysml_state_phaseMs >= 2000)) {
        sysml_state_phaseMs = (sysml_state_phaseMs - 2000);
        if (sysml_state_countdown <= 0) {
            sysml_conditional_0 = 9;
        }
        else {
            sysml_conditional_0 = sysml_state_countdown - 1;
        }
        sysml_state_countdown = sysml_conditional_0;
        if ((sysml_state_countdown - 1) <= 0) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_ledStep + 1;
        }
        sysml_state_ledStep = sysml_conditional_1;
        if ((sysml_state_countdown - 1) <= 0) {
            sysml_conditional_2 = 0;
        }
        else {
            sysml_conditional_2 = sysml_state_rgbStep + 1;
        }
        sysml_state_rgbStep = sysml_conditional_2;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_countdown == 0) {
        sysml_conditional_3 = false;
    }
    else {
        sysml_conditional_3 = sysml_state_ledStep == 0;
    }
    sysml_value_0 = sysml_conditional_3;
    bool sysml_value_1 = 0;
    if (sysml_state_countdown == 0) {
        sysml_conditional_4 = false;
    }
    else {
        sysml_conditional_4 = sysml_state_ledStep == 1;
    }
    sysml_value_1 = sysml_conditional_4;
    bool sysml_value_2 = 0;
    if (sysml_state_countdown == 0) {
        sysml_conditional_5 = false;
    }
    else {
        sysml_conditional_5 = sysml_state_ledStep == 2;
    }
    sysml_value_2 = sysml_conditional_5;
    bool sysml_value_3 = 0;
    if (sysml_state_countdown == 0) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = sysml_state_rgbStep == 0;
    }
    sysml_value_3 = sysml_conditional_6;
    bool sysml_value_4 = 0;
    if (sysml_state_countdown == 0) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = sysml_state_rgbStep == 1;
    }
    sysml_value_4 = sysml_conditional_7;
    bool sysml_value_5 = 0;
    if (sysml_state_countdown == 0) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = sysml_state_rgbStep == 2;
    }
    sysml_value_5 = sysml_conditional_8;
    bool sysml_value_6 = 0;
    if (sysml_state_countdown == 0) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = sysml_state_rgbStep == 0;
    }
    sysml_value_6 = sysml_conditional_9;
    bool sysml_value_7 = 0;
    if (sysml_state_countdown == 0) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = sysml_state_rgbStep == 1;
    }
    sysml_value_7 = sysml_conditional_10;
    bool sysml_value_8 = 0;
    if (sysml_state_countdown == 0) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = sysml_state_rgbStep == 2;
    }
    sysml_value_8 = sysml_conditional_11;
    int sysml_value_9 = 0;
    if (sysml_state_countdown == 8) {
        sysml_conditional_12 = 88;
    }
    else {
        if (sysml_state_countdown == 0) {
            sysml_conditional_13 = 0;
        }
        else {
            sysml_conditional_13 = sysml_state_countdown;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_9 = sysml_conditional_12;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
    *io_9 = sysml_value_8;
    *io_10 = sysml_value_9;
}