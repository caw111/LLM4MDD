#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_armed = false;
/* Global Variable define of  */
bool sysml_state_nightMode = false;
/* Global Variable define of  */
int sysml_state_eventCount = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_alarmRemainingMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_armed = false;
    sysml_state_nightMode = false;
    sysml_state_eventCount = 0;
    sysml_state_previousPressed = false;
    sysml_state_alarmRemainingMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, int io_6, bool io_6_valid, int* io_3, bool* io_4, int* io_5) {
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
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    int sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_conditional_18 = 0;
    sysml_lazy_0 = io_2 < 500.0;
    if (!sysml_lazy_0) {
        sysml_lazy_1 = io_2 >= 500.0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = false;
        }
        sysml_lazy_0 = sysml_lazy_1;
    }
    if (sysml_lazy_0) {
        sysml_state_nightMode = true;
    }
    sysml_lazy_2 = io_2 < 500.0;
    if (!sysml_lazy_2) {
        sysml_lazy_3 = io_2 >= 500.0;
        if (sysml_lazy_3) {
            sysml_lazy_3 = false;
        }
        sysml_lazy_2 = sysml_lazy_3;
    }
    if (!sysml_lazy_2) {
        sysml_state_nightMode = false;
    }
    sysml_lazy_4 = io_6_valid;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_6 == 1;
    }
    if (sysml_lazy_4) {
        sysml_state_armed = true;
    }
    sysml_lazy_5 = io_6_valid;
    if (sysml_lazy_5) {
        sysml_lazy_5 = io_6 == 2;
    }
    if (sysml_lazy_5) {
        sysml_state_armed = false;
    }
    sysml_lazy_6 = io_6_valid;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_6 == 3;
    }
    if (sysml_lazy_6) {
        sysml_state_armed = false;
        sysml_state_eventCount = 0;
        sysml_state_alarmRemainingMs = 0;
    }
    sysml_lazy_7 = io_6_valid;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_6 == 4;
    }
    if (sysml_lazy_7) {
    }
    sysml_lazy_8 = sysml_state_armed;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_nightMode;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = io_1;
    }
    if (sysml_lazy_9) {
    }
    sysml_lazy_10 = sysml_state_armed;
    if (sysml_lazy_10) {
        sysml_lazy_10 = !sysml_state_nightMode;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_1;
    }
    if (sysml_lazy_11) {
    }
    sysml_lazy_12 = sysml_state_armed;
    if (sysml_lazy_12) {
        sysml_lazy_13 = io_1;
        if (sysml_lazy_13) {
            sysml_lazy_14 = io_0;
            if (sysml_lazy_14) {
                sysml_lazy_14 = !sysml_state_previousPressed;
            }
            sysml_lazy_13 = sysml_lazy_14;
        }
        sysml_lazy_12 = sysml_lazy_13;
    }
    if (sysml_lazy_12) {
        sysml_state_eventCount = (sysml_state_eventCount + 1);
        sysml_state_alarmRemainingMs = 5000;
    }
    if ((sysml_state_alarmRemainingMs > 0)) {
        sysml_state_alarmRemainingMs = (sysml_state_alarmRemainingMs - 5);
    }
    sysml_state_previousPressed = io_0;
    int sysml_value_0 = 0;
    sysml_lazy_15 = sysml_state_armed;
    if (sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_alarmRemainingMs > 0;
    }
    if (sysml_lazy_15) {
        sysml_conditional_16 = 2000;
    }
    else {
        sysml_conditional_16 = 0;
    }
    sysml_value_0 = sysml_conditional_16;
    bool sysml_value_1 = 0;
    sysml_lazy_17 = sysml_state_armed;
    if (sysml_lazy_17) {
        sysml_lazy_17 = sysml_state_alarmRemainingMs > 0;
    }
    if (sysml_lazy_17) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_1 = sysml_conditional_18;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_eventCount;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
}