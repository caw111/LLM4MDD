#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_running = false;
/* Global Variable define of  */
bool sysml_state_previousStartPausePressed = false;
/* Global Variable define of  */
bool sysml_state_previousResetPressed = false;
/* Global Variable define of  */
int sysml_state_seconds = 0;
/* Global Variable define of  */
int sysml_state_beepRemainingSeconds = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_running = false;
    sysml_state_previousStartPausePressed = false;
    sysml_state_previousResetPressed = false;
    sysml_state_seconds = 0;
    sysml_state_beepRemainingSeconds = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int* io_2, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    if (io_1) {
        sysml_lazy_0 = io_1;
        if (sysml_lazy_0) {
            sysml_lazy_0 = !sysml_state_previousResetPressed;
        }
        if (sysml_lazy_0) {
            sysml_state_seconds = 0;
            sysml_state_beepRemainingSeconds = 0;
            sysml_state_running = false;
        }
    }
    if (io_0) {
        sysml_lazy_1 = io_0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = !sysml_state_previousStartPausePressed;
        }
        if (sysml_lazy_1) {
            if (sysml_state_running) {
                sysml_conditional_2 = false;
            }
            else {
                sysml_conditional_2 = true;
            }
            if (sysml_conditional_2) {
                sysml_conditional_3 = false;
            }
            else {
                sysml_conditional_3 = true;
            }
            sysml_state_running = sysml_conditional_3;
        }
    }
    sysml_lazy_4 = sysml_state_running;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_beepRemainingSeconds == 0;
    }
    if (sysml_lazy_4) {
        sysml_state_seconds = (sysml_state_seconds + 1);
        if ((sysml_state_seconds == 60)) {
            sysml_state_beepRemainingSeconds = 1;
        }
    }
    else {
        sysml_lazy_5 = sysml_state_running;
        if (sysml_lazy_5) {
            sysml_lazy_5 = sysml_state_beepRemainingSeconds > 0;
        }
        if (sysml_lazy_5) {
            sysml_state_beepRemainingSeconds = (sysml_state_beepRemainingSeconds - 1);
            sysml_state_seconds = (sysml_state_seconds + 1);
            if ((sysml_state_seconds == 60)) {
                sysml_state_beepRemainingSeconds = 1;
            }
        }
    }
    sysml_state_previousStartPausePressed = io_0;
    sysml_state_previousResetPressed = io_1;
    int sysml_value_0 = 0;
    sysml_lazy_6 = sysml_state_running;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_beepRemainingSeconds > 0;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = 1000;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_0 = sysml_conditional_7;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_seconds;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
}