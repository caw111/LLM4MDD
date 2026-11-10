#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_displayHum = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayHum = false;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, bool io_5, double* io_3, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_arithmetic_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    sysml_lazy_0 = io_5;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_displayHum = (!sysml_state_displayHum);
    }
    sysml_state_previousPressed = io_5;
    double sysml_value_0 = 0;
    if (io_0 >= 30.0) {
        if (io_0 <= 30.0) {
            sysml_conditional_2 = 0.0;
        }
        else {
            if (io_0 < 60.0) {
                sysml_arithmetic_4 = ((double)30.0) / 1.0;
                sysml_conditional_3 = ((io_0 - 30.0) * 255.0) * sysml_arithmetic_4;
            }
            else {
                sysml_conditional_3 = 255.0;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    else {
        sysml_conditional_1 = 0.0;
    }
    sysml_value_0 = sysml_conditional_1;
    int sysml_value_1 = 0;
    if (sysml_state_displayHum) {
        if (io_2) {
            if (io_1 < ((double)((int)io_1))) {
                sysml_conditional_7 = 1;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_conditional_6 = ((int)io_1) - sysml_conditional_7;
        }
        else {
            if (io_1 < ((double)((int)io_1))) {
                sysml_conditional_8 = 1;
            }
            else {
                sysml_conditional_8 = 0;
            }
            sysml_conditional_6 = ((int)io_1) - sysml_conditional_8;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    else {
        if (io_2) {
            if (io_0 < ((double)((int)io_0))) {
                sysml_conditional_10 = 1;
            }
            else {
                sysml_conditional_10 = 0;
            }
            sysml_conditional_9 = ((int)io_0) - sysml_conditional_10;
        }
        else {
            if (io_0 < ((double)((int)io_0))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_9 = ((int)io_0) - sysml_conditional_11;
        }
        sysml_conditional_5 = sysml_conditional_9;
    }
    sysml_value_1 = sysml_conditional_5;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
}