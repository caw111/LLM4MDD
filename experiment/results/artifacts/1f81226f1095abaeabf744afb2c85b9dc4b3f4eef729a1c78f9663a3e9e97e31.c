#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode = 1;
/* Global Variable define of  */
bool sysml_state_previousBtn1Pressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 1;
    sysml_state_previousBtn1Pressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, double io_4, double* io_5, double* io_6, double* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    if (io_1) {
        sysml_state_mode = 1;
        sysml_state_previousBtn1Pressed = io_0;
    }
    else {
        sysml_lazy_0 = io_0;
        if (sysml_lazy_0) {
            sysml_lazy_0 = !sysml_state_previousBtn1Pressed;
        }
        if (sysml_lazy_0) {
            if (sysml_state_mode == 4) {
                sysml_conditional_1 = 1;
            }
            else {
                sysml_conditional_1 = sysml_state_mode + 1;
            }
            sysml_state_mode = sysml_conditional_1;
        }
        sysml_state_previousBtn1Pressed = io_0;
    }
    double sysml_value_0 = 0;
    sysml_lazy_2 = sysml_state_mode == 1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_mode == 4;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = io_2;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_0 = sysml_conditional_3;
    double sysml_value_1 = 0;
    sysml_lazy_4 = sysml_state_mode == 2;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_mode == 4;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = io_3;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_1 = sysml_conditional_5;
    double sysml_value_2 = 0;
    sysml_lazy_6 = sysml_state_mode == 3;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_mode == 4;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = io_4;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_2 = sysml_conditional_7;
    *io_5 = sysml_value_0;
    *io_6 = sysml_value_1;
    *io_7 = sysml_value_2;
}