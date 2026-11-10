#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevDoorOpen = false;
/* Global Variable define of  */
bool sysml_state_doorOpenRising = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevDoorOpen = false;
    sysml_state_doorOpenRising = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, bool* io_3, bool* io_4, int* io_5, int* io_6, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2, int* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevDoorOpen;
    }
    sysml_state_doorOpenRising = sysml_lazy_0;
    sysml_state_prevDoorOpen = io_2;
    bool sysml_value_0 = 0;
    sysml_value_0 = (io_0 < 30.0);
    bool sysml_value_1 = 0;
    sysml_lazy_1 = io_1 >= 4.0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_1 < 6.0;
    }
    sysml_value_1 = sysml_lazy_1;
    int sysml_value_2 = 0;
    if (io_0 >= 30.0) {
        sysml_conditional_2 = 2000;
    }
    else {
        if (io_1 >= 6.0) {
            sysml_conditional_3 = 2500;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    sysml_value_2 = sysml_conditional_2;
    int sysml_value_3 = 0;
    if (io_0 >= 30.0) {
        sysml_conditional_4 = 3;
    }
    else {
        if (io_1 >= 6.0) {
            sysml_conditional_5 = 2;
        }
        else {
            if (io_1 >= 4.0) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = 0;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    sysml_value_3 = sysml_conditional_4;
    bool sysml_value_4 = 0;
    sysml_lazy_7 = io_0 != 0.0;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = io_1 != 0.0;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_doorOpenRising;
    }
    sysml_value_4 = sysml_lazy_8;
    double sysml_value_5 = 0;
    sysml_value_5 = io_0;
    double sysml_value_6 = 0;
    sysml_value_6 = io_1;
    bool sysml_value_7 = 0;
    sysml_value_7 = io_2;
    int sysml_value_8 = 0;
    if (io_0 >= 30.0) {
        sysml_conditional_9 = 3;
    }
    else {
        if (io_1 >= 6.0) {
            sysml_conditional_10 = 2;
        }
        else {
            if (io_1 >= 4.0) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_8 = sysml_conditional_9;
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