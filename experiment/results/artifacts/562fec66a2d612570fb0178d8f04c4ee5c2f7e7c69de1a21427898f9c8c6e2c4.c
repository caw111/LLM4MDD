#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_muted = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_muted = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_3, bool io_4, bool* io_1, int* io_2, bool* serial_0_trigger, double* serial_0_value_0, bool* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_muted = (!sysml_state_muted);
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    if (sysml_state_muted) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_value_0 = sysml_conditional_1;
    int sysml_value_1 = 0;
    if (sysml_state_muted) {
        sysml_conditional_2 = 0;
    }
    else {
        if (io_3 < 10.0) {
            sysml_conditional_3 = 2000;
        }
        else {
            if (io_3 < 20.0) {
                sysml_conditional_4 = 1500;
            }
            else {
                if (io_3 < 30.0) {
                    sysml_conditional_5 = 1000;
                }
                else {
                    sysml_conditional_5 = 500;
                }
                sysml_conditional_4 = sysml_conditional_5;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    sysml_value_1 = sysml_conditional_2;
    bool sysml_value_2 = 0;
    sysml_value_2 = io_4;
    double sysml_value_3 = 0;
    sysml_value_3 = io_3;
    bool sysml_value_4 = 0;
    sysml_value_4 = io_4;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
    *serial_0_value_1 = sysml_value_4;
}