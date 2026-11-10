#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_phaseMs = 0;
/* Global Variable define of  */
int sysml_state_colorIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phaseMs = 0;
    sysml_state_colorIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool* io_0, bool* io_1, bool* io_2, double* io_3, double* io_4, double* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    double sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    double sysml_conditional_21 = 0;
    double sysml_conditional_22 = 0;
    double sysml_conditional_23 = 0;
    double sysml_conditional_24 = 0;
    double sysml_conditional_25 = 0;
    double sysml_conditional_26 = 0;
    double sysml_conditional_27 = 0;
    double sysml_conditional_28 = 0;
    double sysml_conditional_29 = 0;
    double sysml_conditional_30 = 0;
    double sysml_conditional_31 = 0;
    double sysml_conditional_32 = 0;
    double sysml_conditional_33 = 0;
    double sysml_conditional_34 = 0;
    if ((sysml_state_phaseMs >= 2000)) {
        sysml_state_phaseMs = (sysml_state_phaseMs - 2000);
        if (sysml_state_colorIndex == 6) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_colorIndex + 1;
        }
        sysml_state_colorIndex = sysml_conditional_0;
    }
    else {
        sysml_state_phaseMs = (sysml_state_phaseMs + 10);
    }
    bool sysml_value_0 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_1 = true;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_2 = false;
        }
        else {
            if (sysml_state_colorIndex == 3) {
                sysml_conditional_3 = true;
            }
            else {
                if (sysml_state_colorIndex == 5) {
                    sysml_conditional_4 = true;
                }
                else {
                    if (sysml_state_colorIndex == 6) {
                        sysml_conditional_5 = true;
                    }
                    else {
                        sysml_conditional_5 = false;
                    }
                    sysml_conditional_4 = sysml_conditional_5;
                }
                sysml_conditional_3 = sysml_conditional_4;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_6 = false;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_7 = true;
        }
        else {
            if (sysml_state_colorIndex == 3) {
                sysml_conditional_8 = true;
            }
            else {
                if (sysml_state_colorIndex == 4) {
                    sysml_conditional_9 = true;
                }
                else {
                    if (sysml_state_colorIndex == 6) {
                        sysml_conditional_10 = true;
                    }
                    else {
                        sysml_conditional_10 = false;
                    }
                    sysml_conditional_9 = sysml_conditional_10;
                }
                sysml_conditional_8 = sysml_conditional_9;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_1 = sysml_conditional_6;
    bool sysml_value_2 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_11 = false;
    }
    else {
        if (sysml_state_colorIndex == 2) {
            sysml_conditional_12 = true;
        }
        else {
            if (sysml_state_colorIndex == 3) {
                sysml_conditional_13 = false;
            }
            else {
                if (sysml_state_colorIndex == 4) {
                    sysml_conditional_14 = true;
                }
                else {
                    if (sysml_state_colorIndex == 5) {
                        sysml_conditional_15 = true;
                    }
                    else {
                        if (sysml_state_colorIndex == 6) {
                            sysml_conditional_16 = true;
                        }
                        else {
                            sysml_conditional_16 = false;
                        }
                        sysml_conditional_15 = sysml_conditional_16;
                    }
                    sysml_conditional_14 = sysml_conditional_15;
                }
                sysml_conditional_13 = sysml_conditional_14;
            }
            sysml_conditional_12 = sysml_conditional_13;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    sysml_value_2 = sysml_conditional_11;
    double sysml_value_3 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_17 = 0.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_18 = 90.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_19 = 180.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_20 = 45.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_21 = 135.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_22 = 180.0;
                        }
                        else {
                            sysml_conditional_22 = 0.0;
                        }
                        sysml_conditional_21 = sysml_conditional_22;
                    }
                    sysml_conditional_20 = sysml_conditional_21;
                }
                sysml_conditional_19 = sysml_conditional_20;
            }
            sysml_conditional_18 = sysml_conditional_19;
        }
        sysml_conditional_17 = sysml_conditional_18;
    }
    sysml_value_3 = sysml_conditional_17;
    double sysml_value_4 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_23 = 90.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_24 = 180.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_25 = 0.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_26 = 135.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_27 = 180.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_28 = 45.0;
                        }
                        else {
                            sysml_conditional_28 = 90.0;
                        }
                        sysml_conditional_27 = sysml_conditional_28;
                    }
                    sysml_conditional_26 = sysml_conditional_27;
                }
                sysml_conditional_25 = sysml_conditional_26;
            }
            sysml_conditional_24 = sysml_conditional_25;
        }
        sysml_conditional_23 = sysml_conditional_24;
    }
    sysml_value_4 = sysml_conditional_23;
    double sysml_value_5 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_29 = 180.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_30 = 0.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_31 = 90.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_32 = 180.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_33 = 45.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_34 = 135.0;
                        }
                        else {
                            sysml_conditional_34 = 180.0;
                        }
                        sysml_conditional_33 = sysml_conditional_34;
                    }
                    sysml_conditional_32 = sysml_conditional_33;
                }
                sysml_conditional_31 = sysml_conditional_32;
            }
            sysml_conditional_30 = sysml_conditional_31;
        }
        sysml_conditional_29 = sysml_conditional_30;
    }
    sysml_value_5 = sysml_conditional_29;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
}