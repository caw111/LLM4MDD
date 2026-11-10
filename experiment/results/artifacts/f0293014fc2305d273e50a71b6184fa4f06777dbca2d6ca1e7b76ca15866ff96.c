#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousDoor = false;
/* Global Variable define of  */
bool sysml_state_previousTempAbnormal = false;
/* Global Variable define of  */
int sysml_state_abnormalCount = 0;
/* Global Variable define of  */
int sysml_state_vibrationCount = 0;
/* Global Variable define of  */
bool sysml_state_displayModeTemperature = true;
/* Global Variable define of  */
bool sysml_state_serialGatePending = false;
/* Global Variable define of  */
int sysml_state_serialPosition = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousDoor = false;
    sysml_state_previousTempAbnormal = false;
    sysml_state_abnormalCount = 0;
    sysml_state_vibrationCount = 0;
    sysml_state_displayModeTemperature = true;
    sysml_state_serialGatePending = false;
    sysml_state_serialPosition = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, int io_6, bool io_6_valid, int* io_3, int* io_4, bool* io_5, bool* serial_0_trigger, double* serial_0_value_0, int* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_conditional_13 = 0;
    sysml_lazy_0 = io_6_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_serialGatePending;
    }
    if (sysml_lazy_0) {
        sysml_state_serialPosition = io_6;
        sysml_state_serialGatePending = false;
    }
    sysml_lazy_1 = io_0 > 30.0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousTempAbnormal;
    }
    if (sysml_lazy_1) {
        sysml_state_abnormalCount = (sysml_state_abnormalCount + 1);
        sysml_state_serialGatePending = true;
    }
    sysml_lazy_2 = io_2;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousDoor;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_displayModeTemperature == true;
    }
    if (sysml_lazy_3) {
        sysml_state_abnormalCount = (sysml_state_abnormalCount + 1);
        sysml_state_serialGatePending = true;
    }
    sysml_lazy_4 = io_2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousDoor;
    }
    if (sysml_lazy_4) {
        sysml_state_abnormalCount = (sysml_state_abnormalCount + 1);
        sysml_state_serialGatePending = true;
    }
    sysml_lazy_5 = io_1;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_displayModeTemperature == true;
    }
    if (sysml_lazy_5) {
        sysml_state_vibrationCount = (sysml_state_vibrationCount + 1);
        sysml_state_displayModeTemperature = false;
    }
    sysml_lazy_6 = io_1;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_displayModeTemperature == false;
    }
    if (sysml_lazy_6) {
        sysml_state_vibrationCount = (sysml_state_vibrationCount + 1);
        sysml_state_displayModeTemperature = true;
    }
    sysml_lazy_7 = io_0 > 30.0;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_2;
    }
    if (sysml_lazy_7) {
        sysml_state_serialGatePending = true;
    }
    sysml_state_previousDoor = io_2;
    sysml_state_previousTempAbnormal = (io_0 > 30.0);
    int sysml_value_0 = 0;
    if (sysml_state_displayModeTemperature) {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_9 = 1;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_conditional_8 = ((int)io_0) - sysml_conditional_9;
    }
    else {
        sysml_conditional_8 = sysml_state_abnormalCount;
    }
    sysml_value_0 = sysml_conditional_8;
    int sysml_value_1 = 0;
    sysml_lazy_10 = io_0 > 30.0;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = io_2;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = 1000;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_1 = sysml_conditional_11;
    bool sysml_value_2 = 0;
    sysml_lazy_12 = io_0 > 30.0;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = io_2;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_2 = sysml_conditional_13;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_serialGatePending;
    double sysml_value_4 = 0;
    sysml_value_4 = io_0;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_abnormalCount;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
}