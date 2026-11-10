#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_menuMode = 1;
/* Global Variable define of  */
bool sysml_state_serialEnabled = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_menuMode = 1;
    sysml_state_serialEnabled = false;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, bool io_3, int io_5, bool io_5_valid, int* io_4, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2, int* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    double sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    if (io_5_valid) {
        sysml_state_menuMode = io_5;
    }
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_serialEnabled) {
            sysml_conditional_1 = false;
        }
        else {
            sysml_conditional_1 = true;
        }
        sysml_state_serialEnabled = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_0;
    int sysml_value_0 = 0;
    if (sysml_state_menuMode == 3) {
        if (io_3) {
            if ((io_1 * 10.0) < ((double)((int)(io_1 * 10.0)))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = ((int)(io_1 * 10.0)) - sysml_conditional_4;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    else {
        if (sysml_state_menuMode == 2) {
            if (io_3) {
                if ((io_2 * 10.0) < ((double)((int)(io_2 * 10.0)))) {
                    sysml_conditional_7 = 1;
                }
                else {
                    sysml_conditional_7 = 0;
                }
                sysml_conditional_6 = ((int)(io_2 * 10.0)) - sysml_conditional_7;
            }
            else {
                sysml_conditional_6 = 0;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        else {
            if (io_3) {
                if ((io_1 * 10.0) < ((double)((int)(io_1 * 10.0)))) {
                    sysml_conditional_9 = 1;
                }
                else {
                    sysml_conditional_9 = 0;
                }
                sysml_conditional_8 = ((int)(io_1 * 10.0)) - sysml_conditional_9;
            }
            else {
                sysml_conditional_8 = 0;
            }
            sysml_conditional_5 = sysml_conditional_8;
        }
        sysml_conditional_2 = sysml_conditional_5;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    sysml_lazy_10 = sysml_state_serialEnabled;
    if (sysml_lazy_10) {
        sysml_lazy_10 = io_5_valid;
    }
    sysml_value_1 = sysml_lazy_10;
    double sysml_value_2 = 0;
    if (sysml_state_menuMode == 3) {
        if (io_3) {
            sysml_conditional_12 = io_1;
        }
        else {
            sysml_conditional_12 = 0.0;
        }
        sysml_conditional_11 = sysml_conditional_12;
    }
    else {
        if (sysml_state_menuMode == 1) {
            if (io_3) {
                sysml_conditional_14 = io_1;
            }
            else {
                sysml_conditional_14 = 0.0;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        else {
            sysml_conditional_13 = 0.0;
        }
        sysml_conditional_11 = sysml_conditional_13;
    }
    sysml_value_2 = sysml_conditional_11;
    double sysml_value_3 = 0;
    if (sysml_state_menuMode == 2) {
        if (io_3) {
            sysml_conditional_16 = io_2;
        }
        else {
            sysml_conditional_16 = 0.0;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    else {
        if (sysml_state_menuMode == 1) {
            if (io_3) {
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
        sysml_conditional_15 = sysml_conditional_17;
    }
    sysml_value_3 = sysml_conditional_15;
    bool sysml_value_4 = 0;
    if (io_3) {
        sysml_conditional_19 = true;
    }
    else {
        sysml_conditional_19 = false;
    }
    sysml_value_4 = sysml_conditional_19;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_menuMode;
    *io_4 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
    *serial_0_value_3 = sysml_value_5;
}