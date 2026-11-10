#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_led1Brightness = 0;
/* Global Variable define of  */
int sysml_state_led2Brightness = 0;
/* Global Variable define of  */
int sysml_state_led3Brightness = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3 = false;
/* Global Variable define of  */
int sysml_state_hold1RemainingMs = 0;
/* Global Variable define of  */
int sysml_state_hold2RemainingMs = 0;
/* Global Variable define of  */
int sysml_state_hold3RemainingMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_led1Brightness = 0;
    sysml_state_led2Brightness = 0;
    sysml_state_led3Brightness = 0;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_prevBtn3 = false;
    sysml_state_hold1RemainingMs = 0;
    sysml_state_hold2RemainingMs = 0;
    sysml_state_hold3RemainingMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, double io_6, double io_7, double io_8, int io_9, double* io_0, double* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    if ((sysml_state_hold1RemainingMs > 0)) {
        sysml_state_hold1RemainingMs = (sysml_state_hold1RemainingMs - io_9);
    }
    if ((sysml_state_hold1RemainingMs <= 0)) {
        sysml_state_hold1RemainingMs = 0;
    }
    if ((sysml_state_hold2RemainingMs > 0)) {
        sysml_state_hold2RemainingMs = (sysml_state_hold2RemainingMs - io_9);
    }
    if ((sysml_state_hold2RemainingMs <= 0)) {
        sysml_state_hold2RemainingMs = 0;
    }
    if ((sysml_state_hold3RemainingMs > 0)) {
        sysml_state_hold3RemainingMs = (sysml_state_hold3RemainingMs - io_9);
    }
    if ((sysml_state_hold3RemainingMs <= 0)) {
        sysml_state_hold3RemainingMs = 0;
    }
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_0) {
        sysml_state_hold1RemainingMs = 2000;
        if (io_6 <= 0.0) {
            sysml_conditional_1 = 0;
        }
        else {
            if (io_6 >= 1023.0) {
                sysml_conditional_2 = 255;
            }
            else {
                if ((((double)(io_6 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_6 * 255.0)) / 1023.0)))) {
                    sysml_conditional_3 = 1;
                }
                else {
                    sysml_conditional_3 = 0;
                }
                sysml_conditional_2 = ((int)(((double)(io_6 * 255.0)) / 1023.0)) - sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_led1Brightness = sysml_conditional_1;
    }
    sysml_lazy_4 = io_4;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_4) {
        sysml_state_hold2RemainingMs = 2000;
        if (io_7 <= 0.0) {
            sysml_conditional_5 = 0;
        }
        else {
            if (io_7 >= 1023.0) {
                sysml_conditional_6 = 255;
            }
            else {
                if ((((double)(io_7 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_7 * 255.0)) / 1023.0)))) {
                    sysml_conditional_7 = 1;
                }
                else {
                    sysml_conditional_7 = 0;
                }
                sysml_conditional_6 = ((int)(((double)(io_7 * 255.0)) / 1023.0)) - sysml_conditional_7;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_state_led2Brightness = sysml_conditional_5;
    }
    sysml_lazy_8 = io_5;
    if (sysml_lazy_8) {
        sysml_lazy_8 = !sysml_state_prevBtn3;
    }
    if (sysml_lazy_8) {
        sysml_state_hold3RemainingMs = 2000;
        if (io_8 <= 0.0) {
            sysml_conditional_9 = 0;
        }
        else {
            if (io_8 >= 1023.0) {
                sysml_conditional_10 = 255;
            }
            else {
                if ((((double)(io_8 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_8 * 255.0)) / 1023.0)))) {
                    sysml_conditional_11 = 1;
                }
                else {
                    sysml_conditional_11 = 0;
                }
                sysml_conditional_10 = ((int)(((double)(io_8 * 255.0)) / 1023.0)) - sysml_conditional_11;
            }
            sysml_conditional_9 = sysml_conditional_10;
        }
        sysml_state_led3Brightness = sysml_conditional_9;
    }
    sysml_state_prevBtn1 = io_3;
    sysml_state_prevBtn2 = io_4;
    sysml_state_prevBtn3 = io_5;
    double sysml_value_0 = 0;
    if (sysml_state_led1Brightness < ((double)((int)sysml_state_led1Brightness))) {
        sysml_conditional_12 = 1;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_0 = ((int)sysml_state_led1Brightness) - sysml_conditional_12;
    double sysml_value_1 = 0;
    if (sysml_state_led2Brightness < ((double)((int)sysml_state_led2Brightness))) {
        sysml_conditional_13 = 1;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_1 = ((int)sysml_state_led2Brightness) - sysml_conditional_13;
    double sysml_value_2 = 0;
    if (sysml_state_led3Brightness < ((double)((int)sysml_state_led3Brightness))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    sysml_value_2 = ((int)sysml_state_led3Brightness) - sysml_conditional_14;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}