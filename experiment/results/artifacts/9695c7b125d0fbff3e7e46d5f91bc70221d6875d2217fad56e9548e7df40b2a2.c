#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_doorOpenMs = 0;
/* Global Variable define of  */
bool sysml_state_prevDoorOpen = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_doorOpenMs = 0;
    sysml_state_prevDoorOpen = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool* io_2, int* io_3, int* io_4, bool* io_5, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    if (io_1) {
        sysml_state_doorOpenMs = sysml_state_doorOpenMs;
    }
    if ((!io_1)) {
        sysml_state_doorOpenMs = 0;
    }
    sysml_lazy_0 = !io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_prevDoorOpen;
    }
    if (sysml_lazy_0) {
    }
    sysml_state_prevDoorOpen = io_1;
    bool sysml_value_0 = 0;
    sysml_value_0 = false;
    int sysml_value_1 = 0;
    sysml_value_1 = 0;
    int sysml_value_2 = 0;
    sysml_value_2 = 0;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    bool sysml_value_4 = 0;
    sysml_value_4 = false;
    double sysml_value_5 = 0;
    sysml_value_5 = 0.0;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
}