#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_menuIndex = 0;
/* Global Variable define of  */
int sysml_state_confirmedIndex = 0;
/* Global Variable define of  */
bool sysml_state_previousMenuPressed = false;
/* Global Variable define of  */
bool sysml_state_previousConfirmPressed = false;
/* Global Variable define of  */
bool sysml_state_tempValid = false;
/* Global Variable define of  */
double sysml_state_lastTemp = 0.0;
/* Global Variable define of  */
double sysml_state_lastHumidity = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_menuIndex = 0;
    sysml_state_confirmedIndex = 0;
    sysml_state_previousMenuPressed = false;
    sysml_state_previousConfirmPressed = false;
    sysml_state_tempValid = false;
    sysml_state_lastTemp = 0.0;
    sysml_state_lastHumidity = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, double io_3, double io_4, bool io_5, int* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    if (io_5) {
        sysml_state_tempValid = true;
        sysml_state_lastTemp = io_3;
        sysml_state_lastHumidity = io_4;
    }
    else {
        sysml_state_tempValid = false;
    }
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousMenuPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_menuIndex == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_menuIndex + 1;
        }
        sysml_state_menuIndex = sysml_conditional_1;
    }
    sysml_lazy_2 = io_2;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousConfirmPressed;
    }
    if (sysml_lazy_2) {
        sysml_state_confirmedIndex = sysml_state_menuIndex;
    }
    sysml_state_previousMenuPressed = io_1;
    sysml_state_previousConfirmPressed = io_2;
    int sysml_value_0 = 0;
    if (sysml_state_confirmedIndex == 0) {
        if (sysml_state_tempValid) {
            if (sysml_state_lastTemp >= 0.0) {
                if (((sysml_state_lastTemp * 10.0) + 0.5) < ((double)((int)((sysml_state_lastTemp * 10.0) + 0.5)))) {
                    sysml_conditional_6 = 1;
                }
                else {
                    sysml_conditional_6 = 0;
                }
                sysml_conditional_5 = ((int)((sysml_state_lastTemp * 10.0) + 0.5)) - sysml_conditional_6;
            }
            else {
                if (((sysml_state_lastTemp * 10.0) - 0.5) < ((double)((int)((sysml_state_lastTemp * 10.0) - 0.5)))) {
                    sysml_conditional_7 = 1;
                }
                else {
                    sysml_conditional_7 = 0;
                }
                sysml_conditional_5 = ((int)((sysml_state_lastTemp * 10.0) - 0.5)) - sysml_conditional_7;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        if (sysml_state_confirmedIndex == 1) {
            if (sysml_state_tempValid) {
                if (sysml_state_lastHumidity >= 0.0) {
                    if (((sysml_state_lastHumidity * 10.0) + 0.5) < ((double)((int)((sysml_state_lastHumidity * 10.0) + 0.5)))) {
                        sysml_conditional_11 = 1;
                    }
                    else {
                        sysml_conditional_11 = 0;
                    }
                    sysml_conditional_10 = ((int)((sysml_state_lastHumidity * 10.0) + 0.5)) - sysml_conditional_11;
                }
                else {
                    if (((sysml_state_lastHumidity * 10.0) - 0.5) < ((double)((int)((sysml_state_lastHumidity * 10.0) - 0.5)))) {
                        sysml_conditional_12 = 1;
                    }
                    else {
                        sysml_conditional_12 = 0;
                    }
                    sysml_conditional_10 = ((int)((sysml_state_lastHumidity * 10.0) - 0.5)) - sysml_conditional_12;
                }
                sysml_conditional_9 = sysml_conditional_10;
            }
            else {
                sysml_conditional_9 = 0;
            }
            sysml_conditional_8 = sysml_conditional_9;
        }
        else {
            if (io_0 <= 1023.0) {
                if (io_0 < ((double)((int)io_0))) {
                    sysml_conditional_14 = 1;
                }
                else {
                    sysml_conditional_14 = 0;
                }
                sysml_conditional_13 = ((int)io_0) - sysml_conditional_14;
            }
            else {
                if (io_0 < ((double)((int)io_0))) {
                    sysml_conditional_15 = 1;
                }
                else {
                    sysml_conditional_15 = 0;
                }
                sysml_conditional_13 = ((int)io_0) - sysml_conditional_15;
            }
            sysml_conditional_8 = sysml_conditional_13;
        }
        sysml_conditional_3 = sysml_conditional_8;
    }
    sysml_value_0 = sysml_conditional_3;
    *io_6 = sysml_value_0;
}