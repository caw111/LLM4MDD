#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousIo0 = false;
/* Global Variable define of  */
bool sysml_state_previousIo1 = false;
/* Global Variable define of  */
bool sysml_state_previousIo2 = false;
/* Global Variable define of  */
int sysml_state_highestBedId = 0;
/* Global Variable define of  */
int sysml_state_oldestWaitingBedId = 0;
/* Global Variable define of  */
int sysml_state_ageOfOldestMs = 0;
/* Global Variable define of  */
bool sysml_state_callActive = false;
/* Global Variable define of  */
bool sysml_state_serialPending = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousIo0 = false;
    sysml_state_previousIo1 = false;
    sysml_state_previousIo2 = false;
    sysml_state_highestBedId = 0;
    sysml_state_oldestWaitingBedId = 0;
    sysml_state_ageOfOldestMs = 0;
    sysml_state_callActive = false;
    sysml_state_serialPending = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool* io_4, int* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_callActive;
    }
    if (sysml_lazy_0) {
        sysml_state_callActive = false;
        sysml_state_highestBedId = 0;
        sysml_state_oldestWaitingBedId = 0;
        sysml_state_ageOfOldestMs = 0;
        sysml_state_serialPending = true;
    }
    else {
        sysml_lazy_1 = io_0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = !sysml_state_previousIo0;
        }
        if (sysml_lazy_1) {
            sysml_state_callActive = true;
            sysml_state_highestBedId = 1;
            sysml_state_oldestWaitingBedId = 1;
            sysml_state_ageOfOldestMs = 0;
        }
        sysml_lazy_2 = io_1;
        if (sysml_lazy_2) {
            sysml_lazy_2 = !sysml_state_previousIo1;
        }
        if (sysml_lazy_2) {
            sysml_state_callActive = true;
            if (sysml_state_highestBedId >= 2) {
                sysml_conditional_3 = sysml_state_highestBedId;
            }
            else {
                sysml_conditional_3 = 2;
            }
            sysml_state_highestBedId = sysml_conditional_3;
            if (sysml_state_callActive) {
                sysml_conditional_4 = sysml_state_oldestWaitingBedId;
            }
            else {
                sysml_conditional_4 = 2;
            }
            sysml_state_oldestWaitingBedId = sysml_conditional_4;
            if (sysml_state_callActive) {
                sysml_conditional_5 = sysml_state_ageOfOldestMs;
            }
            else {
                sysml_conditional_5 = 0;
            }
            sysml_state_ageOfOldestMs = sysml_conditional_5;
        }
        sysml_lazy_6 = io_2;
        if (sysml_lazy_6) {
            sysml_lazy_6 = !sysml_state_previousIo2;
        }
        if (sysml_lazy_6) {
            sysml_state_callActive = true;
            if (sysml_state_highestBedId >= 3) {
                sysml_conditional_7 = sysml_state_highestBedId;
            }
            else {
                sysml_conditional_7 = 3;
            }
            sysml_state_highestBedId = sysml_conditional_7;
            if (sysml_state_callActive) {
                sysml_conditional_8 = sysml_state_oldestWaitingBedId;
            }
            else {
                sysml_conditional_8 = 3;
            }
            sysml_state_oldestWaitingBedId = sysml_conditional_8;
            if (sysml_state_callActive) {
                sysml_conditional_9 = sysml_state_ageOfOldestMs;
            }
            else {
                sysml_conditional_9 = 0;
            }
            sysml_state_ageOfOldestMs = sysml_conditional_9;
        }
    }
    if ((!io_3)) {
        sysml_state_serialPending = false;
    }
    sysml_state_previousIo0 = io_0;
    sysml_state_previousIo1 = io_1;
    sysml_state_previousIo2 = io_2;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_callActive;
    int sysml_value_1 = 0;
    if (sysml_state_callActive) {
        sysml_conditional_10 = 1000;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_1 = sysml_conditional_10;
    int sysml_value_2 = 0;
    if (sysml_state_callActive) {
        sysml_conditional_11 = sysml_state_oldestWaitingBedId;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_2 = sysml_conditional_11;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_serialPending;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_ageOfOldestMs;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_highestBedId;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
}