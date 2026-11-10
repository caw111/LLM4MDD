#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_alarmActive = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int* io_3, bool* io_4, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    sysml_lazy_0 = io_1 <= 20.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_2;
    }
    sysml_state_alarmActive = sysml_lazy_0;
    if ((io_0 > 0.5)) {
        sysml_state_alarmActive = false;
    }
    int sysml_value_0 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_1 = 440;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_alarmActive;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_alarmActive;
    int sysml_value_3 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_3 = sysml_conditional_2;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
}