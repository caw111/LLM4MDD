#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_reportSent = false;
/* Global Variable define of  */
bool sysml_state_reportNow = false;
/* Global Variable define of  */
int sysml_state_capturedEpoch = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_reportSent = false;
    sysml_state_reportNow = false;
    sysml_state_capturedEpoch = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_1, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    sysml_state_reportNow = false;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_reportSent;
    }
    if (sysml_lazy_0) {
        sysml_state_capturedEpoch = io_0;
        sysml_state_reportNow = true;
        sysml_state_reportSent = true;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_reportNow;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_capturedEpoch;
    *serial_0_trigger = sysml_value_0;
    *serial_0_value_0 = sysml_value_1;
}