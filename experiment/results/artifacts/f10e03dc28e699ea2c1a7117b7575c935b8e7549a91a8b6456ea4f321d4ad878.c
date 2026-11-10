#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_displayMode = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_serialAccMs = 0;
/* Global Variable define of  */
bool sysml_state_dhtTempValidLatched = false;
/* Global Variable define of  */
bool sysml_state_dhtHumValidLatched = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayMode = 0;
    sysml_state_previousPressed = false;
    sysml_state_serialAccMs = 0;
    sysml_state_dhtTempValidLatched = false;
    sysml_state_dhtHumValidLatched = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, double io_3, bool io_4, int io_6, int* io_5, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, bool* serial_1_trigger, bool* serial_1_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    double sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_conditional_26 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_displayMode == 0) {
            sysml_conditional_1 = 1;
        }
        else {
            if (sysml_state_displayMode == 1) {
                sysml_conditional_2 = 2;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_displayMode = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_1;
    sysml_state_serialAccMs = (sysml_state_serialAccMs + io_6);
    if ((sysml_state_serialAccMs >= 2000)) {
        sysml_state_serialAccMs = (sysml_state_serialAccMs - 2000);
    }
    if (io_4) {
        sysml_state_dhtTempValidLatched = true;
        sysml_state_dhtHumValidLatched = true;
    }
    int sysml_value_0 = 0;
    if (sysml_state_displayMode == 0) {
        if (io_0 < 0.0) {
            sysml_conditional_4 = 0;
        }
        else {
            if (io_0 > 2147483647.0) {
                sysml_conditional_5 = 2147483647;
            }
            else {
                if (io_0 < ((double)((int)io_0))) {
                    sysml_conditional_6 = 1;
                }
                else {
                    sysml_conditional_6 = 0;
                }
                sysml_conditional_5 = ((int)io_0) - sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        if (sysml_state_displayMode == 1) {
            if (sysml_state_dhtTempValidLatched) {
                if (io_2 < 0.0) {
                    sysml_conditional_9 = 0;
                }
                else {
                    if (io_2 > 2147483647.0) {
                        sysml_conditional_10 = 2147483647;
                    }
                    else {
                        if (io_2 < ((double)((int)io_2))) {
                            sysml_conditional_11 = 1;
                        }
                        else {
                            sysml_conditional_11 = 0;
                        }
                        sysml_conditional_10 = ((int)io_2) - sysml_conditional_11;
                    }
                    sysml_conditional_9 = sysml_conditional_10;
                }
                sysml_conditional_8 = sysml_conditional_9;
            }
            else {
                sysml_conditional_8 = 0;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        else {
            if (sysml_state_dhtHumValidLatched) {
                if (io_3 < 0.0) {
                    sysml_conditional_13 = 0;
                }
                else {
                    if (io_3 > 2147483647.0) {
                        sysml_conditional_14 = 2147483647;
                    }
                    else {
                        if (io_3 < ((double)((int)io_3))) {
                            sysml_conditional_15 = 1;
                        }
                        else {
                            sysml_conditional_15 = 0;
                        }
                        sysml_conditional_14 = ((int)io_3) - sysml_conditional_15;
                    }
                    sysml_conditional_13 = sysml_conditional_14;
                }
                sysml_conditional_12 = sysml_conditional_13;
            }
            else {
                sysml_conditional_12 = 0;
            }
            sysml_conditional_7 = sysml_conditional_12;
        }
        sysml_conditional_3 = sysml_conditional_7;
    }
    sysml_value_0 = sysml_conditional_3;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_serialAccMs >= 0);
    double sysml_value_2 = 0;
    if (sysml_state_serialAccMs >= 0) {
        sysml_conditional_16 = io_0;
    }
    else {
        sysml_conditional_16 = 0.0;
    }
    sysml_value_2 = sysml_conditional_16;
    double sysml_value_3 = 0;
    if (sysml_state_serialAccMs >= 0) {
        if (sysml_state_dhtTempValidLatched) {
            sysml_conditional_18 = io_2;
        }
        else {
            sysml_conditional_18 = 0.0;
        }
        sysml_conditional_17 = sysml_conditional_18;
    }
    else {
        sysml_conditional_17 = 0.0;
    }
    sysml_value_3 = sysml_conditional_17;
    double sysml_value_4 = 0;
    if (sysml_state_serialAccMs >= 0) {
        if (sysml_state_dhtHumValidLatched) {
            sysml_conditional_20 = io_3;
        }
        else {
            sysml_conditional_20 = 0.0;
        }
        sysml_conditional_19 = sysml_conditional_20;
    }
    else {
        sysml_conditional_19 = 0.0;
    }
    sysml_value_4 = sysml_conditional_19;
    bool sysml_value_5 = 0;
    sysml_lazy_21 = !sysml_state_dhtTempValidLatched;
    if (!sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_dhtTempValidLatched;
    }
    sysml_lazy_22 = sysml_lazy_21;
    if (sysml_lazy_22) {
        sysml_lazy_23 = !sysml_state_dhtHumValidLatched;
        if (!sysml_lazy_23) {
            sysml_lazy_23 = sysml_state_dhtHumValidLatched;
        }
        sysml_lazy_22 = sysml_lazy_23;
    }
    sysml_lazy_24 = sysml_lazy_22;
    if (sysml_lazy_24) {
        sysml_lazy_25 = io_0 < 300.0;
        if (sysml_lazy_25) {
            sysml_lazy_25 = io_2 > 30.0;
        }
        sysml_lazy_24 = sysml_lazy_25;
    }
    sysml_value_5 = sysml_lazy_24;
    bool sysml_value_6 = 0;
    if (sysml_value_5) {
        sysml_conditional_26 = true;
    }
    else {
        sysml_conditional_26 = false;
    }
    sysml_value_6 = sysml_conditional_26;
    *io_5 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
    *serial_1_trigger = sysml_value_5;
    *serial_1_value_0 = sysml_value_6;
}