#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode = 2;
/* Global Variable define of  */
double sysml_state_pwmValue = 0.0;
/* Global Variable define of  */
int sysml_state_phaseMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 2;
    sysml_state_pwmValue = 0.0;
    sysml_state_phaseMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, int io_2, bool io_2_valid, double* io_0, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_arithmetic_3 = 0;
    double sysml_conditional_4 = 0;
    sysml_lazy_0 = io_2_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_2 == 1;
    }
    if (sysml_lazy_0) {
        sysml_state_mode = 1;
        sysml_state_phaseMs = 0;
    }
    sysml_lazy_1 = io_2_valid;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2 == 2;
    }
    if (sysml_lazy_1) {
        sysml_state_mode = 2;
        sysml_state_phaseMs = 0;
    }
    if ((sysml_state_mode == 1)) {
        sysml_state_phaseMs = (sysml_state_phaseMs + io_1);
        if ((((double)(((double)sysml_state_phaseMs) / 10)) / 2) < ((double)((int)(((double)(((double)sysml_state_phaseMs) / 10)) / 2)))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_arithmetic_3 = ((int)(((double)(((double)sysml_state_phaseMs) / 10)) / 2)) - sysml_conditional_2;
        if ((((double)sysml_state_phaseMs) / 10) == (2 * sysml_arithmetic_3)) {
            sysml_conditional_4 = 1023.0;
        }
        else {
            sysml_conditional_4 = 0.0;
        }
        sysml_state_pwmValue = sysml_conditional_4;
    }
    else {
        sysml_state_pwmValue = 1023.0;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_pwmValue;
    bool sysml_value_1 = 0;
    sysml_value_1 = true;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_mode;
    double sysml_value_3 = 0;
    sysml_value_3 = sysml_state_pwmValue;
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
}