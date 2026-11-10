#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_currentFloor = 1;
/* Global Variable define of  */
int sysml_state_requestedFloor = 1;
/* Global Variable define of  */
bool sysml_state_movementActive = false;
/* Global Variable define of  */
int sysml_state_movementRemainingSteps = 0;
/* Global Variable define of  */
bool sysml_state_previousIo0 = false;
/* Global Variable define of  */
bool sysml_state_previousIo1 = false;
/* Global Variable define of  */
bool sysml_state_previousIo2 = false;
/* Global Variable define of  */
bool sysml_state_previousOverload = false;
/* Global Variable define of  */
bool sysml_state_overload = false;
/* Global Variable define of  */
int sysml_state_loadPercent = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_currentFloor = 1;
    sysml_state_requestedFloor = 1;
    sysml_state_movementActive = false;
    sysml_state_movementRemainingSteps = 0;
    sysml_state_previousIo0 = false;
    sysml_state_previousIo1 = false;
    sysml_state_previousIo2 = false;
    sysml_state_previousOverload = false;
    sysml_state_overload = false;
    sysml_state_loadPercent = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int* io_4, bool* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, bool* serial_1_trigger, double* serial_1_value_0, int* serial_1_value_1, bool* serial_1_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
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
    bool sysml_lazy_13 = 0;
    bool sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    if (io_3 >= 0.0) {
        if (io_3 <= 1023.0) {
            if ((((double)(io_3 * 100.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 100.0)) / 1023.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int)(((double)(io_3 * 100.0)) / 1023.0)) - sysml_conditional_2;
        }
        else {
            sysml_conditional_1 = 100;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_loadPercent = sysml_conditional_0;
    sysml_state_overload = (sysml_state_loadPercent >= 80);
    sysml_lazy_3 = io_0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousIo0;
    }
    if (sysml_lazy_3) {
        sysml_state_requestedFloor = 1;
    }
    sysml_lazy_4 = io_1;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousIo1;
    }
    if (sysml_lazy_4) {
        sysml_state_requestedFloor = 2;
    }
    sysml_lazy_5 = io_2;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousIo2;
    }
    if (sysml_lazy_5) {
        sysml_state_requestedFloor = 3;
    }
    if (sysml_state_movementActive) {
        sysml_state_movementRemainingSteps = (sysml_state_movementRemainingSteps - 1);
        if ((sysml_state_movementRemainingSteps <= 0)) {
            sysml_state_movementActive = false;
            sysml_state_currentFloor = sysml_state_requestedFloor;
        }
    }
    sysml_lazy_6 = io_0;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = io_1;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = io_2;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = !sysml_state_movementActive;
    }
    if (sysml_lazy_8) {
        if ((!sysml_state_overload)) {
            sysml_state_movementActive = true;
            sysml_state_movementRemainingSteps = 20;
        }
    }
    if (sysml_state_overload) {
        sysml_state_movementActive = false;
        sysml_state_movementRemainingSteps = 0;
    }
    sysml_state_previousIo0 = io_0;
    sysml_state_previousIo1 = io_1;
    sysml_state_previousIo2 = io_2;
    sysml_state_previousOverload = sysml_state_overload;
    int sysml_value_0 = 0;
    if (sysml_state_overload) {
        sysml_conditional_9 = 1000;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_0 = sysml_conditional_9;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_overload;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_currentFloor;
    bool sysml_value_3 = 0;
    if (sysml_state_movementActive == false) {
        sysml_lazy_11 = sysml_state_movementRemainingSteps == 0;
        if (sysml_lazy_11) {
            sysml_lazy_11 = !sysml_state_overload;
        }
        sysml_conditional_10 = sysml_lazy_11;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_3 = sysml_conditional_10;
    int sysml_value_4 = 0;
    if (sysml_state_movementActive == false) {
        sysml_conditional_12 = 0;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_4 = sysml_conditional_12;
    bool sysml_value_5 = 0;
    sysml_lazy_13 = sysml_state_overload;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !sysml_state_previousOverload;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_5 = sysml_conditional_14;
    double sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_loadPercent * 1.0);
    int sysml_value_7 = 0;
    if (sysml_state_overload) {
        sysml_conditional_15 = sysml_state_requestedFloor;
    }
    else {
        sysml_conditional_15 = 1;
    }
    sysml_value_7 = sysml_conditional_15;
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