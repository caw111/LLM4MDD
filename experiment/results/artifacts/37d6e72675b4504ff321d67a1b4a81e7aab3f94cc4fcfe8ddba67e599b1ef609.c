#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_faultLatched = false;
/* Global Variable define of  */
int sysml_state_faultStageLatched = 0;
/* Global Variable define of  */
bool sysml_state_endAlarmActive = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_faultLatched = false;
    sysml_state_faultStageLatched = 0;
    sysml_state_endAlarmActive = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_2, double io_3, bool* io_1, int* io_4, int* io_5, bool* serial_0_trigger, int* serial_0_value_0, bool* serial_1_trigger, int* serial_1_value_0, double* serial_1_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    sysml_state_faultLatched = sysml_state_faultLatched;
    sysml_state_faultStageLatched = sysml_state_faultStageLatched;
    sysml_state_endAlarmActive = sysml_state_endAlarmActive;
    if (sysml_state_endAlarmActive) {
        sysml_lazy_0 = io_0;
        if (sysml_lazy_0) {
            sysml_lazy_0 = !sysml_state_previousPressed;
        }
        if (sysml_lazy_0) {
            sysml_state_endAlarmActive = false;
        }
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_endAlarmActive;
    int sysml_value_1 = 0;
    if (sysml_state_endAlarmActive) {
        sysml_conditional_1 = 1000;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_value_1 = sysml_conditional_1;
    int sysml_value_2 = 0;
    if (sysml_state_endAlarmActive) {
        sysml_conditional_2 = 4;
    }
    else {
        sysml_conditional_2 = 1;
    }
    sysml_value_2 = sysml_conditional_2;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_faultLatched;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_faultStageLatched;
    bool sysml_value_5 = 0;
    sysml_value_5 = true;
    int sysml_value_6 = 0;
    if (sysml_state_endAlarmActive) {
        sysml_conditional_3 = 4;
    }
    else {
        sysml_conditional_3 = 1;
    }
    sysml_value_6 = sysml_conditional_3;
    double sysml_value_7 = 0;
    sysml_value_7 = io_3;
    *io_1 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_1_trigger = sysml_value_5;
    *serial_1_value_0 = sysml_value_6;
    *serial_1_value_1 = sysml_value_7;
}