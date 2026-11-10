#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousDoor = false;
/* Global Variable define of  */
bool sysml_state_previousVibration = false;
/* Global Variable define of  */
int sysml_state_abnormalCount = 0;
/* Global Variable define of  */
int sysml_state_doorOpenCount = 0;
/* Global Variable define of  */
int sysml_state_vibrationEventCount = 0;
/* Global Variable define of  */
bool sysml_state_isAlarmLatched = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousDoor = false;
    sysml_state_previousVibration = false;
    sysml_state_abnormalCount = 0;
    sysml_state_doorOpenCount = 0;
    sysml_state_vibrationEventCount = 0;
    sysml_state_isAlarmLatched = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, int io_6, bool io_6_valid, bool* io_3, int* io_4, int* io_5, bool* serial_0_trigger, double* serial_0_value_0, int* serial_0_value_1, int* serial_0_value_2, int* serial_0_value_3, bool* serial_1_trigger, double* serial_1_value_0, int* serial_1_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    if (io_6_valid) {
        if ((io_6 == 1)) {
            sysml_state_isAlarmLatched = true;
        }
        if ((io_6 == 0)) {
            sysml_state_isAlarmLatched = false;
        }
    }
    if ((io_0 >= 30.0)) {
        sysml_state_abnormalCount = (sysml_state_abnormalCount + 1);
        sysml_state_isAlarmLatched = true;
    }
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousDoor;
    }
    if (sysml_lazy_0) {
        sysml_state_doorOpenCount = (sysml_state_doorOpenCount + 1);
        sysml_state_abnormalCount = (sysml_state_abnormalCount + 1);
        sysml_state_isAlarmLatched = true;
    }
    sysml_lazy_1 = io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousVibration;
    }
    if (sysml_lazy_1) {
        sysml_state_vibrationEventCount = (sysml_state_vibrationEventCount + 1);
    }
    sysml_state_previousDoor = io_2;
    sysml_state_previousVibration = io_1;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_isAlarmLatched;
    int sysml_value_1 = 0;
    if (sysml_state_isAlarmLatched) {
        sysml_conditional_2 = 2000;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_1 = sysml_conditional_2;
    int sysml_value_2 = 0;
    if (io_0 >= 30.0) {
        sysml_conditional_3 = sysml_state_abnormalCount;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_2 = sysml_conditional_3;
    bool sysml_value_3 = 0;
    sysml_lazy_4 = io_0 >= 30.0;
    if (!sysml_lazy_4) {
        sysml_lazy_5 = io_2;
        if (sysml_lazy_5) {
            sysml_lazy_5 = !sysml_state_previousDoor;
        }
        sysml_lazy_4 = sysml_lazy_5;
    }
    sysml_lazy_6 = sysml_lazy_4;
    if (!sysml_lazy_6) {
        sysml_lazy_7 = io_1;
        if (sysml_lazy_7) {
            sysml_lazy_7 = !sysml_state_previousVibration;
        }
        sysml_lazy_6 = sysml_lazy_7;
    }
    sysml_lazy_8 = sysml_lazy_6;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_isAlarmLatched;
    }
    sysml_value_3 = sysml_lazy_8;
    double sysml_value_4 = 0;
    if (io_6_valid) {
        sysml_conditional_9 = io_0;
    }
    else {
        sysml_conditional_9 = io_0;
    }
    sysml_value_4 = sysml_conditional_9;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_abnormalCount;
    int sysml_value_6 = 0;
    if (io_6_valid) {
        sysml_conditional_10 = 0 + 0;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_6 = sysml_conditional_10;
    int sysml_value_7 = 0;
    if (io_6_valid) {
        sysml_conditional_11 = 0 + 0;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_7 = sysml_conditional_11;
    bool sysml_value_8 = 0;
    sysml_lazy_12 = !sysml_state_isAlarmLatched;
    if (sysml_lazy_12) {
        sysml_lazy_12 = io_6_valid;
    }
    sysml_value_8 = sysml_lazy_12;
    double sysml_value_9 = 0;
    sysml_value_9 = io_0;
    int sysml_value_10 = 0;
    sysml_value_10 = sysml_state_abnormalCount;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
    *serial_1_trigger = sysml_value_8;
    *serial_1_value_0 = sysml_value_9;
    *serial_1_value_1 = sysml_value_10;
}