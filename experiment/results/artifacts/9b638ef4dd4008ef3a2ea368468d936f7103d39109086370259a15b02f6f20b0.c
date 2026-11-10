#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_currentChannel = 0;
/* Global Variable define of  */
bool sysml_state_initialized = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_currentChannel = 0;
    sysml_state_initialized = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_4, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, int* serial_0_value_2, int* serial_0_value_3, int* serial_0_value_4) {
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
    bool sysml_lazy_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    int sysml_conditional_19 = 0;
    int sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    int sysml_conditional_22 = 0;
    int sysml_conditional_23 = 0;
    if ((!sysml_state_initialized)) {
        sysml_state_currentChannel = 0;
        sysml_state_initialized = true;
    }
    if (sysml_state_initialized) {
        sysml_state_currentChannel = (sysml_state_currentChannel + 1);
    }
    bool sysml_value_0 = 0;
    sysml_lazy_0 = sysml_state_currentChannel == 1;
    if (!sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_currentChannel == 3;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_currentChannel == 5;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_currentChannel == 7;
    }
    sysml_value_0 = sysml_lazy_2;
    bool sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_currentChannel == 2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_currentChannel == 3;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_currentChannel == 6;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_currentChannel == 7;
    }
    sysml_value_1 = sysml_lazy_5;
    bool sysml_value_2 = 0;
    sysml_lazy_6 = sysml_state_currentChannel == 4;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_currentChannel == 5;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_currentChannel == 6;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_currentChannel == 7;
    }
    sysml_value_2 = sysml_lazy_8;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_initialized;
    int sysml_value_5 = 0;
    sysml_lazy_9 = sysml_state_initialized;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_currentChannel == 0;
    }
    if (sysml_lazy_9) {
        if (io_4 < ((double)((int)io_4))) {
            sysml_conditional_11 = 1;
        }
        else {
            sysml_conditional_11 = 0;
        }
        sysml_conditional_10 = ((int)io_4) - sysml_conditional_11;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_5 = sysml_conditional_10;
    int sysml_value_6 = 0;
    sysml_lazy_12 = sysml_state_initialized;
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_currentChannel == 1;
    }
    if (sysml_lazy_12) {
        if (io_4 < ((double)((int)io_4))) {
            sysml_conditional_14 = 1;
        }
        else {
            sysml_conditional_14 = 0;
        }
        sysml_conditional_13 = ((int)io_4) - sysml_conditional_14;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_6 = sysml_conditional_13;
    int sysml_value_7 = 0;
    sysml_lazy_15 = sysml_state_initialized;
    if (sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_currentChannel == 2;
    }
    if (sysml_lazy_15) {
        if (io_4 < ((double)((int)io_4))) {
            sysml_conditional_17 = 1;
        }
        else {
            sysml_conditional_17 = 0;
        }
        sysml_conditional_16 = ((int)io_4) - sysml_conditional_17;
    }
    else {
        sysml_conditional_16 = 0;
    }
    sysml_value_7 = sysml_conditional_16;
    int sysml_value_8 = 0;
    sysml_lazy_18 = sysml_state_initialized;
    if (sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_currentChannel == 3;
    }
    if (sysml_lazy_18) {
        if (io_4 < ((double)((int)io_4))) {
            sysml_conditional_20 = 1;
        }
        else {
            sysml_conditional_20 = 0;
        }
        sysml_conditional_19 = ((int)io_4) - sysml_conditional_20;
    }
    else {
        sysml_conditional_19 = 0;
    }
    sysml_value_8 = sysml_conditional_19;
    int sysml_value_9 = 0;
    sysml_lazy_21 = sysml_state_initialized;
    if (sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_currentChannel == 4;
    }
    if (sysml_lazy_21) {
        if (io_4 < ((double)((int)io_4))) {
            sysml_conditional_23 = 1;
        }
        else {
            sysml_conditional_23 = 0;
        }
        sysml_conditional_22 = ((int)io_4) - sysml_conditional_23;
    }
    else {
        sysml_conditional_22 = 0;
    }
    sysml_value_9 = sysml_conditional_22;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
    *serial_0_value_2 = sysml_value_7;
    *serial_0_value_3 = sysml_value_8;
    *serial_0_value_4 = sysml_value_9;
}