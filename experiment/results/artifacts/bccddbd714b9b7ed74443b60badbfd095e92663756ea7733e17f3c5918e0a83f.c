#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_redIntensity = 0.0;
/* Global Variable define of  */
double sysml_state_greenIntensity = 0.0;
/* Global Variable define of  */
double sysml_state_blueIntensity = 0.0;
/* Global Variable define of  */
bool sysml_state_previousBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousBtn3Pressed = false;
/* Global Variable define of  */
int sysml_state_activeColor = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_redIntensity = 0.0;
    sysml_state_greenIntensity = 0.0;
    sysml_state_blueIntensity = 0.0;
    sysml_state_previousBtn1Pressed = false;
    sysml_state_previousBtn2Pressed = false;
    sysml_state_previousBtn3Pressed = false;
    sysml_state_activeColor = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_conditional_25 = 0;
    bool sysml_conditional_26 = 0;
    bool sysml_conditional_27 = 0;
    bool sysml_conditional_28 = 0;
    bool sysml_conditional_29 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1Pressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_redIntensity == 255.0) {
            sysml_conditional_1 = 0.0;
        }
        else {
            sysml_conditional_1 = sysml_state_redIntensity + 32.0;
        }
        sysml_state_redIntensity = sysml_conditional_1;
        sysml_state_activeColor = 1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousBtn2Pressed;
    }
    if (sysml_lazy_2) {
        if (sysml_state_greenIntensity == 255.0) {
            sysml_conditional_3 = 0.0;
        }
        else {
            sysml_conditional_3 = sysml_state_greenIntensity + 32.0;
        }
        sysml_state_greenIntensity = sysml_conditional_3;
        sysml_state_activeColor = 2;
    }
    sysml_lazy_4 = io_2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousBtn3Pressed;
    }
    if (sysml_lazy_4) {
        if (sysml_state_blueIntensity == 255.0) {
            sysml_conditional_5 = 0.0;
        }
        else {
            sysml_conditional_5 = sysml_state_blueIntensity + 32.0;
        }
        sysml_state_blueIntensity = sysml_conditional_5;
        sysml_state_activeColor = 3;
    }
    sysml_state_previousBtn1Pressed = io_0;
    sysml_state_previousBtn2Pressed = io_1;
    sysml_state_previousBtn3Pressed = io_2;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_redIntensity;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_greenIntensity;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_blueIntensity;
    bool sysml_value_3 = 0;
    if (sysml_state_activeColor == 1) {
        sysml_conditional_6 = sysml_state_redIntensity >= 32.0;
    }
    else {
        if (sysml_state_activeColor == 2) {
            sysml_conditional_7 = sysml_state_greenIntensity >= 32.0;
        }
        else {
            if (sysml_state_activeColor == 3) {
                sysml_conditional_8 = sysml_state_blueIntensity >= 32.0;
            }
            else {
                sysml_conditional_8 = false;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_3 = sysml_conditional_6;
    bool sysml_value_4 = 0;
    if (sysml_state_activeColor == 1) {
        sysml_conditional_9 = sysml_state_redIntensity >= 64.0;
    }
    else {
        if (sysml_state_activeColor == 2) {
            sysml_conditional_10 = sysml_state_greenIntensity >= 64.0;
        }
        else {
            if (sysml_state_activeColor == 3) {
                sysml_conditional_11 = sysml_state_blueIntensity >= 64.0;
            }
            else {
                sysml_conditional_11 = false;
            }
            sysml_conditional_10 = sysml_conditional_11;
        }
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_4 = sysml_conditional_9;
    bool sysml_value_5 = 0;
    if (sysml_state_activeColor == 1) {
        sysml_conditional_12 = sysml_state_redIntensity >= 96.0;
    }
    else {
        if (sysml_state_activeColor == 2) {
            sysml_conditional_13 = sysml_state_greenIntensity >= 96.0;
        }
        else {
            if (sysml_state_activeColor == 3) {
                sysml_conditional_14 = sysml_state_blueIntensity >= 96.0;
            }
            else {
                sysml_conditional_14 = false;
            }
            sysml_conditional_13 = sysml_conditional_14;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_5 = sysml_conditional_12;
    bool sysml_value_6 = 0;
    if (sysml_state_activeColor == 1) {
        sysml_conditional_15 = sysml_state_redIntensity >= 128.0;
    }
    else {
        if (sysml_state_activeColor == 2) {
            sysml_conditional_16 = sysml_state_greenIntensity >= 128.0;
        }
        else {
            if (sysml_state_activeColor == 3) {
                sysml_conditional_17 = sysml_state_blueIntensity >= 128.0;
            }
            else {
                sysml_conditional_17 = false;
            }
            sysml_conditional_16 = sysml_conditional_17;
        }
        sysml_conditional_15 = sysml_conditional_16;
    }
    sysml_value_6 = sysml_conditional_15;
    bool sysml_value_7 = 0;
    if (sysml_state_activeColor == 1) {
        sysml_conditional_18 = sysml_state_redIntensity >= 160.0;
    }
    else {
        if (sysml_state_activeColor == 2) {
            sysml_conditional_19 = sysml_state_greenIntensity >= 160.0;
        }
        else {
            if (sysml_state_activeColor == 3) {
                sysml_conditional_20 = sysml_state_blueIntensity >= 160.0;
            }
            else {
                sysml_conditional_20 = false;
            }
            sysml_conditional_19 = sysml_conditional_20;
        }
        sysml_conditional_18 = sysml_conditional_19;
    }
    sysml_value_7 = sysml_conditional_18;
    bool sysml_value_8 = 0;
    if (sysml_state_activeColor == 1) {
        sysml_conditional_21 = sysml_state_redIntensity >= 192.0;
    }
    else {
        if (sysml_state_activeColor == 2) {
            sysml_conditional_22 = sysml_state_greenIntensity >= 192.0;
        }
        else {
            if (sysml_state_activeColor == 3) {
                sysml_conditional_23 = sysml_state_blueIntensity >= 192.0;
            }
            else {
                sysml_conditional_23 = false;
            }
            sysml_conditional_22 = sysml_conditional_23;
        }
        sysml_conditional_21 = sysml_conditional_22;
    }
    sysml_value_8 = sysml_conditional_21;
    bool sysml_value_9 = 0;
    if (sysml_state_activeColor == 1) {
        sysml_conditional_24 = sysml_state_redIntensity >= 224.0;
    }
    else {
        if (sysml_state_activeColor == 2) {
            sysml_conditional_25 = sysml_state_greenIntensity >= 224.0;
        }
        else {
            if (sysml_state_activeColor == 3) {
                sysml_conditional_26 = sysml_state_blueIntensity >= 224.0;
            }
            else {
                sysml_conditional_26 = false;
            }
            sysml_conditional_25 = sysml_conditional_26;
        }
        sysml_conditional_24 = sysml_conditional_25;
    }
    sysml_value_9 = sysml_conditional_24;
    bool sysml_value_10 = 0;
    if (sysml_state_activeColor == 1) {
        sysml_conditional_27 = sysml_state_redIntensity >= 256.0;
    }
    else {
        if (sysml_state_activeColor == 2) {
            sysml_conditional_28 = sysml_state_greenIntensity >= 256.0;
        }
        else {
            if (sysml_state_activeColor == 3) {
                sysml_conditional_29 = sysml_state_blueIntensity >= 256.0;
            }
            else {
                sysml_conditional_29 = false;
            }
            sysml_conditional_28 = sysml_conditional_29;
        }
        sysml_conditional_27 = sysml_conditional_28;
    }
    sysml_value_10 = sysml_conditional_27;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6_segment_0 = sysml_value_3;
    *io_6_segment_1 = sysml_value_4;
    *io_6_segment_2 = sysml_value_5;
    *io_6_segment_3 = sysml_value_6;
    *io_6_segment_4 = sysml_value_7;
    *io_6_segment_5 = sysml_value_8;
    *io_6_segment_6 = sysml_value_9;
    *io_6_segment_7 = sysml_value_10;
}