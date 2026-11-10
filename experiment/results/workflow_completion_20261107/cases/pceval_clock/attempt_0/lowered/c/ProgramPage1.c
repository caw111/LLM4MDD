#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_savedDisplayTime = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_savedDisplayTime = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_1, int* io_2, bool* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_arithmetic_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_arithmetic_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_arithmetic_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_arithmetic_17 = 0;
    if (io_1) {
        if ((((double)io_0) / 3600) < ((double)((int)(((double)io_0) / 3600)))) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = 0;
        }
        if ((((double)io_0) / 3600) < ((double)((int)(((double)io_0) / 3600)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        if ((((double)io_0) / 3600) < ((double)((int)(((double)io_0) / 3600)))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        if ((((double)io_0) / 3600) < ((double)((int)(((double)io_0) / 3600)))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        if ((((double)(((int)(((double)io_0) / 3600)) - sysml_conditional_2)) / 24) < ((double)((int)(((double)(((int)(((double)io_0) / 3600)) - sysml_conditional_3)) / 24)))) {
            sysml_conditional_4 = 1;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_arithmetic_5 = ((int)(((double)(((int)(((double)io_0) / 3600)) - sysml_conditional_1)) / 24)) - sysml_conditional_4;
        if ((((double)io_0) / 60) < ((double)((int)(((double)io_0) / 60)))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        if ((((double)io_0) / 60) < ((double)((int)(((double)io_0) / 60)))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        if ((((double)io_0) / 60) < ((double)((int)(((double)io_0) / 60)))) {
            sysml_conditional_8 = 1;
        }
        else {
            sysml_conditional_8 = 0;
        }
        if ((((double)io_0) / 60) < ((double)((int)(((double)io_0) / 60)))) {
            sysml_conditional_9 = 1;
        }
        else {
            sysml_conditional_9 = 0;
        }
        if ((((double)(((int)(((double)io_0) / 60)) - sysml_conditional_8)) / 60) < ((double)((int)(((double)(((int)(((double)io_0) / 60)) - sysml_conditional_9)) / 60)))) {
            sysml_conditional_10 = 1;
        }
        else {
            sysml_conditional_10 = 0;
        }
        sysml_arithmetic_11 = ((int)(((double)(((int)(((double)io_0) / 60)) - sysml_conditional_7)) / 60)) - sysml_conditional_10;
        sysml_state_savedDisplayTime = (((((int)(((double)io_0) / 3600)) - sysml_conditional_0) - (24 * sysml_arithmetic_5)) * 100) + ((((int)(((double)io_0) / 60)) - sysml_conditional_6) - (60 * sysml_arithmetic_11));
    }
    int sysml_value_0 = 0;
    if ((((double)io_0) / 3600) < ((double)((int)(((double)io_0) / 3600)))) {
        sysml_conditional_12 = 1;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_0 = ((int)(((double)io_0) / 3600)) - sysml_conditional_12;
    int sysml_value_1 = 0;
    if ((((double)io_0) / 60) < ((double)((int)(((double)io_0) / 60)))) {
        sysml_conditional_13 = 1;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_1 = ((int)(((double)io_0) / 60)) - sysml_conditional_13;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_savedDisplayTime;
    bool sysml_value_3 = 0;
    sysml_value_3 = true;
    int sysml_value_4 = 0;
    if ((((double)sysml_value_0) / 24) < ((double)((int)(((double)sysml_value_0) / 24)))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    sysml_arithmetic_15 = ((int)(((double)sysml_value_0) / 24)) - sysml_conditional_14;
    sysml_value_4 = sysml_value_0 - (24 * sysml_arithmetic_15);
    int sysml_value_5 = 0;
    if ((((double)sysml_value_1) / 60) < ((double)((int)(((double)sysml_value_1) / 60)))) {
        sysml_conditional_16 = 1;
    }
    else {
        sysml_conditional_16 = 0;
    }
    sysml_arithmetic_17 = ((int)(((double)sysml_value_1) / 60)) - sysml_conditional_16;
    sysml_value_5 = sysml_value_1 - (60 * sysml_arithmetic_17);
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
}