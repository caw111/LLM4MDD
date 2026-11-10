#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_lastValue = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastValue = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    int sysml_conditional_6 = 0;
    if (true) {
        if ((((double)io_0) / 10.0) < ((double)((int)(((double)io_0) / 10.0)))) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = 0;
        }
        sysml_lazy_1 = (((int)(((double)io_0) / 10.0)) - sysml_conditional_0) >= 0;
        if (sysml_lazy_1) {
            if ((((double)io_0) / 10.0) < ((double)((int)(((double)io_0) / 10.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_lazy_1 = (((int)(((double)io_0) / 10.0)) - sysml_conditional_2) <= 99;
        }
        if (sysml_lazy_1) {
            if ((((double)io_0) / 10.0) < ((double)((int)(((double)io_0) / 10.0)))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = ((int)(((double)io_0) / 10.0)) - sysml_conditional_4;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_state_lastValue = sysml_conditional_3;
    }
    int sysml_value_0 = 0;
    sysml_lazy_5 = sysml_state_lastValue >= 0;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_lastValue <= 99;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = sysml_state_lastValue;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_0 = sysml_conditional_6;
    *io_1 = sysml_value_0;
}