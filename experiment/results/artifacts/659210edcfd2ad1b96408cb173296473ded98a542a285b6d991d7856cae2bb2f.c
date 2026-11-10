#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeIsManual = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeIsManual = false;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_3, bool io_4, int* io_2, int* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_modeIsManual = (!sysml_state_modeIsManual);
    }
    sysml_state_previousPressed = io_1;
    int sysml_value_0 = 0;
    sysml_lazy_1 = !sysml_state_modeIsManual;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_4;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_3 <= 15.0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = 1000;
    }
    else {
        sysml_lazy_4 = sysml_state_modeIsManual;
        if (sysml_lazy_4) {
            sysml_lazy_4 = io_4;
        }
        sysml_lazy_5 = sysml_lazy_4;
        if (sysml_lazy_5) {
            if (io_0 < ((double)((int)io_0))) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = 0;
            }
            sysml_lazy_5 = io_3 <= (((int)io_0) - sysml_conditional_6);
        }
        if (sysml_lazy_5) {
            sysml_conditional_7 = 1000;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_conditional_3 = sysml_conditional_7;
    }
    sysml_value_0 = sysml_conditional_3;
    int sysml_value_1 = 0;
    if (sysml_state_modeIsManual) {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_9 = 1;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_conditional_8 = ((int)io_0) - sysml_conditional_9;
    }
    else {
        sysml_conditional_8 = 15;
    }
    sysml_value_1 = sysml_conditional_8;
    *io_2 = sysml_value_0;
    *io_5 = sysml_value_1;
}