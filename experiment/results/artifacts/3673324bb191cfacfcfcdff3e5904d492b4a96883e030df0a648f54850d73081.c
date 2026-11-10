#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_led1Brightness = 0.0;
/* Global Variable define of  */
double sysml_state_led2Brightness = 0.0;
/* Global Variable define of  */
double sysml_state_led3Brightness = 0.0;
/* Global Variable define of  */
int sysml_state_btn1StableUntil = 0;
/* Global Variable define of  */
int sysml_state_btn2StableUntil = 0;
/* Global Variable define of  */
int sysml_state_btn3StableUntil = 0;
/* Global Variable define of  */
int sysml_state_stableTimer = 0;
/* Global Variable define of  */
bool sysml_state_previousBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousBtn3Pressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_led1Brightness = 0.0;
    sysml_state_led2Brightness = 0.0;
    sysml_state_led3Brightness = 0.0;
    sysml_state_btn1StableUntil = 0;
    sysml_state_btn2StableUntil = 0;
    sysml_state_btn3StableUntil = 0;
    sysml_state_stableTimer = 0;
    sysml_state_previousBtn1Pressed = false;
    sysml_state_previousBtn2Pressed = false;
    sysml_state_previousBtn3Pressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, double io_6, double io_7, double io_8, double* io_0, double* io_1, double* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    if ((sysml_state_stableTimer == 0)) {
        sysml_state_led1Brightness = 0.0;
        sysml_state_led2Brightness = 0.0;
        sysml_state_led3Brightness = 0.0;
        sysml_state_btn1StableUntil = 0;
        sysml_state_btn2StableUntil = 0;
        sysml_state_btn3StableUntil = 0;
    }
    sysml_state_stableTimer = (sysml_state_stableTimer + 1);
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1Pressed;
    }
    if (sysml_lazy_0) {
        sysml_state_led1Brightness = ((double)((io_6 * 255.0)) / 1023.0);
        sysml_state_btn1StableUntil = (sysml_state_stableTimer + 200);
    }
    sysml_lazy_1 = io_4;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousBtn2Pressed;
    }
    if (sysml_lazy_1) {
        sysml_state_led2Brightness = ((double)((io_7 * 255.0)) / 1023.0);
        sysml_state_btn2StableUntil = (sysml_state_stableTimer + 200);
    }
    sysml_lazy_2 = io_5;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousBtn3Pressed;
    }
    if (sysml_lazy_2) {
        sysml_state_led3Brightness = ((double)((io_8 * 255.0)) / 1023.0);
        sysml_state_btn3StableUntil = (sysml_state_stableTimer + 200);
    }
    sysml_lazy_3 = io_3;
    if (sysml_lazy_3) {
        sysml_lazy_4 = sysml_state_stableTimer >= sysml_state_btn1StableUntil;
        if (sysml_lazy_4) {
            sysml_lazy_4 = true;
        }
        sysml_lazy_3 = !sysml_lazy_4;
    }
    if (sysml_lazy_3) {
        sysml_state_led1Brightness = sysml_state_led1Brightness;
    }
    sysml_lazy_5 = io_4;
    if (sysml_lazy_5) {
        sysml_lazy_6 = sysml_state_stableTimer >= sysml_state_btn2StableUntil;
        if (sysml_lazy_6) {
            sysml_lazy_6 = true;
        }
        sysml_lazy_5 = !sysml_lazy_6;
    }
    if (sysml_lazy_5) {
        sysml_state_led2Brightness = sysml_state_led2Brightness;
    }
    sysml_lazy_7 = io_5;
    if (sysml_lazy_7) {
        sysml_lazy_8 = sysml_state_stableTimer >= sysml_state_btn3StableUntil;
        if (sysml_lazy_8) {
            sysml_lazy_8 = true;
        }
        sysml_lazy_7 = !sysml_lazy_8;
    }
    if (sysml_lazy_7) {
        sysml_state_led3Brightness = sysml_state_led3Brightness;
    }
    sysml_state_previousBtn1Pressed = io_3;
    sysml_state_previousBtn2Pressed = io_4;
    sysml_state_previousBtn3Pressed = io_5;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_led1Brightness;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_led2Brightness;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_led3Brightness;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}