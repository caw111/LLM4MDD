#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Global Variable define of  */
bool sysml_state_ledState = false;
/* Global Variable define of  */
bool sysml_state_beepState = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_alarmActive = false;
    sysml_state_ledState = false;
    sysml_state_beepState = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, bool* io_3, int* io_4, bool* serial_0_trigger, bool* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    sysml_lazy_0 = io_0 <= 300.0;
    if (sysml_lazy_0) {
        sysml_lazy_1 = io_2;
        if (sysml_lazy_1) {
            sysml_lazy_1 = io_1 <= 20.0;
        }
        sysml_lazy_0 = sysml_lazy_1;
    }
    sysml_state_alarmActive = sysml_lazy_0;
    if (sysml_state_alarmActive) {
        sysml_state_ledState = (!sysml_state_ledState);
        sysml_state_beepState = (!sysml_state_beepState);
    }
    bool sysml_value_0 = 0;
    sysml_lazy_2 = sysml_state_ledState;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_alarmActive;
    }
    sysml_value_0 = sysml_lazy_2;
    int sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_alarmActive;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_beepState;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = 2000;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_1 = sysml_conditional_4;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_alarmActive;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_alarmActive;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
}