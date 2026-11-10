#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_ledAccumMs = 0;
/* Global Variable define of  */
int sysml_state_rgbAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Global Variable define of  */
int sysml_state_rgbIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledAccumMs = 0;
    sysml_state_rgbAccumMs = 0;
    sysml_state_ledOn = false;
    sysml_state_rgbIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_10, bool* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    double sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    double sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    double sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    double sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    double sysml_conditional_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_lazy_31 = 0;
    double sysml_conditional_32 = 0;
    bool sysml_lazy_33 = 0;
    bool sysml_lazy_34 = 0;
    bool sysml_lazy_35 = 0;
    double sysml_conditional_36 = 0;
    sysml_state_ledAccumMs = (sysml_state_ledAccumMs + io_10);
    if ((sysml_state_ledAccumMs >= 2000)) {
        sysml_state_ledAccumMs = (sysml_state_ledAccumMs - 2000);
        sysml_state_ledOn = (!sysml_state_ledOn);
    }
    sysml_state_rgbAccumMs = (sysml_state_rgbAccumMs + io_10);
    if ((sysml_state_rgbAccumMs >= 2000)) {
        sysml_state_rgbAccumMs = (sysml_state_rgbAccumMs - 2000);
        if (sysml_state_rgbIndex == 6) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_rgbIndex + 1;
        }
        sysml_state_rgbIndex = sysml_conditional_0;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_ledOn;
    double sysml_value_1 = 0;
    sysml_lazy_1 = sysml_state_rgbIndex == 0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_rgbIndex == 5;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = 255.0;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_1 = sysml_conditional_4;
    double sysml_value_2 = 0;
    sysml_lazy_5 = sysml_state_rgbIndex == 1;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_rgbIndex == 4;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = 255.0;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_2 = sysml_conditional_8;
    double sysml_value_3 = 0;
    sysml_lazy_9 = sysml_state_rgbIndex == 2;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_rgbIndex == 4;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_rgbIndex == 5;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_11) {
        sysml_conditional_12 = 255.0;
    }
    else {
        sysml_conditional_12 = 0.0;
    }
    sysml_value_3 = sysml_conditional_12;
    double sysml_value_4 = 0;
    sysml_lazy_13 = sysml_state_rgbIndex == 0;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_14 = sysml_lazy_13;
    if (!sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_rgbIndex == 5;
    }
    sysml_lazy_15 = sysml_lazy_14;
    if (!sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_15) {
        sysml_conditional_16 = 255.0;
    }
    else {
        sysml_conditional_16 = 0.0;
    }
    sysml_value_4 = sysml_conditional_16;
    double sysml_value_5 = 0;
    sysml_lazy_17 = sysml_state_rgbIndex == 1;
    if (!sysml_lazy_17) {
        sysml_lazy_17 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_18 = sysml_lazy_17;
    if (!sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_rgbIndex == 4;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (!sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_19) {
        sysml_conditional_20 = 255.0;
    }
    else {
        sysml_conditional_20 = 0.0;
    }
    sysml_value_5 = sysml_conditional_20;
    double sysml_value_6 = 0;
    sysml_lazy_21 = sysml_state_rgbIndex == 2;
    if (!sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_rgbIndex == 4;
    }
    sysml_lazy_22 = sysml_lazy_21;
    if (!sysml_lazy_22) {
        sysml_lazy_22 = sysml_state_rgbIndex == 5;
    }
    sysml_lazy_23 = sysml_lazy_22;
    if (!sysml_lazy_23) {
        sysml_lazy_23 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_23) {
        sysml_conditional_24 = 255.0;
    }
    else {
        sysml_conditional_24 = 0.0;
    }
    sysml_value_6 = sysml_conditional_24;
    double sysml_value_7 = 0;
    sysml_lazy_25 = sysml_state_rgbIndex == 0;
    if (!sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_26 = sysml_lazy_25;
    if (!sysml_lazy_26) {
        sysml_lazy_26 = sysml_state_rgbIndex == 5;
    }
    sysml_lazy_27 = sysml_lazy_26;
    if (!sysml_lazy_27) {
        sysml_lazy_27 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_27) {
        sysml_conditional_28 = 255.0;
    }
    else {
        sysml_conditional_28 = 0.0;
    }
    sysml_value_7 = sysml_conditional_28;
    double sysml_value_8 = 0;
    sysml_lazy_29 = sysml_state_rgbIndex == 1;
    if (!sysml_lazy_29) {
        sysml_lazy_29 = sysml_state_rgbIndex == 3;
    }
    sysml_lazy_30 = sysml_lazy_29;
    if (!sysml_lazy_30) {
        sysml_lazy_30 = sysml_state_rgbIndex == 4;
    }
    sysml_lazy_31 = sysml_lazy_30;
    if (!sysml_lazy_31) {
        sysml_lazy_31 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_31) {
        sysml_conditional_32 = 255.0;
    }
    else {
        sysml_conditional_32 = 0.0;
    }
    sysml_value_8 = sysml_conditional_32;
    double sysml_value_9 = 0;
    sysml_lazy_33 = sysml_state_rgbIndex == 2;
    if (!sysml_lazy_33) {
        sysml_lazy_33 = sysml_state_rgbIndex == 4;
    }
    sysml_lazy_34 = sysml_lazy_33;
    if (!sysml_lazy_34) {
        sysml_lazy_34 = sysml_state_rgbIndex == 5;
    }
    sysml_lazy_35 = sysml_lazy_34;
    if (!sysml_lazy_35) {
        sysml_lazy_35 = sysml_state_rgbIndex == 6;
    }
    if (sysml_lazy_35) {
        sysml_conditional_36 = 255.0;
    }
    else {
        sysml_conditional_36 = 0.0;
    }
    sysml_value_9 = sysml_conditional_36;
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