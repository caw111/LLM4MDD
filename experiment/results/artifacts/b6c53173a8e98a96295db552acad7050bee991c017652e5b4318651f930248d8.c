#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeAuto = true;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_lowChargeAlarmed = false;
/* Global Variable define of  */
int sysml_state_lowChargeStartNonCharge = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeAuto = true;
    sysml_state_previousPressed = false;
    sysml_state_lowChargeAlarmed = false;
    sysml_state_lowChargeStartNonCharge = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_3, double* io_4, int* io_5, int* io_6, bool* serial_0_trigger, bool* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_modeAuto = (!sysml_state_modeAuto);
    }
    sysml_lazy_1 = sysml_state_modeAuto;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_0 > io_1;
    }
    if (sysml_lazy_1) {
        sysml_state_lowChargeStartNonCharge = 0;
    }
    else {
        sysml_state_lowChargeStartNonCharge = 0;
    }
    if ((io_2 <= 0.0)) {
        sysml_state_lowChargeAlarmed = true;
    }
    else {
        sysml_state_lowChargeAlarmed = false;
    }
    sysml_state_previousPressed = io_3;
    double sysml_value_0 = 0;
    if (sysml_state_modeAuto) {
        if (io_0 > io_1) {
            sysml_conditional_3 = 45.0;
        }
        else {
            if (io_0 < io_1) {
                sysml_conditional_4 = -45.0;
            }
            else {
                sysml_conditional_4 = 0.0;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    else {
        sysml_conditional_2 = 0.0;
    }
    sysml_value_0 = sysml_conditional_2;
    int sysml_value_1 = 0;
    if (sysml_state_modeAuto) {
        sysml_conditional_5 = 0;
    }
    else {
        sysml_conditional_5 = 1;
    }
    sysml_value_1 = sysml_conditional_5;
    int sysml_value_2 = 0;
    if (sysml_state_lowChargeAlarmed) {
        sysml_conditional_6 = 1000;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_2 = sysml_conditional_6;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_lowChargeAlarmed;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_lowChargeAlarmed;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
}