#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_currentFloor = 1;
/* Global Variable define of  */
int sysml_state_targetFloor = 1;
/* Global Variable define of  */
int sysml_state_movementRemainingMs = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed0 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed1 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed2 = false;
/* Global Variable define of  */
bool sysml_state_overload = false;
/* Global Variable define of  */
bool sysml_state_movementCompletePending = false;
/* Global Variable define of  */
int sysml_state_movementCompleteLastDurationMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_currentFloor = 1;
    sysml_state_targetFloor = 1;
    sysml_state_movementRemainingMs = 0;
    sysml_state_previousPressed0 = false;
    sysml_state_previousPressed1 = false;
    sysml_state_previousPressed2 = false;
    sysml_state_overload = false;
    sysml_state_movementCompletePending = false;
    sysml_state_movementCompleteLastDurationMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int* io_4, bool* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, bool* serial_1_trigger, double* serial_1_value_0, int* serial_1_value_1, bool* serial_1_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    sysml_lazy_0 = io_0;
    if (!sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = io_2;
    }
    if (sysml_lazy_1) {
    }
    if ((io_3 >= 900.0)) {
        sysml_state_overload = true;
    }
    else {
        sysml_state_overload = false;
    }
    sysml_lazy_2 = io_0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressed0;
    }
    if (sysml_lazy_2) {
        sysml_state_targetFloor = 1;
    }
    sysml_lazy_3 = io_1;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousPressed1;
    }
    if (sysml_lazy_3) {
        sysml_state_targetFloor = 2;
    }
    sysml_lazy_4 = io_2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousPressed2;
    }
    if (sysml_lazy_4) {
        sysml_state_targetFloor = 3;
    }
    sysml_lazy_5 = !sysml_state_overload;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_targetFloor != sysml_state_currentFloor;
    }
    if (sysml_lazy_5) {
        sysml_state_movementRemainingMs = 1000;
        sysml_state_movementCompletePending = false;
    }
    sysml_lazy_6 = sysml_state_movementRemainingMs == 0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_currentFloor == sysml_state_targetFloor;
    }
    if (sysml_lazy_6) {
        sysml_state_movementCompletePending = false;
    }
    sysml_lazy_7 = sysml_state_movementRemainingMs > 0;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_overload;
    }
    if (sysml_lazy_7) {
        sysml_state_movementRemainingMs = 0;
        sysml_state_currentFloor = sysml_state_targetFloor;
        sysml_state_movementCompletePending = true;
        sysml_state_movementCompleteLastDurationMs = 1000;
    }
    sysml_lazy_8 = sysml_state_overload;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_currentFloor != sysml_state_targetFloor;
    }
    if (sysml_lazy_8) {
        sysml_state_movementRemainingMs = 0;
        sysml_state_movementCompletePending = false;
    }
    sysml_state_previousPressed0 = io_0;
    sysml_state_previousPressed1 = io_1;
    sysml_state_previousPressed2 = io_2;
    int sysml_value_0 = 0;
    if (sysml_state_overload) {
        sysml_conditional_9 = 1000;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_0 = sysml_conditional_9;
    bool sysml_value_1 = 0;
    if (sysml_state_overload) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_1 = sysml_conditional_10;
    int sysml_value_2 = 0;
    sysml_lazy_11 = sysml_state_overload;
    if (sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_currentFloor == 1;
    }
    if (sysml_lazy_11) {
        sysml_conditional_12 = 10;
    }
    else {
        if (sysml_state_currentFloor == 1) {
            sysml_conditional_13 = 1;
        }
        else {
            if (sysml_state_currentFloor == 2) {
                sysml_conditional_14 = 2;
            }
            else {
                sysml_conditional_14 = 3;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_2 = sysml_conditional_12;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_movementCompletePending;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_movementCompleteLastDurationMs;
    bool sysml_value_5 = 0;
    sysml_value_5 = sysml_state_overload;
    double sysml_value_6 = 0;
    if (io_3 >= 900.0) {
        sysml_conditional_15 = 100.0;
    }
    else {
        sysml_conditional_15 = 0.0;
    }
    sysml_value_6 = sysml_conditional_15;
    int sysml_value_7 = 0;
    sysml_value_7 = sysml_state_currentFloor;
    bool sysml_value_8 = 0;
    sysml_value_8 = sysml_state_overload;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_1_trigger = sysml_value_5;
    *serial_1_value_0 = sysml_value_6;
    *serial_1_value_1 = sysml_value_7;
    *serial_1_value_2 = sysml_value_8;
}