#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Global Variable define of  */
bool sysml_state_modeIsSetting = false;
/* Global Variable define of  */
double sysml_state_targetTemp = 37.0;
/* Global Variable define of  */
double sysml_state_tempLastValid = 25.0;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Global Variable define of  */
int sysml_state_abnormalHighCount = 0;
/* Global Variable define of  */
int sysml_state_abnormalLowCount = 0;
/* Global Variable define of  */
bool sysml_state_fanOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevPressed = false;
    sysml_state_modeIsSetting = false;
    sysml_state_targetTemp = 37.0;
    sysml_state_tempLastValid = 25.0;
    sysml_state_alarmActive = false;
    sysml_state_abnormalHighCount = 0;
    sysml_state_abnormalLowCount = 0;
    sysml_state_fanOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_5, double io_6, bool io_7, bool* io_1, bool* io_2, bool* io_3, int* io_4, int* io_8, bool* serial_0_trigger, bool* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    if (io_7) {
        sysml_state_tempLastValid = io_5;
    }
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_modeIsSetting = (!sysml_state_modeIsSetting);
        if (sysml_state_modeIsSetting) {
            sysml_conditional_1 = false;
        }
        else {
            sysml_conditional_1 = true;
        }
        if (sysml_conditional_1) {
            sysml_conditional_2 = 37.0;
        }
        else {
            sysml_conditional_2 = 35.0;
        }
        sysml_state_targetTemp = sysml_conditional_2;
    }
    sysml_state_prevPressed = io_0;
    sysml_lazy_3 = io_7;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_tempLastValid >= 38.0;
    }
    if (sysml_lazy_3) {
        sysml_state_abnormalHighCount = (sysml_state_abnormalHighCount + 1);
    }
    else {
        sysml_state_abnormalHighCount = 0;
    }
    sysml_lazy_4 = io_7;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_tempLastValid <= 36.0;
    }
    if (sysml_lazy_4) {
        sysml_state_abnormalLowCount = (sysml_state_abnormalLowCount + 1);
    }
    else {
        sysml_state_abnormalLowCount = 0;
    }
    sysml_lazy_5 = sysml_state_abnormalHighCount >= 6000;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_abnormalLowCount >= 6000;
    }
    if (sysml_lazy_5) {
        sysml_state_alarmActive = true;
    }
    else {
        sysml_state_alarmActive = false;
    }
    sysml_lazy_6 = !sysml_state_alarmActive;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_7;
    }
    if (sysml_lazy_6) {
        if (sysml_state_fanOn) {
            sysml_conditional_7 = false;
        }
        else {
            sysml_conditional_7 = true;
        }
        sysml_state_fanOn = sysml_conditional_7;
    }
    else {
        sysml_state_fanOn = false;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_8 = false;
    }
    else {
        sysml_lazy_9 = io_7;
        if (sysml_lazy_9) {
            sysml_lazy_9 = sysml_state_tempLastValid < sysml_state_targetTemp;
        }
        sysml_conditional_8 = sysml_lazy_9;
    }
    sysml_value_0 = sysml_conditional_8;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_alarmActive;
    bool sysml_value_2 = 0;
    sysml_lazy_10 = sysml_state_fanOn;
    if (sysml_lazy_10) {
        sysml_lazy_10 = !sysml_state_alarmActive;
    }
    sysml_value_2 = sysml_lazy_10;
    int sysml_value_3 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_11 = 2000;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_3 = sysml_conditional_11;
    int sysml_value_4 = 0;
    if (sysml_state_modeIsSetting) {
        if (sysml_state_targetTemp < ((double)((int)sysml_state_targetTemp))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        sysml_conditional_12 = ((int)sysml_state_targetTemp) - sysml_conditional_13;
    }
    else {
        if (sysml_state_tempLastValid < ((double)((int)sysml_state_tempLastValid))) {
            sysml_conditional_14 = 1;
        }
        else {
            sysml_conditional_14 = 0;
        }
        sysml_conditional_12 = ((int)sysml_state_tempLastValid) - sysml_conditional_14;
    }
    sysml_value_4 = sysml_conditional_12;
    bool sysml_value_5 = 0;
    sysml_value_5 = sysml_state_alarmActive;
    bool sysml_value_6 = 0;
    sysml_value_6 = sysml_state_alarmActive;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_8 = sysml_value_4;
    *serial_0_trigger = sysml_value_5;
    *serial_0_value_0 = sysml_value_6;
}