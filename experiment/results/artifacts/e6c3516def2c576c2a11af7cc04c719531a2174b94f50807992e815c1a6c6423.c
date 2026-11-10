#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode = 0;
/* Global Variable define of  */
int sysml_state_brightness = 255;
/* Global Variable define of  */
int sysml_state_breathSpeed = 2000;
/* Global Variable define of  */
int sysml_state_accumMs = 0;
/* Global Variable define of  */
bool sysml_state_prevModeBtnPressed = false;
/* Global Variable define of  */
bool sysml_state_prevParamBtnPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_brightness = 255;
    sysml_state_breathSpeed = 2000;
    sysml_state_accumMs = 0;
    sysml_state_prevModeBtnPressed = false;
    sysml_state_prevParamBtnPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int io_5, double* io_2, int* io_3, bool* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_arithmetic_12 = 0;
    double sysml_arithmetic_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_arithmetic_15 = 0;
    double sysml_arithmetic_16 = 0;
    int sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    sysml_state_accumMs = (sysml_state_accumMs + io_5);
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevModeBtnPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_mode == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_mode + 1;
        }
        sysml_state_mode = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevParamBtnPressed;
    }
    if (sysml_lazy_2) {
        if ((sysml_state_mode == 1)) {
            if (sysml_state_brightness == 255) {
                sysml_conditional_3 = 16;
            }
            else {
                sysml_conditional_3 = sysml_state_brightness + 16;
            }
            sysml_state_brightness = sysml_conditional_3;
        }
        if ((sysml_state_mode == 2)) {
            if (sysml_state_breathSpeed == 5000) {
                sysml_conditional_4 = 500;
            }
            else {
                sysml_conditional_4 = sysml_state_breathSpeed + 500;
            }
            sysml_state_breathSpeed = sysml_conditional_4;
        }
    }
    sysml_state_prevModeBtnPressed = io_0;
    sysml_state_prevParamBtnPressed = io_1;
    if ((sysml_state_mode != 2)) {
        sysml_state_accumMs = 0;
    }
    double sysml_value_0 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_5 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_lazy_7 = sysml_state_brightness >= 0;
            if (sysml_lazy_7) {
                sysml_lazy_7 = sysml_state_brightness <= 255;
            }
            if (sysml_lazy_7) {
                sysml_conditional_8 = sysml_state_brightness * 1.0;
            }
            else {
                sysml_conditional_8 = 0.0;
            }
            sysml_conditional_6 = sysml_conditional_8;
        }
        else {
            if (sysml_state_breathSpeed <= 0) {
                sysml_conditional_9 = 0.0;
            }
            else {
                if (sysml_state_accumMs < (sysml_state_breathSpeed * 0.5)) {
                    if (sysml_state_breathSpeed == 0) {
                        sysml_conditional_11 = 0.0;
                    }
                    else {
                        sysml_arithmetic_12 = sysml_state_accumMs * 1.0;
                        sysml_arithmetic_13 = (sysml_state_breathSpeed * 1.0) * 0.5;
                        sysml_conditional_11 = ((double)((sysml_state_brightness * 1.0) * sysml_arithmetic_12)) / sysml_arithmetic_13;
                    }
                    sysml_conditional_10 = sysml_conditional_11;
                }
                else {
                    if (sysml_state_breathSpeed == 0) {
                        sysml_conditional_14 = 0.0;
                    }
                    else {
                        sysml_arithmetic_15 = ((sysml_state_breathSpeed * 1.0) * 1.0) - (sysml_state_accumMs * 1.0);
                        sysml_arithmetic_16 = (sysml_state_breathSpeed * 1.0) * 0.5;
                        sysml_conditional_14 = ((double)((sysml_state_brightness * 1.0) * sysml_arithmetic_15)) / sysml_arithmetic_16;
                    }
                    sysml_conditional_10 = sysml_conditional_14;
                }
                sysml_conditional_9 = sysml_conditional_10;
            }
            sysml_conditional_6 = sysml_conditional_9;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_value_0 = sysml_conditional_5;
    int sysml_value_1 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_17 = 0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_18 = sysml_state_brightness;
        }
        else {
            sysml_conditional_18 = sysml_state_breathSpeed;
        }
        sysml_conditional_17 = sysml_conditional_18;
    }
    sysml_value_1 = sysml_conditional_17;
    bool sysml_value_2 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_19 = false;
    }
    else {
        sysml_conditional_19 = true;
    }
    sysml_value_2 = sysml_conditional_19;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}