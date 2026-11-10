#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode = 0;
/* Global Variable define of  */
int sysml_state_occupancyCount = 0;
/* Global Variable define of  */
bool sysml_state_prevParkingValid = false;
/* Global Variable define of  */
bool sysml_state_prevEntranceApproach = false;
/* Global Variable define of  */
bool sysml_state_prevReverseAlarmCond = false;
/* Global Variable define of  */
bool sysml_state_lastAlarmOn = false;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_occupancyCount = 0;
    sysml_state_prevParkingValid = false;
    sysml_state_prevEntranceApproach = false;
    sysml_state_prevReverseAlarmCond = false;
    sysml_state_lastAlarmOn = false;
    sysml_state_prevPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_4, bool io_5, double io_6, bool io_7, bool* io_1, bool* io_2, int* io_3, int* io_8) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    int sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    int sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    int sysml_conditional_19 = 0;
    bool sysml_lazy_20 = 0;
    int sysml_conditional_21 = 0;
    int sysml_conditional_22 = 0;
    int sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    int sysml_conditional_25 = 0;
    int sysml_conditional_26 = 0;
    int sysml_conditional_27 = 0;
    int sysml_conditional_28 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_mode == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_mode + 1;
        }
        sysml_state_mode = sysml_conditional_1;
    }
    sysml_state_prevPressed = io_0;
    sysml_lazy_2 = io_7;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevParkingValid;
    }
    if (sysml_lazy_2) {
        if (sysml_state_occupancyCount < 2147483647) {
            sysml_conditional_3 = sysml_state_occupancyCount + 1;
        }
        else {
            sysml_conditional_3 = 2147483647;
        }
        sysml_state_occupancyCount = sysml_conditional_3;
    }
    sysml_state_prevParkingValid = io_7;
    sysml_lazy_4 = io_5;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevEntranceApproach;
    }
    if (sysml_lazy_4) {
        sysml_state_prevEntranceApproach = true;
    }
    else {
        sysml_state_prevEntranceApproach = io_5;
    }
    sysml_lazy_5 = io_7;
    if (sysml_lazy_5) {
        sysml_lazy_5 = io_6 < 5.0;
    }
    sysml_state_prevReverseAlarmCond = sysml_lazy_5;
    sysml_lazy_6 = io_7;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_6 < 5.0;
    }
    sysml_state_lastAlarmOn = sysml_lazy_6;
    bool sysml_value_0 = 0;
    sysml_lazy_7 = !io_7;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = io_6 >= 5.0;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_5;
    }
    sysml_value_0 = sysml_lazy_8;
    bool sysml_value_1 = 0;
    sysml_lazy_9 = io_7;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_6 >= 5.0;
    }
    sysml_value_1 = sysml_lazy_9;
    int sysml_value_2 = 0;
    sysml_lazy_10 = io_7;
    if (sysml_lazy_10) {
        sysml_lazy_10 = io_6 < 5.0;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = 2000;
    }
    else {
        sysml_lazy_12 = io_7;
        if (sysml_lazy_12) {
            sysml_lazy_12 = io_6 >= 2.0;
        }
        sysml_lazy_13 = sysml_lazy_12;
        if (sysml_lazy_13) {
            sysml_lazy_13 = io_6 < 10.0;
        }
        if (sysml_lazy_13) {
            sysml_conditional_14 = 1200;
        }
        else {
            sysml_lazy_15 = io_7;
            if (sysml_lazy_15) {
                sysml_lazy_15 = io_6 >= 10.0;
            }
            sysml_lazy_16 = sysml_lazy_15;
            if (sysml_lazy_16) {
                sysml_lazy_16 = io_6 < 30.0;
            }
            if (sysml_lazy_16) {
                sysml_conditional_17 = 800;
            }
            else {
                sysml_lazy_18 = io_7;
                if (sysml_lazy_18) {
                    sysml_lazy_18 = io_6 >= 30.0;
                }
                if (sysml_lazy_18) {
                    sysml_conditional_19 = 400;
                }
                else {
                    sysml_conditional_19 = 0;
                }
                sysml_conditional_17 = sysml_conditional_19;
            }
            sysml_conditional_14 = sysml_conditional_17;
        }
        sysml_conditional_11 = sysml_conditional_14;
    }
    sysml_value_2 = sysml_conditional_11;
    int sysml_value_3 = 0;
    sysml_lazy_20 = sysml_state_mode == 0;
    if (sysml_lazy_20) {
        sysml_lazy_20 = io_5;
    }
    if (sysml_lazy_20) {
        if (io_4 >= 0.0) {
            if (io_4 < ((double)((int)io_4))) {
                sysml_conditional_23 = 1;
            }
            else {
                sysml_conditional_23 = 0;
            }
            sysml_conditional_22 = ((int)io_4) - sysml_conditional_23;
        }
        else {
            sysml_conditional_22 = 0;
        }
        sysml_conditional_21 = sysml_conditional_22;
    }
    else {
        sysml_lazy_24 = sysml_state_mode == 1;
        if (sysml_lazy_24) {
            sysml_lazy_24 = io_7;
        }
        if (sysml_lazy_24) {
            if (io_6 >= 0.0) {
                if (io_6 < ((double)((int)io_6))) {
                    sysml_conditional_27 = 1;
                }
                else {
                    sysml_conditional_27 = 0;
                }
                sysml_conditional_26 = ((int)io_6) - sysml_conditional_27;
            }
            else {
                sysml_conditional_26 = 0;
            }
            sysml_conditional_25 = sysml_conditional_26;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_28 = sysml_state_occupancyCount;
            }
            else {
                sysml_conditional_28 = 0;
            }
            sysml_conditional_25 = sysml_conditional_28;
        }
        sysml_conditional_21 = sysml_conditional_25;
    }
    sysml_value_3 = sysml_conditional_21;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_8 = sysml_value_3;
}