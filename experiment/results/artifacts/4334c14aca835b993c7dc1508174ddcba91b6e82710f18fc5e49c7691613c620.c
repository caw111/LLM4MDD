#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeAuto = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
double sysml_state_manualLevel = 0.0;
/* Global Variable define of  */
double sysml_state_autoSpeed = 0.0;
/* Global Variable define of  */
double sysml_state_temperatureC = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeAuto = false;
    sysml_state_previousPressed = false;
    sysml_state_manualLevel = 0.0;
    sysml_state_autoSpeed = 0.0;
    sysml_state_temperatureC = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_3, double io_5, double* io_2, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    sysml_state_temperatureC = ((double)((io_0 * 100.0)) / 1023.0);
    sysml_state_manualLevel = ((double)((io_1 * 255.0)) / 1023.0);
    if (sysml_state_temperatureC <= 20.0) {
        sysml_conditional_0 = 0.0;
    }
    else {
        if (sysml_state_temperatureC >= 50.0) {
            sysml_conditional_1 = 255.0;
        }
        else {
            sysml_conditional_1 = ((double)((sysml_state_temperatureC - 20.0) * 255.0)) / 30.0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_state_autoSpeed = sysml_conditional_0;
    sysml_lazy_2 = io_3;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_2) {
        sysml_state_modeAuto = (!sysml_state_modeAuto);
    }
    sysml_state_previousPressed = io_3;
    double sysml_value_0 = 0;
    if (sysml_state_modeAuto) {
        sysml_conditional_3 = sysml_state_autoSpeed;
    }
    else {
        sysml_conditional_3 = sysml_state_manualLevel;
    }
    sysml_value_0 = sysml_conditional_3;
    int sysml_value_1 = 0;
    if (sysml_state_modeAuto) {
        if (sysml_state_temperatureC < ((double)((int)sysml_state_temperatureC))) {
            sysml_conditional_5 = 1;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_4 = ((int)sysml_state_temperatureC) - sysml_conditional_5;
    }
    else {
        if ((((double)sysml_state_manualLevel) / 10.0) < ((double)((int)(((double)sysml_state_manualLevel) / 10.0)))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        sysml_conditional_4 = ((int)(((double)sysml_state_manualLevel) / 10.0)) - sysml_conditional_6;
    }
    sysml_value_1 = sysml_conditional_4;
    *io_2 = sysml_value_0;
    *io_4 = sysml_value_1;
}