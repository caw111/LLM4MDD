#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousParityEven = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousParityEven = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_2, double io_3, double io_4, double* io_0, double* io_1, int* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_arithmetic_4 = 0;
    double sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_arithmetic_7 = 0;
    double sysml_conditional_8 = 0;
    if ((((double)(io_4 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_4 * 99.0)) / 1023.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    if ((((int)(((double)(io_4 * 99.0)) / 1023.0)) - sysml_conditional_0) == 0) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_state_previousParityEven = sysml_conditional_1 == true;
    int sysml_value_0 = 0;
    if ((((double)(io_4 * 99.0)) / 1023.0) < ((double)((int)(((double)(io_4 * 99.0)) / 1023.0)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_0 = ((int)(((double)(io_4 * 99.0)) / 1023.0)) - sysml_conditional_2;
    double sysml_value_1 = 0;
    if ((((double)sysml_value_0) / 2) < ((double)((int)(((double)sysml_value_0) / 2)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_arithmetic_4 = ((int)(((double)sysml_value_0) / 2)) - sysml_conditional_3;
    if (sysml_value_0 == (2 * sysml_arithmetic_4)) {
        sysml_conditional_5 = ((double)(io_2 * 255.0)) / 1023.0;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_1 = sysml_conditional_5;
    double sysml_value_2 = 0;
    if ((((double)sysml_value_0) / 2) < ((double)((int)(((double)sysml_value_0) / 2)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_arithmetic_7 = ((int)(((double)sysml_value_0) / 2)) - sysml_conditional_6;
    if (sysml_value_0 == (2 * sysml_arithmetic_7)) {
        sysml_conditional_8 = 0.0;
    }
    else {
        sysml_conditional_8 = ((double)(io_3 * 255.0)) / 1023.0;
    }
    sysml_value_2 = sysml_conditional_8;
    *io_0 = sysml_value_1;
    *io_1 = sysml_value_2;
    *io_5 = sysml_value_0;
}