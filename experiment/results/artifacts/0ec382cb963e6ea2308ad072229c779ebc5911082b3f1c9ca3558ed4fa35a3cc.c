#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_mode == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_mode + 1;
        }
        sysml_state_mode = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_1;
    int sysml_value_0 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_2 = 0;
    }
    else {
        if (sysml_state_mode == 1) {
            if (io_0 < 0.0) {
                sysml_conditional_4 = 0;
            }
            else {
                if (io_0 > 1023.0) {
                    sysml_conditional_5 = 1023;
                }
                else {
                    if (io_0 < ((double)((int)io_0))) {
                        sysml_conditional_6 = 1;
                    }
                    else {
                        sysml_conditional_6 = 0;
                    }
                    sysml_conditional_5 = ((int)io_0) - sysml_conditional_6;
                }
                sysml_conditional_4 = sysml_conditional_5;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        else {
            sysml_conditional_3 = 1234;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    sysml_value_0 = sysml_conditional_2;
    *io_2 = sysml_value_0;
}