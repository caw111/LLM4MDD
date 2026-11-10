#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_targetTempC = 25.0;
/* Global Variable define of  */
int sysml_state_minFanPwm = 30;
/* Global Variable define of  */
double sysml_state_serialSampleTempC = 0.0;
/* Global Variable define of  */
bool sysml_state_ledOverheatLatched = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_targetTempC = 25.0;
    sysml_state_minFanPwm = 30;
    sysml_state_serialSampleTempC = 0.0;
    sysml_state_ledOverheatLatched = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int io_3, bool io_3_valid, double* io_1, bool* io_2, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, int* serial_0_value_2, bool* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    if (io_3_valid) {
        sysml_state_serialSampleTempC = io_0;
        if ((io_3 == 1)) {
            sysml_state_targetTempC = (sysml_state_targetTempC + 1.0);
        }
        if ((io_3 == 2)) {
            sysml_state_targetTempC = (sysml_state_targetTempC + 2.0);
        }
        if ((io_3 == 3)) {
            sysml_state_targetTempC = (sysml_state_targetTempC + 3.0);
        }
        if ((io_3 == 4)) {
            sysml_state_targetTempC = (sysml_state_targetTempC + 4.0);
        }
        if ((io_3 == 5)) {
            sysml_state_targetTempC = (sysml_state_targetTempC + 5.0);
        }
        if ((io_3 == 21)) {
            sysml_state_targetTempC = (sysml_state_targetTempC - 1.0);
        }
        if ((io_3 == 22)) {
            sysml_state_targetTempC = (sysml_state_targetTempC - 2.0);
        }
        if ((io_3 == 23)) {
            sysml_state_targetTempC = (sysml_state_targetTempC - 3.0);
        }
        if ((io_3 == 24)) {
            sysml_state_targetTempC = (sysml_state_targetTempC - 4.0);
        }
        if ((io_3 == 25)) {
            sysml_state_targetTempC = (sysml_state_targetTempC - 5.0);
        }
        if ((io_3 == 26)) {
            sysml_state_minFanPwm = (sysml_state_minFanPwm + 1);
        }
        if ((io_3 == 27)) {
            sysml_state_minFanPwm = (sysml_state_minFanPwm + 2);
        }
        if ((io_3 == 28)) {
            sysml_state_minFanPwm = (sysml_state_minFanPwm + 3);
        }
        if ((io_3 == 29)) {
            sysml_state_minFanPwm = (sysml_state_minFanPwm + 4);
        }
        if ((io_3 == 30)) {
            sysml_state_minFanPwm = (sysml_state_minFanPwm + 5);
        }
        if ((io_3 == 31)) {
            sysml_state_targetTempC = 25.0;
            sysml_state_minFanPwm = 30;
        }
    }
    if ((sysml_state_serialSampleTempC >= (sysml_state_targetTempC + 5.0))) {
        sysml_state_ledOverheatLatched = true;
    }
    else {
        sysml_state_ledOverheatLatched = false;
    }
    double sysml_value_0 = 0;
    if (sysml_state_serialSampleTempC >= sysml_state_targetTempC) {
        if (sysml_state_serialSampleTempC >= (sysml_state_targetTempC + 5.0)) {
            sysml_conditional_1 = 255.0;
        }
        else {
            if (sysml_state_minFanPwm >= 255) {
                sysml_conditional_2 = 255.0;
            }
            else {
                if (sysml_state_minFanPwm != 0) {
                    if (((sysml_state_serialSampleTempC - sysml_state_targetTempC) * 10.0) < ((double)((int)((sysml_state_serialSampleTempC - sysml_state_targetTempC) * 10.0)))) {
                        sysml_conditional_4 = 1;
                    }
                    else {
                        sysml_conditional_4 = 0;
                    }
                    sysml_conditional_3 = ((int)((sysml_state_serialSampleTempC - sysml_state_targetTempC) * 10.0)) - sysml_conditional_4;
                }
                else {
                    sysml_conditional_3 = 0;
                }
                sysml_conditional_2 = sysml_state_minFanPwm + sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        if (sysml_state_minFanPwm >= 0) {
            if (sysml_state_minFanPwm < ((double)((int)sysml_state_minFanPwm))) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = 0;
            }
            sysml_conditional_5 = ((int)sysml_state_minFanPwm) - sysml_conditional_6;
        }
        else {
            sysml_conditional_5 = 0.0;
        }
        sysml_conditional_0 = sysml_conditional_5;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_ledOverheatLatched;
    bool sysml_value_2 = 0;
    sysml_value_2 = io_3_valid;
    double sysml_value_3 = 0;
    sysml_value_3 = sysml_state_serialSampleTempC;
    double sysml_value_4 = 0;
    sysml_value_4 = sysml_state_targetTempC;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_minFanPwm;
    bool sysml_value_6 = 0;
    sysml_value_6 = sysml_state_ledOverheatLatched;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
    *serial_0_value_1 = sysml_value_4;
    *serial_0_value_2 = sysml_value_5;
    *serial_0_value_3 = sysml_value_6;
}