#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mappedValue = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mappedValue = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10) {
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
    if (io_0 <= 0.0) {
        sysml_conditional_0 = 10;
    }
    else {
        if (io_0 >= 1023.0) {
            sysml_conditional_1 = 0;
        }
        else {
            if ((((double)((1023.0 - io_0) * 10.0)) / 1023.0) < ((double)((int)(((double)((1023.0 - io_0) * 10.0)) / 1023.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int)(((double)((1023.0 - io_0) * 10.0)) / 1023.0)) - sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_state_mappedValue = sysml_conditional_0;
    bool sysml_value_0 = 0;
    if (sysml_state_mappedValue >= 1) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_0 = sysml_conditional_3;
    bool sysml_value_1 = 0;
    if (sysml_state_mappedValue >= 2) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_1 = sysml_conditional_4;
    bool sysml_value_2 = 0;
    if (sysml_state_mappedValue >= 3) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_2 = sysml_conditional_5;
    bool sysml_value_3 = 0;
    if (sysml_state_mappedValue >= 4) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_3 = sysml_conditional_6;
    bool sysml_value_4 = 0;
    if (sysml_state_mappedValue >= 5) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_4 = sysml_conditional_7;
    bool sysml_value_5 = 0;
    if (sysml_state_mappedValue >= 6) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_5 = sysml_conditional_8;
    bool sysml_value_6 = 0;
    if (sysml_state_mappedValue >= 7) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_6 = sysml_conditional_9;
    bool sysml_value_7 = 0;
    if (sysml_state_mappedValue >= 8) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_7 = sysml_conditional_10;
    bool sysml_value_8 = 0;
    if (sysml_state_mappedValue >= 9) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_8 = sysml_conditional_11;
    bool sysml_value_9 = 0;
    if (sysml_state_mappedValue >= 10) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
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