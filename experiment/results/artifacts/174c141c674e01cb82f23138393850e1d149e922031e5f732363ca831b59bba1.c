#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_counter = 0;
/* Global Variable define of  */
bool sysml_state_prevIncPressed = false;
/* Global Variable define of  */
bool sysml_state_prevDecPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_counter = 0;
    sysml_state_prevIncPressed = false;
    sysml_state_prevDecPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int* io_2, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    sysml_state_prevIncPressed = io_0;
    sysml_state_prevDecPressed = io_1;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevIncPressed;
    }
    if (sysml_lazy_0) {
        if ((sysml_state_counter == 20)) {
            sysml_state_counter = 20;
        }
        else {
            sysml_state_counter = (sysml_state_counter + 1);
        }
    }
    sysml_lazy_1 = io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevDecPressed;
    }
    if (sysml_lazy_1) {
        if ((sysml_state_counter == 0)) {
            sysml_state_counter = 0;
        }
        else {
            sysml_state_counter = (sysml_state_counter - 1);
        }
    }
    int sysml_value_0 = 0;
    sysml_lazy_2 = sysml_state_counter == 20;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_counter == 0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = 1000;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_0 = sysml_conditional_3;
    int sysml_value_1 = 0;
    sysml_lazy_4 = sysml_state_counter < 0;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_counter > 20;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = 0;
    }
    else {
        sysml_conditional_5 = sysml_state_counter;
    }
    sysml_value_1 = sysml_conditional_5;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
}