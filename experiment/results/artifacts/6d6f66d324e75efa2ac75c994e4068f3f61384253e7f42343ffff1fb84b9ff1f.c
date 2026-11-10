#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_roomIndex = 0;
/* Global Variable define of  */
int sysml_state_displayMode = 0;
/* Global Variable define of  */
bool sysml_state_prevBtnRoom = false;
/* Global Variable define of  */
bool sysml_state_prevBtnMode = false;
/* Global Variable define of  */
int sysml_state_serialAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_nextSerialReady = false;
/* Global Variable define of  */
int sysml_state_tm1637NumberLatched = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_roomIndex = 0;
    sysml_state_displayMode = 0;
    sysml_state_prevBtnRoom = false;
    sysml_state_prevBtnMode = false;
    sysml_state_serialAccumMs = 0;
    sysml_state_nextSerialReady = false;
    sysml_state_tm1637NumberLatched = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, double io_3, bool io_4, double io_5, double io_6, bool io_7, bool io_8, bool io_9, int io_13, bool* io_10, bool* io_11, int* io_12, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, double* serial_0_value_3, double* serial_0_value_4, double* serial_0_value_5, bool* serial_0_value_6, bool* serial_0_value_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
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
    bool sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    double sysml_conditional_21 = 0;
    double sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    sysml_lazy_0 = io_8;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtnRoom;
    }
    if (sysml_lazy_0) {
        if (sysml_state_roomIndex == 0) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_state_roomIndex = sysml_conditional_1;
    }
    sysml_lazy_2 = io_9;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevBtnMode;
    }
    if (sysml_lazy_2) {
        if (sysml_state_displayMode == 0) {
            sysml_conditional_3 = 1;
        }
        else {
            if (sysml_state_displayMode == 1) {
                sysml_conditional_4 = 2;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        sysml_state_displayMode = sysml_conditional_3;
    }
    sysml_state_prevBtnRoom = io_8;
    sysml_state_prevBtnMode = io_9;
    sysml_state_serialAccumMs = (sysml_state_serialAccumMs + io_13);
    if ((sysml_state_serialAccumMs >= 5000)) {
        sysml_state_serialAccumMs = (sysml_state_serialAccumMs - 5000);
        sysml_state_nextSerialReady = true;
    }
    else {
        sysml_state_nextSerialReady = false;
    }
    if ((sysml_state_roomIndex == 0)) {
        if (sysml_state_displayMode == 0) {
            if (io_2 < ((double)((int)io_2))) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = 0;
            }
            sysml_conditional_5 = ((int)io_2) - sysml_conditional_6;
        }
        else {
            if (sysml_state_displayMode == 1) {
                if (io_3 < ((double)((int)io_3))) {
                    sysml_conditional_8 = 1;
                }
                else {
                    sysml_conditional_8 = 0;
                }
                sysml_conditional_7 = ((int)io_3) - sysml_conditional_8;
            }
            else {
                if (io_0 < ((double)((int)io_0))) {
                    sysml_conditional_9 = 1;
                }
                else {
                    sysml_conditional_9 = 0;
                }
                sysml_conditional_7 = ((int)io_0) - sysml_conditional_9;
            }
            sysml_conditional_5 = sysml_conditional_7;
        }
        sysml_state_tm1637NumberLatched = sysml_conditional_5;
    }
    else {
        if (sysml_state_displayMode == 0) {
            if (io_5 < ((double)((int)io_5))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_10 = ((int)io_5) - sysml_conditional_11;
        }
        else {
            if (sysml_state_displayMode == 1) {
                if (io_6 < ((double)((int)io_6))) {
                    sysml_conditional_13 = 1;
                }
                else {
                    sysml_conditional_13 = 0;
                }
                sysml_conditional_12 = ((int)io_6) - sysml_conditional_13;
            }
            else {
                if (io_1 < ((double)((int)io_1))) {
                    sysml_conditional_14 = 1;
                }
                else {
                    sysml_conditional_14 = 0;
                }
                sysml_conditional_12 = ((int)io_1) - sysml_conditional_14;
            }
            sysml_conditional_10 = sysml_conditional_12;
        }
        sysml_state_tm1637NumberLatched = sysml_conditional_10;
    }
    bool sysml_value_0 = 0;
    if (io_4) {
        sysml_lazy_16 = io_0 >= 700.0;
        if (sysml_lazy_16) {
            sysml_lazy_16 = io_2 >= 30.0;
        }
        sysml_conditional_15 = sysml_lazy_16;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_0 = sysml_conditional_15;
    bool sysml_value_1 = 0;
    if (io_7) {
        sysml_lazy_18 = io_1 >= 700.0;
        if (sysml_lazy_18) {
            sysml_lazy_18 = io_5 >= 30.0;
        }
        sysml_conditional_17 = sysml_lazy_18;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_1 = sysml_conditional_17;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_tm1637NumberLatched;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_nextSerialReady;
    double sysml_value_4 = 0;
    if (io_4) {
        sysml_conditional_19 = io_2;
    }
    else {
        sysml_conditional_19 = io_2;
    }
    sysml_value_4 = sysml_conditional_19;
    double sysml_value_5 = 0;
    if (io_4) {
        sysml_conditional_20 = io_3;
    }
    else {
        sysml_conditional_20 = io_3;
    }
    sysml_value_5 = sysml_conditional_20;
    double sysml_value_6 = 0;
    sysml_value_6 = io_0;
    double sysml_value_7 = 0;
    if (io_7) {
        sysml_conditional_21 = io_5;
    }
    else {
        sysml_conditional_21 = io_5;
    }
    sysml_value_7 = sysml_conditional_21;
    double sysml_value_8 = 0;
    if (io_7) {
        sysml_conditional_22 = io_6;
    }
    else {
        sysml_conditional_22 = io_6;
    }
    sysml_value_8 = sysml_conditional_22;
    double sysml_value_9 = 0;
    sysml_value_9 = io_1;
    bool sysml_value_10 = 0;
    sysml_lazy_23 = io_4;
    if (sysml_lazy_23) {
        sysml_lazy_24 = io_2 >= 30.0;
        if (sysml_lazy_24) {
            sysml_lazy_24 = io_0 >= 700.0;
        }
        sysml_lazy_23 = sysml_lazy_24;
    }
    sysml_value_10 = sysml_lazy_23;
    bool sysml_value_11 = 0;
    sysml_lazy_25 = io_7;
    if (sysml_lazy_25) {
        sysml_lazy_26 = io_5 >= 30.0;
        if (sysml_lazy_26) {
            sysml_lazy_26 = io_1 >= 700.0;
        }
        sysml_lazy_25 = sysml_lazy_26;
    }
    sysml_value_11 = sysml_lazy_25;
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
    *serial_0_value_6 = sysml_value_10;
    *serial_0_value_7 = sysml_value_11;
}