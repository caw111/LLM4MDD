#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_count = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_count = 0;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int* io_1, double* io_2, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_arithmetic_3 = 0;
    double sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_arithmetic_6 = 0;
    double sysml_conditional_7 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_count >= 9) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_count + 1;
        }
        sysml_state_count = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_0;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_count;
    double sysml_value_1 = 0;
    if ((((double)sysml_state_count) / 2) < ((double)((int)(((double)sysml_state_count) / 2)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_arithmetic_3 = ((int)(((double)sysml_state_count) / 2)) - sysml_conditional_2;
    if ((sysml_state_count - (2 * sysml_arithmetic_3)) == 0) {
        sysml_conditional_4 = 0.0;
    }
    else {
        sysml_conditional_4 = 90.0;
    }
    sysml_value_1 = sysml_conditional_4;
    double sysml_value_2 = 0;
    if ((((double)sysml_state_count) / 2) < ((double)((int)(((double)sysml_state_count) / 2)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_arithmetic_6 = ((int)(((double)sysml_state_count) / 2)) - sysml_conditional_5;
    if ((sysml_state_count - (2 * sysml_arithmetic_6)) == 0) {
        sysml_conditional_7 = 0.0;
    }
    else {
        sysml_conditional_7 = 90.0;
    }
    sysml_value_2 = sysml_conditional_7;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
}