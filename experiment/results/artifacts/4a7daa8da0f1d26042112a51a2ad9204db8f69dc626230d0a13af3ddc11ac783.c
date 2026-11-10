#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_autoManual = false;
/* Global Variable define of  */
bool sysml_state_prevButton = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_autoManual = false;
    sysml_state_prevButton = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_6, double* io_3, int* io_4, int* io_5, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    double sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    double sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    sysml_lazy_0 = io_6;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevButton;
    }
    if (sysml_lazy_0) {
        sysml_state_autoManual = (!sysml_state_autoManual);
    }
    sysml_state_prevButton = io_6;
    if (sysml_state_autoManual) {
        sysml_state_prevButton = io_6;
    }
    else {
        sysml_state_prevButton = io_6;
    }
    double sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_autoManual;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_0 > io_1;
    }
    if (sysml_lazy_1) {
        sysml_conditional_2 = 90.0;
    }
    else {
        sysml_lazy_3 = sysml_state_autoManual;
        if (sysml_lazy_3) {
            sysml_lazy_3 = io_1 > io_0;
        }
        if (sysml_lazy_3) {
            sysml_conditional_4 = 0.0;
        }
        else {
            sysml_conditional_4 = 45.0;
        }
        sysml_conditional_2 = sysml_conditional_4;
    }
    sysml_value_0 = sysml_conditional_2;
    int sysml_value_1 = 0;
    if (sysml_state_autoManual) {
        if (io_0 > io_1) {
            sysml_conditional_6 = 2;
        }
        else {
            if (io_0 < io_1) {
                sysml_conditional_7 = 1;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_1 = sysml_conditional_5;
    int sysml_value_2 = 0;
    if (io_2 > 600.0) {
        sysml_conditional_8 = 0;
    }
    else {
        sysml_conditional_8 = 2000;
    }
    sysml_value_2 = sysml_conditional_8;
    bool sysml_value_3 = 0;
    if (io_2 > 600.0) {
        sysml_conditional_9 = false;
    }
    else {
        sysml_conditional_9 = true;
    }
    sysml_value_3 = sysml_conditional_9;
    int sysml_value_4 = 0;
    if (io_2 > 600.0) {
        sysml_conditional_10 = 0;
    }
    else {
        sysml_conditional_10 = 1;
    }
    sysml_value_4 = sysml_conditional_10;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
}