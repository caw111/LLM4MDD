#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_ledState = false;
/* Global Variable define of  */
int sysml_state_pwmValue = 0;
/* Global Variable define of  */
int sysml_state_buzzerHz = 0;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledState = false;
    sysml_state_pwmValue = 0;
    sysml_state_buzzerHz = 0;
    sysml_state_prevPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool* io_0, double* io_1, int* io_2, bool* serial_0_trigger, bool* serial_0_value_0, int* serial_0_value_1, int* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_prevPressed = true;
    }
    else {
        sysml_state_prevPressed = false;
    }
    sysml_lazy_1 = io_3;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevPressed;
    }
    if (!sysml_lazy_1) {
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_ledState;
    double sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_pwmValue * 1.0);
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_buzzerHz;
    bool sysml_value_3 = 0;
    sysml_lazy_2 = io_3;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevPressed;
    }
    sysml_value_3 = sysml_lazy_2;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_ledState;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_pwmValue;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_buzzerHz;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
}