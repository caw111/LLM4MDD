#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_colorIndex = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_colorIndex = 0;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double* io_2, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    double sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    double sysml_conditional_15 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_0) {
        if (sysml_state_colorIndex == 8) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_colorIndex + 1;
        }
        sysml_state_colorIndex = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_2) {
        if (sysml_state_colorIndex == 0) {
            sysml_conditional_3 = 8;
        }
        else {
            sysml_conditional_3 = sysml_state_colorIndex - 1;
        }
        sysml_state_colorIndex = sysml_conditional_3;
    }
    sysml_state_prevBtn1 = io_0;
    sysml_state_prevBtn2 = io_1;
    double sysml_value_0 = 0;
    sysml_lazy_4 = sysml_state_colorIndex == 1;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_colorIndex == 5;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_colorIndex == 7;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = 255.0;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_0 = sysml_conditional_7;
    double sysml_value_1 = 0;
    sysml_lazy_8 = sysml_state_colorIndex == 2;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_colorIndex == 4;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_colorIndex == 6;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_colorIndex == 7;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = 255.0;
    }
    else {
        sysml_conditional_11 = 0.0;
    }
    sysml_value_1 = sysml_conditional_11;
    double sysml_value_2 = 0;
    sysml_lazy_12 = sysml_state_colorIndex == 3;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_colorIndex == 5;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_colorIndex == 6;
    }
    sysml_lazy_14 = sysml_lazy_13;
    if (!sysml_lazy_14) {
        sysml_lazy_14 = sysml_state_colorIndex == 7;
    }
    if (sysml_lazy_14) {
        sysml_conditional_15 = 255.0;
    }
    else {
        sysml_conditional_15 = 0.0;
    }
    sysml_value_2 = sysml_conditional_15;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}