#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeConflict = false;
/* Global Variable define of  */
bool sysml_state_ledGate = false;
/* Global Variable define of  */
bool sysml_state_previousBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2 = false;
/* Global Variable define of  */
int sysml_state_ledPulseAccumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeConflict = false;
    sysml_state_ledGate = false;
    sysml_state_previousBtn1 = false;
    sysml_state_previousBtn2 = false;
    sysml_state_ledPulseAccumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, int io_5, double* io_0, double* io_1, bool* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_4;
    }
    if (sysml_lazy_0) {
        sysml_state_modeConflict = true;
    }
    if ((!sysml_state_modeConflict)) {
        sysml_lazy_1 = io_3;
        if (sysml_lazy_1) {
            sysml_lazy_1 = !sysml_state_previousBtn1;
        }
        sysml_lazy_2 = sysml_lazy_1;
        if (sysml_lazy_2) {
            sysml_lazy_2 = io_4;
        }
        if (sysml_lazy_2) {
            sysml_state_modeConflict = true;
        }
        sysml_lazy_3 = io_4;
        if (sysml_lazy_3) {
            sysml_lazy_3 = !sysml_state_previousBtn2;
        }
        sysml_lazy_4 = sysml_lazy_3;
        if (sysml_lazy_4) {
            sysml_lazy_4 = io_3;
        }
        if (sysml_lazy_4) {
            sysml_state_modeConflict = true;
        }
    }
    else {
        sysml_lazy_5 = io_3;
        if (sysml_lazy_5) {
            sysml_lazy_5 = !io_4;
        }
        sysml_lazy_6 = sysml_lazy_5;
        if (!sysml_lazy_6) {
            sysml_lazy_7 = io_4;
            if (sysml_lazy_7) {
                sysml_lazy_7 = !io_3;
            }
            sysml_lazy_6 = sysml_lazy_7;
        }
        if (sysml_lazy_6) {
            sysml_state_modeConflict = false;
            sysml_state_ledPulseAccumMs = 0;
            sysml_state_ledGate = false;
        }
    }
    if (sysml_state_modeConflict) {
        sysml_state_ledPulseAccumMs = (sysml_state_ledPulseAccumMs + io_5);
        if ((sysml_state_ledPulseAccumMs >= 4000)) {
            sysml_state_ledPulseAccumMs = (sysml_state_ledPulseAccumMs - 4000);
        }
        sysml_state_ledGate = (sysml_state_ledPulseAccumMs < 2000);
    }
    else {
        sysml_state_ledGate = false;
        sysml_state_ledPulseAccumMs = 0;
    }
    sysml_state_previousBtn1 = io_3;
    sysml_state_previousBtn2 = io_4;
    double sysml_value_0 = 0;
    if (sysml_state_modeConflict) {
        sysml_conditional_8 = 0.0;
    }
    else {
        if (io_3) {
            sysml_conditional_9 = 90.0;
        }
        else {
            sysml_conditional_9 = 0.0;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    sysml_value_0 = sysml_conditional_8;
    double sysml_value_1 = 0;
    if (sysml_state_modeConflict) {
        sysml_conditional_10 = 0.0;
    }
    else {
        if (io_4) {
            sysml_conditional_11 = 180.0;
        }
        else {
            sysml_conditional_11 = 0.0;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    sysml_value_1 = sysml_conditional_10;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_ledGate;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}