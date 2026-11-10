#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Global Variable define of  */
int sysml_state_beepPhase = 0;
/* Global Variable define of  */
bool sysml_state_lastValidWithin10 = false;
/* Global Variable define of  */
bool sysml_state_standbyAfterGuard = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_alarmActive = false;
    sysml_state_beepPhase = 0;
    sysml_state_lastValidWithin10 = false;
    sysml_state_standbyAfterGuard = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_2, bool io_3, bool* io_0, int* io_1, bool* io_4, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    if (io_3) {
        sysml_state_lastValidWithin10 = true;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_0 = io_2 >= 10.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_3;
    }
    sysml_value_0 = sysml_lazy_0;
    int sysml_value_1 = 0;
    sysml_lazy_1 = io_2 >= 10.0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_3;
    }
    if (sysml_lazy_1) {
        sysml_conditional_2 = 1000;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_1 = sysml_conditional_2;
    bool sysml_value_2 = 0;
    sysml_lazy_3 = io_2 >= 10.0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_3;
    }
    sysml_value_2 = sysml_lazy_3;
    bool sysml_value_3 = 0;
    sysml_lazy_4 = io_2 >= 10.0;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_3;
    }
    sysml_value_3 = sysml_lazy_4;
    double sysml_value_4 = 0;
    sysml_value_4 = io_2;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_4 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
}