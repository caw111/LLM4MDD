#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_pendingMask = 0;
/* Global Variable define of  */
int sysml_state_activeWaitSeconds = 0;
/* Global Variable define of  */
int sysml_state_activeBedIndex = 0;
/* Global Variable define of  */
bool sysml_state_previousIo0 = false;
/* Global Variable define of  */
bool sysml_state_previousIo1 = false;
/* Global Variable define of  */
bool sysml_state_previousIo2 = false;
/* Global Variable define of  */
bool sysml_state_previousIo3 = false;
/* Global Variable define of  */
bool sysml_state_serialTriggerLatched = false;
/* Global Variable define of  */
int sysml_state_serialElapsedMs = 0;
/* Global Variable define of  */
int sysml_state_serialBedIndex = 0;
/* Global Variable define of  */
int sysml_state_serialWaitS = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_pendingMask = 0;
    sysml_state_activeWaitSeconds = 0;
    sysml_state_activeBedIndex = 0;
    sysml_state_previousIo0 = false;
    sysml_state_previousIo1 = false;
    sysml_state_previousIo2 = false;
    sysml_state_previousIo3 = false;
    sysml_state_serialTriggerLatched = false;
    sysml_state_serialElapsedMs = 0;
    sysml_state_serialBedIndex = 0;
    sysml_state_serialWaitS = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool* io_4, int* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, int* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    if (sysml_state_serialTriggerLatched) {
        sysml_state_serialTriggerLatched = false;
    }
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousIo0;
    }
    if (sysml_lazy_0) {
        if (sysml_state_pendingMask == 0) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = sysml_state_pendingMask;
        }
        sysml_state_pendingMask = sysml_conditional_1;
        sysml_state_activeBedIndex = 1;
        sysml_state_activeWaitSeconds = 0;
        sysml_state_serialElapsedMs = 0;
        sysml_state_serialBedIndex = 1;
        sysml_state_serialWaitS = 0;
        sysml_state_serialTriggerLatched = true;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousIo1;
    }
    if (sysml_lazy_2) {
        if (sysml_state_pendingMask == 0) {
            sysml_conditional_3 = 2;
        }
        else {
            sysml_conditional_3 = sysml_state_pendingMask;
        }
        sysml_state_pendingMask = sysml_conditional_3;
        sysml_state_activeBedIndex = 2;
        sysml_state_activeWaitSeconds = 0;
        sysml_state_serialElapsedMs = 0;
        sysml_state_serialBedIndex = 2;
        sysml_state_serialWaitS = 0;
        sysml_state_serialTriggerLatched = true;
    }
    sysml_lazy_4 = io_2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousIo2;
    }
    if (sysml_lazy_4) {
        if (sysml_state_pendingMask == 0) {
            sysml_conditional_5 = 4;
        }
        else {
            sysml_conditional_5 = sysml_state_pendingMask;
        }
        sysml_state_pendingMask = sysml_conditional_5;
        sysml_state_activeBedIndex = 3;
        sysml_state_activeWaitSeconds = 0;
        sysml_state_serialElapsedMs = 0;
        sysml_state_serialBedIndex = 3;
        sysml_state_serialWaitS = 0;
        sysml_state_serialTriggerLatched = true;
    }
    sysml_lazy_6 = io_3;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_previousIo3;
    }
    if (sysml_lazy_6) {
        if (sysml_state_pendingMask == 0) {
            sysml_conditional_7 = 0;
        }
        else {
            if (sysml_state_pendingMask == 1) {
                sysml_conditional_8 = 0;
            }
            else {
                if (sysml_state_pendingMask == 2) {
                    sysml_conditional_9 = 0;
                }
                else {
                    if (sysml_state_pendingMask == 4) {
                        sysml_conditional_10 = 0;
                    }
                    else {
                        sysml_conditional_10 = sysml_state_pendingMask;
                    }
                    sysml_conditional_9 = sysml_conditional_10;
                }
                sysml_conditional_8 = sysml_conditional_9;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_state_pendingMask = sysml_conditional_7;
        sysml_state_activeBedIndex = 0;
        sysml_state_activeWaitSeconds = 0;
        sysml_state_serialElapsedMs = 0;
        sysml_state_serialBedIndex = 0;
        sysml_state_serialWaitS = 0;
        sysml_state_serialTriggerLatched = true;
    }
    if ((sysml_state_pendingMask != 0)) {
        sysml_state_activeWaitSeconds = sysml_state_activeWaitSeconds;
    }
    else {
        sysml_state_activeWaitSeconds = 0;
    }
    sysml_state_previousIo0 = io_0;
    sysml_state_previousIo1 = io_1;
    sysml_state_previousIo2 = io_2;
    sysml_state_previousIo3 = io_3;
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_pendingMask != 0);
    int sysml_value_1 = 0;
    if (sysml_state_pendingMask != 0) {
        sysml_conditional_11 = 1000;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_1 = sysml_conditional_11;
    int sysml_value_2 = 0;
    if (sysml_state_pendingMask != 0) {
        sysml_conditional_12 = (sysml_state_activeBedIndex * 100) + sysml_state_activeWaitSeconds;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_2 = sysml_conditional_12;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_serialTriggerLatched;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_serialElapsedMs;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_serialBedIndex;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_serialWaitS;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
}