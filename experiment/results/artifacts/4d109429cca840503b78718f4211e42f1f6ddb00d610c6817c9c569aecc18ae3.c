#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_displayValue = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayValue = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, int* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_arithmetic_11 = 0;
    if ((io_0 * 1.0) >= 0.0) {
        if (((((double)io_0) / 1023.0) * 9.0) < ((double)((int)((((double)io_0) / 1023.0) * 9.0)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        if (((((double)io_0) / 1023.0) * 9.0) < ((double)((int)((((double)io_0) / 1023.0) * 9.0)))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        if (((((double)io_0) / 1023.0) * 9.0) < ((double)((int)((((double)io_0) / 1023.0) * 9.0)))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        if ((((double)(((int)((((double)io_0) / 1023.0) * 9.0)) - sysml_conditional_2)) / 1.0) < ((double)((int)(((double)(((int)((((double)io_0) / 1023.0) * 9.0)) - sysml_conditional_3)) / 1.0)))) {
            sysml_conditional_4 = 1;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_conditional_0 = ((int)(((double)(((int)((((double)io_0) / 1023.0) * 9.0)) - sysml_conditional_1)) / 1.0)) - sysml_conditional_4;
    }
    else {
        sysml_conditional_0 = 0;
    }
    if ((io_1 * 1.0) >= 0.0) {
        if (((((double)io_1) / 1023.0) * 9.0) < ((double)((int)((((double)io_1) / 1023.0) * 9.0)))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        if (((((double)io_1) / 1023.0) * 9.0) < ((double)((int)((((double)io_1) / 1023.0) * 9.0)))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        if (((((double)io_1) / 1023.0) * 9.0) < ((double)((int)((((double)io_1) / 1023.0) * 9.0)))) {
            sysml_conditional_8 = 1;
        }
        else {
            sysml_conditional_8 = 0;
        }
        if ((((double)(((int)((((double)io_1) / 1023.0) * 9.0)) - sysml_conditional_7)) / 1.0) < ((double)((int)(((double)(((int)((((double)io_1) / 1023.0) * 9.0)) - sysml_conditional_8)) / 1.0)))) {
            sysml_conditional_9 = 1;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_conditional_5 = ((int)(((double)(((int)((((double)io_1) / 1023.0) * 9.0)) - sysml_conditional_6)) / 1.0)) - sysml_conditional_9;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_state_displayValue = sysml_conditional_0 + sysml_conditional_5;
    if ((((double)sysml_state_displayValue) / 10) < ((double)((int)(((double)sysml_state_displayValue) / 10)))) {
        sysml_conditional_10 = 1;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_arithmetic_11 = ((int)(((double)sysml_state_displayValue) / 10)) - sysml_conditional_10;
    sysml_state_displayValue = sysml_state_displayValue - (10 * sysml_arithmetic_11);
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_displayValue;
    *io_2 = sysml_value_0;
}