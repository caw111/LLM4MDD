#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_running = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_blinkPeriodMs = 0;
/* Global Variable define of  */
int sysml_state_remainingMs = 0;
/* Global Variable define of  */
int sysml_state_tickPhase = 0;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_running = false;
    sysml_state_previousPressed = false;
    sysml_state_blinkPeriodMs = 0;
    sysml_state_remainingMs = 0;
    sysml_state_tickPhase = 0;
    sysml_state_ledOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool* io_2, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_running = (!sysml_state_running);
    }
    sysml_state_previousPressed = io_1;
    sysml_lazy_1 = io_0 >= 0.0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_0 <= 1023.0;
    }
    if (sysml_lazy_1) {
        if (((io_0 * 1000.0) - 0.0) < ((double)((int)((io_0 * 1000.0) - 0.0)))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        if ((((int)((io_0 * 1000.0) - 0.0)) - sysml_conditional_2) == 0) {
            sysml_conditional_3 = 1;
        }
        else {
            if ((1.0 + ((((double)io_0) / 1023.0) * 999.0)) < ((double)((int)(1.0 + ((((double)io_0) / 1023.0) * 999.0))))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = ((int)(1.0 + ((((double)io_0) / 1023.0) * 999.0))) - sysml_conditional_4;
        }
        sysml_state_blinkPeriodMs = sysml_conditional_3;
    }
    else {
        sysml_state_blinkPeriodMs = 1;
    }
    sysml_lazy_5 = sysml_state_running;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_remainingMs == 0;
    }
    if (sysml_lazy_5) {
        sysml_state_remainingMs = sysml_state_blinkPeriodMs;
        sysml_state_tickPhase = 0;
        sysml_state_ledOn = true;
    }
    sysml_lazy_6 = !sysml_state_running;
    if (sysml_lazy_6) {
        sysml_lazy_6 = true;
    }
    if (sysml_lazy_6) {
        sysml_state_remainingMs = 0;
        sysml_state_tickPhase = 0;
        sysml_state_ledOn = false;
    }
    sysml_lazy_7 = sysml_state_running;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_remainingMs != 0;
    }
    if (sysml_lazy_7) {
        sysml_state_remainingMs = (sysml_state_remainingMs - 5);
        if ((sysml_state_remainingMs <= 0)) {
            sysml_state_ledOn = (!sysml_state_ledOn);
            sysml_state_remainingMs = sysml_state_blinkPeriodMs;
            sysml_state_tickPhase = (sysml_state_tickPhase + 1);
        }
    }
    bool sysml_value_0 = 0;
    sysml_lazy_8 = sysml_state_running;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_ledOn;
    }
    sysml_value_0 = sysml_lazy_8;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_blinkPeriodMs;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
}