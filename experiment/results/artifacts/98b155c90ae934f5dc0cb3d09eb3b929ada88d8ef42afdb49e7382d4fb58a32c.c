#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevVibration = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevVibration = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double* io_3, int* io_4, bool* io_5, int* io_6, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, int* serial_0_value_2, bool* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    sysml_state_prevVibration = io_2;
    double sysml_value_0 = 0;
    if (true) {
        if (io_2) {
            sysml_conditional_1 = 255.0;
        }
        else {
            if (io_1 >= 600.0) {
                sysml_conditional_2 = 200.0;
            }
            else {
                sysml_conditional_2 = 60.0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    int sysml_value_1 = 0;
    if (io_2) {
        sysml_conditional_3 = 2000;
    }
    else {
        if (io_1 >= 600.0) {
            sysml_conditional_4 = 1200;
        }
        else {
            if (io_0 >= 800.0) {
                sysml_conditional_5 = 800;
            }
            else {
                sysml_conditional_5 = 0;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    sysml_value_1 = sysml_conditional_3;
    bool sysml_value_2 = 0;
    if (io_2) {
        sysml_conditional_6 = true;
    }
    else {
        if (io_1 >= 600.0) {
            sysml_conditional_7 = true;
        }
        else {
            if (io_0 >= 800.0) {
                sysml_conditional_8 = true;
            }
            else {
                sysml_conditional_8 = false;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_2 = sysml_conditional_6;
    int sysml_value_3 = 0;
    if (io_2) {
        sysml_conditional_9 = 4;
    }
    else {
        if (io_1 >= 600.0) {
            sysml_conditional_10 = 3;
        }
        else {
            if (io_0 >= 800.0) {
                sysml_conditional_11 = 2;
            }
            else {
                sysml_conditional_11 = 1;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_3 = sysml_conditional_9;
    bool sysml_value_4 = 0;
    sysml_value_4 = true;
    double sysml_value_5 = 0;
    sysml_value_5 = io_0;
    double sysml_value_6 = 0;
    sysml_value_6 = io_1;
    int sysml_value_7 = 0;
    if (io_2) {
        sysml_conditional_12 = 4;
    }
    else {
        if (io_1 >= 600.0) {
            sysml_conditional_13 = 3;
        }
        else {
            if (io_0 >= 800.0) {
                sysml_conditional_14 = 2;
            }
            else {
                sysml_conditional_14 = 1;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_7 = sysml_conditional_12;
    bool sysml_value_8 = 0;
    sysml_value_8 = io_2;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
    *serial_0_value_2 = sysml_value_7;
    *serial_0_value_3 = sysml_value_8;
}