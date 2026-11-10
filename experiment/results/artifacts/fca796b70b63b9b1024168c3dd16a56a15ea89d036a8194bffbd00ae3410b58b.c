#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousNear = false;
/* Global Variable define of  */
int sysml_state_todayUsageCount = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousNear = false;
    sysml_state_todayUsageCount = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_2, bool io_3, bool* io_1, int* io_4, int* io_5, bool* io_6, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    int sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousNear;
    }
    if (sysml_lazy_0) {
        sysml_state_todayUsageCount = (sysml_state_todayUsageCount + 1);
    }
    sysml_state_previousNear = io_0;
    bool sysml_value_0 = 0;
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_3;
    }
    if (sysml_lazy_1) {
        sysml_lazy_3 = io_2 >= 10.0;
        if (sysml_lazy_3) {
            sysml_lazy_3 = io_3;
        }
        sysml_conditional_2 = sysml_lazy_3;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_0 = sysml_conditional_2;
    int sysml_value_1 = 0;
    if (io_3) {
        if (io_2 < 2.0) {
            sysml_conditional_5 = 0;
        }
        else {
            if (io_2 >= 400.0) {
                sysml_conditional_6 = 100;
            }
            else {
                if ((100 - (((double)(400.0 - io_2)) / 3.98)) < ((double)((int)(100 - (((double)(400.0 - io_2)) / 3.98))))) {
                    sysml_conditional_7 = 1;
                }
                else {
                    sysml_conditional_7 = 0;
                }
                sysml_conditional_6 = ((int)(100 - (((double)(400.0 - io_2)) / 3.98))) - sysml_conditional_7;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_1 = sysml_conditional_4;
    int sysml_value_2 = 0;
    sysml_lazy_8 = io_0;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_3;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_2 < 10.0;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 2000;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_2 = sysml_conditional_10;
    bool sysml_value_3 = 0;
    sysml_lazy_11 = io_0;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_3;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (sysml_lazy_12) {
        sysml_lazy_12 = io_2 >= 10.0;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_3 = sysml_conditional_13;
    bool sysml_value_4 = 0;
    if (false) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_4 = sysml_conditional_14;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_todayUsageCount;
    double sysml_value_6 = 0;
    if (io_3) {
        if (io_2 < 2.0) {
            sysml_conditional_16 = 0.0;
        }
        else {
            if (io_2 >= 400.0) {
                sysml_conditional_17 = 100.0;
            }
            else {
                sysml_conditional_17 = 100.0 - (((double)(400.0 - io_2)) / 3.98);
            }
            sysml_conditional_16 = sysml_conditional_17;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    else {
        sysml_conditional_15 = 0.0;
    }
    sysml_value_6 = sysml_conditional_15;
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