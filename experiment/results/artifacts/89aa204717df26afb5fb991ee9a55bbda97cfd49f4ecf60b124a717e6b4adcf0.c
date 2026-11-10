#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_thresholdC = 30.0;
/* Global Variable define of  */
double sysml_state_fanMinPwm = 0.0;
/* Global Variable define of  */
bool sysml_state_autoMode = false;
/* Global Variable define of  */
bool sysml_state_prevOverheatCond = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_thresholdC = 30.0;
    sysml_state_fanMinPwm = 0.0;
    sysml_state_autoMode = false;
    sysml_state_prevOverheatCond = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int io_3, bool io_3_valid, double* io_1, bool* io_2, bool* serial_0_trigger, double* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_conditional_13 = 0;
    sysml_lazy_0 = io_3_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_3 == 10;
    }
    if (sysml_lazy_0) {
        sysml_state_thresholdC = 30.0;
        sysml_state_fanMinPwm = 0.0;
        sysml_state_autoMode = false;
        sysml_state_prevOverheatCond = false;
    }
    sysml_lazy_1 = io_3_valid;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_3 == 7;
    }
    if (sysml_lazy_1) {
        sysml_state_thresholdC = 0.0;
    }
    sysml_lazy_2 = io_3_valid;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_3 == 8;
    }
    if (sysml_lazy_2) {
        sysml_state_fanMinPwm = 0.0;
    }
    sysml_lazy_3 = io_3_valid;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_3 == 11;
    }
    if (sysml_lazy_3) {
        sysml_state_autoMode = true;
    }
    sysml_lazy_4 = io_3_valid;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_3 == 12;
    }
    if (sysml_lazy_4) {
        sysml_state_autoMode = false;
    }
    if (io_0 >= (sysml_state_thresholdC + 5.0)) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_state_prevOverheatCond = sysml_conditional_5;
    double sysml_value_0 = 0;
    if (sysml_state_autoMode) {
        if (io_0 >= sysml_state_thresholdC) {
            if (io_0 >= (sysml_state_thresholdC + 5.0)) {
                sysml_conditional_8 = sysml_state_fanMinPwm;
            }
            else {
                sysml_conditional_8 = sysml_state_fanMinPwm;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        else {
            sysml_conditional_7 = 0.0;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_0 = sysml_conditional_6;
    bool sysml_value_1 = 0;
    sysml_lazy_9 = sysml_state_autoMode;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_0 >= (sysml_state_thresholdC + 5.0);
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_1 = sysml_conditional_10;
    bool sysml_value_2 = 0;
    sysml_lazy_11 = sysml_state_autoMode;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_0 >= (sysml_state_thresholdC + 5.0);
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (sysml_lazy_12) {
        sysml_lazy_12 = !sysml_state_prevOverheatCond;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_2 = sysml_conditional_13;
    double sysml_value_3 = 0;
    sysml_value_3 = io_0;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
}