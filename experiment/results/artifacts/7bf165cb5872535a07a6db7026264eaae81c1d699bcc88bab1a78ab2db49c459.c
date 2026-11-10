#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_manualMode = false;
/* Global Variable define of  */
int sysml_state_frozenDisplayValue = 0;
/* Global Variable define of  */
double sysml_state_frozenBrightnessValue = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_manualMode = false;
    sysml_state_frozenDisplayValue = 0;
    sysml_state_frozenBrightnessValue = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_2, double* io_1, bool* io_3, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_manualMode = (!sysml_state_manualMode);
        if (sysml_state_manualMode) {
            if (io_0 < ((double)((int)io_0))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int)io_0) - sysml_conditional_2;
        }
        else {
            if (io_0 < ((double)((int)io_0))) {
                sysml_conditional_3 = 1;
            }
            else {
                sysml_conditional_3 = 0;
            }
            sysml_conditional_1 = ((int)io_0) - sysml_conditional_3;
        }
        sysml_state_frozenDisplayValue = sysml_conditional_1;
        if (sysml_state_manualMode) {
            if (io_0 >= 1023.0) {
                sysml_conditional_5 = 255.0;
            }
            else {
                if (io_0 <= 0.0) {
                    sysml_conditional_6 = 0.0;
                }
                else {
                    sysml_conditional_6 = ((double)(io_0 * 255.0)) / 1023.0;
                }
                sysml_conditional_5 = sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        else {
            if (io_0 >= 1023.0) {
                sysml_conditional_7 = 255.0;
            }
            else {
                if (io_0 <= 0.0) {
                    sysml_conditional_8 = 0.0;
                }
                else {
                    sysml_conditional_8 = ((double)(io_0 * 255.0)) / 1023.0;
                }
                sysml_conditional_7 = sysml_conditional_8;
            }
            sysml_conditional_4 = sysml_conditional_7;
        }
        sysml_state_frozenBrightnessValue = sysml_conditional_4;
        sysml_state_previousPressed = true;
    }
    else {
        sysml_state_previousPressed = io_2;
    }
    double sysml_value_0 = 0;
    if (sysml_state_manualMode) {
        sysml_conditional_9 = sysml_state_frozenBrightnessValue;
    }
    else {
        if (io_0 >= 1023.0) {
            sysml_conditional_10 = 255.0;
        }
        else {
            if (io_0 <= 0.0) {
                sysml_conditional_11 = 0.0;
            }
            else {
                sysml_conditional_11 = ((double)(io_0 * 255.0)) / 1023.0;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_0 = sysml_conditional_9;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_manualMode;
    int sysml_value_2 = 0;
    if (sysml_state_manualMode) {
        sysml_conditional_12 = sysml_state_frozenDisplayValue;
    }
    else {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        sysml_conditional_12 = ((int)io_0) - sysml_conditional_13;
    }
    sysml_value_2 = sysml_conditional_12;
    *io_1 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}