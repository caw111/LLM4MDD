#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int* io_2, bool* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_value_0 = 0;
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    if (io_1) {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = ((int)io_0) - sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_0 <= 30.0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_1 = sysml_conditional_3;
    double sysml_value_2 = 0;
    if (io_1) {
        if (io_0 <= 30.0) {
            sysml_conditional_5 = ((double)(30.0 - io_0)) / 30.0;
        }
        else {
            sysml_conditional_5 = 0.0;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_2 = sysml_conditional_4;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}