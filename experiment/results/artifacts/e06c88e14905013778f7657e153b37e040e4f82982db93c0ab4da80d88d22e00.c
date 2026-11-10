#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeIsTemp = true;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
double sysml_state_lastTempC = 0.0;
/* Global Variable define of  */
double sysml_state_lastHumidityPct = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeIsTemp = true;
    sysml_state_previousPressed = false;
    sysml_state_lastTempC = 0.0;
    sysml_state_lastHumidityPct = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, bool io_3, double* io_4, int* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    if (io_2) {
        sysml_state_lastTempC = io_0;
        sysml_state_lastHumidityPct = io_1;
    }
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_modeIsTemp = (!sysml_state_modeIsTemp);
    }
    sysml_state_previousPressed = io_3;
    double sysml_value_0 = 0;
    if (sysml_state_lastTempC >= 30.0) {
        sysml_conditional_1 = ((sysml_state_lastTempC - 30.0) * 20.0) + 102.0;
    }
    else {
        sysml_conditional_1 = 0.0;
    }
    sysml_value_0 = sysml_conditional_1;
    int sysml_value_1 = 0;
    if (sysml_state_modeIsTemp) {
        if (sysml_state_lastTempC >= 0.0) {
            if (sysml_state_lastTempC < ((double)((int)sysml_state_lastTempC))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = ((int)sysml_state_lastTempC) - sysml_conditional_4;
        }
        else {
            if ((0.0 - sysml_state_lastTempC) < ((double)((int)(0.0 - sysml_state_lastTempC)))) {
                sysml_conditional_5 = 1;
            }
            else {
                sysml_conditional_5 = 0;
            }
            sysml_conditional_3 = 0 - (((int)(0.0 - sysml_state_lastTempC)) - sysml_conditional_5);
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    else {
        if (sysml_state_lastHumidityPct < ((double)((int)sysml_state_lastHumidityPct))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        sysml_conditional_2 = ((int)sysml_state_lastHumidityPct) - sysml_conditional_6;
    }
    sysml_value_1 = sysml_conditional_2;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
}