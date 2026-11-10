#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_lightOn = false;
/* Global Variable define of  */
int sysml_state_count = 0;
/* Global Variable define of  */
bool sysml_state_previousIo0 = false;
/* Global Variable define of  */
bool sysml_state_previousIo1 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lightOn = false;
    sysml_state_count = 0;
    sysml_state_previousIo0 = false;
    sysml_state_previousIo1 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int io_5, bool io_5_valid, bool* io_2, int* io_3, int* io_4, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    sysml_lazy_0 = io_5_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = true;
    }
    if (sysml_lazy_0) {
        if ((io_5 == 6)) {
            sysml_state_lightOn = true;
            sysml_state_count = (sysml_state_count + 1);
        }
        if ((io_5 == 5)) {
            sysml_state_lightOn = false;
            sysml_state_count = (sysml_state_count + 1);
        }
        if ((io_5 == 1)) {
            sysml_state_count = (sysml_state_count + 1);
        }
        if ((io_5 == 3)) {
            sysml_state_count = 0;
        }
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousIo0;
    }
    if (sysml_lazy_1) {
        sysml_state_lightOn = true;
        sysml_state_count = (sysml_state_count + 1);
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousIo1;
    }
    if (sysml_lazy_2) {
        sysml_state_lightOn = false;
        sysml_state_count = (sysml_state_count + 1);
    }
    sysml_state_previousIo0 = io_0;
    sysml_state_previousIo1 = io_1;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_lightOn;
    int sysml_value_1 = 0;
    sysml_lazy_3 = io_5_valid;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_5 == 1;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = 1000;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_1 = sysml_conditional_4;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_count;
    bool sysml_value_3 = 0;
    sysml_value_3 = io_5_valid;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_count;
    int sysml_value_5 = 0;
    if (sysml_state_lightOn) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_5 = sysml_conditional_5;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
}