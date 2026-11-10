#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_ledAccumMs = 0;
/* Global Variable define of  */
int sysml_state_colorIndex = 0;
/* Global Variable define of  */
int sysml_state_colorAccumMs = 0;
/* Global Variable define of  */
int sysml_state_servo1PosDeg = 0;
/* Global Variable define of  */
int sysml_state_servo2PosDeg = 180;
/* Global Variable define of  */
int sysml_state_servo3PosDeg = 90;
/* Global Variable define of  */
int sysml_state_servoAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledAccumMs = 0;
    sysml_state_colorIndex = 0;
    sysml_state_colorAccumMs = 0;
    sysml_state_servo1PosDeg = 0;
    sysml_state_servo2PosDeg = 180;
    sysml_state_servo3PosDeg = 90;
    sysml_state_servoAccumMs = 0;
    sysml_state_ledOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_10, bool* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
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
    double sysml_conditional_35 = 0;
    int sysml_conditional_36 = 0;
    int sysml_conditional_37 = 0;
    int sysml_conditional_38 = 0;
    sysml_state_ledAccumMs = (sysml_state_ledAccumMs + io_10);
    if ((sysml_state_ledAccumMs >= 2000)) {
        sysml_state_ledAccumMs = (sysml_state_ledAccumMs - 2000);
        sysml_state_ledOn = (!sysml_state_ledOn);
    }
    sysml_state_colorAccumMs = (sysml_state_colorAccumMs + io_10);
    if ((sysml_state_colorAccumMs >= 2000)) {
        sysml_state_colorAccumMs = (sysml_state_colorAccumMs - 2000);
        sysml_state_colorIndex = (sysml_state_colorIndex + 1);
    }
    if ((sysml_state_colorIndex >= 7)) {
        sysml_state_colorIndex = 0;
    }
    if (true) {
        sysml_state_servoAccumMs = (sysml_state_servoAccumMs + io_10);
        if ((sysml_state_servoAccumMs >= 2000)) {
            sysml_state_servoAccumMs = (sysml_state_servoAccumMs - 2000);
            if ((sysml_state_servo1PosDeg == 0)) {
                sysml_state_servo1PosDeg = 180;
            }
            else {
                sysml_state_servo1PosDeg = 0;
            }
            if ((sysml_state_servo2PosDeg == 180)) {
                sysml_state_servo2PosDeg = 0;
            }
            else {
                sysml_state_servo2PosDeg = 180;
            }
            if ((sysml_state_servo3PosDeg == 90)) {
                sysml_state_servo3PosDeg = 135;
            }
            else {
                sysml_state_servo3PosDeg = 90;
            }
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_ledOn;
    double sysml_value_1 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_0 = 255.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_1 = 0.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_2 = 0.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_3 = 255.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_4 = 0.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_5 = 255.0;
                        }
                        else {
                            sysml_conditional_5 = 255.0;
                        }
                        sysml_conditional_4 = sysml_conditional_5;
                    }
                    sysml_conditional_3 = sysml_conditional_4;
                }
                sysml_conditional_2 = sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_value_1 = sysml_conditional_0;
    double sysml_value_2 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_6 = 0.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_7 = 255.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_8 = 0.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_9 = 255.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_10 = 255.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_11 = 0.0;
                        }
                        else {
                            sysml_conditional_11 = 255.0;
                        }
                        sysml_conditional_10 = sysml_conditional_11;
                    }
                    sysml_conditional_9 = sysml_conditional_10;
                }
                sysml_conditional_8 = sysml_conditional_9;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_2 = sysml_conditional_6;
    double sysml_value_3 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_12 = 0.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_13 = 0.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_14 = 255.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_15 = 0.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_16 = 255.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_17 = 255.0;
                        }
                        else {
                            sysml_conditional_17 = 255.0;
                        }
                        sysml_conditional_16 = sysml_conditional_17;
                    }
                    sysml_conditional_15 = sysml_conditional_16;
                }
                sysml_conditional_14 = sysml_conditional_15;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_3 = sysml_conditional_12;
    double sysml_value_4 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_18 = 255.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_19 = 0.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_20 = 0.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_21 = 255.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_22 = 0.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_23 = 255.0;
                        }
                        else {
                            sysml_conditional_23 = 255.0;
                        }
                        sysml_conditional_22 = sysml_conditional_23;
                    }
                    sysml_conditional_21 = sysml_conditional_22;
                }
                sysml_conditional_20 = sysml_conditional_21;
            }
            sysml_conditional_19 = sysml_conditional_20;
        }
        sysml_conditional_18 = sysml_conditional_19;
    }
    sysml_value_4 = sysml_conditional_18;
    double sysml_value_5 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_24 = 0.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_25 = 255.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_26 = 0.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_27 = 255.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_28 = 255.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_29 = 0.0;
                        }
                        else {
                            sysml_conditional_29 = 255.0;
                        }
                        sysml_conditional_28 = sysml_conditional_29;
                    }
                    sysml_conditional_27 = sysml_conditional_28;
                }
                sysml_conditional_26 = sysml_conditional_27;
            }
            sysml_conditional_25 = sysml_conditional_26;
        }
        sysml_conditional_24 = sysml_conditional_25;
    }
    sysml_value_5 = sysml_conditional_24;
    double sysml_value_6 = 0;
    if (sysml_state_colorIndex == 0) {
        sysml_conditional_30 = 0.0;
    }
    else {
        if (sysml_state_colorIndex == 1) {
            sysml_conditional_31 = 0.0;
        }
        else {
            if (sysml_state_colorIndex == 2) {
                sysml_conditional_32 = 255.0;
            }
            else {
                if (sysml_state_colorIndex == 3) {
                    sysml_conditional_33 = 0.0;
                }
                else {
                    if (sysml_state_colorIndex == 4) {
                        sysml_conditional_34 = 255.0;
                    }
                    else {
                        if (sysml_state_colorIndex == 5) {
                            sysml_conditional_35 = 255.0;
                        }
                        else {
                            sysml_conditional_35 = 255.0;
                        }
                        sysml_conditional_34 = sysml_conditional_35;
                    }
                    sysml_conditional_33 = sysml_conditional_34;
                }
                sysml_conditional_32 = sysml_conditional_33;
            }
            sysml_conditional_31 = sysml_conditional_32;
        }
        sysml_conditional_30 = sysml_conditional_31;
    }
    sysml_value_6 = sysml_conditional_30;
    double sysml_value_7 = 0;
    if (sysml_state_servo1PosDeg < ((double)((int)sysml_state_servo1PosDeg))) {
        sysml_conditional_36 = 1;
    }
    else {
        sysml_conditional_36 = 0;
    }
    sysml_value_7 = ((int)sysml_state_servo1PosDeg) - sysml_conditional_36;
    double sysml_value_8 = 0;
    if (sysml_state_servo2PosDeg < ((double)((int)sysml_state_servo2PosDeg))) {
        sysml_conditional_37 = 1;
    }
    else {
        sysml_conditional_37 = 0;
    }
    sysml_value_8 = ((int)sysml_state_servo2PosDeg) - sysml_conditional_37;
    double sysml_value_9 = 0;
    if (sysml_state_servo3PosDeg < ((double)((int)sysml_state_servo3PosDeg))) {
        sysml_conditional_38 = 1;
    }
    else {
        sysml_conditional_38 = 0;
    }
    sysml_value_9 = ((int)sysml_state_servo3PosDeg) - sysml_conditional_38;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_8 = sysml_value_8;
    *io_9 = sysml_value_9;
}