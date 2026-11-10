#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_running = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_periodMs = 500;
/* Global Variable define of  */
int sysml_state_blinkPhase = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_running = false;
    sysml_state_previousPressed = false;
    sysml_state_periodMs = 500;
    sysml_state_blinkPhase = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool* io_2, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_running = (!sysml_state_running);
        sysml_state_blinkPhase = 0;
    }
    sysml_state_previousPressed = io_1;
    if (io_0 < ((double)((int)io_0))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    if ((((int)io_0) - sysml_conditional_1) <= 0) {
        sysml_conditional_2 = 1;
    }
    else {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        if ((((int)io_0) - sysml_conditional_3) >= 2000) {
            sysml_conditional_4 = 2000;
        }
        else {
            if (io_0 < ((double)((int)io_0))) {
                sysml_conditional_5 = 1;
            }
            else {
                sysml_conditional_5 = 0;
            }
            sysml_conditional_4 = ((int)io_0) - sysml_conditional_5;
        }
        sysml_conditional_2 = sysml_conditional_4;
    }
    sysml_state_periodMs = sysml_conditional_2;
    if (sysml_state_running) {
        sysml_state_blinkPhase = (sysml_state_blinkPhase + 10);
    }
    sysml_lazy_6 = sysml_state_running;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_blinkPhase >= sysml_state_periodMs;
    }
    if (sysml_lazy_6) {
        sysml_state_blinkPhase = (sysml_state_blinkPhase - sysml_state_periodMs);
    }
    bool sysml_value_0 = 0;
    sysml_lazy_7 = sysml_state_running;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_blinkPhase < (((double)sysml_state_periodMs) / 2);
    }
    sysml_value_0 = sysml_lazy_7;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_periodMs;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
}