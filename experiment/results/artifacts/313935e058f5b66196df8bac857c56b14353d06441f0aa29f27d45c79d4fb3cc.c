#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_ledPhaseMs = 0;
/* Global Variable define of  */
bool sysml_state_ledOnIsLed2 = false;
/* Global Variable define of  */
int sysml_state_sevsegCount = 0;
/* Global Variable define of  */
int sysml_state_sevsegPhaseMs = 0;
/* Global Variable define of  */
int sysml_state_servo1PhaseMs = 0;
/* Global Variable define of  */
int sysml_state_servo2PhaseMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledPhaseMs = 0;
    sysml_state_ledOnIsLed2 = false;
    sysml_state_sevsegCount = 0;
    sysml_state_sevsegPhaseMs = 0;
    sysml_state_servo1PhaseMs = 0;
    sysml_state_servo2PhaseMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_5, bool* io_0, bool* io_1, int* io_2, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    double sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    double sysml_conditional_21 = 0;
    double sysml_conditional_22 = 0;
    double sysml_conditional_23 = 0;
    double sysml_conditional_24 = 0;
    sysml_state_ledPhaseMs = (sysml_state_ledPhaseMs + io_5);
    if ((sysml_state_ledPhaseMs >= 2000)) {
        sysml_state_ledPhaseMs = (sysml_state_ledPhaseMs - 2000);
        sysml_state_ledOnIsLed2 = (!sysml_state_ledOnIsLed2);
    }
    sysml_state_sevsegPhaseMs = (sysml_state_sevsegPhaseMs + io_5);
    if ((sysml_state_sevsegPhaseMs >= 2000)) {
        sysml_state_sevsegPhaseMs = (sysml_state_sevsegPhaseMs - 2000);
        if (sysml_state_sevsegCount == 9) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_sevsegCount + 1;
        }
        sysml_state_sevsegCount = sysml_conditional_0;
    }
    sysml_state_servo1PhaseMs = (sysml_state_servo1PhaseMs + io_5);
    sysml_state_servo2PhaseMs = (sysml_state_servo2PhaseMs + io_5);
    if ((sysml_state_servo1PhaseMs >= 4000)) {
        sysml_state_servo1PhaseMs = (sysml_state_servo1PhaseMs - 4000);
    }
    if ((sysml_state_servo2PhaseMs >= 4000)) {
        sysml_state_servo2PhaseMs = (sysml_state_servo2PhaseMs - 4000);
    }
    bool sysml_value_0 = 0;
    if (sysml_state_ledOnIsLed2) {
        sysml_conditional_1 = false;
    }
    else {
        sysml_conditional_1 = true;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    if (sysml_state_ledOnIsLed2) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_1 = sysml_conditional_2;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_sevsegCount;
    double sysml_value_3 = 0;
    if (sysml_state_servo1PhaseMs < 2000) {
        if (sysml_state_servo1PhaseMs < 200) {
            sysml_conditional_4 = 0.0;
        }
        else {
            if (sysml_state_servo1PhaseMs < 2200) {
                if (sysml_state_servo1PhaseMs <= 1800) {
                    sysml_conditional_6 = ((double)((sysml_state_servo1PhaseMs - 200) * 180.0)) / 1800.0;
                }
                else {
                    sysml_conditional_6 = 180.0;
                }
                sysml_conditional_5 = sysml_conditional_6;
            }
            else {
                if (sysml_state_servo1PhaseMs < 2200) {
                    sysml_conditional_7 = 180.0;
                }
                else {
                    if (sysml_state_servo1PhaseMs < 2400) {
                        sysml_conditional_8 = 180.0;
                    }
                    else {
                        if (sysml_state_servo1PhaseMs < 4000) {
                            if (sysml_state_servo1PhaseMs <= 3800) {
                                sysml_conditional_10 = 180.0 - (((double)((sysml_state_servo1PhaseMs - 2400) * 180.0)) / 1600.0);
                            }
                            else {
                                sysml_conditional_10 = 0.0;
                            }
                            sysml_conditional_9 = sysml_conditional_10;
                        }
                        else {
                            sysml_conditional_9 = 0.0;
                        }
                        sysml_conditional_8 = sysml_conditional_9;
                    }
                    sysml_conditional_7 = sysml_conditional_8;
                }
                sysml_conditional_5 = sysml_conditional_7;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        if (sysml_state_servo1PhaseMs < 2200) {
            sysml_conditional_11 = 180.0;
        }
        else {
            if (sysml_state_servo1PhaseMs < 2400) {
                sysml_conditional_12 = 180.0;
            }
            else {
                if (sysml_state_servo1PhaseMs <= 3800) {
                    sysml_conditional_13 = 180.0 - (((double)((sysml_state_servo1PhaseMs - 2400) * 180.0)) / 1600.0);
                }
                else {
                    sysml_conditional_13 = 0.0;
                }
                sysml_conditional_12 = sysml_conditional_13;
            }
            sysml_conditional_11 = sysml_conditional_12;
        }
        sysml_conditional_3 = sysml_conditional_11;
    }
    sysml_value_3 = sysml_conditional_3;
    double sysml_value_4 = 0;
    if (sysml_state_servo2PhaseMs < 2000) {
        if (sysml_state_servo2PhaseMs < 200) {
            sysml_conditional_15 = 180.0;
        }
        else {
            if (sysml_state_servo2PhaseMs < 2200) {
                if (sysml_state_servo2PhaseMs <= 1800) {
                    sysml_conditional_17 = 180.0 - (((double)((sysml_state_servo2PhaseMs - 200) * 180.0)) / 1800.0);
                }
                else {
                    sysml_conditional_17 = 0.0;
                }
                sysml_conditional_16 = sysml_conditional_17;
            }
            else {
                if (sysml_state_servo2PhaseMs < 2200) {
                    sysml_conditional_18 = 0.0;
                }
                else {
                    if (sysml_state_servo2PhaseMs < 2400) {
                        sysml_conditional_19 = 0.0;
                    }
                    else {
                        if (sysml_state_servo2PhaseMs < 4000) {
                            if (sysml_state_servo2PhaseMs <= 3800) {
                                sysml_conditional_21 = ((double)((sysml_state_servo2PhaseMs - 2400) * 180.0)) / 1600.0;
                            }
                            else {
                                sysml_conditional_21 = 180.0;
                            }
                            sysml_conditional_20 = sysml_conditional_21;
                        }
                        else {
                            sysml_conditional_20 = 180.0;
                        }
                        sysml_conditional_19 = sysml_conditional_20;
                    }
                    sysml_conditional_18 = sysml_conditional_19;
                }
                sysml_conditional_16 = sysml_conditional_18;
            }
            sysml_conditional_15 = sysml_conditional_16;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    else {
        if (sysml_state_servo2PhaseMs < 2200) {
            sysml_conditional_22 = 0.0;
        }
        else {
            if (sysml_state_servo2PhaseMs < 2400) {
                sysml_conditional_23 = 0.0;
            }
            else {
                if (sysml_state_servo2PhaseMs <= 3800) {
                    sysml_conditional_24 = ((double)((sysml_state_servo2PhaseMs - 2400) * 180.0)) / 1600.0;
                }
                else {
                    sysml_conditional_24 = 180.0;
                }
                sysml_conditional_23 = sysml_conditional_24;
            }
            sysml_conditional_22 = sysml_conditional_23;
        }
        sysml_conditional_14 = sysml_conditional_22;
    }
    sysml_value_4 = sysml_conditional_14;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
}