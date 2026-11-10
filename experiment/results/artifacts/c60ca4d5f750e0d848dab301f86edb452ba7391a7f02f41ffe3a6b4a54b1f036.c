#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2Pressed = false;
/* Global Variable define of  */
bool sysml_state_btn1StablePressed = false;
/* Global Variable define of  */
bool sysml_state_btn2StablePressed = false;
/* Global Variable define of  */
int sysml_state_btn1DebounceCounter = 0;
/* Global Variable define of  */
int sysml_state_btn2DebounceCounter = 0;
/* Global Variable define of  */
double sysml_state_servo1Angle = 0.0;
/* Global Variable define of  */
double sysml_state_servo2Angle = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousBtn1Pressed = false;
    sysml_state_previousBtn2Pressed = false;
    sysml_state_btn1StablePressed = false;
    sysml_state_btn2StablePressed = false;
    sysml_state_btn1DebounceCounter = 0;
    sysml_state_btn2DebounceCounter = 0;
    sysml_state_servo1Angle = 0.0;
    sysml_state_servo2Angle = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, double* io_4, double* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1Pressed;
    }
    if (sysml_lazy_0) {
        sysml_state_btn1DebounceCounter = 5;
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_btn1DebounceCounter > 0;
    }
    if (sysml_lazy_1) {
        sysml_state_btn1DebounceCounter = (sysml_state_btn1DebounceCounter - 1);
    }
    if ((!io_0)) {
        sysml_state_btn1DebounceCounter = 0;
        sysml_state_btn1StablePressed = false;
    }
    sysml_lazy_2 = io_0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_btn1DebounceCounter == 0;
    }
    if (sysml_lazy_2) {
        sysml_state_btn1StablePressed = true;
    }
    sysml_lazy_3 = sysml_state_btn1StablePressed;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousBtn1Pressed;
    }
    if (sysml_lazy_3) {
        if (io_2 <= 0.0) {
            sysml_conditional_4 = 0.0;
        }
        else {
            if (io_2 >= 1023.0) {
                sysml_conditional_5 = 180.0;
            }
            else {
                sysml_conditional_5 = ((double)(io_2 * 180.0)) / 1023.0;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_state_servo1Angle = sysml_conditional_4;
    }
    sysml_state_previousBtn1Pressed = io_0;
    sysml_lazy_6 = io_1;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_previousBtn2Pressed;
    }
    if (sysml_lazy_6) {
        sysml_state_btn2DebounceCounter = 5;
    }
    sysml_lazy_7 = io_1;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_btn2DebounceCounter > 0;
    }
    if (sysml_lazy_7) {
        sysml_state_btn2DebounceCounter = (sysml_state_btn2DebounceCounter - 1);
    }
    if ((!io_1)) {
        sysml_state_btn2DebounceCounter = 0;
        sysml_state_btn2StablePressed = false;
    }
    sysml_lazy_8 = io_1;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_btn2DebounceCounter == 0;
    }
    if (sysml_lazy_8) {
        sysml_state_btn2StablePressed = true;
    }
    sysml_lazy_9 = sysml_state_btn2StablePressed;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_previousBtn2Pressed;
    }
    if (sysml_lazy_9) {
        if (io_3 <= 0.0) {
            sysml_conditional_10 = 0.0;
        }
        else {
            if (io_3 >= 1023.0) {
                sysml_conditional_11 = 180.0;
            }
            else {
                sysml_conditional_11 = ((double)(io_3 * 180.0)) / 1023.0;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_state_servo2Angle = sysml_conditional_10;
    }
    sysml_state_previousBtn2Pressed = io_1;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servo1Angle;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servo2Angle;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
}