#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_lightOn = false;
/* Global Variable define of  */
int sysml_state_count = 0;
/* Global Variable define of  */
bool sysml_state_prevIo0 = false;
/* Global Variable define of  */
bool sysml_state_prevIo1 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lightOn = false;
    sysml_state_count = 0;
    sysml_state_prevIo0 = false;
    sysml_state_prevIo1 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int io_5, bool io_5_valid, bool* io_2, int* io_3, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    sysml_lazy_0 = io_5_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_5 == 6;
    }
    if (sysml_lazy_0) {
        sysml_state_lightOn = true;
    }
    sysml_lazy_1 = io_5_valid;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_5 == 5;
    }
    if (sysml_lazy_1) {
        sysml_state_lightOn = false;
    }
    sysml_lazy_2 = io_5_valid;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_5 == 3;
    }
    if (sysml_lazy_2) {
        sysml_state_count = 0;
    }
    sysml_lazy_3 = io_0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevIo0;
    }
    if (sysml_lazy_3) {
        sysml_state_lightOn = true;
        sysml_state_count = (sysml_state_count + 1);
    }
    sysml_lazy_4 = io_1;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevIo1;
    }
    if (sysml_lazy_4) {
        sysml_state_lightOn = false;
        sysml_state_count = (sysml_state_count + 1);
    }
    sysml_state_prevIo0 = io_0;
    sysml_state_prevIo1 = io_1;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_lightOn;
    int sysml_value_1 = 0;
    sysml_value_1 = 0;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_count;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}