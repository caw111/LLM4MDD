#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_displayNumber = 0;
/* Global Variable define of  */
double sysml_state_brightnessLed1 = 0.0;
/* Global Variable define of  */
double sysml_state_brightnessLed2 = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayNumber = 0;
    sysml_state_brightnessLed1 = 0.0;
    sysml_state_brightnessLed2 = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_3, double io_4, double io_5, int* io_0, double* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_arithmetic_2 = 0;
    double sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_arithmetic_5 = 0;
    double sysml_conditional_6 = 0;
    if ((((double)(io_5 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_5 * 99.0)) / 1023.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_displayNumber = ((int)(((double)(io_5 * 99.0)) / 1023.0)) - sysml_conditional_0;
    if ((((double)sysml_state_displayNumber) / 2) < ((double)((int)(((double)sysml_state_displayNumber) / 2)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_arithmetic_2 = ((int)(((double)sysml_state_displayNumber) / 2)) - sysml_conditional_1;
    if (sysml_state_displayNumber == (2 * sysml_arithmetic_2)) {
        sysml_conditional_3 = ((double)(io_3 * 255.0)) / 1023.0;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_state_brightnessLed1 = sysml_conditional_3;
    if ((((double)sysml_state_displayNumber) / 2) < ((double)((int)(((double)sysml_state_displayNumber) / 2)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_arithmetic_5 = ((int)(((double)sysml_state_displayNumber) / 2)) - sysml_conditional_4;
    if (sysml_state_displayNumber == (2 * sysml_arithmetic_5)) {
        sysml_conditional_6 = 0.0;
    }
    else {
        sysml_conditional_6 = ((double)(io_4 * 255.0)) / 1023.0;
    }
    sysml_state_brightnessLed2 = sysml_conditional_6;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_displayNumber;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_brightnessLed1;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_brightnessLed2;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}