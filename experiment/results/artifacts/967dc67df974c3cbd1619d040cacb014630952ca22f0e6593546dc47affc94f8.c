#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Global Variable define of  */
int sysml_state_phaseMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledOn = false;
    sysml_state_phaseMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_phaseMs = sysml_state_phaseMs;
    sysml_state_ledOn = sysml_state_ledOn;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_ledOn;
    *io_1 = sysml_value_0;
}