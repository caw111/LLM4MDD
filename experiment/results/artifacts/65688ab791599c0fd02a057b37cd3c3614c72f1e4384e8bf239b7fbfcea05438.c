#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_muteMode = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_muteMode = false;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_3, bool io_4, bool* io_1, int* io_2, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_muteMode = (!sysml_state_muteMode);
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_muteMode;
    int sysml_value_1 = 0;
    if (sysml_state_muteMode) {
        sysml_conditional_1 = 0;
    }
    else {
        sysml_lazy_2 = io_4;
        if (sysml_lazy_2) {
            sysml_lazy_2 = io_3 >= 2.0;
        }
        if (sysml_lazy_2) {
            if (io_3 < 20.0) {
                sysml_conditional_4 = 2000;
            }
            else {
                if (io_3 < 50.0) {
                    sysml_conditional_5 = 1200;
                }
                else {
                    if (io_3 < 100.0) {
                        sysml_conditional_6 = 700;
                    }
                    else {
                        sysml_conditional_6 = 300;
                    }
                    sysml_conditional_5 = sysml_conditional_6;
                }
                sysml_conditional_4 = sysml_conditional_5;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_conditional_1 = sysml_conditional_3;
    }
    sysml_value_1 = sysml_conditional_1;
    bool sysml_value_2 = 0;
    sysml_value_2 = true;
    double sysml_value_3 = 0;
    if (io_4) {
        sysml_conditional_7 = io_3;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_3 = sysml_conditional_7;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
}