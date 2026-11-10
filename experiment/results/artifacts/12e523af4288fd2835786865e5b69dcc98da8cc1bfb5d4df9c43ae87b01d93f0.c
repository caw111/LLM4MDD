#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_timeSinceTempSample = 0;
/* Global Variable define of  */
int sysml_state_timeSinceDistSample = 0;
/* Global Variable define of  */
int sysml_state_displayPhaseMs = 0;
/* Global Variable define of  */
bool sysml_state_displayPageIsTemp = true;
/* Global Variable define of  */
int sysml_state_buzzerPhaseMs = 0;
/* Global Variable define of  */
bool sysml_state_buzzerWasArmed = false;
/* Global Variable define of  */
bool sysml_state_buzzerPrevAlarm = false;
/* Global Variable define of  */
bool sysml_state_buzzerHasTempAlarm = false;
/* Global Variable define of  */
bool sysml_state_buzzerHasDistAlarm = false;
/* Global Variable define of  */
bool sysml_state_lastReportedTempAlarm = false;
/* Global Variable define of  */
bool sysml_state_lastReportedDistAlarm = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_timeSinceTempSample = 0;
    sysml_state_timeSinceDistSample = 0;
    sysml_state_displayPhaseMs = 0;
    sysml_state_displayPageIsTemp = true;
    sysml_state_buzzerPhaseMs = 0;
    sysml_state_buzzerWasArmed = false;
    sysml_state_buzzerPrevAlarm = false;
    sysml_state_buzzerHasTempAlarm = false;
    sysml_state_buzzerHasDistAlarm = false;
    sysml_state_lastReportedTempAlarm = false;
    sysml_state_lastReportedDistAlarm = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, bool io_4, int io_7, int* io_5, int* io_6, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_conditional_21 = 0;
    sysml_state_timeSinceTempSample = (sysml_state_timeSinceTempSample + io_7);
    sysml_state_timeSinceDistSample = (sysml_state_timeSinceDistSample + io_7);
    if ((sysml_state_timeSinceTempSample >= 2000)) {
        sysml_state_timeSinceTempSample = (sysml_state_timeSinceTempSample - 2000);
    }
    if ((sysml_state_timeSinceDistSample >= 60)) {
        sysml_state_timeSinceDistSample = (sysml_state_timeSinceDistSample - 60);
    }
    sysml_state_displayPhaseMs = (sysml_state_displayPhaseMs + io_7);
    if ((sysml_state_displayPhaseMs >= 2000)) {
        sysml_state_displayPhaseMs = (sysml_state_displayPhaseMs - 2000);
        sysml_state_displayPageIsTemp = (!sysml_state_displayPageIsTemp);
    }
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0 > 32.0;
    }
    if (sysml_lazy_0) {
        sysml_state_buzzerHasTempAlarm = true;
    }
    else {
        sysml_state_buzzerHasTempAlarm = false;
    }
    sysml_lazy_1 = io_4;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_3 < 15.0;
    }
    if (sysml_lazy_1) {
        sysml_state_buzzerHasDistAlarm = true;
    }
    else {
        sysml_state_buzzerHasDistAlarm = false;
    }
    if (sysml_state_buzzerPrevAlarm) {
        sysml_conditional_2 = sysml_state_buzzerPrevAlarm;
    }
    else {
        sysml_conditional_2 = sysml_state_buzzerPrevAlarm;
    }
    sysml_state_buzzerPrevAlarm = sysml_conditional_2;
    sysml_lazy_3 = sysml_state_buzzerHasTempAlarm;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_buzzerHasDistAlarm;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_buzzerWasArmed;
    }
    if (sysml_lazy_4) {
        sysml_state_buzzerWasArmed = true;
        sysml_state_buzzerPhaseMs = 0;
    }
    sysml_lazy_5 = sysml_state_buzzerHasTempAlarm;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_buzzerHasDistAlarm;
    }
    sysml_lazy_6 = !sysml_lazy_5;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_buzzerWasArmed;
    }
    if (sysml_lazy_6) {
        sysml_state_buzzerWasArmed = false;
    }
    if (sysml_state_buzzerWasArmed) {
        sysml_state_buzzerPhaseMs = (sysml_state_buzzerPhaseMs + io_7);
        if ((sysml_state_buzzerPhaseMs >= 1000)) {
            sysml_state_buzzerPhaseMs = (sysml_state_buzzerPhaseMs - 1000);
        }
    }
    else {
        sysml_state_buzzerPhaseMs = 0;
    }
    int sysml_value_0 = 0;
    if (sysml_state_displayPageIsTemp) {
        sysml_lazy_8 = io_2;
        if (sysml_lazy_8) {
            sysml_lazy_8 = io_0 > 0.0;
        }
        if (sysml_lazy_8) {
            if (io_0 < ((double)((int)io_0))) {
                sysml_conditional_10 = 1;
            }
            else {
                sysml_conditional_10 = 0;
            }
            sysml_conditional_9 = ((int)io_0) - sysml_conditional_10;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_conditional_7 = sysml_conditional_9;
    }
    else {
        if (io_4) {
            if (io_3 < ((double)((int)io_3))) {
                sysml_conditional_12 = 1;
            }
            else {
                sysml_conditional_12 = 0;
            }
            sysml_conditional_11 = ((int)io_3) - sysml_conditional_12;
        }
        else {
            sysml_conditional_11 = 0;
        }
        sysml_conditional_7 = sysml_conditional_11;
    }
    sysml_value_0 = sysml_conditional_7;
    int sysml_value_1 = 0;
    if (sysml_state_buzzerWasArmed) {
        if (sysml_state_buzzerPhaseMs < 500) {
            sysml_conditional_14 = 1000;
        }
        else {
            sysml_conditional_14 = 0;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_1 = sysml_conditional_13;
    bool sysml_value_2 = 0;
    sysml_lazy_15 = sysml_state_buzzerWasArmed;
    if (sysml_lazy_15) {
        sysml_lazy_15 = !sysml_state_buzzerPrevAlarm;
    }
    sysml_lazy_16 = sysml_lazy_15;
    if (!sysml_lazy_16) {
        sysml_lazy_17 = sysml_state_buzzerWasArmed;
        if (sysml_lazy_17) {
            sysml_lazy_18 = io_7 == io_7;
            if (sysml_lazy_18) {
                sysml_lazy_18 = true;
            }
            sysml_lazy_17 = sysml_lazy_18 == true;
        }
        sysml_lazy_16 = sysml_lazy_17;
    }
    sysml_lazy_19 = sysml_lazy_16;
    if (!sysml_lazy_19) {
        sysml_lazy_20 = sysml_state_buzzerWasArmed;
        if (sysml_lazy_20) {
            sysml_lazy_20 = sysml_state_buzzerPhaseMs == 0;
        }
        sysml_lazy_19 = sysml_lazy_20;
    }
    if (sysml_lazy_19) {
        sysml_conditional_21 = true;
    }
    else {
        sysml_conditional_21 = false;
    }
    sysml_value_2 = sysml_conditional_21;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_buzzerHasTempAlarm;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_buzzerHasDistAlarm;
    *io_5 = sysml_value_0;
    *io_6 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
    *serial_0_value_1 = sysml_value_4;
}