#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_dhtValidLatched = false;
/* Global Variable define of  */
double sysml_state_tempLatched = 0.0;
/* Global Variable define of  */
double sysml_state_distLatched = 0.0;
/* Global Variable define of  */
bool sysml_state_alarmOn = false;
/* Global Variable define of  */
int sysml_state_alarmPhaseMs = 0;
/* Global Variable define of  */
bool sysml_state_prevAlarmOn = false;
/* Global Variable define of  */
bool sysml_state_lastReasonAny = false;
/* Global Variable define of  */
int sysml_state_displaySwitchAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_displayIsTemp = true;
/* Global Variable define of  */
int sysml_state_reportGateHoldMs = 0;
/* Global Variable define of  */
int sysml_state_reportEveryAccumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_dhtValidLatched = false;
    sysml_state_tempLatched = 0.0;
    sysml_state_distLatched = 0.0;
    sysml_state_alarmOn = false;
    sysml_state_alarmPhaseMs = 0;
    sysml_state_prevAlarmOn = false;
    sysml_state_lastReasonAny = false;
    sysml_state_displaySwitchAccumMs = 0;
    sysml_state_displayIsTemp = true;
    sysml_state_reportGateHoldMs = 0;
    sysml_state_reportEveryAccumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, bool io_4, int io_7, int* io_5, int* io_6, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    if (io_2) {
        sysml_state_dhtValidLatched = true;
        sysml_state_tempLatched = io_0;
    }
    if ((!io_2)) {
        sysml_state_dhtValidLatched = false;
    }
    if (io_4) {
        sysml_state_distLatched = io_3;
    }
    if ((!io_4)) {
        sysml_state_distLatched = sysml_state_distLatched;
    }
    sysml_state_displaySwitchAccumMs = (sysml_state_displaySwitchAccumMs + io_7);
    if ((sysml_state_displaySwitchAccumMs >= 2000)) {
        sysml_state_displaySwitchAccumMs = (sysml_state_displaySwitchAccumMs - 2000);
        sysml_state_displayIsTemp = (!sysml_state_displayIsTemp);
    }
    sysml_lazy_0 = sysml_state_dhtValidLatched;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_tempLatched > 32.0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (!sysml_lazy_1) {
        sysml_lazy_2 = io_4;
        if (sysml_lazy_2) {
            sysml_lazy_2 = sysml_state_distLatched < 15.0;
        }
        sysml_lazy_1 = sysml_lazy_2;
    }
    sysml_state_alarmOn = sysml_lazy_1;
    if (sysml_state_alarmOn) {
        if (sysml_state_prevAlarmOn) {
            sysml_state_alarmPhaseMs = (sysml_state_alarmPhaseMs + io_7);
        }
        else {
            sysml_state_alarmPhaseMs = 0;
        }
    }
    else {
        sysml_state_alarmPhaseMs = sysml_state_alarmPhaseMs;
    }
    sysml_state_prevAlarmOn = sysml_state_alarmOn;
    sysml_state_lastReasonAny = sysml_state_alarmOn;
    if (sysml_state_alarmOn) {
        sysml_state_reportEveryAccumMs = (sysml_state_reportEveryAccumMs + io_7);
        if ((sysml_state_reportEveryAccumMs >= 1000)) {
            sysml_state_reportEveryAccumMs = (sysml_state_reportEveryAccumMs - 1000);
            sysml_state_reportGateHoldMs = 200;
        }
        else {
            sysml_state_reportGateHoldMs = sysml_state_reportGateHoldMs;
        }
    }
    else {
        sysml_state_reportEveryAccumMs = 0;
        sysml_state_reportGateHoldMs = 0;
    }
    if ((sysml_state_reportGateHoldMs > 0)) {
        sysml_state_reportGateHoldMs = (sysml_state_reportGateHoldMs - io_7);
    }
    else {
        sysml_state_reportGateHoldMs = sysml_state_reportGateHoldMs;
    }
    int sysml_value_0 = 0;
    if (sysml_state_displayIsTemp) {
        if (sysml_state_dhtValidLatched) {
            if (sysml_state_tempLatched < ((double)((int)sysml_state_tempLatched))) {
                sysml_conditional_5 = 1;
            }
            else {
                sysml_conditional_5 = 0;
            }
            sysml_conditional_4 = ((int)sysml_state_tempLatched) - sysml_conditional_5;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        if (io_4) {
            if (sysml_state_distLatched < ((double)((int)sysml_state_distLatched))) {
                sysml_conditional_7 = 1;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_conditional_6 = ((int)sysml_state_distLatched) - sysml_conditional_7;
        }
        else {
            sysml_conditional_6 = 0;
        }
        sysml_conditional_3 = sysml_conditional_6;
    }
    sysml_value_0 = sysml_conditional_3;
    int sysml_value_1 = 0;
    if (sysml_state_alarmOn) {
        if (sysml_state_alarmPhaseMs < 500) {
            sysml_conditional_9 = 1000;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_1 = sysml_conditional_8;
    bool sysml_value_2 = 0;
    if (sysml_state_alarmOn) {
        if (sysml_state_reportGateHoldMs > 0) {
            sysml_conditional_11 = true;
        }
        else {
            sysml_conditional_11 = false;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_2 = sysml_conditional_10;
    bool sysml_value_3 = 0;
    if (sysml_state_dhtValidLatched) {
        if (sysml_state_tempLatched > 32.0) {
            sysml_conditional_13 = true;
        }
        else {
            sysml_conditional_13 = false;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_3 = sysml_conditional_12;
    bool sysml_value_4 = 0;
    if (io_4) {
        if (sysml_state_distLatched < 15.0) {
            sysml_conditional_15 = true;
        }
        else {
            sysml_conditional_15 = false;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_4 = sysml_conditional_14;
    *io_5 = sysml_value_0;
    *io_6 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
    *serial_0_value_1 = sysml_value_4;
}