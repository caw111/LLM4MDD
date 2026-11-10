#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_elapsedTotalMs = 0;
/* Global Variable define of  */
int sysml_state_rainbowIndex = 0;
/* Global Variable define of  */
bool sysml_state_ledOn = false;
/* Global Variable define of  */
bool sysml_state_servo1At180 = false;
/* Global Variable define of  */
bool sysml_state_servo2At0 = false;
/* Global Variable define of  */
bool sysml_state_servo3At135 = false;
/* Global Variable define of  */
bool sysml_state_phaseStepDone = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedTotalMs = 0;
    sysml_state_rainbowIndex = 0;
    sysml_state_ledOn = false;
    sysml_state_servo1At180 = false;
    sysml_state_servo2At0 = false;
    sysml_state_servo3At135 = false;
    sysml_state_phaseStepDone = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9, double* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    double sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    double sysml_conditional_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    double sysml_conditional_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    double sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    double sysml_conditional_26 = 0;
    double sysml_conditional_27 = 0;
    double sysml_conditional_28 = 0;
    double sysml_conditional_29 = 0;
    sysml_state_elapsedTotalMs = (sysml_state_elapsedTotalMs + io_0);
    sysml_state_phaseStepDone = (io_0 > (-1));
    sysml_lazy_0 = sysml_state_phaseStepDone;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_elapsedTotalMs >= 2000;
    }
    if (sysml_lazy_0) {
        sysml_state_elapsedTotalMs = (sysml_state_elapsedTotalMs - 2000);
        if (sysml_state_rainbowIndex >= 6) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_rainbowIndex + 1;
        }
        sysml_state_rainbowIndex = sysml_conditional_1;
        sysml_state_ledOn = (!sysml_state_ledOn);
        sysml_state_servo1At180 = (!sysml_state_servo1At180);
        sysml_state_servo2At0 = (!sysml_state_servo2At0);
        sysml_state_servo3At135 = (!sysml_state_servo3At135);
    }
    double sysml_value_0 = 0;
    if (sysml_state_ledOn) {
        sysml_conditional_2 = 1.0;
    }
    else {
        sysml_conditional_2 = 0.0;
    }
    sysml_value_0 = sysml_conditional_2;
    double sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_rainbowIndex == 0;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_rainbowIndex == 3;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_rainbowIndex == 5;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_rainbowIndex == 6;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = 1.0;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_1 = sysml_conditional_6;
    double sysml_value_2 = 0;
    sysml_lazy_7 = sysml_state_rainbowIndex == 1;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_rainbowIndex == 3;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_rainbowIndex == 4;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_rainbowIndex == 6;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 1.0;
    }
    else {
        sysml_conditional_10 = 0.0;
    }
    sysml_value_2 = sysml_conditional_10;
    double sysml_value_3 = 0;
    sysml_lazy_11 = sysml_state_rainbowIndex == 2;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = sysml_state_rainbowIndex == 4;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_rainbowIndex == 5;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (!sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_rainbowIndex == 6;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = 1.0;
    }
    else {
        sysml_conditional_14 = 0.0;
    }
    sysml_value_3 = sysml_conditional_14;
    double sysml_value_4 = 0;
    sysml_lazy_15 = sysml_state_rainbowIndex == 0;
    if (!sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_rainbowIndex == 3;
    }
    sysml_lazy_16 = sysml_lazy_15;
    if (!sysml_lazy_16) {
        sysml_lazy_16 = sysml_state_rainbowIndex == 5;
    }
    sysml_lazy_17 = sysml_lazy_16;
    if (!sysml_lazy_17) {
        sysml_lazy_17 = sysml_state_rainbowIndex == 6;
    }
    if (sysml_lazy_17) {
        sysml_conditional_18 = 1.0;
    }
    else {
        sysml_conditional_18 = 0.0;
    }
    sysml_value_4 = sysml_conditional_18;
    double sysml_value_5 = 0;
    sysml_lazy_19 = sysml_state_rainbowIndex == 1;
    if (!sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_rainbowIndex == 3;
    }
    sysml_lazy_20 = sysml_lazy_19;
    if (!sysml_lazy_20) {
        sysml_lazy_20 = sysml_state_rainbowIndex == 4;
    }
    sysml_lazy_21 = sysml_lazy_20;
    if (!sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_rainbowIndex == 6;
    }
    if (sysml_lazy_21) {
        sysml_conditional_22 = 1.0;
    }
    else {
        sysml_conditional_22 = 0.0;
    }
    sysml_value_5 = sysml_conditional_22;
    double sysml_value_6 = 0;
    sysml_lazy_23 = sysml_state_rainbowIndex == 2;
    if (!sysml_lazy_23) {
        sysml_lazy_23 = sysml_state_rainbowIndex == 4;
    }
    sysml_lazy_24 = sysml_lazy_23;
    if (!sysml_lazy_24) {
        sysml_lazy_24 = sysml_state_rainbowIndex == 5;
    }
    sysml_lazy_25 = sysml_lazy_24;
    if (!sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_rainbowIndex == 6;
    }
    if (sysml_lazy_25) {
        sysml_conditional_26 = 1.0;
    }
    else {
        sysml_conditional_26 = 0.0;
    }
    sysml_value_6 = sysml_conditional_26;
    double sysml_value_7 = 0;
    if (sysml_state_servo1At180) {
        sysml_conditional_27 = 180.0;
    }
    else {
        sysml_conditional_27 = 0.0;
    }
    sysml_value_7 = sysml_conditional_27;
    double sysml_value_8 = 0;
    if (sysml_state_servo2At0) {
        sysml_conditional_28 = 0.0;
    }
    else {
        sysml_conditional_28 = 180.0;
    }
    sysml_value_8 = sysml_conditional_28;
    double sysml_value_9 = 0;
    if (sysml_state_servo3At135) {
        sysml_conditional_29 = 135.0;
    }
    else {
        sysml_conditional_29 = 90.0;
    }
    sysml_value_9 = sysml_conditional_29;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
    *io_9 = sysml_value_8;
    *io_10 = sysml_value_9;
}