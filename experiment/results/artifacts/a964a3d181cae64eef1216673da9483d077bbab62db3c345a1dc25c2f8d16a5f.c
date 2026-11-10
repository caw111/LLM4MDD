#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_running = false;
/* Global Variable define of  */
bool sysml_state_paused = false;
/* Global Variable define of  */
int sysml_state_seconds = 0;
/* Global Variable define of  */
bool sysml_state_previousStartPausePressed = false;
/* Global Variable define of  */
bool sysml_state_previousResetPressed = false;
/* Global Variable define of  */
bool sysml_state_buzzerOn = false;
/* Global Variable define of  */
int sysml_state_buzzerRemainingMs = 0;
/* Global Variable define of  */
int sysml_state_toneHz = 0;
/* Global Variable define of  */
int sysml_state_displayValue = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_running = false;
    sysml_state_paused = false;
    sysml_state_seconds = 0;
    sysml_state_previousStartPausePressed = false;
    sysml_state_previousResetPressed = false;
    sysml_state_buzzerOn = false;
    sysml_state_buzzerRemainingMs = 0;
    sysml_state_toneHz = 0;
    sysml_state_displayValue = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int* io_2, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousStartPausePressed;
    }
    if (sysml_lazy_0) {
        sysml_state_paused = (!sysml_state_paused);
    }
    sysml_lazy_1 = io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousResetPressed;
    }
    if (sysml_lazy_1) {
        sysml_state_running = false;
        sysml_state_paused = false;
        sysml_state_seconds = 0;
        sysml_state_buzzerOn = false;
        sysml_state_buzzerRemainingMs = 0;
        sysml_state_toneHz = 0;
        sysml_state_displayValue = 0;
    }
    if ((sysml_state_buzzerRemainingMs <= 0)) {
        sysml_state_buzzerOn = false;
        sysml_state_toneHz = 0;
    }
    sysml_lazy_2 = !sysml_state_paused;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_buzzerOn;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_buzzerRemainingMs > 0;
    }
    if (sysml_lazy_3) {
        sysml_state_buzzerRemainingMs = sysml_state_buzzerRemainingMs;
        sysml_state_toneHz = sysml_state_toneHz;
    }
    sysml_lazy_4 = sysml_state_buzzerOn;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_buzzerRemainingMs <= 0;
    }
    if (sysml_lazy_4) {
        sysml_state_buzzerOn = false;
        sysml_state_toneHz = 0;
    }
    sysml_lazy_5 = sysml_state_seconds >= 60;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_buzzerOn;
    }
    if (sysml_lazy_5) {
        sysml_state_buzzerOn = true;
        sysml_state_buzzerRemainingMs = 1000;
        sysml_state_toneHz = 1000;
    }
    if ((!sysml_state_paused)) {
        sysml_state_seconds = sysml_state_seconds;
        sysml_state_displayValue = sysml_state_seconds;
    }
    else {
        sysml_state_displayValue = sysml_state_seconds;
    }
    sysml_state_previousStartPausePressed = io_0;
    sysml_state_previousResetPressed = io_1;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_toneHz;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_displayValue;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
}