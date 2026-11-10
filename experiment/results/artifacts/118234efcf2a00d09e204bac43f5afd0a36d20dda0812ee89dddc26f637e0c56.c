#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_sampleCount = 0;
/* Global Variable define of  */
bool sysml_state_buzzerOn = false;
/* Global Variable define of  */
bool sysml_state_beepPending = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_sampleCount = 0;
    sysml_state_buzzerOn = false;
    sysml_state_beepPending = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_1, double io_2, bool* io_3, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, int* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    int sysml_conditional_13 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_sampleCount = (sysml_state_sampleCount + 1);
        if ((sysml_state_sampleCount - 4) == 0) {
            sysml_conditional_1 = true;
        }
        else {
            sysml_conditional_1 = false;
        }
        sysml_state_beepPending = sysml_conditional_1;
        sysml_lazy_2 = sysml_state_buzzerOn;
        if (!sysml_lazy_2) {
            if ((sysml_state_sampleCount - 4) == 0) {
                sysml_conditional_3 = true;
            }
            else {
                sysml_conditional_3 = false;
            }
            sysml_lazy_2 = sysml_conditional_3;
        }
        sysml_state_buzzerOn = sysml_lazy_2;
    }
    sysml_state_previousPressed = io_0;
    if (sysml_state_beepPending) {
        sysml_state_buzzerOn = false;
        sysml_state_beepPending = false;
    }
    bool sysml_value_0 = 0;
    if (io_0) {
        sysml_conditional_4 = sysml_state_buzzerOn;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_0 = sysml_conditional_4;
    bool sysml_value_1 = 0;
    sysml_lazy_5 = io_0;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_5) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_1 = sysml_conditional_6;
    double sysml_value_2 = 0;
    sysml_lazy_7 = io_0;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_7) {
        sysml_conditional_8 = io_1;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_2 = sysml_conditional_8;
    double sysml_value_3 = 0;
    sysml_lazy_9 = io_0;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_9) {
        if (io_2 >= 0.0) {
            sysml_conditional_11 = io_2 * 0.1;
        }
        else {
            sysml_conditional_11 = 0.0;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    else {
        sysml_conditional_10 = 0.0;
    }
    sysml_value_3 = sysml_conditional_10;
    int sysml_value_4 = 0;
    sysml_lazy_12 = io_0;
    if (sysml_lazy_12) {
        sysml_lazy_12 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_12) {
        sysml_conditional_13 = sysml_state_sampleCount;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_4 = sysml_conditional_13;
    *io_3 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
}