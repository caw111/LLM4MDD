#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode = 0;
/* Global Variable define of  */
double sysml_state_servo1Angle = 0.0;
/* Global Variable define of  */
double sysml_state_servo2Angle = 0.0;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Global Variable define of  */
bool sysml_state_previousBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2 = false;
/* Global Variable define of  */
bool sysml_state_btn1Latch = false;
/* Global Variable define of  */
bool sysml_state_btn2Latch = false;
/* Global Variable define of  */
int sysml_state_maintainUntilMs = 0;
/* Global Variable define of  */
int sysml_state_flashAccumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_servo1Angle = 0.0;
    sysml_state_servo2Angle = 0.0;
    sysml_state_ledOn = false;
    sysml_state_previousBtn1 = false;
    sysml_state_previousBtn2 = false;
    sysml_state_btn1Latch = false;
    sysml_state_btn2Latch = false;
    sysml_state_maintainUntilMs = 0;
    sysml_state_flashAccumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, int io_5, double* io_0, double* io_1, bool* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    sysml_lazy_0 = sysml_state_btn1Latch;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_btn2Latch;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_2 = sysml_state_btn2Latch;
        if (sysml_lazy_2) {
            sysml_lazy_2 = !sysml_state_btn1Latch;
        }
        sysml_lazy_1 = !sysml_lazy_2;
    }
    if (sysml_lazy_1) {
        if ((sysml_state_maintainUntilMs <= 0)) {
            sysml_state_mode = 1;
            sysml_state_servo1Angle = 90.0;
            sysml_state_servo2Angle = 0.0;
            sysml_state_ledOn = true;
        }
    }
    sysml_lazy_3 = sysml_state_btn2Latch;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_btn1Latch;
    }
    if (sysml_lazy_3) {
        if ((sysml_state_maintainUntilMs <= 0)) {
            sysml_state_mode = 1;
            sysml_state_servo1Angle = 0.0;
            sysml_state_servo2Angle = 180.0;
            sysml_state_ledOn = false;
        }
    }
    sysml_lazy_4 = sysml_state_btn1Latch;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_btn2Latch;
    }
    if (sysml_lazy_4) {
        if ((sysml_state_maintainUntilMs <= 0)) {
            sysml_state_mode = 2;
            sysml_state_servo1Angle = 0.0;
            sysml_state_servo2Angle = 0.0;
        }
    }
    if (sysml_state_maintainUntilMs > 0) {
        sysml_conditional_5 = sysml_state_maintainUntilMs - io_5;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_state_maintainUntilMs = sysml_conditional_5;
    if ((sysml_state_mode == 0)) {
        sysml_state_servo1Angle = 0.0;
        sysml_state_servo2Angle = 0.0;
        sysml_state_ledOn = false;
        sysml_state_flashAccumulatedMs = 0;
    }
    if ((sysml_state_mode == 1)) {
        sysml_lazy_6 = sysml_state_btn1Latch;
        if (sysml_lazy_6) {
            sysml_lazy_6 = !sysml_state_btn2Latch;
        }
        if (sysml_lazy_6) {
            sysml_state_servo1Angle = 90.0;
            sysml_state_servo2Angle = 0.0;
            sysml_state_ledOn = true;
        }
        else {
            sysml_lazy_7 = sysml_state_btn2Latch;
            if (sysml_lazy_7) {
                sysml_lazy_7 = !sysml_state_btn1Latch;
            }
            if (sysml_lazy_7) {
                sysml_state_servo1Angle = 0.0;
                sysml_state_servo2Angle = 180.0;
                sysml_state_ledOn = false;
            }
            else {
                sysml_state_servo1Angle = 0.0;
                sysml_state_servo2Angle = 0.0;
                sysml_state_ledOn = false;
            }
        }
    }
    if ((sysml_state_mode == 2)) {
        sysml_state_servo1Angle = 0.0;
        sysml_state_servo2Angle = 0.0;
        sysml_state_flashAccumulatedMs = (sysml_state_flashAccumulatedMs + io_5);
        if ((sysml_state_flashAccumulatedMs >= 4000)) {
            sysml_state_flashAccumulatedMs = (sysml_state_flashAccumulatedMs - 4000);
        }
        if (sysml_state_flashAccumulatedMs < 2000) {
            sysml_conditional_8 = true;
        }
        else {
            sysml_conditional_8 = false;
        }
        sysml_state_ledOn = sysml_conditional_8;
    }
    sysml_lazy_9 = io_3;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_previousBtn1;
    }
    if (sysml_lazy_9) {
        if (io_4) {
            sysml_state_btn1Latch = true;
            sysml_state_btn2Latch = true;
            sysml_state_mode = 2;
            sysml_state_servo1Angle = 0.0;
            sysml_state_servo2Angle = 0.0;
            sysml_state_ledOn = true;
            sysml_state_flashAccumulatedMs = 0;
            sysml_state_maintainUntilMs = 2000;
        }
        else {
            sysml_state_btn1Latch = true;
            sysml_state_mode = 1;
            sysml_state_servo1Angle = 90.0;
            sysml_state_servo2Angle = 0.0;
            sysml_state_ledOn = true;
            sysml_state_flashAccumulatedMs = 0;
            sysml_state_maintainUntilMs = 2000;
        }
    }
    sysml_lazy_10 = io_4;
    if (sysml_lazy_10) {
        sysml_lazy_10 = !sysml_state_previousBtn2;
    }
    if (sysml_lazy_10) {
        if (io_3) {
            sysml_state_btn1Latch = true;
            sysml_state_btn2Latch = true;
            sysml_state_mode = 2;
            sysml_state_servo1Angle = 0.0;
            sysml_state_servo2Angle = 0.0;
            sysml_state_ledOn = true;
            sysml_state_flashAccumulatedMs = 0;
            sysml_state_maintainUntilMs = 2000;
        }
        else {
            sysml_state_btn2Latch = true;
            sysml_state_mode = 1;
            sysml_state_servo1Angle = 0.0;
            sysml_state_servo2Angle = 180.0;
            sysml_state_ledOn = false;
            sysml_state_flashAccumulatedMs = 0;
            sysml_state_maintainUntilMs = 2000;
        }
    }
    if ((sysml_state_maintainUntilMs <= 0)) {
        sysml_lazy_11 = sysml_state_btn1Latch;
        if (sysml_lazy_11) {
            sysml_lazy_11 = sysml_state_btn2Latch;
        }
        if (sysml_lazy_11) {
            sysml_state_mode = 2;
            sysml_state_servo1Angle = 0.0;
            sysml_state_servo2Angle = 0.0;
        }
        else {
            sysml_lazy_12 = sysml_state_btn1Latch;
            if (sysml_lazy_12) {
                sysml_lazy_12 = !sysml_state_btn2Latch;
            }
            if (sysml_lazy_12) {
                sysml_state_mode = 1;
                sysml_state_servo1Angle = 90.0;
                sysml_state_servo2Angle = 0.0;
                sysml_state_ledOn = true;
            }
            else {
                sysml_lazy_13 = sysml_state_btn2Latch;
                if (sysml_lazy_13) {
                    sysml_lazy_13 = !sysml_state_btn1Latch;
                }
                if (sysml_lazy_13) {
                    sysml_state_mode = 1;
                    sysml_state_servo1Angle = 0.0;
                    sysml_state_servo2Angle = 180.0;
                    sysml_state_ledOn = false;
                }
                else {
                    sysml_state_mode = 0;
                    sysml_state_servo1Angle = 0.0;
                    sysml_state_servo2Angle = 0.0;
                    sysml_state_ledOn = false;
                    sysml_state_flashAccumulatedMs = 0;
                }
            }
        }
    }
    sysml_state_previousBtn1 = io_3;
    sysml_state_previousBtn2 = io_4;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servo1Angle;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servo2Angle;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_ledOn;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}