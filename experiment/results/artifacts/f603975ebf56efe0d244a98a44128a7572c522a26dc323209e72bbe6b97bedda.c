#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Global Variable define of  */
bool sysml_state_prevLedOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledOn = false;
    sysml_state_prevLedOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, bool io_1_valid, bool* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    sysml_lazy_0 = io_1_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1 == 2;
    }
    if (sysml_lazy_0) {
        sysml_state_ledOn = true;
    }
    sysml_lazy_1 = io_1_valid;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_1 == 1;
    }
    if (sysml_lazy_1) {
        sysml_state_ledOn = false;
    }
    sysml_state_prevLedOn = sysml_state_ledOn;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_ledOn;
    *io_0 = sysml_value_0;
}