#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousVibration = false;
/* Global Variable define of  */
int sysml_state_stateCode = 0;
/* Global Variable define of  */
int sysml_state_faultCode = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousVibration = false;
    sysml_state_stateCode = 0;
    sysml_state_faultCode = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double* io_3, int* io_4, bool* io_5, int* io_6, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2, int* serial_0_value_3, int* serial_0_value_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    double sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousVibration;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = io_2;
    }
    if (sysml_lazy_1) {
        sysml_state_stateCode = 4;
        sysml_state_faultCode = 1;
    }
    else {
        if ((io_1 >= 0.0)) {
            sysml_state_faultCode = 0;
            if ((io_1 >= 30.0)) {
                sysml_state_stateCode = 3;
            }
            else {
                if ((io_0 >= 0.0)) {
                    sysml_state_stateCode = 1;
                }
                else {
                    sysml_state_stateCode = 0;
                }
            }
        }
        else {
            sysml_state_faultCode = 0;
            sysml_state_stateCode = 0;
        }
    }
    sysml_state_previousVibration = io_2;
    double sysml_value_0 = 0;
    if (sysml_state_stateCode == 3) {
        sysml_conditional_2 = 255.0;
    }
    else {
        sysml_conditional_2 = 0.0;
    }
    sysml_value_0 = sysml_conditional_2;
    int sysml_value_1 = 0;
    if (sysml_state_faultCode != 0) {
        sysml_conditional_3 = 2000;
    }
    else {
        if (sysml_state_stateCode == 3) {
            sysml_conditional_4 = 1000;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    sysml_value_1 = sysml_conditional_3;
    bool sysml_value_2 = 0;
    if (sysml_state_faultCode != 0) {
        sysml_conditional_5 = true;
    }
    else {
        if (sysml_state_stateCode == 3) {
            sysml_conditional_6 = true;
        }
        else {
            sysml_conditional_6 = false;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_value_2 = sysml_conditional_5;
    int sysml_value_3 = 0;
    if (sysml_state_faultCode != 0) {
        sysml_conditional_7 = 40;
    }
    else {
        sysml_conditional_7 = sysml_state_stateCode;
    }
    sysml_value_3 = sysml_conditional_7;
    bool sysml_value_4 = 0;
    sysml_value_4 = true;
    double sysml_value_5 = 0;
    sysml_value_5 = io_0;
    double sysml_value_6 = 0;
    sysml_value_6 = io_1;
    bool sysml_value_7 = 0;
    sysml_value_7 = io_2;
    int sysml_value_8 = 0;
    sysml_value_8 = sysml_state_stateCode;
    int sysml_value_9 = 0;
    sysml_value_9 = sysml_state_faultCode;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
    *serial_0_value_2 = sysml_value_7;
    *serial_0_value_3 = sysml_value_8;
    *serial_0_value_4 = sysml_value_9;
}