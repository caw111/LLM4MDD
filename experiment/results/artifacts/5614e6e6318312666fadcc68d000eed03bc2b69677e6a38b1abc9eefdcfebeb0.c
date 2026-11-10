#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_pollTimerMs = 0;
/* Global Variable define of  */
int sysml_state_distanceBelowCount = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_pollTimerMs = 0;
    sysml_state_distanceBelowCount = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double io_4, bool io_5, int io_6, bool* io_0, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_1_trigger, bool* serial_1_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    sysml_state_pollTimerMs = (sysml_state_pollTimerMs + io_6);
    if ((sysml_state_pollTimerMs >= 3000)) {
        sysml_state_pollTimerMs = (sysml_state_pollTimerMs - 3000);
        if (io_5) {
            if ((io_4 < 20.0)) {
                sysml_state_distanceBelowCount = (sysml_state_distanceBelowCount + 1);
            }
            else {
                sysml_state_distanceBelowCount = 0;
            }
        }
        else {
            sysml_state_distanceBelowCount = 0;
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_distanceBelowCount >= 3);
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_pollTimerMs >= 3000);
    double sysml_value_2 = 0;
    sysml_value_2 = io_1;
    double sysml_value_3 = 0;
    sysml_value_3 = io_4;
    bool sysml_value_4 = 0;
    sysml_lazy_0 = sysml_state_pollTimerMs >= 3000;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_distanceBelowCount >= 3;
    }
    sysml_value_4 = sysml_lazy_0;
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_distanceBelowCount >= 3);
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_1_trigger = sysml_value_4;
    *serial_1_value_0 = sysml_value_5;
}