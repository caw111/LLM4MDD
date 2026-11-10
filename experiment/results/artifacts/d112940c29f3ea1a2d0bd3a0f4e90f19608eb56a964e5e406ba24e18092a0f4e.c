#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousProximity = false;
/* Global Variable define of  */
bool sysml_state_valveOn = false;
/* Global Variable define of  */
int sysml_state_valveRemainingMs = 0;
/* Global Variable define of  */
int sysml_state_todayUses = 0;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Global Variable define of  */
int sysml_state_alarmRemainingMs = 0;
/* Global Variable define of  */
bool sysml_state_lastDistanceValid = false;
/* Global Variable define of  */
int sysml_state_lastDisplayedPercent = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousProximity = false;
    sysml_state_valveOn = false;
    sysml_state_valveRemainingMs = 0;
    sysml_state_todayUses = 0;
    sysml_state_alarmActive = false;
    sysml_state_alarmRemainingMs = 0;
    sysml_state_lastDistanceValid = false;
    sysml_state_lastDisplayedPercent = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_2, bool io_3, bool* io_1, int* io_4, int* io_5, bool* io_6, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_2 >= 2.0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2 <= 400.0;
    }
    if (sysml_lazy_1) {
        sysml_state_lastDistanceValid = true;
    }
    else {
        sysml_state_lastDistanceValid = false;
    }
    sysml_lazy_2 = io_0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousProximity;
    }
    if (sysml_lazy_2) {
        sysml_lazy_3 = io_3;
        if (sysml_lazy_3) {
            sysml_lazy_3 = io_2 < 10.0;
        }
        if (sysml_lazy_3) {
            sysml_state_alarmActive = true;
            sysml_state_alarmRemainingMs = 3000;
            sysml_state_valveOn = false;
            sysml_state_valveRemainingMs = 0;
        }
        else {
            sysml_state_todayUses = (sysml_state_todayUses + 1);
            sysml_state_valveOn = true;
            sysml_state_valveRemainingMs = 5000;
            sysml_state_alarmActive = false;
            sysml_state_alarmRemainingMs = 0;
        }
    }
    if (sysml_state_valveOn) {
        sysml_state_valveRemainingMs = (sysml_state_valveRemainingMs - 5);
        if ((sysml_state_valveRemainingMs <= 0)) {
            sysml_state_valveOn = false;
            sysml_state_valveRemainingMs = 0;
        }
    }
    if (sysml_state_alarmActive) {
        sysml_state_alarmRemainingMs = (sysml_state_alarmRemainingMs - 5);
        if ((sysml_state_alarmRemainingMs <= 0)) {
            sysml_state_alarmActive = false;
            sysml_state_alarmRemainingMs = 0;
        }
    }
    if (sysml_state_lastDistanceValid) {
        if (io_2 < 2.0) {
            sysml_conditional_4 = 0;
        }
        else {
            if (io_2 > 400.0) {
                sysml_conditional_5 = 100;
            }
            else {
                if (((((double)(io_2 - 2.0)) / 398.0) * 100.0) < ((double)((int)((((double)(io_2 - 2.0)) / 398.0) * 100.0)))) {
                    sysml_conditional_6 = 1;
                }
                else {
                    sysml_conditional_6 = 0;
                }
                sysml_conditional_5 = ((int)((((double)(io_2 - 2.0)) / 398.0) * 100.0)) - sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_state_lastDisplayedPercent = sysml_conditional_4;
    }
    sysml_state_previousProximity = io_0;
    bool sysml_value_0 = 0;
    sysml_lazy_7 = sysml_state_valveOn;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_lastDistanceValid;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = !sysml_state_alarmActive;
    }
    sysml_value_0 = sysml_lazy_8;
    int sysml_value_1 = 0;
    if (sysml_state_lastDistanceValid) {
        sysml_conditional_9 = sysml_state_lastDisplayedPercent;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_1 = sysml_conditional_9;
    int sysml_value_2 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_10 = 2000;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_2 = sysml_conditional_10;
    bool sysml_value_3 = 0;
    if (!sysml_state_lastDistanceValid) {
        sysml_conditional_11 = true;
    }
    else {
        if (sysml_state_alarmActive) {
            sysml_conditional_12 = true;
        }
        else {
            sysml_conditional_12 = io_0;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    sysml_value_3 = sysml_conditional_11;
    bool sysml_value_4 = 0;
    sysml_value_4 = io_0;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_todayUses;
    double sysml_value_6 = 0;
    if (sysml_state_lastDistanceValid) {
        if ((((double)(sysml_state_lastDisplayedPercent * 1.0)) / 1.0) < ((double)((int)(((double)(sysml_state_lastDisplayedPercent * 1.0)) / 1.0)))) {
            sysml_conditional_14 = 1;
        }
        else {
            sysml_conditional_14 = 0;
        }
        sysml_conditional_13 = ((int)(((double)(sysml_state_lastDisplayedPercent * 1.0)) / 1.0)) - sysml_conditional_14;
    }
    else {
        sysml_conditional_13 = 0.0;
    }
    sysml_value_6 = sysml_conditional_13;
    *io_1 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
}