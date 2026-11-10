#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_countdownSeconds = 0;
/* Global Variable define of  */
bool sysml_state_started = false;
/* Global Variable define of  */
bool sysml_state_prevAddPressed = false;
/* Global Variable define of  */
bool sysml_state_prevStartPressed = false;
/* Global Variable define of  */
bool sysml_state_beepActive = false;
/* Global Variable define of  */
int sysml_state_beepCounterMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_countdownSeconds = 0;
    sysml_state_started = false;
    sysml_state_prevAddPressed = false;
    sysml_state_prevStartPressed = false;
    sysml_state_beepActive = false;
    sysml_state_beepCounterMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int* io_2, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevAddPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_started) {
            sysml_conditional_1 = sysml_state_countdownSeconds + 10;
        }
        else {
            sysml_conditional_1 = sysml_state_countdownSeconds;
        }
        sysml_state_countdownSeconds = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevStartPressed;
    }
    if (sysml_lazy_2) {
        sysml_state_started = true;
        if (sysml_state_countdownSeconds == 0) {
            sysml_conditional_3 = 10;
        }
        else {
            sysml_conditional_3 = sysml_state_countdownSeconds;
        }
        sysml_state_countdownSeconds = sysml_conditional_3;
        sysml_state_beepActive = false;
        sysml_state_beepCounterMs = 0;
    }
    if (sysml_state_started) {
        if ((sysml_state_countdownSeconds > 0)) {
            sysml_state_countdownSeconds = (sysml_state_countdownSeconds - 1);
        }
        sysml_lazy_4 = sysml_state_countdownSeconds == 0;
        if (sysml_lazy_4) {
            sysml_lazy_4 = !sysml_state_beepActive;
        }
        if (sysml_lazy_4) {
            sysml_state_beepActive = true;
            sysml_state_beepCounterMs = 2000;
        }
    }
    if (sysml_state_beepActive) {
        sysml_state_beepCounterMs = (sysml_state_beepCounterMs - 5);
        if ((sysml_state_beepCounterMs <= 0)) {
            sysml_state_beepActive = false;
            sysml_state_beepCounterMs = 0;
        }
    }
    sysml_state_prevAddPressed = io_0;
    sysml_state_prevStartPressed = io_1;
    int sysml_value_0 = 0;
    if (sysml_state_beepActive) {
        sysml_conditional_5 = 1000;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_0 = sysml_conditional_5;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_countdownSeconds;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
}