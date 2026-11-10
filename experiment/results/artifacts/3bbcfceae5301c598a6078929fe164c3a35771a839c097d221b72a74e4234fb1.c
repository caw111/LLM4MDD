#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_todayUsageCount = 0;
/* Global Variable define of  */
bool sysml_state_previousNear = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_todayUsageCount = 0;
    sysml_state_previousNear = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_2, bool io_3, bool* io_1, int* io_4, int* io_5, bool* io_6, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    int sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    int sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_conditional_23 = 0;
    int sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    double sysml_conditional_26 = 0;
    double sysml_conditional_27 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousNear;
    }
    if (sysml_lazy_0) {
        sysml_state_previousNear = io_0;
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousNear;
    }
    if (sysml_lazy_1) {
        sysml_lazy_2 = io_3;
        if (sysml_lazy_2) {
            sysml_lazy_2 = io_2 >= 0.0;
        }
        if (sysml_lazy_2) {
            sysml_state_todayUsageCount = (sysml_state_todayUsageCount + 1);
        }
    }
    sysml_state_previousNear = io_0;
    bool sysml_value_0 = 0;
    sysml_lazy_3 = io_0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_3;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_2 >= 20.0;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_0 = sysml_conditional_5;
    int sysml_value_1 = 0;
    sysml_lazy_6 = io_3;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_2 >= 2.0;
    }
    if (sysml_lazy_6) {
        if (io_2 <= 400.0) {
            if ((((double)(400.0 - io_2)) / 3.98) < ((double)((int)(((double)(400.0 - io_2)) / 3.98)))) {
                sysml_conditional_9 = 1;
            }
            else {
                sysml_conditional_9 = 0;
            }
            if ((((double)(400.0 - io_2)) / 3.98) < ((double)((int)(((double)(400.0 - io_2)) / 3.98)))) {
                sysml_conditional_10 = 1;
            }
            else {
                sysml_conditional_10 = 0;
            }
            if ((((double)(400.0 - io_2)) / 3.98) < ((double)((int)(((double)(400.0 - io_2)) / 3.98)))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            if ((100 - (((int)(((double)(400.0 - io_2)) / 3.98)) - sysml_conditional_10)) < ((double)((int)(100 - (((int)(((double)(400.0 - io_2)) / 3.98)) - sysml_conditional_11))))) {
                sysml_conditional_12 = 1;
            }
            else {
                sysml_conditional_12 = 0;
            }
            sysml_conditional_8 = ((int)(100 - (((int)(((double)(400.0 - io_2)) / 3.98)) - sysml_conditional_9))) - sysml_conditional_12;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_1 = sysml_conditional_7;
    int sysml_value_2 = 0;
    sysml_lazy_13 = io_0;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !io_3;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = 0;
    }
    else {
        sysml_lazy_15 = io_0;
        if (sysml_lazy_15) {
            sysml_lazy_15 = io_3;
        }
        sysml_lazy_16 = sysml_lazy_15;
        if (sysml_lazy_16) {
            sysml_lazy_16 = io_2 < 20.0;
        }
        if (sysml_lazy_16) {
            sysml_conditional_17 = 2000;
        }
        else {
            sysml_conditional_17 = 0;
        }
        sysml_conditional_14 = sysml_conditional_17;
    }
    sysml_value_2 = sysml_conditional_14;
    bool sysml_value_3 = 0;
    sysml_lazy_18 = io_0;
    if (sysml_lazy_18) {
        sysml_lazy_18 = io_3;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (sysml_lazy_19) {
        sysml_lazy_19 = io_2 >= 20.0;
    }
    if (sysml_lazy_19) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_lazy_21 = io_0;
        if (sysml_lazy_21) {
            sysml_lazy_21 = io_3;
        }
        sysml_lazy_22 = sysml_lazy_21;
        if (sysml_lazy_22) {
            sysml_lazy_22 = io_2 < 20.0;
        }
        if (sysml_lazy_22) {
            sysml_conditional_23 = true;
        }
        else {
            sysml_conditional_23 = false;
        }
        sysml_conditional_20 = sysml_conditional_23;
    }
    sysml_value_3 = sysml_conditional_20;
    bool sysml_value_4 = 0;
    sysml_value_4 = io_0;
    int sysml_value_5 = 0;
    if (io_0) {
        sysml_conditional_24 = sysml_state_todayUsageCount;
    }
    else {
        sysml_conditional_24 = sysml_state_todayUsageCount;
    }
    sysml_value_5 = sysml_conditional_24;
    double sysml_value_6 = 0;
    sysml_lazy_25 = io_3;
    if (sysml_lazy_25) {
        sysml_lazy_25 = io_2 >= 2.0;
    }
    if (sysml_lazy_25) {
        if (io_2 <= 400.0) {
            sysml_conditional_27 = ((double)(400.0 - io_2)) / 3.98;
        }
        else {
            sysml_conditional_27 = 0.0;
        }
        sysml_conditional_26 = sysml_conditional_27;
    }
    else {
        sysml_conditional_26 = 0.0;
    }
    sysml_value_6 = sysml_conditional_26;
    bool sysml_value_7 = 0;
    sysml_value_7 = io_3;
    *io_1 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
    *serial_0_value_2 = sysml_value_7;
}