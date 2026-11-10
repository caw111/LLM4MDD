#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousCounterInput = false;
/* Global Variable define of  */
int sysml_state_accumulatedNoProductMs = 0;
/* Global Variable define of  */
int sysml_state_cumulativeCount = 0;
/* Global Variable define of  */
int sysml_state_stopCount = 0;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Global Variable define of  */
bool sysml_state_previousStopButton = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousCounterInput = false;
    sysml_state_accumulatedNoProductMs = 0;
    sysml_state_cumulativeCount = 0;
    sysml_state_stopCount = 0;
    sysml_state_alarmActive = false;
    sysml_state_previousStopButton = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int io_6, bool* io_3, int* io_4, int* io_5, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1, int* serial_0_value_2, bool* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    sysml_state_previousCounterInput = io_0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousStopButton;
    }
    if (sysml_lazy_0) {
        sysml_state_stopCount = (sysml_state_stopCount + 1);
    }
    sysml_state_previousStopButton = io_1;
    sysml_lazy_1 = sysml_state_alarmActive;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !io_1;
    }
    if (sysml_lazy_1) {
        sysml_state_alarmActive = false;
    }
    sysml_lazy_2 = io_1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_previousCounterInput;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = 0;
    }
    else {
        sysml_conditional_3 = sysml_state_accumulatedNoProductMs + io_6;
    }
    sysml_state_accumulatedNoProductMs = sysml_conditional_3;
    sysml_lazy_4 = io_1;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_accumulatedNoProductMs >= 30000;
    }
    if (sysml_lazy_4) {
        sysml_state_alarmActive = true;
    }
    sysml_lazy_5 = io_0;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousCounterInput;
    }
    if (sysml_lazy_5) {
        sysml_state_cumulativeCount = (sysml_state_cumulativeCount + 1);
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_alarmActive;
    int sysml_value_1 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_6 = 2000;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_1 = sysml_conditional_6;
    int sysml_value_2 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_7 = 0;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_2 = sysml_conditional_7;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_alarmActive;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_cumulativeCount;
    double sysml_value_5 = 0;
    sysml_lazy_8 = 0 != 0;
    if (sysml_lazy_8) {
        sysml_lazy_8 = true;
    }
    if (sysml_lazy_8) {
        sysml_conditional_9 = 0.0;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_5 = sysml_conditional_9;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_stopCount;
    bool sysml_value_7 = 0;
    sysml_value_7 = sysml_state_alarmActive;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
}