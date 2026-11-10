#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_phase_ms = 0;
/* Global Variable define of  */
bool sysml_state_led1 = false;
/* Global Variable define of  */
bool sysml_state_led2 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phase_ms = 0;
    sysml_state_led1 = false;
    sysml_state_led2 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_2, bool* io_0, bool* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    sysml_state_phase_ms = (sysml_state_phase_ms + io_2);
    if ((sysml_state_phase_ms >= 15000)) {
        sysml_state_phase_ms = (sysml_state_phase_ms - 15000);
    }
    if ((sysml_state_phase_ms >= 15000)) {
        sysml_state_phase_ms = (sysml_state_phase_ms - 15000);
    }
    if ((sysml_state_phase_ms < 5000)) {
        sysml_state_led1 = false;
        sysml_state_led2 = false;
    }
    sysml_lazy_0 = sysml_state_phase_ms >= 5000;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_phase_ms < 10000;
    }
    if (sysml_lazy_0) {
        sysml_state_led1 = true;
        sysml_state_led2 = false;
    }
    sysml_lazy_1 = sysml_state_phase_ms >= 10000;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_phase_ms < 15000;
    }
    if (sysml_lazy_1) {
        sysml_state_led1 = false;
        sysml_state_led2 = false;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_led1;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_led2;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
}