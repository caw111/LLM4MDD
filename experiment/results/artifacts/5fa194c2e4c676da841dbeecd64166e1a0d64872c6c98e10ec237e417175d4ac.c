#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevDoorOpen = false;
/* Global Variable define of  */
int sysml_state_doorOpenElapsedMs = 0;
/* Global Variable define of  */
int sysml_state_accRunDoorPauseMs = 0;
/* Global Variable define of  */
int sysml_state_alarmLevel = 0;
/* Global Variable define of  */
bool sysml_state_inDefrost = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevDoorOpen = false;
    sysml_state_doorOpenElapsedMs = 0;
    sysml_state_accRunDoorPauseMs = 0;
    sysml_state_alarmLevel = 0;
    sysml_state_inDefrost = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int io_7, double* io_3, bool* io_4, int* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, bool* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    if (io_1 >= 1000.0) {
        sysml_conditional_0 = true;
    }
    else {
        sysml_conditional_0 = sysml_state_inDefrost;
    }
    sysml_state_inDefrost = sysml_conditional_0;
    sysml_lazy_1 = io_0 >= 3000.0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = io_1 >= 2000.0;
    }
    if (sysml_lazy_1) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_state_alarmLevel = sysml_conditional_2;
    if (io_2) {
        sysml_state_doorOpenElapsedMs = (sysml_state_doorOpenElapsedMs + io_7);
    }
    else {
        sysml_state_doorOpenElapsedMs = 0;
    }
    if ((sysml_state_doorOpenElapsedMs >= 600000)) {
        sysml_state_alarmLevel = 2;
    }
    sysml_lazy_3 = io_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevDoorOpen;
    }
    if (sysml_lazy_3) {
        sysml_state_accRunDoorPauseMs = (sysml_state_accRunDoorPauseMs + io_7);
    }
    sysml_state_prevDoorOpen = io_2;
    double sysml_value_0 = 0;
    sysml_lazy_4 = !io_2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_inDefrost;
    }
    if (sysml_lazy_4) {
        if (io_0 > 2500.0) {
            sysml_conditional_6 = 255.0;
        }
        else {
            sysml_conditional_6 = 128.0;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_0 = sysml_conditional_5;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_inDefrost;
    int sysml_value_2 = 0;
    if (sysml_state_alarmLevel >= 2) {
        sysml_conditional_7 = 2000;
    }
    else {
        if (sysml_state_alarmLevel >= 1) {
            sysml_conditional_8 = 1000;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    sysml_value_2 = sysml_conditional_7;
    int sysml_value_3 = 0;
    if (io_2) {
        sysml_conditional_9 = 9;
    }
    else {
        if (sysml_state_inDefrost) {
            sysml_conditional_10 = 8;
        }
        else {
            sysml_conditional_10 = sysml_state_alarmLevel;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_3 = sysml_conditional_9;
    bool sysml_value_4 = 0;
    sysml_value_4 = true;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_alarmLevel;
    double sysml_value_6 = 0;
    sysml_value_6 = io_0;
    double sysml_value_7 = 0;
    sysml_value_7 = io_1;
    bool sysml_value_8 = 0;
    sysml_value_8 = io_2;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
    *serial_0_value_2 = sysml_value_7;
    *serial_0_value_3 = sysml_value_8;
}