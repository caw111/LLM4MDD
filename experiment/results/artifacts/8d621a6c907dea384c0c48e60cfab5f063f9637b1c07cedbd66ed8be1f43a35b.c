#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_totalCount = 0;
/* Global Variable define of  */
int sysml_state_stopCount = 0;
/* Global Variable define of  */
bool sysml_state_prevD2 = false;
/* Global Variable define of  */
bool sysml_state_prevD3 = false;
/* Global Variable define of  */
bool sysml_state_prevD7 = false;
/* Global Variable define of  */
int sysml_state_noProductAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_stopActive = false;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Global Variable define of  */
bool sysml_state_alarmReported = false;
/* Global Variable define of  */
int sysml_state_displayMode = 0;
/* Global Variable define of  */
bool sysml_state_serialGate = false;
/* Global Variable define of  */
int sysml_state_serialTotalCountI = 0;
/* Global Variable define of  */
int sysml_state_serialStopCountI = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_totalCount = 0;
    sysml_state_stopCount = 0;
    sysml_state_prevD2 = false;
    sysml_state_prevD3 = false;
    sysml_state_prevD7 = false;
    sysml_state_noProductAccumMs = 0;
    sysml_state_stopActive = false;
    sysml_state_alarmActive = false;
    sysml_state_alarmReported = false;
    sysml_state_displayMode = 0;
    sysml_state_serialGate = false;
    sysml_state_serialTotalCountI = 0;
    sysml_state_serialStopCountI = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int io_6, bool* io_3, int* io_4, int* io_5, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1, int* serial_0_value_2) {
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
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevD2;
    }
    if (sysml_lazy_0) {
        sysml_state_totalCount = (sysml_state_totalCount + 1);
        sysml_state_noProductAccumMs = 0;
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevD2;
    }
    if (!sysml_lazy_1) {
        sysml_state_noProductAccumMs = (sysml_state_noProductAccumMs + io_6);
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevD3;
    }
    if (sysml_lazy_2) {
        sysml_state_stopActive = true;
        sysml_state_stopCount = (sysml_state_stopCount + 1);
    }
    sysml_lazy_3 = !io_1;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_prevD3;
    }
    if (sysml_lazy_3) {
        sysml_state_stopActive = false;
    }
    sysml_lazy_4 = sysml_state_noProductAccumMs >= 30000;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_stopActive;
    }
    if (sysml_lazy_4) {
        sysml_state_alarmActive = true;
    }
    if (sysml_state_stopActive) {
        sysml_state_alarmActive = true;
    }
    sysml_lazy_5 = !sysml_state_stopActive;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_noProductAccumMs < 30000;
    }
    if (sysml_lazy_5) {
        sysml_state_alarmActive = false;
    }
    sysml_lazy_6 = sysml_state_alarmActive;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_alarmReported;
    }
    if (sysml_lazy_6) {
        sysml_state_serialGate = true;
        sysml_state_serialTotalCountI = sysml_state_totalCount;
        sysml_state_serialStopCountI = sysml_state_stopCount;
        sysml_state_alarmReported = true;
    }
    else {
        sysml_state_serialGate = false;
    }
    if ((!sysml_state_alarmActive)) {
        sysml_state_alarmReported = false;
    }
    sysml_lazy_7 = io_2;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_prevD7;
    }
    if (sysml_lazy_7) {
        if (sysml_state_displayMode < 2) {
            sysml_conditional_8 = sysml_state_displayMode + 1;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_state_displayMode = sysml_conditional_8;
    }
    sysml_state_prevD2 = io_0;
    sysml_state_prevD3 = io_1;
    sysml_state_prevD7 = io_2;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_alarmActive;
    int sysml_value_1 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_9 = 2000;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_1 = sysml_conditional_9;
    int sysml_value_2 = 0;
    if (sysml_state_displayMode == 0) {
        sysml_conditional_10 = sysml_state_totalCount;
    }
    else {
        if (sysml_state_displayMode == 1) {
            if (sysml_state_totalCount == 0) {
                sysml_conditional_12 = 0;
            }
            else {
                if (((((double)(((double)(sysml_state_totalCount * 1.0)) / 600.0)) / 1.0) * 100.0) < ((double)((int)((((double)(((double)(sysml_state_totalCount * 1.0)) / 600.0)) / 1.0) * 100.0)))) {
                    sysml_conditional_13 = 1;
                }
                else {
                    sysml_conditional_13 = 0;
                }
                sysml_conditional_12 = ((int)((((double)(((double)(sysml_state_totalCount * 1.0)) / 600.0)) / 1.0) * 100.0)) - sysml_conditional_13;
            }
            sysml_conditional_11 = sysml_conditional_12;
        }
        else {
            sysml_conditional_11 = sysml_state_stopCount;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    sysml_value_2 = sysml_conditional_10;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_serialGate;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_serialTotalCountI;
    double sysml_value_5 = 0;
    if (sysml_state_serialTotalCountI == 0) {
        sysml_conditional_14 = 0.0;
    }
    else {
        sysml_conditional_14 = ((double)(((double)(sysml_state_serialTotalCountI * 1.0)) / 600.0)) / 1.0;
    }
    sysml_value_5 = sysml_conditional_14;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_serialStopCountI;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
}