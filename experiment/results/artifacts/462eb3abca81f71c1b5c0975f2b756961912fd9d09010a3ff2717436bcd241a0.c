#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeManual = true;
/* Global Variable define of  */
bool sysml_state_prevBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2Pressed = false;
/* Global Variable define of  */
int sysml_state_debounceBtn1Remaining = 0;
/* Global Variable define of  */
int sysml_state_debounceBtn2Remaining = 0;
/* Global Variable define of  */
bool sysml_state_stableBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_stableBtn2Pressed = false;
/* Global Variable define of  */
int sysml_state_autoPhase = 0;
/* Global Variable define of  */
int sysml_state_transitionElapsed = 0;
/* Global Variable define of  */
bool sysml_state_toMax = true;
/* Global Variable define of  */
bool sysml_state_prevStableBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_prevStableBtn2Pressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeManual = true;
    sysml_state_prevBtn1Pressed = false;
    sysml_state_prevBtn2Pressed = false;
    sysml_state_debounceBtn1Remaining = 0;
    sysml_state_debounceBtn2Remaining = 0;
    sysml_state_stableBtn1Pressed = false;
    sysml_state_stableBtn2Pressed = false;
    sysml_state_autoPhase = 0;
    sysml_state_transitionElapsed = 0;
    sysml_state_toMax = true;
    sysml_state_prevStableBtn1Pressed = false;
    sysml_state_prevStableBtn2Pressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, double io_3, double io_4, double io_5, int io_6, double* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    double sysml_conditional_18 = 0;
    double sysml_arithmetic_19 = 0;
    double sysml_conditional_20 = 0;
    double sysml_arithmetic_21 = 0;
    double sysml_arithmetic_22 = 0;
    double sysml_conditional_23 = 0;
    double sysml_conditional_24 = 0;
    double sysml_conditional_25 = 0;
    double sysml_conditional_26 = 0;
    double sysml_arithmetic_27 = 0;
    double sysml_arithmetic_28 = 0;
    double sysml_conditional_29 = 0;
    double sysml_arithmetic_30 = 0;
    double sysml_arithmetic_31 = 0;
    if ((sysml_state_debounceBtn1Remaining > 0)) {
        sysml_state_debounceBtn1Remaining = (sysml_state_debounceBtn1Remaining - io_6);
        if (((sysml_state_debounceBtn1Remaining - io_6) <= 0)) {
            sysml_state_stableBtn1Pressed = io_1;
        }
    }
    sysml_lazy_0 = sysml_state_debounceBtn1Remaining <= 0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevBtn1Pressed;
    }
    if (sysml_lazy_1) {
        sysml_state_debounceBtn1Remaining = 150;
    }
    if ((sysml_state_debounceBtn2Remaining > 0)) {
        sysml_state_debounceBtn2Remaining = (sysml_state_debounceBtn2Remaining - io_6);
        if (((sysml_state_debounceBtn2Remaining - io_6) <= 0)) {
            sysml_state_stableBtn2Pressed = io_2;
        }
    }
    sysml_lazy_2 = sysml_state_debounceBtn2Remaining <= 0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_2;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevBtn2Pressed;
    }
    if (sysml_lazy_3) {
        sysml_state_debounceBtn2Remaining = 150;
    }
    sysml_state_prevBtn1Pressed = io_1;
    sysml_state_prevBtn2Pressed = io_2;
    sysml_lazy_4 = sysml_state_modeManual;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_stableBtn1Pressed;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_prevStableBtn1Pressed;
    }
    if (sysml_lazy_5) {
        sysml_state_modeManual = false;
        sysml_state_autoPhase = 0;
        sysml_state_transitionElapsed = 0;
        sysml_state_toMax = true;
    }
    sysml_lazy_6 = !sysml_state_modeManual;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_stableBtn2Pressed;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_prevStableBtn2Pressed;
    }
    if (sysml_lazy_7) {
        sysml_state_modeManual = true;
        sysml_state_transitionElapsed = 0;
    }
    if ((!sysml_state_modeManual)) {
        sysml_state_transitionElapsed = (sysml_state_transitionElapsed + io_6);
        if ((sysml_state_transitionElapsed >= 2000)) {
            sysml_state_transitionElapsed = (sysml_state_transitionElapsed - 2000);
            sysml_state_toMax = (!sysml_state_toMax);
            sysml_state_autoPhase = (sysml_state_autoPhase + 1);
        }
    }
    sysml_state_prevStableBtn1Pressed = sysml_state_stableBtn1Pressed;
    sysml_state_prevStableBtn2Pressed = sysml_state_stableBtn2Pressed;
    double sysml_value_0 = 0;
    if (sysml_state_modeManual) {
        if (io_3 <= 0.0) {
            sysml_conditional_9 = 0.0;
        }
        else {
            if (io_3 >= 1023.0) {
                sysml_conditional_10 = 255.0;
            }
            else {
                sysml_conditional_10 = ((double)(io_3 * 255.0)) / 1023.0;
            }
            sysml_conditional_9 = sysml_conditional_10;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    else {
        if (sysml_state_transitionElapsed >= 2000) {
            if (sysml_state_toMax) {
                if (io_5 >= 1023.0) {
                    sysml_conditional_13 = 255.0;
                }
                else {
                    sysml_conditional_13 = ((double)(io_5 * 255.0)) / 1023.0;
                }
                sysml_conditional_12 = sysml_conditional_13;
            }
            else {
                if (io_4 <= 0.0) {
                    sysml_conditional_14 = 0.0;
                }
                else {
                    if (io_4 >= 1023.0) {
                        sysml_conditional_15 = 255.0;
                    }
                    else {
                        sysml_conditional_15 = ((double)(io_4 * 255.0)) / 1023.0;
                    }
                    sysml_conditional_14 = sysml_conditional_15;
                }
                sysml_conditional_12 = sysml_conditional_14;
            }
            sysml_conditional_11 = sysml_conditional_12;
        }
        else {
            if (sysml_state_toMax) {
                if (io_4 <= 0.0) {
                    if (io_5 >= 1023.0) {
                        sysml_conditional_18 = ((double)(sysml_state_transitionElapsed * 255.0)) / 2000.0;
                    }
                    else {
                        sysml_arithmetic_19 = ((double)sysml_state_transitionElapsed) / 2000.0;
                        sysml_conditional_18 = (((double)(io_5 * 255.0)) / 1023.0) * sysml_arithmetic_19;
                    }
                    sysml_conditional_17 = sysml_conditional_18;
                }
                else {
                    if (io_5 >= 1023.0) {
                        sysml_arithmetic_21 = ((double)sysml_state_transitionElapsed) / 2000.0;
                        sysml_conditional_20 = (((double)(io_4 * 255.0)) / 1023.0) + ((255.0 - (((double)(io_4 * 255.0)) / 1023.0)) * sysml_arithmetic_21);
                    }
                    else {
                        sysml_arithmetic_22 = ((double)sysml_state_transitionElapsed) / 2000.0;
                        sysml_conditional_20 = (((double)(io_4 * 255.0)) / 1023.0) + (((((double)(io_5 * 255.0)) / 1023.0) - (((double)(io_4 * 255.0)) / 1023.0)) * sysml_arithmetic_22);
                    }
                    sysml_conditional_17 = sysml_conditional_20;
                }
                sysml_conditional_16 = sysml_conditional_17;
            }
            else {
                if (io_5 <= 0.0) {
                    if (io_4 >= 1023.0) {
                        sysml_conditional_24 = 255.0;
                    }
                    else {
                        sysml_conditional_24 = ((double)(io_4 * 255.0)) / 1023.0;
                    }
                    sysml_conditional_23 = sysml_conditional_24;
                }
                else {
                    if (io_4 <= 0.0) {
                        if (io_5 >= 1023.0) {
                            sysml_arithmetic_27 = ((double)sysml_state_transitionElapsed) / 2000.0;
                            sysml_conditional_26 = 255.0 + (((((double)(io_4 * 255.0)) / 1023.0) - 255.0) * sysml_arithmetic_27);
                        }
                        else {
                            sysml_arithmetic_28 = ((double)sysml_state_transitionElapsed) / 2000.0;
                            sysml_conditional_26 = (((double)(io_5 * 255.0)) / 1023.0) + (((((double)(io_4 * 255.0)) / 1023.0) - (((double)(io_5 * 255.0)) / 1023.0)) * sysml_arithmetic_28);
                        }
                        sysml_conditional_25 = sysml_conditional_26;
                    }
                    else {
                        if (io_5 >= 1023.0) {
                            sysml_arithmetic_30 = ((double)sysml_state_transitionElapsed) / 2000.0;
                            sysml_conditional_29 = 255.0 + (((((double)(io_4 * 255.0)) / 1023.0) - 255.0) * sysml_arithmetic_30);
                        }
                        else {
                            sysml_arithmetic_31 = ((double)sysml_state_transitionElapsed) / 2000.0;
                            sysml_conditional_29 = (((double)(io_5 * 255.0)) / 1023.0) + (((((double)(io_4 * 255.0)) / 1023.0) - (((double)(io_5 * 255.0)) / 1023.0)) * sysml_arithmetic_31);
                        }
                        sysml_conditional_25 = sysml_conditional_29;
                    }
                    sysml_conditional_23 = sysml_conditional_25;
                }
                sysml_conditional_16 = sysml_conditional_23;
            }
            sysml_conditional_11 = sysml_conditional_16;
        }
        sysml_conditional_8 = sysml_conditional_11;
    }
    sysml_value_0 = sysml_conditional_8;
    *io_0 = sysml_value_0;
}