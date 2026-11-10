#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_welcomeActive = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_welcomeActive = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool* io_2, int* io_3, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0 <= 10.0;
    }
    if (sysml_lazy_0) {
        sysml_state_welcomeActive = true;
    }
    else {
        sysml_state_welcomeActive = false;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_welcomeActive) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_value_0 = sysml_conditional_1;
    int sysml_value_1 = 0;
    if (sysml_state_welcomeActive) {
        sysml_conditional_2 = 1000;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_1 = sysml_conditional_2;
    bool sysml_value_2 = 0;
    if (sysml_state_welcomeActive) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_2 = sysml_conditional_3;
    int sysml_value_3 = 0;
    if (sysml_state_welcomeActive) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_3 = sysml_conditional_4;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
}