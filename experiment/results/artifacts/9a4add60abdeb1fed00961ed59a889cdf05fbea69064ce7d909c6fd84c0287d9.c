#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_displayMode = 0;
/* Global Variable define of  */
bool sysml_state_heaterEnabled = true;
/* Global Variable define of  */
bool sysml_state_aeratorEnabled = true;
/* Global Variable define of  */
bool sysml_state_alarmEnabled = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_displayMode = 0;
    sysml_state_heaterEnabled = true;
    sysml_state_aeratorEnabled = true;
    sysml_state_alarmEnabled = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_6, double* io_3, double* io_4, double* io_5, int* io_7, int* io_8) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    sysml_lazy_0 = io_6;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_displayMode == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_displayMode + 1;
        }
        sysml_state_displayMode = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_6;
    sysml_lazy_2 = sysml_state_alarmEnabled;
    if (sysml_lazy_2) {
        sysml_lazy_3 = io_2 <= 0.0;
        if (!sysml_lazy_3) {
            sysml_lazy_3 = io_2 < 10.0;
        }
        sysml_lazy_2 = sysml_lazy_3;
    }
    sysml_state_heaterEnabled = !sysml_lazy_2;
    sysml_lazy_4 = io_2 > 0.0;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_2 >= 10.0;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_state_aeratorEnabled = sysml_conditional_5;
    if (io_2 < 10.0) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_state_alarmEnabled = sysml_conditional_6;
    double sysml_value_0 = 0;
    if (sysml_state_alarmEnabled) {
        sysml_conditional_7 = 0.0;
    }
    else {
        if (io_0 < 30.0) {
            sysml_conditional_8 = 255.0;
        }
        else {
            sysml_conditional_8 = 0.0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    sysml_value_0 = sysml_conditional_7;
    double sysml_value_1 = 0;
    sysml_lazy_9 = !sysml_state_alarmEnabled;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_1 < 500.0;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 255.0;
    }
    else {
        sysml_conditional_10 = 0.0;
    }
    sysml_value_1 = sysml_conditional_10;
    double sysml_value_2 = 0;
    if (sysml_state_aeratorEnabled) {
        sysml_conditional_11 = 255.0;
    }
    else {
        sysml_conditional_11 = 0.0;
    }
    sysml_value_2 = sysml_conditional_11;
    int sysml_value_3 = 0;
    if (sysml_state_alarmEnabled) {
        sysml_conditional_12 = 2000;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_3 = sysml_conditional_12;
    int sysml_value_4 = 0;
    if (sysml_state_displayMode == 0) {
        if (io_0 <= 0.0) {
            sysml_conditional_14 = 0;
        }
        else {
            if (io_0 < ((double)((int)io_0))) {
                sysml_conditional_15 = 1;
            }
            else {
                sysml_conditional_15 = 0;
            }
            sysml_conditional_14 = ((int)io_0) - sysml_conditional_15;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    else {
        if (sysml_state_displayMode == 1) {
            if (io_1 < ((double)((int)io_1))) {
                sysml_conditional_17 = 1;
            }
            else {
                sysml_conditional_17 = 0;
            }
            sysml_conditional_16 = ((int)io_1) - sysml_conditional_17;
        }
        else {
            if (io_2 < ((double)((int)io_2))) {
                sysml_conditional_18 = 1;
            }
            else {
                sysml_conditional_18 = 0;
            }
            sysml_conditional_16 = ((int)io_2) - sysml_conditional_18;
        }
        sysml_conditional_13 = sysml_conditional_16;
    }
    sysml_value_4 = sysml_conditional_13;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_7 = sysml_value_3;
    *io_8 = sysml_value_4;
}