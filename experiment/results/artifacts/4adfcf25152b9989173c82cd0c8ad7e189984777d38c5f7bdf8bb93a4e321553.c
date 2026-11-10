#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeIsAuto = true;
/* Global Variable define of  */
bool sysml_state_manualTargetIsFan = true;
/* Global Variable define of  */
bool sysml_state_previousD2Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousD3Pressed = false;
/* Global Variable define of  */
double sysml_state_fanPwmManual = 0.0;
/* Global Variable define of  */
bool sysml_state_ledOnManual = false;
/* Global Variable define of  */
bool sysml_state_pumpOnManual = false;
/* Global Variable define of  */
int sysml_state_tmItemIndex = 0;
/* Global Variable define of  */
int sysml_state_tmElapsedMs = 0;
/* Global Variable define of  */
double sysml_state_lastTempValid = 0.0;
/* Global Variable define of  */
double sysml_state_lastHumValid = 0.0;
/* Global Variable define of  */
double sysml_state_lastIo3 = 0.0;
/* Global Variable define of  */
double sysml_state_prevSoilHumValid = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeIsAuto = true;
    sysml_state_manualTargetIsFan = true;
    sysml_state_previousD2Pressed = false;
    sysml_state_previousD3Pressed = false;
    sysml_state_fanPwmManual = 0.0;
    sysml_state_ledOnManual = false;
    sysml_state_pumpOnManual = false;
    sysml_state_tmItemIndex = 0;
    sysml_state_tmElapsedMs = 0;
    sysml_state_lastTempValid = 0.0;
    sysml_state_lastHumValid = 0.0;
    sysml_state_lastIo3 = 0.0;
    sysml_state_prevSoilHumValid = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, double io_4, double io_5, bool io_11, bool io_12, double* io_6, bool* io_7, bool* io_8, int* io_9, int* io_10, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_1_trigger, double* serial_1_value_0, double* serial_1_value_1, double* serial_1_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    double sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_conditional_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_conditional_27 = 0;
    bool sysml_conditional_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_conditional_30 = 0;
    bool sysml_lazy_31 = 0;
    bool sysml_lazy_32 = 0;
    int sysml_conditional_33 = 0;
    int sysml_conditional_34 = 0;
    int sysml_conditional_35 = 0;
    int sysml_conditional_36 = 0;
    int sysml_conditional_37 = 0;
    int sysml_conditional_38 = 0;
    bool sysml_lazy_39 = 0;
    bool sysml_lazy_40 = 0;
    bool sysml_lazy_41 = 0;
    bool sysml_lazy_42 = 0;
    double sysml_conditional_43 = 0;
    if (io_2) {
        sysml_state_lastTempValid = io_0;
        sysml_state_lastHumValid = io_1;
    }
    if (true) {
        sysml_state_lastIo3 = io_3;
        sysml_state_prevSoilHumValid = io_4;
    }
    sysml_lazy_0 = io_11;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousD2Pressed;
    }
    if (sysml_lazy_0) {
        sysml_state_modeIsAuto = (!sysml_state_modeIsAuto);
    }
    sysml_lazy_1 = io_12;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousD3Pressed;
    }
    if (sysml_lazy_1) {
        sysml_state_manualTargetIsFan = (!sysml_state_manualTargetIsFan);
    }
    sysml_state_previousD2Pressed = io_11;
    sysml_state_previousD3Pressed = io_12;
    if (sysml_state_manualTargetIsFan) {
        if (io_5 < 0.0) {
            sysml_conditional_3 = 0.0;
        }
        else {
            if (io_5 > 1023.0) {
                sysml_conditional_4 = 1023.0;
            }
            else {
                sysml_conditional_4 = io_5;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    else {
        sysml_conditional_2 = 0.0;
    }
    sysml_state_fanPwmManual = sysml_conditional_2;
    if (sysml_state_manualTargetIsFan) {
        sysml_conditional_5 = false;
    }
    else {
        if (io_5 >= 512.0) {
            sysml_conditional_6 = true;
        }
        else {
            sysml_conditional_6 = false;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_state_ledOnManual = sysml_conditional_5;
    if (sysml_state_manualTargetIsFan) {
        sysml_conditional_7 = false;
    }
    else {
        if (io_5 >= 256.0) {
            sysml_conditional_8 = true;
        }
        else {
            sysml_conditional_8 = false;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    sysml_state_pumpOnManual = sysml_conditional_7;
    sysml_lazy_9 = !sysml_state_modeIsAuto;
    if (sysml_lazy_9) {
        sysml_lazy_9 = true;
    }
    if (sysml_lazy_9) {
        sysml_state_tmItemIndex = sysml_state_tmItemIndex;
    }
    if (true) {
        sysml_state_tmElapsedMs = (sysml_state_tmElapsedMs + 10);
        if (sysml_state_tmElapsedMs >= 2000) {
            if (sysml_state_tmItemIndex == 2) {
                sysml_conditional_11 = 0;
            }
            else {
                sysml_conditional_11 = sysml_state_tmItemIndex + 1;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        else {
            sysml_conditional_10 = sysml_state_tmItemIndex;
        }
        sysml_state_tmItemIndex = sysml_conditional_10;
        if (sysml_state_tmElapsedMs >= 2000) {
            sysml_conditional_12 = 0;
        }
        else {
            sysml_conditional_12 = sysml_state_tmElapsedMs;
        }
        sysml_state_tmElapsedMs = sysml_conditional_12;
    }
    if (true) {
        sysml_state_tmItemIndex = sysml_state_tmItemIndex;
    }
    double sysml_value_0 = 0;
    sysml_lazy_13 = sysml_state_modeIsAuto;
    if (sysml_lazy_13) {
        sysml_lazy_13 = io_2;
    }
    if (sysml_lazy_13) {
        if (sysml_state_lastTempValid >= 30.0) {
            if (sysml_state_lastTempValid >= 40.0) {
                sysml_conditional_16 = 1023.0;
            }
            else {
                sysml_conditional_16 = ((double)((sysml_state_lastTempValid - 30.0) * 1023.0)) / 10.0;
            }
            sysml_conditional_15 = sysml_conditional_16;
        }
        else {
            sysml_conditional_15 = 0.0;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    else {
        sysml_lazy_17 = !sysml_state_modeIsAuto;
        if (sysml_lazy_17) {
            sysml_lazy_17 = sysml_state_manualTargetIsFan;
        }
        if (sysml_lazy_17) {
            if (sysml_state_fanPwmManual < 0.0) {
                sysml_conditional_19 = 0.0;
            }
            else {
                if (sysml_state_fanPwmManual > 1023.0) {
                    sysml_conditional_20 = 1023.0;
                }
                else {
                    sysml_conditional_20 = sysml_state_fanPwmManual;
                }
                sysml_conditional_19 = sysml_conditional_20;
            }
            sysml_conditional_18 = sysml_conditional_19;
        }
        else {
            sysml_conditional_18 = 0.0;
        }
        sysml_conditional_14 = sysml_conditional_18;
    }
    sysml_value_0 = sysml_conditional_14;
    bool sysml_value_1 = 0;
    sysml_lazy_21 = sysml_state_modeIsAuto;
    if (sysml_lazy_21) {
        sysml_lazy_21 = io_2;
    }
    if (sysml_lazy_21) {
        if (sysml_state_lastIo3 >= 600.0) {
            sysml_conditional_23 = true;
        }
        else {
            sysml_conditional_23 = false;
        }
        sysml_conditional_22 = sysml_conditional_23;
    }
    else {
        sysml_lazy_24 = !sysml_state_modeIsAuto;
        if (sysml_lazy_24) {
            sysml_lazy_24 = !sysml_state_manualTargetIsFan;
        }
        if (sysml_lazy_24) {
            sysml_conditional_25 = sysml_state_ledOnManual;
        }
        else {
            sysml_conditional_25 = false;
        }
        sysml_conditional_22 = sysml_conditional_25;
    }
    sysml_value_1 = sysml_conditional_22;
    bool sysml_value_2 = 0;
    sysml_lazy_26 = sysml_state_modeIsAuto;
    if (sysml_lazy_26) {
        sysml_lazy_26 = io_2;
    }
    if (sysml_lazy_26) {
        if (sysml_state_prevSoilHumValid <= 400.0) {
            sysml_conditional_28 = true;
        }
        else {
            sysml_conditional_28 = false;
        }
        sysml_conditional_27 = sysml_conditional_28;
    }
    else {
        sysml_lazy_29 = !sysml_state_modeIsAuto;
        if (sysml_lazy_29) {
            sysml_lazy_29 = !sysml_state_manualTargetIsFan;
        }
        if (sysml_lazy_29) {
            sysml_conditional_30 = sysml_state_pumpOnManual;
        }
        else {
            sysml_conditional_30 = false;
        }
        sysml_conditional_27 = sysml_conditional_30;
    }
    sysml_value_2 = sysml_conditional_27;
    int sysml_value_3 = 0;
    sysml_lazy_31 = !io_2;
    if (!sysml_lazy_31) {
        sysml_lazy_31 = io_3 < 0.0;
    }
    sysml_lazy_32 = sysml_lazy_31;
    if (!sysml_lazy_32) {
        sysml_lazy_32 = io_4 < 0.0;
    }
    if (sysml_lazy_32) {
        sysml_conditional_33 = 1000;
    }
    else {
        sysml_conditional_33 = 0;
    }
    sysml_value_3 = sysml_conditional_33;
    int sysml_value_4 = 0;
    if (sysml_state_tmItemIndex == 0) {
        if (sysml_state_lastTempValid < ((double)((int)sysml_state_lastTempValid))) {
            sysml_conditional_35 = 1;
        }
        else {
            sysml_conditional_35 = 0;
        }
        sysml_conditional_34 = ((int)sysml_state_lastTempValid) - sysml_conditional_35;
    }
    else {
        if (sysml_state_tmItemIndex == 1) {
            if (sysml_state_lastIo3 < ((double)((int)sysml_state_lastIo3))) {
                sysml_conditional_37 = 1;
            }
            else {
                sysml_conditional_37 = 0;
            }
            sysml_conditional_36 = ((int)sysml_state_lastIo3) - sysml_conditional_37;
        }
        else {
            if (sysml_state_prevSoilHumValid < ((double)((int)sysml_state_prevSoilHumValid))) {
                sysml_conditional_38 = 1;
            }
            else {
                sysml_conditional_38 = 0;
            }
            sysml_conditional_36 = ((int)sysml_state_prevSoilHumValid) - sysml_conditional_38;
        }
        sysml_conditional_34 = sysml_conditional_36;
    }
    sysml_value_4 = sysml_conditional_34;
    bool sysml_value_5 = 0;
    sysml_lazy_39 = !io_2;
    if (!sysml_lazy_39) {
        sysml_lazy_39 = io_3 < 0.0;
    }
    sysml_lazy_40 = sysml_lazy_39;
    if (!sysml_lazy_40) {
        sysml_lazy_40 = io_4 < 0.0;
    }
    sysml_value_5 = sysml_lazy_40;
    bool sysml_value_6 = 0;
    sysml_lazy_41 = !io_2;
    if (!sysml_lazy_41) {
        sysml_lazy_41 = io_3 < 0.0;
    }
    sysml_lazy_42 = sysml_lazy_41;
    if (!sysml_lazy_42) {
        sysml_lazy_42 = io_4 < 0.0;
    }
    sysml_value_6 = sysml_lazy_42;
    bool sysml_value_7 = 0;
    sysml_value_7 = io_2;
    double sysml_value_8 = 0;
    if (io_2) {
        sysml_conditional_43 = sysml_state_lastTempValid;
    }
    else {
        sysml_conditional_43 = sysml_state_lastTempValid;
    }
    sysml_value_8 = sysml_conditional_43;
    double sysml_value_9 = 0;
    sysml_value_9 = sysml_state_lastIo3;
    double sysml_value_10 = 0;
    sysml_value_10 = sysml_state_prevSoilHumValid;
    *io_6 = sysml_value_0;
    *io_7 = sysml_value_1;
    *io_8 = sysml_value_2;
    *io_9 = sysml_value_3;
    *io_10 = sysml_value_4;
    *serial_0_trigger = sysml_value_5;
    *serial_0_value_0 = sysml_value_6;
    *serial_1_trigger = sysml_value_7;
    *serial_1_value_0 = sysml_value_8;
    *serial_1_value_1 = sysml_value_9;
    *serial_1_value_2 = sysml_value_10;
}