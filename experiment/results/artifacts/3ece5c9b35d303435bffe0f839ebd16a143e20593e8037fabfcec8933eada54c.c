#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeAuto = true;
/* Global Variable define of  */
bool sysml_state_manualTargetFan = true;
/* Global Variable define of  */
bool sysml_state_prevModeBtn = false;
/* Global Variable define of  */
bool sysml_state_prevTargetBtn = false;
/* Global Variable define of  */
bool sysml_state_manualFanActive = false;
/* Global Variable define of  */
bool sysml_state_manualLightActive = false;
/* Global Variable define of  */
bool sysml_state_manualPumpActive = false;
/* Global Variable define of  */
bool sysml_state_faultAlarmed = false;
/* Global Variable define of  */
bool sysml_state_prevFault = false;
/* Global Variable define of  */
int sysml_state_displayIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeAuto = true;
    sysml_state_manualTargetFan = true;
    sysml_state_prevModeBtn = false;
    sysml_state_prevTargetBtn = false;
    sysml_state_manualFanActive = false;
    sysml_state_manualLightActive = false;
    sysml_state_manualPumpActive = false;
    sysml_state_faultAlarmed = false;
    sysml_state_prevFault = false;
    sysml_state_displayIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, double io_4, double io_9, double io_10, bool io_11, double* io_5, bool* io_6, bool* io_7, int* io_8, int* io_12, bool* serial_0_trigger, bool* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    int sysml_conditional_19 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevModeBtn;
    }
    if (sysml_lazy_0) {
        if (sysml_state_modeAuto) {
            sysml_conditional_1 = false;
        }
        else {
            sysml_conditional_1 = true;
        }
        sysml_state_modeAuto = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevTargetBtn;
    }
    if (sysml_lazy_2) {
        if (sysml_state_manualTargetFan) {
            sysml_conditional_3 = false;
        }
        else {
            sysml_conditional_3 = true;
        }
        sysml_state_manualTargetFan = sysml_conditional_3;
    }
    sysml_state_prevModeBtn = io_0;
    sysml_state_prevTargetBtn = io_1;
    if ((!sysml_state_modeAuto)) {
        sysml_state_manualFanActive = sysml_state_manualTargetFan;
        if (sysml_state_manualTargetFan) {
            sysml_conditional_4 = false;
        }
        else {
            sysml_conditional_4 = true;
        }
        sysml_state_manualLightActive = sysml_conditional_4;
        sysml_state_manualPumpActive = false;
    }
    else {
        sysml_state_manualFanActive = false;
        sysml_state_manualLightActive = false;
        sysml_state_manualPumpActive = false;
    }
    sysml_state_faultAlarmed = (!io_11);
    sysml_state_prevFault = sysml_state_faultAlarmed;
    sysml_state_displayIndex = (sysml_state_displayIndex + 1);
    double sysml_value_0 = 0;
    if (sysml_state_modeAuto) {
        if (io_9 >= 30.0) {
            sysml_conditional_6 = 255.0;
        }
        else {
            sysml_conditional_6 = 0.0;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    else {
        if (sysml_state_manualFanActive) {
            sysml_conditional_7 = io_2;
        }
        else {
            sysml_conditional_7 = 0.0;
        }
        sysml_conditional_5 = sysml_conditional_7;
    }
    sysml_value_0 = sysml_conditional_5;
    bool sysml_value_1 = 0;
    if (sysml_state_modeAuto) {
        if (io_3 <= 400.0) {
            sysml_conditional_9 = true;
        }
        else {
            sysml_conditional_9 = false;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    else {
        if (sysml_state_manualLightActive) {
            sysml_conditional_10 = io_2 > 0.0;
        }
        else {
            sysml_conditional_10 = false;
        }
        sysml_conditional_8 = sysml_conditional_10;
    }
    sysml_value_1 = sysml_conditional_8;
    bool sysml_value_2 = 0;
    if (sysml_state_modeAuto) {
        if (io_4 <= 400.0) {
            sysml_conditional_12 = true;
        }
        else {
            sysml_conditional_12 = false;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    else {
        if (sysml_state_manualPumpActive) {
            sysml_conditional_13 = io_2 > 0.0;
        }
        else {
            sysml_conditional_13 = false;
        }
        sysml_conditional_11 = sysml_conditional_13;
    }
    sysml_value_2 = sysml_conditional_11;
    int sysml_value_3 = 0;
    if (sysml_state_faultAlarmed) {
        sysml_conditional_14 = 2000;
    }
    else {
        sysml_conditional_14 = 0;
    }
    sysml_value_3 = sysml_conditional_14;
    int sysml_value_4 = 0;
    if (sysml_state_displayIndex == 0) {
        if (io_9 < ((double)((int)io_9))) {
            sysml_conditional_16 = 1;
        }
        else {
            sysml_conditional_16 = 0;
        }
        sysml_conditional_15 = ((int)io_9) - sysml_conditional_16;
    }
    else {
        if (sysml_state_displayIndex == 1) {
            if (io_3 < ((double)((int)io_3))) {
                sysml_conditional_18 = 1;
            }
            else {
                sysml_conditional_18 = 0;
            }
            sysml_conditional_17 = ((int)io_3) - sysml_conditional_18;
        }
        else {
            if (io_4 < ((double)((int)io_4))) {
                sysml_conditional_19 = 1;
            }
            else {
                sysml_conditional_19 = 0;
            }
            sysml_conditional_17 = ((int)io_4) - sysml_conditional_19;
        }
        sysml_conditional_15 = sysml_conditional_17;
    }
    sysml_value_4 = sysml_conditional_15;
    bool sysml_value_5 = 0;
    sysml_value_5 = sysml_state_faultAlarmed;
    bool sysml_value_6 = 0;
    sysml_value_6 = sysml_state_faultAlarmed;
    double sysml_value_7 = 0;
    sysml_value_7 = io_3;
    double sysml_value_8 = 0;
    sysml_value_8 = io_4;
    *io_5 = sysml_value_0;
    *io_6 = sysml_value_1;
    *io_7 = sysml_value_2;
    *io_8 = sysml_value_3;
    *io_12 = sysml_value_4;
    *serial_0_trigger = sysml_value_5;
    *serial_0_value_0 = sysml_value_6;
    *serial_0_value_1 = sysml_value_7;
    *serial_0_value_2 = sysml_value_8;
}