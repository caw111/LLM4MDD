#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_distanceMode = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_selectedValue = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_distanceMode = false;
    sysml_state_previousPressed = false;
    sysml_state_selectedValue = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, bool io_2, double io_3, bool io_4, int* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_distanceMode = (!sysml_state_distanceMode);
    }
    sysml_state_previousPressed = io_0;
    sysml_lazy_1 = sysml_state_distanceMode;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_4;
    }
    if (sysml_lazy_1) {
        if (io_3 < ((double)((int)io_3))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_state_selectedValue = ((int)io_3) - sysml_conditional_2;
    }
    else {
        sysml_lazy_3 = !sysml_state_distanceMode;
        if (sysml_lazy_3) {
            sysml_lazy_3 = io_2;
        }
        if (sysml_lazy_3) {
            if (io_1 < ((double)((int)io_1))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_state_selectedValue = ((int)io_1) - sysml_conditional_4;
        }
    }
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_selectedValue;
    *io_5 = sysml_value_0;
}