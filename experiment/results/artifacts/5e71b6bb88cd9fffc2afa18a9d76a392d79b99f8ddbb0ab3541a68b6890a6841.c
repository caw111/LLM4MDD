#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_phaseMs = 0;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phaseMs = 0;
    sysml_state_ledOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, bool* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    sysml_state_phaseMs = (sysml_state_phaseMs + io_1);
    if (sysml_state_ledOn) {
        if ((sysml_state_phaseMs >= 1000)) {
            sysml_state_phaseMs = (sysml_state_phaseMs - 1000);
            sysml_state_ledOn = false;
        }
    }
    if ((!sysml_state_ledOn)) {
        if ((sysml_state_phaseMs >= 2000)) {
            sysml_state_phaseMs = (sysml_state_phaseMs - 2000);
            sysml_state_ledOn = true;
        }
    }
    sysml_lazy_0 = sysml_state_ledOn;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_phaseMs >= 3000;
    }
    if (sysml_lazy_0) {
        sysml_state_phaseMs = (sysml_state_phaseMs - 3000);
        sysml_state_ledOn = false;
    }
    sysml_lazy_1 = !sysml_state_ledOn;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_phaseMs >= 3000;
    }
    if (sysml_lazy_1) {
        sysml_state_phaseMs = (sysml_state_phaseMs - 3000);
        sysml_state_ledOn = true;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_ledOn;
    *io_0 = sysml_value_0;
}