#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeAuto = true;
/* Global Variable define of  */
bool sysml_state_prevButton = false;
/* Global Variable define of  */
int sysml_state_manualForcedUntil = 0;
/* Global Variable define of  */
int sysml_state_manualRemainingMs = 0;
/* Global Variable define of  */
int sysml_state_displayPhase = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeAuto = true;
    sysml_state_prevButton = false;
    sysml_state_manualForcedUntil = 0;
    sysml_state_manualRemainingMs = 0;
    sysml_state_displayPhase = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_7, bool* io_3, bool* io_4, int* io_5, int* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    sysml_lazy_0 = io_7;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevButton;
    }
    if (sysml_lazy_0) {
        if (sysml_state_modeAuto) {
            sysml_state_modeAuto = false;
        }
        else {
            sysml_state_modeAuto = true;
        }
        if ((!sysml_state_modeAuto)) {
            sysml_state_manualRemainingMs = 5000;
            sysml_state_manualForcedUntil = 1;
        }
    }
    sysml_state_prevButton = io_7;
    sysml_lazy_1 = sysml_state_manualForcedUntil != 0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_manualRemainingMs > 0;
    }
    if (sysml_lazy_1) {
        sysml_state_manualRemainingMs = sysml_state_manualRemainingMs;
        sysml_state_manualForcedUntil = sysml_state_manualForcedUntil;
    }
    else {
        sysml_state_manualForcedUntil = 0;
    }
    if (sysml_state_modeAuto) {
        sysml_state_displayPhase = (sysml_state_displayPhase + 1);
    }
    else {
        sysml_state_displayPhase = (sysml_state_displayPhase + 1);
    }
    bool sysml_value_0 = 0;
    if (sysml_state_modeAuto) {
        sysml_lazy_3 = io_0 < 512.0;
        if (sysml_lazy_3) {
            sysml_lazy_3 = io_1 >= 512.0;
        }
        sysml_lazy_4 = sysml_lazy_3;
        if (sysml_lazy_4) {
            sysml_lazy_4 = io_2 >= 512.0;
        }
        sysml_conditional_2 = sysml_lazy_4;
    }
    else {
        sysml_lazy_5 = sysml_state_manualForcedUntil != 0;
        if (sysml_lazy_5) {
            sysml_lazy_5 = sysml_state_manualRemainingMs > 0;
        }
        sysml_conditional_2 = sysml_lazy_5;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    sysml_lazy_6 = sysml_state_modeAuto;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_2 < 512.0;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_1 = sysml_conditional_7;
    int sysml_value_2 = 0;
    sysml_lazy_8 = sysml_state_modeAuto;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_2 < 512.0;
    }
    if (sysml_lazy_8) {
        sysml_conditional_9 = 1000;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_2 = sysml_conditional_9;
    int sysml_value_3 = 0;
    if (sysml_state_displayPhase == 0) {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_11 = 1;
        }
        else {
            sysml_conditional_11 = 0;
        }
        sysml_conditional_10 = ((int)io_0) - sysml_conditional_11;
    }
    else {
        if (sysml_state_displayPhase == 1) {
            if (io_2 < ((double)((int)io_2))) {
                sysml_conditional_13 = 1;
            }
            else {
                sysml_conditional_13 = 0;
            }
            sysml_conditional_12 = ((int)io_2) - sysml_conditional_13;
        }
        else {
            if (sysml_state_modeAuto) {
                sysml_conditional_14 = 0;
            }
            else {
                sysml_conditional_14 = 1;
            }
            sysml_conditional_12 = sysml_conditional_14;
        }
        sysml_conditional_10 = sysml_conditional_12;
    }
    sysml_value_3 = sysml_conditional_10;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
}