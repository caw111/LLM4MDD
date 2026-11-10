#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_count = 0;
/* Global Variable define of  */
bool sysml_state_alarmLocked = false;
/* Global Variable define of  */
bool sysml_state_prevUp = false;
/* Global Variable define of  */
bool sysml_state_prevDown = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_count = 0;
    sysml_state_alarmLocked = false;
    sysml_state_prevUp = false;
    sysml_state_prevDown = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool* io_2, int* io_3, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    if (sysml_state_alarmLocked) {
        sysml_lazy_0 = io_1;
        if (sysml_lazy_0) {
            sysml_lazy_0 = !sysml_state_prevDown;
        }
        if (sysml_lazy_0) {
            sysml_state_alarmLocked = false;
            sysml_state_count = 0;
        }
    }
    else {
        sysml_lazy_1 = io_0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = !sysml_state_prevUp;
        }
        if (sysml_lazy_1) {
            if ((sysml_state_count == 15)) {
                sysml_state_alarmLocked = true;
                sysml_state_count = 15;
            }
            else {
                sysml_state_count = (sysml_state_count + 1);
            }
        }
        sysml_lazy_2 = io_1;
        if (sysml_lazy_2) {
            sysml_lazy_2 = !sysml_state_prevDown;
        }
        if (sysml_lazy_2) {
            if (sysml_state_count == 0) {
                sysml_conditional_3 = 15;
            }
            else {
                sysml_conditional_3 = sysml_state_count - 1;
            }
            sysml_state_count = sysml_conditional_3;
        }
    }
    sysml_state_prevUp = io_0;
    sysml_state_prevDown = io_1;
    bool sysml_value_0 = 0;
    if (sysml_state_count == 0) {
        sysml_conditional_4 = true;
    }
    else {
        if ((((double)sysml_state_count) / 2) < ((double)((int)(((double)sysml_state_count) / 2)))) {
            sysml_conditional_5 = 1;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_4 = sysml_state_count == (2 + ((((int)(((double)sysml_state_count) / 2)) - sysml_conditional_5) * 0));
    }
    sysml_value_0 = sysml_conditional_4;
    int sysml_value_1 = 0;
    if (sysml_state_alarmLocked) {
        sysml_conditional_6 = 1000;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_1 = sysml_conditional_6;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_count;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}