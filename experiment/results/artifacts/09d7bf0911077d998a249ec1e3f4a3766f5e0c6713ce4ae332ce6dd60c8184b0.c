#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_pressedStableMs = 0;
/* Global Variable define of  */
bool sysml_state_activeSourcePot1 = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_pressedStableMs = 0;
    sysml_state_activeSourcePot1 = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, int io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
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
    if (io_0) {
        sysml_state_pressedStableMs = (sysml_state_pressedStableMs + io_3);
    }
    else {
        sysml_state_pressedStableMs = 0;
    }
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_pressedStableMs >= 150;
    }
    if (sysml_lazy_1) {
        sysml_state_activeSourcePot1 = (!sysml_state_activeSourcePot1);
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    sysml_lazy_2 = sysml_state_activeSourcePot1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_1 > 0.0;
    }
    if (sysml_lazy_2) {
        sysml_lazy_4 = io_1 >= 102.3;
        if (sysml_lazy_4) {
            sysml_lazy_4 = true;
        }
        sysml_conditional_3 = sysml_lazy_4;
    }
    else {
        sysml_lazy_5 = !sysml_state_activeSourcePot1;
        if (sysml_lazy_5) {
            sysml_lazy_5 = io_2 > 0.0;
        }
        if (sysml_lazy_5) {
            sysml_lazy_7 = io_2 >= 102.3;
            if (sysml_lazy_7) {
                sysml_lazy_7 = true;
            }
            sysml_conditional_6 = sysml_lazy_7;
        }
        else {
            sysml_lazy_8 = sysml_state_activeSourcePot1;
            if (sysml_lazy_8) {
                sysml_lazy_8 = io_1 == 0.0;
            }
            if (sysml_lazy_8) {
                sysml_conditional_9 = false;
            }
            else {
                sysml_lazy_10 = !sysml_state_activeSourcePot1;
                if (sysml_lazy_10) {
                    sysml_lazy_10 = io_2 == 0.0;
                }
                if (sysml_lazy_10) {
                    sysml_conditional_11 = false;
                }
                else {
                    if (sysml_state_activeSourcePot1) {
                        sysml_conditional_12 = io_1 >= 102.3;
                    }
                    else {
                        sysml_conditional_12 = io_2 >= 102.3;
                    }
                    sysml_conditional_11 = sysml_conditional_12;
                }
                sysml_conditional_9 = sysml_conditional_11;
            }
            sysml_conditional_6 = sysml_conditional_9;
        }
        sysml_conditional_3 = sysml_conditional_6;
    }
    sysml_value_0 = sysml_conditional_3;
    bool sysml_value_1 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_13 = io_1 >= 204.6;
    }
    else {
        sysml_conditional_13 = io_2 >= 204.6;
    }
    sysml_value_1 = sysml_conditional_13;
    bool sysml_value_2 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_14 = io_1 >= 307.0;
    }
    else {
        sysml_conditional_14 = io_2 >= 307.0;
    }
    sysml_value_2 = sysml_conditional_14;
    bool sysml_value_3 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_15 = io_1 >= 409.2;
    }
    else {
        sysml_conditional_15 = io_2 >= 409.2;
    }
    sysml_value_3 = sysml_conditional_15;
    bool sysml_value_4 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_16 = io_1 >= 511.5;
    }
    else {
        sysml_conditional_16 = io_2 >= 511.5;
    }
    sysml_value_4 = sysml_conditional_16;
    bool sysml_value_5 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_17 = io_1 >= 613.8;
    }
    else {
        sysml_conditional_17 = io_2 >= 613.8;
    }
    sysml_value_5 = sysml_conditional_17;
    bool sysml_value_6 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_18 = io_1 >= 716.1;
    }
    else {
        sysml_conditional_18 = io_2 >= 716.1;
    }
    sysml_value_6 = sysml_conditional_18;
    bool sysml_value_7 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_19 = io_1 >= 818.4;
    }
    else {
        sysml_conditional_19 = io_2 >= 818.4;
    }
    sysml_value_7 = sysml_conditional_19;
    bool sysml_value_8 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_20 = io_1 >= 920.7;
    }
    else {
        sysml_conditional_20 = io_2 >= 920.7;
    }
    sysml_value_8 = sysml_conditional_20;
    bool sysml_value_9 = 0;
    if (sysml_state_activeSourcePot1) {
        sysml_conditional_21 = io_1 >= 1023.0;
    }
    else {
        sysml_conditional_21 = io_2 >= 1023.0;
    }
    sysml_value_9 = sysml_conditional_21;
    *io_4_segment_0 = sysml_value_0;
    *io_4_segment_1 = sysml_value_1;
    *io_4_segment_2 = sysml_value_2;
    *io_4_segment_3 = sysml_value_3;
    *io_4_segment_4 = sysml_value_4;
    *io_4_segment_5 = sysml_value_5;
    *io_4_segment_6 = sysml_value_6;
    *io_4_segment_7 = sysml_value_7;
    *io_4_segment_8 = sysml_value_8;
    *io_4_segment_9 = sysml_value_9;
}