#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevVehicleApproach = false;
/* Global Variable define of  */
bool sysml_state_prevPedApproach = false;
/* Global Variable define of  */
bool sysml_state_prevDanger = false;
/* Global Variable define of  */
bool sysml_state_dangerActive = false;
/* Global Variable define of  */
int sysml_state_dangerElapsedMs = 0;
/* Global Variable define of  */
int sysml_state_dangerMaxElapsedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevVehicleApproach = false;
    sysml_state_prevPedApproach = false;
    sysml_state_prevDanger = false;
    sysml_state_dangerActive = false;
    sysml_state_dangerElapsedMs = 0;
    sysml_state_dangerMaxElapsedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, bool* io_4, bool* io_5, int* io_6, int* io_7, bool* io_8, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    int sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    int sysml_conditional_16 = 0;
    sysml_lazy_0 = io_0 >= 2.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    if (sysml_lazy_0) {
        sysml_state_prevVehicleApproach = true;
    }
    else {
        sysml_state_prevVehicleApproach = false;
    }
    sysml_lazy_1 = io_2 >= 2.0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_3;
    }
    if (sysml_lazy_1) {
        sysml_state_prevPedApproach = true;
    }
    else {
        sysml_state_prevPedApproach = false;
    }
    sysml_lazy_2 = sysml_state_prevVehicleApproach;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_prevPedApproach;
    }
    if (sysml_lazy_2) {
        sysml_state_dangerActive = true;
    }
    else {
        sysml_state_dangerActive = false;
    }
    sysml_lazy_3 = sysml_state_dangerActive;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevDanger;
    }
    if (sysml_lazy_3) {
        sysml_state_dangerElapsedMs = 0;
    }
    if (sysml_state_dangerActive) {
        sysml_state_dangerElapsedMs = sysml_state_dangerElapsedMs;
    }
    sysml_lazy_4 = !sysml_state_dangerActive;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_prevDanger;
    }
    if (sysml_lazy_4) {
        if (sysml_state_dangerElapsedMs >= sysml_state_dangerMaxElapsedMs) {
            sysml_conditional_5 = sysml_state_dangerElapsedMs;
        }
        else {
            sysml_conditional_5 = sysml_state_dangerMaxElapsedMs;
        }
        sysml_state_dangerMaxElapsedMs = sysml_conditional_5;
    }
    sysml_state_prevDanger = sysml_state_dangerActive;
    if ((!sysml_state_dangerActive)) {
        sysml_state_dangerElapsedMs = sysml_state_dangerElapsedMs;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_dangerActive) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_0 = sysml_conditional_6;
    bool sysml_value_1 = 0;
    if (!sysml_state_dangerActive) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_1 = sysml_conditional_7;
    int sysml_value_2 = 0;
    if (sysml_state_dangerActive) {
        sysml_conditional_8 = 2000;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_2 = sysml_conditional_8;
    int sysml_value_3 = 0;
    sysml_lazy_9 = sysml_state_dangerActive;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_0 <= 20.0;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 3;
    }
    else {
        sysml_lazy_11 = sysml_state_dangerActive;
        if (sysml_lazy_11) {
            sysml_lazy_11 = io_2 <= 20.0;
        }
        if (sysml_lazy_11) {
            sysml_conditional_12 = 3;
        }
        else {
            if (sysml_state_dangerActive) {
                sysml_conditional_13 = 2;
            }
            else {
                sysml_conditional_13 = 0;
            }
            sysml_conditional_12 = sysml_conditional_13;
        }
        sysml_conditional_10 = sysml_conditional_12;
    }
    sysml_value_3 = sysml_conditional_10;
    bool sysml_value_4 = 0;
    sysml_value_4 = false;
    bool sysml_value_5 = 0;
    sysml_lazy_14 = !sysml_state_dangerActive;
    if (sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_prevDanger;
    }
    sysml_value_5 = sysml_lazy_14;
    int sysml_value_6 = 0;
    sysml_lazy_15 = !sysml_state_dangerActive;
    if (sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_prevDanger;
    }
    if (sysml_lazy_15) {
        sysml_conditional_16 = sysml_state_dangerElapsedMs;
    }
    else {
        sysml_conditional_16 = 0;
    }
    sysml_value_6 = sysml_conditional_16;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *io_7 = sysml_value_3;
    *io_8 = sysml_value_4;
    *serial_0_trigger = sysml_value_5;
    *serial_0_value_0 = sysml_value_6;
}