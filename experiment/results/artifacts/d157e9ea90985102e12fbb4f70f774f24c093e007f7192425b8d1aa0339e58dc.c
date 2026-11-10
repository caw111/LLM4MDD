#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_displayRoomIndex = 0;
/* Global Variable define of  */
int sysml_state_displayModeIndex = 0;
/* Global Variable define of  */
int sysml_state_reportAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_prevButton1 = false;
/* Global Variable define of  */
bool sysml_state_prevButton2 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayRoomIndex = 0;
    sysml_state_displayModeIndex = 0;
    sysml_state_reportAccumMs = 0;
    sysml_state_prevButton1 = false;
    sysml_state_prevButton2 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, double io_4, bool io_5, double io_6, double io_7, bool io_8, bool io_9, int io_13, bool* io_10, bool* io_11, int* io_12, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, double* serial_0_value_3, double* serial_0_value_4, double* serial_0_value_5, bool* serial_1_trigger, bool* serial_1_value_0, bool* serial_1_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    int sysml_conditional_19 = 0;
    int sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    double sysml_conditional_22 = 0;
    double sysml_conditional_23 = 0;
    double sysml_conditional_24 = 0;
    double sysml_conditional_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_conditional_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_conditional_33 = 0;
    bool sysml_lazy_34 = 0;
    sysml_state_reportAccumMs = (sysml_state_reportAccumMs + io_13);
    sysml_lazy_0 = io_8;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevButton1;
    }
    if (sysml_lazy_0) {
        sysml_state_displayRoomIndex = (1 - sysml_state_displayRoomIndex);
    }
    sysml_lazy_1 = io_9;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevButton2;
    }
    if (sysml_lazy_1) {
        sysml_state_displayModeIndex = (1 - sysml_state_displayModeIndex);
    }
    if ((sysml_state_reportAccumMs >= 5000)) {
        sysml_state_reportAccumMs = (sysml_state_reportAccumMs - 5000);
    }
    sysml_state_prevButton1 = io_8;
    sysml_state_prevButton2 = io_9;
    bool sysml_value_0 = 0;
    sysml_lazy_2 = io_2;
    if (sysml_lazy_2) {
        sysml_lazy_3 = io_6 >= 600.0;
        if (sysml_lazy_3) {
            sysml_lazy_3 = io_0 >= 30.0;
        }
        sysml_lazy_2 = sysml_lazy_3;
    }
    if (sysml_lazy_2) {
        sysml_lazy_5 = io_0 >= 30.0;
        if (sysml_lazy_5) {
            sysml_lazy_5 = io_6 >= 600.0;
        }
        sysml_conditional_4 = sysml_lazy_5;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_0 = sysml_conditional_4;
    bool sysml_value_1 = 0;
    sysml_lazy_6 = io_5;
    if (sysml_lazy_6) {
        sysml_lazy_7 = io_7 >= 600.0;
        if (sysml_lazy_7) {
            sysml_lazy_7 = io_3 >= 30.0;
        }
        sysml_lazy_6 = sysml_lazy_7;
    }
    if (sysml_lazy_6) {
        sysml_lazy_9 = io_3 >= 30.0;
        if (sysml_lazy_9) {
            sysml_lazy_9 = io_7 >= 600.0;
        }
        sysml_conditional_8 = sysml_lazy_9;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_1 = sysml_conditional_8;
    int sysml_value_2 = 0;
    if (sysml_state_displayRoomIndex == 0) {
        if (sysml_state_displayModeIndex == 0) {
            if (io_2) {
                if (io_0 < ((double)((int)io_0))) {
                    sysml_conditional_13 = 1;
                }
                else {
                    sysml_conditional_13 = 0;
                }
                sysml_conditional_12 = ((int)io_0) - sysml_conditional_13;
            }
            else {
                sysml_conditional_12 = 0;
            }
            sysml_conditional_11 = sysml_conditional_12;
        }
        else {
            if (io_2) {
                if (io_1 < ((double)((int)io_1))) {
                    sysml_conditional_15 = 1;
                }
                else {
                    sysml_conditional_15 = 0;
                }
                sysml_conditional_14 = ((int)io_1) - sysml_conditional_15;
            }
            else {
                sysml_conditional_14 = 0;
            }
            sysml_conditional_11 = sysml_conditional_14;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    else {
        if (sysml_state_displayModeIndex == 0) {
            if (io_5) {
                if (io_3 < ((double)((int)io_3))) {
                    sysml_conditional_18 = 1;
                }
                else {
                    sysml_conditional_18 = 0;
                }
                sysml_conditional_17 = ((int)io_3) - sysml_conditional_18;
            }
            else {
                sysml_conditional_17 = 0;
            }
            sysml_conditional_16 = sysml_conditional_17;
        }
        else {
            if (io_5) {
                if (io_4 < ((double)((int)io_4))) {
                    sysml_conditional_20 = 1;
                }
                else {
                    sysml_conditional_20 = 0;
                }
                sysml_conditional_19 = ((int)io_4) - sysml_conditional_20;
            }
            else {
                sysml_conditional_19 = 0;
            }
            sysml_conditional_16 = sysml_conditional_19;
        }
        sysml_conditional_10 = sysml_conditional_16;
    }
    sysml_value_2 = sysml_conditional_10;
    bool sysml_value_3 = 0;
    sysml_lazy_21 = sysml_state_reportAccumMs < 5000;
    if (sysml_lazy_21) {
        sysml_lazy_21 = (sysml_state_reportAccumMs + io_13) >= 5000;
    }
    sysml_value_3 = sysml_lazy_21;
    double sysml_value_4 = 0;
    if (io_2) {
        sysml_conditional_22 = io_0;
    }
    else {
        sysml_conditional_22 = 0.0;
    }
    sysml_value_4 = sysml_conditional_22;
    double sysml_value_5 = 0;
    if (io_2) {
        sysml_conditional_23 = io_1;
    }
    else {
        sysml_conditional_23 = 0.0;
    }
    sysml_value_5 = sysml_conditional_23;
    double sysml_value_6 = 0;
    sysml_value_6 = io_6;
    double sysml_value_7 = 0;
    if (io_5) {
        sysml_conditional_24 = io_3;
    }
    else {
        sysml_conditional_24 = 0.0;
    }
    sysml_value_7 = sysml_conditional_24;
    double sysml_value_8 = 0;
    if (io_5) {
        sysml_conditional_25 = io_4;
    }
    else {
        sysml_conditional_25 = 0.0;
    }
    sysml_value_8 = sysml_conditional_25;
    double sysml_value_9 = 0;
    sysml_value_9 = io_7;
    bool sysml_value_10 = 0;
    sysml_lazy_26 = io_2;
    if (sysml_lazy_26) {
        sysml_lazy_26 = io_0 >= 30.0;
    }
    sysml_lazy_27 = sysml_lazy_26;
    if (sysml_lazy_27) {
        sysml_lazy_27 = io_6 >= 600.0;
    }
    sysml_lazy_28 = sysml_lazy_27;
    if (!sysml_lazy_28) {
        sysml_lazy_29 = io_5;
        if (sysml_lazy_29) {
            sysml_lazy_29 = io_3 >= 30.0;
        }
        sysml_lazy_30 = sysml_lazy_29;
        if (sysml_lazy_30) {
            sysml_lazy_30 = io_7 >= 600.0;
        }
        sysml_lazy_28 = sysml_lazy_30;
    }
    sysml_value_10 = sysml_lazy_28;
    bool sysml_value_11 = 0;
    if (io_2) {
        sysml_lazy_32 = io_0 >= 30.0;
        if (sysml_lazy_32) {
            sysml_lazy_32 = io_6 >= 600.0;
        }
        sysml_conditional_31 = sysml_lazy_32;
    }
    else {
        sysml_conditional_31 = false;
    }
    sysml_value_11 = sysml_conditional_31;
    bool sysml_value_12 = 0;
    if (io_5) {
        sysml_lazy_34 = io_3 >= 30.0;
        if (sysml_lazy_34) {
            sysml_lazy_34 = io_7 >= 600.0;
        }
        sysml_conditional_33 = sysml_lazy_34;
    }
    else {
        sysml_conditional_33 = false;
    }
    sysml_value_12 = sysml_conditional_33;
    *io_10 = sysml_value_0;
    *io_11 = sysml_value_1;
    *io_12 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
    *serial_0_value_4 = sysml_value_8;
    *serial_0_value_5 = sysml_value_9;
    *serial_1_trigger = sysml_value_10;
    *serial_1_value_0 = sysml_value_11;
    *serial_1_value_1 = sysml_value_12;
}