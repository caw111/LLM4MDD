#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_showHumidity = false;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Global Variable define of  */
int sysml_state_pwmCountFan = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_showHumidity = false;
    sysml_state_prevPressed = false;
    sysml_state_pwmCountFan = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, bool io_5, double* io_3, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    double sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    sysml_lazy_0 = io_5;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_showHumidity = (!sysml_state_showHumidity);
    }
    sysml_lazy_1 = !sysml_state_showHumidity;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2;
    }
    if (sysml_lazy_1) {
        if (io_0 > 30.0) {
            if (io_0 >= 40.0) {
                sysml_conditional_3 = 255;
            }
            else {
                if (io_0 <= 30.0) {
                    sysml_conditional_4 = 0;
                }
                else {
                    if (((io_0 - 30.0) * 25.5) < ((double)((int)((io_0 - 30.0) * 25.5)))) {
                        sysml_conditional_5 = 1;
                    }
                    else {
                        sysml_conditional_5 = 0;
                    }
                    sysml_conditional_4 = ((int)((io_0 - 30.0) * 25.5)) - sysml_conditional_5;
                }
                sysml_conditional_3 = sysml_conditional_4;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_state_pwmCountFan = sysml_conditional_2;
    }
    else {
        sysml_lazy_6 = io_2;
        if (sysml_lazy_6) {
            sysml_lazy_6 = io_0 > 30.0;
        }
        if (sysml_lazy_6) {
            if (io_0 >= 40.0) {
                sysml_conditional_8 = 255;
            }
            else {
                if (io_0 <= 30.0) {
                    sysml_conditional_9 = 0;
                }
                else {
                    if (((io_0 - 30.0) * 25.5) < ((double)((int)((io_0 - 30.0) * 25.5)))) {
                        sysml_conditional_10 = 1;
                    }
                    else {
                        sysml_conditional_10 = 0;
                    }
                    sysml_conditional_9 = ((int)((io_0 - 30.0) * 25.5)) - sysml_conditional_10;
                }
                sysml_conditional_8 = sysml_conditional_9;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_state_pwmCountFan = sysml_conditional_7;
    }
    sysml_state_prevPressed = io_5;
    double sysml_value_0 = 0;
    sysml_lazy_11 = io_2;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_0 > 30.0;
    }
    if (sysml_lazy_11) {
        sysml_conditional_12 = sysml_state_pwmCountFan;
    }
    else {
        sysml_conditional_12 = 0.0;
    }
    sysml_value_0 = sysml_conditional_12;
    int sysml_value_1 = 0;
    if (sysml_state_showHumidity) {
        if (io_2) {
            if (io_1 < ((double)((int)io_1))) {
                sysml_conditional_15 = 1;
            }
            else {
                sysml_conditional_15 = 0;
            }
            sysml_conditional_14 = ((int)io_1) - sysml_conditional_15;
        }
        else {
            sysml_conditional_14 = 0;
        }
        sysml_conditional_13 = sysml_conditional_14;
    }
    else {
        if (io_2) {
            if (io_0 < ((double)((int)io_0))) {
                sysml_conditional_17 = 1;
            }
            else {
                sysml_conditional_17 = 0;
            }
            sysml_conditional_16 = ((int)io_0) - sysml_conditional_17;
        }
        else {
            sysml_conditional_16 = 0;
        }
        sysml_conditional_13 = sysml_conditional_16;
    }
    sysml_value_1 = sysml_conditional_13;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
}