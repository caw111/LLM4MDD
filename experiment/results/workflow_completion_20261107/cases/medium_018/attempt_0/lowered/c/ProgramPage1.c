#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_automaticMode = true;
/* Global Variable define of  */
bool sysml_state_previousAutoPressed = false;
/* Global Variable define of  */
bool sysml_state_previousManualPressed = false;
/* Global Variable define of  */
double sysml_state_rampPhaseMs = 0.0;
/* Global Variable define of  */
double sysml_state_directionDurationMs = 2000.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_automaticMode = true;
    sysml_state_previousAutoPressed = false;
    sysml_state_previousManualPressed = false;
    sysml_state_rampPhaseMs = 0.0;
    sysml_state_directionDurationMs = 2000.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, int io_4, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_arithmetic_0 = 0;
    double sysml_arithmetic_1 = 0;
    double sysml_arithmetic_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_arithmetic_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_arithmetic_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    if (sysml_state_automaticMode) {
        sysml_state_rampPhaseMs = (sysml_state_rampPhaseMs + io_4);
        sysml_arithmetic_0 = 2.0 * sysml_state_directionDurationMs;
        sysml_arithmetic_1 = 2.0 * sysml_state_directionDurationMs;
        sysml_arithmetic_2 = 2.0 * sysml_state_directionDurationMs;
        if ((((double)sysml_state_rampPhaseMs) / sysml_arithmetic_1) < ((double)((int)(((double)sysml_state_rampPhaseMs) / sysml_arithmetic_2)))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_arithmetic_4 = ((int)(((double)sysml_state_rampPhaseMs) / sysml_arithmetic_0)) - sysml_conditional_3;
        sysml_state_rampPhaseMs = sysml_state_rampPhaseMs - ((2.0 * sysml_state_directionDurationMs) * sysml_arithmetic_4);
    }
    sysml_lazy_5 = io_0;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousAutoPressed;
    }
    if (sysml_lazy_5) {
        sysml_state_automaticMode = true;
        sysml_state_rampPhaseMs = 0.0;
    }
    else {
        sysml_lazy_6 = io_1;
        if (sysml_lazy_6) {
            sysml_lazy_6 = !sysml_state_previousManualPressed;
        }
        if (sysml_lazy_6) {
            sysml_state_automaticMode = false;
        }
    }
    sysml_state_previousAutoPressed = io_0;
    sysml_state_previousManualPressed = io_1;
    double sysml_value_0 = 0;
    if (sysml_state_automaticMode) {
        if (sysml_state_rampPhaseMs <= sysml_state_directionDurationMs) {
            sysml_conditional_8 = ((double)(255.0 * sysml_state_rampPhaseMs)) / sysml_state_directionDurationMs;
        }
        else {
            sysml_arithmetic_9 = (2.0 * sysml_state_directionDurationMs) - sysml_state_rampPhaseMs;
            sysml_conditional_8 = ((double)(255.0 * sysml_arithmetic_9)) / sysml_state_directionDurationMs;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    else {
        if (io_2 < 0.0) {
            sysml_conditional_10 = 0.0;
        }
        else {
            if (io_2 > 1023.0) {
                sysml_conditional_11 = 255.0;
            }
            else {
                sysml_conditional_11 = ((double)(io_2 * 255.0)) / 1023.0;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_7 = sysml_conditional_10;
    }
    sysml_value_0 = sysml_conditional_7;
    *io_3 = sysml_value_0;
}