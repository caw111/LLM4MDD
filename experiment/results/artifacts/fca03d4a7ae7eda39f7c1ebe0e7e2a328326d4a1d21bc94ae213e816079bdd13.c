#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_count = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn0 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_count = 0;
    sysml_state_prevBtn0 = false;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, int* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    double sysml_conditional_17 = 0;
    double sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    double sysml_conditional_22 = 0;
    double sysml_conditional_23 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn0;
    }
    if (sysml_lazy_0) {
        if (sysml_state_count >= 15) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_count + 1;
        }
        sysml_state_count = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_2) {
        if (sysml_state_count >= 15) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = sysml_state_count + 1;
        }
        sysml_state_count = sysml_conditional_3;
    }
    sysml_lazy_4 = io_2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_4) {
        if (sysml_state_count >= 15) {
            sysml_conditional_5 = 0;
        }
        else {
            sysml_conditional_5 = sysml_state_count + 1;
        }
        sysml_state_count = sysml_conditional_5;
    }
    sysml_state_prevBtn0 = io_0;
    sysml_state_prevBtn1 = io_1;
    sysml_state_prevBtn2 = io_2;
    double sysml_value_0 = 0;
    if (sysml_state_count >= 10) {
        sysml_conditional_6 = 255.0;
    }
    else {
        if (sysml_state_count >= 5) {
            sysml_conditional_7 = 0.0;
        }
        else {
            if (sysml_state_count < 5) {
                sysml_conditional_8 = 255.0;
            }
            else {
                sysml_conditional_8 = 0.0;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_0 = sysml_conditional_6;
    double sysml_value_1 = 0;
    sysml_lazy_9 = sysml_state_count >= 5;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_count <= 9;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 255.0;
    }
    else {
        sysml_lazy_11 = sysml_state_count >= 10;
        if (sysml_lazy_11) {
            sysml_lazy_11 = sysml_state_count <= 15;
        }
        if (sysml_lazy_11) {
            sysml_conditional_12 = 0.0;
        }
        else {
            sysml_conditional_12 = 0.0;
        }
        sysml_conditional_10 = sysml_conditional_12;
    }
    sysml_value_1 = sysml_conditional_10;
    double sysml_value_2 = 0;
    if (sysml_state_count < 5) {
        sysml_conditional_13 = 255.0;
    }
    else {
        if (sysml_state_count >= 10) {
            sysml_conditional_14 = 255.0;
        }
        else {
            sysml_conditional_14 = 0.0;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    sysml_value_2 = sysml_conditional_13;
    double sysml_value_3 = 0;
    if (sysml_state_count < 5) {
        sysml_conditional_15 = 0.0;
    }
    else {
        sysml_lazy_16 = sysml_state_count >= 5;
        if (sysml_lazy_16) {
            sysml_lazy_16 = sysml_state_count <= 9;
        }
        if (sysml_lazy_16) {
            sysml_conditional_17 = 255.0;
        }
        else {
            if (sysml_state_count >= 10) {
                sysml_conditional_18 = 255.0;
            }
            else {
                sysml_conditional_18 = 0.0;
            }
            sysml_conditional_17 = sysml_conditional_18;
        }
        sysml_conditional_15 = sysml_conditional_17;
    }
    sysml_value_3 = sysml_conditional_15;
    double sysml_value_4 = 0;
    if (sysml_state_count < 5) {
        sysml_conditional_19 = 0.0;
    }
    else {
        if (sysml_state_count <= 9) {
            sysml_conditional_20 = 255.0;
        }
        else {
            sysml_conditional_20 = 0.0;
        }
        sysml_conditional_19 = sysml_conditional_20;
    }
    sysml_value_4 = sysml_conditional_19;
    double sysml_value_5 = 0;
    sysml_lazy_21 = sysml_state_count >= 5;
    if (sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_count <= 9;
    }
    if (sysml_lazy_21) {
        sysml_conditional_22 = 0.0;
    }
    else {
        if (sysml_state_count >= 10) {
            sysml_conditional_23 = 255.0;
        }
        else {
            sysml_conditional_23 = 255.0;
        }
        sysml_conditional_22 = sysml_conditional_23;
    }
    sysml_value_5 = sysml_conditional_22;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_count;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *io_7 = sysml_value_4;
    *io_8 = sysml_value_5;
    *io_9 = sysml_value_6;
}