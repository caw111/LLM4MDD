#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_btn1Stable = false;
/* Global Variable define of  */
bool sysml_state_btn2Stable = false;
/* Global Variable define of  */
int sysml_state_debounceCount1 = 0;
/* Global Variable define of  */
int sysml_state_debounceCount2 = 0;
/* Global Variable define of  */
int sysml_state_colorIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_btn1Stable = false;
    sysml_state_btn2Stable = false;
    sysml_state_debounceCount1 = 0;
    sysml_state_debounceCount2 = 0;
    sysml_state_colorIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double* io_2, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    double sysml_conditional_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    double sysml_conditional_25 = 0;
    if (io_0) {
        if (sysml_state_debounceCount1 >= 2) {
            sysml_conditional_1 = true;
        }
        else {
            sysml_conditional_1 = sysml_state_btn1Stable;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_state_btn1Stable = sysml_conditional_0;
    if (io_1) {
        if (sysml_state_debounceCount2 >= 2) {
            sysml_conditional_3 = true;
        }
        else {
            sysml_conditional_3 = sysml_state_btn2Stable;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_state_btn2Stable = sysml_conditional_2;
    if (io_0) {
        if (sysml_state_debounceCount1 >= 2) {
            sysml_conditional_5 = 2;
        }
        else {
            sysml_conditional_5 = sysml_state_debounceCount1 + 1;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_state_debounceCount1 = sysml_conditional_4;
    if (io_1) {
        if (sysml_state_debounceCount2 >= 2) {
            sysml_conditional_7 = 2;
        }
        else {
            sysml_conditional_7 = sysml_state_debounceCount2 + 1;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_state_debounceCount2 = sysml_conditional_6;
    sysml_lazy_8 = sysml_state_btn1Stable;
    if (sysml_lazy_8) {
        sysml_lazy_8 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_8) {
        if (sysml_state_colorIndex == 8) {
            sysml_conditional_9 = 0;
        }
        else {
            sysml_conditional_9 = sysml_state_colorIndex + 1;
        }
        sysml_state_colorIndex = sysml_conditional_9;
    }
    sysml_lazy_10 = sysml_state_btn2Stable;
    if (sysml_lazy_10) {
        sysml_lazy_10 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_10) {
        if (sysml_state_colorIndex == 0) {
            sysml_conditional_11 = 8;
        }
        else {
            sysml_conditional_11 = sysml_state_colorIndex - 1;
        }
        sysml_state_colorIndex = sysml_conditional_11;
    }
    sysml_state_prevBtn1 = sysml_state_btn1Stable;
    sysml_state_prevBtn2 = sysml_state_btn2Stable;
    double sysml_value_0 = 0;
    sysml_lazy_12 = sysml_state_colorIndex == 1;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_colorIndex == 7;
    }
    sysml_lazy_14 = sysml_lazy_13;
    if (!sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_colorIndex == 6;
    }
    if (sysml_lazy_14) {
        sysml_conditional_15 = 255.0;
    }
    else {
        if (sysml_state_colorIndex == 5) {
            sysml_conditional_16 = 0.0;
        }
        else {
            if (sysml_state_colorIndex == 3) {
                sysml_conditional_17 = 0.0;
            }
            else {
                sysml_conditional_17 = 0.0;
            }
            sysml_conditional_16 = sysml_conditional_17;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    sysml_value_0 = sysml_conditional_15;
    double sysml_value_1 = 0;
    sysml_lazy_18 = sysml_state_colorIndex == 2;
    if (!sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (!sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_colorIndex == 5;
    }
    sysml_lazy_20 = sysml_lazy_19;
    if (!sysml_lazy_20) {
        sysml_lazy_20 = sysml_state_colorIndex == 7;
    }
    if (sysml_lazy_20) {
        sysml_conditional_21 = 255.0;
    }
    else {
        sysml_conditional_21 = 0.0;
    }
    sysml_value_1 = sysml_conditional_21;
    double sysml_value_2 = 0;
    sysml_lazy_22 = sysml_state_colorIndex == 3;
    if (!sysml_lazy_22) {
        sysml_lazy_22 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_23 = sysml_lazy_22;
    if (!sysml_lazy_23) {
        sysml_lazy_23 = sysml_state_colorIndex == 6;
    }
    sysml_lazy_24 = sysml_lazy_23;
    if (!sysml_lazy_24) {
        sysml_lazy_24 = sysml_state_colorIndex == 7;
    }
    if (sysml_lazy_24) {
        sysml_conditional_25 = 255.0;
    }
    else {
        sysml_conditional_25 = 0.0;
    }
    sysml_value_2 = sysml_conditional_25;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}