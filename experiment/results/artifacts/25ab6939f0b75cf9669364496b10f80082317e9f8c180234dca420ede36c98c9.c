#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed1 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed2 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed3 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed4 = false;
/* Global Variable define of  */
bool sysml_state_locked = false;
/* Global Variable define of  */
int sysml_state_winnerId = 0;
/* Global Variable define of  */
int sysml_state_resetAccumulatedMs = 0;
/* Global Variable define of  */
bool sysml_state_resetting = false;
/* Global Variable define of  */
bool sysml_state_resetRequest = false;
/* Global Variable define of  */
bool sysml_state_serialTriggerNext = false;
/* Global Variable define of  */
int sysml_state_serialTimestampMsNext = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed1 = false;
    sysml_state_previousPressed2 = false;
    sysml_state_previousPressed3 = false;
    sysml_state_previousPressed4 = false;
    sysml_state_locked = false;
    sysml_state_winnerId = 0;
    sysml_state_resetAccumulatedMs = 0;
    sysml_state_resetting = false;
    sysml_state_resetRequest = false;
    sysml_state_serialTriggerNext = false;
    sysml_state_serialTimestampMsNext = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool io_4, bool* io_5, int* io_6, int* io_7, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    if (io_4) {
        sysml_state_resetting = true;
        sysml_state_resetAccumulatedMs = 0;
    }
    if ((!io_4)) {
        sysml_state_resetting = false;
        sysml_state_resetAccumulatedMs = 0;
    }
    sysml_lazy_0 = io_4;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_resetAccumulatedMs >= 3000;
    }
    if (sysml_lazy_0) {
        sysml_state_resetRequest = true;
    }
    else {
        sysml_state_resetRequest = false;
    }
    if (sysml_state_resetRequest) {
        sysml_state_locked = false;
        sysml_state_winnerId = 0;
        sysml_state_serialTriggerNext = false;
    }
    if ((!sysml_state_resetRequest)) {
        if ((!sysml_state_locked)) {
            sysml_lazy_1 = io_0;
            if (sysml_lazy_1) {
                sysml_lazy_1 = !sysml_state_previousPressed1;
            }
            if (sysml_lazy_1) {
                sysml_state_locked = true;
                sysml_state_winnerId = 1;
                sysml_state_serialTriggerNext = true;
                sysml_state_serialTimestampMsNext = 0;
            }
            sysml_lazy_2 = io_1;
            if (sysml_lazy_2) {
                sysml_lazy_2 = !sysml_state_previousPressed2;
            }
            if (sysml_lazy_2) {
                sysml_state_locked = true;
                sysml_state_winnerId = 2;
                sysml_state_serialTriggerNext = true;
                sysml_state_serialTimestampMsNext = 0;
            }
            sysml_lazy_3 = io_2;
            if (sysml_lazy_3) {
                sysml_lazy_3 = !sysml_state_previousPressed3;
            }
            if (sysml_lazy_3) {
                sysml_state_locked = true;
                sysml_state_winnerId = 3;
                sysml_state_serialTriggerNext = true;
                sysml_state_serialTimestampMsNext = 0;
            }
            sysml_lazy_4 = io_3;
            if (sysml_lazy_4) {
                sysml_lazy_4 = !sysml_state_previousPressed4;
            }
            if (sysml_lazy_4) {
                sysml_state_locked = true;
                sysml_state_winnerId = 4;
                sysml_state_serialTriggerNext = true;
                sysml_state_serialTimestampMsNext = 0;
            }
        }
        else {
            sysml_state_serialTriggerNext = false;
        }
    }
    sysml_state_serialTriggerNext = false;
    sysml_state_previousPressed1 = io_0;
    sysml_state_previousPressed2 = io_1;
    sysml_state_previousPressed3 = io_2;
    sysml_state_previousPressed4 = io_3;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_locked;
    int sysml_value_1 = 0;
    sysml_value_1 = 0;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_winnerId;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    int sysml_value_4 = 0;
    sysml_value_4 = 0;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_winnerId;
    *io_5 = sysml_value_0;
    *io_6 = sysml_value_1;
    *io_7 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
}