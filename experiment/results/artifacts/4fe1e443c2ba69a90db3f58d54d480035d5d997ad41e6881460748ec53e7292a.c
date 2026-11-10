#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_startupBlank = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_startupBlank = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, bool io_2, bool* io_0, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    sysml_lazy_0 = sysml_state_startupBlank;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_2;
    }
    if (sysml_lazy_0) {
        sysml_state_startupBlank = false;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_1 = !sysml_state_startupBlank;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_1 < 10.0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_0 = sysml_conditional_3;
    int sysml_value_1 = 0;
    sysml_lazy_4 = sysml_state_startupBlank;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = !io_2;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = 10000;
    }
    else {
        sysml_lazy_6 = io_1 < 2.0;
        if (!sysml_lazy_6) {
            sysml_lazy_6 = io_1 > 400.0;
        }
        if (sysml_lazy_6) {
            sysml_conditional_7 = 10000;
        }
        else {
            if (io_2) {
                if (io_1 < ((double)((int)io_1))) {
                    sysml_conditional_9 = 1;
                }
                else {
                    sysml_conditional_9 = 0;
                }
                sysml_conditional_8 = ((int)io_1) - sysml_conditional_9;
            }
            else {
                sysml_conditional_8 = 10000;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_5 = sysml_conditional_7;
    }
    sysml_value_1 = sysml_conditional_5;
    *io_0 = sysml_value_0;
    *io_3 = sysml_value_1;
}