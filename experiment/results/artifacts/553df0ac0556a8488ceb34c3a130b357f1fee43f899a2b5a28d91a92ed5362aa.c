#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_ledPhase = false;
/* Global Variable define of  */
int sysml_state_blinkAccumMs = 0;
/* Global Variable define of  */
int sysml_state_digit = 9;
/* Global Variable define of  */
int sysml_state_countAccumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledPhase = false;
    sysml_state_blinkAccumMs = 0;
    sysml_state_digit = 9;
    sysml_state_countAccumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_3, bool* io_0, bool* io_1, int* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    if (true) {
        sysml_state_blinkAccumMs = (sysml_state_blinkAccumMs + io_3);
        if ((sysml_state_blinkAccumMs >= 2000)) {
            sysml_state_blinkAccumMs = (sysml_state_blinkAccumMs - 2000);
            sysml_state_ledPhase = (!sysml_state_ledPhase);
        }
        sysml_state_countAccumMs = (sysml_state_countAccumMs + io_3);
        if ((sysml_state_countAccumMs >= 4000)) {
            sysml_state_countAccumMs = (sysml_state_countAccumMs - 4000);
            if (sysml_state_digit == 0) {
                sysml_conditional_0 = 9;
            }
            else {
                sysml_conditional_0 = sysml_state_digit - 1;
            }
            sysml_state_digit = sysml_conditional_0;
            if ((sysml_state_digit == 0)) {
                sysml_state_digit = 9;
            }
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_ledPhase;
    bool sysml_value_1 = 0;
    if (sysml_state_ledPhase) {
        sysml_conditional_1 = false;
    }
    else {
        sysml_conditional_1 = true;
    }
    sysml_value_1 = sysml_conditional_1;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_digit;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}