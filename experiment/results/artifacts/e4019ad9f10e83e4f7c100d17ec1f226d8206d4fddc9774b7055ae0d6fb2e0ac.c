#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_displayModeTemp = false;
/* Global Variable define of  */
bool sysml_state_displayModeLight = false;
/* Global Variable define of  */
bool sysml_state_displayModeLevel = true;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayModeTemp = false;
    sysml_state_displayModeLight = false;
    sysml_state_displayModeLevel = true;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_3, bool* io_4, double* io_5, bool* io_6, int* io_7, int* io_8) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_displayModeTemp) {
            sysml_state_displayModeTemp = false;
            sysml_state_displayModeLight = true;
            sysml_state_displayModeLevel = false;
        }
        else {
            if (sysml_state_displayModeLight) {
                sysml_state_displayModeTemp = false;
                sysml_state_displayModeLight = false;
                sysml_state_displayModeLevel = true;
            }
            else {
                sysml_state_displayModeTemp = true;
                sysml_state_displayModeLight = false;
                sysml_state_displayModeLevel = false;
            }
        }
        sysml_state_previousPressed = true;
    }
    else {
        sysml_state_previousPressed = io_3;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_1 = io_2 >= 512.0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_0 <= 600.0;
    }
    if (sysml_lazy_1) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_0 = sysml_conditional_2;
    double sysml_value_1 = 0;
    sysml_lazy_3 = io_2 >= 512.0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_1 <= 512.0;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = 1023.0;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_1 = sysml_conditional_4;
    bool sysml_value_2 = 0;
    sysml_lazy_5 = io_2 >= 512.0;
    if (sysml_lazy_5) {
        sysml_lazy_5 = true;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_2 = sysml_conditional_6;
    int sysml_value_3 = 0;
    if (io_2 < 512.0) {
        sysml_conditional_7 = 2000;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_3 = sysml_conditional_7;
    int sysml_value_4 = 0;
    if (sysml_state_displayModeTemp) {
        sysml_conditional_8 = 0;
    }
    else {
        if (sysml_state_displayModeLight) {
            sysml_conditional_9 = 0;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    sysml_value_4 = sysml_conditional_8;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *io_7 = sysml_value_3;
    *io_8 = sysml_value_4;
}