#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_manualMode = false;
/* Global Variable define of  */
bool sysml_state_previousButtonPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_manualMode = false;
    sysml_state_previousButtonPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_6, bool* io_3, bool* io_4, int* io_5, int* io_7, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, bool* serial_0_value_3, bool* serial_0_value_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    if (io_6) {
        sysml_lazy_0 = io_6;
        if (sysml_lazy_0) {
            sysml_lazy_0 = !sysml_state_previousButtonPressed;
        }
        if (sysml_lazy_0) {
            sysml_state_manualMode = (!sysml_state_manualMode);
        }
    }
    sysml_state_previousButtonPressed = io_6;
    bool sysml_value_0 = 0;
    sysml_value_0 = false;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_manualMode;
    int sysml_value_2 = 0;
    sysml_value_2 = 0;
    int sysml_value_3 = 0;
    if (sysml_state_manualMode) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_value_3 = sysml_conditional_1;
    bool sysml_value_4 = 0;
    sysml_value_4 = false;
    double sysml_value_5 = 0;
    sysml_value_5 = io_0;
    double sysml_value_6 = 0;
    sysml_value_6 = io_1;
    double sysml_value_7 = 0;
    sysml_value_7 = io_2;
    bool sysml_value_8 = 0;
    sysml_value_8 = sysml_state_manualMode;
    bool sysml_value_9 = 0;
    sysml_value_9 = false;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_7 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
    *serial_0_value_2 = sysml_value_7;
    *serial_0_value_3 = sysml_value_8;
    *serial_0_value_4 = sysml_value_9;
}