#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_phaseAccumMs = 0;
/* Global Variable define of  */
int sysml_state_countdown = 99;
/* Global Variable define of  */
int sysml_state_colorIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phaseAccumMs = 0;
    sysml_state_countdown = 99;
    sysml_state_colorIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_10, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, int* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    double sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    double sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    double sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    double sysml_conditional_23 = 0;
    sysml_state_phaseAccumMs = (sysml_state_phaseAccumMs + io_10);
    if ((sysml_state_phaseAccumMs >= 2000)) {
        sysml_state_phaseAccumMs = (sysml_state_phaseAccumMs - 2000);
        if (sysml_state_countdown == 0) {
            sysml_conditional_0 = 99;
        }
        else {
            sysml_conditional_0 = sysml_state_countdown - 1;
        }
        sysml_state_countdown = sysml_conditional_0;
        if (sysml_state_countdown == 0) {
            sysml_conditional_1 = 0;
        }
        else {
            if (sysml_state_colorIndex == 5) {
                sysml_conditional_2 = 0;
            }
            else {
                sysml_conditional_2 = sysml_state_colorIndex + 1;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_colorIndex = sysml_conditional_1;
        if ((sysml_state_countdown == 0)) {
            sysml_state_countdown = 99;
        }
    }
    double sysml_value_0 = 0;
    sysml_lazy_3 = sysml_state_colorIndex == 0;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_colorIndex == 3;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = 255.0;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_0 = sysml_conditional_4;
    double sysml_value_1 = 0;
    sysml_lazy_5 = sysml_state_colorIndex == 1;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_colorIndex == 5;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = 255.0;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_1 = sysml_conditional_7;
    double sysml_value_2 = 0;
    sysml_lazy_8 = sysml_state_colorIndex == 2;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_colorIndex == 3;
    }
    if (sysml_lazy_8) {
        sysml_conditional_9 = 255.0;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_2 = sysml_conditional_9;
    double sysml_value_3 = 0;
    sysml_lazy_10 = sysml_state_colorIndex == 1;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_colorIndex == 4;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = 255.0;
    }
    else {
        sysml_conditional_11 = 0.0;
    }
    sysml_value_3 = sysml_conditional_11;
    double sysml_value_4 = 0;
    sysml_lazy_12 = sysml_state_colorIndex == 4;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_colorIndex == 1;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_colorIndex == 5;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = 255.0;
    }
    else {
        sysml_conditional_14 = 0.0;
    }
    sysml_value_4 = sysml_conditional_14;
    double sysml_value_5 = 0;
    sysml_lazy_15 = sysml_state_colorIndex == 2;
    if (!sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_colorIndex == 5;
    }
    if (sysml_lazy_15) {
        sysml_conditional_16 = 255.0;
    }
    else {
        sysml_conditional_16 = 0.0;
    }
    sysml_value_5 = sysml_conditional_16;
    double sysml_value_6 = 0;
    if (sysml_state_colorIndex == 2) {
        sysml_conditional_17 = 255.0;
    }
    else {
        sysml_conditional_17 = 0.0;
    }
    sysml_value_6 = sysml_conditional_17;
    double sysml_value_7 = 0;
    sysml_lazy_18 = sysml_state_colorIndex == 5;
    if (!sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_colorIndex == 1;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (!sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_colorIndex == 4;
    }
    if (sysml_lazy_19) {
        sysml_conditional_20 = 255.0;
    }
    else {
        sysml_conditional_20 = 0.0;
    }
    sysml_value_7 = sysml_conditional_20;
    double sysml_value_8 = 0;
    sysml_lazy_21 = sysml_state_colorIndex == 2;
    if (!sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_colorIndex == 3;
    }
    sysml_lazy_22 = sysml_lazy_21;
    if (!sysml_lazy_22) {
        sysml_lazy_22 = sysml_state_colorIndex == 5;
    }
    if (sysml_lazy_22) {
        sysml_conditional_23 = 255.0;
    }
    else {
        sysml_conditional_23 = 0.0;
    }
    sysml_value_8 = sysml_conditional_23;
    int sysml_value_9 = 0;
    sysml_value_9 = sysml_state_countdown;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_8 = sysml_value_8;
    *io_9 = sysml_value_9;
}