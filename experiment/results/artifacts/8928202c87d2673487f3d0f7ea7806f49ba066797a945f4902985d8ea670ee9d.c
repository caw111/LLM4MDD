#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_running = false;
/* Global Variable define of  */
int sysml_state_remainingSeconds = 0;
/* Global Variable define of  */
bool sysml_state_beeping = false;
/* Global Variable define of  */
bool sysml_state_prevAddPressed = false;
/* Global Variable define of  */
bool sysml_state_prevStartPressed = false;
/* Global Variable define of  */
int sysml_state_beepRemainingSteps = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_running = false;
    sysml_state_remainingSeconds = 0;
    sysml_state_beeping = false;
    sysml_state_prevAddPressed = false;
    sysml_state_prevStartPressed = false;
    sysml_state_beepRemainingSteps = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int* io_2, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevAddPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_remainingSeconds = (sysml_state_remainingSeconds + 10);
    }
    sysml_lazy_1 = io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevStartPressed;
    }
    if (sysml_lazy_1) {
        sysml_state_running = true;
    }
    sysml_lazy_2 = sysml_state_running;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_beeping;
    }
    if (sysml_lazy_2) {
        sysml_lazy_3 = sysml_state_remainingSeconds == 0;
        if (sysml_lazy_3) {
            sysml_lazy_3 = true;
        }
        if (sysml_lazy_3) {
            sysml_conditional_4 = 0;
        }
        else {
            if (sysml_state_remainingSeconds > 0) {
                sysml_conditional_5 = sysml_state_remainingSeconds - 1;
            }
            else {
                sysml_conditional_5 = sysml_state_remainingSeconds;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_state_remainingSeconds = sysml_conditional_4;
        if ((sysml_state_remainingSeconds <= 0)) {
            sysml_state_beeping = true;
            sysml_state_beepRemainingSteps = 0;
            sysml_state_running = false;
        }
    }
    sysml_lazy_6 = sysml_state_beeping;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_beepRemainingSteps > 0;
    }
    if (sysml_lazy_6) {
        sysml_state_beepRemainingSteps = (sysml_state_beepRemainingSteps - 1);
        if ((sysml_state_beepRemainingSteps <= 1)) {
            sysml_state_beeping = false;
        }
    }
    sysml_state_prevAddPressed = io_0;
    sysml_state_prevStartPressed = io_1;
    int sysml_value_0 = 0;
    if (sysml_state_beeping) {
        sysml_conditional_7 = 31;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_0 = sysml_conditional_7;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_remainingSeconds;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
}