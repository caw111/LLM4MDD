#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousBtn1Pressed = true;
/* Global Variable define of  */
bool sysml_state_previousBtn2Pressed = true;
/* Global Variable define of  */
int sysml_state_activeMode = 0;
/* Global Variable define of  */
int sysml_state_remainingHoldMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousBtn1Pressed = true;
    sysml_state_previousBtn2Pressed = true;
    sysml_state_activeMode = 0;
    sysml_state_remainingHoldMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double* io_2, double* io_3, bool* io_4, bool* io_5, bool* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    if (sysml_lazy_0) {
        sysml_state_activeMode = 3;
        sysml_state_remainingHoldMs = 2000;
        sysml_state_previousBtn1Pressed = io_0;
        sysml_state_previousBtn2Pressed = io_1;
    }
    else {
        sysml_lazy_1 = io_0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = !io_1;
        }
        if (sysml_lazy_1) {
            sysml_lazy_2 = io_0;
            if (sysml_lazy_2) {
                sysml_lazy_2 = !sysml_state_previousBtn1Pressed;
            }
            if (sysml_lazy_2) {
                sysml_state_activeMode = 1;
                sysml_state_remainingHoldMs = 2000;
            }
            sysml_lazy_3 = io_0;
            if (sysml_lazy_3) {
                sysml_lazy_3 = !sysml_state_previousBtn1Pressed;
            }
            if (sysml_lazy_3) {
            }
            sysml_state_previousBtn1Pressed = io_0;
            sysml_state_previousBtn2Pressed = io_1;
        }
        else {
            sysml_lazy_4 = io_1;
            if (sysml_lazy_4) {
                sysml_lazy_4 = !io_0;
            }
            if (sysml_lazy_4) {
                sysml_lazy_5 = io_1;
                if (sysml_lazy_5) {
                    sysml_lazy_5 = !sysml_state_previousBtn2Pressed;
                }
                if (sysml_lazy_5) {
                    sysml_state_activeMode = 2;
                    sysml_state_remainingHoldMs = 2000;
                }
                sysml_state_previousBtn1Pressed = io_0;
                sysml_state_previousBtn2Pressed = io_1;
            }
            else {
                sysml_lazy_6 = sysml_state_remainingHoldMs >= 0;
                if (sysml_lazy_6) {
                    sysml_lazy_6 = sysml_state_remainingHoldMs > 0;
                }
                if (sysml_lazy_6) {
                    sysml_state_remainingHoldMs = 0;
                    sysml_state_activeMode = 0;
                }
                else {
                    sysml_state_activeMode = 0;
                }
                sysml_state_previousBtn1Pressed = io_0;
                sysml_state_previousBtn2Pressed = io_1;
            }
        }
    }
    double sysml_value_0 = 0;
    sysml_lazy_7 = sysml_state_activeMode != 0;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_activeMode == 1;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = 90.0;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_0 = sysml_conditional_8;
    double sysml_value_1 = 0;
    sysml_lazy_9 = sysml_state_activeMode != 0;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_activeMode == 2;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 180.0;
    }
    else {
        sysml_conditional_10 = 0.0;
    }
    sysml_value_1 = sysml_conditional_10;
    bool sysml_value_2 = 0;
    if (sysml_state_activeMode == 1) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_2 = sysml_conditional_11;
    bool sysml_value_3 = 0;
    if (sysml_state_activeMode == 3) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_3 = sysml_conditional_12;
    bool sysml_value_4 = 0;
    if (sysml_state_activeMode == 2) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_4 = sysml_conditional_13;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_4;
}