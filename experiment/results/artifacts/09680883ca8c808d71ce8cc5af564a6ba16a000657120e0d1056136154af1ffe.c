#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_displayMode = 0;
/* Global Variable define of  */
int sysml_state_occupiedCount = 0;
/* Global Variable define of  */
bool sysml_state_prevInPositionOccupied = false;
/* Global Variable define of  */
bool sysml_state_prevOccupied = false;
/* Global Variable define of  */
double sysml_state_entryDistanceCm = 0.0;
/* Global Variable define of  */
double sysml_state_parkingDistanceCm = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_displayMode = 0;
    sysml_state_occupiedCount = 0;
    sysml_state_prevInPositionOccupied = false;
    sysml_state_prevOccupied = false;
    sysml_state_entryDistanceCm = 0.0;
    sysml_state_parkingDistanceCm = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, bool io_8, int io_9, int* io_4, bool* io_5, bool* io_6, int* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    int sysml_conditional_26 = 0;
    bool sysml_lazy_27 = 0;
    int sysml_conditional_28 = 0;
    int sysml_conditional_29 = 0;
    int sysml_conditional_30 = 0;
    if (io_1) {
        sysml_conditional_0 = io_0;
    }
    else {
        sysml_conditional_0 = sysml_state_entryDistanceCm;
    }
    sysml_state_entryDistanceCm = sysml_conditional_0;
    if (io_3) {
        sysml_conditional_1 = io_2;
    }
    else {
        sysml_conditional_1 = sysml_state_parkingDistanceCm;
    }
    sysml_state_parkingDistanceCm = sysml_conditional_1;
    sysml_lazy_2 = io_8;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = (sysml_state_displayMode + 1) + 0;
    }
    else {
        sysml_conditional_3 = sysml_state_displayMode;
    }
    sysml_state_displayMode = sysml_conditional_3;
    if ((sysml_state_displayMode >= 3)) {
        sysml_state_displayMode = 0;
    }
    sysml_lazy_4 = !sysml_state_prevInPositionOccupied;
    if (sysml_lazy_4) {
        sysml_lazy_5 = sysml_state_parkingDistanceCm <= 20.0;
        if (sysml_lazy_5) {
            sysml_lazy_5 = io_3;
        }
        sysml_lazy_4 = sysml_lazy_5;
    }
    if (sysml_lazy_4) {
        sysml_conditional_6 = sysml_state_occupiedCount + 1;
    }
    else {
        sysml_conditional_6 = sysml_state_occupiedCount;
    }
    sysml_state_occupiedCount = sysml_conditional_6;
    sysml_lazy_7 = sysml_state_parkingDistanceCm <= 20.0;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_3;
    }
    sysml_state_prevInPositionOccupied = sysml_lazy_7;
    sysml_lazy_8 = sysml_state_parkingDistanceCm <= 20.0;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_3;
    }
    sysml_state_prevOccupied = sysml_lazy_8;
    sysml_state_previousPressed = io_8;
    int sysml_value_0 = 0;
    if (sysml_state_displayMode == 0) {
        if (io_1) {
            if (sysml_state_entryDistanceCm < ((double)((int)sysml_state_entryDistanceCm))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_10 = ((int)sysml_state_entryDistanceCm) - sysml_conditional_11;
        }
        else {
            sysml_conditional_10 = 0;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    else {
        if (sysml_state_displayMode == 1) {
            if (io_3) {
                if (sysml_state_parkingDistanceCm < ((double)((int)sysml_state_parkingDistanceCm))) {
                    sysml_conditional_14 = 1;
                }
                else {
                    sysml_conditional_14 = 0;
                }
                sysml_conditional_13 = ((int)sysml_state_parkingDistanceCm) - sysml_conditional_14;
            }
            else {
                sysml_conditional_13 = 0;
            }
            sysml_conditional_12 = sysml_conditional_13;
        }
        else {
            if (sysml_state_displayMode == 2) {
                sysml_conditional_15 = sysml_state_occupiedCount;
            }
            else {
                sysml_conditional_15 = sysml_state_occupiedCount;
            }
            sysml_conditional_12 = sysml_conditional_15;
        }
        sysml_conditional_9 = sysml_conditional_12;
    }
    sysml_value_0 = sysml_conditional_9;
    bool sysml_value_1 = 0;
    sysml_lazy_16 = io_3;
    if (sysml_lazy_16) {
        sysml_lazy_16 = sysml_state_parkingDistanceCm < 5.0;
    }
    if (sysml_lazy_16) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_lazy_18 = io_3;
        if (sysml_lazy_18) {
            sysml_lazy_18 = sysml_state_parkingDistanceCm <= 20.0;
        }
        if (sysml_lazy_18) {
            sysml_lazy_20 = sysml_state_parkingDistanceCm >= 5.0;
            if (!sysml_lazy_20) {
                sysml_lazy_20 = true;
            }
            sysml_conditional_19 = sysml_lazy_20;
        }
        else {
            sysml_conditional_19 = false;
        }
        sysml_conditional_17 = sysml_conditional_19;
    }
    sysml_value_1 = sysml_conditional_17;
    bool sysml_value_2 = 0;
    sysml_lazy_21 = io_1;
    if (sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_entryDistanceCm < 30.0;
    }
    sysml_lazy_22 = sysml_lazy_21;
    if (sysml_lazy_22) {
        sysml_lazy_23 = io_3;
        if (sysml_lazy_23) {
            sysml_lazy_23 = sysml_state_parkingDistanceCm <= 20.0;
        }
        sysml_lazy_22 = !sysml_lazy_23;
    }
    if (sysml_lazy_22) {
        sysml_conditional_24 = true;
    }
    else {
        sysml_conditional_24 = false;
    }
    sysml_value_2 = sysml_conditional_24;
    int sysml_value_3 = 0;
    sysml_lazy_25 = io_3;
    if (sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_parkingDistanceCm < 5.0;
    }
    if (sysml_lazy_25) {
        sysml_conditional_26 = 2000;
    }
    else {
        sysml_lazy_27 = io_3;
        if (sysml_lazy_27) {
            sysml_lazy_27 = sysml_state_parkingDistanceCm <= 20.0;
        }
        if (sysml_lazy_27) {
            if (sysml_state_parkingDistanceCm < 10.0) {
                sysml_conditional_29 = 1800;
            }
            else {
                if (sysml_state_parkingDistanceCm < 15.0) {
                    sysml_conditional_30 = 1200;
                }
                else {
                    sysml_conditional_30 = 700;
                }
                sysml_conditional_29 = sysml_conditional_30;
            }
            sysml_conditional_28 = sysml_conditional_29;
        }
        else {
            sysml_conditional_28 = 0;
        }
        sysml_conditional_26 = sysml_conditional_28;
    }
    sysml_value_3 = sysml_conditional_26;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *io_7 = sysml_value_3;
}