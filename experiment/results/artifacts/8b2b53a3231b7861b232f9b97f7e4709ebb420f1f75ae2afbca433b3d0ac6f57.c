#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_currentFloor = 1;
/* Global Variable define of  */
int sysml_state_targetFloor = 1;
/* Global Variable define of  */
bool sysml_state_pendingMove = false;
/* Global Variable define of  */
int sysml_state_moveRemainingMs = 0;
/* Global Variable define of  */
bool sysml_state_previousMoveRequest = false;
/* Global Variable define of  */
bool sysml_state_movementCompletePulse = false;
/* Global Variable define of  */
bool sysml_state_previousPressedAny = false;
/* Global Variable define of  */
int sysml_state_loadPercent = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_currentFloor = 1;
    sysml_state_targetFloor = 1;
    sysml_state_pendingMove = false;
    sysml_state_moveRemainingMs = 0;
    sysml_state_previousMoveRequest = false;
    sysml_state_movementCompletePulse = false;
    sysml_state_previousPressedAny = false;
    sysml_state_loadPercent = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int* io_4, bool* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    int sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    sysml_lazy_0 = io_0;
    if (!sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = io_2;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressedAny;
    }
    if (sysml_lazy_2) {
        sysml_state_movementCompletePulse = false;
        sysml_state_pendingMove = true;
        sysml_state_previousMoveRequest = true;
        if (io_0) {
            sysml_conditional_3 = 1;
        }
        else {
            if (io_1) {
                sysml_conditional_4 = 2;
            }
            else {
                sysml_conditional_4 = 3;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        sysml_state_targetFloor = sysml_conditional_3;
        sysml_state_moveRemainingMs = 1000;
    }
    else {
        if (sysml_state_pendingMove) {
            sysml_state_movementCompletePulse = false;
        }
    }
    if (sysml_state_pendingMove) {
        if (sysml_state_moveRemainingMs <= 0) {
            sysml_conditional_5 = false;
        }
        else {
            sysml_conditional_5 = true;
        }
        sysml_state_pendingMove = sysml_conditional_5;
        if (sysml_state_moveRemainingMs <= 0) {
            sysml_conditional_6 = sysml_state_targetFloor;
        }
        else {
            sysml_conditional_6 = sysml_state_currentFloor;
        }
        sysml_state_currentFloor = sysml_conditional_6;
        if (sysml_state_moveRemainingMs <= 0) {
            sysml_conditional_7 = true;
        }
        else {
            sysml_conditional_7 = false;
        }
        sysml_state_movementCompletePulse = sysml_conditional_7;
    }
    sysml_lazy_8 = io_0;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = io_1;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = io_2;
    }
    if (!sysml_lazy_9) {
        sysml_state_previousMoveRequest = false;
    }
    sysml_lazy_10 = io_0;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = io_1;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = io_2;
    }
    sysml_state_previousPressedAny = sysml_lazy_11;
    int sysml_value_0 = 0;
    if (sysml_state_loadPercent > 80) {
        sysml_conditional_12 = 1000;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_0 = sysml_conditional_12;
    bool sysml_value_1 = 0;
    if (sysml_state_loadPercent > 80) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_1 = sysml_conditional_13;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_currentFloor;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_movementCompletePulse;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_targetFloor;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
}