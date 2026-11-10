#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_releasedDelayRemainingMs = 0;
/* Global Variable define of  */
bool sysml_state_toneOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_releasedDelayRemainingMs = 0;
    sysml_state_toneOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int io_2, int* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_releasedDelayRemainingMs = 0;
        sysml_state_toneOn = true;
    }
    sysml_lazy_1 = !io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_previousPressed;
    }
    if (sysml_lazy_1) {
        sysml_state_releasedDelayRemainingMs = 1000;
    }
    sysml_lazy_2 = !io_0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_releasedDelayRemainingMs > 0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = sysml_state_releasedDelayRemainingMs - io_2;
    }
    else {
        sysml_conditional_3 = sysml_state_releasedDelayRemainingMs;
    }
    sysml_state_releasedDelayRemainingMs = sysml_conditional_3;
    sysml_lazy_4 = !io_0;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_releasedDelayRemainingMs <= 0;
    }
    if (sysml_lazy_4) {
        sysml_state_toneOn = false;
        sysml_state_releasedDelayRemainingMs = 0;
    }
    sysml_state_previousPressed = io_0;
    if (io_0) {
        sysml_state_toneOn = true;
    }
    int sysml_value_0 = 0;
    if (sysml_state_toneOn) {
        sysml_conditional_5 = 440;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_0 = sysml_conditional_5;
    *io_1 = sysml_value_0;
}