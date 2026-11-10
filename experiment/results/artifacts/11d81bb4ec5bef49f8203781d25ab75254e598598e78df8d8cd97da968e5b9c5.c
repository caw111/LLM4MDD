#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_fanEnabled = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_fanEnabled = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double* io_0, int* io_4, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    if (io_3) {
        sysml_state_fanEnabled = (io_1 > 30.0);
    }
    double sysml_value_0 = 0;
    if (io_3) {
        if (io_1 > 30.0) {
            sysml_conditional_1 = 255.0;
        }
        else {
            sysml_conditional_1 = 0.0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        if (sysml_state_fanEnabled) {
            sysml_conditional_2 = 255.0;
        }
        else {
            sysml_conditional_2 = 0.0;
        }
        sysml_conditional_0 = sysml_conditional_2;
    }
    sysml_value_0 = sysml_conditional_0;
    int sysml_value_1 = 0;
    if (io_3) {
        if (io_1 < ((double)((int)io_1))) {
            sysml_conditional_4 = 1;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_conditional_3 = ((int)io_1) - sysml_conditional_4;
    }
    else {
        if (io_1 < ((double)((int)io_1))) {
            sysml_conditional_5 = 1;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_3 = ((int)io_1) - sysml_conditional_5;
    }
    sysml_value_1 = sysml_conditional_3;
    bool sysml_value_2 = 0;
    sysml_lazy_6 = io_3;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_1 > 30.0;
    }
    sysml_value_2 = sysml_lazy_6;
    double sysml_value_3 = 0;
    if (io_3) {
        sysml_conditional_7 = io_1;
    }
    else {
        sysml_conditional_7 = io_1;
    }
    sysml_value_3 = sysml_conditional_7;
    *io_0 = sysml_value_0;
    *io_4 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
}