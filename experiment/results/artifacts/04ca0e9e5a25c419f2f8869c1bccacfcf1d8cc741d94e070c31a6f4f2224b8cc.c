#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_serialEnabled = false;
/* Global Variable define of  */
int sysml_state_mode = 1;
/* Global Variable define of  */
double sysml_state_dispTemp = 0.0;
/* Global Variable define of  */
double sysml_state_dispHumi = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_serialEnabled = false;
    sysml_state_mode = 1;
    sysml_state_dispTemp = 0.0;
    sysml_state_dispHumi = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, bool io_3, int io_5, bool io_5_valid, int* io_4, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    double sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    double sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_conditional_17 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_serialEnabled = (!sysml_state_serialEnabled);
    }
    sysml_state_previousPressed = io_0;
    if (io_5_valid) {
        sysml_state_mode = io_5;
    }
    if (io_3) {
        sysml_state_dispTemp = io_1;
        sysml_state_dispHumi = io_2;
    }
    int sysml_value_0 = 0;
    if (sysml_state_mode == 2) {
        sysml_lazy_2 = sysml_state_dispHumi >= 0.0;
        if (sysml_lazy_2) {
            sysml_lazy_2 = sysml_state_dispHumi <= 2147483647.0;
        }
        if (sysml_lazy_2) {
            if (sysml_state_dispHumi < ((double)((int)sysml_state_dispHumi))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = ((int)sysml_state_dispHumi) - sysml_conditional_4;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_conditional_1 = sysml_conditional_3;
    }
    else {
        if (sysml_state_mode == 3) {
            sysml_lazy_6 = sysml_state_dispTemp >= 0.0;
            if (sysml_lazy_6) {
                sysml_lazy_6 = sysml_state_dispTemp <= 2147483647.0;
            }
            if (sysml_lazy_6) {
                if (sysml_state_dispTemp < ((double)((int)sysml_state_dispTemp))) {
                    sysml_conditional_8 = 1;
                }
                else {
                    sysml_conditional_8 = 0;
                }
                sysml_conditional_7 = ((int)sysml_state_dispTemp) - sysml_conditional_8;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_conditional_5 = sysml_conditional_7;
        }
        else {
            sysml_lazy_9 = sysml_state_dispTemp >= 0.0;
            if (sysml_lazy_9) {
                sysml_lazy_9 = sysml_state_dispTemp <= 2147483647.0;
            }
            if (sysml_lazy_9) {
                if (sysml_state_dispTemp < ((double)((int)sysml_state_dispTemp))) {
                    sysml_conditional_11 = 1;
                }
                else {
                    sysml_conditional_11 = 0;
                }
                sysml_conditional_10 = ((int)sysml_state_dispTemp) - sysml_conditional_11;
            }
            else {
                sysml_conditional_10 = 0;
            }
            sysml_conditional_5 = sysml_conditional_10;
        }
        sysml_conditional_1 = sysml_conditional_5;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_serialEnabled;
    double sysml_value_2 = 0;
    sysml_lazy_12 = sysml_state_serialEnabled;
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_mode == 3;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = sysml_state_dispTemp;
    }
    else {
        sysml_conditional_13 = 0.0;
    }
    sysml_value_2 = sysml_conditional_13;
    double sysml_value_3 = 0;
    sysml_lazy_14 = sysml_state_serialEnabled;
    if (sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_mode == 2;
    }
    if (sysml_lazy_14) {
        sysml_conditional_15 = sysml_state_dispHumi;
    }
    else {
        sysml_conditional_15 = 0.0;
    }
    sysml_value_3 = sysml_conditional_15;
    bool sysml_value_4 = 0;
    sysml_lazy_16 = sysml_state_serialEnabled;
    if (sysml_lazy_16) {
        sysml_lazy_16 = sysml_state_mode == 1;
    }
    if (sysml_lazy_16) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_4 = sysml_conditional_17;
    *io_4 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
}