#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_readyElapsedMs = 0;
/* Global Variable define of  */
int sysml_state_gameElapsedMs = 0;
/* Global Variable define of  */
int sysml_state_roundIndex = 0;
/* Global Variable define of  */
int sysml_state_ledPhase = 0;
/* Global Variable define of  */
int sysml_state_score = 0;
/* Global Variable define of  */
bool sysml_state_previousStopPressed = false;
/* Global Variable define of  */
bool sysml_state_previousBackPressed = false;
/* Global Variable define of  */
bool sysml_state_previousForwardPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_readyElapsedMs = 0;
    sysml_state_gameElapsedMs = 0;
    sysml_state_roundIndex = 0;
    sysml_state_ledPhase = 0;
    sysml_state_score = 0;
    sysml_state_previousStopPressed = false;
    sysml_state_previousBackPressed = false;
    sysml_state_previousForwardPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, bool* io_0, bool* io_1, bool* io_2, int* io_6, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    if (true) {
        sysml_state_readyElapsedMs = sysml_state_readyElapsedMs;
        sysml_state_gameElapsedMs = sysml_state_gameElapsedMs;
        sysml_state_roundIndex = sysml_state_roundIndex;
        sysml_state_ledPhase = sysml_state_ledPhase;
        sysml_state_score = sysml_state_score;
    }
    else {
        sysml_state_readyElapsedMs = sysml_state_readyElapsedMs;
    }
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousStopPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_previousStopPressed = true;
    }
    if ((!io_3)) {
        sysml_state_previousStopPressed = false;
    }
    sysml_lazy_1 = io_4;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousBackPressed;
    }
    if (sysml_lazy_1) {
        sysml_state_previousBackPressed = true;
    }
    if ((!io_4)) {
        sysml_state_previousBackPressed = false;
    }
    sysml_lazy_2 = io_5;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousForwardPressed;
    }
    if (sysml_lazy_2) {
        sysml_state_previousForwardPressed = true;
    }
    if ((!io_5)) {
        sysml_state_previousForwardPressed = false;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = false;
    bool sysml_value_1 = 0;
    sysml_value_1 = false;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    int sysml_value_3 = 0;
    sysml_value_3 = 0;
    bool sysml_value_4 = 0;
    sysml_value_4 = false;
    int sysml_value_5 = 0;
    sysml_value_5 = 0;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_6 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
}