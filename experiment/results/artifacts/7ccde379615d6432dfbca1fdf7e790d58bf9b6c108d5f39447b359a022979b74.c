#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_missedDoseCount = 0;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_missedDoseCount = 0;
    sysml_state_alarmActive = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int io_6, bool io_6_valid, bool* io_3, int* io_4, int* io_5, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    if (io_6_valid) {
        sysml_state_alarmActive = false;
    }
    sysml_lazy_0 = sysml_state_alarmActive;
    if (sysml_lazy_0) {
        sysml_lazy_1 = io_0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = !sysml_state_previousPressed;
        }
        sysml_lazy_0 = sysml_lazy_1;
    }
    if (sysml_lazy_0) {
        sysml_state_alarmActive = false;
    }
    sysml_lazy_2 = io_0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressed;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_alarmActive;
    }
    if (sysml_lazy_3) {
        sysml_state_alarmActive = true;
        sysml_state_missedDoseCount = sysml_state_missedDoseCount;
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_alarmActive;
    int sysml_value_1 = 0;
    sysml_value_1 = 0;
    int sysml_value_2 = 0;
    sysml_value_2 = 0;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_missedDoseCount;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
}