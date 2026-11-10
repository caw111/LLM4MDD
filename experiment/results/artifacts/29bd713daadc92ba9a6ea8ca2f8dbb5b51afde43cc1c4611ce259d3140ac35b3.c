#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_ledPhaseMs = 0;
/* Global Variable define of  */
int sysml_state_sevsegPhaseMs = 0;
/* Global Variable define of  */
int sysml_state_sevsegDigit = 0;
/* Global Variable define of  */
int sysml_state_servo1PhaseMs = 0;
/* Global Variable define of  */
int sysml_state_servo2PhaseMs = 0;
/* Global Variable define of  */
bool sysml_state_servo1AtEndpoint = false;
/* Global Variable define of  */
bool sysml_state_servo2AtEndpoint = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledPhaseMs = 0;
    sysml_state_sevsegPhaseMs = 0;
    sysml_state_sevsegDigit = 0;
    sysml_state_servo1PhaseMs = 0;
    sysml_state_servo2PhaseMs = 0;
    sysml_state_servo1AtEndpoint = false;
    sysml_state_servo2AtEndpoint = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int elapsedMilliseconds, bool* io_0, bool* io_1, int* io_2, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    sysml_state_ledPhaseMs = (sysml_state_ledPhaseMs + elapsedMilliseconds);
    if ((sysml_state_ledPhaseMs >= 2000)) {
        sysml_state_ledPhaseMs = (sysml_state_ledPhaseMs - 2000);
    }
    sysml_state_sevsegPhaseMs = (sysml_state_sevsegPhaseMs + elapsedMilliseconds);
    if ((sysml_state_sevsegPhaseMs >= 2000)) {
        sysml_state_sevsegPhaseMs = (sysml_state_sevsegPhaseMs - 2000);
        if (sysml_state_sevsegDigit == 9) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_sevsegDigit + 1;
        }
        sysml_state_sevsegDigit = sysml_conditional_0;
    }
    sysml_state_servo1PhaseMs = (sysml_state_servo1PhaseMs + elapsedMilliseconds);
    sysml_state_servo2PhaseMs = (sysml_state_servo2PhaseMs + elapsedMilliseconds);
    sysml_lazy_1 = sysml_state_servo1AtEndpoint;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_servo1PhaseMs >= 2000;
    }
    if (sysml_lazy_1) {
        sysml_state_servo1PhaseMs = (sysml_state_servo1PhaseMs - 2000);
        sysml_state_servo1AtEndpoint = false;
    }
    sysml_lazy_2 = !sysml_state_servo1AtEndpoint;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_servo1PhaseMs >= 2000;
    }
    if (sysml_lazy_2) {
        sysml_state_servo1PhaseMs = (sysml_state_servo1PhaseMs - 2000);
        sysml_state_servo1AtEndpoint = true;
    }
    sysml_lazy_3 = sysml_state_servo2AtEndpoint;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_servo2PhaseMs >= 2000;
    }
    if (sysml_lazy_3) {
        sysml_state_servo2PhaseMs = (sysml_state_servo2PhaseMs - 2000);
        sysml_state_servo2AtEndpoint = false;
    }
    sysml_lazy_4 = !sysml_state_servo2AtEndpoint;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_servo2PhaseMs >= 2000;
    }
    if (sysml_lazy_4) {
        sysml_state_servo2PhaseMs = (sysml_state_servo2PhaseMs - 2000);
        sysml_state_servo2AtEndpoint = true;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_ledPhaseMs >= 1000) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_0 = sysml_conditional_5;
    bool sysml_value_1 = 0;
    if (sysml_state_ledPhaseMs >= 1000) {
        sysml_conditional_6 = false;
    }
    else {
        sysml_conditional_6 = true;
    }
    sysml_value_1 = sysml_conditional_6;
    int sysml_value_2 = 0;
    if (sysml_state_sevsegDigit >= 0) {
        sysml_conditional_7 = sysml_state_sevsegDigit;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_2 = sysml_conditional_7;
    double sysml_value_3 = 0;
    if (sysml_state_servo1AtEndpoint) {
        if (sysml_state_servo1PhaseMs < 1) {
            sysml_conditional_9 = 0.0;
        }
        else {
            sysml_conditional_9 = 180.0;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    else {
        if (sysml_state_servo1PhaseMs < 1000) {
            if (sysml_state_servo1PhaseMs == 0) {
                sysml_conditional_11 = 0.0;
            }
            else {
                sysml_conditional_11 = (((double)sysml_state_servo1PhaseMs) / 1000.0) * 180.0;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        else {
            sysml_conditional_10 = (((double)(sysml_state_servo1PhaseMs - 1000)) / 1000.0) * 180.0;
        }
        sysml_conditional_8 = sysml_conditional_10;
    }
    sysml_value_3 = sysml_conditional_8;
    double sysml_value_4 = 0;
    if (sysml_state_servo2AtEndpoint) {
        if (sysml_state_servo2PhaseMs < 1) {
            sysml_conditional_13 = 180.0;
        }
        else {
            sysml_conditional_13 = 0.0;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    else {
        if (sysml_state_servo2PhaseMs < 1000) {
            if (sysml_state_servo2PhaseMs == 0) {
                sysml_conditional_15 = 180.0;
            }
            else {
                sysml_conditional_15 = 180.0 - ((((double)sysml_state_servo2PhaseMs) / 1000.0) * 180.0);
            }
            sysml_conditional_14 = sysml_conditional_15;
        }
        else {
            sysml_conditional_14 = 180.0 - ((((double)(sysml_state_servo2PhaseMs - 1000)) / 1000.0) * 180.0);
        }
        sysml_conditional_12 = sysml_conditional_14;
    }
    sysml_value_4 = sysml_conditional_12;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
}