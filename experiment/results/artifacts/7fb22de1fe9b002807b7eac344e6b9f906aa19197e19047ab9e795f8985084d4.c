#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_autoMode = true;
/* Global Variable define of  */
int sysml_state_manualAnglePercent = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3 = false;
/* Global Variable define of  */
int sysml_state_curAnglePercent = 0;
/* Global Variable define of  */
bool sysml_state_lightMode = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_autoMode = true;
    sysml_state_manualAnglePercent = 0;
    sysml_state_prevBtn2 = false;
    sysml_state_prevBtn3 = false;
    sysml_state_curAnglePercent = 0;
    sysml_state_lightMode = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, bool io_3, int io_7, bool* io_4, double* io_5, int* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    if (sysml_state_autoMode) {
        if ((io_1 >= 30.0)) {
            sysml_state_curAnglePercent = 0;
            sysml_state_lightMode = false;
        }
        else {
            if ((io_0 <= 400.0)) {
                sysml_state_curAnglePercent = 100;
                sysml_state_lightMode = true;
            }
            else {
                sysml_state_curAnglePercent = 0;
                sysml_state_lightMode = false;
            }
        }
    }
    if ((!sysml_state_autoMode)) {
        sysml_state_curAnglePercent = sysml_state_manualAnglePercent;
    }
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_0) {
        if (sysml_state_autoMode) {
            sysml_conditional_1 = false;
        }
        else {
            sysml_conditional_1 = true;
        }
        sysml_state_autoMode = sysml_conditional_1;
    }
    sysml_lazy_2 = io_3;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevBtn3;
    }
    if (sysml_lazy_2) {
        if ((!sysml_state_autoMode)) {
            if (sysml_state_manualAnglePercent == 100) {
                sysml_conditional_3 = 0;
            }
            else {
                sysml_conditional_3 = sysml_state_manualAnglePercent + 10;
            }
            sysml_state_manualAnglePercent = sysml_conditional_3;
        }
    }
    sysml_state_prevBtn2 = io_2;
    sysml_state_prevBtn3 = io_3;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_lightMode;
    double sysml_value_1 = 0;
    if (sysml_state_curAnglePercent >= 0) {
        if (sysml_state_curAnglePercent <= 100) {
            sysml_conditional_5 = sysml_state_curAnglePercent;
        }
        else {
            sysml_conditional_5 = 100.0;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_1 = sysml_conditional_4;
    int sysml_value_2 = 0;
    if (sysml_state_curAnglePercent <= 100) {
        if (sysml_state_curAnglePercent >= 0) {
            sysml_conditional_7 = sysml_state_curAnglePercent;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    else {
        sysml_conditional_6 = 100;
    }
    sysml_value_2 = sysml_conditional_6;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
}